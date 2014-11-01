/** 
* �����^�C�v�̓��������߂�
*
*/

#pragma once

#ifndef ODEN_CHKW_FUNCS
#define ODEN_CHKW_FUNCS

#include <cstring>
#include <cstdlib>
#include <cwchar>

#pragma warning(push)
#pragma warning(disable:4996)

#ifdef _MSC_VER
#	define ODEN_CHIKUWA_NO_FLOAT
#	define ODEN_CHIKUWA_NO_LONGDOUBLE
#	define ODEN_CHIKUWA_INT64
#	define ODEN_CHIKUWA_UINT64
#	define HAS_SECURE_FUNC
#endif

// #define ODEN_CHIKUWA_STANDARD

namespace oden { namespace chikuwa 
{ 

namespace str
{
//###############################################################
// ����
//



//###############################################################
// �����^�C���E�����񑀍샋�[�`���̃}�b�v
//
  /*----------------------------------*/
  // ������ҏW
  /*----------------------------------*/
   // ������ǂ�����A������
	inline char* cat(
		char*			dest,
		const char* 	source
		){
		return strcat(dest,source);
	}
	inline wchar_t* cat(
		wchar_t*		 dest,
		const wchar_t*   source
		){
		return wcscat(dest,source);
	}	
	
   // ��������R�s�[����	
	inline char* copy(
		char*			dest,
		const char* 	source
		){
		return strcpy(dest,source);
	}
	inline wchar_t* copy(
		wchar_t*		dest,
		const wchar_t* 	source
		){
		return wcscpy(dest,source);
	}
	
#ifndef ODEN_CHIKUWA_STANDARD
	inline char* copy(
		char*			dest,
		int				destsize,
		const char* 	source
		){
	return
	#ifdef HAS_SECURE_FUNC
		strcpy_s( dest, destsize, source )==0 ? 0 : dest;
	#endif
	}
	inline wchar_t* copy(
		wchar_t*		dest,
		int				destsize,
		const wchar_t* 	source
		){
	return 
	#ifdef HAS_SECURE_FUNC
		wcscpy_s(dest, destsize, source)==0 ? 0 : dest;
	#endif
	}
		
	inline char* copy_n(
		char*			dest,
		int				destsize,
		const char* 	source,
		int				sourcesize
		){
	return 
	#ifdef HAS_SECURE_FUNC
		strncpy_s(dest, destsize, source, sourcesize)==0 ? 0 : dest;
	#endif
	}
	inline wchar_t* copy_n(
		wchar_t*		dest,
		int				destsize,
		const wchar_t* 	source,
		int				sourcesize
		){
	return 
	#ifdef HAS_SECURE_FUNC
		wcsncpy_s(dest, destsize, source, sourcesize)==0 ? 0 : dest;
	#endif
	}
#endif
	
  /*----------------------------------*/
  // ������𒲂ׂ�
  /*----------------------------------*/
   // ������̃o�C�g�������߂�
	inline size_t length(
		const char* str
		){
		return strlen(str);	
	}
	inline size_t length(
		const wchar_t* str
		){
		return wcslen(str);	
	}		
   	
   // ��������r���� 	
	inline int comp(
		const char* str1,
		const char* str2
		){
		return strcmp(str1,str2);
	}	 	
	inline int comp(
		const wchar_t* str1,
		const wchar_t* str2
		){
		return wcscmp(str1,str2);
	}		
   // �������n�o�C�g������r���� 	
	inline int comp_n(
		const char* str1,
		const char* str2,
		size_t count
		){
		return strncmp(str1,str2,count);
	} 	
	inline int comp_n(
		const wchar_t* str1,
		const wchar_t* str2,
		size_t count
		){
		return wcsncmp(str1,str2,count);
	}	
   // ���������������
	inline const char* find(
		const char* source,
		const char* search_str
		){
		return strstr(source,search_str);
	}	
	inline const wchar_t* find(
		const wchar_t* source,
		const wchar_t* search_str
		){
		return wcsstr(source,search_str);
	}
	
  /*----------------------------------*/
  // ������𑖍�����
  /*----------------------------------*/
   // �g�[�N�����Ƃɋ�؂��Ă���(�}���`�X���b�h�ł͎g���Ȃ�)
	inline char* token(
		char* str,
		const char* derimiter
		){
		return strtok(str,derimiter);	
	}	
	inline wchar_t* token(
		wchar_t* str,
		const wchar_t* derimiter
		){
		return wcstok(str,derimiter);	
	}
	
  /*----------------------------------*/
  // �������ϊ�����
  /*----------------------------------*/	
   // ������𐔒l������   	
	inline long to_long(
		const char* str,
		int base = 10,
		char** errpos = 0
		){
		return strtol(str,errpos,base);
	}   	
	inline long to_long(
		const wchar_t* str,
		int base = 10,
		wchar_t** errpos = 0
		){
		return wcstol(str,errpos,base);
	}	
	
   // ������𐔒l������   	
	inline unsigned long to_ulong(
		const char* str,
		int base = 10,
		char** errpos = 0
		){
		return strtoul(str,errpos,base);
	}   	
	inline unsigned long to_ulong(
		const wchar_t* str,
		int base = 10,
		wchar_t** errpos = 0
		){
		return wcstoul(str,errpos,base);
	}
	
   // �������{���x�����ɕϊ�����  
	inline double to_double(
		const char* str,
		char** errpos = 0
		){
		return strtod(str,errpos);
	} 	
	inline double to_double(
		const wchar_t* str,
		wchar_t** errpos = 0
		){
		return wcstod(str,errpos);
	}
	
#ifdef ODEN_CHIKUWA_NO_FLOAT
#else
   // 
	inline float to_float(
		const char* str,
		char** errpos = 0
		){
		return strtof(str,errpos);
	} 	
	inline float to_float(
		const wchar_t* str,
		wchar_t** errpos = 0
		){
		return wcstof(str,errpos);
	}
#endif
	
#ifdef ODEN_CHIKUWA_NO_LONGDOUBLE
#else
   // 
	inline long double to_longdouble(
		const char* str,
		char** errpos = 0
		){
		return strtold(str,errpos);
	} 	
	inline long double to_longdouble(
		const wchar_t* str,
		wchar_t** errpos = 0
		){
		return wcstold(str,errpos);
	}
#endif

#ifdef ODEN_CHIKUWA_NO_LONGLONG
#elif defined ODEN_CHIKUWA_INT64
	inline __int64 to_longlong(
		const char* str,
		int base = 10,
		char** errpos = 0
		){
		return _strtoi64(str,errpos,base);
	} 	
	inline __int64 to_longlong(
		const wchar_t* str,
		int base = 10,
		wchar_t** errpos = 0
		){
		return _wcstoi64(str,errpos,base);
	}
#else
   // 
	inline long long to_longlong(
		const char* str,
		char** errpos = 0
		){
		return strtoll(str,errpos);
	} 	
	inline long long to_longlong(
		const wchar_t* str,
		wchar_t** errpos = 0
		){
		return wcstoll(str,errpos);
	}
#endif

   // 
#ifdef ODEN_CHIKUWA_NO_ULONGLONG
#elif defined ODEN_CHIKUWA_UINT64
	inline unsigned __int64 to_ulonglong(
		const char* str,
		int base = 10,
		char** errpos = 0
		){
		return _strtoui64(str,errpos,base);
	} 	
	inline unsigned __int64 to_ulonglong(
		const wchar_t* str,
		int base = 10,
		wchar_t** errpos = 0
		){
		return _wcstoui64(str,errpos,base);
	}
#else
	inline unsigned long long to_ulonglong(
		const char* str,
		char** errpos = 0
		){
		return strtoull(str,errpos);
	} 	
	inline unsigned long long to_ulonglong(
		const wchar_t* str,
		wchar_t** errpos = 0
		){
		return wcstoull(str,errpos);
	}
#endif

}
}
}

#undef HAS_SECURE_FUNC

#pragma warning(pop)
#endif