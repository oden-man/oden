#pragma once

#ifndef ODEN_KONBU_DEFS_FWD
#define ODEN_KONBU_DEFS_FWD

#include <oden/konbu/defs.h>

namespace oden { namespace konbu
{
  // callback
	template< typename FuncPtr, typename FunctorInstance >
	struct callback;
	
  // pointer_function
	template< typename FunctionPointerType >
	class pointer_function;
	
  // member_function
	template< typename MemberFunctionPointerType >
	class member_function;
	
}}

#endif
