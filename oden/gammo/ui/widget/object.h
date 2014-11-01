#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_OBJECT
#define ODEN_GAMMO_UI_WIDGETS_OBJECT

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------	
#include <bitset>
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
#include <oden/object/container.h>
// --------- gammo ----------

//
//####################################################


namespace oden { namespace gammo
{
	class window;
	class window_event;
	class preprocessing_event;
	class move_widget_context;

//_____________________________________________________________________________________________________________
//
//		インターフェース
//_____________________________________________________________________________________________________________
//

// ウィジェット・ポインタ
//
	template< typename T >
	class wid_ptr
	{
	public:
		typedef T  value_type;
		typedef T  element_type;
		typedef T* pointer;
		typedef T& reference;

		explicit wid_ptr( T* p = 0 )
			: p_( p )
		{}

	// メンバ関数
 	  // 変更
		void reset( pointer p ) { p_ = p; }
		pointer get()const { return p_; }

	  // 演算子
		pointer operator->()const { return p_; }
		reference operator*()const { return *p_; }

		operator pointer()const { return p_; }

	private:
		pointer p_;
	};

// 状態フラグ
//
	enum widget_flags
	{
		widget_ready,

		widget_no_update_after_init,
		widget_invisible,

		widget_flags_max
	};

//#################################################
//  ウィジェット・インターフェース
//
	class widget_object
		  : private boost::noncopyable
	{
	public:
		typedef object_container< widget_object > 	widget_container;
		typedef widget_container::element			widget_vector_elem;
		typedef std::vector< widget_vector_elem >	widget_vector;
		
		typedef widget_vector::iterator				widget_iterator;
		typedef widget_vector::const_iterator		widget_const_iterator;
		
	// コンストラクタ
		widget_object()
			: parent_(0), id_(0)
		{}
		virtual ~widget_object(){}
	
	// ウィンドウ
	  // そのものにキャスト
		virtual window& to_window() = 0;
		virtual const window& to_window() const = 0;
	  // ハンドル
		window_handle get_window_handle() const;
		void set_window_handle( window_handle wh );
		
	// イベント処理
	  // 全体の流れ
		virtual void handle_event( window_event& ) = 0;
	  // レイアウト関連のイベントに対応
		virtual void handle_layout_event( window_event& ev ) = 0;
	  // ユーザー定義のイベントハンドラを起動 
		virtual void my_handle_event( window_event& ev ) = 0;
	  // 残りものに対応
		virtual void final_handle_event( window_event& ) = 0;
	  // ここで自由な処理を定義
		virtual bool preprocess_event( preprocessing_event& ) = 0;
	  // イベントシステムに自信を追加
		virtual bool attach_event_loop() = 0;
	  // イベントシステムから外れる
		virtual bool detach_event_loop() = 0;

	// レイアウト関連処理
	  // 指定位置へ移動
		virtual bool move_widget ( move_widget_context& c ) = 0;
	  // 位置を指定する
		virtual void set_widget_location ( const rect& r ) = 0;
		virtual const rect& widget_location ()const = 0;
	  // 最適サイズ
		virtual void get_best_size		( size& out )const = 0;
		virtual void get_best_size_now	( size& out )const = 0;
	  // 最適サイズになるよう位置指定
		virtual void fit_widget() = 0;
	  // フォントの変更などで最適サイズが変わった
		virtual void best_size_changed() = 0;

	// 作成フロー
	  // 初期化
		bool start();
	  // 破棄
		bool destroy();

	private:
	  // 初期化ステップ
		// ユーザー定義の初期化(作成前)
		virtual void describe(){}
		// 作成前処理のまとめ
		void precreate();

		// コントロール作成方法
		virtual bool create_control(){ return true; }
		// 初期化
		virtual bool init_widget();
		// ユーザー定義の初期化(作成後)
		virtual bool initialize(){ return true; }
		// 作成処理のまとめ
		bool docreate();


	/* widget_object 固有関数 */
	public:
	// フラグ操作
	  // 開始処理を行ったか?
		bool is_ready()const { return flags_[widget_ready]; }

	  // 作成後、updateを呼ばない
		void no_update_after_init()
		{ flags_.set( widget_no_update_after_init ); }

	  // フラグを操作
		bool test_widget_flag ( widget_flags f )const { return flags_[f]; }
		bool set_widget_flag  ( widget_flags f, bool b = true ) { flags_.set( f, b ); }

	// 親子
	  // 親
		widget_object* get_parent_widget()const { return parent_; }
		void set_parent_widget( widget_object* p ) 
		{ 
			parent_ = p; 
			id_ = p->count_child_widget();
		}

	  // 子を追加
		template< typename Wid >
		Wid* add_child( Wid* pNew ) 
		{
			if( pNew )
			{
				widget_container::push_back( children_, pNew );
				pNew->set_parent_widget( this );
			}
			return pNew;
		}
		template< typename Wid >
		Wid* add_new_child()
		{
			return this->add_child( new Wid() );
		}

		template< typename Wid >
		Wid* new_child( wid_ptr<Wid>& w )
		{
			w.reset( this->add_new_child<Wid>() );
			return w.get();
		}
		
	  // 調べる
		bool has_parent_widget()const { return get_parent_widget() != 0; }
		bool has_child_widget()const  { return count_child_widget() != 0; }
		int count_child_widget()const { return children_.size(); }
		
	  // イテレータ取得
		widget_iterator begin() { return children_.begin(); }
		widget_const_iterator begin()const { return children_.begin(); }
		
		widget_iterator end() { return children_.end(); }
		widget_const_iterator end()const { return children_.end(); }
		
	  // 子ID
		void widget_child_id( int ID ){ id_ = ID; }
		int get_widget_child_id()const { return id_; }
		
	private:
		widget_object* parent_;
		widget_vector  children_;
		int			   id_;
		std::bitset<widget_flags_max> flags_;
	};

}} /* end of namespace oden.gammo */


#endif

