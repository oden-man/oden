#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_WND
#define ODEN_GAMMO_UI_WIDGETS_WND

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl -------------
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
// --------- gammo ---------
#include <oden/gammo/ui/widget/base.h>
#include <oden/gammo/ui/layout/base.h>

//
//####################################################


namespace oden { namespace gammo
{
//______________________________________________________________________________________________________
//
//          ウィジェット・コントロール制作
//______________________________________________________________________________________________________
//
//#################################################
//  ウィンドウクラスの扱い
// 
  // 新規作成
	class new_window_class
	{
	public:
		new_window_class()
		{
			this->describe_window_class()
				.background( COLOR_WINDOW );
		}

		window_class_id supply_class_id()
		{
			cls_.procedure( &widget_hub::static_window_proc );
			return register_window_class( cls_ ).get_id(); // 必ずアトムだから問題ない
		}

		bool is_widget_window_class()const
		{
			return true;
		}

		window_class::write_val& describe_window_class(){ return cls_; }
	private:
		window_class::write_val cls_;
	};

  // IDで指定
	class defined_window_class
	{
	public:
		const window_class_id& supply_class_id()const
		{
			return id_.get();
		}

		bool is_widget_window_class()const
		{
			// プロシージャがwidget_hubのか調べる
			window_class::value_type wcex;
			if( 0 != ::GetClassInfoEx( get_app_handle().value(), id_.get_id(), &wcex ) )
			{
				return wcex.lpfnWndProc == &widget_hub::static_window_proc;
			}
			return false;
		}

		void describe_window_class( window_class_id i ){ id_ = i; }
	private:
		window_class_id_val id_;
	};

  // コントロールクラス
	template< typename T >
	class control_window_class
	{
	public:
		window_class_id supply_class_id()const
		{
			return T::get_control_class();
		}

		bool is_widget_window_class()const
		{
			return false;
		}
	};

//#################################################
//  ウィンドウを生成
// 
	template< typename Base, typename ClassSupply = new_window_class >
	class new_window
		: public Base, public ClassSupply
	{
	public:
		bool create_control();
	
		window_create_info::write_val& describe_window(){ return param_; }
	private:
		window_create_info::write_val param_;
	};

	// create_control
	template< typename B, typename C >
	bool new_window<B,C>::create_control()
	{
		if( has_parent_widget() ) 
		{
			param_.parent( this->get_parent_widget()->get_window_handle() );
			param_.child_id( this->get_widget_child_id() );
		}

		if( this->is_widget_window_class() )
		{
			if( widget_hub* phub = widget_hub::this_thread_hub() ) 
				phub->set_creating_widget( *static_cast<widget_object*>(this) );
			else
				return false;
		}

		if( this->test_widget_flag( widget_invisible ) )
			param_.style( param_.get().style & ~WS_VISIBLE );
		else
			param_.style( param_.get().style | WS_VISIBLE );

		if( window_handle h = create_window( supply_class_id(), param_ ) )
		{
			this->set_window_handle(h);
			return true;
		}
		return false;
	}
	
//#################################################
//  ウィンドウを取得
// 
	template< typename Base >
	class another_window
		: public Base
	{
	public:
		bool create_control(){ return true; }
	
	// initialize の中で呼ぶべし
		void widget_is_child( int id )
		{ if( has_parent_widget() ) set_window_handle( get_parent_widget()->get_child(id) ); }

		void widget_is_parent()
		{ if( has_parent_widget() ) set_window_handle( get_parent_widget()->get_window_handle() ); }

		void widget_is( window_handle h )
		{ set_window_handle( h ); }

	};


namespace ui_layout
{
//______________________________________________________________________________________________________
//
//          ウィジェット・レイアウト管理
//______________________________________________________________________________________________________
//
//####################################################
//  ウィンドウを自動レイアウト - 子ウィジェットの調整も行う
//
	template< typename Base >
	class master
		: public Base
	{
	public:
	// コンストラクタ
		master(){}
		virtual ~master(){}
		
	//オーバーライド
	  // 最適なサイズを返す - 子レイアウトと同じ
		virtual void calc_best_size( size& out )const
		{
			if( layout_tree_ )
				out = layout_tree_->best_size();
			else
				this->Base::calc_best_size(out);
		}
		
	// メンバ関数		
	  // レイアウトをセット
		template< typename T >
		T& describe_layout( T* pNew )
		{
			if( !layout_tree_ ) {
				layout_tree_.reset(pNew);
			}
			
			return *pNew;
		}
		fill_layout& describe_layout()
		{ return this->describe_layout( new fill_layout() ); }

	// 共通
	  // イベントに応じて作動する
		void handle_layout_event( window_event& ev );
	  // このレイアウトを動かす
		virtual bool move_widget( move_widget_context& c );

	private:
	// 内部関数
	  // 子レイアウトを動かす
		bool move_child_wid();
	  // 子以下を再計算する
		bool recalc_child_wid();
	  // 管理する領域を取得( クライアント座標 )
		virtual rect get_managed_rect()const
		{ return this->client_rect(); }
		
	// メンバ変数
		boost::scoped_ptr<layout_object> layout_tree_;
	};

	template< typename B >
	void master<B>::handle_layout_event( window_event& ev )
	{
		switch( ev.code() )
		{
		case WM_SIZE:
			recalc_child_wid();	
			move_child_wid();
			break;
		case WM_SIZING:
			//limit_child_wid(this, size(0,0));
			break;
		default:
			ev.not_handled();
			break;
		}
	}

	template< typename B >
	bool master<B>::move_widget( move_widget_context& c )
	{
		// ターミナルレイアウトとして自分が動く->子供たちを動かす
		if( this->B::move_widget(c) ) {
			return move_child_wid();
		}
		return false;
	}
	
	template< typename B >
	bool master<B>::move_child_wid()
	{
		if( !has_child_widget() )
			return true;

		bool ret = true;

		// dwp はじめ
		move_widget_context this_cx;
		this_cx.dwp.start( count_child_widget() );
		
		// 子ウィジェットを動かす
		widget_object::widget_iterator itr = begin(), eitr = end();
		for( ; itr!=eitr; ++itr )
		{
			if( !(*itr)->move_widget(this_cx) )
			{
				ret = false;
				break;
			}
		}
		
		// dwp を終了、子らを動かす
		this_cx.dwp.end();
		
		return ret;
	}

	template< typename B >
	bool master<B>::recalc_child_wid()
	{
		if( layout_tree_ ) {
			// レイアウト再計算
			layout_tree_->recalc( this->get_managed_rect() );
		}
		return true;
	}

//####################################################
//  ウィンドウの一部に子レイアウトを配置
//
	template< typename B >
	class partial_master
		: public master<B>
	{
	public:
	// オーバーライド関数
      // 管理する領域を返す
		rect get_managed_rect()const
		{ return this->widget_location(); }

	  // このレイアウトは自力では動かない
		bool move_widget( move_widget_context& ){ return true; }
	};

}

//______________________________________________________________________________________________________
//
//          その他基本ウィジェット拡張部品
//______________________________________________________________________________________________________
//
//#################################################
// サブクラス機能を提供  
// 
	template< typename B >
	class subclass_widget
		: public B
	{
	public:
	// コンストラクタ
		subclass_widget()
			: original_proc_(0)
		{}

	// オーバーライド
	  // 最終プロシージャ
		void final_handle_event( window_event& ev );

	// メンバ関数
	  // サブクラス化
		void subclass();
	  // 元に戻す
		void unsubclass();
	  // 元のプロシージャを呼ぶ
		void call_original_proc( window_event& ev );
      // サブクラス化されているか?
		bool is_subclassed()const { return original_proc_ != 0; }
	private:
		WNDPROC original_proc_;
	};

	template< typename Base >
	void subclass_widget<Base>::final_handle_event( window_event& ev )
	{
		if( is_subclassed() )
			call_original_proc( ev );
		else
			this->Base::final_handle_event( ev );
	}
	
	template< typename B >
	void subclass_widget<B>::subclass()
	{
		if( is_subclassed() )
			return;
		
		original_proc_ = procedure_ptr();
		set_procedure_ptr( &widget_hub::static_window_proc );
		
		// ウィジェットハブに追加する
		wid->attach_event_loop();
	}

	template< typename B >
	void subclass_widget<B>::unsubclass()
	{
		if( !is_subclassed() )
			return;
		
		set_procedure_ptr( original_proc_ );
		original_proc_ = 0;

		// ウィジェットハブから取り除く
		wid->detach_event_loop();
	}

	template< typename B >
	void subclass_widget<B>::call_original_proc( window_event& ev )
	{
		ev.result() = ::CallWindowProc( original_proc_, ev.target().value(), ev.code(), ev.wparam(), ev.lparam() ); 
	}

//______________________________________________________________________________________________________
//
//          ウィジェットコア・プリセット
//______________________________________________________________________________________________________
//
  // widget typedef 
	typedef widget<window> 		window_widget;

	class dialog;
	typedef widget<dialog> 		dialog_widget;

  // メインウィンドウ
	template< typename T = gammo::window >
	struct new_widget 
		: public new_window< widget<T> >
	{};

  // コントロール
	template< typename T >
	struct ctl_widget 
		: public subclass_widget< new_window< widget<T>, control_window_class<T> > >
	{
		typedef subclass_widget< another_window< widget<T> > > resource;
	};

}} /* end of namespace oden.gammo */

#endif

