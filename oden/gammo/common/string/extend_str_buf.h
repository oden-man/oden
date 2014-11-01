#pragma once

#ifndef ODEN_GAMMO_CMN_STR_EXSTRBUF
#define ODEN_GAMMO_CMN_STR_EXSTRBUF

//################################################
//   �C���N���[�h
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
//   ������̑傫����Ԃ��Ă���Ȃ�API�Ŏg��
//   ���[�v�̒��ŁA���S�ɕ����񂪂Ƃ��܂�ture��Ԃ��A�o�b�t�@��L����������
//

/* �g�p�� *
	string text_buf;
	while( extend_str_buf(text_buf) ) // ���̏ꍇ�͕K����̂�n���i�����Ȃ��Ƒ����[�v�𔲂����Ⴄ�j
	{
		if( 0==wnd.get_class_name(text_buf) )
			break;			
	}
	
	string text_buf(32);			 // ���̏ꍇ�Ȃ�m�ۍς݂ł��悢
	do {
		int cp=wnd.get_class_name(text_buf);
		if(cp==0)
			break;
	}
	while( extend_str_buf( out, cp ) );
	
	// ��������
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
		if( copied_len == 0 && !buf.empty()  ) // �󕶎��񂪕Ԃ��ꂽ�ꍇ
		{ 
			Buffer().swap( buf );
			return false;
		}
		else if( copied_len != 0 && copied_len + 1 < static_cast<int>( buf.size() ) )
		{ //       			  ^^^^NULL�����̕��𑫂�             ^^^^^^^^^^NULL�������܂߂��T�C�Y	
	
			Buffer trimmed; trimmed.resize( copied_len ); // �������҂�����(NULL�����𗎂Ƃ�)�ɂ���
			std::copy( buf.begin(), boost::next(buf.begin(),copied_len), trimmed.begin() ); 

			trimmed.swap( buf );  // �����������
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
		// NULL�����𓪂���T�[�`
		typename RandomAccessBuffer::iterator end_itr = std::find( buf.begin(), buf.end(), RandomAccessBuffer::value_type(0) );
		if( buf.begin() == end_itr && !buf.empty() ) // NULL�������擪�ɂ���A�o�b�t�@����łȂ��Ȃ�A�󕶎�����擾���������Ƃ݂Ȃ�
		{
			RandomAccessBuffer().swap( buf );
			return false;
		}
		else if( buf.begin() < end_itr && end_itr < boost::prior(buf.end()) ) // NULL�����������ɂ���ꍇ�́A�擾������Ȃ������Ƃ݂Ȃ�
		{
			size_t end_pos = std::distance( buf.begin(), end_itr );
		
			RandomAccessBuffer trimmed; trimmed.resize( end_pos ); // �������҂�����(NULL�����𗎂Ƃ�)�ɂ���
			std::copy( buf.begin(), end_itr, trimmed.begin() ); 

			trimmed.swap( buf ); // �����������
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
