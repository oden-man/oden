#include "../Pch.h"

#include <Shlwapi.h>
#include <oden/gammo/system/com/stream.h>

namespace oden { namespace gammo
{

bool com_stream::create( memory_range<BYTE> init )
{
	if( IStream* pNew = ::SHCreateMemStream( init.get(), init.size() ) )
	{
		this->sp().reset( pNew );	
		return true;
	}

	return false;
}

hresult com_stream::read( buffer_range<BYTE> buf, ULONG &readed )const
{
	return this->get()->Read( buf.get(), buf.size(), &readed );
}

hresult com_stream::write( memory_range<BYTE> mem, ULONG &written )const
{	
	return this->get()->Write( mem.get(), mem.size(), &written );
}


}}

