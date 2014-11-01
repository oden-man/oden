#pragma once

#ifndef ODEN_GAMMO_UI_CTL_PROGBAR
#define ODEN_GAMMO_UI_CTL_PROGBAR

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
//            �v���O���X�o�[ �R���g���[�� �N���X
//
//________________________________________________________________________________
//	
//################################################################
//  �R���g���[���N���X
//
	class progress_bar
		: public window
	{
	private:
		typedef progress_bar self;
	public:	
	// �R���X�g���N�^
		progress_bar( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�	  
      // �o�[�̓��̈ʒu
		// �ʒu���w�肷��
		int set_progress( int p )const;
		int progression()const;
		// �w�肵�����������i�߂�
		int advance()const;			// PBM_STEPIT
		int advance( int v )const;	// PBM_DELTAPOS
		int set_advance_step( int v )const;
		// ��t�ɂ���E��ɂ���
		void fill()const;
		void clear()const;

      // �v���O���X�o�[�͈̔�
		void set_range( int min, int max )const;
		void set_range_low( int min )const;
		void set_range_high( int max )const;
		std::pair<int,int> range()const;
		int range_low()const;
		int range_high()const;

	  // �X�^�C��
		// �o�[�̐F
		colorref set_bar_color( colorref c )const;
		colorref bar_color()const;
		// �w�i�F
		colorref set_back_color( colorref c )const;
		colorref back_color()const;
		// ��Ԃ�ݒ肷��i�����ڂ��ς��j
		void set_progress_state( int v )const;	// PBM_SETSTATE
		int progress_state()const;			// PBM_GETSTATE
		// �}�[�L�[���[�h�Ɉڂ�
		void start_marquee( int speed )const;
		void end_marquee()const;

      // class id
		static window_class_id get_control_class() { return window_class_id(PROGRESS_CLASS); }
	};	
	
}}
#endif
