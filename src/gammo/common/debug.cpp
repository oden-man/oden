#include "../pch.h"

#include <oden/gammo/common/debug.h>
#include <cassert>
#include <vector>

namespace oden { namespace gammo
{
//##################################################
// error
//
  // デバッグ文字を出力
	void debug_string(const char* str)    { OutputDebugStringA(str); }
	void debug_string(const wchar_t* str) { OutputDebugStringW(str); }
	
  // 書式付き文字列をデバッグウィンドウに出力
	void debug_string( size_t buf_size, const char* str,... )
	{
		std::vector<char> buf(buf_size);
	
		va_list vali;
		va_start(vali,str);
		vsprintf_s(&buf[0], buf_size, str, vali);
		va_end(vali);
	
		OutputDebugStringA(&buf[0]);
	}
	
	void debug_string( size_t buf_size, const wchar_t* str,... )
	{
		std::vector<wchar_t> buf(buf_size);
	
		va_list vali;
		va_start(vali,str);
		vswprintf_s(&buf[0] ,buf_size, str, vali);
		va_end(vali);
	
		OutputDebugStringW(&buf[0]);
	}
	
  // デバッグ用簡易書式付きメッセージボックス
	void debug_dialog( size_t buf_size, const char* str, ...)
	{ 
		char* buf = new char[buf_size];
		memset(buf,0,buf_size);
	
		va_list vali;
		va_start(vali,str);
		vsprintf_s(buf,buf_size,str,vali);
		va_end(vali);
	
		MessageBoxA(NULL,buf,"デバッグメッセージ",MB_OK|MB_ICONASTERISK|MB_TOPMOST);

		delete[] buf;
	}
	
	void debug_dialog( size_t buf_size, const wchar_t* str, ...)
	{ 	
		wchar_t* buf = new wchar_t[buf_size];
		memset(buf,0,buf_size);
	
		va_list vali;
		va_start(vali,str);
		vswprintf_s(buf,buf_size,str,vali);
		va_end(vali);
	
		MessageBoxW(NULL,buf,L"デバッグメッセージ",MB_OK|MB_ICONASTERISK|MB_TOPMOST); 

		delete[] buf;
	}

}}
