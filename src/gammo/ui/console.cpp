#include "../Pch.h"

#include <cstdio>
#include <oden/gammo/ui/console.h>

namespace oden { namespace gammo
{
//##########################################################
//        console
//
bool console::allocate()
{
	return 0!=::AllocConsole();
}
bool console::free()
{
	return 0!=::FreeConsole();
}

void console::set_as_stdio()
{
	FILE* fp = 0;
	freopen_s(&fp, "CONOUT$", "w", stdout); //標準出力をコンソールにする
	freopen_s(&fp, "CONIN$", "r", stdin);   //標準入力をコンソールにする
}

//##########################################################
//                  console_out
//

//##########################################################
//                  console_in
//

} /* end of namespace gammo */
} /* end of namespace oden */
