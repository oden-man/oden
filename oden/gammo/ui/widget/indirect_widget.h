#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_INDWIDGET
#define ODEN_GAMMO_UI_WIDGETS_INDWIDGET

//#############################################
//  �C���N���[�h
//
// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl -------------
// --------- win ------------
// --------- boost ----------
#include <boost/mpl/eval_if.hpp>
// --------- oden  ----------
// --------- gammo --------
#include <oden/gammo/common/indirect.h>

//
//############################################


namespace oden { namespace gammo
{
	template< typename X >
	class widget;

//############################################################
//  �R���g���[���^���擾
// 
	template< typename T >
	struct indirect_widget
	{
		// �|�C���^���O���Acv�C�����O��
		typedef typename boost::remove_cv< typename indirect<T>::type >::type raw;
		
		// 
		typedef typename indirect<T>::pointer_ref pointer_ref;
		typedef typename indirect<T>::pointee_ref pointee_ref;
			
		// �E�B���h�E�^�Ȃ炻�̂܂܁A�E�B�W�F�b�g�^�Ȃ炳���control_type�����o��
		template< typename W >
		struct inspect
		{		
			// widget SFINAE�p
			template< typename X >
			static char test_widget( const widget<X>& );

			typedef char( &not_widget )[2];
			static not_widget test_widget( ... );

			static const W& sample; // ���ۂɒ��g���Q�Ƃ��Ȃ��̂ŁA����ł悵

			// �^�����o��
			template< typename T > 
			struct control_type 
			{ 
				typedef typename W::control_type type;
				static pointee_ref get( pointer_ref p ){ return indirect<T>::get(p).to_control(); }
			};
			
			template< typename T > 
			struct identitical_type 
			{ 
				typedef T type; 
				static pointee_ref get( pointer_ref p ){ return indirect<T>::get(p); }
			};

			typedef typename boost::mpl::if_c< 
						sizeof( test_widget(sample) ) != sizeof(not_widget),
						control_type<W>,
						identitical_type<W>
				>::type
			formula;
		};
		
		
		typedef typename inspect<raw>::formula formula;	
		typedef typename formula::type type;	
		
		// �擾����
		static pointee_ref get( pointer_ref p )
		{
			return formula::get(p);
		}
	};

}}

#endif








