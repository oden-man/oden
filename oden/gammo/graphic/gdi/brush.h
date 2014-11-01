#pragma once

#ifndef ODEN_GOB_GRA_GDI_BRUSH
#define ODEN_GOB_GRA_GDI_BRUSH

//#####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
// --------- gammo -----------
#include <oden/gammo/graphic/gdi/object.h>

//
//
//#################################################



namespace oden { namespace gammo
{
	class rgb8;
//_______________________________________________________________________________________
//
//   GDIオブジェクト・ブラシ
//_______________________________________________________________________________________
//  	
//##############################################
// 情報構造体
//
	ODEN_STRUCT( log_brush, LOGBRUSH )
	{
		struct get
		{};

		struct set
		{
		  // スタイル
			Me& style( UINT sty ) {
				self::val(this).lbStyle = sty;
				return *this;
			}
		  // 色
			Me& color( colorref c ) {
				self::val(this).lbColor = c.value();
				return *this;
			}
		  // ハッチパターン
			Me& hatch_pattern( int hat ) {
				self::val(this).lbHatch = hat;
				return *this;			
			}
		};
 	};
	
//##############################################
// インターフェース
//
  // ブラシクラス
	class brush
		: public gdi_obj<brush_handle>
	{
	public:	
	// コンストラクタ
		brush( brush_handle bh = boost::none )
			: gdi_obj<brush_handle>( bh )
		{}
/*
	// 演算子
		operator brush_handle()const {
			return handle();
		}
	// メンバ関数
	  // ハンドル
		brush_handle::value_type handle_value()const{ return reinterpret_cast<HBRUSH>( base_handle_value() ); }
		brush_handle handle()const{ return handle_value(); }
*/
	};

//##############################################
// 構築関数
//
  // 単色のブラシ
	brush_handle create_solid_brush( colorref color );
	
  // システムのブラシを取得・削除しちゃだめ!
	brush_handle get_system_brush( DWORD syscolor );
	
  // システムに備えられている、単色のブラシ
	brush_handle get_stock_brush( int id );
			
  // 模様パターンを描画するブラシ
  	brush_handle create_hatch_brush( int hat, colorref line_color );
  	
  // グラデーションを用いる？ 方法はファンクタで指定？
  
  // ビットマップパターンで描画
	brush_handle create_pattern_brush( bitmap_handle bmh );
  	
  // Indirect
	brush_handle create_brush( const log_brush::value_type& lb );

}}

#endif