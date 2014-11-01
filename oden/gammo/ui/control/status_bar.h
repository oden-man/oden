#pragma once

#ifndef ODEN_GAMMO_UI_CTL_STABAR
#define ODEN_GAMMO_UI_CTL_STABAR

//#######################################################
//  �C���N���[�h
//

//-------- defs -------------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden -----------
//----------- gammo ---------
#include <oden/gammo/ui/control/general.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            �X�e�[�^�X�o�[ �R���g���[�� �N���X
//
//________________________________________________________________________________
//	
//################################################################
//  �R���g���[���N���X
//
	class status_bar
		: public window
	{
	private:
		typedef status_bar self;
	public:	
	// �{�[�_�[�������L�^����N���X
		class borders {
		public:
			borders(){ memset(vals_, 0, 3); }

			int horizontal()const{ return vals_[0]; }
			int vertical()  const{ return vals_[1]; }
			int between()   const{ return vals_[2]; }
			int* get_ptr(){ return vals_; }
		private:
			int vals_[3];
		};
	
	// �R���X�g���N�^
		status_bar( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �p�[�c�e�L�X�g
		BYTE get_part_text( index at, string& out )const;
		string part_text( index at )const;		
		bool set_part_text( index at, string_ptr str, int flags = 0 )const;
		
	  // �p�[�c�e�L�X�g�̒����A�X�^�C�����擾
		int part_text_length( index at )const;
		std::pair<int,int> part_text_info( index at )const;	// first : length, second : style SBT_ 

	  // �p�[�c���I�[�i�[�h���[����
		bool set_owner_draw_part( index at, LPARAM data = 0, int flags = 0 )const;

      // �p�[�c�̃c�[���`�b�v�̃e�L�X�g
		void get_part_tip_text( index at, string& out )const;
		string part_tip_text( index at )const;
		void set_part_tip_text( index at, string_ptr str )const;
		
	  // �p�[�c�A�C�R��
		icon_handle get_part_icon( index at )const;
		bool set_part_icon( index at, icon_handle ih )const;	// : none ����ŏ����ł���
		
	  // �p�[�c���敪������
		bool set_part_layout( memory_range<int> sizes )const;
		void get_part_layout( std::vector<int> &out )const;
		int count_part()const; // �p�[�c�̐�

	  // �p�[�c�̑傫��
		bool get_part_rect( index at, rect& out )const;
		rect part_rect( index at )const;

	  // �V���v�����[�h
		void set_simple_mode( bool b = true )const;
		bool is_simple_mode()const;
	  
	  // �w�i�F
		colorref set_back_color( colorref col )const;

	  // ���C�A�E�g
		void set_min_height( int h )const;
		bool get_border_width( status_bar::borders& out )const;
		status_bar::borders border_width()const;
		
	  // Unicode
		bool use_unicode_format( bool b = true )const;
		bool is_unicode_format()const;

      // class id
		static window_class_id get_control_class() { return window_class_id(STATUSCLASSNAME); }
	};
	
}}
#endif
