#pragma once

#ifndef ODEN_KONBU_CLAZY_DT_EXPR_TRAITS
#define ODEN_KONBU_CLAZY_DT_EXPR_TRAITS

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

// boost.mpl
#include <boost/mpl/identity.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/at.hpp>	
#include <boost/mpl/empty.hpp>	
#include <boost/mpl/back.hpp>	
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/protect.hpp>

// boost.fusion
#	pragma message("	fusion funcs compiling")
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/remove_if.hpp>
#	pragma message("	fusion funcs compile end")

// ---------- oden ------------
// ---------- konbu -----------
#include <oden/konbu/clazy/detail/actions.h>

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
	namespace mpl = boost::mpl;
	namespace fusion = boost::fusion;
	namespace fusion_ret = fusion::result_of;
	
//#######################################################################
//  �^�O
//	
	struct as_rvalue_tag;
	struct as_lvalue_tag;
	struct as_raw_tag;
	typedef as_rvalue_tag as_any_tag;
	
	struct cant_be_lvalue;
	
	namespace detail
	{		
		struct unspecified;
	}

//_____________________________________________________________________________________
//
//                 * * * �߂�l�^ * * *
//_____________________________________________________________________________________
//
//##########################################################
// �߂�l�^���`
//	
  //
	template< typename T, typename R, typename L, typename Terminal >
	struct return_types
	{
		typedef T 			raw;
		typedef L			as_lvalue;
		typedef R			as_rvalue;	
		typedef Terminal	as_terminal;
	};

  // �E�Ӓl��Ԃ��ꍇ
	template< typename T >
	struct return_rvalue : return_types<
		T, 
		T,
		cant_be_lvalue,
		T
	>{};
	
  // �����Ԃ��Ȃ��ꍇ
  	typedef return_rvalue<void> return_nothing;
	
  // ���Ӓl��Ԃ��ꍇ	
	template< typename T >
	struct return_lvalue : return_types<
		T,
		typename boost::add_reference<const T>::type,
		typename boost::call_traits<T>::param_type,
		typename boost::call_traits<T>::param_type
	>{};
	
//##########################################################
// �A��l���o���^�֐�
//
	template< typename Parent, int I, typename Context >
	struct result_of_operand_at
	{
		typedef typename mpl::at_c<typename Parent::operands, I>::type				operand;
		typedef typename operand::template get_traits<Context>::type::ret			candidates;
		typedef typename Parent::template get_traits<Context>::type::value_types	operands_value_types;
		
		template< typename Side >
		struct select
		{};
		template<>
		struct select<as_lvalue_tag> {
			typedef typename candidates::as_lvalue type;
		};
		template<>
		struct select<as_rvalue_tag> {
			typedef typename candidates::as_rvalue type;
		};
		
		typedef typename select< 
					typename mpl::at_c<operands_value_types, I>::type
				>::type
			type;
	};
	
//##########################################################
// ���[�U�[��`�߂�l���_���^�֐�
//	 
	template< typename Tag, typename Extra = mpl::vector0<> >
	struct return_type_static
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Operands, typename Context, typename Extra = mpl::vector0<> >
	struct return_type_raw
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Extra = mpl::vector0<> >
	struct return_type
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Context, typename Extra = mpl::vector0<> >
	struct return_type_contextual
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Extra = mpl::vector0<> >
	struct plain_return_type
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Context, typename Extra = mpl::vector0<> >
	struct plain_return_type_contextual
		: public mpl::identity<detail::unspecified>
	{};

//#################################################################
// ���[�U�[��`�E�E�Ӓl���Ӓl�w�胁�^�֐�
//	 
	template< typename Tag, typename Extra = mpl::vector0<> >
	struct value_types_static
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Operands, typename Context, typename Extra = mpl::vector0<> >
	struct value_types_raw
		: public mpl::identity<detail::unspecified>
	{};

	template< typename Tag, typename Args, typename Extra = mpl::vector0<> >
	struct value_types
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Context, typename Extra = mpl::vector0<> >
	struct value_types_contextual
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Extra = mpl::vector0<> >
	struct plain_value_types
		: public mpl::identity<detail::unspecified>
	{};
	
	template< typename Tag, typename Args, typename Context, typename Extra = mpl::vector0<> >
	struct plain_value_types_contextual
		: public mpl::identity<detail::unspecified>
	{};
	
//############################################################
// ������J�X�^�}�C�Y����N���X�e���v���[�g
//
	template< typename Tag, typename Extra = mpl::vector0<> >
	struct action_impl
	{};
	
//######################################################
//  ���낢��
//
namespace detail
{
  // �I�y�����h�̎��ۂ̖߂�l�^���擾
	template< typename F, typename Context >
	struct get_results
	{
		typedef typename F::template get_traits<Context>::type::ret type;
	};
	
  // �I�y�����h�̎��ۂ̖߂�l�^���擾
	template< typename F, typename Context >
	struct get_raw_result 
	{
		typedef typename get_results<F,Context>::type::raw type;
	};
	
  // �I�y�����h�̓]���^�Ƃ��Ă̖߂�l�^���擾
	template< typename F, typename Context >
	struct get_result_as_rvalue
	{
		typedef typename get_results<F,Context>::type::as_rvalue type;
	};
	
  // �I�y�����h�̖��[�^�Ƃ��Ă̖߂�l�^���擾
	template< typename F, typename Context >
	struct get_result_as_terminal
	{
		typedef  typename get_results<F,Context>::type::as_terminal type;
	};
	
  // �S�ẴI�y�����h�̎��ۂ̖߂�l�^���擾
	template< typename Operands, typename Context >
	struct get_raw_result_all
	{
		typedef typename mpl::transform<
								Operands,
								get_raw_result< mpl::_1, mpl::protect<Context> >,
								mpl::back_inserter< mpl::vector0<> >
							>::type
						type;
	};
	
  // lvalue��Ԃ��邩�H
  	template< typename F, typename Context >
  	struct is_able_to_return_lvalue
  	{
  		typedef boost::is_same<
  					cant_be_lvalue,
  					typename F::template traits<Context>::result_as_rvalue
  				>
  			type;
  			
  		enum { value = type::value };
  	};

  // �S�ē���value_type
	template< typename Tag, typename OperandsOrArgs >	
	struct value_types_all
	{
		typedef typename mpl::transform< 
					OperandsOrArgs,
					mpl::always< as_any_tag >
				>::type
			type;		
	};
	
  // �����^����Q�ƁEcv����������
	template< typename T >
	struct remove_reference_and_cv
			: public boost::remove_cv< typename boost::remove_reference<T>::type >
	{};
 
  // ���s������
	struct failure_no_match_return_type;
	struct failure_no_match_value_types;
	
//_________________________________________________________________________________________________________
//
//                                                     * * * * * �R���e�L�X�g * * * * *
//_________________________________________________________________________________________________________
//
//#######################################################################
// �Ăяo�����R���e�L�X�g
//
	template< typename CallArgs, typename LocalsMap >
	class context;
	
	template< typename OldElem, typename NewLocals >
	struct is_overrided_local
		: public fusion_ret::has_key<
					NewLocals,
					typename fusion_ret::first<OldElem>::type
				>::type
	{};
	
	template< typename Context, typename NewMap >
	struct result_of_context_local_joined
	{
		typedef typename Context::call_args 	call_args;
		typedef typename Context::locals_map 	old_locals_map;
			
		typedef	typename fusion_ret::join< 
					const typename fusion_ret::remove_if< 
						const old_locals_map,
						detail::is_overrided_local<mpl::_1, mpl::protect<NewMap> >
					>::type,
					const NewMap
				>::type
			new_locals_map;
					
		typedef context< call_args, new_locals_map > type;
	};

	template< typename CallArgs, typename LocalsMap >
	class context
	{
	private:
		typedef context<CallArgs,LocalsMap> self;
	public:
		typedef CallArgs  call_args;	// fusion::vector
		typedef LocalsMap locals_map;	// fusion::map 
		
	// �R���X�g���N�^
		context()
			: call_(), local_()
		{}
		template< typename Seq1, typename Seq2 >
		context( const Seq1& calla, const Seq2& locala )
			: call_(calla), local_(locala)
		{}
		
	// ���[�J���ϐ���ǉ�	
		template< typename Map >
		typename result_of_context_local_joined<self, Map>::type 
		local_joined( const Map& new_local )const
		{
			typedef result_of_context_local_joined<self, Map>::type 
			new_context;
		
			return 
			new_context( call_, 
				fusion::join(
					fusion::remove_if< is_overrided_local<mpl::_1, mpl::protect<Map> > >( local_ ),
					new_local
				)
			);
		}
		
	// �Ăяo���������擾
		template< int I >
		typename fusion_ret::at_c<const call_args, I>::type 
		arg_at()const 
		{
			return fusion::at_c<I>( call_ );
		}
		
	// ���[�J���ϐ����擾
		template< typename Key >
		typename fusion_ret::at_key<const locals_map, Key>::type 
		local_at()const
		{
			return fusion::at_key<Key>( local_ );
		}
		
	private:
		call_args 	call_;
		locals_map	local_;	
	};
	
  // �R���e�L�X�g
	template< typename CallArgs >
	struct initial_context
	{
		typedef context< CallArgs, fusion::map<> > type;
	};

//_________________________________________________________________________________________________________
//
//                                                     * * * * * ������`�N���X * * * * *
//_________________________________________________________________________________________________________
//
//######################################################################
//  �I�[�o�[���[�h�\�ȕ\�����H�𒲂ׂāA�������I�Ȏ���������
//
  	template< typename Tag, typename Operands, typename Context, typename Extra,
			  bool bOverloadable = is_overloadable<Tag>::value
			  >
	struct expression_traits_impl
	{};
	
//######################################################################
//  �I�[�o�[���[�h�ł���̂ŁA�e�I�y�����h�̎��ۂɕԂ��^���K�v
//
	template< typename Tag, typename Operands, typename Context, typename Extra >
	struct expression_traits_impl
			< Tag, Operands, Context, Extra, true >
	{
	#pragma message("	expression_traits_impl - overloadable begin")	
		// �e�I�y�����h���Ԃ��l�^�����X�g�ɂ���
		typedef typename get_raw_result_all<Operands, Context>::type
					operands_ret;
		
		// �����cv�E�Q�Ƃ���菜�������^���X�g
		typedef typename mpl::transform<
							operands_ret,
							remove_reference_and_cv< mpl::_1 >,
							mpl::back_inserter< mpl::vector0<> >
						>::type
					plain_operands_ret;		
					
	   // �߂�l�^�̌��
		typedef mpl::vector4<
						typename return_type< Tag, operands_ret, Extra >::type,
						typename return_type_contextual< Tag, operands_ret, Context, Extra >::type,
						typename plain_return_type< Tag, plain_operands_ret, Extra >::type,
						typename plain_return_type_contextual< Tag, plain_operands_ret, Context, Extra >::type
					>
			return_type_candidates;
		
	  // �E�Ӓl�E���Ӓl�^�C�v�̌��
		typedef mpl::vector5<
						typename value_types_static< Tag, Extra >::type,		// 
						typename value_types< Tag, operands_ret, Extra >::type,
						typename value_types_contextual< Tag, operands_ret, Context, Extra >::type,
						typename plain_value_types< Tag, plain_operands_ret, Extra >::type,
						typename plain_value_types_contextual< Tag, plain_operands_ret, Context, Extra >::type
					>
			value_types_candidates;
		
	#pragma message("	expression_traits_impl - overloadable end")	
	};
	
//######################################################################
// �I�[�o�[���[�h�ł��Ȃ��Ȃ�A�킴�킴�S�Ă̈����̎��ۂ̌^���o���Ȃ��Ă�����
//
  	template< typename Tag, typename Operands, typename Context, typename Extra >
	struct expression_traits_impl
			< Tag, Operands, Context, Extra, false >
	{	
	#pragma message("	expression_traits_impl - not overloadable begin.")
	   // �߂�l�^�̌��
		typedef mpl::vector2<
					typename return_type_static< Tag, Extra >::type,
					typename return_type_raw< Tag, Operands, Context, Extra >::type
				>
			return_type_candidates;
		
	  // �E�Ӓl�E���Ӓl�^�C�v�̌��
		typedef mpl::vector2<
					typename value_types_static< Tag, Extra >::type,
					typename value_types_raw< Tag, Operands, Context, Extra >::type
				>
			value_types_candidates;
			
	#pragma message("	expression_traits_impl - not overloadable end.")
	};
  
//######################################################################
//    traits�����N���X - �߂�l�^�A�E�Ӓl���Ӓl�^�C�v��`����
//
  	template< typename Tag, typename Operands, typename Context, typename Extra >
	struct expression_traits
	{
		typedef Tag 		tag;
		typedef Operands 	operands;
		typedef Context		context;
		typedef Extra		extra_args;
		
		typedef expression_traits_impl<tag, operands, context, extra_args> impl;
		
	  // unspecified�Ȃ����ӂ邢���Ƃ�
		typedef typename mpl::remove_if<
					typename impl::return_type_candidates,
					boost::is_same<mpl::_1, unspecified>
				>::type
			return_type_candidates_2;
			
		typedef typename mpl::remove_if<
					typename impl::value_types_candidates,
					boost::is_same<mpl::_1, unspecified>
				>::type
			value_types_candidates_2;
		
	  // ����		
		typedef typename mpl::eval_if< mpl::empty<return_type_candidates_2>,
							mpl::identity<detail::failure_no_match_return_type>,
							mpl::back<return_type_candidates_2>
						>::type
					ret;
					
		typedef typename ret::raw   	  result_raw_type;
		typedef typename ret::as_rvalue   result_as_rvalue;
		typedef typename ret::as_lvalue   result_as_lvalue;
		typedef typename ret::as_terminal result_as_terminal;

		typedef typename mpl::eval_if< mpl::empty<value_types_candidates_2>,
							mpl::identity<detail::failure_no_match_value_types>,
							mpl::back<value_types_candidates_2>
						>::type	
					value_types;
	};  	
}
	
} /* end of namespace clazy */
}} /* end of namespace konbu, oden */

#endif




