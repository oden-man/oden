
/*
* �f�o�b�O�Ɏg��������
*
*
*/
#pragma once

#ifndef ODEN_GAMMO_DBG
#define ODEN_GAMMO_DBG

#include <crtdbg.h>

//#######################################################
//
//
namespace oden { namespace gammo
{
  // �f�o�b�O�������o��
	void debug_string( const char* str );
	void debug_string( const wchar_t* str );
	
  // �����t����������f�o�b�O�E�B���h�E�ɏo��
	void debug_string( size_t buf_size, const char* str, ...);
	void debug_string( size_t buf_size, const wchar_t* str, ... );
	
  // �f�o�b�O�p�ȈՏ����t�����b�Z�[�W�{�b�N�X
	void debug_dialog( size_t buf_size, const char* str, ... );
	void debug_dialog( size_t buf_size, const wchar_t* str, ... );

}} /* end of namespace oden.gammo */

//#######################################################
// DEBUG��
//
#ifdef _DEBUG
	#define GAMMO_DEBUG_LIT(s) 				oden::gammo::debug_string(512,_T("![%s:%d] %s\n"),_T(__FILE__),__LINE__,s)

	#define GAMMO_DEBUG_STR(s,...)			oden::gammo::debug_string(512,_T("![%s:%d]")s _T("\n"),_T(__FILE__),__LINE__,__VA_ARGS__)
	#define GAMMO_DEBUG_STRc(sz,s,...)   	oden::gammo::debug_string(sz+300,_T("![%s:%d]")s _T("\n"),_T(__FILE__),__LINE__,__VA_ARGS__)

	#define GAMMO_DEBUG_DLG(s,...)			oden::gammo::debug_dialog(512,_T("file:%s\nline:%d\n")s _T("\n"),_T(__FILE__),__LINE__,__VA_ARGS__)
	#define GAMMO_DEBUG_DLGc(sz,s,...)		oden::gammo::debug_dialog(sz+300,_T("file:%s\nline:%d\n")s _T("\n"),_T(__FILE__),__LINE__,__VA_ARGS__)

  // �֐������f�o�b�O�E�B���h�E�ɏo��
  /*
	#define SPECIFY_FUNCCALL() \
	{ \
		OutputDebugString(_T("��"));\
		OutputDebugString(_T(__FUNCTION__)); \
		OutputDebugString(_T("\n"));\
	}
	*/

  // ���������[�N�����o
	#define GAMMO_CHECK_MEMORY_LEAK() 			_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF)

  // �G���[���N�������������m�ۂ���new�ɁA�u���[�N�|�C���g��ݒ�
	#define GAMMO_BREAK_ALLOC(number) 			_CrtSetBreakAlloc(number)

//#######################################################
// RELEASE�� (�����)
//
#else
	#define GAMMO_DEBUG_LIT(...)
	#define GAMMO_DEBUG_STR(...)
	#define GAMMO_DEBUG_STRc(...)
	#define GAMMO_DEBUG_DLG(...)
	#define GAMMO_DEBUG_DLGc(...)
	#define GAMMO_CHECK_MEMORY_LEAK()
	#define GAMMO_BREAK_ALLOC(n)

#endif /* ifdef _DEBUG end */

#endif /* include guard end */

