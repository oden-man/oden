#pragma once

#ifndef ODEN_MEM_RANGE_FWD
#define ODEN_MEM_RANGE_FWD

//#######################################################
//  �C���N���[�h
//
//
//####################################################

//#################################################
//  �O���錾�Etypedef
//

namespace oden
{
	typedef unsigned char byte_type;
	
	template< typename T = byte_type >
	class buffer_range;
	
	template< typename X = byte_type >
	class memory_range;
	
  // ���������A�������R���e�i�ł��� - ���ꉻ���Ďg�p
	template< typename T >
	struct container_to_carray
	{};

	
}

#undef ODEN_MEM_RANGE_STDARRAY
#endif
