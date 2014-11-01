#include "../Pch.h"

#include <oden/gammo/system/dll/module.h>

namespace oden { namespace gammo
{
//#######################################################
// module 
//

// �A�h���X���擾
void* module::proc_address( narrow_string_ptr name )const
{
	return ::GetProcAddress( handle_value(), name.c_str() );
} 
void* module::proc_address( int ordinal )const
{
	return ::GetProcAddress( handle_value(), reinterpret_cast<const char*>(ordinal) );
}

//
bool module::release()
{
	return FALSE!=::FreeLibrary( handle_value() );
}

//
bool module::get_path( string& out )const
{
	out.clear();

	DWORD copied = 0;
	while( extend_str_buf( out, copied ) )
	{
		copied = ::GetModuleFileName( handle_value(),  buf_begin<char_t>(out), static_cast<int>(out.size()) );
		if( copied == 0 )
			return false;
	}
	return true;
}

//####################################
//  �����֐�
//
// �t���O�Ȃ�
module_handle load_library( string_ptr path )
{
	return ::LoadLibrary( path.c_str() );	
}

// �t���O�ŐF�X�w��
module_handle load_library( string_ptr path, DWORD option )
{
	return ::LoadLibraryEx( path.c_str(), NULL, option );
}

}} /* end of namespace gammo, oden */

