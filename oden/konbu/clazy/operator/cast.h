#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_CAST
#define ODEN_KONBU_CLAZY_OP_CAST

//#######################################
//
//
// ---------- STL -------------
// ---------- boost.pp --------
// ---------- boost -----------
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/if.hpp>
// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{	
//#######################################################################
// cast‹¤’Ê	
//	
  // 
	template< typename AnyAction, typename Dest >
	struct value_types_static< tag::cast<AnyAction>, mpl::vector1<Dest> >
	{
		typedef mpl::vector1< as_any_tag > type;
	};
	
  //
  	template< typename AnyAction, typename Dest >
  	struct return_type_static< tag::cast<AnyAction>, mpl::vector1<Dest> >
  	{
  		 typedef typename mpl::if_< boost::is_reference<Dest>,
  		 			return_lvalue<Dest>,
  		 			return_rvalue<Dest>
  		 			>::type
  		 		type;
  	};
	
//#######################################################################
// static_cast	
//	
	template< typename T >
	struct action_impl< tag::cast<tag::static_>, mpl::vector1<T> >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Args >
		Ret apply( Operand opr, const Args& )const {
			return static_cast<T>(opr);
		}
	};
	
//#######################################################################
// reinterpret_cast
//
	template< typename T >
	struct action_impl< tag::cast<tag::reinterpret_>, mpl::vector1<T> >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Args >
		Ret apply( Operand opr, const Args& )const {
			return reinterpret_cast<T>(opr);
		}
	};

//#######################################################################
// ƒNƒ‰ƒX
//	
  	#define CLAZY_EXPR 			CLAZY_UNARY_CALLABLE(static_cast_, tag::cast<tag::static_>)
	#define CLAZY_EXPR_EXTRA_T 	1
	#define CLAZY_SYMBOL		_static_cast
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 			CLAZY_UNARY_CALLABLE(reinterpret_cast_, tag::cast<tag::reinterpret_>)
	#define CLAZY_EXPR_EXTRA_T 	1
	#define CLAZY_SYMBOL		_reinterpret_cast
  	#include "../detail/pp_front.h"
	
}}}
#include "../detail/pp_front_end.h"
#endif



