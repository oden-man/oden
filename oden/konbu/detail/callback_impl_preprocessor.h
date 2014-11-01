/*
*  callback preprocessor file
*
*
*/

#ifndef ODEN_KONBU_CALLBACK_IMPL_PP
#define ODEN_KONBU_CALLBACK_IMPL_PP

//################################################
//   インクルード
//
// ------------ boost.pp -----------
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

// ------------ boost.tt ---------------
// ----------- boost.mpl ---------------
// ------------- boost -----------------
// ------------ oden -------------------
// --------- boost.function_types ------

//
//################################################

namespace oden { namespace konbu
{	
//#############################################################################################
// テンプレートひな形
// 
	template< typename FuncPtr, int Free = 0, typename Functor = callback_param >
	struct callback
	{};
	
}} /* end of namespace oden.konbu */

#endif

//###################################################################################
// *** イテレート中 ***
//
#if defined(BOOST_PP_IS_ITERATING)
namespace oden { namespace konbu
{

// Nは確実に１以上
#define N 		BOOST_PP_ITERATION()
#define CC		BOOST_PP_SEQ_ELEM( KONBU_CC_I, ODEN_KONBU_CC )

  // 実装クラス
	// 引数からインスタンスを取る
	template< typename Ret, BOOST_PP_ENUM_PARAMS(N, typename Arg), int Free, typename ParamFunctor >
	struct callback< Ret (CC*)( BOOST_PP_ENUM_PARAMS(N, Arg) ), Free, ParamFunctor >
	{
		typedef Ret (CC *pointer)( BOOST_PP_ENUM_PARAMS(N, Arg) );
		typedef ParamFunctor param_functor;

		template< typename Proc >
		struct set
		{
			static Ret CC entry( BOOST_PP_ENUM_BINARY_PARAMS(N, Arg, arg) )
			{
				detail::select_param<Free> ParamSelect;

				const Proc* proc_ptr
					= ParamFunctor::get<const Proc*>( ParamSelect(BOOST_PP_ENUM_PARAMS(N, arg)) );

				return (*proc_ptr)( BOOST_PP_ENUM_PARAMS(N, arg) );
			}
		};
	};

	// 引数にスペースがない
	template< typename Ret, BOOST_PP_ENUM_PARAMS(N, typename Arg), typename PrF >
	struct callback< Ret (CC*)( BOOST_PP_ENUM_PARAMS(N, Arg) ), 0, PrF >
	{
		typedef Ret (CC *pointer)( BOOST_PP_ENUM_PARAMS(N, Arg) );
		typedef void param_functor;

		template< typename Proc >
		struct set
		{
			static Ret CC entry( BOOST_PP_ENUM_BINARY_PARAMS(N, Arg, arg) )
			{
				return instance_( BOOST_PP_ENUM_PARAMS(N, arg) );
			}

			static const Proc& get_instance(){ return instance_; }
			static Proc& instance(){ return instance_; }
			static void set_instance( const Proc& inst ){ instance_ = inst; }

		private:
			static Proc instance_;
		};
	};
	
	template< typename R, BOOST_PP_ENUM_PARAMS(N, typename A), typename F >
	template< typename Proc >
	Proc callback<R (CC*)( BOOST_PP_ENUM_PARAMS(N, A) ), 0, F>::set<Proc>::instance_;

#undef N
#undef CC

}}
#endif
