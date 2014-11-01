
#ifndef BOOST_PP_IS_ITERATING /* ファイルイテレート中でない */

//#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_CALL
#define ODEN_KONBU_CLAZY_OP_CALL

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/function_types/is_function_reference.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/mpl/always.hpp>

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
// ---------- konbu ------------
#include "../detail/callable_expression.h"

//
//#######################################

//________________________________________________________________________________________________________________
//
//    呼び出し演算子() (オーバーロードはなし)
//________________________________________________________________________________________________________________
//	
//#######################################################################
// 共通
//		
namespace oden { namespace konbu { namespace clazy
{
	namespace ftypes = boost::function_types;
	
	namespace detail
	{
		template< typename Functor, typename Args, int Arity >
		struct call_result_of
		{};
	}
	
  // 共通 - value_types
	template< typename AnyAction, typename Operands, typename Extra >
	struct value_types_raw< tag::call<AnyAction>, Operands, Extra >
		: public 
			detail::value_types_all< as_any_tag, Operands >
	{};
	
  // call - return_type
	template< typename Operands, typename Contextual >
	struct return_type_raw< tag::call<tag::return_type_not_specified>, Operands, Context >
	{
		typedef typename detail::get_raw_result<
					typename mpl::at_c<Operands,0>::type
				>::type
			function_raw_type;
	
		typedef typename mpl::eval_if<
					ftypes::is_function_reference< function_raw_type >,
					mpl::identity< function_raw_type >,
					detail::remove_reference_and_cv< function_raw_type >
				>::type
			functor_type;
	
		typedef typename mpl::eval_if< 
					ftypes::is_callable_builtin< functor_type >,
					ftypes::result_type< functor_type >,				// 関数型から戻り値型がわかる
					boost::result_of<
						typename ftypes::function_type< Args >::type	// result テンプレート等を調べる
					>
				>::type
			ret_type;
		
		typedef typename mpl::if_<
							boost::is_reference<ret_type>,
							return_lvalue<ret_type>,
							return_rvalue<ret_type>
						>::type
					type;
	};
	
  // callr - return_type
	template< typename Ret >
	struct return_type_static< tag::call<tag::return_type_specified>, mpl::vector1<Ret> >
	{
		typedef typename mpl::if_<
							boost::is_reference<Ret>,
							return_lvalue<Ret>,
							return_rvalue<Ret>
						>::type
					type;
	};

  // 実装
	template< typename AnyAction, typename Extra >
	class action_impl< tag::call<AnyAction>, Extra >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
	
	// イテレート開始
	#define BOOST_PP_FILENAME_1 		<oden/konbu/clazy/operator/call.h>
	#define BOOST_PP_ITERATION_LIMITS	( 0, KONBU_CLAZY_OPERAND_LIMIT )
	#include BOOST_PP_ITERATE()
	
	};
	
  // 戻り値型指定なし
	//////// 生成 ////////////
  	#define CLAZY_EXPR 				CLAZY_VARIADIC_CALLABLE(call_, tag::call<tag::return_type_not_specified>)
	#define CLAZY_SYMBOL_VARIADIC	call
  	#include "../detail/pp_front.h"
	
  // 戻り値型指定あり
	//////// 生成 ////////////
  	#define CLAZY_EXPR 				CLAZY_VARIADIC_CALLABLE(callr_, tag::call<tag::return_type_specified>)
	#define CLAZY_EXPR_EXTRA_T 		1
	#define CLAZY_SYMBOL_VARIADIC	call
	#include "../detail/pp_front.h"
	
}}}
	
#endif // インクルードガード

//########################################################################
//        *****  ファイルイテレート中  *****
//
#else // def BOOST_PP_IS_ITERATING	

#define N_CALL			BOOST_PP_ITERATION()
		
// 関数オブジェクトを呼ぶ
	template< 
		typename Ret,
		typename Dummy0 BOOST_PP_ENUM_TRAILING_PARAMS(N_CALL, typename Arg),
		typename Functor, typename RealArgs 
	>
	Ret apply( 
		const Functor& functor BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N_CALL, Arg, arg), 
		const RealArgs&,
		typename boost::disable_if< ftypes::is_callable_builtin<Functor> >::type* = 0
	)const  
	{
		return functor( BOOST_PP_ENUM_PARAMS(N_CALL, arg) );
	}
	
// 関数参照を呼ぶ
	template< 
		typename Ret,
		typename Dummy0 BOOST_PP_ENUM_TRAILING_PARAMS(N_CALL, typename Arg),
		typename Functor, typename RealArgs 
	>
	Ret apply( 
		const Functor& functor BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N_CALL, Arg, arg), 
		const RealArgs&,
		typename boost::enable_if< ftypes::is_function_reference<Functor> >::type* = 0
	)const  
	{
		return functor( BOOST_PP_ENUM_PARAMS(N_CALL, arg) );
	}
	
// 関数ポインタを呼ぶ
	template< 
		typename Ret,
		typename Dummy0 BOOST_PP_ENUM_TRAILING_PARAMS(N_CALL, typename Arg),
		typename Functor,
		typename RealArgs 
	>
	Ret apply( 
		const Functor& functor BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N_CALL, Arg, arg), 
		const RealArgs&,
		typename boost::enable_if< ftypes::is_function_pointer<Functor> >::type* = 0
	)const  
	{
		return (*functor)( BOOST_PP_ENUM_PARAMS(N_CALL, arg) );
	}
	
#if (N_CALL>=1)
#define N_MEMBER_CALL BOOST_PP_DEC(N_CALL)

// メンバ関数ポインタを呼ぶ(インスタンスポインタから)
	template< 
		typename Ret,
		typename Dummy0, typename Dummy1 BOOST_PP_ENUM_TRAILING_PARAMS(N_MEMBER_CALL, typename Arg),
		typename Functor, typename Instance, typename RealArgs 
	>
	Ret apply( 
		const Functor& functor, 
		Instance* 	   instance BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N_MEMBER_CALL, Arg, arg), 
		const RealArgs&,
		typename boost::enable_if< ftypes::is_member_function_pointer<Functor> >::type* = 0 
	)const  
	{
		return (instance->*functor)( BOOST_PP_ENUM_PARAMS(N_MEMBER_CALL, arg) );
	}
	
// メンバ関数ポインタを呼ぶ(インスタンス参照から
	template< 
		typename Ret,
		typename Dummy0, typename Dummy1 BOOST_PP_ENUM_TRAILING_PARAMS(N_MEMBER_CALL, typename Arg),
		typename Functor, typename Instance, typename RealArgs 
	>
	Ret apply( 
		const Functor& functor, 
		Instance& 	   instance BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N_MEMBER_CALL, Arg, arg), 
		const RealArgs&,
		typename boost::enable_if< ftypes::is_member_function_pointer<Functor> >::type* = 0 
	)const  
	{
		return (instance.*functor)( BOOST_PP_ENUM_PARAMS(N_MEMBER_CALL, arg) );
	}
	
#undef N_MEMBER_CALL
#endif
	
#undef N_CALL /* 回数 */
	
#endif // BOOST_PP_IS_ITERATIN




	
	
