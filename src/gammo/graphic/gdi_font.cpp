#include "../Pch.h"

#include <oden/gammo/graphic/gdi/font.h>

namespace oden { namespace gammo
{

//##############################################
// log_font_param
//

// デフォルトデータを設定

//##############################################
// logical_font
//
//

// 論理フォント
font_handle create_font( const log_font::value_type& lof )
{
	return ::CreateFontIndirect( &lof );
}

// システムに備えられているフォント
font_handle get_stock_font( int id )
{
	return reinterpret_cast<HFONT>( ::GetStockObject( id ) );
}
		
}}
		
		
		