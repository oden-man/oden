#pragma once

#ifndef ODEN_GAMMO_CMN_INDIRECT
#define ODEN_GAMMO_CMN_INDIRECT

//#############################################
// インクルード
//

// --------- defs -----------
// -------- win -----------
// -------- boost ---------
#include <boost/pointee.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/has_dereference.hpp>
#include <boost/type_traits/add_lvalue_reference.hpp>
// -------- oden ----------

//
//
//#############################################


namespace oden { namespace gammo
{
//####################################################################
//  ポインタから参照先を得る
//
	template< typename T >
	struct indirect
	{
	private:
		enum { has_deref = boost::has_dereference<T>::value };

		template< typename X, bool B >
		struct pointee_
		{
			typedef typename boost::remove_cv<X>::type		no_cv;
			typedef typename boost::pointee<no_cv>::type 	type;
		};
		template< typename X >
		struct pointee_< X, false >
		{
			typedef X type;
		};
		
	public:		
		typedef T																pointer_type;
		typedef typename pointee_<pointer_type, has_deref>::type 				pointee_type;
		typedef typename boost::add_lvalue_reference<pointee_type>::type 		pointee_ref;
		typedef typename boost::add_lvalue_reference<pointer_type>::type		pointer_ref;
		
	private:
		template< bool B >
		struct indirect_impl
		{
			static pointee_ref get( pointer_ref ptr ){ return *ptr; }
		};
		template<>
		struct indirect_impl< false >
		{
			static pointee_ref get( pointer_ref r ){ return r; }
		};
		
		typedef indirect_impl<has_deref> impl;
		
	public:
		typedef pointee_type type;		
		
		static pointee_ref get( pointer_ref x ){ return impl::get(x); }	
	};
	
	
}
}


#endif
