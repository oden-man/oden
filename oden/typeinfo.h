#pragma once

#ifndef ODEN_TYPEINFO
#define ODEN_TYPEINFO

#include <typeinfo>

namespace oden
{
//#########################################################
// typeid�ł̓g�b�v���x���̎Q�ƁEcv�C���𖳎�����B
// �Ă��Ƃ��Ȍ^�ŕ��ł�邱�ƂŖ������������
//	
	template< typename T >
	struct typeid_t;
	
	template< typename T >
	static const std::type_info& typeid_()
	{
		return typeid( typeid_t<T> );	
	}

	template< typename T >
	static const std::type_info& typeid_( const T& )
	{
		return typeid( typeid_t<T> );	
	}

	template< typename T >
	static const std::type_info& typeid_( T& )
	{
		return typeid( typeid_t<T> );	
	}
	
//#########################################################
// �f�}���O��?
//	
	
}


#endif
