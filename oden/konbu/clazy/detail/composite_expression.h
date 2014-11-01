#if !BOOST_PP_IS_ITERATING

// 
#ifndef ODEN_KONBU_CLAZY_DT_COMPOSITE_EXPR
#define ODEN_KONBU_CLAZY_DT_COMPOSITE_EXPR

// 
#include <oden/konbu/clazy/detail/pp_front_fwd.h>

// boost.pp
#include <boost/preprocessor/iteration/iterate.hpp>

namespace oden { namespace konbu { namespace clazy 
{
//#######################################################################
// traits
//
	template< typename Tag, typename Operands, typename Extra = mpl::vector0<> >
	struct composite_traits
	{};
	
namespace detail
{	
//#######################################################################
// テンプレート部分
//
  // 原型	
	template< typename Derived, typename Tag, typename Parameters, int Arity = mpl::size<typename Parameters::operands>::value >
	class composite_impl
	{};
	
  // 0~MAXまで生成
  	#define BOOST_PP_FILENAME_1 <oden/konbu/clazy/detail/composite_expression.h>
  	#define BOOST_PP_ITERATION_LIMITS ( 1, KONBU_CLAZY_OPERAND_LIMIT )
	#include BOOST_PP_ITERATE()
}

}
}}

//#######################################################################
// ユーティリティ マクロ定義
//
#define CLAZY_COMPOSITE( name, tag, arity ) 		( 4, ( name, tag, arity, composite_impl ) )
#define CLAZY_VARIADIC_COMPOSITE( name, tag )		CLAZY_COMPOSITE( name, tag, KONBU_CLAZY_OPERAND_LIMIT )

#endif /* インクルードガード */



#else 
//#######################################################################
// *** イテレーション中 ***
//	
#define N BOOST_PP_ITERATION()
	
  // 
	template< typename Derived, typename Tag, typename Parameters >
	class composite_impl< Derived, Tag, Parameters, N >
	{
	private:
		typedef composite_impl< Derived, Tag, Parameters, N > self;
	
	public:
		typedef self							implement_type;

		typedef Tag		 						action_tag;
		typedef typename Parameters::operands 	operands;
		typedef typename Parameters::extra		extra_args;
		
	// traits
	  	typedef composite_traits< action_tag, operands, extra_args > traits;
		
	// デフォルトコンストラクタ
		composite_impl()
			: traits_impl_(), operands_()
		{}
	
	// なんだかわからない値を受け渡すコンストラクタ
		template< typename Any >
		composite_impl( ctor_mode::transfer_arg tag, const Any& args )
			: traits_impl_( args ), operands_( tag, args )
		{}
	
	// オペランドファンクタを受け取るコンストラクタ
		template< typename Args >
		composite_impl( ctor_mode::copy_operand tag, const Args& args )
			: traits_impl_(), operands_( tag, args )
		{}
		
	// []演算子
		template< typename X >
		typename traits::template result<X>::type operator[]( const X& x )const
		{
			typedef typename traits::template result<X>::type ret_type;
			
			return traits_impl_.compose< ret_type >( operands_, x );
		}
		
	private:
	// アクション実装
		const traits traits_impl_;
	// オペランド
		const operand_container< operands, N > operands_;
	};
	
#undef N

#endif







