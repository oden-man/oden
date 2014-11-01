#pragma once

#ifndef ODEN_GAMMO_UI_DLG_DLG
#define ODEN_GAMMO_UI_DLG_DLG

//###############################################
//
//
// --------- def -----------
#include <oden/gammo/defs.h>
// --------- win -----------
// --------- oden ----------
#include <oden/gammo/ui/window/window.h>

//
//
//#################################################


namespace oden { namespace gammo
{	
//##############################################################
// ダイアログボックスのクラス
//
	class dialog
		: public window
	{
	private:
		typedef dialog self;
	public:
	// コンストラクタ
		dialog( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数
	  // ダイアログを作成
		bool create( res_id dlg_template, window_handle parent, LPARAM init_param = 0, instance_handle ih = boost::none );
	  
	  // モーダルダイアログを開始
	  	dlg_result do_modal( res_id dlg_template, window_handle parent, LPARAM init_param = 0, instance_handle ih = boost::none )const;
	  // モーダルを終了
	  	bool end_modal( dlg_result Ret )const;
	  	
	  // ダイアログむけのメッセージを処理する
	  	bool process_dlg_message( raw_window_message& out )const;
	  	
	  // アイテムを取得
	  	window_handle get_item( int i )const;
	  	window item( int i )const;
	  	
	  // アイテムの文字を取得
	  	bool get_item_text( int item, string &out )const;
	  	string item_text( int item )const;
	  	
	  // アイテムの文字を設定
	  	bool set_item_text( int item, string_ptr text )const;

	  // アイテムの文字を数値として取得
	  	UINT item_value( int item, bool s = false, bool* pbool = 0 )const;
		template< typename T >
	  	bool get_item_value( int i, bool s, T& out )const
		{
			bool ret = false;
			out = static_cast<T>( item_value( i, s, &ret ) );
			return ret;
		}

	  // アイテムに数値を文字にして設定
	  	bool set_item_value( int item, UINT val, bool s = false )const;
	  	
	  // グループアイテムを取得
	  	window_handle prior_group_item( window_handle begin = boost::none )const;
	  	window_handle next_group_item( window_handle begin = boost::none )const;
	  	
	  // タブグループアイテムを取得
	  	window_handle prior_tab_item( window_handle begin )const;
	  	window_handle next_tab_item( window_handle begin )const;
	  	
	  // ダイアログ単位の座標
		static LONG base_unit();
	  	bool base_unit_to_screen( rect& out )const;

	  // WindowLong
		DLGPROC set_dialog_procedure_ptr( DLGPROC )const;
		LRESULT set_message_result( LRESULT )const;
		LONG_PTR set_dialog_data( LONG_PTR )const;

		DLGPROC dialog_procedure_ptr()const;
		LRESULT message_result()const;
		LONG_PTR dialog_data()const;
	};

//##############################################################
// ダイアログボックスのイベント
//
namespace event {

  /* WM_INITDIALOG */
	typedef event_def<event_code<WM_INITDIALOG> > dialog_init;

  /* その他 引数なし */

}

//
// 定義
//	
	template<> // focused ctl / lparam
	struct event_arg<event::dialog_init> : 
		event::result_< event::bool_<> >, event::wparam_lparam< event::value_object<window_handle>, event::value<LPARAM> >
	{};

}}

#endif