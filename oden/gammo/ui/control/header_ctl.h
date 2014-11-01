#pragma once

#ifndef ODEN_GAMMO_UI_CTL_HEADER
#define ODEN_GAMMO_UI_CTL_HEADER

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
//
//            ヘッダーコントロール クラス
//
//________________________________________________________________________________
//	
//################################################################
//  アイテム
//
	ODEN_STRUCT( header_ctl_item, HDITEM )
	{
		struct get
		{
			
		};
		
		struct set
		{			
			Me& format( int fmt ) {
				self::val(this).fmt = fmt;
				self::val(this).mask|=HDI_FMT;
				return self::me(this);	
			}
			Me& size( int w ) {
				self::val(this).cxy=w;
				self::val(this).mask|=HDI_WIDTH;
				return self::me(this);	
			}
			Me& text( string& buf ) {
				self::val(this).pszText =  buf_begin<char_t>(buf); // 書き込み可能バッファが必要
				self::val(this).mask|=HDI_TEXT;
				return self::me(this);
			}
			Me& text_buf( string& out ) {	// バッファの設定
				self::val(this).cchTextMax = out.size();
				return text(out);
			}
			Me& param( LPARAM v ) {
				self::val(this).lParam = v;
				self::val(this).mask|=HDI_LPARAM;
				return self::me(this);	
			}
			Me& image_index( index i ) {
				self::val(this).iImage = i.value();
				self::val(this).mask|=HDI_IMAGE;
				return self::me(this);	
			}
			Me& order( int val ) {
				self::val(this).iOrder = val;
				self::val(this).mask|=HDI_ORDER;
				return self::me(this);	
			}
		#if (_WIN32_WINNT >= 0x0500)
		#endif
		#if (_WIN32_WINNT >= 0x0600)
		#endif	
		};
	};
	typedef header_ctl_item hd_item;


//#############################################################
//  ヒットテスト
//
	ODEN_STRUCT( header_ctl_hittest_info, HDHITTESTINFO )
	{
		struct get
		{
			point position()const	{ return self::val(this).pt; }
			index index()const		{ return self::val(this).iItem; }
			UINT  state()const		{ return self::val(this).flags; }
		};
		
		struct set
		{
			Me& position( const point& pt ) {
				pt.get_point( self::val(this).pt );
				return self::me(this);
			}
		};
	};
	typedef header_ctl_hittest_info hd_hittest_info;
	
//################################################################
//  コントロールクラス
//
	class header_ctl
		: public window
	{
	public:	
	// コンストラクタ
		header_ctl( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// メンバ関数		
	  //  アイテム操作
		// アイテムを一つ追加
		index insert( index iItem, const header_ctl_item::value_type& data )const;
		// アイテムを削除
		bool erase( index iItem )const;
		// アイテムデータを取得
		bool get_item( index iWhere, header_ctl_item::value_type& out )const;
		// アイテムデータを設定
		bool set( index iWhere, const header_ctl_item::value_type &data )const;
		// アイテム数を取得
		size_t count()const;
		// ヒットテスト
		index hit_test( header_ctl_hittest_info::value_type& out )const;
		
	  // イメージリスト
		// 設定(システム側で自動破棄されます。)
		bool set_image_list( image_list_handle imgh )const;
		image_list_handle image_list()const;
	
	  // その他
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_HEADER); }
	};
}}

#endif
