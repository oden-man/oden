#include "../Pch.h"

//
//#include <oden/gammo/ui/resource/stream.h>

//
#include <oden/gammo/ui/resource/resource.h>
//#include <oden/gammo/system/dll/module.h>

namespace oden { namespace gammo
{
//_____________________________________________________________________________
//
//
//      リソース調査
//
//_____________________________________________________________________________
//
//###########################################################
//  リソースデータを操作
//
res_handle find_resource( module_handle mh, res_id rid, LPCTSTR rtype )
{
	return ::FindResource( mh.value(), rid.get(), rtype );
}

buffer_range<BYTE> get_resource_data( module_handle mh, res_handle rh )
{
	HGLOBAL hLoad = ::LoadResource( mh.value(), rh.value() );
	if( !hLoad )
		return boost::none;
	
	return buffer_range<BYTE>( reinterpret_cast<BYTE*>(::LockResource(hLoad)), ::SizeofResource(mh.value(), rh.value()) );
}
buffer_range<BYTE> get_resource_data( module_handle mh, res_id rid, LPCTSTR rtype )
{
	return get_resource_data( mh, find_resource( mh, rid, rtype ) );
}

// 更新
	
	
//####################################################################
// 列挙
//
// カウント
int count_resource_num ( module_handle mod, LPCTSTR type )
{
  // 通信用データの作成
  	int count_ = 0;
	
  // コールバック関数の定義
	struct Counter
	{
		static BOOL CALLBACK entry( HMODULE, LPCTSTR, LPTSTR, LONG_PTR lParam )
		{
			int* p_count_ = reinterpret_cast<int*>(lParam);
			++(*p_count_);
			return TRUE;
		}
	};

   // モジュール内のリソース数を数える
	::EnumResourceNames(
		mod.value(),
		type,
		&Counter::entry,
		reinterpret_cast< LONG_PTR >( &count_ )
	);
	return count_;
}

// このタイプのリソースを持っているか?
bool contains_resource ( module_handle mod, LPCTSTR type )
{ 
	return count_resource_num( mod, type )!=0; 
}	

} } /* end of namespace oden.gammo */



