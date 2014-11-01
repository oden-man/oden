#include "../Pch.h"
#include <oden/gammo/ui/application.h>
#include <oden/gammo/ui/widget/object.h>
#include <oden/gammo/ui/window/message_queue.h>

namespace oden { namespace gammo
{
//##########################################################
//                  message_loop
//
// ------------------
message_loop::message_loop()
 : hub_()
{
	widget_hub::create_this_thread_hub();
}
message_loop::~message_loop()
{
	widget_hub::release_this_thread_hub();
}

//
int message_loop::enter( app& main_app )
{
	int exit_code = 0;
	raw_window_message msg;
	
	for(;;)
	{
		if( this->retrive( msg ) )
		{
		  // �܂�WM_QUIT�̃`�F�b�N
			if( msg.message==WM_QUIT )
			{
				exit_code = static_cast<int>(msg.wParam);
				break;
			}

		  // ���b�Z�[�W�̏���������
			if( !hub_.preprocess_entry( msg ) )
			{
				// �v���V�[�W���փ��b�Z�[�W�𑗐M
				message_queue::translate( msg );
				message_queue::dispatch( msg );
			}
		}
		else if( this->is_pending() )
		{
			// �A�C�h�����������Ăяo��
			main_app.on_idle( msg );
		}
		else /* error */
		{
			exit_code = -1;
			break;
		}
	}

	return exit_code;
}

//
bool pending_message_loop::retrive( raw_window_message& out )const
{
	return message_queue::peek( out );
}

//
bool waiting_message_loop::retrive( raw_window_message& out )const
{
	return message_queue::get( out );
}

//##########################################################
//             app
//
int app::start( widget_object* wid, message_loop* lop )
{
	if(wid)
		set_main_widget(wid);	
	if(lop)
		set_main_loop(lop);

	if( !main_loop_ )
		this->new_main_loop<waiting_message_loop>();

  // ������
	if( !this->on_entry() )
		return -1;
		
  // ���C���E�B�W�F�b�g�쐬
	if( !this->init_widget() )
		return -1;

	return 0;
}

//
int app::loop()
{
  // ���b�Z�[�W���[�v
	exit_code_ = main_loop_->enter(*this);
	return exit_code_;
}

int app::exit()
{
  // ���C���E�B�W�F�b�g�j��
	if( !this->destroy_widget() )
		return -1;
	
  // �I������
	if( !this->on_exit() )
		return -1;

  // ���[�v�I�u�W�F�N�g�̔j��(�E�B�W�F�b�g�n�u���j��)
	main_loop_.reset();

	return exit_code_;
}

int app::run( widget_object* wid, message_loop* lop )
{
	int ret = this->start(wid,lop);
	if( ret != 0 )
		return ret;
	
	this->loop();
	return this->exit();
}

int run_app( widget_object* wid, message_loop* lop )
{
	if( app* p = app::get_instance() )
		return p->run(wid,lop);
	return -1;
}

int start_app( widget_object* wid, message_loop* lop )
{
	if( app* p = app::get_instance() )
		return p->start(wid,lop);
	return -1;
}

int exit_app()
{
	if( app* p = app::get_instance() )
		return p->exit();
	return -1;
}

bool app::on_entry()
{ 
	return true; 
}
bool app::init_widget()
{
	if( !main_widget_ )
		return false;

	return main_widget_->start();
}
bool app::on_idle( raw_window_message& ) 
{ 
	return true; 
}
bool app::destroy_widget()
{
	main_widget_.reset();
	return true;
}
bool app::on_exit()
{ 
	return true; 
}

void exit_event_loop(int code)
{
	message_queue::post_quit(code);
}

boost::scoped_ptr<app> app::edinstvennoe_;

// declared at def.h
instance_handle get_app_handle()
{
	if( app* p = app::get_instance() )
		return p->get_handle();
	return boost::none;
}

} /* end of namespace gammo */
} /* end of namespace oden */
