#pragma once

#ifndef ODEN_GAMMO_UI_CTL_LISTBOX
#define ODEN_GAMMO_UI_CTL_LISTBOX

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
//           リストボックスコントロールクラス
//________________________________________________________________________________
//
//################################################################
//  コントロール
//
	class list_box_base
		: public window
	{
	public:	
	// コンストラクタ
		list_box_base( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数
	  // アイテムの操作（共通）
	    // テキストを削除
		bool erase( index at )const;
		// 関連データ
		LPARAM item_data( index i )const;
		bool set_item_data( index i, LPARAM data )const;
		// アイテム数
		int count()const;
	    // 全てクリア
		void clear()const;
	    // 空かどうか
		bool empty()const{ return count()==0; }
		// 一番上のアイテム
		index top()const;
		bool set_top( index at )const;
		// マルチカラムモードの場合、カラムあたりのアイテムの数を返す
		int count_column_item()const;
		
	  // アイテムを選択
	    // 一つを選択
		bool select( index i )const;
		void deselect()const;
		// 選択されているか？
		bool selected( index at )const;
		// 選択場所を取得
	  	index selection()const;
		// 選択数を取得
		int count_selection()const;
		// 複数の選択箇所を全て取得
		integer get_selections( buffer_range<index> out )const;
		std::vector<index> selections()const;
		// ある範囲を指定し選択
		bool select( index beg, index end )const;
		bool deselect( index beg, index end )const;
		// 複数選択の開始箇所
		index anchor()const;
		bool set_anchor( index at )const;

	  // レイアウト
	  	// アイテム高さ
	  	integer item_height()const;
	  	integer item_height( index i )const;		// ownerdraw variable のみ
	  	bool set_item_height( int h )const;
	  	bool set_item_height( index i, int h )const;	// ownerdraw variable のみ
		// selection field の高さ
	  	integer selection_height()const;
	  	bool set_selection_height( int h )const;	
		// アイテムの大きさ
		bool get_item_rect( index at, rect &out )const;
		rect item_rect( index at )const;

      // コントロールの操作
		// メモリ確保を予め行う・確保したアイテム数を返す
		integer reserve( int add_item_num, int string_memory_size )const;
		// アイテムのキャレット
		index focused()const;
		bool set_focus( index at )const;
		// カレントディレクトリのファイルを列挙して追加する
		bool add_file_list( string_ptr files, UINT attrs = DDL_READWRITE )const;


	  // XP later
	#if (_WIN32_WINNT >= 0x0501)
	#endif
	
	  // Vista later
	#if (_WIN32_WINNT >= 0x0600)
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_LISTBOX); }
	};	

//################################################################
//  通常のコンボボックス ( 文字列のアイテム)
//
	class list_box
		: public list_box_base
	{
	public:	
	// コンストラクタ
		list_box( window_handle wh = boost::none )
			: list_box_base( wh )
		{}
		
	// メンバ関数
	  // リストアイテムの操作
	    // テキストを追加
		index add( string_ptr text )const;
		index add( string_ptr text, LPARAM param )const;
		// アイテムを挿入
		index insert( index at, string_ptr text )const;
		index insert( index at, string_ptr text, LPARAM param )const;
		// アイテムを設定
		bool set( index at, string_ptr text )const;
		bool set( index at, string_ptr text, LPARAM param )const;
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
	class data_list_box
		: public list_box_base
	{
	public:	
	// コンストラクタ
		data_list_box( window_handle wh = boost::none )
			: list_box_base( wh )
		{}
		
	// メンバ関数
	  // リストアイテムの操作
	    // テキストを追加
		index add( LPARAM data )const;
		// アイテムを挿入
		index insert( index at, LPARAM data )const;
		// アイテムを設定
		bool set( index at, LPARAM data )const;
		// テキストを検索
		index find( LPARAM data, index start = boost::none )const;
		// 検索して選択
		index find_and_select( LPARAM data, index start = boost::none )const;

      // コントロール
		// アイテムの数を決めて設定する
		bool set_item_count( int num )const;
	};

//################################################################
//  イベント
//
namespace event
{
	typedef command_def< event_code< LBN_SELCHANGE > >	list_box_sel_change;
	typedef command_def< event_code< LBN_SELCANCEL > >	list_box_sel_cancel;
	typedef command_def< event_code< LBN_ERRSPACE > >	list_box_error_space;
	typedef command_def< event_code< LBN_DBLCLK > >		list_box_double_click;
}

  // エイリアス
	template<> struct event_alias< event::sel_change, list_box > 	{ typedef event::list_box_sel_change type; };
	template<> struct event_alias< event::double_click, list_box > { typedef event::list_box_double_click type; };


	
}}
#endif
