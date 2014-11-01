#pragma once

#ifndef ODEN_GAMMO_UI_CTL_IMGLIST
#define ODEN_GAMMO_UI_CTL_IMGLIST

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
//           イメージリスト クラス
//
//________________________________________________________________________________
//
//################################################################
// 
//	
	ODEN_STRUCT( image_list_imginfo, IMAGEINFO )
	{
		struct get
		{
			bitmap_handle color()const { return self::val(this).hbmImage; }
			bitmap_handle mask()const { return self::val(this).hbmMask; }
			rect rectangle()const { return self::val(this).rcImage; }
		};
		
		struct set
		{};
	};
	
//################################################################
//  コントロールクラス
//
	class image_list
		: public handle_interface<image_list_handle>
	{
	private:
		typedef image_list self;
	public:	
	// コンストラクタ
		image_list( image_list_handle imgh = boost::none )
			: handle_interface<image_list_handle>( imgh )
		{}
		
	// デリータ
		typedef struct destroyer {
			void operator()( self& obj )const { obj.destroy(); }
		} deleter_type;
		
	// メンバ関数
	  // ハンドル
		// 作成する
 		bool create( const size& size, UINT flags, int initial = 16, int grow = 0 );
		// 削除する
		bool destroy( );
		// 複製する
		image_list_handle duplicate()const;
		
	  // イメージ
		// ひとつ追加
		index add( bitmap_handle img, bitmap_handle mask = boost::none )const;
		index add( icon_handle ico )const;
		// イメージを置き換える
		index replace( index at, bitmap_handle img, bitmap_handle mask = boost::none )const;
		index replace( index at, icon_handle ico )const;
		// 削除
		bool erase( index at )const;
		// すべて削除する
		bool clear()const;
		// くっつける
		image_list_handle merged( image_list_handle l1, index at1, image_list_handle l2, index at2, const point& offset )const;
		// 描画
		bool draw( 
			index at, 
			dc_handle dest, 
			const point& pt, 
			UINT style = ILD_NORMAL 
		  )const;
		bool draw(
			index at, 
			dc_handle dest, 
			const rect& rc, 
			colorref back = CLR_NONE, 
			colorref fore = CLR_NONE, 
			UINT style = ILD_NORMAL 
		  )const;
		//  イメージ情報を取得
		bool get_image_info( index at, image_list_imginfo::value_type& r )const;
		// イメージからアイコンを新たに作成( destroy すべし )
		icon_handle create_icon( index at, UINT flags )const;
		
		// イメージサイズを取得
		size dimension()const;		  
		bool set_sizesion( const size& dm )const;
		// イメージの数を返す/水増しする(後でreplaceで追加すべし)
		size_t count()const;
		bool set_count( UINT cnt )const;

	  // ドラッグ・ドロップ
		// 現在ドラッグに使われているイメージリストを取得
		static image_list_handle current_drag_image( point* at = 0, point* hotspot = 0 );
		// ドラッグを開始
		bool begin_drag( index at, const point& hotspot = point(0,0) )const;
		// ドラッグ状態に入る
		bool drag_enter( window_handle wh, const point &drag_pos )const;
		// ドラッグイメージを動かす
		bool drag_move( const point &drag_pos )const;
		// ドラッグ状態から抜ける
		bool drag_leave( window_handle wh )const;
		// ドラッグイメージを隠す/出す
		bool hide_drag_image( bool hide = true )const;
		// ドラッグを終える
		static void end_drag_image();

	};
	
	
}}

#endif
