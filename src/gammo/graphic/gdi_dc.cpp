#include "../Pch.h"

#include <oden/gammo/graphic/gdi/dc.h>
#include <oden/gammo/ui/widget/object.h>

namespace oden { namespace gammo
{	
/*-----------------------------------------------------------------------------------------------------*/
//
//  DC
//
/* ---------------------------------------------------------------------------------------------------- */
//########################################################################
// DC
//

// デバイスコンテキストを解放
bool dc::release( window_handle wh )const
{
	return 1==::ReleaseDC( wh.value(), handle_value() );
}
// 削除
bool dc::deletes()
{
	if( FALSE!=::DeleteDC( handle_value() ) ) 
		return false;
	return true;
}

// 関連付けられたウィンドウを取得
window_handle dc::assoc_window()const 
{ 
	return ::WindowFromDC( handle_value() ); 
}

//#########################################################################
// GDIオブジェクト
//

// オブジェクトを選択
dc_object_session dc::select_object( const gdi_obj_handle& obj )const 
{
	return dc_object_session( *this, ::SelectObject( handle_value(), obj.value() ) );
}

// DCブラシの色
colorref dc::set_brush_color( colorref col )const
{
	return ::SetDCBrushColor( handle_value(), col.value() );
}
colorref dc::brush_color( )const
{
	return ::GetDCBrushColor( handle_value() );
}

// DCペンの色
colorref dc::set_pen_color( colorref col )const
{
	return ::SetDCPenColor( handle_value(), col.value() );
}
colorref dc::pen_color( )const
{
	return ::GetDCPenColor( handle_value() );
}

// セッションのセーブ・ロード
dc_save_id dc::save( )const
{
	return ::SaveDC( handle_value() );
}
bool dc::restore( dc_save_id id )const
{
	return FALSE!=::RestoreDC( handle_value(), id.value() );
}

//##################################################################################
// ビットマップ
//
// BitBlt
bool dc::bit_blt( 
	dc_handle dest, 
	const rect&      dest_rect,
	const point&	 src_at,
	DWORD      		 rop_code )
{
	return FALSE != ::BitBlt( 
		dest.value(), 
		dest_rect.left(), dest_rect.top(), 
		dest_rect.width(), dest_rect.height(), 
		this->handle_value(),
		src_at.x(), src_at.y(),
		rop_code
	); 
}
// StretchBlt
bool dc::stretch_blt( 
	dc_handle dest, 
	const rect&      dest_rect,
	const rect&      src_rect,
	DWORD      		 rop_code )
{
	return FALSE != ::StretchBlt( 
		dest.value(), 
		dest_rect.left(), dest_rect.top(), 
		dest_rect.width(), dest_rect.height(), 
		this->handle_value(),
		src_rect.left(), src_rect.top(), 
		src_rect.width(), src_rect.height(), 
		rop_code
	); 
}
// AlphaBlend
bool dc::alpha_blend( 
	dc_handle dest, 
	const rect&      dest_rect,
	const rect&	 	 src_rect,
	const blend_func::value_type& bfn )
{
	return FALSE != ::AlphaBlend( 
		dest.value(), 
		dest_rect.left(), dest_rect.top(), 
		dest_rect.width(), dest_rect.height(), 
		this->handle_value(),
		src_rect.left(), src_rect.top(), 
		src_rect.width(), src_rect.height(), 
		bfn
	);
}
		
//##################################################################################
// 描画
//

// 背景色
colorref dc::set_back_color( colorref col )
{
	return ::SetBkColor( handle_value(), col.value() );
}
colorref dc::back_color( )const
{
	return ::GetBkColor( handle_value() );
}

// 背景色
positive dc::set_back_mode( int m )
{
	return ::SetBkMode( handle_value(), m );
}
positive dc::back_mode()const
{
	return ::GetBkMode( handle_value() );
}

//
void dc::background( colorref c )
{
	if(c)
		set_back_color(c);
	else
		set_back_mode(TRANSPARENT);
}

//##################################################################################
// 図形描画
//

// 四角形塗りつぶし
bool dc::paint_rect( const rect& rc, const brush_handle &br )const
{
	RECT rec;
	get_rect( rc, rec );
	return FALSE != ::FillRect( handle_value(), &rec, br.value() );
}
// 枠を描く
bool dc::paint_frame( const rect& rc, const brush_handle &br )const
{
	RECT rec;
	get_rect( rc, rec );
	return FALSE != ::FrameRect( handle_value(), &rec, br.value() );
}
// 四角形
bool dc::rectangle( const rect& rc )const
{
	return FALSE != ::Rectangle( handle_value(), rc.left(), rc.top(), rc.right(), rc.bottom() );
}
// 楕円
bool dc::ellipse( const rect& bound_rect )const
{
	return FALSE != ::Ellipse( handle_value(), bound_rect.left(), bound_rect.top(), bound_rect.right(), bound_rect.bottom() );
}
// 角の丸い四角形
bool dc::round_rect( const rect& rc, const size& round_size )const
{
	return FALSE != ::RoundRect( handle_value(), rc.left(), rc.top(), rc.right(), rc.bottom(), round_size.width(), round_size.height() );
}

// 点
bool dc::pixelv( const point& pt, colorref col )const
{
	return FALSE != ::SetPixelV( handle_value(), pt.x(), pt.y(), col.value() );
}
colorref dc::pixel( const point& pt, colorref col )const
{
	return ::SetPixel( handle_value(), pt.x(), pt.y(), col.value() );
}

//##################################################################################
// テキスト・フォント
//

// テキストを描画
bool dc::text( string_ptr str, const point& pos )const
{
	return FALSE != ::TextOut( handle_value(), pos.x(), pos.y(), str.c_str(), str.size() );
}
bool dc::text( string_ptr str, const point& pos, UINT opts, const rect& rc, memory_range<INT> dists )const
{
	const RECT* prc = 0;
	RECT r;
	if( rc.is_valid() ) 
	{
		get_rect( rc, r );
		prc = &r;
	}

	return FALSE != ::ExtTextOut( handle_value(), pos.x(), pos.y(), opts, prc, str.c_str(), str.size(), dists.get() );
}

positive dc::draw_text( string_ptr str, const rect& rc, UINT fmt )const
{
	RECT rec;
	get_rect( rc, rec );
	return ::DrawText( handle_value(), str.c_str(), str.size(), &rec, fmt );
}

// 背景色
colorref dc::set_text_color( colorref col )
{
	return ::SetTextColor( handle_value(), col.value() );
}
colorref dc::text_color( )const
{
	return ::GetTextColor( handle_value() );
}

/*-----------------------------------------------------------------------------------------------------*/
//
//  DC オブジェクトセッションクラス
//
/* ----------------------------------------------------------------------------------------------------*/
//########################################################################
//  メンバ関数
//

// 元に戻す
void dc_object_session::restore( )const
{
	pdc_->select_object( record_ );
}

/*-----------------------------------------------------------------------------------------------------*/
//
//  DC構築関数
// 
/*-----------------------------------------------------------------------------------------------------*/
//###################################################################
// release
//
// デスクトップのデバイスコンテキスト
shared_dc get_desktop_dc( )
{
	return ::GetDC( NULL );
}
// プライマリディスプレイのデバイスコンテキスト
shared_dc get_display_dc( )
{
	return ::GetWindowDC( NULL );
}	
// Windowの クライアントエリアのデバイスコンテキストを取得
shared_dc get_client_dc( window_handle wh )
{
	return ::GetDC( wh.value() );
}
// Window全体のデバイスコンテキストを取得
shared_dc get_window_dc( window_handle wh )
{
	return ::GetWindowDC( wh.value() );
}

//###################################################################
// delete
//
// メモリDCを作成
new_dc create_memory_dc( dc_handle dh )
{
	return ::CreateCompatibleDC( dh.value() );
}


//###################################################################
// paint beg/end
//
paint_dc::paint_dc( widget_object* pw )
{
	if( pw )
		this->begin( pw->get_window_handle() );
}

dc_handle paint_dc::begin( window_handle w )
{
	if( !w_ && w ) 
	{
		w_ = w;
		if( dc_handle nh = ::BeginPaint( w_.value(), &ps_ ) ) 
		{
			this->attach(nh);
			return nh;
		}
	}
	
	w_ = boost::none;
	return boost::none;
}

void paint_dc::end()
{
	::EndPaint( w_.value(), &ps_ );
	w_ = boost::none;
}





} /* end of namespace gammo */
} /* end of namespace oden */

