#pragma once

#ifndef ODEN_GOB_GRA_IMGFMTS
#define ODEN_GOB_GRA_IMGFMTS

//#####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden ----------
#include <oden/property.h>

// --------- gammo -----------

//
//
//#################################################

namespace oden { namespace gammo
{
//
//  BMP 
//
//##############################################
//  BITMAPINFOHEADER
//
	ODEN_STRUCT( bitmap_info_header, BITMAPINFOHEADER )
	{	
		struct get
		{
			size   dimension		()const	{ return size( self::val(this).biWidth, self::val(this).biHeight ); }
			WORD   plane			()const { return self::val(this).biPlanes; }
			WORD   depth			()const { return self::val(this).biBitsCount; }
			DWORD  compression		()const { return self::val(this).biCompression; }
			DWORD  image_size		()const { return self::val(this).biSizeImage; }
		};

		struct set
		{
			Me& dimension( const size& dm ) 
			{
				self::val(this).biWidth  = dm.width();
				self::val(this).biHeight = dm.height();
				return self::me(this);
			}
			Me& depth( WORD val ) 
			{
				self::val(this).biBitsPixel = val;
				return self::me(this);			
			}
			Me& plane( WORD val ) 
			{
				self::val(this).biPlanes = val;
				return self::me(this);			
			}
		};	

		struct def 
		{
			def(  value_type& out )
			{
				out.biSize = sizeof( BITMAPINFOHEADER );
				out.biPlanes = 1;
			}
		};
	};
	
//##############################################
// BITMAPINFO 
//
	ODEN_STRUCT( bitmap_info, BITMAPINFO )
	{
		typedef ODEN_PROPERTY_EXTENDS< bitmap_info_header, property_member_value<BITMAPINFOHEADER, BITMAPINFO, &BITMAPINFO::bmiHeader> >
		extends;
		
		struct get : extends::get
		{
			bitmap_info_header
				::read_cref info_header()const			{ return self::val(this).bmiHeader; }
			const RGBQUAD* colors( int i = 0 )const		{ return self::val(this).bmiColors + i; } 	// パレットがある場合
			const void*	image()const					{ return self::val(this).bmiColors; }		// 画像データ
		};

		struct set : extends::set
		{
			Me& info_header( const bitmap_info_header::value_type& bmih ) 
			{
				self::val(this).bmiHeader = bmih.value();
				return self::me(this);
			}
		};

		struct def : extends::def
		{
			def( value_type& out ) : extends::def(out) {}
		};
	};
	
//##############################################
// BITMAPFILEHEADER 
//

//
// PNG
//

}}

#endif

