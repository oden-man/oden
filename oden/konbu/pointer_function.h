#pragma once

#ifndef ODEN_KONBU_PTR_FUN_CDECL
#define ODEN_KONBU_PTR_FUN_CDECL

#include <oden/konbu/defs.h>

#include <boost/call_traits.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#define KONBU_CC_N 	  BOOST_PP_SEQ_SIZE(ODEN_KONBU_CC)

//________________________________________________________________________________________________
//
//  pointer_functions
//________________________________________________________________________________________________
//
#if (KONBU_CC_N>0)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_CC, 0)
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_CC_N>1)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_CC, 1)
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_CC_N>2)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_CC, 2)
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_CC_N>3)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_CC, 3)
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_CC_N>4)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_CC, 4)
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#elif (KONBU_CC_N>5)
#
#	define KONBU_CC_QUAL BOOST_PP_SEQ_ELEM(ODEN_KONBU_CC, 5)
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 0, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#endif

// cdecl variadic
#if defined(ODEN_KONBU_ENABLE_VARIADIC)
#
#	define KONBU_CC_QUAL __cdecl
#	define BOOST_PP_FILENAME_1			<oden/konbu/detail/pointer_function_variadic_impl_preprocessor.h>
#	define BOOST_PP_ITERATION_LIMITS	( 2, ODEN_KONBU_MAX_ARITY )
#	include BOOST_PP_ITERATE()
#	undef  KONBU_CC_QUAL
#
#endif

#undef KONBU_CC_N
#endif
