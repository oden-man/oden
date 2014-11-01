#pragma once

#ifndef ODEN_GAMMO_SYS_PROS
#define ODEN_GAMMO_SYS_PROS

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
#include <TlHelp32.h>
#include <PsApi.h>
// --------- boost ----------
#include <boost/optional.hpp>
// --------- oden ----------
#include <oden/property.h>
//----------- gammo ----------
#include <oden/gammo/system/kernel_object.h>
#pragma comment(lib, "PsApi.lib")

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            ファイル クラス
//
//________________________________________________________________________________
//

// typedef
	typedef kernel_obj_handle snapshot_handle;

//################################################################
//  コントロールクラス
//
	class process
		: public kernel_obj
	{
	public:	
	// コンストラクタ
		explicit process( DWORD id = 0, process_handle h = boost::none )
			: kernel_obj(h), id_(id)
		{}
		
	// ハンドル
	  // 指定idのハンドルを開く
		bool open ( DWORD access, bool bInherit = false );
	  // 指定ファイル名

	// プロセスメモリ
		boost::optional<int> read_memory ( DWORD address, buffer_range<BYTE> buf )const;
		boost::optional<int> write_memory ( DWORD address, memory_range<BYTE> data )const;
	// 操作
		bool terminate ( UINT exitcode )const;

    // 取得
	  // id
		DWORD get_id ()const;
		DWORD id ()const { return id_; }
		void set_id ( DWORD dw ){ id_ = dw; } 

	  // スナップショットをとる
		snapshot_handle snapshot( DWORD f )const;

	  // パス名の取得
		bool get_fullpath( string& out )const;

	// 列挙
	  // 全プロセスを列挙
		static boost::optional<DWORD> enumrate_id( buffer_range<DWORD> ids );

	private:
		DWORD id_;
	};
	
//################################################################
//  tool help library
//
  // プロセスのスナップショットをとる
	snapshot_handle create_process_snapshot( DWORD flags, DWORD procid );

// ベースクラス
//
	template< typename T >
	class snapshot_entry_base
		: public kernel_obj
	{
	public:
		snapshot_entry_base( snapshot_handle h = boost::none )
			: handle_interface<snapshot_handle>(h), entry_()
		{
			entry_.dwSize = sizeof(T);
		}

		const T& entry()const { return entry_; }
		T& entry() { return entry_; }

		bool eof()const { return ::GetLastError() == ERROR_NO_MORE_FILES; }

	private:
		T entry_;
	};

// プロセス
//
	class process_entry
		: public snapshot_entry_base<PROCESSENTRY32>
	{
	public:
		bool first();
		bool next();

		DWORD id() const				{ return entry().th32ProcessID; }
		DWORD parent_id() const			{ return entry().th32ParentProcessID; }
		LONG  base_priority() const		{ return entry().pcPriClassBase; }
		string name()const				{ return entry().szExeFile; }
	};

// モジュール
//
	class module_entry
		: public snapshot_entry_base<MODULEENTRY32>
	{
	public:
		bool first();
		bool next();

		DWORD process_id() const		{ return entry().th32ProcessID; }
		BYTE* base_address()const		{ return entry().modBaseAddr; }
		DWORD size()const				{ return entry().modBaseSize; }
		module_handle mod_handle()const { return entry().hModule; }
		string name()const				{ return entry().szModule; }
		string path()const				{ return entry().szExePath; }
	};
}}

#endif
