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
// �R���X�g���N�^������P�ɓ]�����邩�A�W�J���ĕ��z���邩
// 
namespace ctor_mode
{
  // �^�O
 	struct transfer_arg {};
 	struct copy_operand {};
	
  // �^�O��������I�[�o�[���[�h��₩��O��
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
	
  // �����^����v���邩�݂�
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
// �e���v���[�g����
//
  // ���^
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
	
	// �R���X�g���N�^
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
#endif /* �C���N���[�h�K�[�h */


#else 
//#######################################################################
// *** �C�e���[�V������ ***
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
	// �I�y�����h
	#define DECL_OPERAND(z,n,_) typedef typename mpl::at_c<Operands, n>::type A##n; A##n a##n;
		BOOST_PP_REPEAT_FROM_TO(0, N, DECL_OPERAND, _)
	#undef  DECL_OPERAND

	public:	
	// �f�t�H���g�R���X�g���N�^
	#define OPERAND_INIT(z,n,_) a##n()
		operand_container()
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
	
	// �Ȃ񂾂��킩��Ȃ��l���󂯓n���R���X�g���N�^
	#define OPERAND_INIT(z,n,_) a##n( ctor_mode::transfer_arg(), arg )
		template< typename Any >
		operand_container( ctor_mode::transfer_arg, const Any& arg )
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
	
	// �I�y�����h�t�@���N�^���󂯎��R���X�g���N�^	- fusion Sequence
	#define OPERAND_INIT(z,n,_) a##n( fusion::at_c<n>( args ) )
		template< typename Args >
		operand_container( ctor_mode::copy_operand, const Args& args )
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
	
	// �I�y�����h�t�@���N�^���󂯎��R���X�g���N�^	- �����R���e�i
	#define OPERAND_INIT(z,n,_) a##n( args.get<n>() )
		template< typename Oprs >
		operand_container( ctor_mode::copy_operand, const operand_container<Oprs,N>& args )
			: BOOST_PP_ENUM(N, OPERAND_INIT, _)
		{}
	#undef  OPERAND_INIT
		
	// �I�y�����h���擾����
	#define GET_IMPL(z,n,_) A##n const& get_impl( mpl::int_<n> )const{ return a##n; }
		BOOST_PP_REPEAT_FROM_TO(0, N, GET_IMPL, _)
	#undef  GET_IMPL
		template< int I >
		typename mpl::at_c<Operands, I>::type const& get()const 
		{
			return get_impl( mpl::int_<I>() );
		}
		
	// ���݂��ĕԂ�
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







