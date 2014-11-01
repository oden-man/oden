#include "../Pch.h"

#include <oden/gammo/ui/control/static.h>

namespace oden { namespace gammo
{
//#############################################################
// static_ctl
//
/*--------------------------------*/
// ê›íË
/*--------------------------------*/
icon_handle static_ctl::set_icon( icon_handle ih )const
{
	return reinterpret_cast<HICON>( send_message( STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(ih.value()) ) );
}
bitmap_handle static_ctl::set_bitmap( bitmap_handle bh )const
{
	return reinterpret_cast<HBITMAP>( send_message( STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(bh.value()) ) );
}
cursor_handle static_ctl::set_cursor( cursor_handle ch )const
{
	return reinterpret_cast<HCURSOR>( send_message( STM_SETIMAGE, IMAGE_CURSOR, reinterpret_cast<LPARAM>(ch.value()) ) );
}
enhmetafile_handle static_ctl::set_enhmetafile( enhmetafile_handle emfh )const
{
	return reinterpret_cast<HENHMETAFILE>( send_message( STM_SETIMAGE, IMAGE_ENHMETAFILE, reinterpret_cast<LPARAM>(emfh.value()) ) );
}

/*--------------------------------*/
// éÊìæ
/*--------------------------------*/
icon_handle static_ctl::icon()const
{
	return reinterpret_cast<HICON>( send_message( STM_GETIMAGE, IMAGE_ICON, 0 ) );
}
bitmap_handle static_ctl::bitmap()const
{
	return reinterpret_cast<HBITMAP>( send_message( STM_GETIMAGE, IMAGE_BITMAP, 0 ) );
}
cursor_handle static_ctl::cursor()const
{
	return reinterpret_cast<HCURSOR>( send_message( STM_GETIMAGE, IMAGE_CURSOR, 0 ) );
}
enhmetafile_handle static_ctl::enhmetafile()const
{
	return reinterpret_cast<HENHMETAFILE>( send_message( STM_GETIMAGE, IMAGE_ENHMETAFILE, 0 ) );
}

}}


