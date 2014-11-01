#include "../Pch.h"

#include <oden/gammo/ui/widget/window.h>


namespace oden { namespace gammo
{
/*-----------------------------------------------------------------------------------------------------*/
//
// window creator
//
/* ---------------------------------------------------------------------------------------------------- */
// 
bool widget_creator::new_class::create( widget_object* wi )
{
	if( widget_hub* phub = widget_hub::this_thread_hub() ) 
		phub->set_creating_widget( *wi );
	else
		return false;
	
	if( wi->has_parent_widget() )
		param.parent( wi->get_parent_widget()->get_window_handle() );
	
	class_param.procedure( &widget_hub::static_window_proc );
	
	if( window_handle h = create_window( class_param, param ) )
	{
		wi->set_window_handle(h);
		return true;
	}
	return false;
}

// 
bool widget_creator::new_param::create( widget_object* wi )
{
	if( wi->has_parent_widget() )
		param.parent( wi->get_parent_widget()->get_window_handle() );
	
	window_class::value_type wcex;
	if( 0 != ::GetClassInfoEx( get_app_handle().value(), class_id.get(), &wcex ) )
	{
		if( wcex.lpfnWndProc == &widget_hub::static_window_proc )
		{
			if( widget_hub* phub = widget_hub::this_thread_hub() ) 
				phub->set_creating_widget( *wi );
			else
				return false;
		}
	}
	
	if( window_handle h = create_window( class_id, param ) )
	{
		wi->set_window_handle(h);
		return true;
	}
	return false;
}

//
bool widget_creator::system_ctl_impl::create( const window_class_id& cid, widget_object* wi )
{
	if( wi->has_parent_widget() )
		param.parent( wi->get_parent_widget()->get_window_handle() );
	
	if( window_handle h = create_window( cid, param ) )
	{
		wi->set_window_handle(h);
		return true;
	}
	return false;
}

//
bool widget_creator::get_child::create( widget_object* wi )
{
	if( window_handle pa = wi->get_parent_widget()->get_window_handle() )
	{
		if( window_handle h = window( pa ).get_child( child_id ) )
		{
			wi->set_window_handle(h);
			return true;
		}
	}
	return false;
}

//
bool widget_creator::get_parent::create( widget_object* wi )
{
	if( window_handle pa = wi->get_parent_widget()->get_window_handle() )
	{
		wi->set_window_handle(pa);
		return true;
	}
	return false;
}





#if 0
/*-----------------------------------------------------------------------------------------------------*/
//
// window layouter
//
/* ---------------------------------------------------------------------------------------------------- */

//##############################################################
//  window_term_layout
//
// �C�x���g
void window_term_layout::handle_layout_event( widget_object*, window_event& ev )
{
	ev.not_handled();
}
// ���̃E�B�W�F�b�g�𓮂���
bool window_term_layout::move_wid( widget_object* wid )
{
	// �e��dwp�������Ă���΂���𗘗p���A�Ȃ���Ύ��͂œ�����
	if( get_parent_dwp() && get_parent_dwp()->is_started() )
		return get_parent_dwp()->add( wid->get_window_handle(), location_, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER );	
	else 
		return wid->to_window().set_position( location_, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER );
}

//##############################################################
//  window_layout
//
// �C�x���g
void window_layout::handle_layout_event( widget_object* wid, window_event& ev )
{
		switch( ev.code() )
		{
		case WM_SIZE:
			recalc_child_wid(wid);	
			move_child_wid(wid);
			break;
		case WM_SIZING:
			//limit_child_wid(this, size(0,0));
			break;
		default:
			ev.not_handled();
			break;
		}
}

// ���̃��C�A�E�g�𓮂���
bool window_layout::move_wid( widget_object* wid )
{
	// �^�[�~�i�����C�A�E�g�Ƃ��Ď���������->�q�������𓮂���
	if( window_term_layout::move_wid(wid) ) {
		return move_child_wid(wid);
	}
	return false;
}

// �q���C�A�E�g�𓮂���
bool window_layout::move_child_wid( widget_object* wid )
{
	if( !wid->has_child_widget() )
		return true;

	bool ret = true;

	// dwp �͂���
	get_dwp().start( wid->to_window().count_child() );
	
	// �q�E�B�W�F�b�g�𓮂���
	widget_object::widget_iterator itr = wid->begin(), end = wid->end();
	for( ; itr!=end; ++itr )
	{
		if( !(*itr)->move_widget() )
		{
			ret = false;
			break;
		}
	}
	
	// dwp ���I���A�q��𓮂���
	get_dwp().end();
	
	return ret;
}

// �q�ȉ����Čv�Z����
bool window_layout::recalc_child_wid( widget_object* wid )
{
	if( layout_tree_ )
	{
		// �T�C�Y�擾
		rect rc;
		get_layout_rect(wid,rc); 

		// ���C�A�E�g�Čv�Z
		layout_tree_->recalc( rc );
	}
	return true;
}

// �ő�E�ŏ����v�Z
/*
// �C�x���g�n���h��
// WM_CREATE : ���̃E�B���h�E���쐬���ꂽ���A�e�E�B���h�E�����炩���ߌv�Z���Ă������ʒu�Ɉړ�����
void window_layout::layout_on_start( widget_object* wid )
{
	if( wid->has_parent() )
		move_widget(wid);
}

// WM_SIZING : ���̃E�B���h�E�̑傫�����ς���Ă���ԁA�T�C�Y������K�p����
void window_layout::layout_on_sizing( widget_object*, window_event& ev )
{
	WPARAM edge = ev.wparam();
	rect rc = *(reinterpret_cast<RECT*>( ev.lparam() ));

	edge = rc.area();
	edge = 0;
	
	//if( children_layout )
	//	min_size = children_layout>minimum();
	// size max_size = layout_->maximum();
	
	if( rc.height() < min_size.height() ) 
	{
		if( edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT )
		{
			point br = rc.botright();
			rc.set_height( min_size.height() ); 
			rc.move_botright(br);
		}
		else
			rc.set_height( min_size.height() ); 
	}
	if( rc.width() < min_size.width() )
	{
		if( edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT )
		{
			point br = rc.botright();
			rc.set_width( min_size.width() );
			rc.move_botright(br);
		}
		else
			rc.set_width( min_size.width() );
	}
}
*/
/*-----------------------------------------------------------------------------------------------------*/
//
// window final handle event
//
/* ---------------------------------------------------------------------------------------------------- */
//##############################################################
//  window_def_event_handler
//
void window_def_event_handler::final_handle_event( window_event& ev )
{
	widget_hub::call_default_window_proc( ev );
}

//##############################################################
//  subclassable_window
//
void subclassable_window::final_handle_event( window_event& ev )
{
	call_original_proc( ev );
}

//
void subclassable_window::subclass_wid( widget_object* wid )
{
	if( is_subclassed() )
		return;
	
	original_proc_ = wid->to_window().procedure_ptr();
	wid->to_window().set_procedure_ptr( &widget_hub::static_window_proc );
	
	// �E�B�W�F�b�g�n�u�ɒǉ�����
	if( widget_hub* phub = widget_hub::this_thread_hub() )
		phub->add_widget_to_map( *wid );
}

	// ���ɖ߂�
	void subclassable_window::unsubclass_wid( widget_object* wid )
	{
		if( !is_subclassed() )
			return;
		
		wid->to_window().set_procedure_ptr( original_proc_ );
		original_proc_ = 0;

		// �E�B�W�F�b�g�n�u�����菜��
		if( widget_hub* phub = widget_hub::this_thread_hub() )
			phub->remove_widget_from_map( wid->to_window().handle_value() );
	}

// ���̃v���V�[�W�����Ă�
void subclassable_window::call_original_proc( window_event& ev )
{
	ev.result() = ::CallWindowProc( original_proc_, ev.target().handle_value(), ev.code(), ev.wparam(), ev.lparam() ); 
}
#endif

} /* end of namespace gammo */
} /* end of namespace oden */

