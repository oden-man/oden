#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_LOGICAL
#define ODEN_KONBU_CLAZY_OP_LOGICAL

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
// ò_óùââéZéq - ÇŸÇ⁄ã§í  -
//	
  // sideness
	template< typename AnyAction >
	struct value_types_raw< tag::logical<AnyAction>, Operands >
		: public
			detail::value_types_all< as_any_tag, Operands >
	{};
	
  // return_type
	template< typename AnyAction, typename Args >
	struct return_type< tag::logical<AnyAction>, Args >
	{
		typedef return_rvalue<bool> type;
	};
	
  //&&,||ÇÕíZóçï]âøÇÃÇΩÇﬂÅAé¿ëïÇÕÇ¢ÇÁÇ»Ç¢
	template< typename AnyAction >
	class action_impl< tag::logical<AnyAction> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
  	};
	
  // !
	template<>
	class action_impl< tag::logical<tag::not_> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Args >
		Ret apply( Operand opr, const Args& )const {
			return !opr;
		}
	};

//#######################################################################
// ÉNÉâÉXê∂ê¨
//	
  	#define CLAZY_EXPR  CLAZY_BINARY_CALLABLE(logical_and_, tag::logical<tag::and_>)
	#define CLAZY_SHORT_CIRCUIT_OPERATOR &&
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR  CLAZY_BINARY_CALLABLE(logical_or_, tag::logical<tag::or_>)
	#define CLAZY_SHORT_CIRCUIT_OPERATOR ||
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR  CLAZY_UNARY_CALLABLE(logical_not_, tag::logical<tag::not_>)
	#define CLAZY_SYMBOL_OPERATOR operator!
  	#include "../detail/pp_front.h"	

}
}}

#endif



