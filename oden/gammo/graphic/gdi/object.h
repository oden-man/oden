#pragma once

#ifndef ODEN_GOB_GRA_GDI_OBJ
#define ODEN_GOB_GRA_GDI_OBJ

//#####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
#include <oden/property.h>
// --------- gammo -----------

//
//
//#################################################



namespace oden { namespace gammo
{
//_______________________________________________________________________________________
//
//    GDIオブジェクト
//_______________________________________________________________________________________
//		
//###############################################
//  gdi_object ベースクラス
//
	template< typename H = gdi_obj_handle >
	class gdi_obj
		: public handle_interface<H>
	{
	public:
	// コンストラクタ
		gdi_obj( H h = boost::none )
			: handle_interface<H>( h )
		{}
		
	// デリータ
		typedef struct destroyer {
			void operator()( gdi_obj& o )const { o.destroy(); }
		} deleter_type;
		
	// メンバ関数
	  // 共通型のハンドルを取得
		gdi_obj_handle::value_type base_handle_value()const { return this->handle_value(); }
		gdi_obj_handle base_handle()const { return this->base_handle_value(); }

	  // タイプを取得
		DWORD type()const {
			return ::GetObjectType( base_handle_value() ); 
		}
	  // データを取得（汎用型）
		template< typename Struct >
		bool get_info( Struct& r )const {
			return sizeof(Struct) == ::GetObject( base_handle_value(), sizeof(Struct), &r );
		}
	  // 破棄する
		bool destroy( )const {
			return FALSE!=::DeleteObject( base_handle_value() );
		}
	};
	
//##############################################################
//  gdi_objectクラス
//
	
//##############################################################
//  gdi_object エイリアス
//	
	
//##############################################################
//  gdi_object typedef
//
}}

#endif