#include "../Pch.h"

#include <oden/gammo/system/thread/tls.h>

namespace oden { namespace gammo
{

// �m��
tls_index allocate_tls()
{
	return ::TlsAlloc();
}

//
//  thread_local_stroage �N���X
//

// ���
bool thread_local_stroage::release( )
{
	return ::TlsFree( index_.value() ) != FALSE;
}

// �l�l��
LONG_PTR gammo::thread_local_stroage::value( )const
{
	return reinterpret_cast<LONG_PTR>( ::TlsGetValue( index_.value() ) );
}

// �l�ݒ�
bool gammo::thread_local_stroage::set( LONG_PTR val )const
{
	return ::TlsSetValue( index_.value(), reinterpret_cast<LPVOID>(val) ) != FALSE;
}

}}

