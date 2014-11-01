#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_ARITHMETIC
#define ODEN_KONBU_CLAZY_OP_ARITHMETIC

//#######################################
//
//
// ---------- STL -------------
#include <cstddef>
// ---------- boost.pp --------
// ---------- boost -----------
#include <boost/type_traits/integral_promotion.hpp>

// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"
#include <oden/konbu/clazy/operator/assign.h>
#include <oden/konbu/clazy/detail/usual_arithmetic_conversion.h>

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//#######################################################################
// arithmetic 共通
//	
namespace detail
{
	struct illegal_pointer_arithmetic_operation
	{};
}

  // value_types - arithmetic generic
	template< typename AnyAction, typename AnyArgs >
	struct value_types< tag::arithmetic<AnyAction>, AnyArgs >
		: public 
			detail::value_types_all< as_any_tag, AnyArgs >
	{};
	
  // return_type - arithmetic generic
	template< typename AnyAction, typename L, typename R >
	struct plain_return_type< tag::arithmetic<AnyAction>, mpl::vector2<L,R> >
	{
		typedef return_rvalue< 
			typename detail::usual_arithmetic_conversion< L, R >::type
			> 
		type;
	};

//#######################################################################
// + (binary)
//	
  // 実装クラス
	template<>
	class action_impl< tag::arithmetic<tag::plus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l + r;
		}
	};
	
  // return_type - arithmetic generic
	template< typename L, typename R >
	struct plain_return_type< tag::arithmetic<tag::plus>, mpl::vector2<L,R> >
	{
		typedef
		typename mpl::eval_if<
					boost::is_pointer<L>,
					mpl::eval_if<
						boost::is_pointer<R>,
						mpl::identity< detail::illegal_pointer_arithmetic_operation >,
						mpl::identity< L >
					>,
					mpl::eval_if<
						boost::is_pointer<R>,
						mpl::identity< R >,
						detail::usual_arithmetic_conversion< L, R >
					>
				>::type
			ret_type;

		typedef return_rvalue< ret_type	> type;
	};
	
  ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(plus_, tag::arithmetic<tag::plus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator+
  	#include "../detail/pp_front.h"
	
//#######################################################################
// - (binary)
//	
  // 実装クラス
	template<>
	class action_impl< tag::arithmetic<tag::minus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l - r;
		}
	};
	
  // return_type - arithmetic generic
	template< typename L, typename R >
	struct plain_return_type< tag::arithmetic<tag::minus>, mpl::vector2<L,R> >
	{	
		typedef
		typename mpl::eval_if<
					boost::is_pointer<L>,
					mpl::eval_if<
						boost::is_pointer<R>,
						mpl::identity< std::ptrdiff >,
						mpl::identity< L >
					>,
					mpl::eval_if<
						boost::is_pointer<R>,
						mpl::identity< detail::illegal_pointer_arithmetic_operation >,
						detail::usual_arithmetic_conversion< L, R >
					>
				>::type
			ret_type;
					
		typedef return_rvalue< ret_type	> type;
	};
	
  ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(minus_, tag::arithmetic<tag::minus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator-
  	#include "../detail/pp_front.h"
	

//#######################################################################
// * / %
//		
	template<>
	class action_impl< tag::arithmetic<tag::multiply> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l * r;
		}
	};
	
	template<>
	class action_impl< tag::arithmetic<tag::divide> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l / r;
		}
	};
	
	template<>
	class action_impl< tag::arithmetic<tag::modulus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l % r;
		}
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(multiply_, tag::arithmetic<tag::multiply>)
	#define CLAZY_SYMBOL_OPERATOR 	operator*
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(divide_, tag::arithmetic<tag::divide>)
	#define CLAZY_SYMBOL_OPERATOR 	operator/
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(modulo_, tag::arithmetic<tag::modulus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator%
  	#include "../detail/pp_front.h"  

//#######################################################################
// +=, -=, *=, /=, %= ( assign.h にその他の定義 )
//	
  // 実装クラス
	template<>
	class action_impl< tag::assign< tag::arithmetic<tag::plus_> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l += r;
		}
	};
	
	template<>
	class action_impl<  tag::assign< tag::arithmetic<tag::minus_> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l -= r;
		}
	};
	
	template<>
	class action_impl<  tag::assign< tag::arithmetic<tag::multiply> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l *= r;
		}
	};
	
	template<>
	class action_impl<  tag::assign< tag::arithmetic<tag::divide> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l /= r;
		}
	};
	
	template<>
	class action_impl<  tag::assign< tag::arithmetic<tag::modulus> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l %= r;
		}
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(plus_assign_, tag::assign<tag::arithmetic<tag::plus> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator+=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(minus_assign_, tag::assign<tag::arithmetic<tag::minus> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator-=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(multiply_assign_, tag::assign<tag::arithmetic<tag::multiply> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator*=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(divide_assign_, tag::assign<tag::arithmetic<tag::divide> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator/=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(modulo_assign_, tag::assign<tag::arithmetic<tag::modulus> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator%=
  	#include "../detail/pp_front.h"
	
//#######################################################################
// unary +, -
//	
  // 実装クラス
	template<>
	class action_impl< tag::unary_arithmetic<tag::plus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Context >
		Ret apply( Operand opr, const Context& )const {
			return +opr;
		}
	};	

	template<>
	class action_impl< tag::unary_arithmetic<tag::minus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Context >
		Ret apply( Operand opr, const Context& )const {
			return -opr;
		}
	};	
	
  // value_types 
	template< typename AnyAction, typename AnyArgs >
	struct value_types< tag::arithmetic<AnyAction>, AnyArgs >
	{
		typedef mpl::vector1<as_any_tag> type;
	};
	
  // return_type
	template< typename AnyAction, typename T >
	struct return_type< tag::unary_arithmetic<AnyAction>, mpl::vector1<T> >
	{
		typedef return_rvalue< typename boost::integral_promotion<T>::type > 
		type;
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(unary_plus_, tag::unary_arithmetic<tag::plus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator+
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(negate_, tag::unary_arithmetic<tag::minus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator-
  	#include "../detail/pp_front.h"
	
//#######################################################################
// ++ ,-- (前置)
//	
  // 実装クラス
	template<>
	class action_impl< tag::pre_incr_decr<tag::plus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Context >
		Ret apply( Operand opr, const Context& )const {
			return ++opr;
		}
	};	

	template<>
	class action_impl< tag::pre_incr_decr<tag::minus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Context >
		Ret apply( Operand opr, const Context& )const {
			return --opr;
		}
	};	
	
  // value_types 
	template< typename AnyAction, typename AnyArgs >
	struct value_types< tag::pre_incr_decr<AnyAction>, AnyArgs >
	{
		typedef mpl::vector1<as_lvalue_tag> type;
	};
	
  // return_type
	template< typename AnyAction, typename T >
	struct return_type< tag::pre_incr_decr<AnyAction>, mpl::vector1<T> >
	{
		typedef return_lvalue<T> type;
	};
	
    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(increment_, tag::pre_incr_decr<tag::plus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator++
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(decrement_, tag::pre_incr_decr<tag::minus>)
	#define CLAZY_SYMBOL_OPERATOR 	operator--
  	#include "../detail/pp_front.h"	
	

//#######################################################################
// ++ ,-- (後置)
//	
  // 実装クラス
	template<>
	class action_impl< tag::post_incr_decr<tag::plus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Context >
		Ret apply( Operand opr, const Context& )const {
			return opr++;
		}
	};	

	template<>
	class action_impl< tag::post_incr_decr<tag::minus> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Context >
		Ret apply( Operand opr, const Context& )const {
			return opr--;
		}
	};	
	
  // value_types 
	template< typename AnyAction, typename AnyArgs >
	struct value_types< tag::post_incr_decr<AnyAction>, AnyArgs >
	{
		typedef mpl::vector1<as_lvalue_tag> type;
	};
	
  // return_type
	template< typename AnyAction, typename T >
	struct return_type< tag::post_incr_decr<AnyAction>, mpl::vector1<T> >
	{
		typedef return_rvalue<T> type;
	};
	
  ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(post_increment_, tag::post_incr_decr<tag::plus>)
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(post_decrement_, tag::post_incr_decr<tag::minus>)
  	#include "../detail/pp_front.h"	
	
  // 演算子の定義
	template< typename Arg >
	post_increment_< Arg > operator++ ( const functor<Arg>& a, int )
	{
		return post_increment_< Arg >( a.to_derived() );
	}
	
	template< typename Arg >
	post_decrement_< Arg > operator-- ( const functor<Arg>& a, int )
	{
		return post_decrement_< Arg >( a.to_derived() );
	}	
  
}}}
#endif

