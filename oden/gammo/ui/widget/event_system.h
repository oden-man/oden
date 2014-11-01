#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_EVENT_SYS
#define ODEN_GAMMO_UI_WIDGETS_EVENT_SYS

//#############################################
//  インクルード
//
// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl -------------
// --------- win ------------
// --------- boost ----------
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/eval_if.hpp>
// --------- oden  ----------
#include <oden/object/container.h>
// --------- gammo --------
#include <oden/gammo/common/mini_mpl.h>
#include <oden/gammo/ui/widget/indirect_widget.h>
#include <oden/gammo/ui/window/event.h>

//#include <boost/mpl/print.hpp>

//
//############################################

#pragma warning(push)
//#pragma warning(disable:4512) // 代入演算子を生成できない


namespace oden { namespace gammo
{

namespace detail
{


/*-------------------------------------------------------------------------------------------------------------------------*/
//
//	引数を用意しスロットを実行する
//
/*-------------------------------------------------------------------------------------------------------------------------*/
//##################################################
//  イベントローカル引数を準備する
//
  // 特殊化によるユーザー定義もいちおう可能！
	template< typename EvDef, typename At >
	struct event_arg_cradle
	{
		typedef typename event_arg_def<EvDef, At>::type def;
		typedef event::arg_cradle<def> type;
	};

//##################################################
//  イベントローカル引数を準備し、ハンドラ関数に渡す
//
  // テンプレート
	template< typename Sig, typename Parameter = typelist<> >
	struct event_frame 
	{};

  //  引数なし
	template< typename Sig >
	struct event_frame< Sig, typelist<> >
	{
		template< typename Handler >
		void enter( Handler& handler, window_event& )
		{
			handler();
		}
	};

  //  引数1
	template< typename Sig, typename A1 >
	struct event_frame< Sig, typelist<A1> >
	{
		template< typename Handler >
		void enter( Handler& handler, window_event& wev )
		{
			typename event_arg_cradle<Sig,A1>::type l1(wev);
			
			handler(l1.value);
		}
	};

  //  引数2
	template< typename Sig, typename A1, typename A2 >
	struct event_frame< Sig, typelist<A1,A2> >
	{
		template< typename Handler >
		void enter( Handler& handler, window_event& wev )
		{
			typename event_arg_cradle<Sig,A1>::type l1(wev);
			typename event_arg_cradle<Sig,A2>::type l2(wev);

			handler(l1.value, l2.value);
		}
	};

  // 引数3
	template< typename Sig, typename A1, typename A2, typename A3 >
	struct event_frame< Sig, typelist<A1,A2,A3> >
	{
		template< typename Handler >
		void enter( Handler& handler, window_event& wev )
		{
			typename event_arg_cradle<Sig,A1>::type l1(wev);
			typename event_arg_cradle<Sig,A2>::type l2(wev);
			typename event_arg_cradle<Sig,A3>::type l3(wev);

			handler(l1.value, l2.value, l3.value);
		}
	};

/*-------------------------------------------------------------------------------------------------------------------------*/
//
//	イベントが起こったか検知するトリガー
//
/*-------------------------------------------------------------------------------------------------------------------------*/
//###################################################################
//   テンプレート
//
	template< typename Sig, typename Args >
	class event_trigger
	{};	
	
//###################################################################
//   メッセージコード + 静的引数で判断
//
	template< typename Sig >
	class event_trigger< Sig, typelist<> >
	{
	public:
		typedef Sig sig;

		bool fired( window_event& ev )const
		{
			return event_branch<sig>::branch( ev );
		}
	};
	
//###################################################################
//   メッセージコード + 一引数で判断
//
	template< typename Sig, typename Arg1 >
	class event_trigger< Sig, typelist<Arg1> >
	{
	public:
		typedef Sig sig;

		bool fired( window_event& ev )const
		{
			return event_branch<sig>::branch( ev, a1_ );
		}

		explicit event_trigger( const Arg1& a1 )
			: a1_( a1 )
		{}

	private:
		Arg1 a1_;
	};

//###################################################################
//   メッセージコード + 二引数で判断
//
	template< typename Sig, typename Arg1, typename Arg2 >
	class event_trigger< Sig, typelist<Arg1, Arg2> >
	{
	public:
		typedef Sig sig;

		bool fired( window_event& ev )const
		{
			return event_branch<sig>::branch( ev, a1_, a2_ );
		}

		event_trigger( const Arg1& a1, const Arg2& a2 )
			: a1_( a1 ), a2_( a2 )
		{}

	private:
		Arg1 a1_; Arg2 a2_;
	};

/*-------------------------------------------------------------------------------------------------------------------------*/
//
//	ハンドラクラス
//
/*-------------------------------------------------------------------------------------------------------------------------*/
//#####################################################
//	イベントハンドラークラス
//
  // ハンドラ挙動フラグ
	enum event_handler_state
	{
		handler_fired = 0x0001,
		handler_no_recurse = 0x0100,
		handler_once = 0x0200,
		handler_peeker = 0x0800
	};

  // インターフェース
	class event_handler
	{
	public:
		virtual void invoke( window_event& wev ) = 0;
		virtual int  state()const = 0;
	};

  // テンプレート
	template< typename Trigger, typename Handler, typename EventParam >
	class the_event_handler
		: public event_handler
	{
	public:
		// コンストラクタ
		the_event_handler( const Trigger& trigger, const Handler& hdl, int f )
			: trigger_( trigger ), handler_( hdl ), flags_( f )
		{}

		// メンバ関数
		void invoke( window_event& wev )
		{
			if( flags_ & handler_once && flags_ & handler_fired )
				return;
			else
				if( flags_ & handler_no_recurse && wev.call_depth() != 1 )
					return;

			// 反応をチェック、イベント引数を構築
			if( trigger_.fired( wev ) )
			{
				if( !(flags_ & handler_peeker) )
					wev.handled();

				typedef typename Trigger::sig event_sig;

				event_frame<event_sig, EventParam> frm;
				frm.enter( handler_, wev );

				flags_ |= handler_fired;
			}
		}

		int state()const     { return flags_; }

	private:
		Trigger	  trigger_;
		Handler	  handler_;
		int       flags_;
	};

  // イベントハンドラーが実行されたことが発動条件
	class event_handler_trigger
	{
	public:
		event_handler_trigger( const event_handler& r )
			: phdlr_( &r )
		{}

		bool fired()const
		{
			return phdlr_->state() & handler_fired;
		}

	private:
		const event_handler* phdlr_;
	};

/*-------------------------------------------------------------------------------------------------------------------------*/
//
//	ハンドラを生成する
//
/*-------------------------------------------------------------------------------------------------------------------------*/
//############################################################
//  イベントエイリアス名を解決
// 
	template< typename Src, typename T1 = unused, typename T2 = unused >
	struct alias_resolver
	{
	private:
		typedef typename indirect_widget<T1>::type c1;
		typedef typename indirect_widget<T2>::type c2;
	public:
		// コントロール型を取得し、エイリアス解決
		typedef typename event_alias< Src, c1, c2 >::type type;
	};
		
//####################################################################
//  さまざまな形式の関数を実行する
//
	struct invoker 
	{
	private:
	// 関数の種類を識別
	  // タグ
		struct callable{};
		struct callable_pointer{};
		struct member_function_pointer{};

	  // 2-1. 関数ポインタ・それ以外で分ける
		template< typename T, bool b_ptr = boost::is_pointer<T>::value >
		struct dispatch_some_ptr {};

		template< typename T > 	
		struct dispatch_some_ptr<T, true> { typedef callable_pointer type; };

		template< typename T > 	
		struct dispatch_some_ptr<T, false> { typedef callable type; };

	  // 1. メンバ関数ポインタ・それ以外で分ける
		template< typename T, bool b_pmfn = boost::is_member_function_pointer<T>::value >
		struct dispatch_mem_fun {};

		template< typename T > 	
		struct dispatch_mem_fun<T, true> { typedef member_function_pointer type; };

		template< typename T > 	
		struct dispatch_mem_fun<T, false> : dispatch_some_ptr<T> {};
		
	  // 0. 関数ディスパッチ用のタグを生成
		template< typename F >
		static typename dispatch_mem_fun<F>::type tag_dispatch() { return typename dispatch_mem_fun<F>::type(); }		

	// 実行
	  // apply_
		// 0
		template< typename F >
		static void apply_
		( F& f, callable ) { f(); }

		template< typename F >
		static void apply_
		( F& f, callable_pointer ) { (*f)(); }

		template< typename F, typename A1 >
		static void apply_
		( F& f, A1& a1, member_function_pointer ) { ( indirect<A1>::get(a1).*f )(); }
		
		// 1
		template< typename F, typename A1 >
		static void apply_
		( F& f, A1& a1, callable ) { f( a1 ); }

		template< typename F, typename A1 >
		static void apply_
		( F& f, A1& a1, callable_pointer ) { (*f)( a1 ); }

		template< typename F, typename A1, typename A2 >
		static void apply_
		( F& f, A1& a1, A2& a2, member_function_pointer ) { ( indirect<A1>::get(a1).*f )( a2 ); }
		
		// 2
		template< typename F, typename A1, typename A2 >
		static void apply_
		( F& f, A1& a1, A2& a2, callable ) { f( a1, a2 ); }

		template< typename F, typename A1, typename A2 >
		static void apply_
		( F& f, A1& a1, A2& a2, callable_pointer ) { (*f)( a1, a2 ); }

		template< typename F, typename A1, typename A2, typename A3 >
		static void apply_
		( F& f, A1& a1, A2& a2, A3& a3, member_function_pointer ) { ( indirect<A1>::get(a1).*f )( a2, a3 ); }

		// 3
		template< typename F, typename A1, typename A2, typename A3 >
		static void apply_
		( F& f, A1& a1, A2& a2, A3& a3, callable ) { f( a1, a2, a3 ); }

		template< typename F, typename A1, typename A2, typename A3 >
		static void apply_
		( F& f, A1& a1, A2& a2, A3& a3, callable_pointer ) { (*f)( a1, a2, a3 ); }

		template< typename F, typename A1, typename A2, typename A3, typename A4 >
		static void apply_
		( F& f, A1& a1, A2& a2, A3& a3, A4& a4, member_function_pointer ) { ( indirect<A1>::get(a1).*f )( a2, a3, a4 ); }

	public:
	// インターフェース	
	  // apply 
		template< typename F >
		static void apply( F& f ) { apply_( f, tag_dispatch<F>() ); }
		
		template< typename F, typename A1 >
		static void apply( F& f, A1& a1 ) { apply_( f, a1, tag_dispatch<F>() ); }
		
		template< typename F, typename A1, typename A2 >
		static void apply( F& f, A1& a1, A2& a2 ) { apply_( f, a1, a2, tag_dispatch<F>() ); }
		
		template< typename F, typename A1, typename A2, typename A3 >
		static void apply( F& f, A1& a1, A2& a2, A3& a3 ) { apply_( f, a1, a2, a3, tag_dispatch<F>() ); }
		
		template< typename F, typename A1, typename A2, typename A3, typename A4 >
		static void apply( F& f, A1& a1, A2& a2, A3& a3, A4& a4 ) { apply_( f, a1, a2, a3, a4, tag_dispatch<F>() ); }
	};

	// 引数型を保持できる型に直す
	template< typename T >
	class bind_arg
	{
	public:
		template< typename X >
		struct conv : boost::decay<X>
		{};

		typedef typename conv<T>::type type;

		bind_arg( const type& t )
			: value( t )
		{}

		type value;
	};

} /* end of namespace detail */

//##############################################################################
// ハンドラ実行クラス
//
  // 実行クラス Bind:束縛した引数 Event:イベント固有の引数
	template< typename F, typename Bind >
	struct slot_function
	{};

	template< typename F >
	struct slot_function< F, typelist<> >
	{
		slot_function( const F& f )
			: fn_(f)
		{}

		void operator()()const
		{ detail::invoker::apply( fn_ ); }

		template< typename E1 >
		void operator()( E1 &e1 )const
		{ detail::invoker::apply( fn_, e1 ); }

		template< typename E1, typename E2 >
		void operator()( E1 &e1, E2 &e2 )const
		{ detail::invoker::apply( fn_, e1, e2 ); }

		template< typename E1, typename E2, typename E3 >
		void operator()( E1 &e1, E2 &e2, E3 &e3 )const
		{ detail::invoker::apply( fn_, e1, e2, e3 ); }

	private:
		F fn_;
	};

	template< typename F, typename A1 >
	struct slot_function< F, typelist<A1> >
	{
		slot_function( const F& f, const A1& a1 )
			: fn_(f), a1_(a1)
		{}

		void operator()()const
		{ detail::invoker::apply( fn_, a1_.value ); }

		template< typename E1 >
		void operator()( E1 &e1 )const
		{ detail::invoker::apply( fn_, a1_.value, e1 ); }

		template< typename E1, typename E2 >
		void operator()( E1 &e1, E2 &e2 )const
		{ detail::invoker::apply( fn_, a1_.value, e1, e2 ); }

		template< typename E1, typename E2, typename E3 >
		void operator()( E1 &e1, E2 &e2, E3 &e3 )const
		{ detail::invoker::apply( fn_, a1_.value, e1, e2, e3 ); }

	private:
		F fn_; 
		detail::bind_arg<A1> a1_;
	};

	template< typename F, typename A1, typename A2 >
	struct slot_function< F, typelist<A1,A2> >
	{
		slot_function( const F& f, const A1& a1, const A2& a2 )
			: fn_(f), a1_(a1), a2_(a2)
		{}

		void operator()()const
		{ detail::invoker::apply( fn_, a1_.value, a2_.value ); }

	private:
		F fn_; 
		detail::bind_arg<A1> a1_;
		detail::bind_arg<A2> a2_;
	};

	template< typename F, typename A1, typename A2, typename A3 >
	struct slot_function< F, typelist<A1,A2,A3> >
	{
		slot_function( const F& f, const A1& a1, const A2& a2, const A3& a3 )
			: fn_(f), a1_(a1), a2_(a2), a3_(a3)
		{}

		void operator()()const
		{ detail::invoker::apply( fn_, a1_.value, a2_.value, a3_.value ); }

	private:
		F fn_;
		detail::bind_arg<A1> a1_;
		detail::bind_arg<A2> a2_;
		detail::bind_arg<A3> a3_;
	};
	
  //  関数で生成
	template< typename F >  
	slot_function< F, typelist<> >
		slot_func ( const F& f ) 
	{
		return slot_function< F, typelist<> >(f);
	}
	template< typename F, typename A1 >  
	slot_function< F, typelist<A1> >
		slot_func ( const F& f, const A1& a1 ) 
	{
		return slot_function< F, typelist<A1> >(f,a1);
	}
	template< typename F, typename A1, typename A2 >  
	slot_function< F, typelist<A1,A2> >
		slot_func ( const F& f, const A1& a1, const A2& a2 ) 
	{
		return slot_function< F, typelist<A1,A2> >(f,a1,a2);
	}
	template< typename F, typename A1, typename A2, typename A3 >  
	slot_function< F, typelist<A1,A2,A3> >
		slot_func ( const F& f, const A1& a1, const A2& a2, const A3& a3 ) 
	{
		return slot_function< F, typelist<A1,A2,A3> >(f,a1,a2,a3);
	}


//##############################################################################
// イベント引数を指定するハンドラ
//
  // イベント引数リストを保持しておく
	template< typename F, typename EParam >
	struct slot_event_function
	{
		typedef EParam							event_param_list;
		typedef slot_function< F, typelist<> >	func_type;
		
		slot_event_function( const F& f )
			: function(f)
		{}		

		func_type function;
	};	


namespace detail
{
//_____________________________________________________________________________________________________
//
//                              イベントハンドラ 生成文
//_____________________________________________________________________________________________________
//
//###########################################
//  引数つきハンドラ作成構文 [ Handler ] or .arg(A1,...)
// 
	template< typename Trigger, typename Master >
	class event_handler_gen
	{
	private:
		typedef event_handler_gen< Trigger, Master > self;
		typedef typename Trigger::sig				 sig;
		typedef event_arg<sig>						 e_args;

	public:
	// コンストラクタ
		event_handler_gen( const Trigger& tg, Master* pmaster )
			: trigger_(tg), master_(pmaster), flags_(0)
		{}
	
	// ジェネレーター
      // スロット
		// 変換なし
		template< typename EventParam, typename Handler >
		self& make_slot( const Handler& h ) 
		{
			master_->add_handler( new the_event_handler< Trigger, Handler, EventParam >( trigger_, h, flags_ ) );
			flags_ = 0;
			return *this;
		}

	  // 関数、関数オブジェクトをラップ
		// イベントパラメータリストを自分で指定
		template< typename E, typename F >
		self& slotf( const F& f )
		{
			return make_slot<E>( slot_func( f ) ); 
		}
		template< typename E, typename F, typename A1 >
		self& slotf( const F& f, const A1& a1 )
		{  
			return make_slot<E>( slot_func( f, a1 ) ); 
		}
		template< typename E, typename F, typename A1, typename A2 >
		self& slotf( const F& f, const A1& a1, const A2& a2 )
		{
			return make_slot<E>( slot_func( f, a1, a2 ) ); 
		}
		
	  // インターフェース
	   // メンバ関数ポインタ 
		// 型導出
		template< typename EParams, typename Params, 
				typename Class = typename indirect< typename typelist_at<Params,0>::type >::type, 
				bool Enable = boost::is_class<Class>::value >
		struct mfn_ptr
		{
			// Params先頭はクラスのインスタンスが入る

			template< typename C, typename EParams, typename Params >
			struct ptr;
			
			template< typename C, typename A1 > 
			struct ptr< C, typelist<>, typelist<A1> > 	
			{ typedef void (C::*type)( ); };
			
			template< typename C, typename E1, typename A1 >
			struct ptr< C, typelist<E1>, typelist<A1> > 
			{ typedef void (C::*type)( typename event_arg_param_type<sig,E1>::type ); };
			  
			template< typename C, typename E1, typename E2, typename A1 >
			struct ptr< C, typelist<E1,E2>, typelist<A1> >
			{ typedef void (C::*type)( typename event_arg_param_type<sig,E1>::type, typename event_arg_param_type<sig,E2>::type ); };

			template< typename C, typename E1, typename E2, typename E3, typename A1 >
			struct ptr< C, typelist<E1,E2,E3>, typelist<A1> >
			{ typedef void (C::*type)( typename event_arg_param_type<sig,E1>::type, typename event_arg_param_type<sig,E2>::type, typename event_arg_type<sig,E3>::type ); };

			template< typename C, typename A1, typename A2 > 
			struct ptr< C, typelist<>, typelist<A1,A2> > 	
			{ typedef void (C::*type)( A2 ); };

			typedef typename ptr<Class, EParams, Params>::type type;

			enum { disabled = 0 };
		}; 
		template< typename EP, typename P, typename C >
		struct mfn_ptr<EP, P, C, false>
		{
			enum { disabled = 1 };
		};

		// 1,0~3
		template< typename A1 >
		self& slot( typename mfn_ptr< typelist<>, typelist<A1> >::type f, const A1& a1 )
		{ return slotf< typelist<> >( f, a1 ); }

		template< typename E1, typename A1 >
		self& slot( typename mfn_ptr< typelist<E1>, typelist<A1> >::type f, const A1& a1 )
		{ return slotf< typelist<E1> >( f, a1 ); }

		template< typename E1, typename E2, typename A1 >
		self& slot( typename mfn_ptr< typelist<E1, E2>, typelist<A1> >::type f, const A1& a1 )
		{ return slotf< typelist<E1, E2> >( f, a1 ); }

		template< typename E1, typename E2, typename E3, typename A1 >
		self& slot( typename mfn_ptr< typelist<E1, E2, E3>, typelist<A1> >::type f, const A1& a1 )
		{ return slotf< typelist<E1, E2, E3> >( f, a1 ); }

	   // グローバル関数ポインタ	
		// 型導出
		template< typename EParams, typename Params = typelist<> >
		struct fn_ptr
		{
			template< typename EParams, typename Params >
			struct ptr;

			template<>
			struct ptr< typelist<>, typelist<> >
			{ typedef void (*type)(); };

			template< typename E1 >
			struct ptr< typelist<E1>, typelist<> >
			{ typedef void (*type)(typename event_arg_param_type<sig, E1>::type); };

			template< typename E1, typename E2 >
			struct ptr< typelist<E1, E2>, typelist<> >
			{ typedef void (*type)(typename event_arg_param_type<sig, E1>::type, typename event_arg_param_type<sig, E2>::type); };

			template< typename E1, typename E2, typename E3 >
			struct ptr< typelist<E1, E2, E3>, typelist<> >
			{ typedef void (*type)(typename event_arg_param_type<sig, E1>::type, typename event_arg_param_type<sig, E2>::type, typename event_arg_type<sig, E3>::type); };

			template< typename A1 >
			struct ptr< typelist<>, typelist<A1> >
			{ typedef void (*type)(A1); };

			template< typename E1, typename A1 >
			struct ptr< typelist<E1>, typelist<A1> >
			{ typedef void (*type)(typename event_arg_param_type<sig, E1>::type, A1); };

			template< typename A1, typename A2 >
			struct ptr< typelist<>, typelist<A1,A2> >
			{ typedef void (*type)(A1,A2); };

			typedef typename ptr<EParams, Params>::type type;
		};
		
		// 0,0~3
		self& slot( typename fn_ptr< typelist<> >::type f )
		{ return slotf< typelist<> >( f ); }

		template< typename E1 >
		self& slot( typename fn_ptr< typelist<E1> >::type f )
		{ return slotf< typelist<E1> >( f ); }

		template< typename E1, typename E2 >
		self& slot( typename fn_ptr< typelist<E1, E2> >::type f )
		{ return slotf< typelist<E1, E2> >( f ); }

		template< typename E1, typename E2, typename E3 >
		self& slot( typename fn_ptr< typelist<E1, E2, E3> >::type f )
		{ return slotf< typelist<E1, E2, E3> >( f ); }

		// 1,0~1
		template< typename A1 >
		self& slot( typename fn_ptr< typelist<>, typelist<A1> >::type f, const A1& a1 )
		{ return slotf< typelist<> >( f, a1 ); }

		template< typename E1, typename A1 >
		self& slot( typename fn_ptr< typelist<E1>, typelist<A1> >::type f, const A1& a1 )
		{ return slotf< typelist<E1> >( f, a1 ); }

		// 2,0
		template< typename A1, typename A2 >
		self& slot( typename fn_ptr< typelist<>, typelist<A1,A2> >::type f, const A1& a1, const A2& a2 )
		{ return slotf< typelist<> >( f, a1, a2 ); }

	   // その他（関数オブジェクト、シグネチャの異なる関数ポインタ・メンバ関数ポインタ）		
		// 型導出
		template< typename F, typename EParams, typename Params >
		struct is_other_functor
		{
			// SFINAE用
			typedef char( &yes_type )[2];
			typedef char no_type;

			static const F& sample; // 実際に中身を参照しないので、これでよし

			// mfn チェック
			template< typename MFN, int Disabled = MFN::disabled >
			struct match_mfn_ptr
			{
				static yes_type test( typename MFN::type );
				static no_type  test( ... );

				enum { value = (sizeof(test( sample )) == sizeof(yes_type)) ? 1 : 0 };
			};
			template< typename M >
			struct match_mfn_ptr< M, 1 >
			{
				enum { value = 0 };
			};

			// pfn チェック
			struct match_fn_ptr
			{
				static yes_type test( typename fn_ptr< EParams, Params >::type );
				static no_type  test( ... );

				enum { value = (sizeof(test( sample )) == sizeof(yes_type)) ? 1 : 0 };
			};

			enum { value = ( match_mfn_ptr< mfn_ptr<EParams, Params> >::value + match_fn_ptr::value )==0 ? 1 : 0 };
		};
		template< typename F, typename EP, typename P = typelist<>, int B = is_other_functor<F,EP,P>::value >
		struct fn_other
		{
			typedef self& type;
		};
		template< typename F, typename EP, typename P >
		struct fn_other<F, EP, P, 0>
		{};

		// 0,0~3
		template< typename F >
		typename fn_other< F, typelist<> >::type 
			slot( const F& f )
		{  return slotf< typelist<> >( f ); }

		template< typename E1, typename F >
		typename fn_other< F, typelist<E1> >::type 
			slot( const F& f )
		{  return slotf< typelist<E1> >( f ); }

		template< typename E1, typename E2, typename F >
		typename fn_other< F, typelist<E1,E2> >::type 
			slot( const F& f )
		{  return slotf< typelist<E1,E2> >( f ); }

		template< typename E1, typename E2, typename E3, typename F >
		typename fn_other< F, typelist<E1,E2,E3> >::type 
			slot( const F& f )
		{  return slotf< typelist<E1,E2,E3> >( f ); }

		// 1,0~3
		template< typename F, typename A1 >
		typename fn_other< F, typelist<>, typelist<A1> >::type 
			slot( const F& f, const A1& a1 )
		{  return slotf< typelist<> >( f, a1 ); }

		template< typename E1, typename F, typename A1 >
		typename fn_other< F, typelist<E1>, typelist<A1> >::type
			slot( const F& f, const A1& a1 )
		{  return slotf< typelist<E1> >( f, a1 ); }

		template< typename E1, typename E2, typename F, typename A1 >
		typename fn_other< F, typelist<E1,E2>, typelist<A1> >::type
			slot( const F& f, const A1& a1 )
		{  return slotf< typelist<E1,E2> >( f, a1 ); }

		template< typename E1, typename E2, typename E3, typename F, typename A1 >
		typename fn_other< F, typelist<E1,E2,E3>, typelist<A1> >::type
			slot( const F& f, const A1& a1 )
		{  return slotf< typelist<E1,E2,E3> >( f, a1 ); }

		// 2,0
		template< typename F, typename A1, typename A2 >
		typename fn_other< F, typelist<>, typelist<A1,A2> >::type
			slot( const F& f, const A1& a1, const A2& a2 )
		{  return slotf< typelist<> >( f, a1, a2 ); }

		// 3,0
		template< typename F, typename A1, typename A2, typename A3 >
		typename fn_other< F, typelist<>, typelist<A1,A2,A3> >::type
			slot( const F& f, const A1& a1, const A2& a2, const A3& a3 )
		{  return slotf< typelist<> >( f, a1, a2, a3 ); }
		
       // イベント引数型を型で指定したスロット
		template< typename F, typename EP >
		self& slot( const slot_event_function< F, EP >& fn )
		{ return make_slot<EP>( fn.function ); }

      // フラグの指定
		self& norec()
		{ flags_ |= event_handler::call_no_recurse; return *this; }

		self& once()
		{ flags_ |= event_handler::call_once; return *this; }

		self& peek()
		{ flags_ |= event_handler::peeker; return *this; }

	private:
		Trigger trigger_;
		Master* master_;
		int     flags_;
	};	

//############################################################
//  引数つきハンドラ作成 構文の起点
// 
	class event_handler_master
	{
	private:
	// 次のevent_handler_gen の型を返す
		template< typename Resolver, typename Args = typelist<> >
		struct ret
		{
			typedef typename Resolver::type sig;

			typedef event_trigger< sig, Args > trigger;
			typedef event_handler_gen< trigger, event_handler_master > type;
		};
		
		template< typename Ret >
		typename Ret::type
			signal_()
		{
			return typename Ret::type( typename Ret::trigger(), this );
		}
		template< typename Ret, typename A1 >
		typename Ret::type
			signal_( const A1& a1 )
		{
			return typename Ret::type( typename Ret::trigger(a1), this );
		}
		template< typename Ret, typename A1, typename A2 >
		typename Ret::type
			signal_( const A1& a1, const A2& a2 )
		{
			return typename Ret::type( typename Ret::trigger(a1,a2), this );
		}

	public:
		typedef oden::object_container<detail::event_handler>	container;
		typedef std::vector<container::element>					list_type;

	// ジェネレーター
	  // 構文を開始
		template< typename Sig >
		typename ret< alias_resolver<Sig> >::type // signal<S>()
			signal() 
		{
			return this->signal_< ret< alias_resolver<Sig> > >();
		}

		template< typename Sig, typename T1 >
		typename ret< alias_resolver<Sig,T1> >::type // signal<S,T1>()
			signal() 
		{
			return this->signal_< ret< alias_resolver<Sig,T1> > >();
		}

		template< typename Sig, typename A1 >
		typename ret< alias_resolver<Sig,A1>, typelist<A1> >::type // signal<S>(a1)
			signal( const A1& a1 ) 
		{
			return this->signal_< ret< alias_resolver<Sig,A1>, typelist<A1> > >( a1 );
		}

		template< typename Sig, typename T1, typename A1 >
		typename ret< alias_resolver<Sig,T1>, typelist<A1> >::type // signal<S,T1>(a1)
			signal( const A1& a1 ) 
		{
			return this->signal_< ret< alias_resolver<Sig,T1>, typelist<A1> > >( a1 );
		}

		template< typename Sig, typename A1, typename A2 >
		typename ret< alias_resolver<Sig,A1,A2>, typelist<A1,A2> >::type // signal<S>(a1,a2)
			signal( const A1& a1, const A2& a2 ) 
		{
			return this->signal_< ret< alias_resolver<Sig,A1,A2>, typelist<A1,A2> > >( a1, a2 );
		}

	  // ハンドラを追加
		void add_handler( event_handler* pnhdr )
		{
			container::push_back( handlers_, pnhdr );
		}

	// メンバ関数
	protected:
		void invoke_all_slot( window_event& ev )
		{
			// ハンドラを回す
			list_type::const_iterator itr = handlers_.begin(), eitr = handlers_.end();
			for( ; itr != eitr; ++itr )
				(*itr)->invoke(ev);
		}
		
	private:
		list_type handlers_;
	};

} /* end of namespace detail */


}}

#pragma warning(pop)

#endif








