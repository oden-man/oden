#pragma once

#ifndef ODEN_GAMMO_UI_CTL_STABAR
#define ODEN_GAMMO_UI_CTL_STABAR

//#######################################################
//  インクルード
//

//-------- defs -------------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden -----------
//----------- gammo ---------
#include <oden/gammo/ui/control/general.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            ステータスバー コントロール クラス
//
//________________________________________________________________________________
//	
//################################################################
//  コントロールクラス
//
	class status_bar
		: public window
	{
	private:
		typedef status_bar self;
	public:	
	// ボーダー長さを記録するクラス
		class borders {
		public:
			borders(){ memset(vals_, 0, 3); }

			int horizontal()const{ return vals_[0]; }
			int vertical()  const{ return vals_[1]; }
			int between()   const{ return vals_[2]; }
			int* get_ptr(){ return vals_; }
		private:
			int vals_[3];
		};
	
	// コンストラクタ
		status_bar( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数
	  // パーツテキスト
		BYTE get_part_text( index at, string& out )const;
		string part_text( index at )const;		
		bool set_part_text( index at, string_ptr str, int flags = 0 )const;
		
	  // パーツテキストの長さ、スタイルを取得
		int part_text_length( index at )const;
		std::pair<int,int> part_text_info( index at )const;	// first : length, second : style SBT_ 

	  // パーツをオーナードローする
		bool set_owner_draw_part( index at, LPARAM data = 0, int flags = 0 )const;

      // パーツのツールチップのテキスト
		void get_part_tip_text( index at, string& out )const;
		string part_tip_text( index at )const;
		void set_part_tip_text( index at, string_ptr str )const;
		
	  // パーツアイコン
		icon_handle get_part_icon( index at )const;
		bool set_part_icon( index at, icon_handle ih )const;	// : none 代入で除去できる
		
	  // パーツを区分けする
		bool set_part_layout( memory_range<int> sizes )const;
		void get_part_layout( std::vector<int> &out )const;
		int count_part()const; // パーツの数

	  // パーツの大きさ
		bool get_part_rect( index at, rect& out )const;
		rect part_rect( index at )const;

	  // シンプルモード
		void set_simple_mode( bool b = true )const;
		bool is_simple_mode()const;
	  
	  // 背景色
		colorref set_back_color( colorref col )const;

	  // レイアウト
		void set_min_height( int h )const;
		bool get_border_width( status_bar::borders& out )const;
		status_bar::borders border_width()const;
		
	  // Unicode
		bool use_unicode_format( bool b = true )const;
		bool is_unicode_format()const;

      // class id
		static window_class_id get_control_class() { return window_class_id(STATUSCLASSNAME); }
	};
	
}}
#endif
