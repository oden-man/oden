#pragma once

#ifndef ODEN_KONBU_CLAZY_STATEMENT_IF
#define ODEN_KONBU_CLAZY_STATEMENT_IF

//#######################################
//
//
// ---------- STL -----------
// ---------- boost ---------
// ---------- oden ------------
// ---------- konbu -----------
#include "statement_common.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//##################################################################
// if_then_else
//
  // テンプレート引数を加工 - Bodyが二つ
  	template< typename Cond, typename Body1, typename Body2 >
  	struct custom_template_parameter< tag::statement<tag::if_then_else>, mpl::vector3<Cond, Body1, Body2> >
  	{
  		// 両bodyの評価を遅延する
  		typedef mpl::vector3< Cond, defer_<Body1>, defer_<Body2> > operands;
  		
  		typedef mpl::vector0<> extra;	
  	};
  	
  // 実装
	template<>
	class action_impl< tag::statement<tag::if_then_else> >
	{
	public:
		action_impl(){}
		template< typename Args >
		explicit action_impl( Args& ){}		
			
	  // apply
		template< typename Ret, typename Cond, typename Body1, typename Body2 >
		Ret apply( Cond cond, Body1 body1, Body2 body2, const Context& context )const
		{
			if( cond )
				body1.call( context );
			else
				body2.call( context );
		}
	};
	
  ///////// 生成! ///////////
	#define CLAZY_EXPR 				CLAZY_CALLABLE(if_else_, tag::statement<tag::if_then_else>, 3)
	#include "../detail/pp_front.h"
	
  ////////////////////////////////////
  // 文生成クラス
  //

  //
	template< typename Operands >
	struct composite_traits< tag::statement_gen<tag::if_then_else>, Operands >
	{
		template< typename Body >
		struct result
		{
			typedef if_else_< 
						typename mpl::at_c<Operands,0>::type, 
						typename mpl::at_c<Operands,1>::type > type;
		};

		template< typename Ret, typename Ops, typename Body >
		Ret compose( const Ops& ops, const Body& body )const
		{
			return Ret( ops.template get<0>(), ops.template get<1>(), body );
		}
	};

  //
	#define CLAZY_EXPR 				CLAZY_COMPOSITE(if_else_gen_, tag::statement_gen<tag::if_then_else>, 2)
	#include "../detail/pp_front.h"
	
//##################################################################
// if_then
//	
  // 実装
	template<>
	class action_impl< tag::statement<tag::if_then> >
	{
	public:
		action_impl(){}
		template< typename Args >
		explicit action_impl( Args& ){}		

	  // apply
		template< typename Ret, typename Cond, typename Body >
		Ret apply( Cond cond, Body body, const Context& context )const
		{
			if( cond )
				body.call( context );
		}
	};
	
  ///////// 生成! //////////////
	#define CLAZY_EXPR 				CLAZY_CALLABLE(if_, tag::statement<tag::if_then>, 2)
	#define CLAZY_EXPR_MEMBER_1		if_else_gen<Arg0, Arg1> _else()const;		
	#include "../detail/pp_front.h"
	
  /// else_genにつなげる関数を実装
	template< typename Cond, typename Body >
	const if_else_gen_<Cond, Body> if_<Cond,Body>::_else()const
	{
		return if_else_gen_<Cond, Body>( this->get_operand_at<0>(), this->get_operand_at<1>() );
	}
	
  ////////////////////////////////////
  // 文生成クラス
  //

  //
	template< typename Operands >
	struct composite_traits< tag::statement_gen<tag::if_then>, Operands >
	{
		template< typename Body >
		struct result
		{
			typedef if_< typename mpl::at_c<Operands,0>::type > type;
		};

		template< typename Ret, typename Ops, typename Body >
		Ret compose( const Ops& ops, const Body& body )const
		{
			return Ret( ops.template get<0>(), body );
		}
	};

  //
	#define CLAZY_EXPR 		CLAZY_COMPOSITE(if_gen_, tag::statement_gen<tag::if_then>, 1)
	#define CLAZY_SYMBOL	_if
	#include "../detail/pp_front.h"

}
}} /* end of namespace gobou, oden */

#endif


