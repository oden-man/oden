#pragma once

#ifndef ODEN_GOB_GRA_GDI_FONT
#define ODEN_GOB_GRA_GDI_FONT

//#####################################################
//  �C���N���[�h
//


// --------- defs -----------
#include <oden/gammo/defs.h>

// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
#include <oden/chikuwa/std_funcs.h>
// --------- gammo -----------
#include <oden/gammo/graphic/gdi/object.h>

//
//
//#################################################



namespace oden { namespace gammo
{

//_______________________________________________________________________________________
//
//�@�@GDI�I�u�W�F�N�g�E�t�H���g
//_______________________________________________________________________________________
//
//###############################################
// �_���t�H���g�̃f�[�^
//
	ODEN_STRUCT( log_font, LOGFONT )
	{
		struct set
		{
		// �����o�֐�
		  // �T�C�Y
			Me& height( int h ) { 
				return dimension(0,h); 
			}
			Me& dimension( const size& dim ) { 
				return dimension( dim.width(), dim.height() ); 
			}	
			Me& dimension( int w, int h ) {
				self::val(this).lfWidth = w;
				self::val(this).lfHeight = h;	
				return self::me(this);
			}
		  // ����
			Me& weight( DWORD val ) {
				self::val(this).lfWeight = val;
				return self::me(this);	
			}
		  // ����
			Me& italic() { 
				self::val(this).lfItalic = TRUE; 
				return self::me(this); 
			}
			Me& underlined() { 
				self::val(this).lfUnderline = TRUE; 
				return self::me(this); 
			}
			Me& strike_out() { 
				self::val(this).lfStrikeOut = TRUE; 
				return self::me(this); 
			}
		  // �����Z�b�g
			Me& charset( BYTE val ) {
				self::val(this).lfCharSet = val;
				return self::me(this);	
			}
		  // �o�͂̐��x
			Me& out_precision( BYTE val ) {
				self::val(this).lfOutPrecision = val;
				return self::me(this);	
			}
		  // �N���b�s���O�̐��x
			Me& clip_precision( BYTE val ) {
				self::val(this).lfClipPrecision = val;
				return self::me(this);	
			}
		  // �o�͂̕i��
			Me& quality( BYTE val ) {
				self::val(this).lfQuality = val;
				return self::me(this);	
			}
		  // �s�b�`
			Me& pitch( BYTE val ) {
				self::val(this).lfPitchAndFamily |= val;
				return self::me(this);
			}
		  // �t�@�~��
			Me& family( BYTE val ) {
				self::val(this).lfPitchAndFamily |= val;
				return self::me(this);	
			}
		  // �t�H���g��
			Me& name( string_ptr str ) {
				chikuwa::str::copy( self::val(this).lfFaceName, /*sizeof(self::val(this).lfFaceName)/sizeof(TCHAR),*/ str.c_str() );
				return self::me(this);
			}
		  // ���̑�
			Me& escapement( int val ) {
				self::val(this).nEscapement = val;
				return self::me(this);	  		
			}
			Me& orientation( int val ) {
				self::val(this).nOrientation = val;
				return self::me(this);	  		
			}
		};

		struct def
		{
			def( value_type& out )
			{
				out.lfWeight		= FW_DONTCARE;
				out.lfItalic		= FALSE;
				out.lfUnderline		= FALSE;
				out.lfStrikeOut		= FALSE;
				out.lfCharSet		= DEFAULT_CHARSET;
				out.lfOutPrecision	= OUT_TT_ONLY_PRECIS;
				out.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
				out.lfQuality		= PROOF_QUALITY;
				out.lfPitchAndFamily= FIXED_PITCH | FF_MODERN;
			}
		};
	};

//##############################################
// �\�z�֐�
//
  // �_���t�H���g
	font_handle create_font( const log_font::value_type& lof );

  // �V�X�e���ɔ������Ă���t�H���g
	font_handle get_stock_font( int id );
	
}}

#endif