#pragma once

#ifndef ODEN_KONBU_CLAZY_SCOPE_LET
#define ODEN_KONBU_CLAZY_SCOPE_LET

//#######################################
//
//

// ---------- STL -----------
// ---------- boost ---------
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/remove.hpp>
#include <boost/fusion/include/push_back.hpp>

// ---------- oden ------------
// ---------- konbu -----------
#include "../detail/callable_expression.h"
#include "../detail/composite_expression.h"
#include <oden/konbu/clazy/primitive/defer.h>
#include <boost/mpl/print.hpp>

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
namespace detail
{  				
  // �����p�b�P�[�W���󂯎��A���[�J���ϐ����\�z����
  	template< typename Binding >
  	class local_frame
  	{
  	public:
  		typedef typename Binding::name_tag		name_tag;
  		typedef typename Binding::local_type	local_type;
  		typedef typename boost::add_reference<local_type>::type local_reference;
		
		typedef  fusion::pair< name_tag, local_reference > map_elem_type;
  		
  	// �R���X�g���N�^
		template< typename Dummy > /* ���̉���h�� */
		explicit local_frame( Dummy&, typename boost::enable_if< boost::is_same<Dummy,fusion::vector0<>> >::type* = 0 )
  			: val_()
  		{}
  	
  	#define AT_C(z,n,_) fusion::at_c<n>( args )
  	#define CTOR(z,n,_) \
  		template< BOOST_PP_ENUM_PARAMS(n, typename Arg) >																\
  		explicit local_frame( const fusion::vector##n <BOOST_PP_ENUM_PARAMS(n, Arg)> &args )	\
  			: val_( BOOST_PP_ENUM(n, AT_C, _) )																					\
  		{}
  		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_OPERAND_LIMIT), CTOR, _)
  	#undef  CTOR
  	#undef  AT_C
  	
  	// �}�b�v�Ɋi�[����v�f
  		map_elem_type make_elem(){ return val_; }
  		
  	private:
  		local_type val_;
  	};
	
}

//##################################################################
// �X�R�[�v (����)
//	
  // �e���v���[�g���������H
  	template< typename A, typename Operands >
  	struct custom_template_parameter< tag::scope<A>, Operands >
  	{
  	private:
  		typedef typename mpl::back< Operands >::type let_body_operand;
  	public:
  		typedef typename mpl::push_back<
  			typename mpl::pop_back< Operands >::type,
  			defer_<let_body_operand>					// body�̕]����x������
  		>::type
  		operands;
  		
  		typedef mpl::vector0<> extra;	
  	};
	
  // value_types - ���ׂ�as_any
	template< typename A, typename Operands, typename Context >
	struct value_types_raw< tag::scope<A>, Operands, Context >
		: public detail::value_types_all< as_any_tag, Operands >
	{};
	
  // �����p�}�N��
	// ���[�J���ϐ��̎���
	#define LOCAL_FRAME(z,n,_) \
				typedef detail::local_frame<Bind##n> lframe##n; \
				lframe##n l##n ( BOOST_PP_CAT(bind,n).right_ );
				
	// ���[�J���ϐ��̎��ʎq + �Q��
	#define LOCAL_MAP_ELEM(z,n,_) \
				typedef lframe##n::map_elem_type lmap_elem##n ; \
				lmap_elem##n lpair##n ( BOOST_PP_CAT(l,n).make_elem() );
				
    // apply�֐��𐶐�
	#define APPLY(z,n,SIG) \
		template< 																						\
			typename Ret, 																			\
			BOOST_PP_ENUM_PARAMS(n, typename Bind),								\
			typename Body,																			\
			typename Context 																		\
		>																										\
		Ret apply( 																						\
			BOOST_PP_ENUM_BINARY_PARAMS(n, Bind, bind),							\
			Body body,																					\
			const Context& context 															\
		)const 																							\
		{																										\
			BOOST_PP_REPEAT_FROM_TO(0, n, LOCAL_FRAME, _);						\
			BOOST_PP_REPEAT_FROM_TO(0, n, LOCAL_MAP_ELEM, _);					\
																													\
			typedef fusion::map< BOOST_PP_ENUM_PARAMS(n, lmap_elem) >	\
			locals_map;																						\
																													\
			return body.SIG(																			\
				context.local_joined( 																\
					locals_map( BOOST_PP_ENUM_PARAMS(n, lpair) )					\
				)																									\
			);																									\
		}

//##################################################################
// �X�R�[�v (�l��Ԃ�)
//	
  // ����
	template<>
	class action_impl< tag::scope<tag::return_body> >
	{
	public:
		action_impl(){}
		template< typename Args >
		explicit action_impl( Args& ){}		
			
	  // apply
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_OPERAND_LIMIT), APPLY, eval<Ret>)
	};
	
  // return_type - body�̖߂�l�^�Ɠ���
	template< typename Operands, typename Context >
	struct return_type_raw< tag::scope<tag::return_body>, Operands, Context >
	{
	private:
	  // �e������]�������^
	  	typedef typename detail::get_raw_result_all<Operands, Context>::type Args;
		
	  // �x������Body�������o��
		typedef typename mpl::back<Args>::type body_expr;
	  
	  // Bind�����疼�O+�ϐ��^�̃��X�g������
		typedef typename mpl::pop_back<Args>::type bindings;
			
	  // ���[�J���ϐ��̒ǉ����ꂽ�A�V�����R���e�L�X�g
		template< typename Binding >
		struct make_map_elem {
			typedef typename detail::local_frame<Binding>::map_elem_type type;
		};
		
		typedef typename detail::result_of_context_local_joined<
					Context,
					typename fusion_ret::as_map< 
						typename mpl::transform< bindings, make_map_elem<mpl::_1> >::type
					>::type
				>::type
			new_context;
		
	public:
		// body���Ɠ����Ԃ�l
		typedef typename detail::get_results<body_expr, new_context>::type
		type;
	};
	
  ///////// ����! ///////////
	#define CLAZY_EXPR 				CLAZY_VARIADIC_CALLABLE(let_, tag::scope<tag::return_body>)
	#include "../detail/pp_front.h"
	
//##################################################################
// �X�R�[�v (�l��Ԃ��Ȃ�)
//	
	template<>
	class action_impl< tag::scope<tag::no_return> >
	{
	public:
		action_impl(){}
		template< typename Args >
		explicit action_impl( Args& ){}		
		
	  // apply
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_OPERAND_LIMIT), APPLY, eval)
	};
	
  // return_type - none
	template<>
	struct return_type_static< tag::scope<tag::no_return> >
	{
		typedef return_nothing type;
	};
	
  ///////// ����! ///////////
	#define CLAZY_EXPR 				CLAZY_VARIADIC_CALLABLE(local_block_, tag::scope<tag::no_return>)
	#include "../detail/pp_front.h"	

  //
  // �}�N����n��
  //	
	#undef APPLY
	#undef LOCAL_FRAME
	#undef LOCAL_MAP_ELEM

//##################################################################
// �����֐� - let
//
	namespace tag
	{
		template< typename X > 
		struct scope_gen;
	}

  // 
	template< typename Operands >
	struct composite_traits< tag::scope_gen<tag::return_body>, Operands >
	{
		template< typename Body >
		struct result
		{
			typedef typename mpl::push_back< Operands, Body >::type new_operands;
			typedef typename as_let_< new_operands >::type type;
		};
		
		template< typename Ret, typename Os, typename Body >
		Ret compose( const Os& operands, const Body& body )const
		{
			return Ret( ctor_mode::copy_operand(), operands.push_back( body ) );
		}
	};
	
	#define CLAZY_EXPR					CLAZY_VARIADIC_COMPOSITE(let_gen_, tag::scope_gen<tag::return_body>)
	#define CLAZY_SYMBOL_VARIADIC_R		( 3, ( let, 1, KONBU_CLAZY_OPERAND_LIMIT ) )
	#include "../detail/pp_front.h"
	
//##################################################################
// �����֐� - local_block
//
  // 
	template< typename Operands >
	struct composite_traits< tag::scope_gen<tag::no_return>, Operands >
	{
		template< typename Body >
		struct result
		{
			typedef typename mpl::push_back< Operands, Body >::type new_operands;
			typedef typename as_local_block_< new_operands >::type type;
		};
		
		template< typename Ret, typename Os, typename Body >
		Ret compose( const Os& operands, const Body& body )const
		{
			return Ret( ctor_mode::copy_operand(), operands.push_back( body ) );
		}
	};
	
	#define CLAZY_EXPR					CLAZY_VARIADIC_COMPOSITE(local_block_gen_, tag::scope_gen<tag::no_return>)
	#define CLAZY_SYMBOL_VARIADIC_R		( 3, ( local_block, 1, KONBU_CLAZY_OPERAND_LIMIT ) )
	#include "../detail/pp_front.h"
	
}
}} /* end of namespace gobou, oden */


#endif




