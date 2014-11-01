#include "../Pch.h"

#include <oden/gammo/common/string/extend_str_buf.h>

#include <oden/gammo/ui/control/list_box.h>

namespace oden { namespace gammo
{	
//#############################################################
// list_box
//
bool list_box_base::erase( index i )const
{
	return LB_ERR != ListBox_DeleteString( handle_value(), i.value() );
}
//
int list_box_base::count()const
{
	return ListBox_GetCount( handle_value() );
}
// クリア
void list_box_base::clear()const
{
	ListBox_ResetContent( handle_value());
}
// 
LPARAM list_box_base::item_data( index i )const
{
	return ListBox_GetItemData( handle_value(), i.value() );	
}
bool list_box_base::set_item_data( index i, LPARAM data )const
{
	return CB_ERR != ListBox_SetItemData( handle_value(), i.value(), data );
}
//
index list_box_base::top()const
{
	return ListBox_GetTopIndex( handle_value() );
}
bool list_box_base::set_top( index at )const
{
	return LB_ERR != ListBox_SetTopIndex( handle_value(), at.value() );
}

/*--------------------------------*/
// コントロール操作
/*--------------------------------*/
//
bool list_box_base::select( index i )const
{
	int ret = ListBox_SetCurSel( handle_value(), i.value() );
	
	return i==-1 || ret!=LB_ERR;
}
void list_box_base::deselect()const
{
	ListBox_SetCurSel( handle_value(), -1 );
}
//
bool list_box_base::selected( index at )const
{
	return 0!=ListBox_GetSel( handle_value(), at.value() );
}
//
index list_box_base::selection()const
{	
	return ListBox_GetCurSel( handle_value() );
}
//
int list_box_base::count_selection()const
{
	if( style() & LBS_MULTIPLESEL )
		return static_cast<int>( send_message( LB_GETSELCOUNT, 0, 0 ) );
	return 1;
}
//
integer list_box_base::get_selections( buffer_range<index> out )const
{
	return static_cast<int>( send_message( LB_GETSELITEMS, out.size(), reinterpret_cast<LPARAM>(out.get()) ) );
}
std::vector<index> list_box_base::selections()const
{
	std::vector<index> buf( count_selection() );
	get_selections( buf_range<index>(buf) );
	return buf;
}
//
bool list_box_base::select( index beg, index end )const
{
	return LB_ERR != send_message( LB_SELITEMRANGEEX, beg.value(), end.value() );
}
bool list_box_base::deselect( index beg, index end )const
{
	return LB_ERR != ListBox_SelItemRange( handle_value(), FALSE, beg.value(), end.value() ); 
}

index list_box_base::anchor()const
{
	return static_cast<int>( send_message( LB_GETANCHORINDEX, 0, 0 ) );
}
bool list_box_base::set_anchor( index at )const
{
	return 0 == send_message( LB_SETANCHORINDEX, at.value(), 0 );
}

int list_box_base::count_column_item()const
{
	return static_cast<int>( GetListBoxInfo( handle_value() ) );
}

/*-----------------------------------------*/
//  レイアウト
/*-----------------------------------------*/
//
integer list_box_base::item_height()const
{
	return item_height( 0 );
}
integer list_box_base::item_height( index i )const
{
	return static_cast<int>( send_message( LB_GETITEMHEIGHT, i.value(), 0 ) );
}
bool list_box_base::set_item_height( int h )const
{
	return set_item_height( 0, h );
}
bool list_box_base::set_item_height( index i, int h )const
{
	return LB_ERR != send_message( LB_SETITEMHEIGHT, i.value(), h );
}
//
integer list_box_base::selection_height()const
{
	return item_height( -1 );	
}
bool list_box_base::set_selection_height( int h )const
{
	return set_item_height( -1, h );
}
//
bool list_box_base::get_item_rect( index at, rect &out )const
{
	RECT rc;
	if( LB_ERR == ListBox_GetItemRect( handle_value(), at.value(), &rc ) )
		return false;

	out = make_rect( rc );
	return true;
}
rect list_box_base::item_rect( index at )const
{
	RECT rc;
	if( LB_ERR == ListBox_GetItemRect( handle_value(), at.value(), &rc ) )
		return rect();

	return make_rect(rc);
}


/*-----------------------------------------*/
//  コントロール
/*-----------------------------------------*/
// メモリ確保を予め行う・確保したアイテム数を返す
integer list_box_base::reserve( int add_item_num, int string_memory_size )const
{
	return static_cast<int>( send_message( LB_INITSTORAGE, add_item_num, string_memory_size ) );
}

index list_box_base::focused()const
{
	return ListBox_GetCaretIndex( handle_value() );
}
bool list_box_base::set_focus( index at )const
{
	return LB_ERR != ListBox_SetCaretIndex( handle_value(), at.value() );
}

bool list_box_base::add_file_list( string_ptr files, UINT attrs )const
{
	return LB_ERR != ListBox_Dir( handle_value(), attrs, files.c_str() );
}

/*-----------------------------------------*/
//  XP
/*-----------------------------------------*/
#if ( _WIN32_WINNT >= 0x0501 )	
#endif

/*-----------------------------------------*/
//  vista
/*-----------------------------------------*/
#if ( _WIN32_WINNT >= 0x0600 )
#endif

//##############################################################
//
//
/*--------------------------------*/
// 文字操作
/*--------------------------------*/
//
index list_box::add( string_ptr text )const
{
	return ListBox_AddString( handle_value(), text.c_str() );
}
index list_box::add( string_ptr text, LPARAM param )const
{
	index r = add( text );
	if( r )
		set_item_data( count()-1, param );
	return r;
}
// 挿入
index list_box::insert( index i, string_ptr text )const
{	
	return ListBox_InsertString( handle_value(), i.value(), text.c_str());
}
index list_box::insert( index i, string_ptr text, LPARAM param )const
{
	index ret = insert( i, text );
	if( ret )
		set_item_data( ret, param );
	return ret;
}
// アイテムを設定
bool list_box::set( index at, string_ptr text )const
{
	if( !this->empty() && at < this->count() )
	{
		bool bs = this->selected(at);
		if( index ni = this->insert(at, text) ) 
		{
			if( this->erase(ni+1) )
			{
				if(bs)
					this->select(at);
				return true;
			}
		}
	}
	return false;
}
bool list_box::set( index at, string_ptr text, LPARAM param )const
{
	return this->set( at, text ) && this->set_item_data( at, param );
}
//
index list_box::find_exact( string_ptr text, index start )const
{
	return ListBox_FindStringExact( handle_value(), start.value(), text.c_str() );
}
index list_box::find( string_ptr text, index start )const
{
	return ListBox_FindString( handle_value(), start.value(), text.c_str() );
}
//
bool list_box::get_item_text( index i, string& out )const
{
	out.resize( item_text_length(i) + 1 );
	return LB_ERR != ListBox_GetText( handle_value(), i.value(), buf_begin<char_t>(out) );
}
string list_box::item_text( index i )const
{
	string out;
	get_item_text( i, out );
	return out;	
}
//
size_t list_box::item_text_length( index i )const
{
	return ListBox_GetTextLen( handle_value(), i.value() );
}
//
index list_box::find_and_select( string_ptr text, index start )const
{
	return ListBox_SelectString( handle_value(), start.value(), text.c_str() );
}

//##############################################################
//
//
index data_list_box::add( LPARAM data )const
{
	return ListBox_AddItemData( handle_value(), data );
}
index data_list_box::insert( index i, LPARAM data )const
{
	return ListBox_InsertItemData( handle_value(), i.value(), data );
}
bool data_list_box::set( index at, LPARAM data )const
{
	bool bs = selected(at);
	if( this->erase( at ) && this->insert( at, data ) ) {
		if( bs ) {
			select(at);
			return true;
		}
	}
	return false;
}
index data_list_box::find( LPARAM data, index start )const
{
	return ListBox_FindItemData( handle_value(), start.value(), data );
}
index data_list_box::find_and_select( LPARAM data, index start )const
{
	return ListBox_SelectItemData( handle_value(), start.value(), data );
}

//
bool data_list_box::set_item_count( int num )const
{
	index ret = static_cast<int>( send_message( LB_SETCOUNT, num, 0 ) );
	return ret.is_valid();
}

} /* end of namespace gammo */
} /* end of namespace oden */
