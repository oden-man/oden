#include "../Pch.h"

#include <boost/optional.hpp>

#include <oden/gammo/ui/window/message_queue.h>

namespace oden { namespace gammo
{
//##########################################################
//                  message_queue
//
namespace message_queue
{
bool get( 
	raw_window_message& out, 
	window_handle src_wnd,
	const std::pair<UINT,UINT>&	filter )
{
	return -1!=::GetMessage( &out, src_wnd.value(), filter.first, filter.second );		
}

bool peek(
	raw_window_message& out, 
	DWORD option,
	window_handle src_wnd,
	const std::pair<UINT,UINT>&	filter )
{
	return FALSE!=::PeekMessage( &out, src_wnd.value(), filter.first, filter.second, option );	
}

bool translate( const raw_window_message& out )
{
	return FALSE!=::TranslateMessage( &out );	
}

LRESULT dispatch( const raw_window_message& out )
{
	return ::DispatchMessage( &out );	
}

boost::optional<UINT> register_message( string_ptr str )
{
	UINT m = ::RegisterWindowMessage( str.c_str() );
	return boost::make_optional( m!=0, m ); 
}

bool reply( LRESULT ret_code )
{
	return FALSE != ::ReplyMessage( ret_code );	
}

DWORD current_send_state()
{
	return ::InSendMessageEx(NULL);
}


bool wait()
{
	return FALSE!=::WaitMessage();	
}

void post_quit( int exitcode )
{
	::PostQuitMessage(exitcode);
}

point last_cursor_pos()
{
	DWORD dw = ::GetMessagePos();
	return point( GET_X_LPARAM(dw), GET_Y_LPARAM(dw) );	
}
LONG last_time_stamp()
{
	return ::GetMessageTime();	
}
LPARAM extra_info()
{
	return ::GetMessageExtraInfo();
}
LPARAM set_extra_info( LPARAM l )
{
	return ::SetMessageExtraInfo(l);
}

} /* end of namespace message_queue */

} /* end of namespace gammo */
} /* end of namespace oden */
