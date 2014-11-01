#pragma once

#ifndef ODEN_GAMMO_UI_CTL_TAB
#define ODEN_GAMMO_UI_CTL_TAB

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden ----------
#include <oden/konbu/callback.h>
//----------- gammo ----------
#include <oden/gammo/ui/control/general.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            タブコントロール クラス
//
//________________________________________________________________________________
//
//################################################################
//  コントロールクラス
//
	class tab
		: public window
	{
	private:
		typedef tab self;
	public:	
	// コンストラクタ
		tab( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_TABCONTROL); }

	  //
		// void calc_best_size( size& out )const;
	};

//################################################################
//  アイテム
//
	ODEN_STRUCT( tab_item, LVCOLUMN )
	{		
		struct set
		{
			Me& text( string_ptr str ) 
			{
				self::val(this).pszText = const_cast<LPTSTR>( str.c_str() ); // キャストしてOK
				self::val(this).mask |= TCIF_TEXT;
				return self::me(this);
			}
			Me& text_buf( string& out ) // バッファの設定
			{	
				self::val(this).cchTextMax = out.size();
				return text(out);
			}
			Me& image_index( index i ) 
			{
				self::val(this).iImage = i.value();
				self::val(this).mask |= TCIF_IMAGE;
				return self::me(this);	
			}
			Me& param( LPARAM lparam ) 
			{
				self::val(this).lParam = lparam;
				self::val(this).mask |= TCIF_PARAM;
				return self::me(this);	
			}
		};		
	};
	
//#############################################################
//  ヒットテスト
//
	ODEN_STRUCT( tab_hittest_info, TCHITTESTINFO )
	{
		struct get 
		{
			point position()const	{ return self::val(this).pt; }
			UINT  state()const		{ return self::val(this).flags; }
		};
		
		struct set 
		{
			Me& position( const point& pt ) 
			{
				get_point( pt, self::val(this).pt );
				return self::me(this);
			}
		};
	};	

//#########################################################
//  イベント
//
  // アイテム挿入
	GAMMO_DEFINE_EVENT( notify_def, tab_insert_item, TCM_INSERTITEM, 2 )
	
	template<>
	struct event_arg<event::tab_insert_item> :
		event::result_< event::value_object<index> >,
		event::wparam_lparam< event::value_object<index>, event::property_cref<tab_item> >
	{};

  // アイテム数
	GAMMO_DEFINE_EVENT( notify_def, tab_count_item, TCM_GETITEMCOUNT, 0 )
	
	template<>
	struct event_arg<event::tab_count_item> :
		event::result_< event::value<size_t> >,
		event::noparam
	{};

  // アイテム取得
	GAMMO_DEFINE_EVENT( notify_def, tab_get_item, TCM_GETITEM, 2 )
	
	template<>
	struct event_arg<event::tab_get_item> :
		event::result_< event::bool_<> >,
		event::wparam_lparam< event::value_object<index>, event::property_ref<tab_item> >
	{};
	
}}

#endif
