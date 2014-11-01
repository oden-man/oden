#pragma once

#ifndef ODEN_GAMMO_SYS_COM_STREAM
#define ODEN_GAMMO_SYS_COM_STREAM


//#############################################
//  インクルード
//
//-------- defs ---------
#include <oden/gammo/defs.h>
//-------- vc -----------
//-------- win ----------
//-------- boost --------
//-------- oden ---------
//-------- gammo --------
#include <oden/gammo/system/com.h>

//
//#############################################


namespace oden { namespace gammo
{
//########################################
//  IStream
//
	class com_stream
		: public com_wrapper<IStream>
	{
	public:
	// 構築
	  // メモリから(shell)
		bool create	( memory_range<BYTE> init = boost::none );
	// メンバ
	  // 読み書き
		hresult read	( buffer_range<BYTE> buf, ULONG &readed )const;
		hresult write	( memory_range<BYTE> mem, ULONG &written )const;	
	};
	
}}

#endif
