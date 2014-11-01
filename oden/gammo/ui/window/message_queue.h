#pragma once

#ifndef ODEN_GAMMO_UI_WND_MSGQUE
#define ODEN_GAMMO_UI_WND_MSGQUE

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ------ STL ---------
// ----- boost --------
#include <boost/optional.hpp>

// ------- oden -------
// ------- gammo ------
#include <oden/gammo/common/geometry.h>

//
//
//###############################################################

namespace oden { namespace gammo
{
	
//____________________________________________________________________________
//
//
//           ���b�Z�[�W�@�\(MsgLoop.cpp)
//
//____________________________________________________________________________
//	
//############################################################
// ���b�Z�[�W�֐�
//	
namespace message_queue
{
// �֐�
  // �L���[���烁�b�Z�[�W���Ƃ�
	bool get( 
		raw_window_message& 		out, 
		window_handle src_wnd = boost::none,
		const std::pair<UINT,UINT>&	filter  = std::make_pair(0,0) );
  // �L���[���Ď����A���b�Z�[�W������΂Ƃ�
	bool peek( 
		raw_window_message& 		out, 
		DWORD 						opt     = PM_NOREMOVE,
		window_handle src_wnd = boost::none,
		const std::pair<UINT,UINT>&	filter  = std::make_pair(0,0) );
	
  // ���b�Z�[�W�ɉ�����Ԃ��A������Ăяo���X���b�h�ɖ߂�
	bool reply( LRESULT ret_code );
  // ���b�Z�[�W����̎��A���̊ԑ��X���b�h�ɐ����n��
	bool wait();		
  // ���X���b�h���烁�b�Z�[�W������ꂽ���H�܂��A�ǂ�Ȏ�i�ŁH
	DWORD current_send_state();
  // �L���[��Ԃ��擾
	//DWORD       		current_state();
		
  // ���j�[�N�ȃ��b�Z�[�W���쐬
	boost::optional<UINT> register_message( string_ptr str );
  // WM_QUIT���|�X�g����
	void post_quit( int exitcode );
  // ���̃X���b�h�Ƀ��b�Z�[�W�𑗂�
	// broad_cast_message		
	
  // ���b�Z�[�W�̃L�[�R�[�h��|�󂷂�
	bool 	translate( const raw_window_message& out );
  // ���b�Z�[�W���v���V�[�W���ɂ�����
	LRESULT dispatch( const raw_window_message& out );	
	
  // �Ō�Ƀ��b�Z�[�W���󂯂��Ƃ��̃J�[�\���ʒu
	point	last_cursor_pos();
  // �Ō�Ƀ��b�Z�[�W���󂯂��Ƃ��̎���
	LONG	last_time_stamp();
  // �g�����
	LPARAM	extra_info();
  // �g�������Z�b�g
	LPARAM	set_extra_info( LPARAM val );
	
}
	
//############################################################
// 
//	
	
	
	
}}

#endif
