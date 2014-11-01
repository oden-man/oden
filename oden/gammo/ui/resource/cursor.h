#pragma once

#ifndef ODEN_GAMMO_UI_RES_CURS
#define ODEN_GAMMO_UI_RES_CURS

//#######################################################
//  �C���N���[�h
//
// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
// --------- win ------------

// --------- boost ----------
// --------- oden ----------
#include <oden/property.h>
//----------- gammo ----------

//
//###############################################


namespace oden { namespace gammo
{		
//_______________________________________________________________________________________
//
//
//     �J�[�\��
//
//_______________________________________________________________________________________
//
//###########################################################
// �R���g���[���N���X
//
	class cursor
		: public handle_interface<cursor_handle>
	{
	public:
		cursor( cursor_handle h = boost::none )
			: handle_interface<cursor_handle>(h)
		{}

	// �����o�֐�
      // ���݂̈ʒu
        static bool get_screen_pos( point& out );
        static point screen_pos( );
        static bool set_screen_pos( const point& pt );

      // ���݂̃J�[�\��
        static cursor_handle set( cursor_handle h );
        static cursor_handle get( );
	};

//###########################################################
//  ���̑��֐�
//	
	// �V�X�e���J�[�\��
	bool set_system_cursor( cursor_handle h, DWORD id );


}}
#endif