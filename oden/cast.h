#pragma once

#ifndef ODEN_CAST
#define ODEN_CAST

//################################################
//   �C���N���[�h
//

// ------------ boost -----------------
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

//
//################################################



namespace oden
{
//________________________________________________________________________________________________
//
//
//  �L���X�g
//
//________________________________________________________________________________________________
//
//##########################################################################################
// static_cast ��reinterpret_cast ���g�������� - LPARAM�ł悭�g��
//	
  // �p�����[�^������o��
	template< typename To, typename From>
	To param_pack_cast(
		From from,
		typename boost::enable_if< boost::is_convertible<From,To> >::type* = 0 )
	{
		return static_cast<To>(from);	
	}
	template< typename To, typename From>
	To param_pack_cast(
		From from,
		typename boost::enable_if< boost::is_pointer<From> ( sizeof(To) > sizeof(void*) ) >::type* = 0 )
	{
		return reinterpret_cast<To>(from);	
	}
	
  // �p�����[�^������o��
	template< typename To, typename From>
	To param_unpack_cast(
		From from,
		typename boost::enable_if< boost::is_convertible<From,To> >::type* = 0 )
	{
		return static_cast<To>(from);	
	}
	template< typename To, typename From>
	To param_unpack_cast(
		From from,
		typename boost::enable_if< boost::is_pointer<To> >::type* = 0 )
	{
		return reinterpret_cast<To>(from);	
	}

//##########################################################################################
// �����I�Öٌ^�ϊ�
//	
	template<typename To, typename From>
	inline To implicit_cast(const From &x)
	{
		return x;
	}

//##########################################################################################
// �|�C���^�Ԃ̔�r�I���S�Ȍ^�ϊ�
//	
	template<typename PtrTo, typename From>
	inline PtrTo pointer_cast(From* x)
	{
		return static_cast<PtrTo>(static_cast<void*>(x));
	}
	template<typename PtrTo, typename From>
	inline PtrTo pointer_cast(const From* x)
	{
		return static_cast<PtrTo>(static_cast<const void*>(x));
	}

}


#endif
