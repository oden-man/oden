#pragma once

#ifndef ODEN_OBJ_SCP
#define ODEN_OBJ_SCP

//#####################################################
//  �C���N���[�h
//

// ------ defs -----------
// ------ STL ---------
// ----- boost --------
#include <boost/compressed_pair.hpp>

// ------- oden ---------
#include <oden/disallow.h>

// ------- gammo --------
#include <oden/object/object_facade.h>


//
//
//###############################################################

namespace oden
{
//###########################################################################
//  scoped_ptr
//	
/*
*  Base - (const)object_manager�̔h���łȂ���΂Ȃ�Ȃ��B
*  Deleter
*/
	template< 
		typename T, 
		typename Deleter = inner_deleter<> >
	class scoped
		: public object_facade< scoped< T, Deleter >, T >
	{
	private:		
		typedef scoped< T, Deleter > self;
	public:
		typedef Deleter	deleter;
		
	// �R���X�g���N�^
		scoped()
		  : data_()
		{}
		scoped( typename self::param_type value )
		  : data_( value )
		{}
		scoped( const deleter& deler )
		  : data_( deler )
		{}
		template< typename X >
		scoped( const X& value, const deleter& deler )
		  : data_( value, deler )
		{}
	// �f�X�g���N�^
		~scoped()
		{
			this->delete_object();
		}
		
	// �����o�֐�
	  // �I�u�W�F�N�g�̎擾
		typename self::reference get( ) { return data_.first(); }
		typename self::param_type get( )const { return data_.first(); }
	  // �f���[�^�̎擾
		deleter& get_deleter() { return data_.second(); } 
		const deleter& get_deleter()const { return data_.second(); }
	  // ���
		void delete_object(){ get_deleter()( get() ); }
		
	// swap
		void swap( scoped& r ) 
		 { using std::swap; swap( data_, r.data_ ); }
		friend void swap( scoped& l, scoped& r ) 
		 { l.swap(r); }
	private:
	// �R�s�[�֎~!
		ODEN_NO_COPY_CLASS( self )
		
	private:
		boost::compressed_pair<T,deleter> data_;	// �Ǘ��I�u�W�F�N�g
	};
}
	
#endif
