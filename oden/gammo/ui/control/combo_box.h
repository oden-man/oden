#pragma once

#ifndef ODEN_GAMMO_UI_CTL_COMBOBOX
#define ODEN_GAMMO_UI_CTL_COMBOBOX

//#######################################################
//  �C���N���[�h
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden ----------
//----------- gammo ----------
#include <oden/gammo/ui/control/general.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//           �R���{�{�b�N�X�R���g���[���N���X
//________________________________________________________________________________
//
//################################################################
//  �R���g���[��
//
	class combo_box_base
		: public window
	{
	public:	
	// �R���X�g���N�^
		combo_box_base( const window_handle& wh )
			: window( wh )
		{}
		
	// �����o�֐�
	    // �e�L�X�g���폜
		bool erase( index at )const;
		// �A�C�e���֘A�f�[�^
		LPARAM item_data( index i )const;
		bool set_item_data( index i, LPARAM data )const;
		// �A�C�e����
		int count()const;
	    // �S�ăN���A
		void clear()const;
	    // �󂩂ǂ���
		bool empty()const{ return count()==0; }
		
	    // �w�蕔����I��
		bool select( index i )const;
		void deselect()const;	    
		// �I���ꏊ���擾
	  	index selection()const;

	    // �ő啶�����̐�����݂���
	  	void set_edit_limit( int num )const;
		// �������m�ۂ�\�ߍs���E�m�ۂ����A�C�e������Ԃ�
		integer reserve( int add_item_num, int string_memory_size )const;

	  	// ���X�g�A�C�e������
	  	integer item_height()const;
	  	integer item_height( index i )const;		// ownerdraw variable �̂�
	  	bool set_item_height( int h )const;
	  	bool set_item_height( index i, int h )const;	// ownerdraw variable �̂�
		// selection field �̍���
	  	integer selection_height()const;
	  	bool set_selection_height( int h )const;

		// �h���b�v�_�E��
		void show_drop_down(bool b = true)const;
		// �h���b�v���X�g�������Ă��邩
		bool drop_downed()const;
		
	  // XP later
	#if (_WIN32_WINNT >= 0x0501)		
		// ���X�g�̃X�N���[���Ȃ��ŕ\������A�C�e���̍ő吔���擾�E�Z�b�g
		INT visible_item_limit()const;
		bool set_visible_item_limit(INT)const;
	#endif
	
	  // Vista later
	#if (_WIN32_WINNT >= 0x0600)
		// �G�f�B�b�g�R���g���[���ɏo�锖����������擾�E�Z�b�g
		bool get_cue_banner(wide_string&)const;
		wide_string cue_banner()const;
		bool set_cue_banner(wide_string_ptr)const;
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_COMBOBOX); }
	};	

//################################################################
//  �ʏ�̃R���{�{�b�N�X ( ������̃A�C�e��)
//
	class combo_box
		: public combo_box_base
	{
	public:	
	// �R���X�g���N�^
		combo_box( window_handle wh = boost::none )
			: combo_box_base( wh )
		{}
		
	// �����o�֐�
	  // ���X�g�A�C�e���̑���
	    // �e�L�X�g�𖖔��ɒǉ�
		void add( string_ptr text )const;
		// �A�C�e����}��
		index insert( index at, string_ptr text )const;
		// �e�L�X�g������
		index find_exact( string_ptr text, index start = boost::none )const;
		index find( string_ptr text, index start = boost::none )const;
		// �A�C�e�����擾
		bool get_item_text( index i, string& out )const;
		string item_text( index i )const;
		// �e�L�X�g�������擾
		size_t item_text_length( index i )const;
		// �������đI��
		index find_and_select( string_ptr text, index start = boost::none )const;
	};

//################################################################
//  �I�[�i�[�h���[�ŁA������������Ȃ��R���{�{�b�N�X
//
	class data_combo_box
		: public combo_box_base
	{
	public:	
	// �R���X�g���N�^
		data_combo_box( window_handle wh = boost::none )
			: combo_box_base( wh )
		{}
		
	// �����o�֐�
	  // ���X�g�A�C�e���̑���
	    // �e�L�X�g�𖖔��ɒǉ�
		void add( LPARAM data )const;
		// �A�C�e����}��
		index insert( index at, LPARAM data )const;
		// �e�L�X�g������
		index find( LPARAM data, index start = boost::none )const;
		// �������đI��
		index find_and_select( LPARAM data, index start = boost::none )const;
	};

//################################################################
//  �R���{�{�b�N�X�̃C�x���g
//
namespace event
{
	typedef command_def< event_code< CBN_SELCHANGE > >	combo_box_sel_change;
	typedef command_def< event_code< CBN_EDITCHANGE > >	combo_box_edit_change;
	typedef event_def_or< 
				combo_box_sel_change, 
				combo_box_edit_change > 		combo_box_text_change;

	typedef command_def< event_code< CBN_DROPDOWN > >	combo_box_drop_down;
	typedef command_def< event_code< CBN_CLOSEUP > >	combo_box_close_up;
}

  // �G�C���A�X
	//template<> struct event_alias< event::drop_down, combo_box > { typedef event::combo_box_drop_down type; };
	//template<> struct event_alias< event::close_up, combo_box > { typedef event::combo_box_close_up type; };
	template<> struct event_alias< event::sel_change, combo_box > 	{ typedef event::combo_box_sel_change type; };
	template<> struct event_alias< event::change, combo_box > 		{ typedef event::combo_box_text_change type; };


}}
#endif
