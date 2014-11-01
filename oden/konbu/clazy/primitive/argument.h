#pragma once

#ifndef ODEN_KONBU_CLAZY_ARG
#define ODEN_KONBU_CLAZY_ARG

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
#include <boost/type.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>

// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{	
	namespace detail
	{
		struct not_enough_actual_parameters
		{};
	}

//##################################################################
// 実引数を取り出す
//	
	template< typename Index >
	class action_impl< tag::argument, mpl::vector1<Index> >
	{
	public:
		action_impl(){}
		template< typename Args >
		explicit action_impl( Args& ){}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& context )const 
		{
			return checked_at<Ret>( context, boost::type<Ret>() );
		}
		
	private:
	// エラー戻り値か、オーバーロードで分別する
		template< typename Ret, typename Context, typename CheckT >
		Ret checked_at( const Context& context, CheckT )const 
		{
			return context.template arg_at<Index::value-1>();
		}
		template< typename Ret, typename Context >
		Ret checked_at( const Context&, boost::type<detail::not_enough_actual_parameters> )const
		{
			return detail::not_enough_actual_parameters();
		}
	};
	
	template< typename Context, typename Index >
	struct return_type_raw< tag::argument, mpl::vector0<>, Context, mpl::vector1<Index> >
	{	
		typedef typename Context::call_args call_args;
	
		typedef 
		typename mpl::eval_if< mpl::less< mpl::size<call_args>, Index >,
						mpl::identity< detail::not_enough_actual_parameters >,
						mpl::at_c< call_args, Index::value-1 >  
					>::type
				raw_type;
				
		typedef 
		typename mpl::if_< boost::is_same< raw_type, detail::not_enough_actual_parameters >,
						return_rvalue< raw_type >,
						return_lvalue< raw_type >
					>::type
				type;
	};
	
	template< typename Extra >
	struct value_types_static< tag::argument, Extra >
	{
		typedef mpl::vector0<> type;
	};
	
//##################################################################
// 式クラス
//
	///////// 生成! ///////////
	#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(arg_, tag::argument)
	#define CLAZY_EXPR_EXTRA_NT 	(int)
	#include "../detail/pp_front.h"

	// もしこれらを左辺値としてアドレスをとったらODR違反になる！ -> でもstatelessだから実質大丈夫か？
	namespace arg_names
	{
	#define ARGUMENT(z,n,NAME) const arg_<n> NAME##n;
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_ARG_LIMIT), ARGUMENT, _)
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_ARG_LIMIT), ARGUMENT, arg)
	#undef  ARGUMENT
	}

}
}} /* end of namespace gobou, oden */


#endif




