#pragma once

#ifndef ODEN_KONBU_CALLBACK
#define ODEN_KONBU_CALLBACK

#include <oden/konbu/defs.h>

#include <boost/preprocessor/iteration/iterate.hpp> 
#define KONBU_CC_N BOOST_PP_SEQ_SIZE(ODEN_KONBU_CC)


namespace oden { namespace konbu
{
//________________________________________________________________________________________________
//
//  callbacks arg select
//________________________________________________________________________________________________
//
namespace detail
{
	template< int I > /* 1~6 */
	struct select_param
	{};

	#define USE_PARAM(p) (void)(p)
	
	#define SELECT_PARAM_1(I) \
		template< typename A1 >										\
		const A##I& operator()( const A1& a1 )const					\
		{ return a##I; }											\
		
	#define SELECT_PARAM_2(I) \
		template< typename A1, typename A2 >						\
		const A##I& operator()( const A1& a1, const A2& a2 )const	\
		{ USE_PARAM(a1); USE_PARAM(a2);								\
		  return a##I; }											\

	#define SELECT_PARAM_3(I) \
		template< typename A1, typename A2, typename A3 >			\
		const A##I& operator()( const A1& a1, const A2& a2, 		\
								const A3& a3 )const					\
		{ USE_PARAM(a1); USE_PARAM(a2); USE_PARAM(a3);				\
		  return a##I; }											\

	#define SELECT_PARAM_4(I) \
		template< typename A1, typename A2, typename A3, 			\
											typename A4 >			\
		const A##I& operator()( const A1& a1, const A2& a2, 		\
								const A3& a3, const A4& a4 )const	\
		{ USE_PARAM(a1); USE_PARAM(a2); USE_PARAM(a3); 				\
										USE_PARAM(a4);				\
		  return a##I; }											\

	#define SELECT_PARAM_5(I) \
		template< typename A1, typename A2, typename A3, 			\
								typename A4, typename A5 >			\
		const A##I& operator()( const A1& a1, const A2& a2, 		\
								const A3& a3, const A4& a4,			\
								const A5& a5 )const					\
		{ USE_PARAM(a1); USE_PARAM(a2); USE_PARAM(a3); 				\
						 USE_PARAM(a4);	USE_PARAM(a5);				\
		  return a##I; }											\

	#define SELECT_PARAM_6(I) \
		template< typename A1, typename A2, typename A3, 			\
				typename A4, typename A5, typename A6 >				\
		const A##I& operator()( const A1& a1, const A2& a2, 		\
								const A3& a3, const A4& a4,			\
								const A5& a5, const A6& a6 )const	\
		{ USE_PARAM(a1); USE_PARAM(a2); USE_PARAM(a3); 				\
		  USE_PARAM(a4); USE_PARAM(a5);	USE_PARAM(a6);				\
		  return a##I; }											\


	template<>
	struct select_param<1>
	{
		SELECT_PARAM_1(1)
		SELECT_PARAM_2(1)
		SELECT_PARAM_3(1)
		SELECT_PARAM_4(1)
		SELECT_PARAM_5(1)
		SELECT_PARAM_6(1)
	};

	template<>
	struct select_param<2>
	{
		SELECT_PARAM_2(2)
		SELECT_PARAM_3(2)
		SELECT_PARAM_4(2)
		SELECT_PARAM_5(2)
		SELECT_PARAM_6(2)
	};

	template<>
	struct select_param<3>
	{
		SELECT_PARAM_3(3)
		SELECT_PARAM_4(3)
		SELECT_PARAM_5(3)
		SELECT_PARAM_6(3)
	};

	template<>
	struct select_param<4>
	{
		SELECT_PARAM_4(4)
		SELECT_PARAM_5(4)
		SELECT_PARAM_6(4)
	};

	template<>
	struct select_param<5>
	{
		SELECT_PARAM_5(5)
		SELECT_PARAM_6(5)
	};

	template<>
	struct select_param<6>
	{
		SELECT_PARAM_6(6)
	};

	#undef SELECT_PARAM_1
	#undef SELECT_PARAM_2
	#undef SELECT_PARAM_3
	#undef SELECT_PARAM_4
	#undef SELECT_PARAM_5
	#undef SELECT_PARAM_6
	#undef USE_PARAM
}

//________________________________________________________________________________________________
//
//  callbackで使える、簡易ファンクタ抽出器
//________________________________________________________________________________________________
//
//##############################################
// int->ポインタへreinterpret_castする
//
	struct callback_param
	{
		template< typename ProcPtr, typename Val >
		static ProcPtr get( Val val )
		{ return reinterpret_cast<ProcPtr>(val); }
	};
	
//##############################################
// 引数のメンバ
//
	template< typename Type, typename Class, Type Class::*MemVarPtr >
	struct callback_param_member
	{
		template< typename ProcPtr >
		static ProcPtr get( const Class& val )
		{
			return reinterpret_cast<ProcPtr>( val.*MemVarPtr );
		}
		template< typename ProcPtr >
		static ProcPtr get( const Class* val )
		{
			return reinterpret_cast<ProcPtr>( val->*MemVarPtr );
		}
	};

}} /* end of namespace oden.konbu */

//________________________________________________________________________________________________
//
//  callbacks
//________________________________________________________________________________________________
//
#if (KONBU_CC_N>=1)
#
#	define KONBU_CC_I					0
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/callback_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 1, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_I
#
#endif
#if (KONBU_CC_N>=2)
#
#	define KONBU_CC_I					1
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/callback_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 1, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_I
#
#endif
#if (KONBU_CC_N>=3)
#
#	define KONBU_CC_I					2
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/callback_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 1, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_I
#
#endif
#if (KONBU_CC_N>=4)
#
#	define KONBU_CC_I					3
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/callback_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 1, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_I
#
#endif
#if (KONBU_CC_N>=5)
#
#	define KONBU_CC_I					4
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/callback_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 1, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_I
#
#endif

// callbackとして、variadicは生成しなくてよい.
  	
#undef KONBU_CC_N

#endif
