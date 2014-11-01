#include "../Pch.h"
#include <oden/gammo/common/string/extend_str_buf.h>

#include <oden/gammo/ui/control/edit.h>

#include <boost/optional.hpp>

namespace oden { namespace gammo
{
//#############################################################
// edit
//

/*
	EM_EMPTYUNDOBUFFER
	EM_GET/SETIMESTATUS	
	EM_GET/SETIHANDLE
	EM_GETITHUMB
	EM_GET/SETIWORDBREAKPROC
	EM_SETTABSTOPS
*/

/*--------------------------------*/
// 文字操作
/*--------------------------------*/
//
void edit::append( string_ptr text )const
{
	insert( end_pos(), text );
}
//
void edit::insert( index at, string_ptr text )const
{
	set_cursor_pos( at ); // カーソルを挿入位置にもってくる
	replace_selected( text );
}
//
void edit::set( string_ptr txt )const
{
	set_text( txt );
}
//
void edit::replace( index first, index last, string_ptr text )const
{
	select( first, last );
	replace_selected( text );
}
//
void edit::replace_selected( string_ptr text )const
{
	Edit_ReplaceSel( handle_value(), text.c_str() );
}
//
void edit::erase_all( )const
{
	set_text( _T("") );
}
//
bool edit::empty( )const
{
	return text_length() == 0;
}
//
void edit::insert_soft_line_break( )const
{
	this->send_message( EM_FMTLINES,TRUE,0 );
}
void edit::remove_soft_line_break( )const
{
	this->send_message( EM_FMTLINES,FALSE,0 );
}

/*-----------------------------------*/
//  選択位置など
/*-----------------------------------*/
//
void edit::select( index beg, index end )const
{
	Edit_SetSel( handle_value(), beg.value(), end.value() );
}
void edit::select_all( )const
{
	select( 0, -1 );
}
void edit::deselect( )const
{
	select( -1, -1 );
}
//
index_range edit::selection()const
{
	index b, e;
	send_message( EM_GETSEL, reinterpret_cast<WPARAM>( &b.value() ), reinterpret_cast<LPARAM>( &e.value() ) );
	return index_range( b, e );
}
//
void edit::set_cursor_pos( index ichar )const
{
	select( ichar, ichar );
}
//
index edit::cursor_pos( )const
{
	return selection().end();
}
//
void edit::set_cursor_home( )const
{
	set_cursor_pos( home_pos() );
}
//
void edit::set_cursor_end( )const
{
	set_cursor_pos( end_pos() );
}
//
index edit::home_pos( )const
{
	return 0;	
}
index edit::tail_pos( )const
{
	return index( end_pos() - 1 ).value_or(0);
}
index edit::end_pos( )const
{
	return text_length();	
}

//
index edit::char_from_point( const point& pos )const
{
	return LOWORD( this->send_message( EM_CHARFROMPOS, 0, MAKELPARAM(pos.x(),pos.y( ) ) ) );
}
edit::line edit::line_from_point( const point& pos )const
{
	return edit::line( *this, HIWORD( this->send_message( EM_CHARFROMPOS, 0, MAKELPARAM(pos.x(),pos.y()) ) ) );
}
std::pair< index, index > edit::offset_from_point( const point& pos )const
{
	DWORD dw = this->send_message( EM_CHARFROMPOS, 0, MAKELPARAM(pos.x(),pos.y( ) ) );
	return std::make_pair( LOWORD( dw ), HIWORD( dw ) );
}
//
boost::optional<point> edit::point_from_char( index ichar )const
{
	DWORD dw = send_message( EM_POSFROMCHAR, ichar.value(), 0 );
	if( dw == -1 )
		return boost::none;
	return boost::optional<point>( point( LOWORD(dw), HIWORD(dw) ) );
}

/*-----------------------------------*/
//  複数行の操作
/*-----------------------------------*/

// edit.line
//---------------------------------
//
edit::line::line( window_handle ow, index i )
	: owner_(ow), i_(i)
{}

//
edit edit::line::owner()const
{
	return owner_;
}

// テキストの取得
bool edit::line::get_text( string& out )const
{
	return owner().get_line_text( i_, out );
}
string edit::line::text()const
{
	string out;
	this->get_text(out);
	return out;
}
//
int edit::line::length()const
{
	return owner().line_length_from_char( this->begin_pos() );
}
//
void edit::line::set( string_ptr str )const
{
	this->select_all();
	owner().replace_selected( str );
	owner().deselect();
}
void edit::line::replace( index a, index b, string_ptr str )const
{
	this->select( a, b );
	owner().replace_selected( str );
	owner().deselect();
}
//
void edit::line::erase_all()const
{
	this->select_all();
	owner().replace_selected( _T("") );	
	owner().deselect();
}
//
void edit::line::select_all()const
{
	owner().select( this->begin_pos(), this->end_pos() );
}
void edit::line::select( index a, index b )const
{
	owner().select( this->begin_pos() + a, this->begin_pos() + b );
}
// 
index edit::line::begin_pos()const
{
	return Edit_LineIndex( owner().handle_value(), i_.value() );
}
index edit::line::tail_pos()const
{
	return index( end_pos() - 1 ).value_or(0);
}
index edit::line::end_pos()const
{
	return begin_pos() + this->length();
}

index_range edit::line::pos()const
{
	return index_range( begin_pos(), end_pos() );
}

//-------------------------------------


// 
edit::line edit::line_at( index iline )const
{
	return edit::line( *this, iline );
}
// 
bool edit::get_line_text( index iline, string& out )const
{
	out.resize( line_at(iline).length()+1 );
	if( 0==Edit_GetLine( handle_value(), iline.value(), buf_begin<char_t>(out), out.size() ) )
		return false;

	out.erase( out.size()-1 );
	return true;
}	
string edit::line_text( index iline )const
{
	string out;
	get_line_text( iline, out );
	return out;
}
//
edit::line edit::cursor_line()const
{
	return line_from_char( this->cursor_pos() );
}
//
int edit::line_length_from_char( index ichar )const
{
	return Edit_LineLength( handle_value(), ichar.value() );	
}
//
int edit::count_line( )const
{
	return Edit_GetLineCount( handle_value() );
}
//
edit::line edit::first_visible_line( )const
{
	if( !(style() & ES_MULTILINE ) )
		return edit::line( *this, boost::none );
	return edit::line( *this, this->send_message( EM_GETFIRSTVISIBLELINE, 0, 0 ) );
}
index edit::first_visible_char( )const
{
	if( style() & ES_MULTILINE )
		return boost::none;
	return this->send_message( EM_GETFIRSTVISIBLELINE, 0, 0 );
}
//
edit::line edit::line_from_char( index ichar )const
{
	return edit::line( *this, Edit_LineFromChar( this->handle_value(), ichar.value() ) );
}

/*--------------------------------*/
//  文字の制限
/*--------------------------------*/
//
bool edit::read_only(bool on)const
{
	return Edit_SetReadOnly( handle_value(),(on?TRUE:FALSE) )==TRUE ? true:false;
}
//
void edit::use_password_mask( char_t ch )const
{
	Edit_SetPasswordChar( handle_value(), static_cast<unsigned int>( ch ) );
}
boost::optional<char_t> edit::password_mask_char( )const
{
	char_t ch = static_cast< char_t >( send_message( EM_GETPASSWORDCHAR, 0, 0 ) );
	return boost::optional<char_t>( ch!=0, ch );
}
//
void edit::set_text_limit( size_t num )const
{
	Edit_LimitText( handle_value(), num );	
}
size_t edit::text_limit( )const
{
	return this->send_message( EM_GETLIMITTEXT, 0, 0 );
}

/*--------------------------------*/
//  レイアウト
/*--------------------------------*/
int edit::left_margin_width( )const
{
	return LOWORD( send_message(EM_GETMARGINS,0,0) );
}
int edit::right_margin_width( )const
{
	return HIWORD( send_message(EM_GETMARGINS,0,0) );
}
std::pair<int,int> edit::margin_width( )const
{
	DWORD dw = send_message( EM_GETMARGINS,0,0 );
	return std::make_pair( LOWORD(dw), HIWORD(dw) );
}
rect edit::formatting_rect( )const
{
	RECT rc;
	Edit_GetRect( handle_value(), &rc );
	return make_rect(rc);
}
void edit::set_formatting_rect( const rect& rc, bool repaint )const
{
	RECT rec;
	get_rect( rc, rec);

	if( repaint )
		send_message( EM_SETRECT, 0, reinterpret_cast<LPARAM>(&rec) );
	else
		send_message( EM_SETRECTNP, 0, reinterpret_cast<LPARAM>(&rec) );
}

/*--------------------------------*/
//  コントロールの操作
/*--------------------------------*/
//
bool edit::is_modified( )const
{
	return send_message( EM_GETMODIFY, 0, 0 ) != 0;
}
void edit::set_modify_flag(bool f)const
{
	send_message( EM_SETMODIFY, f?TRUE:FALSE, 0 );
}
// 
bool edit::scroll( int vert, int horiz )const
{
	return FALSE!=send_message( EM_LINESCROLL, horiz, vert );
}
void edit::scroll_to_caret( )const
{
	send_message( EM_SCROLLCARET, 0, 0 );
}
uinteger edit::scroll_one( int flags )const
{
	LRESULT r = send_message( EM_SCROLL, MAKEWPARAM(flags,0), 0 );
	if( r == FALSE )
		return boost::none;
	return LOWORD(r);
}
//
bool edit::undo( )const
{
	return TRUE==Edit_Undo( handle_value() ) ? true:false;
}
bool edit::can_undo( )const
{
	return TRUE==Edit_CanUndo( handle_value() ) ? true:false;
}
void edit::clear_selected( )const
{
	send_message( WM_CLEAR, 0, 0 );
}
void edit::clear( )const
{
	select_all();
	clear_selected();
}
//
void edit::copy_to_clipboard( )const
{
	send_message( WM_COPY, 0, 0 );
}
void edit::cut_to_clipboard( )const
{
	send_message( WM_CUT, 0, 0 );
}
void edit::paste_from_clipboard( )const
{
	send_message( WM_PASTE, 0, 0 );
}

/*--------------------------------------*/
// XP later
/*--------------------------------------*/
#if ( _WIN32_WINNT >= 0x0501 )
//
bool edit::get_cue_banner( wide_string& out )const
{
	if( out.empty() ) 
		out.resize(32);
	do{
		if( FALSE==Edit_GetCueBannerText( handle_value(),  buf_begin<char_t>(out), out.size() ) )
			return false;
	}
	while( extend_str_buf(out) );
	
	return true;
}
wide_string edit::cue_banner()const
{
	wide_string out;
	get_cue_banner(out);
	return out;
}
bool edit::set_cue_banner( wide_string_ptr text )const
{
	return TRUE==Edit_SetCueBannerText( handle_value(), text.c_str() );
}
bool edit::set_cue_banner_focused( wide_string_ptr text )const
{
	return TRUE==Edit_SetCueBannerTextFocused( handle_value(), text.c_str(), TRUE );
}

// 
bool edit::hide_balloon_tip( )const
{
	return TRUE==Edit_HideBalloonTip( handle_value() );
}
bool edit::show_balloon_tip( edit_balloon_tip::value_type& param )const
{
	return TRUE==Edit_ShowBalloonTip( handle_value(), &param );
}

#endif

}}


