#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_BITWISE
#define ODEN_KONBU_CLAZY_OP_BITWISE

//#######################################
//
//
// ---------- STL -------------
// ---------- boost.pp --------
// ---------- boost -----------
#include <boost/type_traits/integral_promotion.hpp>
#include <boost/mpl/always.hpp>

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
// 共通
//	
  // bitwise 
	template< typename AnyAction, typename AnyArgs >
	struct value_types< tag::bitwise<AnyAction>, AnyArgs >
		: public 
			detail::value_types_all< as_any_tag, AnyArgs >
	{};

//#######################################################################
// &, |, ^
//	
  // 実装クラス
	template<>
	class action_impl< tag::bitwise<tag::and_> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l & r;
		}
	};
	
	template<>
	class action_impl< tag::bitwise<tag::or_> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l | r;
		}
	};
	
	template<>
	class action_impl< tag::bitwise<tag::xor_> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l ^ r;
		}
	};
	
  // return_type - bitwise generic
	template< typename AnyAction, typename L, typename R >
	struct return_type< tag::bitwise<AnyAction>, mpl::vector2<L,R> >
	{
		typedef return_rvalue< 
			typename detail::usual_arithmetic_conversion< L, R >::type
			> 
		type;
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(bitand_, tag::bitwise<tag::and_>)
	#define CLAZY_SYMBOL_OPERATOR 	operator&
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(bitor_, tag::bitwise<tag::or_>)
	#define CLAZY_SYMBOL_OPERATOR 	operator|
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(bitxor_, tag::bitwise<tag::xor_>)
	#define CLAZY_SYMBOL_OPERATOR 	operator^
  	#include "../detail/pp_front.h"  

//#######################################################################
// &=, |=, ^=
//	
  // 実装クラス
	template<>
	class action_impl< tag::assign< tag::bitwise<tag::and_> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l &= r;
		}
	};
	
	template<>
	class action_impl<  tag::assign< tag::bitwise<tag::or_> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l |= r;
		}
	};
	
	template<>
	class action_impl<  tag::assign< tag::bitwise<tag::xor_> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l ^= r;
		}
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(bitand_assign_, tag::assign<tag::bitwise<tag::and_> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator&=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(bitor_assign_, tag::assign<tag::bitwise<tag::or_> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator|=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(bitxor_assign_, tag::assign<tag::bitwise<tag::xor_> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator^=
  	#include "../detail/pp_front.h"  
	
//#######################################################################
// <<, >>
//	
  // 実装クラス
	template<>
	class action_impl< tag::bitwise<tag::left_shift> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l<<r;
		}
	};	

	template<>
	class action_impl< tag::bitwise<tag::right_shift> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l>>r;
		}
	};
	
  // return_type
	template< typename L, typename R >
	struct return_type< tag::bitwise<tag::left_shift>, mpl::vector2<L,R> >
	{
		typedef return_rvalue< 
			typename boost::integral_promotion<L>::type 
			> 
		type;
	};
	
	template< typename L, typename R >
	struct return_type< tag::bitwise<tag::right_shift>, mpl::vector2<L,R> >
	{
		typedef return_rvalue< 
			typename boost::integral_promotion<L>::type 
			> 
		type;
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(left_shift_, tag::bitwise<tag::left_shift>)
	#define CLAZY_SYMBOL_OPERATOR 	operator<<
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(right_shift_, tag::bitwise<tag::right_shift>)
	#define CLAZY_SYMBOL_OPERATOR 	operator>>
  	#include "../detail/pp_front.h"
	
	
//#######################################################################
// <<=, >>=
//	
  // 実装クラス
	template<>
	class action_impl< tag::assign< tag::bitwise<tag::left_shift> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l<<=r;
		}
	};	

	template<>
	class action_impl< tag::assign< tag::bitwise<tag::right_shift> > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l>>=r;
		}
	};
	
    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(left_shift_assign_, tag::assign<tag::bitwise<tag::left_shift> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator<<=
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(right_shift_assign_, tag::assign<tag::bitwise<tag::right_shift> >)
	#define CLAZY_SYMBOL_OPERATOR 	operator>>=
  	#include "../detail/pp_front.h"	

//#######################################################################
// ~
//
  // 実装クラス
	template<>
	class action_impl< tag::bitwise<tag::not_> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Args >
		Ret apply( Operand opr, const Args& )const {
			return ~opr;
		}
	};
	
  // return_type
	template< typename T >
	struct return_type< tag::bitwise<tag::not_>, mpl::vector1<T> >
	{
		typedef return_rvalue< 
			typename boost::integral_promotion<T>::type 
			> 
		type;
	};

    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(complement_, tag::bitwise<tag::not_>)
	#define CLAZY_SYMBOL_OPERATOR	operator~
  	#include "../detail/pp_front.h"
  
  
}}}
#endif

