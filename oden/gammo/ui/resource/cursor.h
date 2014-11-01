#pragma once

#ifndef ODEN_GAMMO_UI_RES_CURS
#define ODEN_GAMMO_UI_RES_CURS

//#######################################################
//  インクルード
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
//     カーソル
//
//_______________________________________________________________________________________
//
//###########################################################
// コントロールクラス
//
	class cursor
		: public handle_interface<cursor_handle>
	{
	public:
		cursor( cursor_handle h = boost::none )
			: handle_interface<cursor_handle>(h)
		{}

	// メンバ関数
      // 現在の位置
        static bool get_screen_pos( point& out );
        static point screen_pos( );
        static bool set_screen_pos( const point& pt );

      // 現在のカーソル
        static cursor_handle set( cursor_handle h );
        static cursor_handle get( );
	};

//###########################################################
//  その他関数
//	
	// システムカーソル
	bool set_system_cursor( cursor_handle h, DWORD id );


}}
#endif