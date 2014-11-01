#include "../Pch.h"
#include <oden/gammo/common/string/extend_str_buf.h>

#include <oden/gammo/ui/control/rich_edit.h>

namespace oden { namespace gammo
{
//#############################################################
// rich_edit
//
module_handle rich_edit::load_library( int var )
{
	gammo::string path( ::GetSystemDirectory(0,0), 0xfc );
	::GetSystemDirectory( buf_begin<char_t>(path), path.size() );
	path.resize( path.size()-1 );

	switch( var )
	{
	case 1:
		path += _T("\\riched32.dll");
		break;
	case 2:
	case 3:
		path += _T("\\riched20.dll");
		break;
	default:
		return boost::none;
	}

	return ::LoadLibrary( path.c_str() );
}

/*---------------------------------*/
// 
/*---------------------------------*/
//
bool rich_edit::set_char_format( UINT target, const char_format::value_type& fmt )const
{
	return 0 != send_message( EM_SETCHARFORMAT, target, reinterpret_cast<LPARAM>(&fmt) );
}

colorref rich_edit::set_background( colorref c )const
{
	return send_message( EM_SETBKGNDCOLOR, c ? 0 : 1, c.value() );
}

int rich_edit::input( edit_stream::value_type& stm, UINT opt )const
{
	return send_message( EM_STREAMIN, opt, reinterpret_cast<LPARAM>(&stm) );
}
int rich_edit::output( edit_stream::value_type& stm, UINT opt )const
{
	return send_message( EM_STREAMOUT, opt, reinterpret_cast<LPARAM>(&stm) );
}

}}


