#include "../Pch.h"

#include <oden/gammo/common/string/conv.h>

namespace oden { namespace gammo
{	
//#######################################################
//  •ÏŠ·ŠÖ”
//

// string->wstring‚É•ÏŠ·
bool widen( wide_string& wstr, const narrow_string& c )
{
	UINT cp = ::GetACP();

	int sz = ::MultiByteToWideChar( cp, 0, c.c_str(), c.size(), 0, 0 );
	if( sz == 0 )
		return false;

	wstr.resize(sz); 
	::MultiByteToWideChar( cp, 0, c.c_str(), c.size(), &wstr[0], wstr.size() );

	return true;
}

// 
bool narrow( narrow_string& str, const wide_string& w )
{
	UINT cp = ::GetACP();
	
	int sz = ::WideCharToMultiByte( cp, 0, w.c_str(), w.size(), 0, 0, 0, 0 );
	if( sz == 0 )
		return false;

	str.resize(sz); 
	::WideCharToMultiByte( cp, 0, w.c_str(), w.size(), &str[0], str.size(), 0, 0 );

	return true;
}

}}

