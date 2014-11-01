#pragma once

#ifndef ODEN_OBJ_LINK
#define ODEN_OBJ_LINK

//#####################################################
//  �C���N���[�h
//

// ------ defs -----------
// ------ STL ---------
// ----- boost --------
#include <boost/swap.hpp>
#include <boost/compressed_pair.hpp>
// ------- oden ---------
#include <oden/object/link_ref.h>
#include <oden/object/object_facade.h>
// ------- gammo --------


//
//
//###############################################################

namespace oden 
{
//###########################################################################
//  unique_ptr
//
	template< 
		typename T, 
		typename Deleter = inner_deleter<> >
	class link_shared
		: public object_facade< link_shared<T,Deleter>, T >
	{
	private:
		typedef link_shared<T,Deleter> self;
	public:		
		typedef Deleter	deleter;
		
	// �R���X�g���N�^�E�f�X�g���N�^
		link_shared()
			: data_( ), cnt_( )
		{}
		link_shared( typename self::param_type object )
			: data_( object ), cnt_( )
		{}
		link_shared( const deleter& deler )
			: data_( deler ), cnt_( )
		{}
		link_shared( typename self::param_type object, const deleter& deler )
			: data_( object, deler ), cnt_( )
		{}
		~link_shared()
		{
			if( cnt_.alone() )
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
		
	// �����o�֐�
	  // �擾
		typename self::param_type get()const{ return object_; }
		typename self::reference get_ref(){ return object_; }
	  // �f���[�^�̎擾
		deleter& get_deleter() { return deleter_; } 
		const deleter& get_deleter()const { return deleter_; } 

	  // swap
		void swap( self& r )
		{
			boost::swap( object_, r.data_ );
			cnt_.swap( r.cnt_ );
		}
		friend void swap( self& l, self& r )
		 { l.swap(r); }
		
	  // ����\ 
	  	void assignable(){}
		
	private:
	// �Ǘ��I�u�W�F�N�g
		boost::compressed_pair<T,deleter> data_;	// �Ǘ��I�u�W�F�N�g
		link_ref cnt_; // �����N���X�g���J�E���^	
	};
	
}
	
#endif
