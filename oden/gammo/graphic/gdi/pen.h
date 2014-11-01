#pragma once

#ifndef ODEN_GOB_GRA_GDI_PEN
#define ODEN_GOB_GRA_GDI_PEN

//#####################################################
//  �C���N���[�h
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
//   GDI�I�u�W�F�N�g�E�y��
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
		  // �X�^�C��
			Me& style( UINT sty ) 
			{ self::val(this).lopnStyle = sty; return self::me(this); }
		  // �F
			Me& color( colorref c ) 
			{ self::val(this).lopnColor = c.value(); return self::me(this); }
		  // ����
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
// �\�z�֐�
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