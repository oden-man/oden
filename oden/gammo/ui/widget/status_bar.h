#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_STATUSBAR
#define ODEN_GAMMO_UI_WIDGETS_STATUSBAR

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------	
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
// --------- gammo ---------
#include <oden/gammo/ui/control/status_bar.h>
#include <oden/gammo/ui/widget/window.h>

//
//####################################################


namespace oden { namespace gammo
{
//______________________________________________________________________________________________________
//
//          ステータスバー・ウィジェット・パーツ
//______________________________________________________________________________________________________
//
namespace widget_layout
{
//######################################################
// 　ステータスバーの機能をつかったレイアウト
//
	template< typename Base >
	class status_bar
		: public Base
	{
	public:
	// コンストラクタ
		status_bar(){}
		~status_bar(){}

	// メンバ関数
	  // パーツのサイズを格納
		class parts {
		public:
		  // 幅を指定
			parts& fix( int w ) 
			{ data_.push_back(w); return *this; }
		  // 無制限に伸びる
			parts& expand() 
			{ return fix(-1); }
		  // クリア
			void clear()
			{ data_.clear(); }
		  // パーツをゲット
			const std::vector<int>& get()const { return data_; }
		private:
			std::vector<int> data_;
		};	

	  // レイアウトクラスを返す
		parts& describe_parts_layout()
		{ parts_.clear(); return parts_; }

	// 共通関数
	  // このウィジェットを動かす
		bool move_widget();

	private:
		typename status_bar<Base>::parts parts_;
	};

	template< typename B >
	bool status_bar<B>::move_widget()
	{
		if( on_start() ) /* 初期化中のみ */
		{
			set_part_layout( parts_.get() );  // パーツを分割
		}
		else if( has_parent_widget() )
		{
			rect rc;
			client_rect(rc);
			send_message( WM_SIZE, rc.width(), rc.height() );
			return true;
		}
		return false;
	}
/*
//######################################################
// 　自前でパーツサイズを再計算するレイアウト
//
	class status_bar_layout_ex
	{
	public:
	// コンストラクタ
		status_bar_layout_ex(){}
		virtual ~status_bar_layout_ex(){}

	// レイアウトパーツクラス
		class part_term
			: public layout_terminal, private boost::noncopyable
		{
		public:
		// コンストラクタ
			part_term( status_bar_layout_ex* pinst, int at )
				: pinst_(pinst), at_(at)
			{}
			~part_term(){}
			
		// オーバーライド関数		
		  // layout_terminal - オーバーライド
			void register_location( const rect& lo ) 
			{ 
				pinst_->parts_[at_] = lo.width(); 
			}
		
		private:
			status_bar_layout_ex* pinst_;
			int at_;
		};

	// メンバ関数	
	  // パーツ
		part_term* bar_part()
		{
			parts_.push_back(0);
			return new part_term( this, parts_.size()-1 );
		}
	  // パーツサイズを再計算
		void recalc_parts( widget_object* wid )
		{
			if( layout_tree_ )
			{
				rect rc;
				wid->get_window().client_rect(rc);

				layout_tree_->recalc(rc);
			}

			status_bar bar = wid->get_window_handle();
			bar.set_part_layout( parts_ );
		}
	  // レイアウトをセット
		void set_layout( layout_object_pointer pobj )
		{ layout_tree_.reset( pobj.get() ); }
		
	// このウィジェットを動かす
		void move_wid( widget_object* wid )
		{
			if( wid->has_parent() )
			{
				rect rc;
				wid->get_parent()->get_window().client_rect(rc);

				wid->get_window().send_message( WM_SIZE, rc.width(), rc.height() );

				recalc_parts( wid );
			}	
		}

	// イベントハンドラ
	  // WM_CREATE : ステータスバーを区分けする
		void layout_on_start( widget_object* wid )
		{ recalc_parts( wid ); }

	  // ダミー
		void layout_on_size( widget_object* ) {}
		void layout_on_sizing( widget_object*, const window_event& ) {}
		
	private:
		boost::scoped_ptr<layout_object> layout_tree_;	
		std::vector<int> parts_;		
	};
	*/
}

//______________________________________________________________________________________________________
//
//          ウィジェットパーツ・プリセット
//______________________________________________________________________________________________________
//
  // コントロール
	typedef new_window< widget< status_bar >, control_window_class<status_bar> > 
		status_bar_widget;
		
	typedef widget_layout::status_bar< status_bar_widget > 
		status_bar_layout_widget;


}} /* end of namespace oden.gammo */

#endif

