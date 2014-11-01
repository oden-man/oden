#pragma once

#ifndef ODEN_GAMMO_UI_CONSOLE
#define ODEN_GAMMO_UI_CONSOLE

//#####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ------ STL ---------
// ----- boost --------
// ------- oden ---------
// ------- gammo --------
#include <oden/gammo/system/kernel_object.h>

//
//
//###############################################################

namespace oden { namespace gammo
{	
//____________________________________________________________________________
//
//           コンソール
//____________________________________________________________________________
//		
//########################################################################
//  全体の割り当て・設定など
//
	class console
	{
	public:
		static bool allocate();
		static bool free();
		
		typedef struct freer {
			void operator()( console& ){ console::free(); }
		} deleter_type;
		
		static void set_as_stdio();
		
		
	};

//########################################################################
// 出力  
//
	class console_out
		: public kernel_obj
	{
	public:
	// コンストラクタ
		console_out( kernel_obj_handle h = boost::none )
			: kernel_obj(h)
		{}

	// メンバ関数

	};

}}

#endif

