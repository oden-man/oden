#pragma once

#ifndef ODEN_MEM_RANGE_STD
#define ODEN_MEM_RANGE_STD

//#######################################################
//  インクルード
//

#include <oden/memory/memory_range_fwd.h>

#include <string>
#include <vector>
#include <boost/array.hpp>

#if defined(_MSC_VER) && _MSC_VER >= 1600
#	define ODEN_MEM_RANGE_STDARRAY
#endif

#ifdef ODEN_MEM_RANGE_STDARRAY
#	include <array>
#endif

//
//####################################################

//#################################################
//  マクロ
//

namespace oden
{
	
//#############################################################
// コンテナのポインタを取得する
//
  // vector
	template< typename T, typename Al >
	struct container_to_carray< std::vector<T,Al> >
	{
		typedef T elem_type;
		
		template< typename Range, typename Container >
		static Range get( Container& cont )
		{
			if( cont.empty() )
				return boost::none;	
				
			return Range( &cont[0], cont.size() );
		}
	};
	
  // string
	template< typename T, typename Tr, typename Al >
	struct container_to_carray< std::basic_string<T,Tr,Al> >
	{
		typedef T elem_type;
		
		template< typename Range, typename Container >
		static Range get( Container& cont )
		{
			if( cont.empty() )
				return boost::none;	
				
			return Range( &cont[0], cont.size() );
		
		}
	};	
	
  // boost.array
	template< typename T, std::size_t N >
	struct container_to_carray< boost::array<T,N> >
	{
		typedef T elem_type;
		
		template< typename Range, typename Container >
		static Range get( const Container& cont )
		{
			return Range( cont.data(), N );
		}
		template< typename Range, typename Container >
		static Range get( Container& cont )
		{
			return Range( cont.c_array(), N );
		}
	};
		
#ifdef ODEN_MEM_RANGE_STDARRAY

  // std.array
	template< typename T, std::size_t N >
	struct container_to_carray< std::array<T,N> >
	{
		typedef T elem_type;
		
		template< typename Range, typename Container >
		static Range get( Container& cont )
		{
			return Range( cont.data(), N );
		}
	};

#endif

}

#undef ODEN_MEM_RANGE_STDARRAY
#endif
