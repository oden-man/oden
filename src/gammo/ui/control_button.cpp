#include "../Pch.h"

#include <oden/gammo/ui/control/button.h>


namespace oden { namespace gammo
{
//#############################################################
// button
//
/*
   イメージリスト・スプリットボタン関連
*/
/*--------------------------------------*/
// 共通
/*--------------------------------------*/
// 
bool button::checked()const
{
	return BST_CHECKED == Button_GetCheck(handle_value());
}
bool button::unchecked()const
{
	return BST_UNCHECKED == Button_GetCheck(handle_value());
}
bool button::indeterminate()const
{
	return BST_INDETERMINATE == Button_GetCheck(handle_value());
}
//
void button::check()const
{
	Button_SetCheck(handle_value(),BST_CHECKED);
}
void button::uncheck()const 
{
	Button_SetCheck(handle_value(),BST_UNCHECKED);
}
void button::set_indeterminate()const
{
	Button_SetCheck(handle_value(),BST_INDETERMINATE);
}
void button::check( bool bCheck )const
{	
	if( bCheck )
		this->check();
	else
		this->uncheck();
}

DWORD button::state()const
{
	return Button_GetState(handle_value());
}
//
void button::set_highlight()const
{
	Button_SetState(handle_value(),TRUE);
}
//
void button::remove_highlight()const
{
	Button_SetState(handle_value(),FALSE);
}


/*--------------------------------------*/
// XP later 
/*--------------------------------------*/
#if (_WIN32_WINNT >= 0x0501 )  /* XP later */
//
bool button::get_text_margin( rect& out )const
{
	RECT rc;
	if(FALSE!=Button_GetTextMargin( handle_value(), &rc )) {
		out = make_rect( rc );
		return true;
	}
	return false;
}
rect button::text_margin()const
{
	rect rc;
	get_text_margin(rc);
	return rc;
}
bool button::set_text_margin( const rect& marg )const
{
	RECT rc;
	get_rect( marg, rc );
	return FALSE!=Button_SetTextMargin( handle_value(), &rc );
}
//
bool button::get_ideal_size( size& out )const
{
	SIZE sz = {0,0};
	if( FALSE!=Button_GetIdealSize( handle_value(), &sz ) ) {
		out = make_size( sz );
		return true;
	}
	return false;
}
size button::ideal_size()const
{
	size dm;
	get_ideal_size(dm);
	return dm;
}
// イメージリスト (comctl6.0)
bool button::set_image_list( const button_image_list::value_type& pa )const
{
	return FALSE != Button_SetImageList( handle_value(), &pa );
}
bool button::get_image_list( button_image_list::value_type& out )const
{
	return FALSE != Button_GetImageList( handle_value(), &out );
}

#endif

/*--------------------------------------*/
// Vista later 
/*--------------------------------------*/
#if (_WIN32_WINNT >= 0x0600 )  /* Vista later */

// 管理者権限アイコン
bool button::add_elevated_icon()const
{
	LRESULT l = Button_SetElevationRequiredState( handle_value(),TRUE );
	return l==1;
}
bool button::remove_elevated_icon()const
{
	return 1==Button_SetElevationRequiredState( handle_value(),FALSE );
}
//
bool button::get_note( wide_string& out )const
{
	out.resize( note_length() );
	return FALSE != Button_GetNote( handle_value(),  buf_begin<wchar_t>(out), out.size() );
}
wide_string button::note()const
{
	wide_string str;
	get_note(str);
	return str;
}
bool button::set_note( wide_string_ptr s )const
{
	return FALSE!=Button_SetNote( handle_value(), s.c_str() );
}
//
size_t button::note_length()const
{
	return Button_GetNoteLength( handle_value() );
}

#endif // Vista later

void button::calc_best_size( size& )const
{/*
	DWORD sty = this->style();

	// 画像のみ
	if( sty & BS_BITMAP ) {

	}
	else if( sty & BS_ICON ) {

	}

	// 文字あり
	size txt_dim;
	string txt;
	this->get_text(txt);
	if( !txt.empty() )
	{
		// デバイスコンテキストからのサイズ割り出し
	}

	if( sty & BS_COMMANDLINK ) {
		out = txt_dim;
		return;
	}
*/
}


}} /* end of namespace oden.gammo */




