#include "../Pch.h"

#include <oden/gammo/graphic/gdi/font.h>

namespace oden { namespace gammo
{

//##############################################
// log_font_param
//

// �f�t�H���g�f�[�^��ݒ�

//##############################################
// logical_font
//
//

// �_���t�H���g
font_handle create_font( const log_font::value_type& lof )
{
	return ::CreateFontIndirect( &lof );
}

// �V�X�e���ɔ������Ă���t�H���g
font_handle get_stock_font( int id )
{
	return reinterpret_cast<HFONT>( ::GetStockObject( id ) );
}
		
}}
		
		
		