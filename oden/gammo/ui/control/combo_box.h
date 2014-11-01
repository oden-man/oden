#pragma once

#ifndef ODEN_GAMMO_UI_CTL_COMBOBOX
#define ODEN_GAMMO_UI_CTL_COMBOBOX

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
//________________________________________________________________________________
//
//           コンボボックスコントロールクラス
//________________________________________________________________________________
//
//################################################################
//  コントロール
//
	class combo_box_base
		: public window
	{
	public:	
	// コンストラクタ
		combo_box_base( const window_handle& wh )
			: window( wh )
		{}
		
	// メンバ関数
	    // テキストを削除
		bool erase( index at )const;
		// アイテム関連データ
		LPARAM item_data( index i )const;
		bool set_item_data( index i, LPARAM data )const;
		// アイテム数
		int count()const;
	    // 全てクリア
		void clear()const;
	    // 空かどうか
		bool empty()const{ return count()==0; }
		
	    // 指定部分を選択
		bool select( index i )const;
		void deselect()const;	    
		// 選択場所を取得
	  	index selection()const;

	    // 最大文字数の制限を設ける
	  	void set_edit_limit( int num )const;
		// メモリ確保を予め行う・確保したアイテム数を返す
		integer reserve( int add_item_num, int string_memory_size )const;

	  	// リストアイテム高さ
	  	integer item_height()const;
	  	integer item_height( index i )const;		// ownerdraw variable のみ
	  	bool set_item_height( int h )const;
	  	bool set_item_height( index i, int h )const;	// ownerdraw variable のみ
		// selection field の高さ
	  	integer selection_height()const;
	  	bool set_selection_height( int h )const;

		// ドロップダウン
		void show_drop_down(bool b = true)const;
		// ドロップリストが見えているか
		bool drop_downed()const;
		
	  // XP later
	#if (_WIN32_WINNT >= 0x0501)		
		// リストのスクロールなしで表示するアイテムの最大数を取得・セット
		INT visible_item_limit()const;
		bool set_visible_item_limit(INT)const;
	#endif
	
	  // Vista later
	#if (_WIN32_WINNT >= 0x0600)
		// エディットコントロールに出る薄い文字列を取得・セット
		bool get_cue_banner(wide_string&)const;
		wide_string cue_banner()const;
		bool set_cue_banner(wide_string_ptr)const;
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_COMBOBOX); }
	};	

//################################################################
//  通常のコンボボックス ( 文字列のアイテム)
//
	class combo_box
		: public combo_box_base
	{
	public:	
	// コンストラクタ
		combo_box( window_handle wh = boost::none )
			: combo_box_base( wh )
		{}
		
	// メンバ関数
	  // リストアイテムの操作
	    // テキストを末尾に追加
		void add( string_ptr text )const;
		// アイテムを挿入
		index insert( index at, string_ptr text )const;
		// テキストを検索
		index find_exact( string_ptr text, index start = boost::none )const;
		index find( string_ptr text, index start = boost::none )const;
		// アイテムを取得
		bool get_item_text( index i, string& out )const;
		string item_text( index i )const;
		// テキスト長さを取得
		size_t item_text_length( index i )const;
		// 検索して選択
		index find_and_select( string_ptr text, index start = boost::none )const;
	};

//################################################################
//  オーナードローで、文字列を持たないコンボボックス
//
	class data_combo_box
		: public combo_box_base
	{
	public:	
	// コンストラクタ
		data_combo_box( window_handle wh = boost::none )
			: combo_box_base( wh )
		{}
		
	// メンバ関数
	  // リストアイテムの操作
	    // テキストを末尾に追加
		void add( LPARAM data )const;
		// アイテムを挿入
		index insert( index at, LPARAM data )const;
		// テキストを検索
		index find( LPARAM data, index start = boost::none )const;
		// 検索して選択
		index find_and_select( LPARAM data, index start = boost::none )const;
	};

//################################################################
//  コンボボックスのイベント
//
namespace event
{
	typedef command_def< event_code< CBN_SELCHANGE > >	combo_box_sel_change;
	typedef command_def< event_code< CBN_EDITCHANGE > >	combo_box_edit_change;
	typedef event_def_or< 
				combo_box_sel_change, 
				combo_box_edit_change > 		combo_box_text_change;

	typedef command_def< event_code< CBN_DROPDOWN > >	combo_box_drop_down;
	typedef command_def< event_code< CBN_CLOSEUP > >	combo_box_close_up;
}

  // エイリアス
	//template<> struct event_alias< event::drop_down, combo_box > { typedef event::combo_box_drop_down type; };
	//template<> struct event_alias< event::close_up, combo_box > { typedef event::combo_box_close_up type; };
	template<> struct event_alias< event::sel_change, combo_box > 	{ typedef event::combo_box_sel_change type; };
	template<> struct event_alias< event::change, combo_box > 		{ typedef event::combo_box_text_change type; };


}}
#endif
