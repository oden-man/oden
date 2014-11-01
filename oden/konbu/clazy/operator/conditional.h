#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_COND
#define ODEN_KONBU_CLAZY_OP_COND

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
// conditional
//	
namespace detail
{
  // T==F, lvalue
  	template< typename T, typename F >
  	struct condi_ret
  	{
  		
  		typedef 
  		typename mpl::eval_if< 
  							
  						
  					
  	};
}
  // sideness
	template<>
	struct value_types_static< tag::conditional >
	{
		typedef mpl::vector3< as_any_tag, as_any_tag, as_any_tag > type;
	};
	
  // return_type 
  	template< typename C, typename T, typename F, typename Context >
  	struct return_type_raw< tag::conditional, mpl::vector3<C,T,F>, Context >
  	{
  		typedef typename detail::get_raw_result<T, Context>::type true_ret;
  		typedef typename detail::get_raw_result<F, Context>::type false_ret;
  		
  		typedef typename boost::decay< typename detail::lvalue_to_rvalue_conversion<true_ret>::type >::type  t_type;
  		typedef typename boost::decay< typename detail::lvalue_to_rvalue_conversion<false_ret>::type >::type f_type;
  		
		typedef 
		typename mpl::eval_if< // ともに同じか？
  					boost::is_same<t_type, f_type>,	
  					mpl::identity<t_type>,
  					mpl::eval_if< // ともに算術・列挙型か？
  						mpl::and_< 
  							mpl::or_< boost::is_arithmetic<t_type>, boost::is_enum<t_type> >, 
  							mpl::or_< boost::is_arithmetic<f_type>, boost::is_enum<f_type> > 
  							>,
  						detail::usual_arithmetic_conversion<t_type, f_type>,
  						mpl::eval_if< // ともにポインタ
  						
  						
					mpl::and_< boost::is_same<T,F>, mpl::or_< boost::is_reference<T>, boost::is_same<T, void> > >
					mpl::identity<T>,
					detail::condi_ret<T,F>
				>::type
			ret_type;
		
		typedef 
		typename mpl::if_< boost::is_reference<ret_type>, 
					return_lvalue<ret_type>, 
					return_rvalue<ret_type> 
				>::type
			type;
				
			
			
			is_both_void,
				mpl::identity<void>,
				mpl::if_< is_neither_void,
					 mpl::if_< boost::is_same<
				
		
		boost::is_reference<R>,
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
	
}
}}

#endif



