
// cdecl版はすでに導入済みとする

// 
// 自己インクルードモード
//
#ifdef BOOST_PP_IS_ITERATING

namespace oden { namespace konbu
{
	
#define N 	BOOST_PP_ITERATION()
#define CC	KONBU_CC_QUAL

  // ラッパクラス
	template< typename Ret, BOOST_PP_ENUM_PARAMS(N, typename Arg) >
	class pointer_function<
		Ret (CC*)( BOOST_PP_ENUM_PARAMS(N, Arg)... )
	>
	{
	public:
		typedef Ret (CC *function_pointer)( BOOST_PP_ENUM_PARAMS(N, Arg)... ); 
		typedef Ret result_type;
		
	// コンストラクタ
		pointer_function( function_pointer pF )
			: pf_(pF)
		{}
		
	// 呼び出し演算子をN~MAXまで生成
	#define VCALL(z,n,_) \
		template< BOOST_PP_ENUM_PARAMS(n, typename A) > 						\
		result_type operator()( BOOST_PP_ENUM_BINARY_PARAMS(n, const A, &arg) )	\
		{																		\
			return (*pf_)( BOOST_PP_ENUM_PARAMS(n, arg) ); 						\
		}
		BOOST_PP_REPEAT_FROM_TO(N, BOOST_PP_ADD(ODEN_KONBU_MAX_ARITY,1), VCALL, _)
	#undef  VCALL
	
	private: 
		function_pointer pf_;
	};
	
#undef N 
#undef CC

}}	
#endif
