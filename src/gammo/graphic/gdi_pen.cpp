#include "../Pch.h"

#include <oden/gammo/graphic/gdi/pen.h>

namespace oden { namespace gammo
{
//#############################################################
//    creator
//

// 
pen_handle create_pen( int style, int width, colorref col )
{
	return ::CreatePen( style, width, col.value() );
}

// Indirect
pen_handle create_pen( const log_pen::value_type& lop )
{
	return ::CreatePenIndirect( &lop );
}

// 
pen_handle ext_create_pen( DWORD style, DWORD width, const log_brush::value_type& lob, memory_range<DWORD> ptr )
{
	return ::ExtCreatePen( style, width, &lob, ptr.size(), ptr.get() );
}

// ’è‹`Ï‚İ‚Ìƒyƒ“‚ğæ“¾
pen_handle get_stock_pen( int id )
{
	return reinterpret_cast<HPEN>( ::GetStockObject( id ) );
}

} } /* end of namespace oden.gammo */
	