#include "../Pch.h"

#include <oden/gammo/ui/control/list_view.h>
#include <oden/gammo/ui/control/header_ctl.h>

#include <oden/gammo/common/string/extend_str_buf.h>

namespace oden { namespace gammo
{
/*--------------------------------*/
//  LVGROUP
/*--------------------------------*/
//
/*

	// DescTop

	// DescBottom

	// SubSetTitle

	// TitleImageIndex
	if(TitleImageIndex!=-1){
		grp.mask|=LVGF_TITLEIMAGE;		
		grp.iTitleImage=TitleImageIndex;
	}

	// ExtendedImageIndex
	if(ExtendedImageIndex!=-1)
	{
		grp.mask|=LVGF_EXTENDEDIMAGE;		
		grp.iExtendedImage=ExtendedImageIndex;
	}
	*/


//###############################################################
// コントロール
//
/*-------------------------------------*/
// カラム
/*-------------------------------------*/
//
index list_view::insert( index at, const list_view_column::value_type &desc )const
{
	return ListView_InsertColumn( handle_value(), at.value(), &desc );
}
//
index list_view::append( const list_view_column::value_type &desc )const
{
	header_ctl head = header();
	if( !head.fail() )
		return insert( head.count(), desc );
	return boost::none;
}
//
bool list_view::erase_column( index iColm )const
{
	return ListView_DeleteColumn( handle_value(), iColm.value() )!=FALSE;
}
// データを取得
bool list_view::get( index iColm, list_view_column::value_type &out )const
{
	return TRUE==ListView_GetColumn( handle_value(), iColm.value(), &out );
}
// データをセット
bool list_view::set( index iColm, const list_view_column::value_type &desc )const
{
	return TRUE==ListView_SetColumn( handle_value(), iColm.value(), &desc );
}

// 
UINT list_view::column_width( index iColm )const
{
	return ListView_GetColumnWidth( handle_value(), iColm.value() );
}
// カラムの幅を設定
bool list_view::set_column_width( index iColm, UINT width )const
{
	return FALSE!=ListView_SetColumnWidth( handle_value(), iColm.value(), width );
}

//
bool list_view::get_column_order( std::vector<int> &out )const
{
	out.resize( this->header().count() );
	return FALSE!=ListView_GetColumnOrderArray( handle_value(), out.size(), buf_begin<int>(out) );
}
bool list_view::set_column_order( memory_range<int> out )const
{
	return FALSE==ListView_SetColumnOrderArray( handle_value(), out.size(), out.get() );
}

/*------------------------------------------*/
//  アイテム
/*------------------------------------------*/
//
index list_view::insert( const list_view_item::value_type& data )const
{	
	// 挿入
	return ListView_InsertItem( handle_value(), &data );
}
/*
bool list_view::insert_sub_item( const list_view_item::value_type& data )const
{
	// 挙動は全く同じ
	return this->set_item( data );
}*/

// 削除
bool list_view::erase( index iItem )const
{
	return FALSE!=ListView_DeleteItem( handle_value(), iItem.value() );
}
void list_view::clear()const
{
	ListView_DeleteAllItems( handle_value() );
}

// 取得
bool list_view::get( list_view_item::value_type& out )const
{
	return FALSE!=ListView_GetItem( handle_value(), &out );
}
string list_view::item_text( index i, index sub )const
{
	string str;
	while( extend_str_buf(str) ) {
		ListView_GetItemText( handle_value(), i.value(), sub.value(), buf_begin<char_t>(str), str.size() );
	}
	
	return str;
}
rect list_view::item_rect( index i, int code )const
{
	RECT rc={};
	ListView_GetItemRect( handle_value(), i.value(), &rc, code );
	
	return make_rect( rc );
}
rect list_view::item_rect( index i, index sub, int code )const
{
	RECT rc={};
	ListView_GetSubItemRect( handle_value(), i.value(), sub.value(), code, &rc );
	
	return make_rect( rc );
}

// 編集
bool list_view::set( const list_view_item::value_type &data )const
{
	// 挿入
	return FALSE!=ListView_SetItem( handle_value(), &data );
}
// アイテムの状態を設定
void list_view::set_item_state( index i, UINT mask, UINT val )const
{
	ListView_SetItemState( handle_value(), i.value(), mask, val );
}
void list_view::clear_item_state( index i, UINT mask )const
{
	set_item_state( i, mask, 0 );
}

//
int list_view::count()const
{
	return ListView_GetItemCount(handle_value());
}

//
index list_view::find( const list_view_find_info::value_type& cond, index start )const
{
	return ListView_FindItem( handle_value(), start.value(), &cond );
}

//
index list_view::next( UINT flags, index start )const
{
	return ListView_GetNextItem( handle_value(), start.value(), flags );
}

//
index list_view::hit_test( list_view_hittest_info::value_type& out, bool ex_mode )const
{
	if( ex_mode )
		return ListView_HitTestEx( handle_value(), &out );
	return ListView_HitTest( handle_value(), &out );
}
//
index list_view::sub_item_hit_test( list_view_hittest_info::value_type& out, bool ex_mode )const
{
	if( ex_mode )
		return ListView_SubItemHitTestEx( handle_value(), &out );
	return ListView_SubItemHitTest( handle_value(), &out );
}

/*-------------------------------------------------*/
//  グループ
/*-------------------------------------------------*/
//
index list_view::insert( const list_view_group::value_type& data, index iBefore )const
{
	// グループを作成
	return ListView_InsertGroup( handle_value(), iBefore.value(), &data );
}
// 
bool list_view::erase_group( index iGroup )const
{
	return FALSE!=ListView_RemoveGroup( handle_value(), iGroup.value() );
}
void list_view::clear_group()const
{
	ListView_RemoveAllGroups(handle_value());
}
//
int list_view::count_group()const
{
	return ListView_GetGroupCount(handle_value());
}
//
bool list_view::enable_group_view( bool b )const
{
	return FALSE!=ListView_EnableGroupView( handle_value(), b?TRUE:FALSE );
}

/*-------------------------------------------------*/
//  イメージリスト
/*-------------------------------------------------*/
//
bool list_view::set_image_list( image_list_handle imgh, int type )const
{
	return FALSE!=ListView_SetImageList( handle_value(), imgh.value(), type );
}
image_list_handle list_view::image_list( int type )const
{
	return ListView_GetImageList( handle_value(), type );
}
//
image_list_handle list_view::create_drag_image( index at, const point& pt )const
{
	POINT pnt = get_point( pt );
	return ListView_CreateDragImage( handle_value(), at.value(), &pnt );
}

/*-------------------------------------------------*/
//  その他
/*-------------------------------------------------*/
//
DWORD list_view::view_type()const
{
	return ListView_GetView( handle_value() );
}
bool list_view::set_view_type( DWORD val )const
{
	return 1==ListView_SetView( handle_value(), val );
}

bool list_view::arrange( int option )const
{
	return TRUE==ListView_Arrange( handle_value(), option );
}

size list_view::approximate_view_size( const size& proposed, integer itemNum )const
{
	dword_size dwm( ListView_ApproximateViewRect( handle_value(), proposed.width(), proposed.height(), itemNum.value() ) );
	return make_size( dwm );
}

void list_view::reserve( int number, DWORD ex_mode_option )const
{
	if( style() & LVS_OWNERDRAWFIXED )
		ListView_SetItemCountEx( handle_value(), number, ex_mode_option );
	else
		ListView_SetItemCount( handle_value(), number );
}

header_ctl list_view::header()const
{
	return ListView_GetHeader( handle_value() );
}

DWORD list_view::exstyle()const
{
	return ListView_GetExtendedListViewStyle( handle_value() );
}
void list_view::set_exstyle( DWORD style )const
{
	ListView_SetExtendedListViewStyle( handle_value(), style );
}
void list_view::set_exstyle( DWORD mask, DWORD style )const
{
	ListView_SetExtendedListViewStyleEx( handle_value(), mask, style );
}

void list_view::calc_best_size( size& out )const
{
	out = this->approximate_view_size();
}


}}


