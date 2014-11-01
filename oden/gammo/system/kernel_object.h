#pragma once

#ifndef ODEN_GAMMO_SYS_KO
#define ODEN_GAMMO_SYS_KO

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden -----------
//----------- gammo ----------

//
//#################################################

namespace oden { namespace gammo
{
	class kernel_obj
		: public handle_interface<kernel_obj_handle>
	{
	public:
	// コンストラクタ
		kernel_obj( kernel_obj_handle h = boost::none )
			: handle_interface<kernel_obj_handle>(h)
		{}

	// メンバ
		bool close()const;
		kernel_obj_handle duplicate_handle( process_handle src, process_handle dest, bool inherit = true, DWORD option = DUPLICATE_SAME_ACCESS, DWORD access = 0 )const;

		typedef struct closer {
			void operator()( kernel_obj& o ){ o.close(); }
		} deleter_type;

	};
	
}}

#endif