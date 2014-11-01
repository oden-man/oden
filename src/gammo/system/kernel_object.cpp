#include "../Pch.h"

#include <oden/gammo/system/file/file.h>

namespace oden { namespace gammo
{

bool kernel_obj::close()const
{
	return FALSE!=::CloseHandle(handle_value());
}

kernel_obj_handle kernel_obj::duplicate_handle( process_handle src, process_handle dest, bool inherit, DWORD option, DWORD access )const
{
	kernel_obj_handle h;
	if( 0!=::DuplicateHandle( src.value(), handle_value(), dest.value(), &h.value(), access, inherit?TRUE:FALSE, option ) )
		return h;
	return boost::none;
}



}}

