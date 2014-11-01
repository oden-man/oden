#pragma once

#ifndef ODEN_GAMMO_UI_SH_FSYS
#define ODEN_GAMMO_UI_SH_FSYS

//#######################################################
//  インクルード
//

// --------- def ------------
//#include <oden/gammo/system/com.h>
// --------- stl ------------
// --------- win ------------
#include <ShlObj.h>
// --------- boost ----------
// --------- oden  ----------
// --------- chikuwa  --------
// --------- gammo ----------

//
//####################################################


namespace oden { namespace gammo
{
namespace shell
{
//#################################################
// shell item 取得関数
//
// vista
#if (_WIN32_IE >= _WIN32_IE_IE70)

	template< typename Out >
	hresult create_item( string_ptr name, Out& out, IBindCtx* pbc = 0 )
	{
		return ::SHCreateItemFromParsingName( name.c_str(), pbc, out.iid(), out.to_source() );
	}

	template< typename Out >
	hresult create_item( IShellItem* parent, string_ptr name, Out& out, IBindCtx* pbc = 0 )
	{
		return ::SHCreateItemFromRelativeName( parent, name.c_str(), pbc, out.iid(), out.to_source() );
	}

	template< typename Out >
	hresult create_item( const KNOWNFOLDERID& id, DWORD flags, string_ptr name, Out& out )
	{
		return ::SHCreateItemFromKnownFolder( id, flags, name.c_str(), out.iid(), out.to_source() );
	}

#endif

}

}} /* end of namespace oden.gammo */

#endif

