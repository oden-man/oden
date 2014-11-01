#pragma once

#ifndef ODEN_GOB_GRA_GDI_DC
#define ODEN_GOB_GRA_GDI_DC

//#####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
#include <oden/property.h>
// --------- gammo -----------
#include <oden/gammo/ui/window/event.h>
#include <oden/gammo/graphic/gdi/object.h>

//
//
//#################################################


namespace oden { namespace gammo
{
	class dc;
	class widget_object;
	
	typedef index dc_save_id;
	
//_______________________________________________________________________________________
//
//   デバイスコンテキスト
//_______________________________________________________________________________________
//
//################################################################
//  定数など
//	
  // ブレンドファンクション
	ODEN_STRUCT( blend_func, BLENDFUNCTION )
  	{
		struct get
		{};

		struct set
  		{
			Me& src_alpha( BYTE val ) 
			{ self::val(this).SourceConstantAlpha = val; return self::me(this); }

			Me& original_src_alpha() 
			{ self::val(this).AlphaFormat = AC_SRC_ALPHA; return self::me(this); }
		};
	};
	
//################################################################
// デバイスコンテキスト・セッション保持クラス
//	
	class dc_object_session
	{
	private:
		typedef dc_object_session self;	
	public:
	// コンストラクタ
		dc_object_session( const dc& dc_ref, gdi_obj_handle old )
			: pdc_( &dc_ref ), record_( old )
		{}
	// メンバ関数
		void restore()const;
		
		typedef struct restorer {
			void operator()( self& obj )const { obj.restore(); }			
		} deleter_type;

	private:
		const dc* pdc_;
		gdi_obj_handle record_;
	};
	
	// 使用例
	// scoped<dc_object_session> pensess( dc.select_pen( penn ) );

//################################################################
// デバイスコンテキストクラス
//
	class dc
		: public handle_interface<dc_handle>
	{
	private:
		typedef dc self;
	public:	
	// コンストラクタ
		dc( dc_handle h = boost::none )
			: handle_interface<dc_handle>( h )
		{}
		
	// DC		
	  // 解放する
	  	bool release( window_handle wh )const;
	  // 削除する
	  	bool deletes();
	  	
	  // 関連付けられたウィンドウを取得
	  	window_handle assoc_window( )const;
	  	
	// GDIオブジェクト
	  // GDIオブジェクトの取り換え
		dc_object_session select_object	( const gdi_obj_handle& obj )const;
	    dc_object_session select_brush	( const brush_handle& br )const		{ return select_object( br.value() ); }
		dc_object_session select_font	( const font_handle& fo )const		{ return select_object( fo.value() ); }
		dc_object_session select_bitmap	( const bitmap_handle& bo )const	{ return select_object( bo.value() ); }
		
	  // DCブラシの色を取得・変更
	    colorref set_brush_color( colorref )const;
	    colorref brush_color( )const;
	    
	  // DCペンの色を取得・変更
	    colorref set_pen_color( colorref )const;
	    colorref pen_color( )const;
	    
	  // セッションをシステムで保存・元に戻す
	  	dc_save_id save( )const;
	  	bool restore( dc_save_id id )const;
		
	  // 現在のオブジェクトを取得
	  
	// ビットマップ
	  // BitBlt
		bool bit_blt( 
			dc_handle 	 dest, 
			const rect&  dest_rect,
			const point& src_at,
			DWORD	     rop_code = SRCCOPY
		);
	  // StretchBlt
		bool stretch_blt( 
			dc_handle 	 dest, 
			const rect&  dest_rect,
			const rect&	 src_rect,
			DWORD	     rop_code = SRCCOPY
		);
	  // AlphaBlend
		bool alpha_blend( 
			dc_handle dest, 
			const rect& dest_rect,
			const rect&	src_rect,
			const blend_func::value_type& bfn 
		);
		
	// 描画
	  // 背景色の取得・変更
	  	colorref set_back_color( colorref );
	  	colorref back_color( )const;
	  // 背景モード
		positive set_back_mode( int );
		positive back_mode()const;
	  // 背景 noneでTRANSPARENT
		void background( colorref );
		
	// 図形
      // ブラシで塗りつぶした四角形を描く
		bool paint_rect		( const rect& rc, const brush_handle &br )const;
	  // ブラシで１ピクセルのフレームを描く
		bool paint_frame		( const rect& rc, const brush_handle &br )const;
	  // 四角形を描画
		bool rectangle		( const rect& rc )const;
	  // 楕円を描画
		bool ellipse			( const rect& bound_rect )const;
	  // 角の丸い四角形を描画
		bool round_rect		( const rect& rc, const size& round_size )const;
	  // 点を書く
		bool  pixelv			( const point& pos, colorref col )const;
		colorref pixel			( const point& pos, colorref col )const;
		
	  // フォーカスの四角形を描画
		void draw_focus_rect    ( const rect& rc )const;
		
	// テキスト、フォント
	  // カレントのフォントで文字を書く
		bool text( string_ptr str, const point& pos )const;
		bool text( string_ptr str, const point& pos, UINT options, const rect& rc = rect(), memory_range<INT> distances = boost::none )const;
	  // 
		positive draw_text( string_ptr str, const rect& rc, UINT fmt = 0 )const;
	  
	  // 文字色設定
	  	colorref set_text_color( colorref );
	  	colorref text_color( )const;
	  	
		//void current_text_metrics(TEXTMETRIC &Out);
		
	// デリーター・プリセット
	  // 解放
		struct releaser {
			void operator()( dc& o )const { o.release( o.assoc_window() ); }
		};
	  // 削除
		struct deleter {
			void operator()( dc& o )const { o.deletes(); }
		};
	};

	class shared_dc : public dc
	{
	public:
		shared_dc( dc_handle h = boost::none ) : dc(h) {}
		typedef dc::releaser deleter_type;
	};

	class new_dc : public dc
	{
	public:
		new_dc( dc_handle h = boost::none ) : dc(h) {}
		typedef dc::deleter deleter_type;
	};
	
  // WM_PAINT内で使用
	class paint_dc : public dc
	{
	public:
	// コンストラクタ
		paint_dc(){}
		paint_dc( window_handle h ){ this->begin(h); }
		paint_dc( widget_object* w );
		
		typedef struct ender {
			void operator()( paint_dc& o ){ o.end(); }
		} deleter_type;
		
	// メンバ関数
	  // 開始・終了
		dc_handle begin( window_handle w );
		void end();
		
	  // 情報取得
		dc_handle dc()const   { return ps_.hdc; }
		rect area()const      { return gammo::make_rect( ps_.rcPaint ); }
		bool is_bg_erased()const { return ps_.fErase!=0; }
		
	  // オーバーライド
		window_handle assoc_window()const { return w_; }
		
	private:
		window_handle w_;
		PAINTSTRUCT ps_;
	};
	
	// scoped<paint_dc> pdc(h); // 若干のコピーコスト　まあ問題にはならないだろうが・・・

	// scoped<paint_dc> pdc; // コストなし！
	// pdc->begin(h);
	
//_______________________________________________________________________________________
//
//   構築関数
//_______________________________________________________________________________________
//
//#####################################################################
// 取得 -> releaser
//
  // デスクトップのデバイスコンテキスト
	shared_dc  get_desktop_dc( );

  // プライマリディスプレイのデバイスコンテキスト
	shared_dc  get_display_dc( );
	
  // Windowの クライアントエリアのデバイスコンテキストを取得
	shared_dc  get_client_dc( window_handle wh );
	
  // Window全体のデバイスコンテキストを取得
	shared_dc  get_window_dc( window_handle wh );

	// scoped<shared_dc> dc( get_desktop_dc() );
	
//#####################################################################
// 作成 -> deleter
//
  // メモリDCを作成
	new_dc  create_memory_dc( dc_handle dh );

	// scoped<new_dc> dc( create_memory_dc( desktopdc_ ) );
	
//_______________________________________________________________________________________
//
//   DC イベント
//_______________________________________________________________________________________
//
// 名前
//
namespace event 
{
	typedef event_def< event_code<WM_PAINT> >        paint;
	typedef event_def< event_code<WM_SYNCPAINT> >    sync_paint;
	typedef event_def< event_code<WM_NCPAINT> >      nc_paint;
}

//
// 引数
//

	
}}

#endif
