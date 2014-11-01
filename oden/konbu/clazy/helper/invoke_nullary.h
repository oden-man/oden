#pragma once

#ifndef ODEN_KONBU_CLAZY_HELPER_INVOKE_NULLARY
#define ODEN_KONBU_CLAZY_HELPER_INVOKE_NULLARY

//#######################################
//
//

// ---------- boost.tt -----------
// ---------- oden ------------
// ---------- konbu -----------
// ---------- clazy -----------
#include <oden/konbu/clazy/detail/expression_traits.h>

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
	namespace detail
	{
		struct invoke_nullary_dummy_parameter
		{};
	}
	
//#######################################################################
//  à¯êîÇ»ÇµÇ∆ÇµÇƒåƒÇ—èoÇµÇÇ∑ÇÈ
//
	template< typename F >
	typename F::template traits< 
		typename detail::initial_context< fusion::vector1<detail::invoke_nullary_dummy_parameter&> >::type
	>::result_as_terminal
	invoke_nullary( const F& func )
	{
		detail::invoke_nullary_dummy_parameter p_;
		return func(p_);
	}
	
} /* end of namespace clazy */
}} /* end of namespace konbu, oden */

#endif




