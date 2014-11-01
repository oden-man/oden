#include "../Pch.h"

#include <oden/gammo/ui/widget/base.h>

namespace oden { namespace gammo
{
//##################################################################
//   widget_object
//
window_handle widget_object::get_window_handle()const
{ 
	return this->to_window().handle(); 
}
void widget_object::set_window_handle( window_handle wh ) 
{ 
	this->to_window().attach( wh ); 
}

// ������
bool widget_object::init_widget()
{
	// �R���g���[�����e�Ȃǂ�������
	if( !this->initialize() )
		return false;

	// �ŏ��̃��C�A�E�g����
	if( this->has_parent_widget() )
	{
		move_widget_context dummy = {};
		this->move_widget(dummy);
	}

	return true;
}

// �����܂Ƃ�
void widget_object::precreate()
{
	// �E�B���h�E�̑����E�n���h���Ȃǂ�ݒ�
	describe();
}
bool widget_object::docreate()
{
	// ���ۂɍ쐬
	if( !create_control() )
		return false;

	// ����������
	if( !init_widget() )
		return false;

	// �A�b�v�f�[�g
	if( !flags_[widget_no_update_after_init] )
		to_window().update();

	flags_.set( widget_ready );

	return true;
}

// start
bool widget_object::start()
{
	// �쐬�O����
	this->precreate();

	widget_iterator itr = this->begin(), end = this->end();
	for( ; itr != end; ++itr )
		(*itr)->precreate();

	// �쐬����
	if( !this->docreate() )
		return false;

	itr = this->begin();
	for( ; itr != end; ++itr )
	{
		if( !(*itr)->docreate() )
			return false;
	}

	return true;
}

bool widget_object::destroy()
{
	if( get_window_handle().is_valid() && to_window().destroy_handle() ) // WM_DESTROY
	{
		detach_event_loop();
		to_window().detach();
	}

	return false;
}

//##################################################################
//   widget_base
//
void widget_base::handle_event( window_event& ev )
{
	// ���C�A�E�g�E�C�x���g��O����
	ev.handle_addref();
	this->handle_layout_event( ev );

	// �n���h�����Ăяo��
	this->my_handle_event(ev);

	// �ŏI�v���V�[�W�����Ă�ŏ���
	if( !ev.is_handled() ) {
		this->final_handle_event( ev );
	}
}

void widget_base::my_handle_event( window_event& ev )
{
	this->invoke_all_slot(ev);
}

void widget_base::final_handle_event( window_event& ev )
{
	widget_hub::call_default_window_proc( ev );
}

bool widget_base::attach_event_loop()
{
	if( widget_hub* phub = widget_hub::this_thread_hub() )
		return phub->add_widget( this );
		
	return false;
}

bool widget_base::detach_event_loop()
{
	if( widget_hub* phub = widget_hub::this_thread_hub() )
	{
		phub->remove_event_filter( this );
		phub->remove_widget( this->get_window_handle().value() );
		return true;
	}
	
	return false;
}

bool widget_base::move_widget( move_widget_context& c )
{
	// �e��dwp������΂���𗘗p���A�Ȃ���Ύ��͂œ�����
	if( c.dwp.is_started() )
		return c.dwp.add( get_window_handle(), location_, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER );	
	
	return to_window().set_position( location_, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER );
}

void widget_base::get_best_size( size& out )const
{
	if( best_size_cache_.is_null() )
	{
		this->get_best_size_now(best_size_cache_);
	}
	
	out = best_size_cache_;
}

void widget_base::fit_widget()
{
	size dim;
	this->get_best_size(dim);
	location_.set_dimension(dim);
}

void widget_base::best_size_changed()
{
	best_size_cache_ = size();

	if( widget_object* pa = this->get_parent_widget() )
		pa->best_size_changed();
}

//##################################################################
//   widget_hub
//
// ���b�Z�[�W�𑗂�A�v���V�[�W�����N��
bool widget_hub::proc_entry( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& ret )
{
	++proc_call_depth_;
	
	window_event wev;
	wev.set( hWnd, msg, wParam, lParam, &ret, proc_call_depth_ );

	set_current_widget( hWnd );

	if( cur_widget_ptr_ )
		cur_widget_ptr_->handle_event( wev );
	
	--proc_call_depth_;

	return wev.is_handled();
}

bool widget_hub::preprocess_entry( raw_window_message& out )
{
	preprocessing_event ppev;
	ppev.set( out );
	
	int pp_max = event_filter_.size();
	for( int i=0; i<pp_max; ++i ) {
		if( !event_filter_[i]->preprocess_event( ppev ) ) 
			break;
	}
	
	return ppev.is_processed();
}

void widget_hub::set_current_widget( window_handle hWnd )
{
	if( cur_widget_ptr_ ) 
	{
		// �ΏۃE�B�W�F�b�g�͕ύX���Ă��Ȃ����H
		if( cur_widget_ptr_->get_window_handle() == hWnd )
		{
			return;
		}
		// �ΏۃE�B�W�F�b�g�Ƀn���h�����ݒ肳��ĂȂ���΁ACreateWindow�Ăяo�����Ƃ݂Ȃ�
		else if( cur_widget_ptr_->get_window_handle() == boost::none )
		{	
			// �n���h���������Ă��
			cur_widget_ptr_->set_window_handle( hWnd );
			
			// �C�x���g���[�v�ɒǉ�
			this->add_widget( cur_widget_ptr_ );

			return;
		}
	}
	
	//  �E�B�W�F�b�g��T���Đݒ肷��
	widget_map::iterator itr = widget_map_.find( hWnd.value() );
	if( itr != widget_map_.end() ) {
		cur_widget_ptr_ = itr->second;
	}
	else { /* �݂���Ȃ������ꍇ��... null �ɐݒ� */
		cur_widget_ptr_ = 0;
	}
	
}

// �E�B�W�F�b�g���}�b�v�ɒǉ�����
bool widget_hub::add_widget( widget_object* pwid )
{
	return widget_map_.insert( std::make_pair( pwid->get_window_handle().value(), pwid ) ).second;
}
bool widget_hub::remove_widget( window_handle hWnd )
{
	if( cur_widget_ptr_ )
	{
		if( cur_widget_ptr_->get_window_handle() == hWnd )
			cur_widget_ptr_ = 0;
	}

	// �폜
	widget_map_.erase( hWnd.value() );

	return true;
}

// �E�B�W�F�b�g���}�b�v�ɒǉ�����
void widget_hub::add_event_filter( widget_object* pwid )
{
	event_filter_.push_back( pwid );
}
bool widget_hub::remove_event_filter( widget_object* pwid )
{
	widget_vector::iterator itr = std::find( event_filter_.begin(), event_filter_.end(), pwid );
	if( itr != event_filter_.end() ) 
	{
		event_filter_.erase(itr);
		return true;
	}
	return false;
}

//
int widget_hub::proc_entry_depth()const
{
	return proc_call_depth_;
}

// �쐬���E�B���h�E
void widget_hub::set_creating_widget( widget_object& wid )
{
	cur_widget_ptr_ = &wid;
}

// ���̃X���b�h�Ƀn�u���֘A�t����
bool widget_hub::create_this_thread_hub()
{
	if( 0!=this_thread_hub() )
		return true;

	widget_hub* pNew = new widget_hub();
	return tl_hub->set( reinterpret_cast<LONG_PTR>(pNew) );
}
// ���̃X���b�h�Ɋ֘A�t����ꂽ�n�u���擾����
widget_hub* widget_hub::this_thread_hub()
{
	return reinterpret_cast< widget_hub* >( tl_hub->value() );
}
bool widget_hub::release_this_thread_hub()
{
	if( widget_hub* p = this_thread_hub() )
	{
		delete p;
		tl_hub->set(0);
		return true;
	}
	return false;
}

// WNDPROC �G���g���[
LRESULT CALLBACK widget_hub::static_window_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( widget_hub* hub = this_thread_hub() )
	{
		LRESULT Ret = 0;
		hub->proc_entry( hWnd, msg, wParam, lParam, Ret );
		return Ret;
	}
	return 0;
}

// DLGPROC �G���g���[
INT_PTR CALLBACK widget_hub::static_dialog_proc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( widget_hub* hub = this_thread_hub() )
	{
		LRESULT Ret = 0;
		if( hub->proc_entry( hDlg, msg, wParam, lParam, Ret ) ) {
			return static_cast<INT_PTR>( SetDlgMsgResult( hDlg, msg, Ret ) );
		}
	}
	return FALSE;
}

// �f�t�H���g�E�B���h�E�v���V�[�W��
void widget_hub::call_default_window_proc( window_event& arg )
{
	arg.result() = ::DefWindowProc( arg.target().value(), arg.code(), arg.wparam(), arg.lparam() );
}

// �f�t�H���g�_�C�A���O�v���V�[�W��
void widget_hub::call_default_dialog_proc( window_event& arg )
{
	arg.result() = ::DefDlgProc( arg.target().value(), arg.code(), arg.wparam(), arg.lparam() );
}

// �X���b�h���[�J���ȃ��[�^
scoped<thread_local_stroage> widget_hub::tl_hub( allocate_tls() );


} /* end of namespace gammo */
} /* end of namespace oden */
