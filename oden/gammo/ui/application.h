#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_APP
#define ODEN_GAMMO_UI_WIDGETS_APP

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ------ STL ---------
// ----- boost --------
#include <boost/scoped_ptr.hpp>
// ------- oden ---------
// ------- gammo --------
#include <oden/gammo/ui/widget/base.h>

//
//
//###############################################################

namespace oden { namespace gammo
{	
	class widget_object;
	class app;

//____________________________________________________________________________
//
//           ���b�Z�[�W�X���b�h
//____________________________________________________________________________
//		
//###########################################################################################
//  application_flow
//
  // �C���^�[�t�F�[�X
	class message_loop
	{
	public:		
	// �R���X�g���N�^
		message_loop();
		virtual ~message_loop();
		
	// �����o�֐�
	  // �J�n
		virtual int enter( app& main_app );
      // ���b�Z�[�W
	private:
		virtual bool retrive( raw_window_message& out )const = 0;
		virtual bool is_pending()const = 0;
		
	protected:
	  // �n�u���擾
		widget_hub& hub() {return hub_;}
		const widget_hub& hub()const {return hub_;}
		
	private:
		widget_hub hub_;
	};

  // �A�C�h�����������t���[
	class pending_message_loop
		: public message_loop
	{
		bool retrive( raw_window_message& out )const;
		bool is_pending()const { return true; }
	};
	
  // ���b�Z�[�W��҂t���[
	class waiting_message_loop
		: public message_loop
	{
		bool retrive( raw_window_message& out )const;
		bool is_pending()const { return false; }
	};
	
  // �Ȃɂ����Ȃ����[�v
	class null_message_loop
		: public message_loop
	{
		bool retrive( raw_window_message& )const { return false; }
		bool is_pending()const { return false; }
		
		int enter( app& ){ return 0; }
	};
		
//###########################################################################################
//  application_flow
//
	class app
	{
	public:
	// �R���X�g���N�^
		app() : hInstance_(), cmdline_(0), nCmdShow_(0), exit_code_(0) {}
		virtual ~app(){}

	// �ÓI�����o
	  // �C���X�^���X�̐ݒ�E�擾
		static void set_instance( app* pInst ) 	
		{ edinstvennoe_.reset( pInst ); }

		static app* get_instance() 
		{ return edinstvennoe_.get(); }
	
	// �����o�֐�
	  // ���C���E�B�W�F�b�g�쐬
		template< typename Wid >
		Wid* new_widget() 
		{ return set_main_widget( new Wid() ); }
	
		template< typename Wid, typename A1 >
		Wid* new_widget( const A1& a1 )
		{ return set_main_widget( new Wid(a1) ); }

		template< typename Wid, typename A1, typename A2 >
		Wid* new_widget( const A1& a1, const A2& a2 )
		{ return set_main_widget( new Wid(a1,a2) ); }

		template< typename Wid >
		Wid* set_main_widget( Wid* p )
		{
			main_widget_.reset( p ); 
			return p;
		}

      // ���C�����[�v�쐬
		template< typename T >
		T* new_main_loop()
		{ return set_main_loop( new T() ); }

		template< typename T >
		T* set_main_loop( T* p )
		{
			main_loop_.reset( p ); 
			return p;
		}
		
	  // �J�n�����蔲����
		int run( widget_object* wid = 0, message_loop* lop = 0 );

	  // �ʁX�Ɏg�p����Ȃ�
		int start( widget_object* wid = 0, message_loop* lop = 0 );
		int loop();
		int exit();
		
	  // �I�[�o�[���C�h�ł������
		virtual bool on_entry();
		virtual bool init_widget();
		virtual bool on_idle( raw_window_message& );
		virtual bool destroy_widget();
		virtual bool on_exit();
		
	  // WINMAIN �̈�����ݒ�E�擾
		void set_arguments( HINSTANCE hInstance, LPTSTR lpCmdline, int nCmdShow ) 
		{
			hInstance_ = hInstance;
			cmdline_ = lpCmdline;
			nCmdShow_ = nCmdShow;
		}
		
		instance_handle get_handle()const { return hInstance_; }
		
	private:
		boost::scoped_ptr<widget_object> 	main_widget_;
		boost::scoped_ptr<message_loop>		main_loop_;
		
		instance_handle hInstance_;
		LPTSTR	  		cmdline_;
		int             nCmdShow_;
		int             exit_code_;

		static boost::scoped_ptr<app>  edinstvennoe_;
	};

//###########################################################################################
// app -> static �֐�
//
  // app.�B��̎��̂��쐬
	template< typename App >
	App* new_app( HINSTANCE hInstance, LPTSTR lpCmdline = 0, int nCmdShow = SW_SHOWNORMAL )
	{
		App* napp = new App();
		napp->set_arguments( hInstance, lpCmdline, nCmdShow );
		app::set_instance( napp );

		return napp;
	}
	inline app* new_app( HINSTANCE hInstance, LPTSTR lpCmdline = 0, int nCmdShow = SW_SHOWNORMAL )
	{
		return new_app<app>( hInstance, lpCmdline, nCmdShow );
	}

  // gammo::app::run
	int run_app( widget_object* wid = 0, message_loop* lop = 0 );
  // gammo::app::start
	int start_app( widget_object* wid = 0, message_loop* lop = 0 );
  // gammo::app::exit
	int exit_app();

  // �C�x���g���[�v�𔲂���
	void exit_event_loop(int code);

}}

#endif

