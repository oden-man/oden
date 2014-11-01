#pragma once

#ifndef ODEN_KONBU_CLAZY_PRIM_DEFER
#define ODEN_KONBU_CLAZY_PRIM_DEFER

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
// ���[�J���ϐ��̐錾�q
//	
  // �R���X�g���N�^�ł�Expr�ɓ]�������A���s�����Expr��Ԃ�
	template< typename Expr >
	class action_impl< tag::defer, mpl::vector1<Expr> >
	{
	public:
		action_impl()
			: expr_()
		{} 
		action_impl( const Expr& expr )
			: expr_( expr )
		{}
		template< typename Args >
		explicit action_impl( Args& args )
			: expr_( ctor_mode::transfer_arg(), args )
		{}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& )const 
		{
			return expr_;
		}
	private:
		Expr expr_;
	};
	
  // �߂�l�^
	template< typename Expr >
	struct return_type_static< tag::defer, mpl::vector1<Expr> >
	{
		typedef return_lvalue<Expr> type;
	};
	
  // 
  	template< typename Extra >
  	struct value_types_static< tag::defer, Extra >
  		: public detail::value_types_all< as_any_tag, mpl::vector0<> >
  	{};

  //
	template< typename T, typename Expr >
	struct is_constructor_argument< T, tag::defer, mpl::vector0<>, mpl::vector1<Expr> >
	{
		typedef boost::is_convertible<T, Expr> type;
	};
	
  ///////// ����! ///////////
	#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(defer_, tag::defer)
	#define CLAZY_EXPR_EXTRA_T 		1
	#include "../detail/pp_front.h"
	
  // �����֐�
  	template< typename T >
  	defer_<T> defer( const functor<T>& f )
  	{
	  // �^�O�̖����I�w�肪�K�v
  		return defer_<T>( ctor_mode::transfer_arg(), f.to_derived() );	
  	}
	
}
}} /* end of namespace gobou, oden */


#endif




