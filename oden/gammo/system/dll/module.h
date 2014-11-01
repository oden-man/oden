#pragma once

#ifndef ODEN_GAMMO_SYS_DLL_MOD
#define ODEN_GAMMO_SYS_DLL_MOD

//#############################################
//  �C���N���[�h
//

//-------- defs ---------
#include <oden/gammo/defs.h>

//-------- boost --------

//-------- oden ---------

//-------- gammo ---------

//
//#############################################


namespace oden { namespace gammo
{	
//########################################
// �����L���X�g�N���X
//
namespace detail
{
	class proc_caster
	{
	public:
		proc_caster( void* pfn )
			: ptr_( pfn )
		{}
		
		template< typename FuncPtr >
		operator FuncPtr()const
		{
			return static_cast< FuncPtr >( ptr_ );
		}
	private:
		void* ptr_;
	};
}

//########################################
// ���W���[���E��{�N���X
//
	class module
		: public handle_interface<module_handle>
	{
	private:
		typedef module self;
	public:
	// �R���X�g���N�^�E�f�X�g���N�^
		module( module_handle h = boost::none )
			: handle_interface<module_handle>( h )
		{}
		
	// �f���[�^
		typedef struct releaser {
			void operator()( self& mod )const{ mod.release(); }
		} deleter_type;
		
	// �����o�֐�
	  // �֐�����肾��
		void* proc_address( narrow_string_ptr proc_name )const;
		void* proc_address( int ordinal )const;
	  // �L���X�g�@�\�t���Ŋ֐����Ƃ肾��
		detail::proc_caster get_proc_address( narrow_string_ptr proc_name )const 
		 { return detail::proc_caster( this->proc_address( proc_name ) ); }
		detail::proc_caster get_proc_address( int ordinal )const 
		 { return detail::proc_caster( this->proc_address( ordinal ) ); }
	  // ���
		bool release();
	  // ���W���[���̃p�X���擾
		bool get_path( string& out )const;
		string path()const { string out; this->get_path(out); return out; }

	};
	
//########################################
// �R���X�g���N�g�֐�
//		
    // �t���O�Ȃ��Ń��[�h����
	module_handle load_library ( string_ptr path );
	
    // �t���O���w�肵�ă��[�h����
	module_handle load_library ( string_ptr path, DWORD option );
	
}}

#endif
