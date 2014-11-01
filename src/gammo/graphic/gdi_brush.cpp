#include "../Pch.h"

#include <oden/gammo/graphic/gdi/brush.h>

#include <oden/gammo/common/color.h>

namespace oden { namespace gammo
{
//#############################################################
//    creator
//

// �P�F�u���V
brush_handle create_solid_brush( colorref color )
{
	return ::CreateSolidBrush( color.value() );
}

// �V�X�e���̃u���V���擾
brush_handle get_system_brush( DWORD col )
{
	return ::GetSysColorBrush( col );
}

// ��`�ς݂̃u���V���擾
brush_handle get_stock_brush( int id )
{
	return reinterpret_cast<HBRUSH>( ::GetStockObject( id ) );
}
			
// �͗l�p�^�[����`�悷��u���V
brush_handle create_hatch_brush( int hat, colorref line_color )
{
	return ::CreateHatchBrush( hat, line_color.value() ); 
}
  	
// �O���f�[�V������p����H ���@�̓t�@���N�^�Ŏw��H
  
// �r�b�g�}�b�v�p�^�[���ŕ`��
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
	