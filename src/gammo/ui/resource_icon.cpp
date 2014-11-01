#include "../Pch.h"
	
#include <oden/gammo/ui/resource/icon.h>
//#include <oden/gammo/graphic/gdi/bitmap.h>

//#include <oden/gammo/ui/resource/resource.h>

namespace oden { namespace gammo
{
//______________________________________________________________________________________________
//
//
//    control
//
//______________________________________________________________________________________________
//		
//#############################################################
// icon
//

// ビットマップの取得
bitmap_handle icon::color_bitmap()const
{
	icon_info::read_val info;
	get_info( info );
    return info.color_bitmap();
}
bitmap_handle icon::mask_bitmap()const
{
	icon_info::read_val info;
	get_info( info );
    return info.mask_bitmap();
}

/*
// 取得―サイズ
size icon::dimension()const
{	
	// ビットマップを取得
	scoped_bitmap bmp;
	if( !(bmp = color_bitmap()) && !(bmp = mask_bitmap()) )
	    return size(0,0);
	
	logical_bitmap_info binfo;
	bmp->get_info( binfo );
	
	// 高さが二倍なのを修正
	return binfo.dimension().scaled( 1.0f, 0.5f );
}
*/

// アイコンを破棄する
bool icon::destroy( )
{
	if( FALSE!=::DestroyIcon( handle_value() ) ) 
		return true;
	return false;
}

// アイコンを複製する
icon_handle icon::duplicate( )const
{
	return ::DuplicateIcon( NULL, handle_value() );
}

// 別のモジュールへアイコンをコピーする
icon_handle icon::copy( )const
{
	return ::CopyIcon( handle_value() ); 	
}

// アイコン情報をコピー
bool icon::get_info( icon_info::value_type& out )const
{
	return FALSE != ::GetIconInfo( handle_value(), &out );
}
/*
bool icon::get_info( icon_info_ex::viewer& out )const
{
	return ::GetIconInfoEx( get(), &out.get() );
}
*/

//  アイコンを描画
bool icon::draw(
	dc_handle 		to_dc,
	const rect& 	place, 
	brush_handle 	offscreen_brush, 
	UINT 			draw_option
	)const
{
	return FALSE != ::DrawIconEx( 
		to_dc.value(), 
		place.left(),
		place.top(),
		this->handle_value(),
		place.width(),
		place.height(),
		0, 
		offscreen_brush.value(),
		draw_option
	);
}

//_____________________________________________________________________________________________________________
//
//
//    構築ゥ
//
//_____________________________________________________________________________________________________________
//
//#############################################################
// ファイル名・リソース名からロード
//
// アイコンをファイル名から読む
icon_handle load_icon( 
	string_ptr 		path, 
	UINT 			option, 
	const size& 	desired_size )
{
	return reinterpret_cast< HICON >( 
		::LoadImage( 
			NULL, 
			path.c_str(), 
			IMAGE_ICON, 
			desired_size.width(), desired_size.height(),
			option | LR_LOADFROMFILE 
		)
	);		
}

// アイコンをリソース名から読む
icon_handle load_icon( 
	module_handle modh, 
	const res_id& rid, 
	UINT option, 
	const size& desired_size )
{
	return reinterpret_cast< HICON >( ::LoadImage(modh.value(), rid.get(), IMAGE_ICON, desired_size.width(), desired_size.height(), option) );
}
/*
icon load_icon( 
	string_ptr 					path,
	const res_id& 				rid, 
	UINT 	opt, 
	const size& 				desired_size )
{
	scoped<module> mod( path );
	return load_icon( mod.get(), rid, opt, desired_size );
}*/

//#############################################################
// リソースデータから構築
//
// リソースデータからアイコンを作成
icon create_icon_resource (
	buffer_range<BYTE> bits,
	const size& desired_size,
	UINT opt )
{
	return ::CreateIconFromResourceEx(
		bits.get(), bits.size(),
		TRUE,
		0x00030000,
		desired_size.width(), desired_size.height(),
		opt
	);
}

// リソースから指定の大きさのアイコンを取得
res_id lookup_icon_resource (
	buffer_range<BYTE> bits,
	const size& desired_size,
	UINT opt )
{
	return ::LookupIconIdFromDirectoryEx(
		bits.get(),
		TRUE,
		desired_size.width(), desired_size.height(),
		opt
	);
}

//#############################################################
// システム・アイコン
//

// システムのアイコンをロードする
icon_handle load_system_icon( LPCTSTR icoid ) 
{
	return ::LoadIcon( NULL, icoid );	
}

icon_handle load_system_icon( LPCTSTR icoid, UINT option, const size& desired_size )
{
	return reinterpret_cast< HICON >( 
		::LoadImage( 
			NULL,
			icoid, 
			IMAGE_ICON, 
			desired_size.width(), desired_size.height(), 
			option | LR_SHARED 
		)
	);
}


} /* end of namespace gammo */
} /* end of namespace oden */






