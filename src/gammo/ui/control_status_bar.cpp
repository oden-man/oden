#include "../Pch.h"

#include <oden/gammo/ui/control/status_bar.h>

#include <oden/gammo/common/string/extend_str_buf.h>

namespace oden { namespace gammo
{

//############################################
// StatusBar
// 
BYTE status_bar::get_part_text( index at, string& out )const
{
	out.resize( part_text_length(at)+1 );
	BYTE ret = static_cast<BYTE>( send_message( SB_GETTEXT, at.value(), reinterpret_cast<LPARAM>( buf_begin<char_t>(out)) ) );

	out.erase( out.size()-1 );
	return ret;
}
string status_bar::part_text( index at )const
{
	string out;
	get_part_text( at, out );
	return out;
}
bool status_bar::set_part_text( index at, string_ptr str, int flags )const
{
	return TRUE == 
	send_message( SB_SETTEXT, MAKEWORD(at.value(),0)|flags, reinterpret_cast<LPARAM>(str.c_str()) );
}

//
int status_bar::part_text_length( index at )const
{
	return part_text_info(at).first;
}
std::pair<int,int> status_bar::part_text_info( index at )const
{
	DWORD ret = send_message( SB_GETTEXTLENGTH, at.value(), 0 );
	return std::make_pair( LOWORD(ret), HIWORD(ret) );
}

//
bool status_bar::set_owner_draw_part( index at, LPARAM data, int flags )const
{
	return TRUE ==
	send_message( SB_SETTEXT, MAKEWORD(at.value(),0)|flags|SBT_OWNERDRAW, data );
}

//
void status_bar::get_part_tip_text( index at, string& out )const
{
	while( extend_str_buf(out) ) {
		send_message( SB_GETTIPTEXT, MAKEWPARAM(at.value(), out.size()), reinterpret_cast<LPARAM>( buf_begin<char_t>(out)) );
	}
}
string status_bar::part_tip_text( index at )const
{
	string out;
	get_part_tip_text( at, out );
	return out;
}
void status_bar::set_part_tip_text( index at, string_ptr str )const
{
	send_message( SB_SETTIPTEXT, static_cast<WPARAM>(at.value()), reinterpret_cast<LPARAM>(str.c_str()) );
}

//
icon_handle status_bar::get_part_icon( index at )const
{
	return reinterpret_cast<HICON>( send_message( SB_GETICON, at.value(), 0 ) );
}
bool status_bar::set_part_icon( index at, icon_handle ih )const
{
	return 0 != 
	send_message( SB_SETICON, static_cast<WPARAM>(at.value()), reinterpret_cast<LPARAM>(ih.value()) );
}

//
bool status_bar::set_part_layout( memory_range<int> sizes )const
{
	return TRUE ==
	send_message( SB_SETPARTS, static_cast<WPARAM>(sizes.size()), reinterpret_cast<LPARAM>(sizes.get()) ); 
}
void status_bar::get_part_layout( std::vector<int> &out )const
{
	out.resize( count_part() );
	send_message( SB_GETPARTS, out.size(), reinterpret_cast<LPARAM>(buf_begin<int>(out)) );
}
int status_bar::count_part()const
{
	return static_cast<int>( send_message( SB_GETPARTS, 0, 0 ) ); 
}

//
bool status_bar::get_part_rect( index at, rect& out )const
{
	RECT rc;
	if( FALSE==send_message( SB_GETRECT, at.value(), reinterpret_cast<LPARAM>(&rc) ) )
		return false;

	out = make_rect( rc );
	return true;
}
rect status_bar::part_rect( index at )const
{
	rect out;
	get_part_rect( at, out );
	return out;
}

//
void status_bar::set_simple_mode( bool b )const
{
	send_message( SB_SIMPLE, b?TRUE:FALSE, 0 );
}
bool status_bar::is_simple_mode()const
{
	return 0 != send_message( SB_ISSIMPLE, 0, 0 ); 
}

//
colorref status_bar::set_back_color( colorref col )const
{
	return send_message( SB_SETBKCOLOR, 0, col.value() );
}

//
void status_bar::set_min_height( int h )const
{
	send_message( SB_SETMINHEIGHT, h, 0 );
}
bool status_bar::get_border_width( status_bar::borders& out )const
{
	return TRUE == send_message( SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(out.get_ptr()) );
}
status_bar::borders status_bar::border_width()const
{
	status_bar::borders out;
	get_border_width(out);
	return out;
}

//
bool status_bar::use_unicode_format( bool b )const
{
	return 0 != send_message( SB_SETUNICODEFORMAT, b?TRUE:FALSE, 0 );
}
bool status_bar::is_unicode_format()const
{
	return 0 != send_message( SB_GETUNICODEFORMAT, 0, 0 );
}

}}