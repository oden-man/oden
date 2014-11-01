#pragma once

#ifndef ODEN_GAMMO_UI_INPUT_MOUSE
#define ODEN_GAMMO_UI_INPUT_MOUSE

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------	
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
// --------- chikuwa  --------
// --------- gammo ----------
#include <oden/gammo/ui/window/event.h>

//
//####################################################


namespace oden { namespace gammo
{
	class widget_object;
//______________________________________________________________________________
//
//
//      マウス入力
//
//______________________________________________________________________________
//
//###########################################################
// 関数
//
  // ダブルクリック
	UINT double_click_time();
	bool set_double_click_time(UINT c);
  // 左右入れ替える
	bool swap_mouse_button(bool b=true);

//###########################################################
// マウスキャプチャ
//
	class mouse_capture
	{
	public:
		mouse_capture(){}
		mouse_capture( window_handle w );
		mouse_capture( widget_object* p );
		
		typedef struct releaser {
			void operator()( mouse_capture ){ mouse_capture::release(); }
		} deleter_type;
	
	// メンバ関数
      // 開始・終了
		static window_handle start( window_handle w );
		static bool release();
	  // 取得
		static window_handle get();
	};
	
//###########################################################
// 履歴
//
	

//###########################################################
//  イベント
//	
namespace event
{
  // 引数並びの同じマウスクリックイベント
	template< typename C >
	struct mouse_move_event_def
	{
		enum{ code = C::value };
	};

	typedef mouse_move_event_def< event_code<WM_LBUTTONDBLCLK> >    mouse_left_double;
	typedef mouse_move_event_def< event_code<WM_LBUTTONDOWN> >      mouse_left_down;
	typedef mouse_move_event_def< event_code<WM_LBUTTONUP> >        mouse_left_up;
	
	typedef mouse_move_event_def< event_code<WM_RBUTTONDBLCLK> >    mouse_right_double;
	typedef mouse_move_event_def< event_code<WM_RBUTTONDOWN> >      mouse_right_down;
	typedef mouse_move_event_def< event_code<WM_RBUTTONUP> >        mouse_right_up;
	
	typedef mouse_move_event_def< event_code<WM_MBUTTONDBLCLK> >    mouse_mid_double;
	typedef mouse_move_event_def< event_code<WM_MBUTTONDOWN> >      mouse_mid_down;
	typedef mouse_move_event_def< event_code<WM_MBUTTONUP> >        mouse_mid_up;
	
	typedef mouse_move_event_def< event_code<WM_XBUTTONDBLCLK> >    mouse_x_double;
	typedef mouse_move_event_def< event_code<WM_XBUTTONDOWN> >      mouse_x_down;
	typedef mouse_move_event_def< event_code<WM_XBUTTONUP> >        mouse_x_up;
	
	typedef mouse_move_event_def< event_code<WM_NCLBUTTONDBLCLK> >  mouse_nc_left_double;
	typedef mouse_move_event_def< event_code<WM_NCLBUTTONDOWN> >    mouse_nc_left_down;
	typedef mouse_move_event_def< event_code<WM_NCLBUTTONUP> >      mouse_nc_left_up;
	
	typedef mouse_move_event_def< event_code<WM_NCRBUTTONDBLCLK> >  mouse_nc_right_double;
	typedef mouse_move_event_def< event_code<WM_NCRBUTTONDOWN> >    mouse_nc_right_down;
	typedef mouse_move_event_def< event_code<WM_NCRBUTTONUP> >      mouse_nc_right_up;
	
	typedef mouse_move_event_def< event_code<WM_NCMBUTTONDBLCLK> >  mouse_nc_mid_double;
	typedef mouse_move_event_def< event_code<WM_NCMBUTTONDOWN> >    mouse_nc_mid_down;
	typedef mouse_move_event_def< event_code<WM_NCMBUTTONUP> >      mouse_nc_mid_up;
	
	typedef mouse_move_event_def< event_code<WM_NCXBUTTONDBLCLK> >  mouse_nc_x_double;
	typedef mouse_move_event_def< event_code<WM_NCXBUTTONDOWN> >    mouse_nc_x_down;
	typedef mouse_move_event_def< event_code<WM_NCXBUTTONUP> >      mouse_nc_x_up;
	
	typedef mouse_move_event_def< event_code<WM_MOUSEHOVER> >       mouse_hover;
	typedef mouse_move_event_def< event_code<WM_MOUSEMOVE> >        mouse_move;
	typedef mouse_move_event_def< event_code<WM_MOUSEWHEEL> >       mouse_wheel;
	typedef mouse_move_event_def< event_code<WM_MOUSEHWHEEL> >      mouse_hwheel;
	
	typedef event_def< event_code<WM_MOUSEACTIVATE> >               mouse_activate;
	typedef event_def< event_code<WM_MOUSELEAVE> >                  mouse_leave;
}

//
// 分岐
//
	template< typename C >
	struct event_branch< event::mouse_move_event_def<C> >
	{
		static bool branch( window_event& ev )
		{ return ev.code() == C::code; }
	};

//
// 引数
//
  // BUTTON～、MOUSEMOVE/HOVERでは
  // 　1. 押されたキー 2.XBUTTON 3.位置(クライアント座標)
  // NCBUTTON～では、
  // 　1. ヒットテストの結果 2.XBUTTON 3.位置(スクリーン座標)
  // WHEELでは、
  // 　1. 押されたキー 2.ホイール回転量 3.位置(スクリーン座標)
  
	template< typename C >
	struct event_arg< event::mouse_move_event_def<C> > :
		event::wparam_lparam< event::value<WORD>, event::value<WORD>, event::point_ >
	{};


}} /* end of namespace oden.gammo */

#endif

