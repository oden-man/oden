#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_COMMA
#define ODEN_KONBU_CLAZY_OP_COMMA

//#######################################
//
//
// ---------- STL -------------
// ---------- boost.pp --------
// ---------- boost -----------
#include <boost/type_traits/is_reference.hpp>
// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//#######################################################################
// comma
//	
  // sideness
	template<>
	struct value_types_static< tag::comma >
	{
		typedef mpl::vector2< as_any_tag, as_any_tag > type;
	};
	
  // return_type 
  	template< typename L, typename R >
  	struct return_type< tag::comma, mpl::vector2<L,R> >
  	{
		typedef typename mpl::if_< boost::is_reference<R>,
					return_lvalue<R>,
					return_rvalue<R>
				>::type
			type;
  	};
  	
  // 実装
  	template<>
	struct action_impl< tag::comma >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Context >
		Ret apply( L l, R r, const Context& )const {
			return l,r;
		}
	};
	
//
// クラス
//	
	#define CLAZY_EXPR				CLAZY_BINARY_CALLABLE(comma_, tag::comma)
	#define CLAZY_SYMBOL_OPERATOR	operator,
	#include "../detail/pp_front.h"
	
//#######################################################################
// progn
//	
  // sideness
	template< typename Operands, typename Context >
	struct value_types_raw< tag::progn, Operands, Context >
		: public 
		detail::value_types_all< as_any_tag, Operands >
	{};
	
  // return_type 
  	template< typename Operands, typename Context >
  	struct return_type_raw< tag::progn, Operands, Context >
  	{
  		typedef typename detail::get_raw_result<
  					typename mpl::back< Operands >::type,
  					Context
  				>::type
  			last_ret_type;
  		
		typedef typename mpl::if_< boost::is_reference<last_ret_type>,
					return_lvalue<last_ret_type>,
					return_rvalue<last_ret_type>
				>::type
			type;
  	};
  	
  // 実装
  	template<>
	struct action_impl< tag::progn >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
	#define APPLY(z,n,_) \
		template< 												\
			typename Ret, 										\
			BOOST_PP_ENUM_PARAMS(n, typename Dummy),			\
			typename Body,										\
			typename Context 									\
		>														\
		Ret apply( 												\
			BOOST_PP_ENUM_PARAMS(n, Dummy), 					\
			Body body, 											\
			const Context& 										\
		)const {												\
			return body;										\
		}
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_DEC(KONBU_CLAZY_OPERAND_LIMIT), APPLY, _)
	#undef  APPLY
	
	};
	
//
// クラス
//	
	#define CLAZY_EXPR		CLAZY_VARIADIC_CALLABLE(progn_, tag::progn)
	#define CLAZY_SYMBOL	progn
	#include "../detail/pp_front.h"
	
}
}}

#endif



