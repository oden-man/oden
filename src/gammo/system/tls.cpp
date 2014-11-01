#include "../Pch.h"

#include <oden/gammo/system/thread/tls.h>

namespace oden { namespace gammo
{

// 確保
tls_index allocate_tls()
{
	return ::TlsAlloc();
}

//
//  thread_local_stroage クラス
//

// 解放
bool thread_local_stroage::release( )
{
	return ::TlsFree( index_.value() ) != FALSE;
}

// 値獲得
LONG_PTR gammo::thread_local_stroage::value( )const
{
	return reinterpret_cast<LONG_PTR>( ::TlsGetValue( index_.value() ) );
}

// 値設定
bool gammo::thread_local_stroage::set( LONG_PTR val )const
{
	return ::TlsSetValue( index_.value(), reinterpret_cast<LPVOID>(val) ) != FALSE;
}

}}

