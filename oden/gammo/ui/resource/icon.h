#pragma once

#ifndef ODEN_GAMMO_UI_RES_ICON_CTL
#define ODEN_GAMMO_UI_RES_ICON_CTL

//#######################################################
//  インクルード
//
// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
// --------- win ------------

// --------- boost ----------
// --------- oden ----------
#include <oden/property.h>
//----------- gammo ----------
//#include <oden/gammo/ui/resource/resource.h>

//
//###############################################


//___________________________________________________
//
//  前方宣言
//___________________________________________________
//
namespace oden { namespace gammo
{		
//_______________________________________________________________________________________
//
//
//     アイコンクラス
//
//_______________________________________________________________________________________
//		
//########################################################
// アイコン情報クラス
//	
/*
    iconクラスは外部の画像などが読み込まれ新たに作られた後の姿を表しており、
    元データのサイズ・深度などは失われている。
    そのため、サイズ・深度などを使いたい場合はデータを読み込む際に、必要な情報を保存しておく。
*/
	ODEN_STRUCT( icon_info, ICONINFO )
	{
		struct get
		{
			bitmap_handle color_bitmap ()const { return self::val(this).hbmColor; }
			bitmap_handle mask_bitmap  ()const { return self::val(this).hbmMask; }
		};

		struct def
		{
			def( value_type& out )
			{
				out.fIcon = TRUE;
			}
		};
	};	

//########################################################
// アイコン拡張情報クラス
//
	
//########################################################
// アイコン操作クラス
//
	class icon
		: public handle_interface<icon_handle>
	{
	public:		
	// コンストラクタ	
		icon( const icon_handle& h = boost::none )
			: handle_interface<icon_handle>( h )
		{}
	
	// デリータの定義
		typedef struct destroyer {
			void operator()( icon& o ) { o.destroy(); }
		} deleter_type;
		
	// メンバ関数
	  // ビットマップを取得 ( ほぼDDB )
		bitmap_handle color_bitmap( )const;
		bitmap_handle mask_bitmap( )const;
	  // アイコンを破棄する
		bool destroy( );
	  // アイコンを複製する
		icon_handle duplicate( )const;
	  // 別のモジュールへアイコンをコピーする
	  	icon_handle copy( )const;
	  // アイコン情報の取得
		bool get_info( icon_info::value_type& out )const;
		//bool get_info( icon_info_ex& out )const;
	  // アイコンを描画する
		bool draw( 
			dc_handle Todc,
			const rect& place, 
			brush_handle offscreen_brush = boost::none,
			UINT draw_option = DI_NORMAL 
		)const;

	};
	
//__________________________________________________________________________________________________
//
//
//    構築関数 
// 
//__________________________________________________________________________________________________
//
//#######################################################
//  ファイル名・リソース名からロードする関数
//	
  // アイコンをファイルから読む
	icon_handle load_icon( 
		string_ptr path, 
		UINT opt = LR_DEFAULTCOLOR, 
		const size& desired_size = size( 0, 0 ) 
	);
	
  // アイコンをリソースから読む
  	icon_handle load_icon( 
		module_handle mod, 
		res_id	rid, 
		UINT opt = LR_DEFAULTCOLOR, 
		const size& desired_size = size( 0, 0 ) 
	);

//#######################################################
// リソースのアイコン
//
  // リソース(RT_ICON)のビットからアイコンを作成
	icon create_icon_resource (
		buffer_range<BYTE> bits,
		const size& desired_size,
		UINT opt = LR_DEFAULTCOLOR
	);
	
  // リソースから指定の大きさのアイコンを取得
	res_id lookup_icon_resource (
		buffer_range<BYTE> bits,
		const size& desired_size,
		UINT opt = LR_DEFAULTCOLOR
	);
	
  // リソース内のアイコン構造体
#	pragma pack( push )
#	pragma pack( 2 )

	struct MEMICONDIRENTRY
	{
		BYTE	bWidth;               // Width of the image
		BYTE	bHeight;              // Height of the image (times 2)
		BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
		BYTE	bReserved;            // Reserved
		WORD	wPlanes;              // Color Planes
		WORD	wBitCount;            // Bits per pixel
		DWORD	dwBytesInRes;         // how many bytes in this resource?
		WORD    nID;				  // resource id
	};
	
	struct MEMICONDIR
	{
		WORD			idReserved;   // Reserved
		WORD			idType;       // resource type (1 for icons)
		WORD			idCount;      // how many images?
		MEMICONDIRENTRY	idEntries[1]; // the entries for each image
	};

#	pragma pack( pop )
	
	ODEN_STRUCT( resource_icon_dir_entry, MEMICONDIRENTRY )
	{
		struct get
		{
			size  dimension() const { return oden::size( width(), height() ); }
			BYTE  width()  const { return self::val(this).bWidth; }
			BYTE  height() const { return self::val(this).bHeight; }
			BYTE  color()  const { return self::val(this).bColorCount; }
			WORD  depth()  const { return self::val(this).wBitCount; }
			DWORD size()   const { return self::val(this).dwBytesInRes; }
			WORD  id()     const { return self::val(this).nID; }
		};
	};
	
	ODEN_STRUCT( resource_icon_dir, MEMICONDIR )
	{
		struct get
		{
			bool is_icon() const { return self::val(this).idType == 1; }
			int count()    const { return self::val(this).idCount; }
			resource_icon_dir_entry::read_cref 
				entry( int i = 0 )const { return self::val(this).idEntries + i; }
		};
	};
	
//###########################################################
// システムのアイコン
//	
   // システムのサイズでロードする
  	icon_handle load_system_icon( DWORD icoid );
   // 任意のサイズでロード
  	icon_handle load_system_icon( DWORD icoid, UINT load_option, const size& desired_size = size( 0, 0 ) );
		
//###########################################################
// ビット
//	


}}

#endif
