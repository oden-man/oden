
//
// 一回しか読み込まれない部分 
//
#ifndef ODEN_KONBU_DT_MFIMPLPP
#define ODEN_KONBU_DT_MFIMPLPP

//################################################
//   インクルード
//

// ------------ boost.pp -----------
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
//
//################################################


namespace oden { namespace konbu
{
//###################################################################
// ひな形
//
	template< typename MemberFunctionPointerType >
	class member_function
	{};
	
//###################################################################################
// member_functionを生成する
//	
  // ポインタ
	template<typename MFPtr>
	member_function<MFPtr>
	mem_fun( MFPtr pMemFunPtr, typename member_function<MFPtr>::inst_pointer pInst )			
	{
		return konbu::member_function<MFPtr>( pMemFunPtr, pInst );				
	}
	
  // 参照
	template<typename MFPtr>									
	member_function<MFPtr>
	mem_fun( MFPtr pMemFunPtr, typename member_function<MFPtr>::inst_reference rInst )			
	{
		return konbu::member_function<MFPtr>( pMemFunPtr, rInst );				
	}
	
}} /* end of namespace oden.konbu */

#endif // include guard

//
// *** ファイルイテレート中 ***
//	
#ifdef BOOST_PP_IS_ITERATING
namespace oden { namespace konbu
{

#define N 	BOOST_PP_ITERATION()
#define CC	KONBU_CC_QUAL
#define CV  KONBU_MF_CV_QUAL

  // ラッパークラス
	template< typename Ret, typename Class BOOST_PP_ENUM_TRAILING_PARAMS(N, typename Arg) >
	class member_function< 
		Ret (CC Class::*)( BOOST_PP_ENUM_PARAMS(N, Arg) ) CV
	>
	{
	public:
		typedef Ret (CC Class::*)( BOOST_PP_ENUM_PARAMS(N, Arg) ) CV; 
		
		typedef Ret 		result_type;
		typedef Class CV*	inst_pointer;
		typedef Class CV&	inst_reference;
		
	// コンストラクタ
		member_function( member_function_pointer pMf, inst_pointer pInst )
			: pmf_(pMf), pinst_(pInst)
		{}
		member_function( member_function_pointer pMf, inst_reference rInst )
			: pmf_(pMf), pinst_(&rInst)
		{}
		
	// 呼び出し演算子
		result_type operator()( BOOST_PP_ENUM_BINARY_PARAMS(N, typename boost::call_traits<Arg, >::param_type arg) ) CV
		{
			return (pinst_->*pmf_)( BOOST_PP_ENUM_PARAMS(N, arg) ); 
		}
	private:
		member_function_pointer	pmf_; 
		inst_pointer			pinst_;
	};
	
}}

#undef N
#undef CC
#undef CV

#endif

