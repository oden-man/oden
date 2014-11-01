#pragma once

#ifndef ODEN_KONBU_DEFS
#define ODEN_KONBU_DEFS

#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>

//
// マクロ定義
//

// konbuが対応するいろいろな引数の最大数
#ifndef ODEN_KONBU_MAX_ARITY
#	define ODEN_KONBU_MAX_ARITY 10
#endif

// デフォルト
#ifndef ODEN_KONBU_CC
#	define ODEN_KONBU_CC (__cdecl)(__stdcall)(__fastcall)
#endif
#ifndef ODEN_KONBU_MF_CC
#	define ODEN_KONBU_MF_CC (__thiscall)
#endif

#if (6<BOOST_PP_SEQ_SIZE(ODEN_KONBU_CC)) || (6<BOOST_PP_SEQ_SIZE(ODEN_KONBU_MF_CC))
#	error "oden.konbu - Too many CC. The limit is 6."
#endif

namespace oden { namespace konbu
{
	namespace detail
	{
	  // 使われないテンプレート引数を示す
		struct unused_parameter{};
	}
}}

#endif
