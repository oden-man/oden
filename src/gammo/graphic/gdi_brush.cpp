#include "../Pch.h"

#include <oden/gammo/graphic/gdi/brush.h>

#include <oden/gammo/common/color.h>

namespace oden { namespace gammo
{
//#############################################################
//    creator
//

// 単色ブラシ
brush_handle create_solid_brush( colorref color )
{
	return ::CreateSolidBrush( color.value() );
}

// システムのブラシを取得
brush_handle get_system_brush( DWORD col )
{
	return ::GetSysColorBrush( col );
}

// 定義済みのブラシを取得
brush_handle get_stock_brush( int id )
{
	return reinterpret_cast<HBRUSH>( ::GetStockObject( id ) );
}
			
// 模様パターンを描画するブラシ
brush_handle create_hatch_brush( int hat, colorref line_color )
{
	return ::CreateHatchBrush( hat, line_color.value() ); 
}
  	
// グラデーションを用いる？ 方法はファンクタで指定？
  
// ビットマップパターンで描画
brush_handle create_pattern_brush( bitmap_handle bmh )
{
	return ::CreatePatternBrush( bmh.value() );
}

// Indirect
brush_handle create_brush( const log_brush::value_type& data )
{
	return CreateBrushIndirect( &data );
}

namespace { void test()
{
	brush b = get_stock_brush(2);
	gdi_obj_handle goh = b.base_handle();
	brush_handle bh = b;

	if(!b)
		b.destroy();
} }

} } /* end of namespace oden.gammo */
	