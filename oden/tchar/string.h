
/*
* STL,boost��tchar�Ή��ɂ���typedef�W�B
*
*
*
*/
#pragma once

#ifndef ODEN_TCH_STRING
#define ODEN_TCH_STRING

#include <tchar.h>
#include <string>

namespace oden
{
	// string
	typedef std::basic_string< TCHAR > tstring;
}

#endif
