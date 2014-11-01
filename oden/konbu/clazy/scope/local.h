#pragma once

#ifndef ODEN_KONBU_CLAZY_SCOPE_LOCALVAR
#define ODEN_KONBU_CLAZY_SCOPE_LOCALVAR

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
#include <boost/type.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/at_key.hpp>

#include <boost/fusion/include/as_vector.hpp>

#include <boost/mpl/print.hpp>

// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{	
namespace detail
{
  // エラー
	template< typename Name >
	struct local_name_not_found : public mpl::pair<Name,Name>
	{};
	
//##################################################################
// ローカル変数の束縛を表す
// x = a / x = {a,b,c,d}	
//
	template< typename Name, typename Type, typename RightExpr >
	class local_binding
	{
	public:
		typedef Name name_tag;
		typedef Type local_type;
		
	// コンストラクタ
		template< typename RightExpr >
		local_binding( const RightExpr& r )
			: right_(r)
		{}
		
	// 構築引数
		RightExpr right_;
	}; 	
}

//##################################################################
// ローカル変数を代入式で宣言
//	
  // ローカル変数のコピー引数を返す
	template< typename Name >
	class action_impl< tag::local_binder, mpl::vector1<Name> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Expr, typename Context >
		Ret apply( Expr expr, const Context& )const 
		{
			return fusion::vector1<Expr>(expr);
		}
	};
	
  // 値型 - 全てas_rvalue
  	template< typename Operands, typename Context, typename Extra >
  	struct value_types_raw< tag::local_binder, Operands, Context, Extra >
  		: public 
  		detail::value_types_all< as_rvalue_tag, Operands >
  	{};
	
  // 戻り値型
	template< typename Expr, typename Context, typename Name >
	struct return_type_raw< tag::local_binder, mpl::vector1<Expr>, Context, mpl::vector1<Name> >
	{
		typedef typename detail::get_results<Expr, Context>::type expr_ret;
		
		typedef detail::local_binding<Name, typename expr_ret::raw, fusion::vector1<typename expr_ret::as_rvalue>> bindi_type;
		
		typedef return_rvalue< bindi_type > type;
	};

//##################################################################
// ローカル変数を構築式で宣言
//	
  // ローカル変数のコンストラクタ引数をまとめて返す
	template< typename Name, typename Type >
	class action_impl< tag::local_constructor, mpl::vector2<Name,Type> >
	{
	public:
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& )const
		{
			return fusion::vector0<>();
		}
		
	#define APPLY(z,n,_) \
		template< typename Ret, BOOST_PP_ENUM_PARAMS(n, typename Arg), typename Context > 	\
		Ret apply( BOOST_PP_ENUM_BINARY_PARAMS(n, Arg, arg), const Context& )const 			\
		{																					\
			return fusion::vector##n< BOOST_PP_ENUM_PARAMS(n, Arg) >						\
			 											( BOOST_PP_ENUM_PARAMS(n, arg) );	\
		}
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_OPERAND_LIMIT), APPLY, _)
	#undef  APPLY
	
	};
	
  // 値型 - 全てas_rvalue
  	template< typename Operands, typename Context, typename Extra >
  	struct value_types_raw< tag::local_constructor, Operands, Context, Extra >
  		: public 
  		detail::value_types_all< as_rvalue_tag, Operands >
  	{};
	
  // 戻り値型
	template< typename Operands, typename Context, typename Name, typename Type >
	struct return_type_raw< tag::local_constructor, Operands, Context, mpl::vector2<Name,Type> >
	{	
		typedef typename mpl::transform< 
							Operands, 
							detail::get_result_as_rvalue< mpl::_1, mpl::protect<Context> >
						>::type
					operands_forwarding_result;
		
		typedef detail::local_binding<
			Name, Type,
			typename fusion_ret::as_vector< operands_forwarding_result >::type
		> 
		var_type;
		
		typedef return_rvalue< var_type > type;
	};

//##################################################################
// 評価時にローカル変数を参照する
//	
  // 動作
	template< typename Name >
	class action_impl< tag::local, mpl::vector1<Name> >
	{
	public:
		action_impl(){}
		template< typename Args >
		explicit action_impl( Args& ){}
		
		template< typename Ret, typename Context >
		Ret apply( const Context& context )const 
		{
			return apply_impl<Ret>( context, boost::type<Ret>() );
		}
		
	private:
		template< typename Ret, typename Context >
		Ret apply_impl( const Context& context, boost::type< detail::local_name_not_found<Name> > )const 
		{
			return detail::local_name_not_found<Name>();
		}
		template< typename Ret, typename Context, typename Test >
		Ret apply_impl( const Context& context, Test )const 
		{
			return context.local_at<Name>();
		}
	};
	
  // return_type 
	template< typename Context, typename Name >
	struct return_type_raw< tag::local, mpl::vector0<>, Context, mpl::vector1<Name> >
	{
	private:
		// fusion::mapです。
		typedef typename Context::locals_map locals_map;

		// 参照するローカル変数を取り出す : 見つからなければ、エラー型を返す
		typedef typename mpl::eval_if< fusion_ret::has_key< locals_map, Name >,
							fusion_ret::at_key< locals_map, Name >,
							mpl::identity< detail::local_name_not_found<Name> >
						>::type
					found_type;
		
	public:
		typedef 
		typename mpl::if_< boost::is_same< found_type, detail::local_name_not_found<Name> >,
					return_rvalue< detail::local_name_not_found<Name> >,
					return_lvalue< found_type >
				>::type
			type;
	};

//##################################################################
//  ユーザーインターフェース
//	
	namespace local_id
	{
		struct a {};
		struct b {};
		struct c {};
		
		struct i {};
		struct j {};
		struct k {};
		
		struct x {};
		struct y {};
		struct z {};
	}
	
	template< typename Name >
	struct local_
	{
		typedef local_<Name> this_local;

	  ///////// 生成! ///////////
		#define CLAZY_EXPR 				CLAZY_UNARY_CALLABLE(bind, tag::local_binder)
		#define CLAZY_EXPR_LOCAL_T 		(Name)
		#define CLAZY_SYMBOL			_bind_impl
		#include "../detail/pp_front.h"
		
	  ///////// 生成! ///////////
		#define CLAZY_EXPR 				CLAZY_VARIADIC_CALLABLE(construct, tag::local_constructor)
		#define CLAZY_EXPR_LOCAL_T 		(Name)
		#define CLAZY_EXPR_EXTRA_T 		1
		#define CLAZY_SYMBOL_VARIADIC	_ctor_impl
		#include "../detail/pp_front.h"
		
	  ///////// 生成! ///////////
		#define CLAZY_EXPR 				CLAZY_NULLARY_CALLABLE(_, tag::local)
		#define CLAZY_EXPR_LOCAL_T 		(Name)

		// メンバ生成関数
		#define CLAZY_EXPR_MEMBER_1 \
			template< typename X > bind<X> _bind( const X& x )const{ return this_local()._bind_impl<X>(x); }
		
		#define CLAZY_EXPR_MEMBER_2 \
			template< typename T > construct<T> _ctor()const{ return this_local()._ctor_impl<T>(); }
		
		#define MEM_CTOR(z,n,_)	\
			template< typename T, BOOST_PP_ENUM_PARAMS(n, typename ctorA) >				\
			construct< T, BOOST_PP_ENUM_PARAMS(n, ctorA) >								\
			_ctor( BOOST_PP_ENUM_BINARY_PARAMS(n, const ctorA, &ctora) )				\
			{																			\
				return this_local()._ctor_impl<T>( BOOST_PP_ENUM_PARAMS(n, ctora) );	\
			}
		#define CLAZY_EXPR_MEMBER_3	\
			BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(N), MEM_CTOR, _)
		
		// 生成
		#include "../detail/pp_front.h"

		#undef MEM_CTOR
	};

	typedef local_<local_id::a> local_a_;
	typedef local_<local_id::b> local_b_;
	typedef local_<local_id::c> local_c_;
	
	typedef local_<local_id::i> local_i_;
	typedef local_<local_id::j> local_j_;
	typedef local_<local_id::k> local_k_;
	
	typedef local_<local_id::x> local_x_;
	typedef local_<local_id::y> local_y_;
	typedef local_<local_id::z> local_z_;

	// 
#ifndef ODEN_KONBU_CLAZY_NO_PREDEFINED_TERMINAL
	const local_a_::_  _a;
	const local_b_::_  _b;
	const local_c_::_  _c;
	
	const local_i_::_  _i;
	const local_j_::_  _j;
	const local_k_::_  _k;
	
	const local_x_::_  _x;
	const local_y_::_  _y;
	const local_z_::_  _z;
#endif
  
  // 指定のファンクタと同じローカル変数体となる
	template< typename F >
	struct same_local_
		: public local_< typename mpl::at_c<typename F::extra_args, 0>::type >
	{};

}
}} /* end of namespace gobou, oden */


#endif




