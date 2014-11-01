
/*
* STL,boost‚ğtchar‘Î‰‚É‚·‚étypedefWB
*
*
*
*/
#pragma once

#ifndef ODEN_TCH_REGEX
#define ODEN_TCH_REGEX

#include <oden/tchar/string.h>
#include <regex>

namespace oden
{
	// regex
	typedef std::basic_regex< TCHAR > 						tregex;
	typedef std::match_results< const TCHAR* > 				tcmatch;
	typedef std::match_results< tstring::const_iterator > 	tsmatch;
	typedef std::sub_match< const TCHAR* > 					tcsub_match;
	typedef std::sub_match< tstring::const_iterator > 		tssub_match;
}

#endif
