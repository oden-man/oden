#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_BASE
#define ODEN_GAMMO_UI_WIDGETS_BASE

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
#include <boost/unordered_map.hpp>
// --------- oden  ----------
#include <oden/object/scoped.h>
#include <oden/object/container.h>
// --------- gammo ---------
#include <oden/gammo/ui/window/window.h>
#include <oden/gammo/ui/widget/object.h>
#include <oden/gammo/ui/widget/event_system.h>
#include <oden/gammo/system/thread/tls.h>

//
//####################################################


namespace oden { namespace gammo
{

//_____________________________________________________________________________________________________
//
//                              ウィジェット　コア基底クラス
//_____________________________________________________________________________________________________
//
//#################################################
//  move_widgetにわたす
//	
	class move_widget_context
	{
	public:
		defer_window_pos dwp;
	};
	
//#################################################
//  オーバーライド用
// 
	class widget_base
		: public widget_object, public detail::event_handler_master
	{
	protected:
		widget_base() : location_( 10, 10, 20, 20 ) {}
	public:
		virtual ~widget_base(){}

	// メンバ関数
	  // イベント処理・オーバーライド
		virtual void handle_event			( window_event& );
		virtual void handle_layout_event	( window_event& ev ){ ev.not_handled(); }
		virtual void my_handle_event		( window_event& ev );
		virtual void final_handle_event		( window_event& );
		virtual bool preprocess_event		( preprocessing_event& ){ return true; }
		bool attach_event_loop();
		bool detach_event_loop();

	  // レイアウト処理
		bool move_widget					( move_widget_context& c );
		void set_widget_location			( const rect& r ) { location_ = r; }
		const rect& widget_location			()const { return location_; }
		void get_best_size					( size& out )const;
		void fit_widget ();
		void best_size_changed ();
		
	private:
		rect location_;
		mutable size best_size_cache_;
	};

//#################################################
//  コントロールと作成方法を決定しておく  
// 
	template< typename T >
	class widget
		: public widget_base, public T
	{
	public:	
		typedef T control_type;

	// コンストラクタ
		widget(){}
		virtual ~widget()
		{
			this->widget_object::destroy();
		}

	// メンバ関数
	  // ウィンドウへ
		window& to_window(){ return *this; }
		const window& to_window()const { return *this; }

	  // コントロールへ
		T& to_control(){ return *this; }
		const T& to_control()const { return *this; }
		
	  // window::destroyを上書き
		bool destroy(){ return this->widget_object::destroy(); }

	  // デリータも一応上書き
		typedef struct destroyer {
			void operator()( widget& o )const { o.destroy(); }
		} deleter_type;
		
	  // レイアウト・オーバーライド
		virtual void get_best_size_now( size& out )const
		{ /*to_control*/to_window().calc_best_size(out); }
	};
	
//################################################################
// プリセットのハンドラ 
//
  // イベントを転送
	struct widget_event_transfer
	{
		void operator()( widget_object* p, window_event& e )const
		{
			p->handle_event(e);
			e.not_handled();
		}
	};
	//typedef slot_function< widget_event_transfer, slot_bind_param<widget_object*>, event_param<event::args> > transfer_event;

//_________________________________________________________________________________________________________
//
//           ウィジェットをまとめる機構
//_________________________________________________________________________________________________________
//	
//################################################################
//  ウィンドウプリプロセスプロシージャ
//	
	class preprocessing_event
	{
	public:
	// コンストラクタ
		preprocessing_event()
			: pmsg_(0), processed_(false)
		{}

	// メンバ関数
	  // 取得
		raw_window_message& msg(){ return *pmsg_; }
		const raw_window_message& msg()const { return *pmsg_; }

		HWND& rhwnd(){ return msg().hwnd; }
		HWND hwnd(){ return msg().hwnd; }

		UINT& rcode(){ return msg().message; }
		UINT code(){ return msg().message; }
		
	  // 更新
		void set( raw_window_message& out )
		{
			pmsg_ = &out;
			processed_ = false;
		}

	  // 処理されたか？フラグの操作
		void set_processed() { processed_ = true; }
		bool is_processed()const { return processed_; }

	private:
		raw_window_message* pmsg_;
		bool processed_;
	};
	
//###################################################################
// イベント分配ハブ
//	
	class widget_hub
	{
	private:
		typedef boost::unordered_map<HWND, widget_object*> 	widget_map;
		typedef std::vector<widget_object*> 				widget_vector;
		
	public:	
	// コンストラクタ
		widget_hub()
			: cur_widget_ptr_(0), widget_map_(), proc_call_depth_(0)
		{}
		
	// メンバ関数
		// メッセージをハンドラテーブルへと送信
		bool proc_entry( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& ret );
		// メッセージを前処理
		bool preprocess_entry( raw_window_message& out );
		// 対象ウィジェットを探し出して変更
		void set_current_widget( window_handle hwnd );
		// 作成中のウィンドウを設定する
		void set_creating_widget( widget_object& wid );
		// ウィジェットをマップに追加・削除する
		bool add_widget( widget_object* wid );
		bool remove_widget( window_handle hwnd );
		// メッセージを先に処理するウィジェットを追加
		void add_event_filter( widget_object* pmp );
		bool remove_event_filter( widget_object* pmp );
		// 
		int proc_entry_depth()const;
		
	// 静的メンバ関数
	  // tl_hub操作
		static bool create_this_thread_hub();
		static widget_hub* this_thread_hub();
		static bool release_this_thread_hub();

	  // メッセージプロシージャ
		// WNDPROC用メッセージプロシージャ
		static LRESULT CALLBACK static_window_proc	( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		// DLGPROC用メッセージプロシージャ
		static INT_PTR CALLBACK static_dialog_proc	( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );

		// デフォルトのウィンドウプロシージャを呼ぶ
		static void call_default_window_proc( window_event& arg );
		// デフォルトのダイアログプロシージャを呼ぶ
		static void call_default_dialog_proc( window_event& arg );
		
	private:
		widget_object*	cur_widget_ptr_;	// 現在のウィジェットへのポインタ
		widget_map		widget_map_;		// 
		widget_vector 	event_filter_;

		int	            proc_call_depth_;	// proc_entryの再帰呼び出し階数
		
		// スレッドローカルなメッセージハブのデータ
		static scoped<thread_local_stroage> tl_hub;
	};


}} /* end of namespace oden.gammo */


#endif

