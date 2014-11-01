#pragma once

#ifndef ODEN_GAMMO_CMN_STR_EXSTRBUF
#define ODEN_GAMMO_CMN_STR_EXSTRBUF

//################################################
//   インクルード
//

// --------- defs -----------
// --------- STL ------------
#include <algorithm>

// --------- boost -----------
#include <boost/next_prior.hpp>

// --------- oden ------------

//
//################################################

//###################################################
//
//

namespace oden { namespace gammo
{	
//#########################################################################
//   *** next_str_buf ***
//   文字列の大きさを返してくれないAPIで使う
//   ループの中で、完全に文字列がとれるまでtureを返し、バッファを伸長し続ける
//

/* 使用例 *
	string text_buf;
	while( extend_str_buf(text_buf) ) // この場合は必ず空のを渡す（さもないと即ループを抜けちゃう）
	{
		if( 0==wnd.get_class_name(text_buf) )
			break;			
	}
	
	string text_buf(32);			 // この場合なら確保済みでもよい
	do {
		int cp=wnd.get_class_name(text_buf);
		if(cp==0)
			break;
	}
	while( extend_str_buf( out, cp ) );
	
	// 複数も可
	while( expand_str_buf(tbuf) && expand_str_buf(abuf) )
	{
		data.text(tbuf).title(abuf);
		
		if(! f( &data ) )
			return false;
	} 
*/
	template< typename Buffer, typename SizeT >
	bool extend_str_buf( Buffer& buf, SizeT copied_len )
	{
		if( copied_len == 0 && !buf.empty()  ) // 空文字列が返された場合
		{ 
			Buffer().swap( buf );
			return false;
		}
		else if( copied_len != 0 && copied_len + 1 < static_cast<int>( buf.size() ) )
		{ //       			  ^^^^NULL文字の分を足す             ^^^^^^^^^^NULL文字を含めたサイズ	
	
			Buffer trimmed; trimmed.resize( copied_len ); // 長さをぴったり(NULL文字を落とす)にする
			std::copy( buf.begin(), boost::next(buf.begin(),copied_len), trimmed.begin() ); 

			trimmed.swap( buf );  // メモリを解放
			return false;
		}
		
		if( buf.empty() )
			buf.resize( 32 );
		else
			buf.resize( buf.size()*2, 0 );
		
		return true;
	}
	
	template< typename RandomAccessBuffer >
	bool extend_str_buf( RandomAccessBuffer& buf )
	{
		// NULL文字を頭からサーチ
		typename RandomAccessBuffer::iterator end_itr = std::find( buf.begin(), buf.end(), RandomAccessBuffer::value_type(0) );
		if( buf.begin() == end_itr && !buf.empty() ) // NULL文字が先頭にあり、バッファが空でないなら、空文字列を取得しきったとみなす
		{
			RandomAccessBuffer().swap( buf );
			return false;
		}
		else if( buf.begin() < end_itr && end_itr < boost::prior(buf.end()) ) // NULL文字が末尾にある場合は、取得しきれなかったとみなす
		{
			size_t end_pos = std::distance( buf.begin(), end_itr );
		
			RandomAccessBuffer trimmed; trimmed.resize( end_pos ); // 長さをぴったり(NULL文字を落とす)にする
			std::copy( buf.begin(), end_itr, trimmed.begin() ); 

			trimmed.swap( buf ); // メモリを解放
			return false;
		}
		
		if( buf.empty() )
			buf.resize( 32 );
		else
			buf.resize( buf.size()*2, 0 );
			
		return true;
	}
	
}}

#endif
