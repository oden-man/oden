#pragma once

#ifndef ODEN_KONBU_CLAZY_OP_ASSIGN
#define ODEN_KONBU_CLAZY_OP_ASSIGN

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
// ������Z�q
//	
  // �����N���X
	template<>
	class action_impl< tag::assign<tag::plain> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename L, typename R, typename Args >
		Ret apply( L l, R r, const Args& )const {
			return l=r;
		}
	};
	
  // value_types - �S�Ă̑���A�N�V�����ŋ���
	template< typename AnyAction, typename Args >
	struct value_types< tag::assign<AnyAction>, Args >
	{
		typedef mpl::vector2< as_lvalue_tag, as_any_tag > type;
	};
	
  // return_type
	template< typename AnyAction, typename L, typename R >
	struct return_type< tag::assign<AnyAction>, mpl::vector2<L,R> >
	{
		typedef return_lvalue<L> type;
	};

  // �N���X�̂ݐ���
  	#define CLAZY_EXPR CLAZY_BINARY_CALLABLE(assign_, tag::assign<tag::plain>)
  	#include "../detail/pp_front.h"

  // �I�[�o�[���[�h
	// functor�N���X�ł̐錾������
	template< typename Left >
	template< typename Right >
	assign_< Left, Right > functor<Left>::operator=( const Right& r )const
	{	
		return assign_< Left, Right >( this->to_derived(), detail::wrap_value(r) );
	}
}}}	


#endif



