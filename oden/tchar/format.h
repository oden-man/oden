
/*
* STL,boostをtchar対応にするtypedef集。
*
*
*
*/
#pragma once

#ifndef ODEN_TCH_BOOSTFORMAT
#define ODEN_TCH_BOOSTFORMAT

#include <tchar.h>
#include <boost/format.hpp>

namespace oden
{	
	// boost.format
	typedef boost::basic_format< TCHAR > tformat;
}

#endif
