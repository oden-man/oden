#pragma once

#ifndef ODEN_GOB_GRA_GDI_BRUSH
#define ODEN_GOB_GRA_GDI_BRUSH

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// ---------- stl ------------
// ---------- win ------------
// ---------- boost -----------
// ---------- oden -----------
// --------- gammo -----------
#include <oden/gammo/graphic/gdi/object.h>

//
//
//#################################################



namespace oden { namespace gammo
{
	class rgb8;
//_______________________________________________________________________________________
//
//   GDI�I�u�W�F�N�g�E�u���V
//_______________________________________________________________________________________
//  	
//##############################################
// ���\����
//
	ODEN_STRUCT( log_brush, LOGBRUSH )
	{
		struct get
		{};

		struct set
		{
		  // �X�^�C��
			Me& style( UINT sty ) {
				self::val(this).lbStyle = sty;
				return *this;
			}
		  // �F
			Me& color( colorref c ) {
				self::val(this).lbColor = c.value();
				return *this;
			}
		  // �n�b�`�p�^�[��
			Me& hatch_pattern( int hat ) {
				self::val(this).lbHatch = hat;
				return *this;			
			}
		};
 	};
	
//##############################################
// �C���^�[�t�F�[�X
//
  // �u���V�N���X
	class brush
		: public gdi_obj<brush_handle>
	{
	public:	
	// �R���X�g���N�^
		brush( brush_handle bh = boost::none )
			: gdi_obj<brush_handle>( bh )
		{}
/*
	// ���Z�q
		operator brush_handle()const {
			return handle();
		}
	// �����o�֐�
	  // �n���h��
		brush_handle::value_type handle_value()const{ return reinterpret_cast<HBRUSH>( base_handle_value() ); }
		brush_handle handle()const{ return handle_value(); }
*/
	};

//##############################################
// �\�z�֐�
//
  // �P�F�̃u���V
	brush_handle create_solid_brush( colorref color );
	
  // �V�X�e���̃u���V���擾�E�폜�����Ⴞ��!
	brush_handle get_system_brush( DWORD syscolor );
	
  // �V�X�e���ɔ������Ă���A�P�F�̃u���V
	brush_handle get_stock_brush( int id );
			
  // �͗l�p�^�[����`�悷��u���V
  	brush_handle create_hatch_brush( int hat, colorref line_color );
  	
  // �O���f�[�V������p����H ���@�̓t�@���N�^�Ŏw��H
  
  // �r�b�g�}�b�v�p�^�[���ŕ`��
	brush_handle create_pattern_brush( bitmap_handle bmh );
  	
  // Indirect
	brush_handle create_brush( const log_brush::value_type& lb );

}}

#endif