#pragma once

#ifndef ODEN_GAMMO_GRA_GDIPLUS
#define ODEN_GAMMO_GRA_GDIPLUS

//#############################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
//-------- NOMINMAX問題に対処 ----------
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

//-------- boost --------
//#include <boost/smart_ptr/intrusive_ptr.hpp>
//-------- oden ---------
//-------- gammo --------

#pragma comment(lib, "gdiplus.lib")

//
//#############################################


namespace oden { namespace gammo
{
	namespace gdipp = ::Gdiplus;

//########################################
// typedef
//
	typedef nullable< gdipp::Status, greater_null<gdipp::Status, gdipp::GenericError> > gdipp_status;

//########################################
// 自動初期化
//
	class gdiplus
	{
	public:
		explicit gdiplus( gdipp::GdiplusStartupOutput* soutput = 0 )
			: token_(0)
		{
			stat_ = gdipp::GdiplusStartup( &token_, &sinput_, soutput );
		}

		typedef struct shutdowner {
			void operator()( gdiplus& o ) { o.shutdown(); }
		} deleter_type;

		void shutdown()const
		{
			if( stat_.is_valid() )
				gdipp::GdiplusShutdown( token_ );
		}

	private:
		gdipp_status 				stat_;
		ULONG_PTR					token_;
		gdipp::GdiplusStartupInput	sinput_;
	};
	
}}

#endif
