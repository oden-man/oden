#include "../Pch.h"

#include <oden/gammo/system/file/file.h>

namespace oden { namespace gammo
{
//############################################################
//
//
bool file::create( string_ptr filename, DWORD access, DWORD sharemode, DWORD disposition, DWORD attr )
{
	if( file_handle h = ::CreateFile( filename.c_str(), access, sharemode, 0, disposition, attr, 0 ) )
	{
		attach(h);
		return true;
	}
	return false;
}

bool file::read( buffer_range<BYTE> buf, DWORD& read_size )const
{
	return FALSE!=::ReadFile( handle_value(), buf.get(), buf.size(), &read_size, NULL );
}

bool file::write( memory_range<BYTE> dat, DWORD& write_size )const
{
	return FALSE!=::WriteFile( handle_value(), dat.get(), dat.size(), &write_size, NULL );
}

bool file::seek( const large_int& offs, DWORD dir )const
{
	return FALSE!=::SetFilePointerEx( handle_value(), offs.data(), 0, dir );
}

bool file::tell( large_int& out )const
{
	return FALSE!=::SetFilePointerEx( handle_value(), large_int().data(), &out.data(), FILE_CURRENT );
}

bool file::flush()const
{
	return FALSE!=::FlushFileBuffers( handle_value() );
}

bool file::lock( const large_int& start, const large_int& length )const
{
	return FALSE!=::LockFile( handle_value(), start.low(), start.high(), length.low(), length.high() );
}
bool file::unlock( const large_int& start, const large_int& length )const
{
	return FALSE!=::UnlockFile( handle_value(), start.low(), start.high(), length.low(), length.high() );
}

bool file::get_size( large_int& out )const
{
	return FALSE!=::GetFileSizeEx( handle_value(), &out.data() );
}

bool file::set_size()const
{
	return FALSE!=::SetEndOfFile( handle_value() );
}

DWORD file::type()const
{
	return ::GetFileType( handle_value() );
}


}}

