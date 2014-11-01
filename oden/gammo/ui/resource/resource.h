#pragma once

#ifndef ODEN_GOB_UI_RES_DEFS
#define ODEN_GOB_UI_RES_DEFS

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- std ------------
// --------- boost ----------
// --------- oden -----------
#include <oden/konbu/callback.h>

// --------- gammo ----------

//
//
//####################################################


namespace oden { namespace gammo
{	
	typedef konbu::callback< ENUMRESNAMEPROC, 4 >	enum_res_name_callback;
	typedef konbu::callback< ENUMRESTYPEPROC, 4 >	enum_res_type_callback;
	typedef konbu::callback< ENUMRESLANGPROC, 5 >	enum_res_lang_callback;

//###########################################################
//  リソースデータを操作
//
  // リソースハンドルを見つける
	res_handle find_resource( module_handle mh, res_id rid, LPCTSTR rtype );
  // リソースデータを取得する
  	buffer_range<BYTE> get_resource_data( module_handle mh, res_handle rh );
	buffer_range<BYTE> get_resource_data( module_handle mh, res_id rid, LPCTSTR rtype );
  	
  // リソースの変更・更新
	  
	  /*
	class resource
	{
		resource( const module_handle
	private:
		res_handle    rh_;
		module_handle mh_;
	};*/
	
//###########################################################
//  リソース入りファイルを調査する
//
  // IDを列挙する
  	template< typename EnumResNameHandler >
	bool enum_resource_id( module_handle mod, LPCTSTR type, const EnumResNameHandler &handler );
	
  // 含まれるリソースタイプを列挙する
  	template< typename EnumResTypeHandler >
	bool enum_resource_type( module_handle mod, const EnumResTypeHandler &handler );
	
  // 言語IDを列挙
  	template< typename EnumResLangHandler >
	bool enum_resource_lang( module_handle mod, LPCTSTR type, res_id id, const EnumResLangHandler &handler );

  // リソースの数を数える
	int count_resource_num( module_handle mod, LPCTSTR type );
  
  // このタイプのリソースを持ってるか？
	bool contains_resource( module_handle mod, LPCTSTR type );
		
//----------------------------------------------------------------------------
//
//  テンプレートの実装
//
//----------------------------------------------------------------------------
//
  	template< typename EnumResNameHandler >
	bool enum_resource_id( module_handle mod, LPCTSTR type, const EnumResNameHandler &handler )
	{
		return 
		FALSE!=::EnumResourceNames(
					mod.value(),
					type,
					&enum_res_name_callback::set<EnumResNameHandler>::entry,
					reinterpret_cast<LONG_PTR>( &handler )
				);
	}
	
  	template< typename EnumResTypeHandler >
	bool enum_resource_type( module_handle mod, const EnumResTypeHandler &handler )
	{
		return 
		FALSE!=::EnumResourceTypes(
					mod.value(),
					&enum_res_type_callback::set<EnumResTypeHandler>::entry,
					reinterpret_cast<LONG_PTR>( &handler )
				);
	}
	
  	template< typename EnumResLangHandler >
	bool enum_resource_lang( module_handle mod, LPCTSTR type, res_id id, const EnumResLangHandler &handler )
	{
		return 
		FALSE!=::EnumResourceLanguages(
					mod.value(),
					type,
					id.get(),
					&enum_res_lang_callback::set<EnumResLangHandler>::entry,
					reinterpret_cast<LONG_PTR>( &handler )
				);		
	}

}} /* end of namespace oden.gammo */


#endif
