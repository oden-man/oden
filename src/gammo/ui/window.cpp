#include "../Pch.h"

#include <oden/gammo/ui/window/window.h>

#include <oden/gammo/graphic/gdi/dc.h>

#include <oden/gammo/common/string/extend_str_buf.h>


namespace oden { namespace gammo
{
/*-----------------------------------------------------------------------------------------------------*/
//
// window class
//
/* ---------------------------------------------------------------------------------------------------- */
//##############################################################
// 
//
// ウィンドウクラスを登録
window_class_id_val register_window_class( window_class::value_type& rdesc )
{
	if( rdesc.lpszClassName == 0 )
	{
		string buf;

		if( rdesc.hIcon || rdesc.hbrBackground || rdesc.hCursor )
		{ // gammo-w.[style][hinstance][hIcon][hCursor][hBg]
			buf.resize(7+sizeof(DWORD)*2+sizeof(LONG_PTR)*2*4+1);

			_stprintf_s( buf_begin<char_t>(buf), buf.size(), _T("gammo.w%x%x%x%x%x"), 
				rdesc.style, 
				reinterpret_cast<LONG_PTR>(rdesc.hInstance), 
				reinterpret_cast<LONG_PTR>(rdesc.hIcon),
				reinterpret_cast<LONG_PTR>(rdesc.hCursor),
				reinterpret_cast<LONG_PTR>(rdesc.hbrBackground)
			);
		}
		else
		{ // gammo-w.[style][hinstance]
			buf.resize(7+sizeof(DWORD)*2+sizeof(LONG_PTR)*2+1);

			_stprintf_s( buf_begin<char_t>(buf), buf.size(), _T("gammo.w%x%x"), 
				rdesc.style, 
				reinterpret_cast<LONG_PTR>(rdesc.hInstance) 
			);		
		}

		rdesc.lpszClassName = buf.c_str();

		//
		ATOM atm = ::RegisterClassEx( &rdesc );
		if( atm == 0 )
		{
			window_class_id_val ret;
			if( ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS )
				ret.set_string( buf );

			return ret;
		}
		
		return atm;
	}
	else
	{
		return ::RegisterClassEx( &rdesc );
	}
}

// ウィンドウクラスを登録から外す
bool unregister_window_class( window_class_id id, instance_handle h )
{
	if( !h ) h = get_app_handle();

	return FALSE!=::UnregisterClass( id.get(), h.value() );
}

/*
*/

/*-----------------------------------------------------------------------------------------------------*/
//
// window
//
/* ---------------------------------------------------------------------------------------------------- */
//##############################################################
// ハンドル関連
//
// at message_hub.cpp
bool window::create( const window_class_id &cls_id, window_create_info::value_type& desc )
{
	window_handle h = create_window( cls_id, desc );
	if( h )
		attach( h );
		
	return h.is_valid();
}
bool window::create( window_class::value_type& cls_desc, window_create_info::value_type& desc )
{
	window_handle h = create_window( cls_desc, desc );
	if( h )
		this->attach(h);
	
	return h.is_valid();
}
//
bool window::destroy()
{
	if( this->destroy_handle() ) {
		this->detach();
		return true;
	}
	return false;
}
bool window::destroy_handle()const
{
	return FALSE!=::DestroyWindow( handle_value() );
}

//##############################################################
// ウィンドウの描画
//
// Show
void window::show( int nCmdShow )const
{
	::ShowWindow( handle_value(), nCmdShow );
}

bool window::update( )const
{
	return FALSE!=::UpdateWindow( handle_value() );
}

void window::invalidate( bool erase )const
{
	::InvalidateRect( handle_value(), NULL, (erase?TRUE:FALSE) );
}
void window::invalidate( const rect &fr, bool erase )const
{
	RECT rc;
	get_rect( fr, rc );
	::InvalidateRect( handle_value(), &rc, (erase?TRUE:FALSE) );
}

/*
bool window::redraw( UINT flags )const;
bool window::redraw( const rect &fr, UINT flags )const;
*/
//##############################################################
// ウィンドウの状態
//
void window::enable( bool b )const
{
	::EnableWindow( handle_value(), b?TRUE:FALSE ) ;
}
void window::disable()const
{
	enable(false);
}

window_handle window::activate()const
{
	return ::SetActiveWindow( handle_value() );
}

bool window::close( )const
{
	return FALSE!=::CloseWindow( handle_value() );
}

//##############################################################
// ほかのウィンドウとの関係・ハンドル取得
//
bool window::has_parent( )const 
{
	return !get_parent_or_owner().is_null();
}
bool window::has_child( )const
{
	return ::GetWindow( handle_value(), GW_CHILD ) != NULL;
};

bool window::is_ancestor_of( window_handle child )const 
{
	return FALSE != ::IsChild( this->handle_value(), child.value() );
}
bool window::is_descendant_of( window_handle parent )const
{
	return FALSE != ::IsChild( parent.value(), this->handle_value() );
}

bool window::is_parent_of( window child )const
{
	return child.get_parent() == this->handle();
}
bool window::is_child_of( window parent )const 
{
	return this->get_parent() == parent.handle();
}

bool window::is_owned_by( window owner )const
{
	return this->get_owner() == owner.handle();	
}
bool window::is_owner_of( window ownee )const
{
	return ownee.get_owner() == this->handle();	
}

window_handle window::get_parent( )const  
{ 
	return ::GetAncestor( handle_value(), GA_PARENT ); 
}
window_handle window::get_parent_or_owner( )const  
{ 
	return ::GetParent( handle_value() ); 
}
window_handle window::get_owner( )const
{
	return ::GetWindow( handle_value(), GW_OWNER );
}

window_handle window::get_child( int id )const
{
	return ::GetDlgItem( handle_value(), id );
}
window_handle window::get_child_from_point( const point& pt, UINT types )const
{
	return ::ChildWindowFromPointEx( handle_value(), get_point(pt), types );
}

int window::count_child()const
{
	// 数えるコールバック関数の定義
	struct CallBack {
		static BOOL CALLBACK Proc(HWND, LPARAM lparam)
		{
			++(*reinterpret_cast<int*>(lparam));
			return TRUE;
		}
	};
	
	int ret = 0;
	::EnumChildWindows( handle_value(), 
		&CallBack::Proc,
		reinterpret_cast<LPARAM>( &ret )
		);
	return ret;	
}

//##############################################################
// ウィンドウの座標取得・変換
//		
bool window::screen_rect( rect& out )const
{
	RECT rc;
	bool r( FALSE!=::GetWindowRect( handle_value(), &rc ) );
	if( r )
		out = make_rect( rc );
	return r;
}
rect window::screen_rect( )const
{
	rect rc; 
	screen_rect( rc );
	return rc;
}

bool window::client_rect( rect& out )const
{
	RECT rc;
	bool r( FALSE != ::GetClientRect( handle_value(), &rc ) );
	if( r )
		out = make_rect( rc );
	return r;
}
rect window::client_rect( )const
{
	rect rc; 
	client_rect( rc );
	return rc;
}

bool window::screen_to_client( point& out )const
{
	POINT pt; 
	get_point( out, pt );
	
	bool r( FALSE != ::ScreenToClient( handle_value(), &pt ) );
	if( r )
		out = make_point(pt);
		
	return r;	
}	
point window::to_client( const point& scrpt )const
{
	POINT pt; 
	get_point( scrpt, pt );

	if( FALSE != ::ScreenToClient( handle_value(), &pt ) )
		return make_point(pt);
	return point();	
}
bool window::screen_to_client( rect& out )const
{
	return screen_to_client( out.rtopleft() ) && screen_to_client( out.rbotright() );
}

bool window::client_to_screen( point& out )const
{
	POINT pt;
	get_point( out, pt );
	
	bool r( FALSE != ::ClientToScreen( handle_value(), &pt ) );
	if( r )
		out = make_point( pt );
		
	return r;
}
point window::to_screen( const point& clipt )const
{
	POINT pt; 
	get_point( clipt, pt );

	if( FALSE != ::ClientToScreen( handle_value(), &pt ) )
		return make_point( pt );
	return point();	
}
bool window::client_to_screen( rect& out )const
{
	return client_to_screen( out.rtopleft() ) && client_to_screen( out.rbotright() );
}

//##############################################################
// ウィンドウのサイズ変更
//		
bool window::adjust_rect( const size& client_size, rect& out, bool hasMenu )const
{
	RECT rc = { 0, 0, client_size.width(), client_size.height() };
	
	if( FALSE == ::AdjustWindowRectEx( &rc, style(), (hasMenu ? TRUE : FALSE), style_ex() ) )
		return false;
	
	out = make_rect(rc);
	return true;
}

bool window::move( const rect& new_rc, bool repaint )const
{
	return FALSE != ::MoveWindow(
		handle_value(),
		new_rc.left(), new_rc.top(),
		new_rc.width(), new_rc.height(), 
		repaint ? TRUE : FALSE
	);
}

bool window::set_position(
	const rect&	new_rc,
	DWORD flags,
	window_handle insert_after 
) const
{
	return FALSE != ::SetWindowPos(
		handle_value(),
		insert_after.value(),
		new_rc.left(),
		new_rc.top(),
		new_rc.width(),
		new_rc.height(),
		flags
	);	
}
	
//##############################################################
// ウィンドウのデータ取得
//		
// classlong
bool window::get_class_name( string& out )const
{
	int copied = 0;
	while( extend_str_buf( out, copied ) )
	{
		copied = ::GetClassName( handle_value(),  buf_begin<char_t>(out), static_cast<int>(out.size()) );
		if( copied == 0 )
			return false;
	}
	return true;
}
string window::class_name()const
{
	string out;
	get_class_name( out );
	return out;
}

window_class_atom window::class_atom( )const
{
	return static_cast<ATOM>( ::GetClassLongPtr( handle_value(), GCW_ATOM ) );
}

// windowlong
LONG_PTR window::set_window_long( int i, LONG_PTR l )const
{
	return ::SetWindowLongPtr( handle_value(), i, l );
}
LONG_PTR window::window_long( int i )const
{
	return ::GetWindowLongPtr( handle_value(), i );
}

DWORD window::set_style( DWORD flags )const
{ return static_cast<DWORD>( set_window_long( GWL_STYLE,  static_cast<LONG_PTR>(flags) ) ); }

DWORD window::set_style_ex( DWORD flags )const
{ return static_cast<DWORD>( set_window_long( GWL_EXSTYLE, static_cast<LONG_PTR>(flags) ) ); }

WNDPROC window::set_procedure_ptr( WNDPROC newProc )const
{ return reinterpret_cast<WNDPROC>( set_window_long( GWLP_WNDPROC, reinterpret_cast<LONG>(newProc) ) ); }

LONG_PTR window::set_child_id( LONG_PTR id )const
{ return static_cast<int>( set_window_long( GWLP_ID, id ) ); }

LONG_PTR window::set_user_data( LONG_PTR data )const
{ return set_window_long( GWLP_USERDATA, data ); }

LONG_PTR window::set_extra_data( int index, LONG_PTR data )const
{ return set_window_long( index, data ); }
	
DWORD window::style()const
{ return static_cast<DWORD>( window_long( GWL_STYLE ) ); }

DWORD window::style_ex()const
{ return static_cast<DWORD>( window_long( GWL_EXSTYLE ) ); }

WNDPROC window::procedure_ptr()const
{ return reinterpret_cast<WNDPROC>(window_long(GWLP_WNDPROC)); }

LONG_PTR window::child_id()const
{ return window_long(GWLP_ID); }	

instance_handle window::app_handle()const
{ return reinterpret_cast<HINSTANCE>( window_long(GWLP_HINSTANCE)); }

LONG_PTR window::user_data()const
{ return window_long( GWLP_USERDATA ); }	

LONG_PTR window::extra_data( int index )const
{ return window_long( index );	}	

//
void window::update_window_longs()const
{
	::SetWindowPos( handle_value(), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
}

//###############################################################
//  テキスト
//	
bool window::get_text( string &out )const
{
	out.resize( text_length() + 1 );
	if( 0==::GetWindowText( handle_value(),  buf_begin<char_t>(out), static_cast<int>(out.size()) ) )
	{
		out.clear();
		return false;
	}

	out.resize( out.size()-1 );
	return true;
}
string window::text()const
{
	string out;
	get_text( out );
	return out;
}

// NULL文字含めない長さ
int window::text_length()const
{
	return ::GetWindowTextLength( handle_value() );
}

// 文字列をせっと
bool window::set_text( string_ptr text )const
{
	return FALSE != ::SetWindowText( handle_value(), text.c_str() );
}

//###############################################################
//  メニュー
//	
menu_handle window::menu()const
{
	if( !is_top_level() )
		return boost::none;

	return ::GetMenu( handle_value() );
}
menu_handle window::set_menu( menu_handle mh )const
{
	if( !is_top_level() )
		return boost::none;

	menu_handle old = this->menu();
	if( 0==::SetMenu( handle_value(), mh.value() ) )
		return boost::none;
	return old;
}
bool window::draw_menu_bar()const
{
	return 0 != ::DrawMenuBar( handle_value() );
} 

menu_handle window::system_menu()const
{
	return ::GetSystemMenu( handle_value(), FALSE );
}
void window::reset_system_menu()const
{
	::GetSystemMenu( handle_value(), TRUE );
}

//###########################################################
// メッセージ送信
//

LRESULT window::send( const window_message& m )const
{
	return ::SendMessage( handle_value(), m.code(), m.wparam(), m.lparam() );
}
LRESULT window::send_message( UINT code, WPARAM w, LPARAM l )const
{
	return ::SendMessage( handle_value(), code, w, l );
}

//
bool window::send_timeout( const window_message& m, UINT flags, DWORD timeoutMilisec, LRESULT& retOut)const
{
	 return 0 != ::SendMessageTimeout( handle_value(), m.code(), m.wparam(), m.lparam(), flags, timeoutMilisec, reinterpret_cast<PDWORD_PTR>(&retOut) );
}

//
bool window::send_notify( const window_message& m )const
{
	return FALSE != ::SendNotifyMessage( handle_value(), m.code(), m.wparam(), m.lparam() ) ;
}

//
bool window::post( const window_message& m )const
{
	return FALSE != ::PostMessage( handle_value(), m.code(), m.wparam(), m.lparam() );
}
bool window::post_message( UINT code, WPARAM w, LPARAM l )const
{
	return FALSE != ::PostMessage( handle_value(), code, w, l );
}

//##################################################################
// その他
//
// ドラッグ
bool window::drag_detect( const point& scr_start )const
{
	return 0!=::DragDetect( handle_value(), get_point(scr_start) );
}
//  このウィンドウ上でのマウスの動きを伝えるメッセージを送る(一度のみ)
bool window::track_mouse_event( DWORD flags, DWORD tm )const
{
	TRACKMOUSEEVENT ev;
	ev.cbSize = sizeof(TRACKMOUSEEVENT);
	ev.dwFlags = flags;
	ev.hwndTrack = handle_value();
	ev.dwHoverTime = tm;
	
	return 0!=::TrackMouseEvent(&ev);
}

//##################################################################
// その他
//
bool window::animate( DWORD Dura, DWORD AnimateType )const
{
	return FALSE != ::AnimateWindow( handle_value(), Dura, AnimateType );
}

colorref window::sys_color( int dw )
{
	return ::GetSysColor(dw);
}
bool window::set_sys_color( memory_range<int> i, memory_range<COLORREF> cols )
{
	return 0!=::SetSysColors( i.size(), i.get(), cols.get() );
}



/*-----------------------------------------------------------------------------------------------------*/
//
// CreateWindowEx
//
/* ---------------------------------------------------------------------------------------------------- */
//	
// クラスID + ウィンドウパラメータ
window_handle create_window( const window_class_id &cls_id, window_create_info::value_type& cri )
{		
	return ::CreateWindowEx(
		cri.dwExStyle,
		cls_id.get(),	// クラスID
		cri.lpszName,
		static_cast<DWORD>(cri.style),
		cri.x, cri.y,
		cri.cx, cri.cy,
		cri.hwndParent,
		cri.hMenu,
		cri.hInstance,
		cri.lpCreateParams
	);
}

// ウィンドウクラスパラメータ + ウィンドウパラメータ
window_handle create_window( window_class::value_type& cls_desc, window_create_info::value_type& desc )
{
	window_class_id_val cls_id_val = register_window_class( cls_desc );
		
	return create_window( cls_id_val.get_id(), desc );
}

/*-----------------------------------------------------------------------------------------------------*/
//
// DeferWindowPos
//
/* ---------------------------------------------------------------------------------------------------- */
//	
bool defer_window_pos::start( int reserve_size )
{
	if( !fail() )
		return false;
	
	attach( ::BeginDeferWindowPos( reserve_size ) );
	return !fail();
}

bool defer_window_pos::end()
{
	if( fail() )
		return false;
		
	if( FALSE == ::EndDeferWindowPos( handle_value() ) )
		return false;

	attach(boost::none);
	return true;
}

bool defer_window_pos::add( window_handle wnd, const rect& fr, DWORD flags, window_handle insert_after )
{
	if( fail() )
		return false;
		
	attach( 
		::DeferWindowPos( 
			handle_value(), 
			wnd.value(), 
			insert_after.value(), 
			fr.left(),
			fr.top(),
			fr.width(),
			fr.height(),
			flags
		) );
	
	return !fail();
}

} /* end of namespace gammo */
} /* end of namespace oden */

