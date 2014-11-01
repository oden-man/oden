#pragma once

#ifndef ODEN_GAMMO_UI_CTL_LISTBOX
#define ODEN_GAMMO_UI_CTL_LISTBOX

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
//           ���X�g�{�b�N�X�R���g���[���N���X
//________________________________________________________________________________
//
//################################################################
//  �R���g���[��
//
	class list_box_base
		: public window
	{
	public:	
	// �R���X�g���N�^
		list_box_base( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �A�C�e���̑���i���ʁj
	    // �e�L�X�g���폜
		bool erase( index at )const;
		// �֘A�f�[�^
		LPARAM item_data( index i )const;
		bool set_item_data( index i, LPARAM data )const;
		// �A�C�e����
		int count()const;
	    // �S�ăN���A
		void clear()const;
	    // �󂩂ǂ���
		bool empty()const{ return count()==0; }
		// ��ԏ�̃A�C�e��
		index top()const;
		bool set_top( index at )const;
		// �}���`�J�������[�h�̏ꍇ�A�J����������̃A�C�e���̐���Ԃ�
		int count_column_item()const;
		
	  // �A�C�e����I��
	    // ���I��
		bool select( index i )const;
		void deselect()const;
		// �I������Ă��邩�H
		bool selected( index at )const;
		// �I���ꏊ���擾
	  	index selection()const;
		// �I�𐔂��擾
		int count_selection()const;
		// �����̑I���ӏ���S�Ď擾
		integer get_selections( buffer_range<index> out )const;
		std::vector<index> selections()const;
		// ����͈͂��w�肵�I��
		bool select( index beg, index end )const;
		bool deselect( index beg, index end )const;
		// �����I���̊J�n�ӏ�
		index anchor()const;
		bool set_anchor( index at )const;

	  // ���C�A�E�g
	  	// �A�C�e������
	  	integer item_height()const;
	  	integer item_height( index i )const;		// ownerdraw variable �̂�
	  	bool set_item_height( int h )const;
	  	bool set_item_height( index i, int h )const;	// ownerdraw variable �̂�
		// selection field �̍���
	  	integer selection_height()const;
	  	bool set_selection_height( int h )const;	
		// �A�C�e���̑傫��
		bool get_item_rect( index at, rect &out )const;
		rect item_rect( index at )const;

      // �R���g���[���̑���
		// �������m�ۂ�\�ߍs���E�m�ۂ����A�C�e������Ԃ�
		integer reserve( int add_item_num, int string_memory_size )const;
		// �A�C�e���̃L�����b�g
		index focused()const;
		bool set_focus( index at )const;
		// �J�����g�f�B���N�g���̃t�@�C����񋓂��Ēǉ�����
		bool add_file_list( string_ptr files, UINT attrs = DDL_READWRITE )const;


	  // XP later
	#if (_WIN32_WINNT >= 0x0501)
	#endif
	
	  // Vista later
	#if (_WIN32_WINNT >= 0x0600)
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_LISTBOX); }
	};	

//################################################################
//  �ʏ�̃R���{�{�b�N�X ( ������̃A�C�e��)
//
	class list_box
		: public list_box_base
	{
	public:	
	// �R���X�g���N�^
		list_box( window_handle wh = boost::none )
			: list_box_base( wh )
		{}
		
	// �����o�֐�
	  // ���X�g�A�C�e���̑���
	    // �e�L�X�g��ǉ�
		index add( string_ptr text )const;
		index add( string_ptr text, LPARAM param )const;
		// �A�C�e����}��
		index insert( index at, string_ptr text )const;
		index insert( index at, string_ptr text, LPARAM param )const;
		// �A�C�e����ݒ�
		bool set( index at, string_ptr text )const;
		bool set( index at, string_ptr text, LPARAM param )const;
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
	class data_list_box
		: public list_box_base
	{
	public:	
	// �R���X�g���N�^
		data_list_box( window_handle wh = boost::none )
			: list_box_base( wh )
		{}
		
	// �����o�֐�
	  // ���X�g�A�C�e���̑���
	    // �e�L�X�g��ǉ�
		index add( LPARAM data )const;
		// �A�C�e����}��
		index insert( index at, LPARAM data )const;
		// �A�C�e����ݒ�
		bool set( index at, LPARAM data )const;
		// �e�L�X�g������
		index find( LPARAM data, index start = boost::none )const;
		// �������đI��
		index find_and_select( LPARAM data, index start = boost::none )const;

      // �R���g���[��
		// �A�C�e���̐������߂Đݒ肷��
		bool set_item_count( int num )const;
	};

//################################################################
//  �C�x���g
//
namespace event
{
	typedef command_def< event_code< LBN_SELCHANGE > >	list_box_sel_change;
	typedef command_def< event_code< LBN_SELCANCEL > >	list_box_sel_cancel;
	typedef command_def< event_code< LBN_ERRSPACE > >	list_box_error_space;
	typedef command_def< event_code< LBN_DBLCLK > >		list_box_double_click;
}

  // �G�C���A�X
	template<> struct event_alias< event::sel_change, list_box > 	{ typedef event::list_box_sel_change type; };
	template<> struct event_alias< event::double_click, list_box > { typedef event::list_box_double_click type; };


	
}}
#endif
