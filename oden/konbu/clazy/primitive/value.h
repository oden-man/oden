#pragma once

#ifndef ODEN_KONBU_CLAZY_PRIM_VALUE
#define ODEN_KONBU_CLAZY_PRIM_VALUE

// 
#define CLAZY_VALUE_BEING_INVOKED

//#######################################
//
//
// ---------- STL -----------
// ---------- boost ---------
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/pop_front.hpp>

// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"
	
#include <oden/konbu/clazy/detail/is_nttp_type.h>

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{		
//##################################################################
// 基本共通
//
  // value_types - nullary
  	template< typename AnyAction, typename Extra >
  	struct value_types_static< tag::value<AnyAction>, Extra >
  	{
  		typedef mpl::vector0<> type;
  	};
  	
  // return_type - lvalue
  	template< typename AnyAction, typename Extra >
	struct return_type_static< tag::value<AnyAction>, Extra >
	{
		typedef return_lvalue<
					typename mpl::at_c<Extra, 0>::type
				>	 
			type;
	};
	
  // 型に若干変換を加える
  	template< typename AnyAction, typename Extra >
	struct custom_template_parameter< tag::value<AnyAction>, mpl::vector0<>, Extra >
	{
		typedef mpl::vector0<> operands;
		
		typedef typename mpl::at_c<Extra, 0>::type value_type;
		
		typedef typename mpl::push_front<
					typename mpl::pop_front<Extra>::type,		// 配列 -> ポインタ, さらにconstを足す
					typename boost::add_const< typename boost::decay<value_type>::type >::type
				>::type
			extra;
	};
	
//##################################################################
// 構築時に値を直接とる
//
	template< typename Type >
	class action_impl< tag::value<tag::terminal>, mpl::vector1<Type> >
	{
	public:
		action_impl()
			: value_()
		{}
		explicit action_impl( typename boost::call_traits<Type>::param_type v )
			: value_(v)
		{}
		
		template< typename Ret, typename Args >
		Ret apply( const Args& )const {
			return value_;
		}
		
	private:
		Type value_;
	};
	
	template< typename T, typename ValueType >
	struct is_constructor_argument< T, tag::value<tag::terminal>, mpl::vector0<>, mpl::vector1<ValueType> >
	{
		typedef boost::is_convertible<T, ValueType> type;
	};
	
//##################################################################
// 指定されたコンストラクタ引数から値とる
//
	template< typename Type, typename Index >
	class action_impl< tag::value<tag::ctor_at>, mpl::vector2<Type,Index> >
	{
	public:
		action_impl()
			: value_()
		{}
		template< typename Sequence >
		explicit action_impl( const Sequence& seq )
			: value_( fusion::at_c<Index::value-1>( seq ) )
		{}
		
		template< typename Ret, typename Args >
		Ret apply( const Args& )const {
			return value_;
		}
		
	private:
		Type value_;
	};
	
/*
	call_< f::_1, val::_2, val::_<90>, val::_3 > x;
	x( fn, 20, 30 ); -> x( transfer, fusion(fn,20,30) ) -> OK.
	
	call_< f::_, val::_, val::_<90>, val::_ > x;
	x( fn, 20, 30 ); -> x( operand_copy, fusion(fn,20,30) ) -> OK.
*/

//##################################################################
// コンパイル時に値を確定
//
	template< typename Type, typename Value >
	class action_impl< tag::value<tag::nttp>, mpl::vector2<Type,Value> >
	{
	public:
		action_impl(){}
		template< typename A > explicit action_impl( A& ){}
		
		template< typename Ret, typename Args >
		Ret apply( const Args& )const {
			return Value::value;
		}
	};
	
	template< typename Type, typename Value >
	struct custom_template_parameter< tag::value<tag::nttp>, mpl::vector0<>, mpl::vector2<Type,Value> >
	{
		typedef mpl::vector0<> operands;
		typedef mpl::vector2<
					typename boost::decay<Type>::type, 
					Value
				>
			extra;
	};
	
	template< typename Type, typename Value >
	struct return_type_static< tag::value<tag::nttp>, mpl::vector2<Type,Value> >
	{
		typedef return_rvalue<Type> type;
	};

//##################################################################
// 式が渡された場合、評価して値を返す
//
	template< typename Expr >
	class action_impl< tag::value<tag::expr>, mpl::vector1<Expr> >
	{
	public:
		action_impl(){}
		
		explicit action_impl( const Expr& v )
			: e_(v)
		{}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& ctx )const {
			typedef typename detail::get_result_as_rvalue<Expr,Context>::type ret_t;
			return e_.eval<ret_t>( ctx );
		}
	
	private:
		Expr e_;
	};
	
	template< typename Type, typename Context, typename Unused >
	struct return_type_raw< tag::value<tag::expr>, Unused, Context, mpl::vector1<Type> >
	{
		typedef typename detail::get_result_as_rvalue<Type,Context>::type ret_type;	
		typedef return_rvalue<ret_type> type;
	};
	
	// 変更はなし
  	template<  typename Extra >
	struct custom_template_parameter< tag::value<tag::expr>, mpl::vector0<>, Extra >
	{
		typedef mpl::vector0<> operands;
		typedef Extra extra;
	};
	
//##################################################################
// フロントクラスを定義
//
	template< typename ValT >
	struct val_base
	{
		///////// 生成! ///////////
		#define CLAZY_EXPR 			CLAZY_NULLARY_CALLABLE(_, tag::value<tag::terminal>)
		#define CLAZY_EXPR_LOCAL_T   (ValT)
		#include "../detail/pp_front.h"

		///////// 生成! ///////////
		#define CLAZY_EXPR 			CLAZY_NULLARY_CALLABLE(at, tag::value<tag::ctor_at>)
		#define CLAZY_EXPR_LOCAL_T   (ValT)
		#define CLAZY_EXPR_EXTRA_NT 	(int)
		#include "../detail/pp_front.h"
		
		// typedef
	#define AT(z,n,_) typedef at<n> _##n;
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_CTOR_LIMIT), AT, _)
	#undef  AT
	};
	
  // Tはコンパイル時定数になれない
	template< typename ValT, bool T_canbeNTTParam = detail::is_nttp_type<ValT>::value >
	struct val_
		: public val_base<ValT>
	{};

  // Tはコンパイル時定数になれる
	template< typename ValT >
	struct val_<ValT,true>
		: public val_base<ValT>
	{
	   ///////// 生成! ///////////
	   #define CLAZY_EXPR 			CLAZY_NULLARY_CALLABLE(_c, tag::value<tag::nttp>)
	   #define CLAZY_EXPR_LOCAL_T   (ValT)
	   #define CLAZY_EXPR_EXTRA_NT 	(ValT)
	   #include "../detail/pp_front.h"
	};
	
    // 式の値
	///////// 生成! ///////////
	#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(expr_val_, tag::value<tag::expr>)
	#define CLAZY_EXPR_EXTRA_T 1
	#include "../detail/pp_front.h"
	
	
  // ファンクタをつくる
  //
	template< typename T >
	typename val_<T>::_ val( const T& v, typename boost::disable_if< detail::is_functor<T> >::type* = 0 )
	{
	  // タグの明示的指定が必要
		return val_<T>::_( ctor_mode::transfer_arg(), v );
	}
	
	template< typename T >
	expr_val_<T> val( const functor<T>& v )
	{
	  // タグの明示的指定が必要
		return expr_val_<T>( ctor_mode::transfer_arg(), v.to_derived() );
	}
	
//##################################################################
//  いろんなtypedef
//	
  // nttpになれる
	typedef val_<char>                    char_;	
	typedef val_<short>                  short_;
	typedef val_<int>                      int_;
	typedef val_<long>                    long_;
	typedef val_<signed char>        schar_;
	typedef val_<unsigned char>     uchar_;
	typedef val_<unsigned short>   ushort_;
	typedef val_<unsigned int>       uint_;
	typedef val_<unsigned long>     ulong_;
	typedef val_<bool>                    bool_;
	
	typedef bool_::_c<true>            true_;
	typedef bool_::_c<false>          false_;
	
  // nttpになれない
  	typedef val_<float>				float_;
  	typedef val_<double>			double_;
	
//##################################################################
// 値をファンクタにつつむ
//
namespace detail
{
  // メタ関数
	template< typename X >
	struct value_wrapper
		: public mpl::if_<
					detail::is_functor<X>,
					mpl::identity<X>,
					mpl::identity<typename val_<X>::_>
				>::type
	{};
	
  // 関数
	template< typename X >
	const X& wrap_value( const functor<X>& x )
	{
		return x.to_derived();	
	}
	template< typename X >
	typename val_<X>::_ wrap_value( const X& x, 
		const typename boost::disable_if< detail::is_functor<X> >::type* = 0 
	)
	{
		return val(x);
	}
}


	
}
}} /* end of namespace gobou, oden */

// 脱出
#undef CLAZY_VALUE_BEING_INVOKED	

#endif




