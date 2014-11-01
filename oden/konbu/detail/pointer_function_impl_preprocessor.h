
//
// ��񂵂��ǂݍ��܂�Ȃ����� 
//
#ifndef ODEN_KONBU_DT_PFIMPLPP
#define ODEN_KONBU_DT_PFIMPLPP

//################################################
//   �C���N���[�h
//

// ------------ boost.pp -----------
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
// ------------- boost --------------
// ------------ oden -------------

//
//################################################


namespace oden { namespace konbu
{
//###########################################################
// �ЂȌ^
//
	template< typename FunctionPointerType >
	class pointer_function
	{};
		
//###########################################################
// pointer_function�𐶐�����
//
	template<typename FPtr>								
	pointer_function<FPtr> ptr_fun( FPtr pFunPtr )
	{
		return konbu::pointer_function<FPtr>( pFunPtr );
	}
	
}} /* end of namespace oden.konbu */

#endif // include guard

//
// *** �t�@�C���C�e���[�g���[�h ***
//
#ifdef BOOST_PP_IS_ITERATING

namespace oden { namespace konbu
{
#define N 	BOOST_PP_ITERATION()
#define CC 	KONBU_CC_QUAL


// ���b�p�N���X
	template< typename Ret BOOST_PP_ENUM_TRAILING_PARAMS(N, typename Arg) >
	class pointer_function< Ret (CC *)( BOOST_PP_ENUM_PARAMS(N, Arg) ) >
	{
	public:
		typedef Ret (CC *function_pointer)( BOOST_PP_ENUM_PARAMS(N, Arg) ); 
		
		typedef Ret result_type;
		
	// �R���X�g���N�^
		pointer_function( function_pointer pF )
			: pf_(pF)
		{}
		
	// �Ăяo�����Z�q
		result_type operator()( BOOST_PP_ENUM_BINARY_PARAMS(N, typename boost::call_traits<Arg,>::param_type arg) )
		{
			return (*pf_)( BOOST_PP_ENUM_PARAMS(N, arg) ); 
		}
	private: 
		function_pointer pf_;
	};	
	
}}

#undef N
#undef CC

#endif
