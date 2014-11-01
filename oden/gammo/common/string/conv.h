#pragma once

#ifndef ODEN_GAMMO_CMN_STR_CV
#define ODEN_GAMMO_CMN_STR_CV
//################################################
//   インクルード
//

// --------- defs -----------
// --------- boost ------------
// --------- oden -----------
#include <oden/gammo/common/string/forward.h>

//
//################################################

//###################################################
//
//

namespace oden { namespace gammo
{
//#####################################################
// 文字変換関数
//
	// wstring->stringに変換 
	bool widen( wide_string& w, const narrow_string& c );
	inline wide_string widen( const narrow_string& c ){ wide_string w; widen(w,c); return w; }
	inline wide_string widen( const wide_string& w ){ return w; }

	// string->wstringに変換
	bool narrow( narrow_string& c, const wide_string& w );
	inline narrow_string narrow( const wide_string& w ){ narrow_string c; narrow(c,w); return c; }
	inline narrow_string narrow( const narrow_string& c ){ return c; }

	// 転送
	template< typename T >
	bool conv_string( T& out, const T& c ){ out = c; return true; }	
	inline bool conv_string( narrow_string& out, const wide_string& c ){ return narrow(out, c); }
	inline bool conv_string( wide_string& out, const narrow_string& c ){ return widen(out,c); }

	// 
	inline gammo::string generic_string( const narrow_string& c )
	{
		gammo::string out;
		conv_string( out, c );
		return out;
	}
	inline gammo::string generic_string( const wide_string& c )
	{
		gammo::string out;
		conv_string( out, c );
		return out;
	}
	


}}

#endif
