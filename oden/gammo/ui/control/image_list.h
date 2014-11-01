#pragma once

#ifndef ODEN_GAMMO_UI_CTL_IMGLIST
#define ODEN_GAMMO_UI_CTL_IMGLIST

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
//           �C���[�W���X�g �N���X
//
//________________________________________________________________________________
//
//################################################################
// 
//	
	ODEN_STRUCT( image_list_imginfo, IMAGEINFO )
	{
		struct get
		{
			bitmap_handle color()const { return self::val(this).hbmImage; }
			bitmap_handle mask()const { return self::val(this).hbmMask; }
			rect rectangle()const { return self::val(this).rcImage; }
		};
		
		struct set
		{};
	};
	
//################################################################
//  �R���g���[���N���X
//
	class image_list
		: public handle_interface<image_list_handle>
	{
	private:
		typedef image_list self;
	public:	
	// �R���X�g���N�^
		image_list( image_list_handle imgh = boost::none )
			: handle_interface<image_list_handle>( imgh )
		{}
		
	// �f���[�^
		typedef struct destroyer {
			void operator()( self& obj )const { obj.destroy(); }
		} deleter_type;
		
	// �����o�֐�
	  // �n���h��
		// �쐬����
 		bool create( const size& size, UINT flags, int initial = 16, int grow = 0 );
		// �폜����
		bool destroy( );
		// ��������
		image_list_handle duplicate()const;
		
	  // �C���[�W
		// �ЂƂǉ�
		index add( bitmap_handle img, bitmap_handle mask = boost::none )const;
		index add( icon_handle ico )const;
		// �C���[�W��u��������
		index replace( index at, bitmap_handle img, bitmap_handle mask = boost::none )const;
		index replace( index at, icon_handle ico )const;
		// �폜
		bool erase( index at )const;
		// ���ׂč폜����
		bool clear()const;
		// ��������
		image_list_handle merged( image_list_handle l1, index at1, image_list_handle l2, index at2, const point& offset )const;
		// �`��
		bool draw( 
			index at, 
			dc_handle dest, 
			const point& pt, 
			UINT style = ILD_NORMAL 
		  )const;
		bool draw(
			index at, 
			dc_handle dest, 
			const rect& rc, 
			colorref back = CLR_NONE, 
			colorref fore = CLR_NONE, 
			UINT style = ILD_NORMAL 
		  )const;
		//  �C���[�W�����擾
		bool get_image_info( index at, image_list_imginfo::value_type& r )const;
		// �C���[�W����A�C�R����V���ɍ쐬( destroy ���ׂ� )
		icon_handle create_icon( index at, UINT flags )const;
		
		// �C���[�W�T�C�Y���擾
		size dimension()const;		  
		bool set_sizesion( const size& dm )const;
		// �C���[�W�̐���Ԃ�/����������(���replace�Œǉ����ׂ�)
		size_t count()const;
		bool set_count( UINT cnt )const;

	  // �h���b�O�E�h���b�v
		// ���݃h���b�O�Ɏg���Ă���C���[�W���X�g���擾
		static image_list_handle current_drag_image( point* at = 0, point* hotspot = 0 );
		// �h���b�O���J�n
		bool begin_drag( index at, const point& hotspot = point(0,0) )const;
		// �h���b�O��Ԃɓ���
		bool drag_enter( window_handle wh, const point &drag_pos )const;
		// �h���b�O�C���[�W�𓮂���
		bool drag_move( const point &drag_pos )const;
		// �h���b�O��Ԃ��甲����
		bool drag_leave( window_handle wh )const;
		// �h���b�O�C���[�W���B��/�o��
		bool hide_drag_image( bool hide = true )const;
		// �h���b�O���I����
		static void end_drag_image();

	};
	
	
}}

#endif
