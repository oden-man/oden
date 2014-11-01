#pragma once

#ifndef ODEN_KONBU_CLAZY_ACT
#define ODEN_KONBU_CLAZY_ACT

//#################################################
//
//
// --------------------------------------
// --------------------------------------
#include <boost/mpl/bool.hpp>
// --------------------------------------
//
//
//###################################################

namespace oden { namespace konbu { namespace clazy
{
namespace tag
{	
//###################################################
// アクションタグ
//	
  // プリミティブ
	template<typename T>
	struct value;
	template<typename T>
	struct reference;
	
	struct terminal;
	struct ctor_at;
	struct nttp;
	struct expr;
	
	struct argument;
  	struct nothing;
	struct defer;

  // 等価・関係演算子 == != < <= > >=
  	template<typename T>
  	struct relational;
  	
	struct equal;
	struct not_equal;
	struct less;
	struct less_equal;
	struct greater;
	struct greater_equal;
	
  // 論理演算子 && || !
  	template<typename T>
  	struct logical;
  	
  	struct and_;
  	struct or_;
  	struct not_;
  	
  // 数学的 + - * / %
  	template<typename T>
  	struct arithmetic; 
  	template<typename T>
  	struct unary_arithmetic;
  	
  	struct plus;
  	struct minus;
  	struct multiply;
  	struct divide;
  	struct modulus;
	
  // 増加・減少 ++ --
  	template<typename T>
  	struct pre_incr_decr;
  	template<typename T>
  	struct post_incr_decr;
  	
  // ビット & | ^ ~ << >> 
  	template<typename T>
  	struct bitwise;
  	
  	struct xor_;
  	struct left_shift;
  	struct right_shift;
	
  // キャスト
	template<typename T>
	struct cast;
	
	struct static_;
	struct reinterpret;
	struct dynamic;
	struct const_;
	
  // 呼び出し ()
	template< typename T >
	struct call;
	
	struct return_type_specified;
	struct return_type_not_specified;
	
  // メンバ変数 .* ->*
  	template< typename T >
  	struct pointer_to_member;
  	
  	struct through_reference;
  	struct through_pointer;
	
  // 代入演算子 = += -= *= /= %= &= |= ^= <<= >>=
	template< typename T >
  	struct assign;
	
	struct plain;
	
  // その他の演算子 * & [] 
  	struct indirect;
  	struct address;
  	struct subscript;
	
  // カンマ演算子 ,
  	struct comma;
  	
  // 三項演算子
  	struct conditional;
  	
  // コンストラクタ・デストラクタ
  	struct constructor;
  	struct destructor;
  	
  // new, delete
  	struct new_;
  	struct delete_;
  	
  // ローカルスコープ {}
  	template< typename T >
  	struct scope;
  	
  	struct return_body;
  	struct no_return;
  	
	struct local;
	struct local_binder;
	struct local_constructor;
  
  // if文 if(){...}, if(){...}else{...}
  	struct if_then;
  	struct if_then_else;
  	
  // 順次実行
  	struct progn;
  
  // STL関数
  	
}

//###########################################################
// オーバーロード可能な表現
//
namespace detail
{
	namespace mpl = boost::mpl;

	template< typename Tag >
	struct is_overloadable
		: public mpl::false_
	{};
	
	template< typename T >
	struct is_overloadable< tag::relational<T> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::logical<T> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::arithmetic<T> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::unary_arithmetic<T> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::pre_incr_decr<T> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::post_incr_decr<T> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::bitwise<T> >
		: public mpl::true_
	{};
	template<>
	struct is_overloadable< tag::pointer_to_member<tag::through_pointer> >
		: public mpl::true_
	{};
	template< typename T >
	struct is_overloadable< tag::assign<T> >
		: public mpl::true_
	{};
	template<>
	struct is_overloadable< tag::indirect >
		: public mpl::true_
	{};
	template<>
	struct is_overloadable< tag::address >
		: public mpl::true_
	{};
	template<>
	struct is_overloadable< tag::subscript >
		: public mpl::true_
	{};
	template<>
	struct is_overloadable< tag::comma >
		: public mpl::true_
	{};
}
	
}
}}

#endif

/*
procedure() = 
	on_window( mainWnd )
	[ 
		on_create()[ 
			handle_event( ) <- comma_< callr_< void, F, A1, A2,... >, assign_< handle_state_, int_::_c<HANDLED> > >
		],
		on_size()[ <- event_< equal_< message_, uint_<WM_SIZE> >, Vals,... >
			handle_event
		]
		on_scar()[
			handle_event( )
		]
	],
	on_window( subWnd )
	[
	
	]
	
	if_< Expr, Body >
	
	if_< Cond, let_< Vals,... Action > >
	
	let_helper_< Vals,... >
	{
		
		let_< Vals,..., Body > operator[]( Body )
		
		Vals v1, v2, v3;
	};
	
	event_< Cond, Vals,... >
	{
		if_< Cond, let_< Vals,..., Body > > operator[]( Body )
		{
			typedef if_then_< Cond, let_< Vals,..., Body > > ret_type:
			
			return if_type( c, let_type( v1,...v3, body ) );
		}
		
		Cond c;
		Vals v1,v2,v3;	
	};
	
	on_create -> if_< Cond, scope_gen< tag, Locals,.... > >
	
	struct value_types_static -> mpl::vector3< any, any, any >
	struct return_types_raw -> void
	struct custom_template_parameter -> defer all.
	struct impl
	
	
	template< typename Cond >


*/

