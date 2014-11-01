
/*
* STL,boostをtchar対応にするtypedef集。
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
