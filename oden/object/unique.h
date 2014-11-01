#pragma once

#ifndef ODEN_GAMMO_CMN_OBJ_UNQ
#define ODEN_GAMMO_CMN_OBJ_UNQ

//#####################################################
//  �C���N���[�h
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
*  Base - (const)object_manager�̔h���łȂ���΂Ȃ�Ȃ��B
*  Deleter - pointer�^�������Ă��Ȃ���΂Ȃ�Ȃ��B
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
		
	// �����o�֐�
	  // �I�u�W�F�N�g�̎擾
		param_type get_object()const{ return object_; }
		reference get_object_ref(){ return object_; }
	  // reset
	  	void reset(){ object_.reset(); }
	  	void reset(param_type newobj){ object_.reset(newobj); }
	  // �f���[�^�̎擾
		deleter& get_deleter() { return object_.get_deleter(); } 
		const deleter& get_deleter()const { return object_.get_deleter(); }
	// ���Z�q
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
	// �R���X�g���N�^
		unique_object( 
			param_type elem, 
			const deleter& deler = deleter()
			):
		  object_(elem, deler)
		{}			
	private:
		object object_;	// �Ǘ��I�u�W�F�N�g
	};
}}
	
#endif
