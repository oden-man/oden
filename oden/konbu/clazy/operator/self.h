#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_SELF
#define ODEN_KONBU_CLAZY_OP_SELF

//#######################################
//
//
// ---------- STL -------------
#include <iterator>
// ---------- boost.pp --------
// ---------- boost -----------
#include <boost/indirect_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//#######################################################################
// indirect
//	
  // side 
	template<>
	struct value_types_static< tag::indirect >
	{
		typedef mpl::vector1< as_any_tag > type;
	};
	
  // return type
  	template< typename T >
  	struct return_type< tag::indirect, boost::vector1<T> >
  	{
  		typedef return_lvalue<
			typename boost::indirect_reference< typename boost::remove_cv<T>::type >::type 
		> type;
  	};
  	
  // ŽÀ‘•
	template<>
	struct action_impl< tag::indirect >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Args >
		Ret apply( Operand opr, const Args& )const {
			return *opr;
		}
	};
	
//#######################################################################
// address
//	
  // sideness
	template< typename T >
	struct value_types< tag::address, mpl::vector1<T> >
	{
		typedef mpl::vector1< as_lvalue_tag > type;
	};
	
  // return_type 
  	template< typename T >
  	struct return_type< tag::address, boost::vector1<T> >
  	{
		typedef return_lvalue<
			typename boost::add_pointer<T>::type,
			typename boost::add_pointer<T>::type 
		> type;
  	};
  	
  // address
  	template<>
	struct action_impl< tag::address >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Operand, typename Args >
		Ret apply( Operand opr, const Args& )const {
			return &opr;
		}
	};
	
//#######################################################################
// subscript
//	
  // sideness
	template<>
	struct value_types_static< tag::subscript >
	{
		typedef mpl::vector2< as_any_tag, as_any_tag > type;
	};
	
  // return_type 
  	template< typename L, typename R >
  	struct return_type< tag::subscript, boost::vector2<L,R> >
  	{  		
		typedef return_lvalue<
			typename boost::remove_pointer<L>::type
		> type;
  	};
  	
  // ŽÀ‘•
  	template<>
	struct action_impl< tag::subscript >
	{
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Context >
		Ret apply( L l, R r, const Context& )const {
			return l[r];
		}
	};
	

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
  	struct return_type< tag::comma, boost::vector2<L,R> >
  	{
		typedef typename mpl::if_< boost::is_reference<R>,
					return_lvalue<R>,
					return_rvalue<R>
				>::type
			type;
  	};
	
//###############################################################
// ƒNƒ‰ƒX
//
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(indirect_, tag::indirect)
	#define CLAZY_SYMBOL_OPERATOR	operator*
  	#include "../detail/pp_front.h"
  	
  	#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(address_, tag::address)
	#define CLAZY_SYMBOL_OPERATOR	operator&
  	#include "../detail/pp_front.h"
	
	#define CLAZY_EXPR				CLAZY_BINARY_CALLABLE(subscript_, tag::subscript)
	#include "../detail/pp_front.h"
	
	template< typename Left >
	template< typename Right >
	subscript_< Left, Right > subscript_operator_impl<Left>::operator[]( const Right& r )const
	{	
		return subscript_< Left, Right >( static_cast<const Left&>(*this), detail::wrap_value(r) );
	}
	
}
}}

#endif



