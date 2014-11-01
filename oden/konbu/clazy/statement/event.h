#pragma once

#ifndef ODEN_KONBU_CLAZY_STATEMENT_EVENT
#define ODEN_KONBU_CLAZY_STATEMENT_EVENT

//#######################################
//
//
// ---------- STL -----------
// ---------- boost ---------
// ---------- oden ------------
// ---------- konbu -----------
#include "if.h"
#include "../scope/let.h"

//
//#######################################

/*
*
*	event = if_ + local_block_
*
*/

namespace oden { namespace konbu { namespace clazy
{
//##################################################################
// 文生成クラス
//
  //
	namespace tag { struct if_block; }

  //
	template< typename Operands >
	struct composite_traits< tag::statement_gen<tag::if_block>, Operands >
	{
		template< typename Body >
		struct result
		{
			typedef if_< 
						typename mpl::at_c<Operands,0>::type,
						typename as_local_block_<
							typename mpl::push_back<
								typename mpl::pop_front<Operands>::type, Body
							>::type
						>::type
					>
				type;
		};

		template< typename Ret, typename Ops, typename Body >
		Ret compose( const Ops& ops, const Body& body )const
		{
			

			return Ret( ops.get<0>(), local_block_( ctor_mode::copy_operand, locals )[ body ] );
		}
	};

  //
	#define CLAZY_EXPR 				CLAZY_COMPOSITE(if_else_gen_, tag::statement_gen<tag::if_then_else>, 2)
	#include "../detail/pp_front.h"

	template< typename Cond, BOOST_PP_ENUM_BINARY_PARAMS(KONBU_CLAZY_OPERAND_LIMIT, typename Local,= detail::unspecified BOOST_PP_INTERCEPT) >
	class if_block_gen
	{
	public:
	// ローカルブロック型
		scope_gen< detail::gen_local_block, BOOST_PP_ENUM_PARAMS(KONBU_CLAZY_OPERAND_LIMIT, Local) > block_gen_type;
		
	// コンストラクタ
		if_block_gen()
			: cond_(), block_gen_()
			
	#define CTOR(z,n,_)\
		template< BOOST_PP_ENUM_PARAMS(n, typename Arg) > 							\
		explicit if_block_gen( BOOST_PP_ENUM_BINARY_PARAMS(n, const Arg, &arg) )	\
			: cond_( BOOST_PP_ENUM_PARAMS(n, arg) ), 								\
			  block_gen_( BOOST_PP_ENUM_PARAMS(n, arg) ) 							\
		{}
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_CTOR_LIMIT), CTOR, _)
	#undef  CTOR

	// []
		template< typename Body >
		if_< Cond, typename block_gen_type::template gen_ret<Body>::type > 
		operator[]( const Body& body )const
		{
			return _if( cond_ )[ block_gen_[ body ] ];
		}
	
	private:
		Cond 		  	cond_;
		block_gen_type	block_gen_;
	};
	
	template< typename A,.... >
	composer< >
	{
		composer_traits< A,... > tr;
		
		tr::ret< X >::type operator[ ]( const X& x )
		{
			return tr::apply( a,... );
		}
		
	private:
		A,... a...;
	};

}
}} /* end of namespace gobou, oden */

#endif


