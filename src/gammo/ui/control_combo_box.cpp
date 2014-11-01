#include "../Pch.h"

#include <oden/gammo/common/string/extend_str_buf.h>

#include <oden/gammo/ui/control/combo_box.h>

namespace oden { namespace gammo
{	
//#############################################################
// combo_box_base
//
/*--------------------------------*/
// 文字操作
/*--------------------------------*/
// 
LPARAM combo_box_base::item_data( index i )const
{
	return ComboBox_GetItemData( handle_value(), i.value() );	
}
bool combo_box_base::set_item_data( index i, LPARAM data )const
{
	return CB_ERR != ComboBox_SetItemData( handle_value(), i.value(), data );
}
//
int combo_box_base::count()const
{
	return ComboBox_GetCount( handle_value() );
}
//
bool combo_box_base::erase( index i )const
{
	return CB_ERR != ComboBox_DeleteString( handle_value(), i.value() );
}
// クリア
void combo_box_base::clear()const
{
	ComboBox_ResetContent( handle_value());
}

/*--------------------------------*/
// コントロール操作
/*--------------------------------*/
//
bool combo_box_base::select( index i )const
{
	int ret = ComboBox_SetCurSel( handle_value(), i.value() );
	
	return i==-1 || ret!=CB_ERR;
}
void combo_box_base::deselect()const
{
	ComboBox_SetCurSel( handle_value(), -1 );
}
//
index combo_box_base::selection()const
{	
	return ComboBox_GetCurSel( handle_value() );
}

//
void combo_box_base::set_edit_limit( int num )const
{
	ComboBox_LimitText( handle_value(), num );
}
//
integer combo_box_base::reserve( int add_item_num, int string_memory_size )const
{
	return static_cast<int>( send_message( CB_INITSTORAGE, add_item_num, string_memory_size ) );
}

//
integer combo_box_base::item_height()const
{
	return item_height( 0 );	
}
integer combo_box_base::item_height( index i )const
{
	return static_cast<int>( send_message( CB_GETITEMHEIGHT, i.value(), 0 ) );
}
bool combo_box_base::set_item_height( int h )const
{
	return set_item_height( 0, h );
}
bool combo_box_base::set_item_height( index i, int h )const
{
	return CB_ERR != send_message( CB_SETITEMHEIGHT, i.value(), h );
}
//
integer combo_box_base::selection_height()const
{
	return item_height( -1 );	
}
bool combo_box_base::set_selection_height( int h )const
{
	return set_item_height( -1, h );
}

//
void combo_box_base::show_drop_down( bool b )const
{
	ComboBox_ShowDropdown( handle_value(), b?TRUE:FALSE );
}
//
bool combo_box_base::drop_downed()const
{
	return FALSE != ComboBox_GetDroppedState( handle_value() );
}

/*-----------------------------------------*/
//  XP
/*-----------------------------------------*/
#if ( _WIN32_WINNT >= 0x0501 )	
// 
INT combo_box_base::visible_item_limit()const
{
	return ComboBox_GetMinVisible( handle_value() );
}
bool combo_box_base::set_visible_item_limit( INT num )const
{
	return TRUE==ComboBox_SetMinVisible( handle_value(), num );
}
#endif

/*-----------------------------------------*/
//  vista
/*-----------------------------------------*/
#if ( _WIN32_WINNT >= 0x0600 )
// 
bool combo_box_base::get_cue_banner( wide_string& out )const
{
	if( out.empty() )
		out.resize(32);
	do {
		if( FALSE==ComboBox_GetCueBannerText( handle_value(), buf_begin<wchar_t>(out), out.size() ) )
			return false;		
	} 
	while( extend_str_buf(out) );
	
	return true;
}
wide_string combo_box_base::cue_banner()const
{
	wide_string out;
	get_cue_banner( out );	
	return out;
}
bool combo_box_base::set_cue_banner( wide_string_ptr text )const
{
	return TRUE==ComboBox_SetCueBannerText( handle_value(), text.c_str() );	
}
#endif

//#############################################################
// combo_box
//
/*--------------------------------*/
// 文字操作
/*--------------------------------*/
//
void combo_box::add( string_ptr text )const
{
	ComboBox_AddString( handle_value(), text.c_str() );
}
// 挿入
index combo_box::insert( index i, string_ptr text )const
{	
	return ComboBox_InsertString( handle_value(), i.value(), text.c_str());
}
//
index combo_box::find_exact( string_ptr text, index start )const
{
	return ComboBox_FindStringExact( handle_value(), start.value(), text.c_str() );
}
index combo_box::find( string_ptr text, index start )const
{
	return ComboBox_FindString( handle_value(), start.value(), text.c_str() );
}
//
bool combo_box::get_item_text( index i, string& out )const
{
	out.resize( item_text_length(i)+1 );
	return CB_ERR!=ComboBox_GetLBText( handle_value(), i.value(),  buf_begin<char_t>(out) );
}
string combo_box::item_text( index i )const
{
	string out;
	get_item_text( i, out );
	return out;	
}

//
size_t combo_box::item_text_length( index i )const
{
	return ComboBox_GetLBTextLen( handle_value(), i.value() );
}
//
index combo_box::find_and_select( string_ptr text, index start )const
{
	return ComboBox_SelectString( handle_value(), start.value(), text.c_str() );
}

//#########################################################################
//  data_combo_box
//
// テキストを末尾に追加
void data_combo_box::add( LPARAM data )const
{
	ComboBox_AddItemData( handle_value(), data );
}
// アイテムを挿入
index data_combo_box::insert( index at, LPARAM data )const
{
	return ComboBox_InsertItemData( handle_value(), at.value(), data );
}
// テキストを検索
index data_combo_box::find( LPARAM data, index start )const
{
	return ComboBox_FindItemData( handle_value(), start.value(), data );
}
//
index data_combo_box::find_and_select( LPARAM data, index start )const
{
	return ComboBox_SelectItemData( handle_value(), start.value(), data );
}



} /* end of namespace gammo */
} /* end of namespace oden */
