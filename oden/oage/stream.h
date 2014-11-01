#ifndef ODEN_OAGE_STRM
#define ODEN_OAGE_STRM

#include <istream>
#include <ostream>
#include <vector>
#include <oden/memory_range.h>
#include <oden/oage/defs.h>
#include <oden/oage/stream_buffer.h>

namespace oden { namespace oage 
{
//####################################################
//  �o�C�i���X�g���[���̓ǂݏ���
//
  // sizeof(element) = 1�̃R���e�i�Ƃ݂Ȃ��A�P���ɐ擪����o�C�i�����˂����ށB


  // ���g��S���o�b�t�@�ɓf��
	template< typename StreamBuf, typename Container >
	bool read_stream_buf( StreamBuf& sbuf, Container &dest )
	{
		typedef typename Container::value_type dest_value_type;
	
		while( sbuf.read() ) 
		{
			int readed_size = sbuf.buffer_size()/sizeof(dest_value_type);
			const dest_value_type* ptr = reinterpret_cast<const dest_value_type*>( sbuf.buffer_ptr() );
			
			dest.insert( dest.end(), ptr, ptr + readed_size ); 
		}
		
		return !sbuf.stream().good();
	}
	
	template< int SBuf, typename Container >
	bool read_stream( std::istream& is, Container &dest )
	{
		stream_buffer<SBuf> sbuf(is);
		return read_stream_buf( sbuf, dest );
	}
	
	template< typename Container >
	bool read_stream( std::istream& is, Container &dest, int bufsize = 1024 )
	{
		stream_buffer<> sbuf(is, bufsize);
		return read_stream_buf( sbuf, dest );
	}

  // �f�[�^��S�ď����o��
	bool write_stream( std::ostream& os, memory_range<byte_type> src )
	{		
		return !os.write( reinterpret_cast<const char*>(src.get()), src.size() ).good();
	}
	
}}

#endif

