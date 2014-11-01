#include "../Pch.h"

#include <oden/gammo/ui/control/image_list.h>

namespace oden { namespace gammo
{
//###############################################################
// 構造体
//
/*--------------------------------*/
/*--------------------------------*/

//###############################################################
// コントロール
//
/*-------------------------------------*/
// ハンドル
/*-------------------------------------*/
//
bool image_list::create( const size& size, UINT flags, int initial, int grow )
{
	HIMAGELIST hNew = ImageList_Create( size.width(), size.height(), flags, initial, grow );
	if( hNew ) {
		attach(hNew);
		return true;
	}
	return false;
}
//
bool image_list::destroy( )
{
	if( FALSE!=ImageList_Destroy( handle_value() ) ) {
		attach(boost::none);
		return true;
	}
	return false;
}
//
image_list_handle image_list::duplicate()const
{
	return ImageList_Duplicate( handle_value() );
}

/*-------------------------------------------*/
//  イメージ
/*-------------------------------------------*/
//
index image_list::add( icon_handle ico )const
{
	return ImageList_AddIcon( handle_value(), ico.value() );
}
index image_list::add( bitmap_handle img, bitmap_handle mask )const
{
	return ImageList_Add( handle_value(), img.value(), mask.value() );
}
//
index image_list::replace( index at, icon_handle ico )const
{
	return ImageList_ReplaceIcon( handle_value(), at.value(), ico.value() );
}
index image_list::replace( index at, bitmap_handle img, bitmap_handle mask )const
{
	return ImageList_Replace( handle_value(), at.value(), img.value(), mask.value() );
}
//
bool image_list::erase( index at )const
{
	return FALSE!=ImageList_Remove( handle_value(), at.value() );
}
//
bool image_list::clear()const
{
	return 0!=ImageList_RemoveAll( handle_value() );
}
//
image_list_handle image_list::merged( image_list_handle l1, index at1, image_list_handle l2, index at2, const point& offset )const
{
	return ImageList_Merge( l1.value(), at1.value(), l2.value(), at2.value(), offset.x(), offset.y() );
}

//
bool image_list::draw(
			index at, 
			dc_handle dest, 
			const point& pt, 
			UINT style
	)const
{
	return FALSE!=::ImageList_Draw( handle_value(), at.value(), dest.value(), pt.x(), pt.y(), style );
}
bool image_list::draw(
			index at, 
			dc_handle dest, 
			const rect& rc, 
			colorref back, 
			colorref fore, 
			UINT style
	)const
{
	return FALSE!=::ImageList_DrawEx( handle_value(), at.value(), dest.value(), rc.left(), rc.top(), rc.width(), rc.height(), back.value(), fore.value(), style );
}
//
bool image_list::get_image_info( index at, image_list_imginfo::value_type& r )const
{
	return 0 != ImageList_GetImageInfo( handle_value(), at.value(), &r );
}
//
icon_handle image_list::create_icon( index at, UINT flags )const
{
	return ImageList_GetIcon( handle_value(), at.value(), flags );
}
//
size image_list::dimension()const
{
	int w = 0, h = 0;
	ImageList_GetIconSize( handle_value(), &w, &h );
	return size( w, h );
}
bool image_list::set_sizesion( const size& dm )const
{
	return FALSE!=ImageList_SetIconSize( handle_value(), dm.width(), dm.height() );
}
//
size_t image_list::count()const
{
	return static_cast<size_t>( ImageList_GetImageCount( handle_value() ) );
}
bool image_list::set_count( UINT cnt )const
{
	return FALSE!=ImageList_SetImageCount( handle_value(), cnt );
}

/*-------------------------------------------*/
//  ドラッグ
/*-------------------------------------------*/
//
image_list_handle image_list::current_drag_image( point* at, point* hotspot )
{
	POINT pt = {};
	POINT pt_hotspot = {};

	if( image_list_handle dih = ImageList_GetDragImage( &pt, &pt_hotspot ) )
	{
		if( at )		*at = make_point(pt);
		if( hotspot )	*hotspot = make_point(pt_hotspot);
		return dih;
	}
	return boost::none;
}
//
bool image_list::begin_drag( index at, const point& hotspot )const
{	
	return FALSE!=ImageList_BeginDrag( handle_value(), at.value(), hotspot.x(), hotspot.y() );
}
//
bool image_list::drag_enter( window_handle wh, const point &drag_pos)const
{
	return FALSE!=ImageList_DragEnter( wh.value(), drag_pos.x(), drag_pos.y() );
}
//
bool image_list::drag_move( const point &drag_pos )const
{
	return FALSE!=ImageList_DragMove( drag_pos.x(), drag_pos.y() );
}
//
bool image_list::drag_leave( window_handle wh )const
{
	return FALSE!=ImageList_DragLeave( wh.value() );
}
//
bool image_list::hide_drag_image( bool hide )const
{
	return FALSE!=ImageList_DragShowNolock( hide?FALSE:TRUE );
}
//
void image_list::end_drag_image()
{
	ImageList_EndDrag();
}

}}