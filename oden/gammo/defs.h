#pragma once

#ifndef ODEN_GAMMO_DEFS
#define ODEN_GAMMO_DEFS


// pragma VC
//
#pragma warning(disable:4503)	// 装飾された名前の切り捨て警告

//#######################################################
//  Ansi Wideの切り替え
//
#if ( defined UNICODE || defined _UNICODE )
	#define ODEN_GAMMO_UNICODE_BUILD
#else
	#define ODEN_GAMMO_ANSI_BUILD
#endif

//#######################################################
//  インクルード
//

#pragma message("oden-gammo-defs inc begin.")
// --------- stl ------------
#include <iosfwd>
#include <utility>
	
// --------- win ------------
#ifndef NOMINMAX
	#define NOMINMAX
#endif
//#define WIN32_LEAN_AND_MEAN  gdiplusと併用できない
#include <windows.h>

// --------- boost ----------
#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/utility/swap.hpp>

// --------- oden  ----------
#include <oden/disallow.h>
#include <oden/index.h>
#include <oden/memory_range.h>

// --------- konbu ----------
// use default cc settings
#include <oden/konbu/defs.h>

// --------- chikuwa  ----------
#include <oden/chikuwa/string_ptr.h>
// --------- gammo ----------
#include <oden/gammo/common/dual_id.h>		// forward
#include <oden/gammo/common/geometry.h>		// forward
#include <oden/gammo/common/string/forward.h>
#include <oden/gammo/common/debug.h>			

// --------- gammo-winver -----------
//#include <oden/gammo/winver.h>

//
//####################################################

#pragma message("oden-gammo-defs inc end.")

//#################################################
//  マクロ
//

namespace oden { namespace gammo
{
//#################################################################
//  データ型定義
//
/* ----------------------------------------------------------------- */
//  汎用
/* ----------------------------------------------------------------- */

  // ハンドルを扱うクラスの基礎
	template< typename T >
	class handle_interface
	{
	public:
		typedef T handle_type;
		typedef typename T::value_type handle_value_type;

		explicit handle_interface( const T& h )
			: handle_(h)
		{}
	
		const T& handle()const { return handle_; }
		T& rhandle() { return handle_; }

		operator handle_type()const { return handle_; }

		handle_value_type handle_value()const { return handle_.value(); }

		bool fail()const { return handle_.is_null(); }
		bool operator!()const { return this->fail(); }

		handle_type attach( handle_type h )
		{
			handle_type x = h;
			handle_.swap( x );
			return x;
		}

		handle_type detach()
		{
			return this->attach( handle_type() );
		}

		void swap( handle_interface& other ) 
		  { boost::swap( handle_, other.handle_ ); }
		friend void swap( handle_interface& l, handle_interface& r ) 
		  { l.swap( r ); }
		
	private:
		T handle_;
	};
	
  // ポインタ、参照両方受け取ります(constはダメ)
	template< typename T >
	class object_ptr
	{
	private:
		template<typename X> struct T_cant_be_const { typedef X type; };
		template<typename X> struct T_cant_be_const<const X> {};
		template<typename X> struct T_cant_be_const<const volatile X> {}; // 万一のため!?
	public:
	
		typedef typename T_cant_be_const<T>::type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
	
		object_ptr( )
			: p_( 0 )
		{}
		object_ptr( pointer p )
			: p_( p )
		{}
		object_ptr( reference r )
			: p_( &r )
		{}
		
		pointer get()const{ return p_; }
	private:
		pointer p_;
	};

  // INTBOOL -> bool
	inline BOOL to_int_bool( bool b ){ return b ? TRUE : FALSE; }
	
  // INTBOOL クラス
	typedef limen< BOOL, TRUE, singular_null<BOOL,FALSE> > int_bool_value;
	
	class int_bool 
		: public int_bool_value
	{
	public:
		typedef int_bool_value base;
	
		int_bool()
			: base()
		{}
		int_bool( bool b )
			: base( to_int_bool(b) )
		{}
		explicit int_bool( BOOL v )
			: base( v )
		{}
		
		void true_() { this->value() = TRUE; }
		void false_(){ this->value() = FALSE; }
	};
  
/* ----------------------------------------------------------------- */
//  system.dll
/* ----------------------------------------------------------------- */
	typedef nullable< HMODULE,	singular_null<HMODULE,0> >		module_handle;
	
/* ----------------------------------------------------------------- */
//  system.file
/* ----------------------------------------------------------------- */
	typedef nullable< HANDLE, 
			singular_null2<HANDLE,0,INVALID_HANDLE_VALUE> > 	kernel_obj_handle;
	typedef kernel_obj_handle 									file_handle;
	typedef kernel_obj_handle 									file_mapping_handle;
	typedef kernel_obj_handle 									process_handle;

/* ----------------------------------------------------------------- */
//  system.com
/* ----------------------------------------------------------------- */
	struct hresult_failed 
	{
		bool operator()( HRESULT hr )const { return FAILED(hr); }
	};	
	struct hresult_succeeded
	{
		bool operator()( HRESULT hr )const { return SUCCEEDED(hr); }
	};
	
	typedef limen< HRESULT, S_OK,
				custom_null<HRESULT,E_FAIL,hresult_failed> >	hresult, hr;
				
	typedef limen< HRESULT, E_FAIL,
				custom_null<HRESULT,S_OK,hresult_succeeded>, failure_t > hr_err;
	// if( hr_err err = Func() ){ show(err); return; }

/* ----------------------------------------------------------------- */
//  graphic
/* ----------------------------------------------------------------- */
	typedef nullable< COLORREF, 
				singular_null<COLORREF,CLR_INVALID> >			colorref;

  // システムカラーを取得
	colorref get_system_color( int dw );
	
/* ----------------------------------------------------------------- */
//  graphic.gdi
/* ----------------------------------------------------------------- */
	typedef nullable< HDC,		singular_null<HDC,0> >			dc_handle;
	typedef nullable< HGDIOBJ,	singular_null<HGDIOBJ,0> >		gdi_obj_handle;
	typedef nullable< HBITMAP,	singular_null<HBITMAP,0> >		bitmap_handle;
	typedef nullable< HBRUSH,	singular_null<HBRUSH,0> >		brush_handle;
	typedef nullable< HFONT,	singular_null<HFONT,0> >		font_handle;
	typedef nullable< HPEN,	singular_null<HPEN,0> >				pen_handle;
	typedef nullable< HENHMETAFILE, 
						singular_null<HENHMETAFILE,0> >			enhmetafile_handle;
  
/* ----------------------------------------------------------------- */
//  ui.resource
/* ----------------------------------------------------------------- */
	typedef nullable< HRSRC,	singular_null<HRSRC,0> >		res_handle;
	
  // IS_INTRESOURCE
	struct is_int_resource
	{
		bool operator()( const void* ptr )const
		{
			return IS_INTRESOURCE(ptr); // 上位ビットが0かチェック
		}
	};
	typedef dual_id< TCHAR, DWORD, is_int_resource >			res_id;
	typedef dual_id_val< res_id >								res_id_val;
	
	typedef nullable< HICON,	singular_null<HICON,0> >		icon_handle;
	typedef nullable< HCURSOR,	singular_null<HCURSOR,0> >		cursor_handle;

/* ----------------------------------------------------------------- */
//  ui.dialog
/* ----------------------------------------------------------------- */
	typedef nullable< INT_PTR,	less_eq_null<INT_PTR, 0> >		dlg_result;

/* ----------------------------------------------------------------- */
//  ui.menu
/* ----------------------------------------------------------------- */
	typedef nullable< HMENU,	singular_null<HMENU,0> >		menu_handle;
	typedef nullable< UINT,     singular_null<UINT,0> >			menu_item_id;
								
/* ----------------------------------------------------------------- */
//  ui.window.window
/* ----------------------------------------------------------------- */
	typedef nullable< HINSTANCE,singular_null<HINSTANCE,0> >	instance_handle;
	typedef nullable< HWND, 	singular_null<HWND,0> >			window_handle;	
  	typedef nullable< ATOM, 	singular_null<ATOM,0> > 		window_class_atom;	
	typedef dual_id< TCHAR, ATOM, is_int_resource >				window_class_id;
	typedef dual_id_val< window_class_id >						window_class_id_val;
	typedef nullable< HDWP,		singular_null<HDWP,0> >			defer_window_pos_handle;
	
/* ----------------------------------------------------------------- */
//  ui.window.control
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
//  ui.window.message
/* ----------------------------------------------------------------- */
	typedef MSG raw_window_message;
	
  // メッセージ定義
	#define GAMMO_WM_CALLDEFPROC				(WM_APP+1)		// コントロールがサブクラス中、デフォルトプロシージャを使う必要が生じたときに送られる
	#define GAMMO_WM_SENDMSGCALLBACK			(WM_APP+2)		// SendMsgCallBackで呼び出し先が制御を返した
	
	#define GAMMO_WM_APP						(WM_APP+20)		// 

  // ウィンドウメッセージを表す
	class window_message;

/* ----------------------------------------------------------------- */
//  ui.layout
/* ----------------------------------------------------------------- */
  // 様々な定数
	enum layout_size_flag
	{
		layout_best_size = -1
	};
		
	enum layout_orientation 
	{
		layout_vertical,
		layout_horizontal
	};
	
/* ----------------------------------------------------------------- */
//  ui.application - 実装はapplication.cppにて
/* ----------------------------------------------------------------- */
  // インスタンスハンドルの取得
	instance_handle get_app_handle();
  // コマンドラインの取得
  // nCmdShowの取得

/* ----------------------------------------------------------------- */
//  ui.international
/* ----------------------------------------------------------------- */

}}

#endif
