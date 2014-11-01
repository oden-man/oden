
// (一回定義で十分)
#ifndef ODEN_KONBU_CLAZY_DT_PPFRONT_FWD_INCLUDED
#define ODEN_KONBU_CLAZY_DT_PPFRONT_FWD_INCLUDED
  
//#######################################################################
// 必要なインクルード
//  
  // limit
	#include <oden/konbu/clazy/limits.h>
	
  // fusion containers	
	#pragma message("	fusion containers compiling")
	#if   (FUSION_VECTOR_MAX<=10)
	#	include <boost/fusion/include/vector10.hpp>
	#elif (FUSION_VECTOR_MAX<=20)
	#	include <boost/fusion/include/vector20.hpp>
	#else
	#	include <boost/fusion/include/vector.hpp>
	#endif
	#include <boost/fusion/include/map.hpp>
	#pragma message("	fusion containers compile end")
	
  // boost.pp include
	#include <boost/preprocessor/array/elem.hpp>
	#include <boost/preprocessor/seq/elem.hpp>
	#include <boost/preprocessor/seq/size.hpp>
	#include <boost/preprocessor/seq/enum.hpp>
	#include <boost/preprocessor/arithmetic/add.hpp>
	#include <boost/preprocessor/logical/and.hpp>
	#include <boost/preprocessor/repetition/enum.hpp>
	#include <boost/preprocessor/repetition/enum_params.hpp>
	#include <boost/preprocessor/repetition/enum_binary_params.hpp>
	#include <boost/preprocessor/repetition/repeat_from_to.hpp>
	#include <boost/preprocessor/cat.hpp>
	#include <boost/preprocessor/punctuation/comma_if.hpp>
	#include <boost/preprocessor/facilities/intercept.hpp>
	
  // boost.mpl
	#include <boost/mpl/vector.hpp>
	#include <boost/mpl/copy.hpp>
	#include <boost/mpl/remove.hpp>
	#include <boost/mpl/transform.hpp>
	#include <boost/mpl/if.hpp>
	#include <boost/mpl/integral_c.hpp>
	#include <boost/mpl/iterator_range.hpp>
	#include <boost/mpl/begin.hpp>
	#include <boost/mpl/advance.hpp>
	
  // boost.tt
	#include <boost/type_traits/is_integral.hpp>
	
  // boost
	#include <boost/type.hpp>
	#include <boost/utility/enable_if.hpp>
	
  // clazy
	#include <oden/konbu/clazy/detail/operand_container.h>
	#include <oden/konbu/clazy/functor.h>
	#include <oden/konbu/clazy/detail/actions.h>

//
namespace oden { namespace konbu{ namespace clazy
{	
	namespace mpl = boost::mpl;

//############################################################
// 型引数に処理を加えるためのクラステンプレート. デフォルトで何もしない
//	
	template< typename Tag, typename RawOperands, typename RawExtra = mpl::vector0<> >
	struct custom_template_parameter
	{
		typedef RawOperands operands;	
		typedef RawExtra 	extra;
	};
	
//############################################################
// 型引数に処理を加えるためのクラステンプレート. デフォルトで何もしない
//	
	template< typename T, typename Tag, typename Operands, typename Extra = mpl::vector0<> >
	struct is_constructor_argument
		: public mpl::false_	
	{};
	
namespace detail
{
//############################################################
// nttpのホルダ  
//	
    // 外で定義するほかなく、定数式にはなれない
	template< typename T, T Val >
	struct nttp_constant
	{
		static const T value;
	};
	template< typename T, T Val >
	const T nttp_constant<T,Val>::value = Val;
	
    // nttp
	template< typename T, T Val >
	struct nttp 
		: public mpl::if_< boost::is_integral<T>,
					mpl::integral_c<T, Val>,
					nttp_constant<T, Val>
				>::type
	{};
	
//#######################################################################
// SFINAE用functor
//
	template< typename T >
	struct is_functor
		: public boost::is_convertible< T, functor<T> >
	{};
	
//#######################################################################
// 演算子オーバーロードを助ける、SFINAE用enabler
//
	template< typename T, typename Args >
	struct enable_always
		: public mpl::identity<T>
	{};

	template< typename T, typename Args, typename R = typename mpl::at_c<Args,1>::type >
	struct disable_if_right_func
		: public boost::disable_if< detail::is_functor<R>, T >
	{};
	
	template< typename T, typename Args, typename L = typename mpl::at_c<Args,0>::type >
	struct disable_if_left_func
		: public boost::disable_if< detail::is_functor<L>, T >
	{};
	
} // detail
	
//###############################################################
//  右辺値を参照で受け取れない問題を解決
//
	template< typename T >
	T& help_rvalue_deduction( T& v ) {
		return v;
	}
	
	template< typename T >
	const T& help_rvalue_deduction( const T& v ) {
		return v;
	}

}}}

#endif /* ODEN_KONBU_CLAZY_DT_PPFRONT_INCLUDED */




