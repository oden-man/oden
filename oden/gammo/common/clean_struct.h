#pragma once

#ifndef ODEN_GAMMO_COMMON_CLEARSTRUCT
#define ODEN_GAMMO_COMMON_CLEARSTRUCT

//##############################################
//  �C���N���[�h
//

//
//
//#############################################

namespace oden { namespace gammo
{
//#############################################################
//
//
	// �\����(aggragate_type)������������ 
	template< typename T >
	T cleaned_struct()
	{
		static const T cleaned = {};
		return cleaned;
	}

	// �\����(aggragate_type)������������ 
	template< typename T >
	void clean_struct( T& out )
	{
		static const T cleaned = {};
		out = cleaned;
	}
}
}
	
#endif