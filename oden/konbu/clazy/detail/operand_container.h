#if !BOOST_PP_IS_ITERATING

// 
#ifndef ODEN_KONBU_CLAZY_DT_OPERAND_CONT
#define ODEN_KONBU_CLAZY_DT_OPERAND_CONT

//
#include <oden/konbu/clazy/limits.h>

//
#include <boost/type_traits/is_convertible.hpp>

// boost.mpl
#include <boost/mpl/identity.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>

// boost.pp 
#include <boost/preprocessor/iteration/iterate.hpp>

namespace oden { namespace konbu { namespace clazy 
{
//#######################################################################
// コンストラクタ引数を単に転送するか、展開して分配するか
// 
namespace ctor_mode
{
  // タグ
 	struct transfer_arg {};
 	struct copy_operand {};
	
  // タグだったらオーバーロード候補から外す
	template< typename T > 
	struct disable_if_tag {
		typedef void type;
	};
	template<> struct disable_if_tag< transfer_arg > {};
	template<> struct disable_if_tag< copy_operand > {};
}
	
namespace detail
{				
	namespace mpl = boost::mpl;
	
  // 引数型が一致するかみる
	template< typename Args, typename CtorArgs >
	struct select_ctor_mode
	{
	private:
		enum { ArgNum = mpl::size<Args>::value };
	
		template< int I >
		struct is_convertible_to_operands
		{
			typedef typename mpl::at_c<CtorArgs,I>::type	src;
			typedef typename mpl::at_c<Args,I>::type		dest;

			typedef
			typename mpl::eval_if< 
						typename dest::template is_constructible_with<src>,
						is_convertible_to_operands<I+1>,
						mpl::identity< ctor_mode::transfer_arg >
					>::type
				type;
		};
		template<>
		struct is_convertible_to_operands< ArgNum >
		{
			typedef ctor_mode::copy_operand type;
		};
		
	public:
		typedef typename mpl::eval_if_c<
							( ArgNum == mpl::size<CtorArgs>::value ),
							is_convertible_to_operands<0>,
							mpl::identity< ctor_mode::transfer_arg >
						>::type
					type;
	};
	
//#######################################################################
// テンプレート部分
//
  // 原型
	template< typename Operands, int Arity >
	class operand_container
	{};
	
  // 0
	template< typename Operands >
	class operand_container< Operands, 0 >
	{
	private:
		typedef operand_container< Operands, 0 > self;
	public:
		enum { size = 0 };
		typedef Operands operands;
	
	// コンストラクタ
		operand_container(){}
		template< typename Tag, typename X > operand_container( Tag, const X& ){}
	};

//#########################################################################	
// 1~MAX
//
  	#define BOOST_PP_FILENAME_1 <oden/konbu/clazy/detail/operand_container.h>
  	#define BOOST_PP_ITERATION_LIMITS ( 1, KONBU_CLAZY_OPERAND_LIMIT )
	#include BOOST_PP_ITERATE()
}

}}}
#endif /* インクルードガード */


#else 
//#######################################################################
// *** イテレーション中 ***
//	
#define N BOOST_PP_ITERATION()

	template< typename Operands >
	class operand_container< Operands, N >
	{
	private:
		typedef operand_container< Operands, N > self;

	public:
		enum { size = N };
		typedef Operands operands;

	private:
	// オペランド
	#define DECL_OPERAND(z,n,_) typedef typename mpl::at_c<Operands, n>::type A##n; A##n a##n;
		BOOST_PP_REPEAT_FROM_TO(0, N, DECL_OPERAND, _)
	#undef  DECL_OPERAND

	public:	
	// デフォルトコンストラクタ
	#define OPERAND_INIT(z,n,_) a##n()
		operand_container()
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
	
	// なんだかわからない値を受け渡すコンストラクタ
	#define OPERAND_INIT(z,n,_) a##n( ctor_mode::transfer_arg(), arg )
		template< typename Any >
		operand_container( ctor_mode::transfer_arg, const Any& arg )
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
	
	// オペランドファンクタを受け取るコンストラクタ	- fusion Sequence
	#define OPERAND_INIT(z,n,_) a##n( fusion::at_c<n>( args ) )
		template< typename Args >
		operand_container( ctor_mode::copy_operand, const Args& args )
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
	
	// オペランドファンクタを受け取るコンストラクタ	- 同じコンテナ
	#define OPERAND_INIT(z,n,_) a##n( args.get<n>() )
		template< typename Oprs >
		operand_container( ctor_mode::copy_operand, const operand_container<Oprs,N>& args )
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
		
	// オペランドを取得する
	#define GET_IMPL(z,n,_) A##n const& get_impl( mpl::int_<n> )const{ return a##n; }
		BOOST_PP_REPEAT_FROM_TO(0, N, GET_IMPL, _)
	#undef  GET_IMPL
		template< int I >
		typename mpl::at_c<Operands, I>::type const& get()const 
		{
			return get_impl( mpl::int_<I>() );
		}
		
	// 増設して返す
		template< typename X >
		operand_container<
			typename mpl::push_back< operands, X >::type, N+1
		>
		push_back( const X& x )const
		{
			return
			operand_container<
				typename mpl::push_back< operands, X >::type, N+1
			>(
				ctor_mode::copy_operand(), 
				fusion::vector<BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& BOOST_PP_INTERCEPT), X const&>( BOOST_PP_ENUM_PARAMS(N, a), x ) 
			);
		}
	};
	
#undef N

#endif







