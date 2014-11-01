#pragma once

#ifndef ODEN_OBJ_SCP
#define ODEN_OBJ_SCP

//#####################################################
//  インクルード
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
*  Base - (const)object_managerの派生でなければならない。
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
		
	// コンストラクタ
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
	// デストラクタ
		~scoped()
		{
			this->delete_object();
		}
		
	// メンバ関数
	  // オブジェクトの取得
		typename self::reference get( ) { return data_.first(); }
		typename self::param_type get( )const { return data_.first(); }
	  // デリータの取得
		deleter& get_deleter() { return data_.second(); } 
		const deleter& get_deleter()const { return data_.second(); }
	  // 解放
		void delete_object(){ get_deleter()( get() ); }
		
	// swap
		void swap( scoped& r ) 
		 { using std::swap; swap( data_, r.data_ ); }
		friend void swap( scoped& l, scoped& r ) 
		 { l.swap(r); }
	private:
	// コピー禁止!
		ODEN_NO_COPY_CLASS( self )
		
	private:
		boost::compressed_pair<T,deleter> data_;	// 管理オブジェクト
	};
}
	
#endif
