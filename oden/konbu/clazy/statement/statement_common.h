#pragma once

#ifndef ODEN_KONBU_CLAZY_STATEMENT_COMMON
#define ODEN_KONBU_CLAZY_STATEMENT_COMMON

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/back.hpp>

// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"
#include "../detail/composite_expression.h"
#include <oden/konbu/clazy/primitive/defer.h>

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//##################################################################
// statement‹¤’Ê
//	
  // ƒeƒ“ƒvƒŒ[ƒgˆø”‚ğ‰ÁH - ––”ö‚ªBody
  	template< typename A, typename Operands, typename Extra >
  	struct custom_template_parameter< tag::statement<A>, Operands, Extra >
  	{
  	private:
  		typedef typename mpl::back< Operands >::type body_operand;
  	public:
  		typedef typename mpl::push_back<
  			typename mpl::pop_back< Operands >::type,
  			defer_<body_operand>						// body‚Ì•]‰¿‚ğ’x‰„‚·‚é
  		>::type
  		operands;
  		
  		typedef Extra extra;	
  	};
	
  // return_type - none
	template< typename A, typename Extra >
	struct return_type_static< tag::statement<A>, Extra >
	{
		typedef return_nothing type;
	};	
	
  // value_types - ‚·‚×‚Äas_any
	template< typename A, typename Operands, typename Context, typename Extra >
	struct value_types_raw< tag::statement<A>, Operands, Context, Extra >
		: public 
		detail::value_types_all< as_any_tag, Operands >
	{};

	namespace tag
	{
		template< typename T >
		struct statement_gen;
	}	
	
}
}} /* end of namespace gobou, oden */


#endif
