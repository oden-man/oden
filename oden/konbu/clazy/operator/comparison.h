#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_COMPARE
#define ODEN_KONBU_CLAZY_OP_COMPARE

//#######################################
//
//
// ---------- STL -------------
// ---------- boost.pp --------
// ---------- boost -----------
// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//#######################################################################
// 比較演算子 - 共通 -
//	
  // sideness
	template< typename AnyAction >
	struct value_types_static< tag::relational<AnyAction> >
	{
		typedef mpl::vector2< as_any_tag, as_any_tag > type;
	};
	
  // return_type - オーバーロードできます
	template< typename AnyAction, typename Args >
	struct return_type< tag::relational<AnyAction>, Args >
	{
		typedef return_rvalue<bool> type;
	};

//#######################################################################
// 比較演算子 - それぞれ -
//	
  // ==
	template<>
	class action_impl< tag::relational<tag::equal> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l==r;
		}
	};
	
  // !=
	template<>
	class action_impl< tag::relational<tag::not_equal> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l!=r;
		}
	};
	
  //<
	template<>
	class action_impl< tag::relational<tag::less> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l<r;
		}
	};
	
  //<=
	template<>
	class action_impl< tag::relational<tag::less_equal> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l<=r;
		}
	};	
	
  //>
	template<>
	class action_impl< tag::relational<tag::greater> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l>r;
		}
	};	
	
  //>=
	template<>
	class action_impl< tag::relational<tag::greater_equal> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l>=r;
		}
	};

//#######################################################################
// 式クラス
//	
    ////////// 生成! /////////////
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(equal_, tag::relational<tag::equal>)
	#define CLAZY_SYMBOL_OPERATOR 	operator==
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(not_equal_, tag::relational<tag::not_equal>)
	#define CLAZY_SYMBOL_OPERATOR 	operator!=
  	#include "../detail/pp_front.h"	
	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(less_, tag::relational<tag::less>)
	#define CLAZY_SYMBOL_OPERATOR 	operator<
  	#include "../detail/pp_front.h"	
	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(less_equal_, tag::relational<tag::less_equal>)
	#define CLAZY_SYMBOL_OPERATOR 	operator<=
  	#include "../detail/pp_front.h"	
	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(greater_, tag::relational<tag::greater>)
	#define CLAZY_SYMBOL_OPERATOR 	operator>
  	#include "../detail/pp_front.h"	
	
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(greater_equal_, tag::relational<tag::greater_equal>)
	#define CLAZY_SYMBOL_OPERATOR 	operator>=
  	#include "../detail/pp_front.h"	
	
}
}}


#endif









