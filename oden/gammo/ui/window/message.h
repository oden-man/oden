#pragma once

#ifndef ODEN_GAMMO_UI_WND_MSG
#define ODEN_GAMMO_UI_WND_MSG

//#####################################################
//  インクルード
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
// メッセージ送信のためのオブジェクト
//
	class window_message
	{
	public:
	// コンストラクタ
		window_message( UINT c, WPARAM wp = 0, LPARAM lp = 0 ):
			code_(c),wParam_(wp),lParam_(lp)
		{}
		
	// メンバ関数
		UINT   code()const 		{ return code_; }
		WPARAM wparam()const	{ return wParam_; }
		LPARAM lparam()const	{ return lParam_; }		
		
	private:
		UINT   code_;
		WPARAM wParam_;
		LPARAM lParam_;
	};

//############################################################
//  イベントエンコーダエイリアス名を解決
// 
	template< typename Src, typename Obj >
	struct event_message_alias_resolver
	{
	private:
		typedef typename indirect_widget<Obj>::type o;
	public:
		// コントロール型を取得し、エイリアス解決
		typedef typename event_message_alias< Src, o >::type type;
	};


//############################################################
//  イベントを作成
//	
	template< typename Derived, typename Sig >
	class event_message_base
	{
	public:
		typedef Sig								sig;
		typedef event_message_base<Derived,Sig>	self;
		
	// コンストラクタ
	protected:
		event_message_base(){}
	public:
	
	// メンバ関数
	  // 確定する
		template< typename Object >
		window_message make( Object& )const
		{
			typedef event_message_alias_resolver<sig, Object>::type 
			resolved_event_def;
			
			const Derived* deriv = static_cast<const Derived*>(this);
			event::wlparam p = deriv->create_message<resolved_event_def>();
			
			return window_message( resolved_event_def::code, p.w, p.l );
		}
		
	  // メッセージ返り値型を取得
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

	// 演算子オーバーロード
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
//   event_message - エンコードを行う
//
  // 確定したメッセージ
	template< typename Sig, int Size = event_message_arity<Sig>::value > // Sizeを省略した場合、event_argが実体化される
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
  // 確定していないメッセージ
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
	
  // 構築関数
	/*
	template< typename A1 >
	some_event_message< get_text, typelist<const A1&> > get_text( const A1& a1 )
	{ return some_event_message< get_text, typelist<const A1&> >( a1 ); }
	*/
	
}}

#endif
