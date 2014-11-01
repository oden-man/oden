#include "../Pch.h"

#include <oden/gammo/ui/resource/cursor.h>

namespace oden { namespace gammo
{
//
// 
//
bool cursor::get_screen_pos( point& out )
{
    POINT pt;
    if( GetCursorPos( &pt )!=FALSE ) {
        out = make_point(pt);
        return true;
    }
    return false;
}

point cursor::screen_pos( )
{
	point out;
    get_screen_pos( out );
	return out;
}

bool cursor::set_screen_pos( const point& pt )
{
    return SetCursorPos( pt.x(), pt.y() )!=FALSE;
}

cursor_handle cursor::set( cursor_handle h )
{
    return SetCursor( h.value() );
}
cursor_handle cursor::get( )
{
    return ::GetCursor( );
}

//
//
//
bool set_system_cursor( cursor_handle h, DWORD id )
{
    return ::SetSystemCursor( h.value(), id )!=FALSE;
}

} /* end of namespace gammo */
} /* end of namespace oden */




