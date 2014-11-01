#pragma once

#ifndef ODEN_GOB_GRA_GDI_DC
#define ODEN_GOB_GRA_GDI_DC

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
#include <oden/gammo/ui/window/event.h>
#include <oden/gammo/graphic/gdi/object.h>

//
//
//#################################################


namespace oden { namespace gammo
{
	class dc;
	class widget_object;
	
	typedef index dc_save_id;
	
//_______________________________________________________________________________________
//
//   �f�o�C�X�R���e�L�X�g
//_______________________________________________________________________________________
//
//################################################################
//  �萔�Ȃ�
//	
  // �u�����h�t�@���N�V����
	ODEN_STRUCT( blend_func, BLENDFUNCTION )
  	{
		struct get
		{};

		struct set
  		{
			Me& src_alpha( BYTE val ) 
			{ self::val(this).SourceConstantAlpha = val; return self::me(this); }

			Me& original_src_alpha() 
			{ self::val(this).AlphaFormat = AC_SRC_ALPHA; return self::me(this); }
		};
	};
	
//################################################################
// �f�o�C�X�R���e�L�X�g�E�Z�b�V�����ێ��N���X
//	
	class dc_object_session
	{
	private:
		typedef dc_object_session self;	
	public:
	// �R���X�g���N�^
		dc_object_session( const dc& dc_ref, gdi_obj_handle old )
			: pdc_( &dc_ref ), record_( old )
		{}
	// �����o�֐�
		void restore()const;
		
		typedef struct restorer {
			void operator()( self& obj )const { obj.restore(); }			
		} deleter_type;

	private:
		const dc* pdc_;
		gdi_obj_handle record_;
	};
	
	// �g�p��
	// scoped<dc_object_session> pensess( dc.select_pen( penn ) );

//################################################################
// �f�o�C�X�R���e�L�X�g�N���X
//
	class dc
		: public handle_interface<dc_handle>
	{
	private:
		typedef dc self;
	public:	
	// �R���X�g���N�^
		dc( dc_handle h = boost::none )
			: handle_interface<dc_handle>( h )
		{}
		
	// DC		
	  // �������
	  	bool release( window_handle wh )const;
	  // �폜����
	  	bool deletes();
	  	
	  // �֘A�t����ꂽ�E�B���h�E���擾
	  	window_handle assoc_window( )const;
	  	
	// GDI�I�u�W�F�N�g
	  // GDI�I�u�W�F�N�g�̎�芷��
		dc_object_session select_object	( const gdi_obj_handle& obj )const;
	    dc_object_session select_brush	( const brush_handle& br )const		{ return select_object( br.value() ); }
		dc_object_session select_font	( const font_handle& fo )const		{ return select_object( fo.value() ); }
		dc_object_session select_bitmap	( const bitmap_handle& bo )const	{ return select_object( bo.value() ); }
		
	  // DC�u���V�̐F���擾�E�ύX
	    colorref set_brush_color( colorref )const;
	    colorref brush_color( )const;
	    
	  // DC�y���̐F���擾�E�ύX
	    colorref set_pen_color( colorref )const;
	    colorref pen_color( )const;
	    
	  // �Z�b�V�������V�X�e���ŕۑ��E���ɖ߂�
	  	dc_save_id save( )const;
	  	bool restore( dc_save_id id )const;
		
	  // ���݂̃I�u�W�F�N�g���擾
	  
	// �r�b�g�}�b�v
	  // BitBlt
		bool bit_blt( 
			dc_handle 	 dest, 
			const rect&  dest_rect,
			const point& src_at,
			DWORD	     rop_code = SRCCOPY
		);
	  // StretchBlt
		bool stretch_blt( 
			dc_handle 	 dest, 
			const rect&  dest_rect,
			const rect&	 src_rect,
			DWORD	     rop_code = SRCCOPY
		);
	  // AlphaBlend
		bool alpha_blend( 
			dc_handle dest, 
			const rect& dest_rect,
			const rect&	src_rect,
			const blend_func::value_type& bfn 
		);
		
	// �`��
	  // �w�i�F�̎擾�E�ύX
	  	colorref set_back_color( colorref );
	  	colorref back_color( )const;
	  // �w�i���[�h
		positive set_back_mode( int );
		positive back_mode()const;
	  // �w�i none��TRANSPARENT
		void background( colorref );
		
	// �}�`
      // �u���V�œh��Ԃ����l�p�`��`��
		bool paint_rect		( const rect& rc, const brush_handle &br )const;
	  // �u���V�łP�s�N�Z���̃t���[����`��
		bool paint_frame		( const rect& rc, const brush_handle &br )const;
	  // �l�p�`��`��
		bool rectangle		( const rect& rc )const;
	  // �ȉ~��`��
		bool ellipse			( const rect& bound_rect )const;
	  // �p�̊ۂ��l�p�`��`��
		bool round_rect		( const rect& rc, const size& round_size )const;
	  // �_������
		bool  pixelv			( const point& pos, colorref col )const;
		colorref pixel			( const point& pos, colorref col )const;
		
	  // �t�H�[�J�X�̎l�p�`��`��
		void draw_focus_rect    ( const rect& rc )const;
		
	// �e�L�X�g�A�t�H���g
	  // �J�����g�̃t�H���g�ŕ���������
		bool text( string_ptr str, const point& pos )const;
		bool text( string_ptr str, const point& pos, UINT options, const rect& rc = rect(), memory_range<INT> distances = boost::none )const;
	  // 
		positive draw_text( string_ptr str, const rect& rc, UINT fmt = 0 )const;
	  
	  // �����F�ݒ�
	  	colorref set_text_color( colorref );
	  	colorref text_color( )const;
	  	
		//void current_text_metrics(TEXTMETRIC &Out);
		
	// �f���[�^�[�E�v���Z�b�g
	  // ���
		struct releaser {
			void operator()( dc& o )const { o.release( o.assoc_window() ); }
		};
	  // �폜
		struct deleter {
			void operator()( dc& o )const { o.deletes(); }
		};
	};

	class shared_dc : public dc
	{
	public:
		shared_dc( dc_handle h = boost::none ) : dc(h) {}
		typedef dc::releaser deleter_type;
	};

	class new_dc : public dc
	{
	public:
		new_dc( dc_handle h = boost::none ) : dc(h) {}
		typedef dc::deleter deleter_type;
	};
	
  // WM_PAINT���Ŏg�p
	class paint_dc : public dc
	{
	public:
	// �R���X�g���N�^
		paint_dc(){}
		paint_dc( window_handle h ){ this->begin(h); }
		paint_dc( widget_object* w );
		
		typedef struct ender {
			void operator()( paint_dc& o ){ o.end(); }
		} deleter_type;
		
	// �����o�֐�
	  // �J�n�E�I��
		dc_handle begin( window_handle w );
		void end();
		
	  // ���擾
		dc_handle dc()const   { return ps_.hdc; }
		rect area()const      { return gammo::make_rect( ps_.rcPaint ); }
		bool is_bg_erased()const { return ps_.fErase!=0; }
		
	  // �I�[�o�[���C�h
		window_handle assoc_window()const { return w_; }
		
	private:
		window_handle w_;
		PAINTSTRUCT ps_;
	};
	
	// scoped<paint_dc> pdc(h); // �኱�̃R�s�[�R�X�g�@�܂����ɂ͂Ȃ�Ȃ����낤���E�E�E

	// scoped<paint_dc> pdc; // �R�X�g�Ȃ��I
	// pdc->begin(h);
	
//_______________________________________________________________________________________
//
//   �\�z�֐�
//_______________________________________________________________________________________
//
//#####################################################################
// �擾 -> releaser
//
  // �f�X�N�g�b�v�̃f�o�C�X�R���e�L�X�g
	shared_dc  get_desktop_dc( );

  // �v���C�}���f�B�X�v���C�̃f�o�C�X�R���e�L�X�g
	shared_dc  get_display_dc( );
	
  // Window�� �N���C�A���g�G���A�̃f�o�C�X�R���e�L�X�g���擾
	shared_dc  get_client_dc( window_handle wh );
	
  // Window�S�̂̃f�o�C�X�R���e�L�X�g���擾
	shared_dc  get_window_dc( window_handle wh );

	// scoped<shared_dc> dc( get_desktop_dc() );
	
//#####################################################################
// �쐬 -> deleter
//
  // ������DC���쐬
	new_dc  create_memory_dc( dc_handle dh );

	// scoped<new_dc> dc( create_memory_dc( desktopdc_ ) );
	
//_______________________________________________________________________________________
//
//   DC �C�x���g
//_______________________________________________________________________________________
//
// ���O
//
namespace event 
{
	typedef event_def< event_code<WM_PAINT> >        paint;
	typedef event_def< event_code<WM_SYNCPAINT> >    sync_paint;
	typedef event_def< event_code<WM_NCPAINT> >      nc_paint;
}

//
// ����
//

	
}}

#endif
