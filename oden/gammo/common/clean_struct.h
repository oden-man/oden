#pragma once

#ifndef ODEN_GAMMO_COMMON_CLEARSTRUCT
#define ODEN_GAMMO_COMMON_CLEARSTRUCT

//##############################################
//  インクルード
//

//
//
//#############################################

namespace oden { namespace gammo
{
//#############################################################
//
//
	// 構造体(aggragate_type)を初期化する 
	template< typename T >
	T cleaned_struct()
	{
		static const T cleaned = {};
		return cleaned;
	}

	// 構造体(aggragate_type)を初期化する 
	template< typename T >
	void clean_struct( T& out )
	{
		static const T cleaned = {};
		out = cleaned;
	}
}
}
	
#endif