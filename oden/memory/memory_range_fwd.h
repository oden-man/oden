#pragma once

#ifndef ODEN_MEM_RANGE_FWD
#define ODEN_MEM_RANGE_FWD

//#######################################################
//  インクルード
//
//
//####################################################

//#################################################
//  前方宣言・typedef
//

namespace oden
{
	typedef unsigned char byte_type;
	
	template< typename T = byte_type >
	class buffer_range;
	
	template< typename X = byte_type >
	class memory_range;
	
  // メモリが連続したコンテナである - 特殊化して使用
	template< typename T >
	struct container_to_carray
	{};

	
}

#undef ODEN_MEM_RANGE_STDARRAY
#endif
