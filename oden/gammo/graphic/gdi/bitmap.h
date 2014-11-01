#pragma once

#ifndef ODEN_GOB_GRA_GDI_BITMAP
#define ODEN_GOB_GRA_GDI_BITMAP

//#####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
// --------- gammo -----------
#include <oden/gammo/graphic/gdi/object.h>
#include <oden/gammo/graphic/image_format.h>

//
//
//#################################################

namespace oden { namespace gammo
{
//_______________________________________________________________________________________
//
//   構造体
//_______________________________________________________________________________________
//
//##############################################
// BITMAP
//
	ODEN_STRUCT( log_bitmap, BITMAP )
	{
		struct get
		{
			size  dimension		()const;
			LONG   bytes_in_width	()const;
			WORD   plane			()const;
			WORD   bit_count		()const;	// DDB:ディスプレイの色数となる
			void*  bit_begin		()const;	// DDB:?
		};

		struct set
		{
			Me& dimension( const size& dm ) {
				self::val(this).bmWidth = dm.width();
				self::val(this).bmHeight = dm.height();
				return self::me(this);
			}
			Me& bit_count( WORD val ) {
				self::val(this).bmBitsPixel = val;
				return self::me(this);			
			}
			Me& bit_data( buffer_range<BYTE> ptr ) {
				self::val(this).bmBits = ptr.get();
				return self::me(this);			
			}
		};

	};
	
//##############################################
// 情報クラス ( DIB )
//
	ODEN_STRUCT( dib_section, DIBSECTION )
	{
		typedef ODEN_PROPERTY_EXTENDS< log_bitmap, property_member_value<BITMAP, DIBSECTION, &DIBSECTION::dsBm> >
		extends_log;

		typedef ODEN_PROPERTY_EXTENDS< bitmap_info_header, property_member_value<BITMAPINFOHEADER, DIBSECTION, &DIBSECTION::dsBmih> >
		extends_header;

		struct get : extends_log::get, extends_header::get
		{
			log_bitmap::read_cref 			bitmap()		const { return self::val(this).dsBm; }
			bitmap_info_header::read_cref 	info_header()	const { return self::val(this).dsBmih; }

			DWORD 	red_bit_field	()const { return self::val(this).dsBitFields[0]; }
			DWORD 	green_bit_field	()const { return self::val(this).dsBitFields[1]; }
			DWORD 	blue_bit_field	()const { return self::val(this).dsBitFields[2]; }
			DWORD  	offset			()const { return self::val(this).dsOffset; }
			file_mapping_handle 
					section			()const { return self::val(this).dshSection; }
		};

		struct set : extends_log::set, extends_header::set
		{};
	
		struct def : extends_log::def, extends_header::def
		{
			def( value_type& out )
				: extends_log::def(out), extends_header::def(out)
			{}
		};
	}; 
	
//_______________________________________________________________________________________
//
//   GDIオブジェクト・ビットマップ
//_______________________________________________________________________________________
//  
//##############################################
//  共通クラス
//
  // DIBまたはDDB
	class bitmap
		: public gdi_obj<bitmap_handle>
	{
	public:
	// コンストラクタ
		bitmap( bitmap_handle bh = boost::none )
			: gdi_obj<bitmap_handle>( bh )
		{}

	// メンバ関数
	  // DIBか？DDBか？
	    bool independent()const;
	    bool compatible()const{ return !independent(); }
	
	};
	
//##############################################
// 構築
//
  // ファイル/リソースからロード
	bitmap_handle load_bitmap( 
		string_ptr 	 name, 
		const size& desired_size, 
		DWORD 		 lr_option = LR_DEFAULTCOLOR 
	);	
	bitmap_handle load_bitmap( 
		const res_id& name, 
		const size&  desired_size, 
		DWORD 		  lr_option = LR_DEFAULTCOLOR 
	);
	
  //  空のDDBを作成
	bitmap_handle create_compatible_bitmap( 
		dc_handle dh,
		const size& size
	);

}}

#endif

