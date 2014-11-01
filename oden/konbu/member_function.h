#pragma once

#ifndef ODEN_KONBU_MEM_FUN
#define ODEN_KONBU_MEM_FUN

#include <oden/konbu/defs.h>

#include <boost/call_traits.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#define KONBU_CC_MF_N BOOST_PP_SEQ_SIZE(ODEN_KONBU_MF_CC)

//________________________________________________________________________________________________
//
//  member_function
//________________________________________________________________________________________________
//
#if (KONBU_MF_CC_N>0)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_MF_CC, 0)
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_MF_CC_N>1)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_MF_CC, 1)
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_MF_CC_N>2)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_MF_CC, 2)
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_MF_CC_N>3)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_MF_CC, 3)
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_MF_CC_N>4)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_MF_CC, 4)
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_MF_CC_N>5)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_MF_CC, 5)
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#endif

// cdecl variadic
/*
#if defined(ODEN_KONBU_ENABLE_MF_VARIADIC)	
#
#	define KONBU_CC_QUAL __cdecl
#
#	define KONBU_MF_CV_QUAL
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_variadic_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 2, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	define KONBU_MF_CV_QUAL const
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/member_function_variadic_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 2, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_MF_CV_QUAL
#
#	undef  KONBU_CC_QUAL
#
#endif */

#undef KONBU_CC_MF_N

#endif
