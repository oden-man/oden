#pragma once

#ifndef ODEN_GAMMO_SYS_PROS
#define ODEN_GAMMO_SYS_PROS

//#######################################################
//  �C���N���[�h
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
//            �t�@�C�� �N���X
//
//________________________________________________________________________________
//

// typedef
	typedef kernel_obj_handle snapshot_handle;

//################################################################
//  �R���g���[���N���X
//
	class process
		: public kernel_obj
	{
	public:	
	// �R���X�g���N�^
		explicit process( DWORD id = 0, process_handle h = boost::none )
			: kernel_obj(h), id_(id)
		{}
		
	// �n���h��
	  // �w��id�̃n���h�����J��
		bool open ( DWORD access, bool bInherit = false );
	  // �w��t�@�C����

	// �v���Z�X������
		boost::optional<int> read_memory ( DWORD address, buffer_range<BYTE> buf )const;
		boost::optional<int> write_memory ( DWORD address, memory_range<BYTE> data )const;
	// ����
		bool terminate ( UINT exitcode )const;

    // �擾
	  // id
		DWORD get_id ()const;
		DWORD id ()const { return id_; }
		void set_id ( DWORD dw ){ id_ = dw; } 

	  // �X�i�b�v�V���b�g���Ƃ�
		snapshot_handle snapshot( DWORD f )const;

	  // �p�X���̎擾
		bool get_fullpath( string& out )const;

	// ��
	  // �S�v���Z�X���
		static boost::optional<DWORD> enumrate_id( buffer_range<DWORD> ids );

	private:
		DWORD id_;
	};
	
//################################################################
//  tool help library
//
  // �v���Z�X�̃X�i�b�v�V���b�g���Ƃ�
	snapshot_handle create_process_snapshot( DWORD flags, DWORD procid );

// �x�[�X�N���X
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

// �v���Z�X
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

// ���W���[��
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
