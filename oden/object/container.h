#pragma once

#ifndef ODEN_OBJ_CONT
#define ODEN_OBJ_CONT

/*
*  ptr_vector�I�ȃR���e�i
*  �X�}�[�g�|�C���^�̌^�ƁA����֐����
*  ��{unique_ptr���g���A�Ȃ���Ύ���()�����N���X�g�|�C���^�ő�p
*
*/

#ifdef ODEN_NO_UNIQUE_PTR
#	define OBJ_CONT_SMP(t)  oden::linked_ptr<t>
#	define OBJ_CONT_MOVE(e) e
#	include <oden/object/linked_ptr.h>
#else
#	define OBJ_CONT_SMP(t)  std::unique_ptr<t>
#	define OBJ_CONT_MOVE(e) std::move(e)
#	include <memory>
#endif

namespace oden 
{		
//#######################################################
//  
//	
	template< typename T >
	struct object_container
	{	
		typedef OBJ_CONT_SMP(T) element;
		
		template< typename X >
		static void reset_elem( element& elem, X* np )
		{
			element sp(np);
			elem = OBJ_CONT_MOVE(sp);
		}
		
		template< typename Cont, typename X >
		static void push_back( Cont& cont, X* np )
		{
			element sp(np);
			cont.push_back( OBJ_CONT_MOVE(sp) );
		}
		
		template< typename Cont, typename X >
		static void push_front( Cont& cont, X* np )
		{
			element sp(np);
			cont.push_front( OBJ_CONT_MOVE(sp) );
		}
		
		template< typename Cont, typename Iter, typename X >
		static void insert( Cont& cont, Iter& itr, X* np )
		{
			element sp(np);
			cont.insert( itr, OBJ_CONT_MOVE(sp) );
		}
	};
	
	
}

#undef OBJ_CONT_MOVE
#undef OBJ_CONT_SMP

#endif
