#pragma once

#ifndef ODEN_GAMMO_UI_CTL_STATIC
#define ODEN_GAMMO_UI_CTL_STATIC

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
//            �X�^�e�B�b�N �R���g���[�� �N���X
//
//________________________________________________________________________________
//	
//################################################################
//  �R���g���[���N���X
//
	class static_ctl
		: public window
	{
	private:
		typedef static_ctl self;
	public:	
	// �R���X�g���N�^
		static_ctl( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �ݒ�
		icon_handle 		set_icon		( icon_handle ih )const;
		bitmap_handle 		set_bitmap		( bitmap_handle bh )const;
		cursor_handle 		set_cursor		( cursor_handle ch )const;
		enhmetafile_handle 	set_enhmetafile	( enhmetafile_handle emfh )const;
	  // �擾
	    icon_handle 		icon			()const;
	    bitmap_handle 		bitmap			()const;
	    cursor_handle 		cursor			()const;
	    enhmetafile_handle 	enhmetafile		()const;

      // class id
		static window_class_id get_control_class() { return window_class_id(WC_STATIC); }
	};	
	
}}
#endif
