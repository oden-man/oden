#include "../pch.h"

#include <oden/gammo/common/error.h>

namespace oden { namespace gammo
{
//##################################################
// error
//

//
void win32_error::update()
{
	c_ = ::GetLastError();
}

//
string win32_error::format()const
{
	string buf;
	DWORD len = 0;

	while( extend_str_buf(buf,len) )
	{
		len = ::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			c_.value(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			 buf_begin<char_t>(buf),
			buf.size(),
			NULL );

		if( len == 0 )
			return _T("エラーメッセージの文章化に失敗");
	}

	return buf;
}

win32_error win32_error::last()
{
	return win32_error_code(::GetLastError());
}
void win32_error::set_last( win32_error_code c )
{
	::SetLastError( c.value() );
}

//######################################################
//
//

}}
