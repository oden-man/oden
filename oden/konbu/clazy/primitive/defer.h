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
// ローカル変数の宣言子
//	
  // コンストラクタではExprに転送をし、実行するとExprを返す
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
	
  // 戻り値型
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
	
  ///////// 生成! ///////////
	#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(defer_, tag::defer)
	#define CLAZY_EXPR_EXTRA_T 		1
	#include "../detail/pp_front.h"
	
  // 生成関数
  	template< typename T >
  	defer_<T> defer( const functor<T>& f )
  	{
	  // タグの明示的指定が必要
  		return defer_<T>( ctor_mode::transfer_arg(), f.to_derived() );	
  	}
	
}
}} /* end of namespace gobou, oden */


#endif




