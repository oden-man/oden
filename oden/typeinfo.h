#pragma once

#ifndef ODEN_TYPEINFO
#define ODEN_TYPEINFO

#include <typeinfo>

namespace oden
{
//#########################################################
// typeidではトップレベルの参照・cv修飾を無視する。
// てきとうな型で包んでやることで無視を回避する
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
// デマングる?
//	
	
}


#endif
