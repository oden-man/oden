/**
*
*/

#pragma once

#ifndef ODEN_OAGE_ALGORITHM
#define ODEN_OAGE_ALGORITHM

#include <cstdlib>

#ifdef _MSC_VER
#	define OAGE_MS_SECURE_FUNCS
#endif

namespace oden { namespace oage
{
//###############################################################
// write
/*
	copyと同じだが、src と dest の最大サイズを指定でき、
	どちらかが限界に達した時点でコピーをやめる。
*/
	template< typename T, typename Size >
	T* write( const T* src, Size ssize, T* dest, Size dsize )
	{
		Size size = 0;
		if( ssize > dsize )
			size = dsize;
		else
			size = ssize;
			
#ifdef OAGE_MS_SECURE_FUNCS
		memcpy_s( dest, size, src, size );
#else
		memcpy( dest, size, src );
#endif
		return dest+size;
	}
	template< typename T >
	T* write( const T* sfirst, const T* slast, T* dfirst, T* dlast )
	{
		return oage::write( sfirst, slast-sfirst, dfirst, dlast-dfirst );
	}
	
	template< typename InputIterator, typename OutputIterator >
	OutputIterator write( InputIterator first, InputIterator last, OutputIterator ofirst, OutputIterator olast )
	{
		while( first!=last && ofirst!=olast )
		{
			*ofirst = *first;
			
			++first;
			++ofirst;
		} 
		return ofirst;		
	}
	
}
}

#endif