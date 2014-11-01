#pragma once

#ifndef ODEN_KONBU_CLAZY_NOTHING
#define ODEN_KONBU_CLAZY_NOTHING

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//##################################################################
// �Ȃɂ����܂���B
//	
	template< typename Dummy >
	class action_impl< tag::nothing, Dummy >
	{
	public:
		action_impl(){}
		template< typename Args > explicit action_impl( Args& ){}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& )const 
		{
			return;
		}
	};
	
	template<>
	struct return_type_static< tag::nothing >
	{
		typedef return_nothing type;
	};
	
	template<>
	struct value_types_static< tag::nothing >
	{
		typedef mpl::vector0<> type;
	};
	
  ///////// ����! ///////////
	#define CLAZY_EXPR 			CLAZY_NULLARY_CALLABLE( nothing_impl_, tag::nothing )
	#define CLAZY_EXPR_EXTRA_T	1
	#include "../detail/pp_front.h" 

	typedef nothing_impl_<void> nothing_;
  	
  // �����֐�
  	inline nothing_ nothing()
	{
		return nothing_();
	}

}
}} /* end of namespace gobou, oden */


#endif




