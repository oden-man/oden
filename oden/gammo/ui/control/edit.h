#pragma once

#ifndef ODEN_GAMMO_UI_CTL_EDIT
#define ODEN_GAMMO_UI_CTL_EDIT

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden ----------
//----------- gammo ----------
#include <oden/gammo/ui/control/general.h>

//
//#################################################

namespace oden { namespace gammo
{
	class edit;

//________________________________________________________________________________
//
//
//            エディット コントロール クラス
//
//________________________________________________________________________________
//	
//################################################################
//  バルーンチップの引数
//	
#if (_WIN32_WINNT >= 0x0501) /* XP later */

	ODEN_STRUCT( edit_balloon_tip, EDITBALLOONTIP )
  	{
		struct get
  		{};
		
		struct set
  		{
		  // タイトル
			Me& title( wide_string_ptr str ) {		
				self::val(this).pszTitle = str.c_str();
				return self::me(this);
			}
		  // 本文
			Me& text( wide_string_ptr str ) {
				self::val(this).pszText = str.c_str();
				return self::me(this);
			}
		  // アイコン
	  		Me& icon( INT type ) {
				self::val(this).ttiIcon = type;
				return self::me(this);
			}
		};
		
		struct def
		{ 
			def( value_type& out )
			{
				out.cbStruct = self::value_size();
			}
		};
	};
	
#endif
	
//################################################################
//  コントロールクラス
//
	class edit
		: public window
	{
	private:
		typedef edit self;
	public:	
	// コンストラクタ
		edit( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数
	  // テキスト操作
	    // テキストを末尾に追加
		void append( string_ptr text )const;
	    // テキストを挿入
		void insert( index at, string_ptr text )const;
	    // テキストを設定
		void set( string_ptr text )const;
		//void send( string_ptr text )const;
		// 取得
		void get( string& out )const { get_text(out); }
		// 置換
		void replace( index first, index last, string_ptr text )const;
	    // 選択された部分を置換
		void replace_selected( string_ptr text )const;
	    // 空かどうか
		bool empty( )const;
	    // テキストを完全に削除
		void erase_all( )const;
		// ソフト改行文字の扱い
		void insert_soft_line_break( )const;
		void remove_soft_line_break( )const;

	  // ライン
		class line {
		public:
		// メンバ関数
			// テキストの取得
			string text()const;
			bool get_text( string& out )const;
			// 長さを取得
			int length()const;
			// 行の全体/一部を置換する
			void set( string_ptr str )const;
			void replace( index b, index e, string_ptr str )const;
			// 行全体を削除する
			void erase_all()const;
			// 行を選択
			void select_all()const;
			void select( index b, index e )const;
			void select( index_range r )const { this->select( r.begin(), r.end() ); }
			// 行の先頭オフセット
			index begin_pos()const;
			index tail_pos()const;
			index end_pos()const;
			// 行のオフセット
			index_range pos()const;
			// 取得
			index get_index()const { return i_; }
			edit owner()const;
			bool fail()const { return owner_.is_null() || i_.is_null(); }
			
		// コンストラクタ
			line( window_handle ow, index i );
		private:
			window_handle owner_; index i_;
		};
		
	  // 選択位置・キャレット位置	  
	    // 指定部分を選択
		void select( index first, index last )const; 
		void select( index_range rng )const { this->select( rng.begin(), rng.end() ); }
		void select_all( )const;
		void deselect()const;
	    // 指定部分を取得
		index_range selection()const;
		// カーソル位置を指定
		void set_cursor_pos( index ichar )const;
		void set_cursor_home()const;
		void set_cursor_end()const;
		// キャレット位置を取得
		index cursor_pos()const;
		// 位置
		index home_pos()const;
		index tail_pos()const;
		index end_pos()const;
		//  座標からオフセットを取得( 16bitまで )
		index char_from_point( const point& )const;	// 文字
		edit::line line_from_point( const point& )const;	// ライン
		std::pair<index, index> offset_from_point( const point& )const;  // 両方
		// オフセットから座標を得る
		boost::optional< point > point_from_char( index ichar )const;
		
	  // 複数行の操作
	    // 行の取得
		edit::line line_at( index iline )const;
	    // 行テキストの取得
		string line_text( index iline )const;
	  	bool get_line_text( index iline, string& out )const;
		// 現在カーソルのある行を取得
		edit::line cursor_line()const;
		// 文字オフセットから行を取得
		edit::line line_from_char( index ichar )const;
		// 文字オフセットから含まれる行の長さを取得
		int line_length_from_char( index ichar )const;
	    // 行数の取得
	  	int count_line( )const;
		// 最初の目に見える文字を含む行を取得
		edit::line first_visible_line( )const;
		index first_visible_char( )const;
		
	  // 文字の制限
	    // リードオンリーの切り替え
		bool read_only( bool on )const;
	    // パスワードマスク文字の設定
	  	void use_password_mask( char_t ch )const;
		boost::optional< char_t > password_mask_char( )const;
	    // 最大文字数( バイト数 )の制限
	  	void set_text_limit( size_t num )const;
		size_t text_limit( )const;
		
	  // レイアウト
		// マージンを取得
		int left_margin_width( )const;
		int right_margin_width( )const;
		std::pair<int,int> margin_width( )const;
		// テキスト枠四角形
		rect formatting_rect( )const;
		void set_formatting_rect( const rect&, bool repaint=true )const;
		
	  // コントロールの操作
	    // ユーザーによる変更のフラグ
		bool is_modified( )const;
		void set_modify_flag( bool bflag )const;
		// スクロール
		bool scroll( int vert, int horiz = 0 )const;
		uinteger scroll_one( int flags )const;
		void scroll_to_caret()const;
	    // アンドゥ・アンドゥ可能な削除
	  	bool undo( )const;
		bool can_undo( )const;
	  	void clear_selected( )const;
		void clear( )const;
	    // クリップボード関連
	  	void copy_to_clipboard( )const;
  		void cut_to_clipboard( )const;
  		void paste_from_clipboard( )const;
		
	  // XP later
	#if (_WIN32_WINNT >= 0x0501)
		// キューバナーをセット・取得 ( multilineでは失敗する)
		bool get_cue_banner( wide_string& out )const;
		wide_string cue_banner( )const;
		bool set_cue_banner( wide_string_ptr text )const;
		bool set_cue_banner_focused( wide_string_ptr text )const;
		// バルーンチップ
		bool hide_balloon_tip( )const;
		bool show_balloon_tip( edit_balloon_tip::value_type& )const;		
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_EDIT); }
	};

//#############################################################################
//  イベント
//
  // edit_color
	GAMMO_DEFINE_EVENT( event_def, edit_color, WM_CTLCOLOREDIT, 2 )
	
	template<>
	struct event_arg<event::edit_color> :
		event::result_< event::value_object<brush_handle> >, 
		event::wparam_lparam< event::value_object<dc_handle>, event::value_object<window_handle> > 
	{};
	
	template<>
	struct event_branch<event::edit_color>
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& ctl )
		{
			window w = get_event_arg<event::edit_color, event::_2>( ev );
			return branch(ev) && branch_child_id( w.child_id(), ctl );
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_CTLCOLOREDIT;
		}
	};
	
	template<> 
	struct event_alias<event::color, edit>  { typedef event::edit_color type; };
	
  // その他の通知
	GAMMO_DEFINE_NOTIFY_EVENT( command_def, edit_change, EN_CHANGE )
	GAMMO_DEFINE_NOTIFY_EVENT( command_def, edit_update, EN_UPDATE )

  // エイリアス解決
	template<> 
	struct event_alias<event::change, edit> { typedef event::edit_change type; };

	
}}
#endif
