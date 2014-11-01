#pragma once

#ifndef ODEN_GOB_GRA_GDI_PEN
#define ODEN_GOB_GRA_GDI_PEN

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
#include <oden/gammo/graphic/gdi/brush.h>

//
//
//#################################################


namespace oden { namespace gammo
{
//_______________________________________________________________________________________
//
//   GDIオブジェクト・ペン
//_______________________________________________________________________________________
//
//##############################################
// LOGPEN
//
	ODEN_STRUCT( log_pen, LOGPEN )
	{
		struct get
		{};

		struct set
		{
		  // スタイル
			Me& style( UINT sty ) 
			{ self::val(this).lopnStyle = sty; return self::me(this); }
		  // 色
			Me& color( colorref c ) 
			{ self::val(this).lopnColor = c.value(); return self::me(this); }
		  // 太さ
			Me& width( int w )
			{ self::val(this).lopnWidth.x = w; return self::me(this); }
		};
 	};
	
//##############################################
//  EXTLOGPEN
// 
	ODEN_STRUCT( ext_log_pen, EXTLOGPEN )
	{
		struct get
		{
			DWORD 	 style()const { return self::val(this).elpPenStyle; }
			colorref color()const { return self::val(this).elpColor; }
			DWORD	 width()const { return self::val(this).elpWidth; }
			UINT 	 brush_style()const { return self::val(this).elpBrushStyle; }
			DWORD	 hatch_pattern()const { return self::val(this).elpHatch; }
			
			memory_range<DWORD> user_style()const 
			{ return memory_range<DWORD>( self::val(this).elpStyleEntry, self::val(this).elpNumEntries ); }
		};

		struct set
		{};
 	};

//##############################################
// 構築関数
//
  // 
	pen_handle create_pen( int style, int width, colorref col );
  // Indirect
	pen_handle create_pen( const log_pen::value_type& lop );
  // 
	pen_handle ext_create_pen( DWORD style, DWORD width, const log_brush::value_type& lob, memory_range<DWORD> ptr = boost::none );  	
  //
	pen_handle get_stock_pen( int id );

}}

#endif