
/*
* STL,boost��tchar�Ή��ɂ���typedef�W�B
*
*
*
*/
#pragma once

#ifndef ODEN_TCH_SSTREAM
#define ODEN_TCH_SSTREAM

#include <tchar.h>
#include <sstream>

namespace oden
{
	// strstream
	typedef std::basic_stringstream< TCHAR > tstringstream;
}

#endif
