#include "../Pch.h"

//
#include <oden/gammo/ui/menu.h>

namespace oden { namespace gammo
{
//###########################################################
//  メニューコントロール
//

//
bool menu::destroy()const
{
	return 0 != ::DestroyMenu( handle_value() );
}
menu_handle menu::sub_at( index pos )const
{
	return ::GetSubMenu( handle_value(), pos.value() );
}

positive menu::track_popup( window_handle owner, const point& screen_pt, UINT flag )const
{
	return ::TrackPopupMenu( handle_value(), flag, screen_pt.x(), screen_pt.y(), 0, owner.value(), 0 );
}
positive menu::track_popup( window_handle owner, const point& screen_pt, const rect& ex_rect, UINT flag )const
{
	TPMPARAMS tpm;
	tpm.cbSize    = sizeof(TPMPARAMS);
	tpm.rcExclude = get_rect(ex_rect);

	return ::TrackPopupMenuEx( handle_value(), flag, screen_pt.x(), screen_pt.y(), owner.value(), &tpm );
}

//
bool menu::insert( const menu_item_info::value_type& mifo, menu_item_id id )const
{
	if( !id )
		return 0 != ::InsertMenuItem( handle_value(), this->count().value(), TRUE, &mifo );
	return 0 != ::InsertMenuItem( handle_value(), id.value(), FALSE, &mifo );

/*
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_FTYPE | MIIM_STRING;
	mii.fType = MFT_STRING;
	mii.wID = 8000;
	mii.dwTypeData = const_cast<LPTSTR>(L"ファイル");

	InsertMenuItem( handle_value(), this->count().value(), TRUE, &mii );
	return true;
	*/
}
bool menu::insert_at( const menu_item::value_type& mifo, index at )const
{
	return 0 != ::InsertMenuItem( handle_value(), at.value(), TRUE, &mifo );
}

bool menu::set( const menu_item_info::value_type& mifo, index at )const
{
	if( at )
		return 0 != ::SetMenuItemInfo( handle_value(), at.value(), TRUE, &mifo );
	return 0 != ::SetMenuItemInfo( handle_value(), mifo.wID, FALSE, &mifo );
}
bool menu::get( menu_item_info::value_type& out, index at )const
{
	if( at )
		return 0 != ::GetMenuItemInfo( handle_value(), at.value(), TRUE, &out );
	return 0 != ::GetMenuItemInfo( handle_value(), out.wID, FALSE, &out );
}

bool menu::erase( menu_item_id id, bool by_index )const
{
	return 0 != ::DeleteMenu( handle_value(), id.value(), by_index ? MF_BYPOSITION : MF_BYCOMMAND );
}
bool menu::detach( menu_item_id id, bool by_index )const
{
	return 0 != ::RemoveMenu( handle_value(), id.value(), by_index ? MF_BYPOSITION : MF_BYCOMMAND );
}
bool menu::hilite( window_handle wh, menu_item_id id, bool on, bool by_index )const
{
	return 0 != ::HiliteMenuItem( wh.value(), handle_value(), id.value(), (on?MF_HILITE:MF_UNHILITE) | (by_index?MF_BYPOSITION:MF_BYCOMMAND) );
}
bool menu::set_default( menu_item_id id, bool by_index )const
{
	return 0 != ::SetMenuDefaultItem( handle_value(), id.value(), by_index?TRUE:FALSE );
}
menu_item_id menu::get_default( UINT flags, bool by_index )const
{
	return ::GetMenuDefaultItem( handle_value(), by_index?TRUE:FALSE, flags );
}

integer menu::count()const
{
	return ::GetMenuItemCount( handle_value() );
}

//###########################################################
//  メニュー作成関数
//
//  空のメニューをつくる
menu create_menu()
{
	return menu(menu_handle(::CreateMenu()));
}
menu create_popup_menu()
{
	return menu(menu_handle(::CreatePopupMenu()));
}

// リソースから読み出す
menu load_menu( res_id id, instance_handle ih )
{
	if( !ih )
		ih = get_app_handle();

	return menu(menu_handle(::LoadMenu( ih.value(), id.get() )));
}


} } /* end of namespace oden.gammo */



