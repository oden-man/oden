#pragma once

#ifndef ODEN_GAMMO_UI_CTL_BTN
#define ODEN_GAMMO_UI_CTL_BTN

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
//
//            �{�^�� �R���g���[�� �N���X
//
//________________________________________________________________________________
//	
//################################################################
// 
//	
#if (_WIN32_WINNT >= 0x0501)

	ODEN_STRUCT( button_image_list, BUTTON_IMAGELIST )
  	{ 
		struct get
  		{};
		
		struct set
  		{
			Me& image_list( image_list_handle imh ) 
			{ self::val(this).himl = imh.handle_value(); return self::me(this); }
			Me& margin( const rect& ico_ma ) 
			{ ico_ma.get_rect( self::val(this).margin ); return self::me(this); }
	  		Me& alignment( int type ) 
			{ self::val(this).uAlign = type; return self::me(this); }
		};
	};
	
#endif
	
//################################################################
//  �R���g���[���N���X
//
	class button
		: public window
	{
	private:
		typedef button self;
	public:	
	// �R���X�g���N�^
		button( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	    // �`�F�b�N��Ԃ�m��
		bool checked()const;
		bool unchecked()const;
		bool indeterminate()const;
	    // �`�F�b�N��Ԃ�ݒ�
	  	void check()const; 
	  	void uncheck()const; 
		void set_indeterminate()const;
		void check( bool )const;		
	  	// �{�^���̏��
	  	DWORD state()const;
	  	// �n�C���C�g���
	  	void set_highlight()const;
	  	void remove_highlight()const;
	  	
	  /*** XP later ***/
	  	// �{�^���e�L�X�g�̃}�[�W�� ( comctl 6.0 )
	  	bool get_text_margin( rect& out )const;
	  	rect text_margin()const;
	  	bool set_text_margin( const rect& mag )const;
	    // �C���[�W�\���ɍœK�ȃT�C�Y  ( comctl 6.0 )
	  	bool get_ideal_size( size& out )const;
	  	size ideal_size()const;
	    // �C���[�W���X�g (comctl6.0)
	    bool set_image_list( const button_image_list::value_type& pa )const;
	    bool get_image_list( button_image_list::value_type& out )const;
	
	  /*** Vista later ***/
		// �Ǘ��Ҍ����A�C�R�� ( comctl 6.0 )
		bool add_elevated_icon()const;
		bool remove_elevated_icon()const;	
		// �R�}���h�����N�{�^���̃m�[�g����
		bool get_note( wide_string& out )const;
		wide_string note()const;
		bool set_note( wide_string_ptr s )const;
		// �m�[�g��������
		size_t note_length()const;	

	// ���̑�
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_BUTTON); }
	  // �ŗǂ̃T�C�Y
		void calc_best_size(size& out)const;
	};

//###########################################################
//   �C�x���g��`
//	
GAMMO_DEFINE_EVENT( event_def, button_color, WM_CTLCOLORBTN, 2 )
	
	template<>	// button dc / handle
	struct event_arg<event::button_color> :
		event::result_< event::value_object<brush_handle> >, 
		event::wparam_lparam< event::value_object<dc_handle>, event::value_object<window_handle> >
	{};

	template<> // ��p�̃u�����`����
	struct event_branch<event::button_color>
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& ctl )
		{
			return branch(ev) && branch_child_id( window(reinterpret_cast<HWND>(ev.lparam())).child_id(), ctl );
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_CTLCOLORBTN;
		}
	};
	
	template<> 
	struct event_alias<event::color, button> { typedef event::button_color type; };
	
	
GAMMO_DEFINE_NOTIFY_EVENT( command_def, button_click, BN_CLICKED )
	
	namespace event { typedef button_click button; }
	
	template<> 
	struct event_alias<event::click, button> { typedef event::button_click type; };
	

GAMMO_DEFINE_NOTIFY_EVENT( command_def, button_double_click, BN_DBLCLK )
	
	template<> 
	struct event_alias<event::double_click, button> { typedef event::button_double_click type; };
	
	
	
}}
#endif