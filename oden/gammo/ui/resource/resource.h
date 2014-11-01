#pragma once

#ifndef ODEN_GOB_UI_RES_DEFS
#define ODEN_GOB_UI_RES_DEFS

//#######################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- std ------------
// --------- boost ----------
// --------- oden -----------
#include <oden/konbu/callback.h>

// --------- gammo ----------

//
//
//####################################################


namespace oden { namespace gammo
{	
	typedef konbu::callback< ENUMRESNAMEPROC, 4 >	enum_res_name_callback;
	typedef konbu::callback< ENUMRESTYPEPROC, 4 >	enum_res_type_callback;
	typedef konbu::callback< ENUMRESLANGPROC, 5 >	enum_res_lang_callback;

//###########################################################
//  ���\�[�X�f�[�^�𑀍�
//
  // ���\�[�X�n���h����������
	res_handle find_resource( module_handle mh, res_id rid, LPCTSTR rtype );
  // ���\�[�X�f�[�^���擾����
  	buffer_range<BYTE> get_resource_data( module_handle mh, res_handle rh );
	buffer_range<BYTE> get_resource_data( module_handle mh, res_id rid, LPCTSTR rtype );
  	
  // ���\�[�X�̕ύX�E�X�V
	  
	  /*
	class resource
	{
		resource( const module_handle
	private:
		res_handle    rh_;
		module_handle mh_;
	};*/
	
//###########################################################
//  ���\�[�X����t�@�C���𒲍�����
//
  // ID��񋓂���
  	template< typename EnumResNameHandler >
	bool enum_resource_id( module_handle mod, LPCTSTR type, const EnumResNameHandler &handler );
	
  // �܂܂�郊�\�[�X�^�C�v��񋓂���
  	template< typename EnumResTypeHandler >
	bool enum_resource_type( module_handle mod, const EnumResTypeHandler &handler );
	
  // ����ID���
  	template< typename EnumResLangHandler >
	bool enum_resource_lang( module_handle mod, LPCTSTR type, res_id id, const EnumResLangHandler &handler );

  // ���\�[�X�̐��𐔂���
	int count_resource_num( module_handle mod, LPCTSTR type );
  
  // ���̃^�C�v�̃��\�[�X�������Ă邩�H
	bool contains_resource( module_handle mod, LPCTSTR type );
		
//----------------------------------------------------------------------------
//
//  �e���v���[�g�̎���
//
//----------------------------------------------------------------------------
//
  	template< typename EnumResNameHandler >
	bool enum_resource_id( module_handle mod, LPCTSTR type, const EnumResNameHandler &handler )
	{
		return 
		FALSE!=::EnumResourceNames(
					mod.value(),
					type,
					&enum_res_name_callback::set<EnumResNameHandler>::entry,
					reinterpret_cast<LONG_PTR>( &handler )
				);
	}
	
  	template< typename EnumResTypeHandler >
	bool enum_resource_type( module_handle mod, const EnumResTypeHandler &handler )
	{
		return 
		FALSE!=::EnumResourceTypes(
					mod.value(),
					&enum_res_type_callback::set<EnumResTypeHandler>::entry,
					reinterpret_cast<LONG_PTR>( &handler )
				);
	}
	
  	template< typename EnumResLangHandler >
	bool enum_resource_lang( module_handle mod, LPCTSTR type, res_id id, const EnumResLangHandler &handler )
	{
		return 
		FALSE!=::EnumResourceLanguages(
					mod.value(),
					type,
					id.get(),
					&enum_res_lang_callback::set<EnumResLangHandler>::entry,
					reinterpret_cast<LONG_PTR>( &handler )
				);		
	}

}} /* end of namespace oden.gammo */


#endif
