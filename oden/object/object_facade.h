#pragma once

#ifndef ODEN_GAMMO_CMN_OBJ_FACADE
#define ODEN_GAMMO_CMN_OBJ_FACADE

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
// ------ STL ---------
// ----- boost --------
#include <boost/call_traits.hpp>
#include <boost/type_traits/add_lvalue_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>

// ------- oden ---------
#include <oden/object/deleter.h>
// ------- gammo --------

//
//###################################################

namespace oden
{
	
//###########################################################################
//  �I�u�W�F�N�g�Ǘ��N���X�E�t�@�T�[�h
//
	template< typename Derived, typename Elem >
	struct object_facade
	{
		typedef Derived		derived;
		typedef Elem		element_type;
		typedef typename boost::call_traits<element_type>::param_type  				param_type;
		typedef typename boost::add_lvalue_reference<element_type>::type  			reference;
		typedef typename boost::add_pointer<element_type>::type 					pointer;
		typedef typename boost::add_lvalue_reference<const element_type>::type		const_reference;
		typedef typename boost::add_pointer<const element_type>::type 				const_pointer;

	public:
	// ���Z�q		
	  // *
		reference operator*( ) { return get_derived().get(); }
		param_type operator*( )const { return get_derived().get(); }
	  // ->
		pointer operator->( ) { return &get_derived().get(); }
		const_pointer operator->( )const { return &get_derived().get(); }
	  // ==
	  	bool operator == ( const derived& obj )const { return get_derived().get() == obj.get(); }
	  	bool operator == ( param_type val )const 	 { return get_derived().get() == val; }
	  	bool operator != ( const derived& obj )const { return !( get_derived() == obj ); }
	  	bool operator != ( param_type val )const     { return !( get_derived() == val ); }
	  // �L���X�g
		//operator reference() { return get_derived().get(); }
		operator param_type()const { return get_derived().get(); }
	  // ���
		object_facade& operator=( derived src )
		{
			src.swap( this->get_derived() );			
			return (*this);
	  	}
		object_facade& operator=( param_type val ) 
		{
			get_derived().assignable();
			reset(val);
			return (*this);	
		}
		
	// �֐�
	  // reset
	  	void reset() 
		{
			derived().swap( this->get_derived() );
	  	}
	  	void reset( param_type newobj ) 
		{
			derived( newobj ).swap( this->get_derived() );
	  	}
		
	  // to_source
		reference to_source()
		{
			reset();
			return get_derived().get();
		}
		
	private:
	// �h���N���X���擾
		Derived& get_derived( ) { return *static_cast< Derived* >( this ); }
		const Derived& get_derived( )const { return *static_cast< const Derived* >( this ); }
	};
	
	
	/*
//###########################################################
//	�I�u�W�F�N�g�ꎞ�󂯎��N���X
//
	template<class T>
	struct object_ptr
	{
	public:
		typedef typename object_manager<T>::reference reference;
		typedef typename boost::add_pointer<T>::type  pointer;
	
	// �R���X�g���N�^
	  // ���^
		object_ptr(const object_manager<T>& o)
			: r_(o.get_object_ref())
		{}
	  // �ÖكL���X�g���s��
		template<class X>
		object_ptr(const object_manager<X>& o)
			: r_(o.get_object_ref())
		{}
	// �擾����
		reference get()const { return r_; }
		reference operator*()const { return get(); }
		pointer operator->()const { return &get(); } 
	private:
		reference r_;
	};
	
//#####################################################
//	�I�u�W�F�N�g�ꎞ�󂯎��N���X : const
//
	template<class T>
	struct const_object_ptr
	{
	public:
		typedef typename object_manager<T>::param_type  parameter;
		typedef typename boost::add_pointer<const T>::type  const_pointer;
	
	// �R���X�g���N�^
	  // ���^
		const_object_ptr(const object_manager<T>& o)
			: r_(o.get_object())
		{}
	  // �ÖكL���X�g���s��
		template<class X>
		const_object_ptr(const object_manager<X>& o)
			: r_(o.get_object())
		{}
	// �擾����
		parameter get()const{ return r_; }
		parameter operator*()const{ return r_; }
		const_pointer operator->()const { return &r_; } 
	private:
		parameter r_;
	};
	*/
	
} /* end of namespace oden */
	
#endif
