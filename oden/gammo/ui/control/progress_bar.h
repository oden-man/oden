#pragma once

#ifndef ODEN_GAMMO_UI_CTL_PROGBAR
#define ODEN_GAMMO_UI_CTL_PROGBAR

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
//            プログレスバー コントロール クラス
//
//________________________________________________________________________________
//	
//################################################################
//  コントロールクラス
//
	class progress_bar
		: public window
	{
	private:
		typedef progress_bar self;
	public:	
	// コンストラクタ
		progress_bar( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数	  
      // バーの頭の位置
		// 位置を指定する
		int set_progress( int p )const;
		int progression()const;
		// 指定した増分だけ進める
		int advance()const;			// PBM_STEPIT
		int advance( int v )const;	// PBM_DELTAPOS
		int set_advance_step( int v )const;
		// 一杯にする・空にする
		void fill()const;
		void clear()const;

      // プログレスバーの範囲
		void set_range( int min, int max )const;
		void set_range_low( int min )const;
		void set_range_high( int max )const;
		std::pair<int,int> range()const;
		int range_low()const;
		int range_high()const;

	  // スタイル
		// バーの色
		colorref set_bar_color( colorref c )const;
		colorref bar_color()const;
		// 背景色
		colorref set_back_color( colorref c )const;
		colorref back_color()const;
		// 状態を設定する（見た目が変わる）
		void set_progress_state( int v )const;	// PBM_SETSTATE
		int progress_state()const;			// PBM_GETSTATE
		// マーキーモードに移る
		void start_marquee( int speed )const;
		void end_marquee()const;

      // class id
		static window_class_id get_control_class() { return window_class_id(PROGRESS_CLASS); }
	};	
	
}}
#endif
