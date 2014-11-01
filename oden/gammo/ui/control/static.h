#pragma once

#ifndef ODEN_GAMMO_UI_CTL_STATIC
#define ODEN_GAMMO_UI_CTL_STATIC

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
//            スタティック コントロール クラス
//
//________________________________________________________________________________
//	
//################################################################
//  コントロールクラス
//
	class static_ctl
		: public window
	{
	private:
		typedef static_ctl self;
	public:	
	// コンストラクタ
		static_ctl( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数
	  // 設定
		icon_handle 		set_icon		( icon_handle ih )const;
		bitmap_handle 		set_bitmap		( bitmap_handle bh )const;
		cursor_handle 		set_cursor		( cursor_handle ch )const;
		enhmetafile_handle 	set_enhmetafile	( enhmetafile_handle emfh )const;
	  // 取得
	    icon_handle 		icon			()const;
	    bitmap_handle 		bitmap			()const;
	    cursor_handle 		cursor			()const;
	    enhmetafile_handle 	enhmetafile		()const;

      // class id
		static window_class_id get_control_class() { return window_class_id(WC_STATIC); }
	};	
	
}}
#endif
