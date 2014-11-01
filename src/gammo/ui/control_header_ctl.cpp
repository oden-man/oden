#include "../Pch.h"

#include <oden/gammo/ui/control/header_ctl.h>

namespace oden { namespace gammo
{
//###############################################################
// コントロール
//
/*------------------------------------------*/
//  アイテム
/*------------------------------------------*/
//
index header_ctl::insert( index at, const header_ctl_item::value_type& data )const
{	
	// 挿入
	return Header_InsertItem( handle_value(), at.value(), &data );
}
// 削除
bool header_ctl::erase( index iItem )const
{
	return FALSE!=Header_DeleteItem( handle_value(), iItem.value() );
}
// 取得
bool header_ctl::get_item( index at, header_ctl_item::value_type& out )const
{
	return FALSE!=Header_GetItem( handle_value(), at.value(), &out );
}
// 編集
bool header_ctl::set( index at, const header_ctl_item::value_type &data )const
{
	// 挿入
	return FALSE!=Header_SetItem( handle_value(), at.value(), &data );
}
//
size_t header_ctl::count()const
{
	return Header_GetItemCount( handle_value() );
}

//
index header_ctl::hit_test( header_ctl_hittest_info::value_type& out )const
{
	return static_cast<int>( send_message( HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&out) ) );
}

/*-------------------------------------------------*/
//  イメージリスト
/*-------------------------------------------------*/
//
bool header_ctl::set_image_list( image_list_handle imgh )const
{
	return FALSE!=Header_SetImageList( handle_value(), imgh.value() );
}
image_list_handle header_ctl::image_list()const
{
	return Header_GetImageList( handle_value() );
}

/*-------------------------------------------------*/
//  その他
/*-------------------------------------------------*/
//





}}


