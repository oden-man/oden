#pragma once

#ifndef ODEN_GAMMO_CMN_OBJ_UNQ
#define ODEN_GAMMO_CMN_OBJ_UNQ

//#####################################################
//  インクルード
//

// ------ defs -----------
// ------ STL ---------
// ----- boost --------
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

// ------- oden ---------
#include <oden/disallow.h>

// ------- gammo --------
#include <oden/gammo/common/object/interface.h>


//
//
//###############################################################

namespace oden { namespace gammo
{
//###########################################################################
//  unique_ptr
//	
/*
*  Base - (const)object_managerの派生でなければならない。
*  Deleter - pointer型をもっていなければならない。
*/
	template< class T, class Deleter >
	class unique_object
		: public object_facade<T>
	{
	public:		
		typedef unique_object<T,Deleter> self_type;
		typedef object_manager<T> base;	
		typedef Deleter	deleter;
		typedef typename base::reference reference;
		typedef typename base::param_type param_type;
		typedef typename base::element element;
	
		typedef boost::interprocess::unique_ptr<element,deleter> object;
		
	// メンバ関数
	  // オブジェクトの取得
		param_type get_object()const{ return object_; }
		reference get_object_ref(){ return object_; }
	  // reset
	  	void reset(){ object_.reset(); }
	  	void reset(param_type newobj){ object_.reset(newobj); }
	  // デリータの取得
		deleter& get_deleter() { return object_.get_deleter(); } 
		const deleter& get_deleter()const { return object_.get_deleter(); }
	// 演算子
		self_type& operator=(param_type e) {
			reset(e); 
			return (*this);	
		}
		param_type operator*() {
			return get_object();
		}
		pointer operator->() {
			return &get_object_ref();
		}
	public:		
	// コンストラクタ
		unique_object( 
			param_type elem, 
			const deleter& deler = deleter()
			):
		  object_(elem, deler)
		{}			
	private:
		object object_;	// 管理オブジェクト
	};
}}
	
#endif
