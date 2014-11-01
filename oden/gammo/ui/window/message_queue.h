#pragma once

#ifndef ODEN_GAMMO_UI_WND_MSGQUE
#define ODEN_GAMMO_UI_WND_MSGQUE

//#####################################################
//  インクルード
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
//           メッセージ機構(MsgLoop.cpp)
//
//____________________________________________________________________________
//	
//############################################################
// メッセージ関数
//	
namespace message_queue
{
// 関数
  // キューからメッセージをとる
	bool get( 
		raw_window_message& 		out, 
		window_handle src_wnd = boost::none,
		const std::pair<UINT,UINT>&	filter  = std::make_pair(0,0) );
  // キューを監視し、メッセージがあればとる
	bool peek( 
		raw_window_message& 		out, 
		DWORD 						opt     = PM_NOREMOVE,
		window_handle src_wnd = boost::none,
		const std::pair<UINT,UINT>&	filter  = std::make_pair(0,0) );
	
  // メッセージに応答を返し、制御を呼び出しスレッドに戻す
	bool reply( LRESULT ret_code );
  // メッセージが空の時、その間他スレッドに制御を渡す
	bool wait();		
  // 他スレッドからメッセージが送られたか？また、どんな手段で？
	DWORD current_send_state();
  // キュー状態を取得
	//DWORD       		current_state();
		
  // ユニークなメッセージを作成
	boost::optional<UINT> register_message( string_ptr str );
  // WM_QUITをポストする
	void post_quit( int exitcode );
  // 他のスレッドにメッセージを送る
	// broad_cast_message		
	
  // メッセージのキーコードを翻訳する
	bool 	translate( const raw_window_message& out );
  // メッセージをプロシージャにおくる
	LRESULT dispatch( const raw_window_message& out );	
	
  // 最後にメッセージを受けたときのカーソル位置
	point	last_cursor_pos();
  // 最後にメッセージを受けたときの時間
	LONG	last_time_stamp();
  // 拡張情報
	LPARAM	extra_info();
  // 拡張情報をセット
	LPARAM	set_extra_info( LPARAM val );
	
}
	
//############################################################
// 
//	
	
	
	
}}

#endif
