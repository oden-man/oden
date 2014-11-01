#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_MEMBERVAR
#define ODEN_KONBU_CLAZY_OP_MEMBERVAR

//#######################################
//
//
// ---------- STL -------------
// ---------- boost.pp --------
// ---------- boost -----------
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/is_member_object_pointer.hpp>
// ---------- oden ------------
#include <oden/konbu/member_variable_types.h>
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
	
	
  // return_type impl
	namespace detail
	{
	  	template< typename T >
	  	struct return_type_pointer_to_member_impl_1 
	  	{
			typedef return_rvalue< konbu::member_function<T> >
				type;
		};
		
	  	template< typename T >
	  	struct return_type_pointer_to_member_impl_2 
	  	{
			typedef return_lvalue< typename konbu::member_variable_type<R>::type >
				type;
		};
		
	  	template< typename T >
	  	struct return_type_pointer_to_member_impl_3 
	  	{
			typedef return_rvalue< typename konbu::member_variable_type<R>::type >
				type;
		};
	}
	
//#######################################################################
// メンバ選択演算子 .*
//	
  // sideness
	template<>
	struct value_types_static< tag::pointer_to_member<tag::through_reference> >
	{
		typedef mpl::vector2< as_any_tag, as_any_tag > type;
	};
	
  // return_type
	template< typename L, typename R, typename Context>
	struct return_type_raw< tag::pointer_to_member<tag::through_reference>, mpl::vector2<L,R>, Context >
	{
		typedef typename detail::get_raw_result<R,Context>::type right_type;
		
		typedef mpl::eval_if<
					ftypes::is_member_function_pointer<right_type>,
					detail::return_type_pointer_to_member_impl_1<right_type>,
					mpl::eval_if<
						detail::can_be_lvalue<L,Context>,
						detail::return_type_pointer_to_member_impl_2<right_type>,
						detail::return_type_pointer_to_member_impl_3<right_type>
					>
				>::type
			type;
	};
	
  // 実装クラス
	template<>
	class action_impl< tag::pointer_to_member< tag::through_reference > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Context >
		Ret apply( 
			L l, R r, const Context&, 
			typename boost::enable_if< ftypes::is_member_object_pointer<R> >::type* = 0
		)const 
		{
			return l.*r;
		}
		
		template< typename Ret, typename L, typename R, typename Context >
		Ret apply( 
			L l, R r, const Context&, 
			typename boost::enable_if< ftypes::is_member_function_pointer<R> >::type* = 0
		)const 
		{
			return member_function<R>( r, &l );
		}
	};

  // クラスのみ生成
  	#define CLAZY_EXPR 		CLAZY_BINARY_CALLABLE(member_, tag::pointer_to_member<tag::through_reference>)
	#define CLAZY_SYMBOL	member
  	#include "../detail/pp_front.h"

//#######################################################################
// メンバ選択演算子 ->*
//	
  // sideness
	template< typename A >
	struct value_types_static< tag::pointer_to_member<tag::through_pointer> >
	{
		typedef mpl::vector2< as_lvalue_tag, as_any_tag > type;
	};
	
  // return_type
	template< typename L, typename R >
	struct return_type< tag::pointer_to_member<tag::through_pointer>, mpl::vector2<L,R> >
	{
		typedef mpl::eval_if<
					ftypes::is_member_function_pointer<R>,
					detail::return_type_pointer_to_member_impl_1<R>,
					detail::return_type_pointer_to_member_impl_2<R>
				>::type
			type;
	};
	
  // 実装クラス
	template<>
	class action_impl< tag::pointer_to_member< tag::through_pointer > >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Context >
		Ret apply( 
			L l, R r, const Context&, 
			typename boost::enable_if< ftypes::is_member_object_pointer<R> >::type* = 0
		)const 
		{
			return l->*r;
		}
		
		template< typename Ret, typename L, typename R, typename Context >
		Ret apply( 
			L l, R r, const Context&, 
			typename boost::enable_if< ftypes::is_member_function_pointer<R> >::type* = 0
		)const 
		{
			return member_function<R>( r, l );
		}
	};

  // クラスのみ生成
  	#define CLAZY_EXPR 				CLAZY_BINARY_CALLABLE(pointer_member_, tag::pointer_to_member<tag::through_pointer>)
	#define CLAZY_SYMBOL_OPERATOR	operator->*
	#include "../detail/pp_front.h"

}}}	


#endif

