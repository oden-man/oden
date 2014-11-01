#include "../Pch.h"
	
#include <oden/gammo/ui/resource/icon.h>
//#include <oden/gammo/graphic/gdi/bitmap.h>

//#include <oden/gammo/ui/resource/resource.h>

namespace oden { namespace gammo
{
//______________________________________________________________________________________________
//
//
//    control
//
//______________________________________________________________________________________________
//		
//#############################################################
// icon
//

// �r�b�g�}�b�v�̎擾
bitmap_handle icon::color_bitmap()const
{
	icon_info::read_val info;
	get_info( info );
    return info.color_bitmap();
}
bitmap_handle icon::mask_bitmap()const
{
	icon_info::read_val info;
	get_info( info );
    return info.mask_bitmap();
}

/*
// �擾�\�T�C�Y
size icon::dimension()const
{	
	// �r�b�g�}�b�v���擾
	scoped_bitmap bmp;
	if( !(bmp = color_bitmap()) && !(bmp = mask_bitmap()) )
	    return size(0,0);
	
	logical_bitmap_info binfo;
	bmp->get_info( binfo );
	
	// ��������{�Ȃ̂��C��
	return binfo.dimension().scaled( 1.0f, 0.5f );
}
*/

// �A�C�R����j������
bool icon::destroy( )
{
	if( FALSE!=::DestroyIcon( handle_value() ) ) 
		return true;
	return false;
}

// �A�C�R���𕡐�����
icon_handle icon::duplicate( )const
{
	return ::DuplicateIcon( NULL, handle_value() );
}

// �ʂ̃��W���[���փA�C�R�����R�s�[����
icon_handle icon::copy( )const
{
	return ::CopyIcon( handle_value() ); 	
}

// �A�C�R�������R�s�[
bool icon::get_info( icon_info::value_type& out )const
{
	return FALSE != ::GetIconInfo( handle_value(), &out );
}
/*
bool icon::get_info( icon_info_ex::viewer& out )const
{
	return ::GetIconInfoEx( get(), &out.get() );
}
*/

//  �A�C�R����`��
bool icon::draw(
	dc_handle 		to_dc,
	const rect& 	place, 
	brush_handle 	offscreen_brush, 
	UINT 			draw_option
	)const
{
	return FALSE != ::DrawIconEx( 
		to_dc.value(), 
		place.left(),
		place.top(),
		this->handle_value(),
		place.width(),
		place.height(),
		0, 
		offscreen_brush.value(),
		draw_option
	);
}

//_____________________________________________________________________________________________________________
//
//
//    �\�z�D
//
//_____________________________________________________________________________________________________________
//
//#############################################################
// �t�@�C�����E���\�[�X�����烍�[�h
//
// �A�C�R�����t�@�C��������ǂ�
icon_handle load_icon( 
	string_ptr 		path, 
	UINT 			option, 
	const size& 	desired_size )
{
	return reinterpret_cast< HICON >( 
		::LoadImage( 
			NULL, 
			path.c_str(), 
			IMAGE_ICON, 
			desired_size.width(), desired_size.height(),
			option | LR_LOADFROMFILE 
		)
	);		
}

// �A�C�R�������\�[�X������ǂ�
icon_handle load_icon( 
	module_handle modh, 
	const res_id& rid, 
	UINT option, 
	const size& desired_size )
{
	return reinterpret_cast< HICON >( ::LoadImage(modh.value(), rid.get(), IMAGE_ICON, desired_size.width(), desired_size.height(), option) );
}
/*
icon load_icon( 
	string_ptr 					path,
	const res_id& 				rid, 
	UINT 	opt, 
	const size& 				desired_size )
{
	scoped<module> mod( path );
	return load_icon( mod.get(), rid, opt, desired_size );
}*/

//#############################################################
// ���\�[�X�f�[�^����\�z
//
// ���\�[�X�f�[�^����A�C�R�����쐬
icon create_icon_resource (
	buffer_range<BYTE> bits,
	const size& desired_size,
	UINT opt )
{
	return ::CreateIconFromResourceEx(
		bits.get(), bits.size(),
		TRUE,
		0x00030000,
		desired_size.width(), desired_size.height(),
		opt
	);
}

// ���\�[�X����w��̑傫���̃A�C�R�����擾
res_id lookup_icon_resource (
	buffer_range<BYTE> bits,
	const size& desired_size,
	UINT opt )
{
	return ::LookupIconIdFromDirectoryEx(
		bits.get(),
		TRUE,
		desired_size.width(), desired_size.height(),
		opt
	);
}

//#############################################################
// �V�X�e���E�A�C�R��
//

// �V�X�e���̃A�C�R�������[�h����
icon_handle load_system_icon( LPCTSTR icoid ) 
{
	return ::LoadIcon( NULL, icoid );	
}

icon_handle load_system_icon( LPCTSTR icoid, UINT option, const size& desired_size )
{
	return reinterpret_cast< HICON >( 
		::LoadImage( 
			NULL,
			icoid, 
			IMAGE_ICON, 
			desired_size.width(), desired_size.height(), 
			option | LR_SHARED 
		)
	);
}


} /* end of namespace gammo */
} /* end of namespace oden */






