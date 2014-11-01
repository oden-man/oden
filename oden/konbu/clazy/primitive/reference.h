#pragma once

#ifndef ODEN_KONBU_CLAZY_PRIM_REF
#define ODEN_KONBU_CLAZY_PRIM_REF

//#######################################
//
//
// ---------- STL -----------
// ---------- boost ---------
#include <boost/ref.hpp>
// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//##################################################################
// 基本共通
//
  // value_types - nullary
  	template< typename AnyAction, typename Extra >
  	struct value_types_static< tag::reference<AnyAction>, Extra >
  	{
  		typedef mpl::vector0<> type;
  	};
  	
  // return_type - lvalue
	template< typename AnyAction, typename Extra >
	struct return_type_static< tag::reference<AnyAction>, Extra >
	{
		typedef typename mpl::at_c<Extra, 0>::type value_type;
		
		typedef return_lvalue<value_type&> type;
	};
	
//##################################################################
// 構築時に値を確定
//
	template< typename Type >
	class action_impl< tag::reference<tag::terminal>, mpl::vector1<Type> >
	{
	public:
		explicit action_impl( Type& ref )
			: ref_( ref )
		{}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& )const {
			return ref_;
		}
		
	private:
		Type& ref_;
	};
	
	template< typename T, typename RawType >
	struct is_constructor_argument< T, tag::reference<tag::terminal>, mpl::vector0<>, mpl::vector1<RawType> >
	{
		typedef boost::is_convertible<T, RawType&> type;
	};
	
//##################################################################
// 指定のコンストラクタ引数から値を確定
//
	template< typename Type, typename Index >
	class action_impl< tag::reference<tag::ctor_at>, mpl::vector2<Type,Index> >
	{
	public:
		template< typename Sequence >
		explicit action_impl( const Sequence& seq )
			: ref_( fusion::at_c<Index::value-1>( seq ) )
		{}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& )const {
			return ref_;
		}
		
	private:
		Type& ref_;
	};
	
//##################################################################
// コンパイル時に値を確定
//
	template< typename Type, typename Ref >
	class action_impl< tag::reference<tag::nttp>, mpl::vector2<Type,Ref> >
	{
	public:
		action_impl(){}
		template< typename A > explicit action_impl( A& ){}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& )const {
			return Ref::value;
		}
	};

//##################################################################
// 式クラス
//
	template< typename RefT >
	struct ref_base
	{
		///////// コンストラクト時 - 直接 ///////////
		#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(_, tag::reference<tag::terminal>)
		#define CLAZY_EXPR_LOCAL_T  	(RefT)
		#include "../detail/pp_front.h"
		
		///////// コンストラクト時 - 引数から ///////////
		#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(at, tag::reference<tag::ctor_at>)
		#define CLAZY_EXPR_LOCAL_T   	(RefT)
		#define CLAZY_EXPR_EXTRA_NT 	(int)
		#include "../detail/pp_front.h"
		
		// typedef
	#define AT(z,n,_) typedef at<n> _##n;
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_CTOR_LIMIT), AT, _)
	#undef  AT
		
		///////// コンパイル時 ///////////
		#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(_c, tag::reference<tag::nttp>)
		#define CLAZY_EXPR_LOCAL_T   	(RefT)
		#define CLAZY_EXPR_EXTRA_NT 	(RefT&)
		#include "../detail/pp_front.h"
	};
  
  // const / non-constを切り替える
	template< typename T >
	struct ref_
		: public ref_base< T >
	{};
	
	template< typename T >
	struct cref_
		: public ref_base< const T >
	{};
	
  //  ファンクタをつくる
  //
	template< typename T >
	typename ref_<T>::_ ref( T& v )
	{
		return ref_<T>::_(ctor_mode::transfer_arg(), boost::ref(v));
	}
	template< typename T >
	typename cref_<T>::_ cref( const T& v )
	{
		return cref_<T>::_(ctor_mode::transfer_arg(), v);
	}
	
}
}} /* end of namespace gobou, oden */

#endif




