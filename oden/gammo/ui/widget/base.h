#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_BASE
#define ODEN_GAMMO_UI_WIDGETS_BASE

//#######################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
#include <boost/unordered_map.hpp>
// --------- oden  ----------
#include <oden/object/scoped.h>
#include <oden/object/container.h>
// --------- gammo ---------
#include <oden/gammo/ui/window/window.h>
#include <oden/gammo/ui/widget/object.h>
#include <oden/gammo/ui/widget/event_system.h>
#include <oden/gammo/system/thread/tls.h>

//
//####################################################


namespace oden { namespace gammo
{

//_____________________________________________________________________________________________________
//
//                              �E�B�W�F�b�g�@�R�A���N���X
//_____________________________________________________________________________________________________
//
//#################################################
//  move_widget�ɂ킽��
//	
	class move_widget_context
	{
	public:
		defer_window_pos dwp;
	};
	
//#################################################
//  �I�[�o�[���C�h�p
// 
	class widget_base
		: public widget_object, public detail::event_handler_master
	{
	protected:
		widget_base() : location_( 10, 10, 20, 20 ) {}
	public:
		virtual ~widget_base(){}

	// �����o�֐�
	  // �C�x���g�����E�I�[�o�[���C�h
		virtual void handle_event			( window_event& );
		virtual void handle_layout_event	( window_event& ev ){ ev.not_handled(); }
		virtual void my_handle_event		( window_event& ev );
		virtual void final_handle_event		( window_event& );
		virtual bool preprocess_event		( preprocessing_event& ){ return true; }
		bool attach_event_loop();
		bool detach_event_loop();

	  // ���C�A�E�g����
		bool move_widget					( move_widget_context& c );
		void set_widget_location			( const rect& r ) { location_ = r; }
		const rect& widget_location			()const { return location_; }
		void get_best_size					( size& out )const;
		void fit_widget ();
		void best_size_changed ();
		
	private:
		rect location_;
		mutable size best_size_cache_;
	};

//#################################################
//  �R���g���[���ƍ쐬���@�����肵�Ă���  
// 
	template< typename T >
	class widget
		: public widget_base, public T
	{
	public:	
		typedef T control_type;

	// �R���X�g���N�^
		widget(){}
		virtual ~widget()
		{
			this->widget_object::destroy();
		}

	// �����o�֐�
	  // �E�B���h�E��
		window& to_window(){ return *this; }
		const window& to_window()const { return *this; }

	  // �R���g���[����
		T& to_control(){ return *this; }
		const T& to_control()const { return *this; }
		
	  // window::destroy���㏑��
		bool destroy(){ return this->widget_object::destroy(); }

	  // �f���[�^���ꉞ�㏑��
		typedef struct destroyer {
			void operator()( widget& o )const { o.destroy(); }
		} deleter_type;
		
	  // ���C�A�E�g�E�I�[�o�[���C�h
		virtual void get_best_size_now( size& out )const
		{ /*to_control*/to_window().calc_best_size(out); }
	};
	
//################################################################
// �v���Z�b�g�̃n���h�� 
//
  // �C�x���g��]��
	struct widget_event_transfer
	{
		void operator()( widget_object* p, window_event& e )const
		{
			p->handle_event(e);
			e.not_handled();
		}
	};
	//typedef slot_function< widget_event_transfer, slot_bind_param<widget_object*>, event_param<event::args> > transfer_event;

//_________________________________________________________________________________________________________
//
//           �E�B�W�F�b�g���܂Ƃ߂�@�\
//_________________________________________________________________________________________________________
//	
//################################################################
//  �E�B���h�E�v���v���Z�X�v���V�[�W��
//	
	class preprocessing_event
	{
	public:
	// �R���X�g���N�^
		preprocessing_event()
			: pmsg_(0), processed_(false)
		{}

	// �����o�֐�
	  // �擾
		raw_window_message& msg(){ return *pmsg_; }
		const raw_window_message& msg()const { return *pmsg_; }

		HWND& rhwnd(){ return msg().hwnd; }
		HWND hwnd(){ return msg().hwnd; }

		UINT& rcode(){ return msg().message; }
		UINT code(){ return msg().message; }
		
	  // �X�V
		void set( raw_window_message& out )
		{
			pmsg_ = &out;
			processed_ = false;
		}

	  // �������ꂽ���H�t���O�̑���
		void set_processed() { processed_ = true; }
		bool is_processed()const { return processed_; }

	private:
		raw_window_message* pmsg_;
		bool processed_;
	};
	
//###################################################################
// �C�x���g���z�n�u
//	
	class widget_hub
	{
	private:
		typedef boost::unordered_map<HWND, widget_object*> 	widget_map;
		typedef std::vector<widget_object*> 				widget_vector;
		
	public:	
	// �R���X�g���N�^
		widget_hub()
			: cur_widget_ptr_(0), widget_map_(), proc_call_depth_(0)
		{}
		
	// �����o�֐�
		// ���b�Z�[�W���n���h���e�[�u���ւƑ��M
		bool proc_entry( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& ret );
		// ���b�Z�[�W��O����
		bool preprocess_entry( raw_window_message& out );
		// �ΏۃE�B�W�F�b�g��T���o���ĕύX
		void set_current_widget( window_handle hwnd );
		// �쐬���̃E�B���h�E��ݒ肷��
		void set_creating_widget( widget_object& wid );
		// �E�B�W�F�b�g���}�b�v�ɒǉ��E�폜����
		bool add_widget( widget_object* wid );
		bool remove_widget( window_handle hwnd );
		// ���b�Z�[�W���ɏ�������E�B�W�F�b�g��ǉ�
		void add_event_filter( widget_object* pmp );
		bool remove_event_filter( widget_object* pmp );
		// 
		int proc_entry_depth()const;
		
	// �ÓI�����o�֐�
	  // tl_hub����
		static bool create_this_thread_hub();
		static widget_hub* this_thread_hub();
		static bool release_this_thread_hub();

	  // ���b�Z�[�W�v���V�[�W��
		// WNDPROC�p���b�Z�[�W�v���V�[�W��
		static LRESULT CALLBACK static_window_proc	( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		// DLGPROC�p���b�Z�[�W�v���V�[�W��
		static INT_PTR CALLBACK static_dialog_proc	( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );

		// �f�t�H���g�̃E�B���h�E�v���V�[�W�����Ă�
		static void call_default_window_proc( window_event& arg );
		// �f�t�H���g�̃_�C�A���O�v���V�[�W�����Ă�
		static void call_default_dialog_proc( window_event& arg );
		
	private:
		widget_object*	cur_widget_ptr_;	// ���݂̃E�B�W�F�b�g�ւ̃|�C���^
		widget_map		widget_map_;		// 
		widget_vector 	event_filter_;

		int	            proc_call_depth_;	// proc_entry�̍ċA�Ăяo���K��
		
		// �X���b�h���[�J���ȃ��b�Z�[�W�n�u�̃f�[�^
		static scoped<thread_local_stroage> tl_hub;
	};


}} /* end of namespace oden.gammo */


#endif

