#include "../Pch.h"

#include <oden/gammo/graphic/gdi/bitmap.h>


namespace oden { namespace gammo
{
//______________________________________________________________________________________________
//
//    controller
//______________________________________________________________________________________________
//
//#############################################################
// 
//

//______________________________________________________________________________________________
//
//    creator
//______________________________________________________________________________________________
//

//
bitmap_handle load_bitmap( string_ptr name, const size& ds, DWORD option )
{
	return reinterpret_cast<HBITMAP>( ::LoadImage( NULL, name.c_str(), IMAGE_BITMAP, ds.width(), ds.height(), option|LR_LOADFROMFILE ) );
}
bitmap_handle load_bitmap( const res_id& name, const size& ds, DWORD option )
{
	return reinterpret_cast<HBITMAP>( ::LoadImage( NULL, name.get(), IMAGE_BITMAP, ds.width(), ds.height(), option ) );
}
// 
bitmap_handle create_compatible_bitmap( dc_handle dh, const size& size )
{
	return ::CreateCompatibleBitmap( dh.value(), size.width(), size.height() );
}




}}