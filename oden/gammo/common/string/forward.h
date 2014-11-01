#pragma once

#ifndef ODEN_GAMMO_CMN_STR_FWD
#define ODEN_GAMMO_CMN_STR_FWD
//################################################
//   �C���N���[�h
//

// --------- defs -----------
#include <tchar.h>
// --------- STL ------------
//#include <vector>
#include <string>
// --------- oden.chikuwa -----------

//
//################################################

//###################################################
//
//

namespace oden 
{ 
namespace chikuwa
{
	template< typename > class string_ptr;
}
	
namespace gammo
{
	
//#####################################################
// typedefs
//
  // WindowsAPIunicode�ł̕����R�[�h��utf-16BE  	
  // �܂�tchar�ɍ��킹�Ƃ�
  	typedef TCHAR 									char_t;	
	
  // �Ή�����basic_string
	typedef std::basic_string< char_t > 			string;
	
  // string_ptr
	typedef chikuwa::string_ptr< char_t > 			string_ptr;
	
  // ANSI			
	typedef std::basic_string< char >   			narrow_string;
	typedef chikuwa::string_ptr< char > 			narrow_string_ptr;
	
  // WIDE
	typedef std::basic_string< wchar_t >   			wide_string;
	typedef chikuwa::string_ptr< wchar_t >   		wide_string_ptr;

}}

#endif
