#pragma once

#ifndef ODEN_GAMMO_UI_DLG_DLG
#define ODEN_GAMMO_UI_DLG_DLG

//###############################################
//
//
// --------- def -----------
#include <oden/gammo/defs.h>
// --------- win -----------
// --------- oden ----------
#include <oden/gammo/ui/window/window.h>

//
//
//#################################################


namespace oden { namespace gammo
{	
//##############################################################
// �_�C�A���O�{�b�N�X�̃N���X
//
	class dialog
		: public window
	{
	private:
		typedef dialog self;
	public:
	// �R���X�g���N�^
		dialog( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �_�C�A���O���쐬
		bool create( res_id dlg_template, window_handle parent, LPARAM init_param = 0, instance_handle ih = boost::none );
	  
	  // ���[�_���_�C�A���O���J�n
	  	dlg_result do_modal( res_id dlg_template, window_handle parent, LPARAM init_param = 0, instance_handle ih = boost::none )const;
	  // ���[�_�����I��
	  	bool end_modal( dlg_result Ret )const;
	  	
	  // �_�C�A���O�ނ��̃��b�Z�[�W����������
	  	bool process_dlg_message( raw_window_message& out )const;
	  	
	  // �A�C�e�����擾
	  	window_handle get_item( int i )const;
	  	window item( int i )const;
	  	
	  // �A�C�e���̕������擾
	  	bool get_item_text( int item, string &out )const;
	  	string item_text( int item )const;
	  	
	  // �A�C�e���̕�����ݒ�
	  	bool set_item_text( int item, string_ptr text )const;

	  // �A�C�e���̕����𐔒l�Ƃ��Ď擾
	  	UINT item_value( int item, bool s = false, bool* pbool = 0 )const;
		template< typename T >
	  	bool get_item_value( int i, bool s, T& out )const
		{
			bool ret = false;
			out = static_cast<T>( item_value( i, s, &ret ) );
			return ret;
		}

	  // �A�C�e���ɐ��l�𕶎��ɂ��Đݒ�
	  	bool set_item_value( int item, UINT val, bool s = false )const;
	  	
	  // �O���[�v�A�C�e�����擾
	  	window_handle prior_group_item( window_handle begin = boost::none )const;
	  	window_handle next_group_item( window_handle begin = boost::none )const;
	  	
	  // �^�u�O���[�v�A�C�e�����擾
	  	window_handle prior_tab_item( window_handle begin )const;
	  	window_handle next_tab_item( window_handle begin )const;
	  	
	  // �_�C�A���O�P�ʂ̍��W
		static LONG base_unit();
	  	bool base_unit_to_screen( rect& out )const;

	  // WindowLong
		DLGPROC set_dialog_procedure_ptr( DLGPROC )const;
		LRESULT set_message_result( LRESULT )const;
		LONG_PTR set_dialog_data( LONG_PTR )const;

		DLGPROC dialog_procedure_ptr()const;
		LRESULT message_result()const;
		LONG_PTR dialog_data()const;
	};

//##############################################################
// �_�C�A���O�{�b�N�X�̃C�x���g
//
namespace event {

  /* WM_INITDIALOG */
	typedef event_def<event_code<WM_INITDIALOG> > dialog_init;

  /* ���̑� �����Ȃ� */

}

//
// ��`
//	
	template<> // focused ctl / lparam
	struct event_arg<event::dialog_init> : 
		event::result_< event::bool_<> >, event::wparam_lparam< event::value_object<window_handle>, event::value<LPARAM> >
	{};

}}

#endif