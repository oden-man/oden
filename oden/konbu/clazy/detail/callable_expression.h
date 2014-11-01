#if !BOOST_PP_IS_ITERATING

//  インクルードガード
#ifndef ODEN_KONBU_CLAZY_DT_CALLABLE_EXPR
#define ODEN_KONBU_CLAZY_DT_CALLABLE_EXPR

//
#include <oden/konbu/clazy/detail/pp_front_fwd.h>
#include <oden/konbu/clazy/detail/expression_traits.h>

// boost.mpl
#include <boost/mpl/at.hpp>

// boost.pp
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

namespace oden { namespace konbu { namespace clazy 
{
namespace detail
{	
//#######################################################################
// テンプレート部分
//
  // 原型	
	template< typename Derived, typename Tag, typename Parameters, int Arity = mpl::size<typename Parameters::operands>::value >
	class callable_impl
	{};
	
  // 0~MAXまで生成
  	#define BOOST_PP_FILENAME_1 <oden/konbu/clazy/detail/callable_expression.h>
  	#define BOOST_PP_ITERATION_LIMITS ( 0, KONBU_CLAZY_OPERAND_LIMIT )
	#include BOOST_PP_ITERATE()
}

}
}}

//#######################################################################
// ユーティリティ マクロ定義
//
#define CLAZY_CALLABLE( name, tag, arity ) 			( 4, ( name, tag, arity, callable_impl ) )
#define CLAZY_VARIADIC_CALLABLE( name, tag )		CLAZY_CALLABLE( name, tag, KONBU_CLAZY_OPERAND_LIMIT )
#define CLAZY_NULLARY_CALLABLE( name, tag )			CLAZY_CALLABLE( name, tag, 0 )
#define CLAZY_UNARY_CALLABLE( name, tag )			CLAZY_CALLABLE( name, tag, 1 )
#define CLAZY_BINARY_CALLABLE( name, tag )			CLAZY_CALLABLE( name, tag, 2 )

//################################################################
// value_wrapper, wrap_value の定義のため、valueをインクルード
//
#ifndef CLAZY_VALUE_BEING_INVOKED
	#include <oden/konbu/clazy/primitive/value.h>
#endif

#endif /* インクルードガード */

#else 
//#######################################################################
// *** イテレーション中 ***
//	
#define N BOOST_PP_ITERATION()
	
  // 
	template< typename Derived, typename Tag, typename Parameters >
	class callable_impl< Derived, Tag, Parameters, N >
		: public functor< Derived >
	{
	private:
		typedef callable_impl< Derived, Tag, Parameters, N > self;
	public:
		typedef self							implement_type;
	
		typedef Tag		 						action_tag;
		typedef typename Parameters::operands 	operands;
		typedef typename Parameters::extra		extra_args;
		
	// traits
		template< typename Context >
		struct get_traits
		{
			typedef detail::expression_traits< action_tag, operands, Context, extra_args > type;
		};
	
	// result_ofのサポート	
		template<typename Sig>
		struct result
		{};
		
	#define RESULT(z, n, _) \
		template< 																\
			typename F BOOST_PP_COMMA_IF(n)	BOOST_PP_ENUM_PARAMS(n, typename A) \
		>																		\
		struct result<F( BOOST_PP_ENUM_PARAMS(n,A) )> 							\
		{																		\
			typedef typename													\
				detail::get_result_as_terminal<									\
					self,														\
					typename detail::initial_context<							\
						fusion::vector<BOOST_PP_ENUM_PARAMS(n, A)>				\
					>::type														\
				>::type															\
			type;																\
		};	
		BOOST_PP_REPEAT_FROM_TO(0, BOOST_PP_INC(KONBU_CLAZY_ARG_LIMIT), RESULT, )
	#undef RESULT
		
	// デフォルトコンストラクタ
		callable_impl()
			: action_(), operands_()
		{}
	
	// なんだかわからない値を受け渡すコンストラクタ
		template< typename Any >
		callable_impl( ctor_mode::transfer_arg tag, const Any& args )
			: action_( args ), operands_( tag, args )
		{}
	
	// オペランドファンクタを受け取るコンストラクタ
		template< typename Args >
		callable_impl( ctor_mode::copy_operand tag, const Args& args )
			: action_(), operands_( tag, args )
		{}
		
	// 一つのオペランドを評価する
		template< int I, typename Context >
  		typename result_of_operand_at<self, I, Context>::type 
  		eval_operand_at( const Context& context )const 
		{
  			typedef typename result_of_operand_at<self, I, Context>::type ret_type;
			
  			return operands_.get<I>().template eval< ret_type >( context );
  		}
		
	// 全てのオペランドを評価し、実装に渡して実行
		template< typename Ret, typename Context >
		Ret eval( const Context& context )const
		{
		#define RESULT_OF_OPERAND(z,n,_)	typename result_of_operand_at<self, n, Context>::type
		#define CALL_OPERAND(z,n,_) 		this->eval_operand_at<n>( context )
		
			return action_.apply< Ret BOOST_PP_ENUM_TRAILING(N, RESULT_OF_OPERAND, _) >(
				BOOST_PP_ENUM(N, CALL_OPERAND, _) BOOST_PP_COMMA_IF(N) context
			);

		#undef RESULT_OF_OPERAND
		#undef CALL_OPERAND
		}
		
	// 帰り値を切り捨てるeval
		template< typename Context >
		void eval( const Context& context )const
		{
			typedef typename detail::get_result_as_rvalue<self, Context>::type ret_type;
			
			this->eval< ret_type >( context );
		}
		
	// オペランドを取得する
		template< int I >
		const typename mpl::at_c<operands, I>::type &get_operand_at()const 
		{
			return operands_.get<I>();
		}
		
	// ()演算子
	#define CALL(z, n, _) \
		template< BOOST_PP_ENUM_PARAMS(n, typename A) >									\
		typename detail::get_result_as_terminal<										\
			self,																		\
			typename detail::initial_context<											\
				fusion::vector<															\
					BOOST_PP_ENUM_BINARY_PARAMS(n, A,& BOOST_PP_INTERCEPT)				\
				>																		\
			>::type																		\
		>::type																			\
		operator()( BOOST_PP_ENUM_BINARY_PARAMS(n, A, &a) )const						\
		{																				\
			typedef fusion::vector<														\
						BOOST_PP_ENUM_BINARY_PARAMS(n, A,& BOOST_PP_INTERCEPT)			\
					> 																	\
				param_vector_type;														\
																						\
			typedef detail::initial_context< param_vector_type >::type const			\
				context_type;															\
																						\
			typedef detail::get_result_as_terminal<	self, context_type >::type			\
				ret_type;																\
																						\
			const context_type& context_ = context_type(								\
				  		param_vector_type( BOOST_PP_ENUM_PARAMS(n, a) ),				\
				  		fusion::vector0<>()												\
					);																	\
																						\
			return this->template eval< ret_type >( context_ );							\
		}
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_ARG_LIMIT), CALL, _)
	#undef CALL
		
	// 演算子をさらに引き込む
		using functor< Derived >::operator=;
		using functor< Derived >::operator[];
	
	private:
	// アクション実装
		const action_impl< action_tag, extra_args > action_;
	// オペランド
		const operand_container< operands, N >  	operands_;
	};
	
#undef N

#endif







