#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_WND
#define ODEN_GAMMO_UI_WIDGETS_WND

//#######################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl -------------
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
// --------- gammo ---------
#include <oden/gammo/ui/widget/base.h>
#include <oden/gammo/ui/layout/base.h>

//
//####################################################


namespace oden { namespace gammo
{
//______________________________________________________________________________________________________
//
//          �E�B�W�F�b�g�E�R���g���[������
//______________________________________________________________________________________________________
//
//#################################################
//  �E�B���h�E�N���X�̈���
// 
  // �V�K�쐬
	class new_window_class
	{
	public:
		new_window_class()
		{
			this->describe_window_class()
				.background( COLOR_WINDOW );
		}

		window_class_id supply_class_id()
		{
			cls_.procedure( &widget_hub::static_window_proc );
			return register_window_class( cls_ ).get_id(); // �K���A�g����������Ȃ�
		}

		bool is_widget_window_class()const
		{
			return true;
		}

		window_class::write_val& describe_window_class(){ return cls_; }
	private:
		window_class::write_val cls_;
	};

  // ID�Ŏw��
	class defined_window_class
	{
	public:
		const window_class_id& supply_class_id()const
		{
			return id_.get();
		}

		bool is_widget_window_class()const
		{
			// �v���V�[�W����widget_hub�̂����ׂ�
			window_class::value_type wcex;
			if( 0 != ::GetClassInfoEx( get_app_handle().value(), id_.get_id(), &wcex ) )
			{
				return wcex.lpfnWndProc == &widget_hub::static_window_proc;
			}
			return false;
		}

		void describe_window_class( window_class_id i ){ id_ = i; }
	private:
		window_class_id_val id_;
	};

  // �R���g���[���N���X
	template< typename T >
	class control_window_class
	{
	public:
		window_class_id supply_class_id()const
		{
			return T::get_control_class();
		}

		bool is_widget_window_class()const
		{
			return false;
		}
	};

//#################################################
//  �E�B���h�E�𐶐�
// 
	template< typename Base, typename ClassSupply = new_window_class >
	class new_window
		: public Base, public ClassSupply
	{
	public:
		bool create_control();
	
		window_create_info::write_val& describe_window(){ return param_; }
	private:
		window_create_info::write_val param_;
	};

	// create_control
	template< typename B, typename C >
	bool new_window<B,C>::create_control()
	{
		if( has_parent_widget() ) 
		{
			param_.parent( this->get_parent_widget()->get_window_handle() );
			param_.child_id( this->get_widget_child_id() );
		}

		if( this->is_widget_window_class() )
		{
			if( widget_hub* phub = widget_hub::this_thread_hub() ) 
				phub->set_creating_widget( *static_cast<widget_object*>(this) );
			else
				return false;
		}

		if( this->test_widget_flag( widget_invisible ) )
			param_.style( param_.get().style & ~WS_VISIBLE );
		else
			param_.style( param_.get().style | WS_VISIBLE );

		if( window_handle h = create_window( supply_class_id(), param_ ) )
		{
			this->set_window_handle(h);
			return true;
		}
		return false;
	}
	
//#################################################
//  �E�B���h�E���擾
// 
	template< typename Base >
	class another_window
		: public Base
	{
	public:
		bool create_control(){ return true; }
	
	// initialize �̒��ŌĂԂׂ�
		void widget_is_child( int id )
		{ if( has_parent_widget() ) set_window_handle( get_parent_widget()->get_child(id) ); }

		void widget_is_parent()
		{ if( has_parent_widget() ) set_window_handle( get_parent_widget()->get_window_handle() ); }

		void widget_is( window_handle h )
		{ set_window_handle( h ); }

	};


namespace ui_layout
{
//______________________________________________________________________________________________________
//
//          �E�B�W�F�b�g�E���C�A�E�g�Ǘ�
//______________________________________________________________________________________________________
//
//####################################################
//  �E�B���h�E���������C�A�E�g - �q�E�B�W�F�b�g�̒������s��
//
	template< typename Base >
	class master
		: public Base
	{
	public:
	// �R���X�g���N�^
		master(){}
		virtual ~master(){}
		
	//�I�[�o�[���C�h
	  // �œK�ȃT�C�Y��Ԃ� - �q���C�A�E�g�Ɠ���
		virtual void calc_best_size( size& out )const
		{
			if( layout_tree_ )
				out = layout_tree_->best_size();
			else
				this->Base::calc_best_size(out);
		}
		
	// �����o�֐�		
	  // ���C�A�E�g���Z�b�g
		template< typename T >
		T& describe_layout( T* pNew )
		{
			if( !layout_tree_ ) {
				layout_tree_.reset(pNew);
			}
			
			return *pNew;
		}
		fill_layout& describe_layout()
		{ return this->describe_layout( new fill_layout() ); }

	// ����
	  // �C�x���g�ɉ����č쓮����
		void handle_layout_event( window_event& ev );
	  // ���̃��C�A�E�g�𓮂���
		virtual bool move_widget( move_widget_context& c );

	private:
	// �����֐�
	  // �q���C�A�E�g�𓮂���
		bool move_child_wid();
	  // �q�ȉ����Čv�Z����
		bool recalc_child_wid();
	  // �Ǘ�����̈���擾( �N���C�A���g���W )
		virtual rect get_managed_rect()const
		{ return this->client_rect(); }
		
	// �����o�ϐ�
		boost::scoped_ptr<layout_object> layout_tree_;
	};

	template< typename B >
	void master<B>::handle_layout_event( window_event& ev )
	{
		switch( ev.code() )
		{
		case WM_SIZE:
			recalc_child_wid();	
			move_child_wid();
			break;
		case WM_SIZING:
			//limit_child_wid(this, size(0,0));
			break;
		default:
			ev.not_handled();
			break;
		}
	}

	template< typename B >
	bool master<B>::move_widget( move_widget_context& c )
	{
		// �^�[�~�i�����C�A�E�g�Ƃ��Ď���������->�q�������𓮂���
		if( this->B::move_widget(c) ) {
			return move_child_wid();
		}
		return false;
	}
	
	template< typename B >
	bool master<B>::move_child_wid()
	{
		if( !has_child_widget() )
			return true;

		bool ret = true;

		// dwp �͂���
		move_widget_context this_cx;
		this_cx.dwp.start( count_child_widget() );
		
		// �q�E�B�W�F�b�g�𓮂���
		widget_object::widget_iterator itr = begin(), eitr = end();
		for( ; itr!=eitr; ++itr )
		{
			if( !(*itr)->move_widget(this_cx) )
			{
				ret = false;
				break;
			}
		}
		
		// dwp ���I���A�q��𓮂���
		this_cx.dwp.end();
		
		return ret;
	}

	template< typename B >
	bool master<B>::recalc_child_wid()
	{
		if( layout_tree_ ) {
			// ���C�A�E�g�Čv�Z
			layout_tree_->recalc( this->get_managed_rect() );
		}
		return true;
	}

//####################################################
//  �E�B���h�E�̈ꕔ�Ɏq���C�A�E�g��z�u
//
	template< typename B >
	class partial_master
		: public master<B>
	{
	public:
	// �I�[�o�[���C�h�֐�
      // �Ǘ�����̈��Ԃ�
		rect get_managed_rect()const
		{ return this->widget_location(); }

	  // ���̃��C�A�E�g�͎��͂ł͓����Ȃ�
		bool move_widget( move_widget_context& ){ return true; }
	};

}

//______________________________________________________________________________________________________
//
//          ���̑���{�E�B�W�F�b�g�g�����i
//______________________________________________________________________________________________________
//
//#################################################
// �T�u�N���X�@�\���  
// 
	template< typename B >
	class subclass_widget
		: public B
	{
	public:
	// �R���X�g���N�^
		subclass_widget()
			: original_proc_(0)
		{}

	// �I�[�o�[���C�h
	  // �ŏI�v���V�[�W��
		void final_handle_event( window_event& ev );

	// �����o�֐�
	  // �T�u�N���X��
		void subclass();
	  // ���ɖ߂�
		void unsubclass();
	  // ���̃v���V�[�W�����Ă�
		void call_original_proc( window_event& ev );
      // �T�u�N���X������Ă��邩?
		bool is_subclassed()const { return original_proc_ != 0; }
	private:
		WNDPROC original_proc_;
	};

	template< typename Base >
	void subclass_widget<Base>::final_handle_event( window_event& ev )
	{
		if( is_subclassed() )
			call_original_proc( ev );
		else
			this->Base::final_handle_event( ev );
	}
	
	template< typename B >
	void subclass_widget<B>::subclass()
	{
		if( is_subclassed() )
			return;
		
		original_proc_ = procedure_ptr();
		set_procedure_ptr( &widget_hub::static_window_proc );
		
		// �E�B�W�F�b�g�n�u�ɒǉ�����
		wid->attach_event_loop();
	}

	template< typename B >
	void subclass_widget<B>::unsubclass()
	{
		if( !is_subclassed() )
			return;
		
		set_procedure_ptr( original_proc_ );
		original_proc_ = 0;

		// �E�B�W�F�b�g�n�u�����菜��
		wid->detach_event_loop();
	}

	template< typename B >
	void subclass_widget<B>::call_original_proc( window_event& ev )
	{
		ev.result() = ::CallWindowProc( original_proc_, ev.target().value(), ev.code(), ev.wparam(), ev.lparam() ); 
	}

//______________________________________________________________________________________________________
//
//          �E�B�W�F�b�g�R�A�E�v���Z�b�g
//______________________________________________________________________________________________________
//
  // widget typedef 
	typedef widget<window> 		window_widget;

	class dialog;
	typedef widget<dialog> 		dialog_widget;

  // ���C���E�B���h�E
	template< typename T = gammo::window >
	struct new_widget 
		: public new_window< widget<T> >
	{};

  // �R���g���[��
	template< typename T >
	struct ctl_widget 
		: public subclass_widget< new_window< widget<T>, control_window_class<T> > >
	{
		typedef subclass_widget< another_window< widget<T> > > resource;
	};

}} /* end of namespace oden.gammo */

#endif

