#pragma once

#ifndef ODEN_GOB_GRA_GDI_OBJ
#define ODEN_GOB_GRA_GDI_OBJ

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
#include <oden/property.h>
// --------- gammo -----------

//
//
//#################################################



namespace oden { namespace gammo
{
//_______________________________________________________________________________________
//
//    GDI�I�u�W�F�N�g
//_______________________________________________________________________________________
//		
//###############################################
//  gdi_object �x�[�X�N���X
//
	template< typename H = gdi_obj_handle >
	class gdi_obj
		: public handle_interface<H>
	{
	public:
	// �R���X�g���N�^
		gdi_obj( H h = boost::none )
			: handle_interface<H>( h )
		{}
		
	// �f���[�^
		typedef struct destroyer {
			void operator()( gdi_obj& o )const { o.destroy(); }
		} deleter_type;
		
	// �����o�֐�
	  // ���ʌ^�̃n���h�����擾
		gdi_obj_handle::value_type base_handle_value()const { return this->handle_value(); }
		gdi_obj_handle base_handle()const { return this->base_handle_value(); }

	  // �^�C�v���擾
		DWORD type()const {
			return ::GetObjectType( base_handle_value() ); 
		}
	  // �f�[�^���擾�i�ėp�^�j
		template< typename Struct >
		bool get_info( Struct& r )const {
			return sizeof(Struct) == ::GetObject( base_handle_value(), sizeof(Struct), &r );
		}
	  // �j������
		bool destroy( )const {
			return FALSE!=::DeleteObject( base_handle_value() );
		}
	};
	
//##############################################################
//  gdi_object�N���X
//
	
//##############################################################
//  gdi_object �G�C���A�X
//	
	
//##############################################################
//  gdi_object typedef
//
}}

#endif