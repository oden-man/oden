#pragma once

#ifndef ODEN_KONBU_CLAZY_DT_ISNTTPT
#define ODEN_KONBU_CLAZY_DT_ISNTTPT

//#######################################
//
//

// ---------- boost.tt -----------
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/type_traits/decay.hpp>

#include <boost/type_traits/ice.hpp>

// ---------- oden ------------
// ---------- konbu -----------

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{

//#######################################################################
//  Tは非型テンプレート引数の型になれるか? (14.1.4)
//
namespace detail
{ 
	template< typename T >
	struct is_nttp_type
		: public boost::type_traits::ice_or<
					boost::is_integral<T>::value,
					boost::is_enum<T>::value,
					boost::is_pointer< typename boost::decay<T>::type >::value,
					boost::is_reference<T>::value,
					boost::is_member_pointer<T>::value 
				>
	{};
}
	
} /* end of namespace clazy */
}} /* end of namespace konbu, oden */

#endif




