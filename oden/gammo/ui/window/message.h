#pragma once

#ifndef ODEN_GAMMO_UI_WND_MSG
#define ODEN_GAMMO_UI_WND_MSG

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// ------ STL ---------
// ----- boost --------
#include <boost/static_assert.hpp>
// ------- oden -------
// ------- gammo ------
#include <oden/gammo/common/mini_mpl.h>
#include <oden/gammo/ui/window/event.h>
#include <oden/gammo/ui/widget/indirect_widget.h>

//
//
//###############################################################

namespace oden { namespace gammo
{
	
//##################################################################
// ���b�Z�[�W���M�̂��߂̃I�u�W�F�N�g
//
	class window_message
	{
	public:
	// �R���X�g���N�^
		window_message( UINT c, WPARAM wp = 0, LPARAM lp = 0 ):
			code_(c),wParam_(wp),lParam_(lp)
		{}
		
	// �����o�֐�
		UINT   code()const 		{ return code_; }
		WPARAM wparam()const	{ return wParam_; }
		LPARAM lparam()const	{ return lParam_; }		
		
	private:
		UINT   code_;
		WPARAM wParam_;
		LPARAM lParam_;
	};

//############################################################
//  �C�x���g�G���R�[�_�G�C���A�X��������
// 
	template< typename Src, typename Obj >
	struct event_message_alias_resolver
	{
	private:
		typedef typename indirect_widget<Obj>::type o;
	public:
		// �R���g���[���^���擾���A�G�C���A�X����
		typedef typename event_message_alias< Src, o >::type type;
	};


//############################################################
//  �C�x���g���쐬
//	
	template< typename Derived, typename Sig >
	class event_message_base
	{
	public:
		typedef Sig								sig;
		typedef event_message_base<Derived,Sig>	self;
		
	// �R���X�g���N�^
	protected:
		event_message_base(){}
	public:
	
	// �����o�֐�
	  // �m�肷��
		template< typename Object >
		window_message make( Object& )const
		{
			typedef event_message_alias_resolver<sig, Object>::type 
			resolved_event_def;
			
			const Derived* deriv = static_cast<const Derived*>(this);
			event::wlparam p = deriv->create_message<resolved_event_def>();
			
			return window_message( resolved_event_def::code, p.w, p.l );
		}
		
	  // ���b�Z�[�W�Ԃ�l�^���擾
		template< 
			typename Object, 
			typename Resolved = typename event_message_alias_resolver<sig, Object>::type,
			bool 	 NoRet    = !event_has_result<Resolved>::value >
		struct send_result
		{
			typedef typename event_result_def<Resolved>::type	def;
			typedef typename def::value_type					type;
		};
		template< typename Obj, typename Resolved >
		struct send_result<Obj, Resolved, true>
		{
			typedef void void_type;
		};
		
	  // send
		template< typename Object >
		LRESULT send_l( const Object& to )const
		{
			return indirect<const Object>::get(to).send( this->make(to) );			
		}
		template< typename Object >
		typename send_result<Object>::type send( const Object& to )const
		{
			return send_result<Object>::def::decode_message( this->send_l( to )  );			
		}
		template< typename Object >
		typename send_result<Object>::void_type send( const Object& to )const
		{
			this->send_l( to );			
		}
		
	  // post
		template< typename Object >
		bool post( const Object& to )const
		{
			return indirect<const Object>::get(to).post( this->make(to) );		
		}

	// ���Z�q�I�[�o�[���[�h
	  // send <<
		template< typename Object >
		friend typename send_result<Object>::type 
			operator<< ( Object& obj, const self& emsg )
		{
			return emsg.send( obj );
		}
		template< typename Object >
		friend typename send_result<Object>::void_type 
			operator<< ( Object& obj, const self& emsg )
		{
			emsg.send_l( obj );
		}
	};

//############################################################
//   event_message - �G���R�[�h���s��
//
  // �m�肵�����b�Z�[�W
	template< typename Sig, int Size = event_message_arity<Sig>::value > // Size���ȗ������ꍇ�Aevent_arg�����̉������
	class event_message;

	// 0
	template< typename Sig >
	class event_message< Sig, 0 >
		: public event_message_base< event_message< Sig, 0 >, Sig >
	{
		BOOST_STATIC_ASSERT( event_message_arity<Sig>::value == 0 );
	public:
		
		template< typename >
		event::wlparam create_message()const { return event::wlparam(0,0); }
	};
	
	// 1
	template< typename Sig >
	class event_message< Sig, 1 >
		: public event_message_base< event_message< Sig, 1 >, Sig >
	{
		BOOST_STATIC_ASSERT( event_message_arity<Sig>::value == 1 );
	public:
		
		typedef event_message_arg<Sig>							args;
		typedef typename event_message_input_type<Sig,0>::type	type1;
		
		event_message( type1 a1 )
			: p_( args::encode_whole( a1 ) )
		{}

		template< typename >
		event::wlparam create_message()const { return p_; }
	private:
		event::wlparam p_;
	};
	
	// 2
	template< typename Sig >
	class event_message< Sig, 2 >
		: public event_message_base< event_message< Sig, 2 >, Sig >
	{
		BOOST_STATIC_ASSERT( event_message_arity<Sig>::value == 2 );
	public:
		
		typedef event_message_arg<Sig>							args;
		typedef typename event_message_input_type<Sig,0>::type	type1;
		typedef typename event_message_input_type<Sig,1>::type	type2;
		
		event_message( type1 a1, type2 a2 )
			: p_( args::encode_whole( a1, a2 ) )
		{}

		template< typename >
		event::wlparam create_message()const { return p_; }
	private:
		event::wlparam p_;
	};
	
//############################################################
//   some_event_message 
//
  // �m�肵�Ă��Ȃ����b�Z�[�W
	template< typename Sig, typename Args >
	class some_event_message;

	// 0
	template< typename Sig >
	class some_event_message< Sig, typelist<> >
		: public event_message_base< some_event_message< Sig, typelist<> >, Sig >
	{
	public:

		template< typename >
		event::wlparam create_message()const { return event::wlparam(0,0); }
	};
	
	// 1
	template< typename Sig, typename A1 >
	class some_event_message< Sig, typelist<A1> >
		: public event_message_base< some_event_message< Sig, typelist<A1> >, Sig >
	{
	public:		
		some_event_message( A1 a1 )
			: a1_(a1)
		{}
		
		template< typename EvDef >
		event::wlparam create_message()const 
		{ 
			typedef typename event_message_arg<EvDef>::args args;
			return args::encode_whole( a1_ ); 
		}
		
	private:
		A1 a1_;
	};
	
	// 2
	template< typename Sig, typename A1, typename A2 >
	class some_event_message< Sig, typelist<A1,A2> >
		: public event_message_base< some_event_message< Sig, typelist<A1,A2> >, Sig >
	{
	public:		
		some_event_message( A1 a1, A2 a2 )
			: a1_(a1), a2_(a2)
		{}
		
		template< typename EvDef >
		event::wlparam create_message()const 
		{ 
			typedef typename event_message_arg<EvDef>::args args;
			return args::encode_whole( a1_, a2_ ); 
		}
		
	private:
		A1 a1_; A2 a2_;
	};
	
  // �\�z�֐�
	/*
	template< typename A1 >
	some_event_message< get_text, typelist<const A1&> > get_text( const A1& a1 )
	{ return some_event_message< get_text, typelist<const A1&> >( a1 ); }
	*/
	
}}

#endif
