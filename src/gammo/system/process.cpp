#include "../Pch.h"
#include <oden/gammo/common/string/extend_str_buf.h>

#include <boost/optional.hpp>
#include <oden/gammo/system/process.h>

namespace oden { namespace gammo
{
//############################################################
//
//
bool process::open( DWORD access, bool bInherit )
{
	if( HANDLE h = ::OpenProcess( access, bInherit?TRUE:FALSE, id_ ) )
	{
		this->attach(h);
		return true;
	}
	return false;
}

boost::optional<int> process::read_memory( DWORD address, buffer_range<BYTE> buf )const
{
	assert(!this->fail());

	DWORD sz = 0;
	if( 0 == ::ReadProcessMemory( handle_value(), reinterpret_cast<LPCVOID>(address), buf.get(), buf.size(), &sz ) )
		return boost::none;
	return static_cast<int>(sz);
}

boost::optional<int> process::write_memory( DWORD address, memory_range<BYTE> data )const
{
	assert(!this->fail());

	DWORD sz = 0;
	if( 0 == ::WriteProcessMemory( handle_value(), reinterpret_cast<LPVOID>(address), data.get(), data.size(), &sz ) )
		return boost::none;
	return static_cast<int>(sz);
}

bool process::terminate( UINT exitcode )const
{
	assert(!this->fail());
	return 0!=::TerminateProcess( handle_value(), exitcode );
}

DWORD process::get_id()const
{
	assert(!this->fail());
	return ::GetProcessId( handle_value() );
}

snapshot_handle process::snapshot( DWORD f )const
{
	return create_process_snapshot( f, id_ );
}

bool process::get_fullpath( string& out )const
{
	assert(!this->fail());

	DWORD sz=0;
	out.resize(256);

	do {
		sz = out.size();
		if( 0 == QueryFullProcessImageName( handle_value(), 0, &out[0], &sz ) )
			return false;

	}while( extend_str_buf( out, sz ) );

	return true;
}

boost::optional<DWORD> process::enumrate_id( buffer_range<DWORD> ids )
{
	DWORD ret = 0;
	if( 0==::EnumProcesses( ids.get(), ids.size()*sizeof(DWORD), &ret ) )
		return boost::none;

	return ret/sizeof(DWORD);
}


//
//
snapshot_handle create_process_snapshot( DWORD flags, DWORD procid )
{
	return ::CreateToolhelp32Snapshot( flags, procid );
}

bool process_entry::first()
{
	return 0!=::Process32First( handle_value(), &entry() );
}
bool process_entry::next()
{
	return 0!=::Process32Next( handle_value(), &entry() );
}
 
bool module_entry::first()
{
	return 0!=::Module32First( handle_value(), &entry() );
}
bool module_entry::next()
{
	return 0!=::Module32Next( handle_value(), &entry() );
}

}}

