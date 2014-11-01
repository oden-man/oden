#include "../Pch.h"

#include <oden/gammo/ui/control/tree_view.h>

namespace oden { namespace gammo
{
//#######################################################
// TreeView
//

/*--------------------------------------------*/
// アイテム編集
/*--------------------------------------------*/
// ツリービューに挿入
tree_item_handle tree_view::insert( const tv_inserter::value_type &tvIns )const
{
	return TreeView_InsertItem( handle_value(), &tvIns );
}
bool tree_view::set( const tv_item::value_type& itm )const
{
	return TreeView_SetItem( handle_value(), &itm )!=FALSE;
}
bool tree_view::erase( tree_item_handle h )const
{
	return FALSE!=TreeView_DeleteItem( handle_value(), h.value() );
}
bool tree_view::clear()const
{
	return FALSE!=TreeView_DeleteAllItems( handle_value() );
}
bool tree_view::get_item( tv_item::value_type& out )const
{
	return FALSE!=TreeView_GetItem( handle_value(), &out );
}
int tree_view::count()const
{
	return TreeView_GetCount( handle_value() );
}

tree_item_handle tree_view::hit_test( tv_hittest_info::value_type& out )const
{
	// 指定された範囲にあるアイテムのハンドルを取得
	return TreeView_HitTest( handle_value(), &out );
}

bool tree_view::sort_child( tree_item_handle h, bool recursive )const
{
	return FALSE!=TreeView_SortChildren( handle_value(), h.value(), recursive?TRUE:FALSE );
}
bool tree_view::sort_child( tv_sort::value_type& desc, bool recursive )const
{
	return FALSE!=TreeView_SortChildrenCB( handle_value(), &desc, recursive?TRUE:FALSE );
}

/*--------------------------------------------*/
// アイテム取得
/*--------------------------------------------*/
tree_item_handle tree_view::get_next_item( UINT flag, tree_item_handle h )const
{
	return TreeView_GetNextItem( handle_value(), h.value(), flag );
}

tree_item_handle tree_view::selection()const
{
	return TreeView_GetSelection(handle_value());
}

tree_item_handle tree_view::root_item()const
{
	return TreeView_GetRoot(handle_value());
}

tree_item_handle tree_view::parent_item( tree_item_handle h )const
{
	return TreeView_GetParent( handle_value(), h.value() );
}

tree_item_handle tree_view::child_item( tree_item_handle h )const
{
	return TreeView_GetChild( handle_value(), h.value() );
}

tree_item_handle tree_view::prior_item( tree_item_handle h )const
{
	return TreeView_GetPrevSibling( handle_value(), h.value() );
}

tree_item_handle tree_view::next_item( tree_item_handle h )const
{
	return TreeView_GetNextSibling( handle_value(), h.value() );
}

/*--------------------------------------------*/
// アイテム状態
/*--------------------------------------------*/
//
window_handle tree_view::edit_label( tree_item_handle h )const
{
	return TreeView_EditLabel( handle_value(), h.value() );
}
bool tree_view::end_edit_label( bool bsave )const
{
	return FALSE!=TreeView_EndEditLabelNow( handle_value(), bsave?TRUE:FALSE );
}

bool tree_view::ensure_visible( tree_item_handle  h )const
{
	return FALSE!=TreeView_EnsureVisible( handle_value(), h.value() );
}

bool tree_view::select( tree_item_handle h, UINT flags )const
{
	return FALSE!=TreeView_Select( handle_value(), h.value(), flags );
}

bool tree_view::expand( tree_item_handle h, UINT flags )const
{
	return TreeView_Expand( handle_value(), h.value(), flags ) != FALSE;
}

bool tree_view::get_item_check( tree_item_handle h )const
{
	return 1==TreeView_GetCheckState( handle_value(), h.value() );
}
void tree_view::set_item_check( tree_item_handle h, bool b )const
{
	TreeView_SetCheckState( handle_value(), h.value(), b?TRUE:FALSE );
}

UINT tree_view::get_item_state( tree_item_handle h, UINT mask )const
{
	return TreeView_GetItemState( handle_value(), h.value(), mask );
}
void tree_view::set_item_state( tree_item_handle h, UINT mask, UINT state )const
{
	TreeView_SetItemState( handle_value(), h.value(), state, mask );
}

/*--------------------------------------------*/
// イメージリスト
/*--------------------------------------------*/
//
image_list_handle tree_view::set_image_list( int i, image_list_handle h )const
{
	return TreeView_SetImageList( handle_value(), h.value(), i );
}

image_list_handle tree_view::image_list( int i )const
{
	return TreeView_GetImageList( handle_value(), i );
}

image_list_handle tree_view::create_drag_image_list( tree_item_handle h )const
{
	return TreeView_CreateDragImage(handle_value(), h.value());	
}

/*--------------------------------------------*/
// その他
/*--------------------------------------------*/
//
colorref tree_view::set_line_color( colorref c )const
{
	return TreeView_SetLineColor( handle_value(), c.value() );
}
colorref tree_view::line_color()const
{
	return TreeView_GetLineColor( handle_value() );
}
colorref tree_view::set_back_color( colorref c )const
{
	return TreeView_SetBkColor( handle_value(), c.value() );
}
colorref tree_view::back_color()const
{
	return TreeView_GetBkColor( handle_value() );
}
colorref tree_view::set_text_color( colorref c )const
{
	return TreeView_SetTextColor( handle_value(), c.value() );
}
colorref tree_view::text_color()const
{
	return TreeView_GetTextColor( handle_value() );
}

//
bool tree_view::insert_mark( tree_item_handle h, bool after )const
{
	return FALSE!=TreeView_SetInsertMark( handle_value(), h.value(), after?TRUE:FALSE );
}
colorref tree_view::mark_color()const
{
	return TreeView_GetInsertMarkColor( handle_value() );
}
colorref tree_view::set_mark_color( colorref c )const
{
	return TreeView_SetInsertMarkColor( handle_value(), c.value() );
}

//
DWORD tree_view::extended_style()const
{
	return TreeView_GetExtendedStyle( handle_value() );
}
hresult tree_view::set_extended_style( DWORD mask, DWORD style )const
{
	return TreeView_SetExtendedStyle( handle_value(), style, mask );
}


}}


