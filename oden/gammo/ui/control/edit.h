#pragma once

#ifndef ODEN_GAMMO_UI_CTL_EDIT
#define ODEN_GAMMO_UI_CTL_EDIT

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
	class edit;

//________________________________________________________________________________
//
//
//            �G�f�B�b�g �R���g���[�� �N���X
//
//________________________________________________________________________________
//	
//################################################################
//  �o���[���`�b�v�̈���
//	
#if (_WIN32_WINNT >= 0x0501) /* XP later */

	ODEN_STRUCT( edit_balloon_tip, EDITBALLOONTIP )
  	{
		struct get
  		{};
		
		struct set
  		{
		  // �^�C�g��
			Me& title( wide_string_ptr str ) {		
				self::val(this).pszTitle = str.c_str();
				return self::me(this);
			}
		  // �{��
			Me& text( wide_string_ptr str ) {
				self::val(this).pszText = str.c_str();
				return self::me(this);
			}
		  // �A�C�R��
	  		Me& icon( INT type ) {
				self::val(this).ttiIcon = type;
				return self::me(this);
			}
		};
		
		struct def
		{ 
			def( value_type& out )
			{
				out.cbStruct = self::value_size();
			}
		};
	};
	
#endif
	
//################################################################
//  �R���g���[���N���X
//
	class edit
		: public window
	{
	private:
		typedef edit self;
	public:	
	// �R���X�g���N�^
		edit( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �e�L�X�g����
	    // �e�L�X�g�𖖔��ɒǉ�
		void append( string_ptr text )const;
	    // �e�L�X�g��}��
		void insert( index at, string_ptr text )const;
	    // �e�L�X�g��ݒ�
		void set( string_ptr text )const;
		//void send( string_ptr text )const;
		// �擾
		void get( string& out )const { get_text(out); }
		// �u��
		void replace( index first, index last, string_ptr text )const;
	    // �I�����ꂽ������u��
		void replace_selected( string_ptr text )const;
	    // �󂩂ǂ���
		bool empty( )const;
	    // �e�L�X�g�����S�ɍ폜
		void erase_all( )const;
		// �\�t�g���s�����̈���
		void insert_soft_line_break( )const;
		void remove_soft_line_break( )const;

	  // ���C��
		class line {
		public:
		// �����o�֐�
			// �e�L�X�g�̎擾
			string text()const;
			bool get_text( string& out )const;
			// �������擾
			int length()const;
			// �s�̑S��/�ꕔ��u������
			void set( string_ptr str )const;
			void replace( index b, index e, string_ptr str )const;
			// �s�S�̂��폜����
			void erase_all()const;
			// �s��I��
			void select_all()const;
			void select( index b, index e )const;
			void select( index_range r )const { this->select( r.begin(), r.end() ); }
			// �s�̐擪�I�t�Z�b�g
			index begin_pos()const;
			index tail_pos()const;
			index end_pos()const;
			// �s�̃I�t�Z�b�g
			index_range pos()const;
			// �擾
			index get_index()const { return i_; }
			edit owner()const;
			bool fail()const { return owner_.is_null() || i_.is_null(); }
			
		// �R���X�g���N�^
			line( window_handle ow, index i );
		private:
			window_handle owner_; index i_;
		};
		
	  // �I���ʒu�E�L�����b�g�ʒu	  
	    // �w�蕔����I��
		void select( index first, index last )const; 
		void select( index_range rng )const { this->select( rng.begin(), rng.end() ); }
		void select_all( )const;
		void deselect()const;
	    // �w�蕔�����擾
		index_range selection()const;
		// �J�[�\���ʒu���w��
		void set_cursor_pos( index ichar )const;
		void set_cursor_home()const;
		void set_cursor_end()const;
		// �L�����b�g�ʒu���擾
		index cursor_pos()const;
		// �ʒu
		index home_pos()const;
		index tail_pos()const;
		index end_pos()const;
		//  ���W����I�t�Z�b�g���擾( 16bit�܂� )
		index char_from_point( const point& )const;	// ����
		edit::line line_from_point( const point& )const;	// ���C��
		std::pair<index, index> offset_from_point( const point& )const;  // ����
		// �I�t�Z�b�g������W�𓾂�
		boost::optional< point > point_from_char( index ichar )const;
		
	  // �����s�̑���
	    // �s�̎擾
		edit::line line_at( index iline )const;
	    // �s�e�L�X�g�̎擾
		string line_text( index iline )const;
	  	bool get_line_text( index iline, string& out )const;
		// ���݃J�[�\���̂���s���擾
		edit::line cursor_line()const;
		// �����I�t�Z�b�g����s���擾
		edit::line line_from_char( index ichar )const;
		// �����I�t�Z�b�g����܂܂��s�̒������擾
		int line_length_from_char( index ichar )const;
	    // �s���̎擾
	  	int count_line( )const;
		// �ŏ��̖ڂɌ����镶�����܂ލs���擾
		edit::line first_visible_line( )const;
		index first_visible_char( )const;
		
	  // �����̐���
	    // ���[�h�I�����[�̐؂�ւ�
		bool read_only( bool on )const;
	    // �p�X���[�h�}�X�N�����̐ݒ�
	  	void use_password_mask( char_t ch )const;
		boost::optional< char_t > password_mask_char( )const;
	    // �ő啶����( �o�C�g�� )�̐���
	  	void set_text_limit( size_t num )const;
		size_t text_limit( )const;
		
	  // ���C�A�E�g
		// �}�[�W�����擾
		int left_margin_width( )const;
		int right_margin_width( )const;
		std::pair<int,int> margin_width( )const;
		// �e�L�X�g�g�l�p�`
		rect formatting_rect( )const;
		void set_formatting_rect( const rect&, bool repaint=true )const;
		
	  // �R���g���[���̑���
	    // ���[�U�[�ɂ��ύX�̃t���O
		bool is_modified( )const;
		void set_modify_flag( bool bflag )const;
		// �X�N���[��
		bool scroll( int vert, int horiz = 0 )const;
		uinteger scroll_one( int flags )const;
		void scroll_to_caret()const;
	    // �A���h�D�E�A���h�D�\�ȍ폜
	  	bool undo( )const;
		bool can_undo( )const;
	  	void clear_selected( )const;
		void clear( )const;
	    // �N���b�v�{�[�h�֘A
	  	void copy_to_clipboard( )const;
  		void cut_to_clipboard( )const;
  		void paste_from_clipboard( )const;
		
	  // XP later
	#if (_WIN32_WINNT >= 0x0501)
		// �L���[�o�i�[���Z�b�g�E�擾 ( multiline�ł͎��s����)
		bool get_cue_banner( wide_string& out )const;
		wide_string cue_banner( )const;
		bool set_cue_banner( wide_string_ptr text )const;
		bool set_cue_banner_focused( wide_string_ptr text )const;
		// �o���[���`�b�v
		bool hide_balloon_tip( )const;
		bool show_balloon_tip( edit_balloon_tip::value_type& )const;		
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_EDIT); }
	};

//#############################################################################
//  �C�x���g
//
  // edit_color
	GAMMO_DEFINE_EVENT( event_def, edit_color, WM_CTLCOLOREDIT, 2 )
	
	template<>
	struct event_arg<event::edit_color> :
		event::result_< event::value_object<brush_handle> >, 
		event::wparam_lparam< event::value_object<dc_handle>, event::value_object<window_handle> > 
	{};
	
	template<>
	struct event_branch<event::edit_color>
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& ctl )
		{
			window w = get_event_arg<event::edit_color, event::_2>( ev );
			return branch(ev) && branch_child_id( w.child_id(), ctl );
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_CTLCOLOREDIT;
		}
	};
	
	template<> 
	struct event_alias<event::color, edit>  { typedef event::edit_color type; };
	
  // ���̑��̒ʒm
	GAMMO_DEFINE_NOTIFY_EVENT( command_def, edit_change, EN_CHANGE )
	GAMMO_DEFINE_NOTIFY_EVENT( command_def, edit_update, EN_UPDATE )

  // �G�C���A�X����
	template<> 
	struct event_alias<event::change, edit> { typedef event::edit_change type; };

	
}}
#endif
