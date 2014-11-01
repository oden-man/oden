#pragma once

#ifndef ODEN_GAMMO_UI_RES_ICON_CTL
#define ODEN_GAMMO_UI_RES_ICON_CTL

//#######################################################
//  �C���N���[�h
//
// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
// --------- win ------------

// --------- boost ----------
// --------- oden ----------
#include <oden/property.h>
//----------- gammo ----------
//#include <oden/gammo/ui/resource/resource.h>

//
//###############################################


//___________________________________________________
//
//  �O���錾
//___________________________________________________
//
namespace oden { namespace gammo
{		
//_______________________________________________________________________________________
//
//
//     �A�C�R���N���X
//
//_______________________________________________________________________________________
//		
//########################################################
// �A�C�R�����N���X
//	
/*
    icon�N���X�͊O���̉摜�Ȃǂ��ǂݍ��܂�V���ɍ��ꂽ��̎p��\���Ă���A
    ���f�[�^�̃T�C�Y�E�[�x�Ȃǂ͎����Ă���B
    ���̂��߁A�T�C�Y�E�[�x�Ȃǂ��g�������ꍇ�̓f�[�^��ǂݍ��ލۂɁA�K�v�ȏ���ۑ����Ă����B
*/
	ODEN_STRUCT( icon_info, ICONINFO )
	{
		struct get
		{
			bitmap_handle color_bitmap ()const { return self::val(this).hbmColor; }
			bitmap_handle mask_bitmap  ()const { return self::val(this).hbmMask; }
		};

		struct def
		{
			def( value_type& out )
			{
				out.fIcon = TRUE;
			}
		};
	};	

//########################################################
// �A�C�R���g�����N���X
//
	
//########################################################
// �A�C�R������N���X
//
	class icon
		: public handle_interface<icon_handle>
	{
	public:		
	// �R���X�g���N�^	
		icon( const icon_handle& h = boost::none )
			: handle_interface<icon_handle>( h )
		{}
	
	// �f���[�^�̒�`
		typedef struct destroyer {
			void operator()( icon& o ) { o.destroy(); }
		} deleter_type;
		
	// �����o�֐�
	  // �r�b�g�}�b�v���擾 ( �ق�DDB )
		bitmap_handle color_bitmap( )const;
		bitmap_handle mask_bitmap( )const;
	  // �A�C�R����j������
		bool destroy( );
	  // �A�C�R���𕡐�����
		icon_handle duplicate( )const;
	  // �ʂ̃��W���[���փA�C�R�����R�s�[����
	  	icon_handle copy( )const;
	  // �A�C�R�����̎擾
		bool get_info( icon_info::value_type& out )const;
		//bool get_info( icon_info_ex& out )const;
	  // �A�C�R����`�悷��
		bool draw( 
			dc_handle Todc,
			const rect& place, 
			brush_handle offscreen_brush = boost::none,
			UINT draw_option = DI_NORMAL 
		)const;

	};
	
//__________________________________________________________________________________________________
//
//
//    �\�z�֐� 
// 
//__________________________________________________________________________________________________
//
//#######################################################
//  �t�@�C�����E���\�[�X�����烍�[�h����֐�
//	
  // �A�C�R�����t�@�C������ǂ�
	icon_handle load_icon( 
		string_ptr path, 
		UINT opt = LR_DEFAULTCOLOR, 
		const size& desired_size = size( 0, 0 ) 
	);
	
  // �A�C�R�������\�[�X����ǂ�
  	icon_handle load_icon( 
		module_handle mod, 
		res_id	rid, 
		UINT opt = LR_DEFAULTCOLOR, 
		const size& desired_size = size( 0, 0 ) 
	);

//#######################################################
// ���\�[�X�̃A�C�R��
//
  // ���\�[�X(RT_ICON)�̃r�b�g����A�C�R�����쐬
	icon create_icon_resource (
		buffer_range<BYTE> bits,
		const size& desired_size,
		UINT opt = LR_DEFAULTCOLOR
	);
	
  // ���\�[�X����w��̑傫���̃A�C�R�����擾
	res_id lookup_icon_resource (
		buffer_range<BYTE> bits,
		const size& desired_size,
		UINT opt = LR_DEFAULTCOLOR
	);
	
  // ���\�[�X���̃A�C�R���\����
#	pragma pack( push )
#	pragma pack( 2 )

	struct MEMICONDIRENTRY
	{
		BYTE	bWidth;               // Width of the image
		BYTE	bHeight;              // Height of the image (times 2)
		BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
		BYTE	bReserved;            // Reserved
		WORD	wPlanes;              // Color Planes
		WORD	wBitCount;            // Bits per pixel
		DWORD	dwBytesInRes;         // how many bytes in this resource?
		WORD    nID;				  // resource id
	};
	
	struct MEMICONDIR
	{
		WORD			idReserved;   // Reserved
		WORD			idType;       // resource type (1 for icons)
		WORD			idCount;      // how many images?
		MEMICONDIRENTRY	idEntries[1]; // the entries for each image
	};

#	pragma pack( pop )
	
	ODEN_STRUCT( resource_icon_dir_entry, MEMICONDIRENTRY )
	{
		struct get
		{
			size  dimension() const { return oden::size( width(), height() ); }
			BYTE  width()  const { return self::val(this).bWidth; }
			BYTE  height() const { return self::val(this).bHeight; }
			BYTE  color()  const { return self::val(this).bColorCount; }
			WORD  depth()  const { return self::val(this).wBitCount; }
			DWORD size()   const { return self::val(this).dwBytesInRes; }
			WORD  id()     const { return self::val(this).nID; }
		};
	};
	
	ODEN_STRUCT( resource_icon_dir, MEMICONDIR )
	{
		struct get
		{
			bool is_icon() const { return self::val(this).idType == 1; }
			int count()    const { return self::val(this).idCount; }
			resource_icon_dir_entry::read_cref 
				entry( int i = 0 )const { return self::val(this).idEntries + i; }
		};
	};
	
//###########################################################
// �V�X�e���̃A�C�R��
//	
   // �V�X�e���̃T�C�Y�Ń��[�h����
  	icon_handle load_system_icon( DWORD icoid );
   // �C�ӂ̃T�C�Y�Ń��[�h
  	icon_handle load_system_icon( DWORD icoid, UINT load_option, const size& desired_size = size( 0, 0 ) );
		
//###########################################################
// �r�b�g
//	


}}

#endif
