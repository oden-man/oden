#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_INDWIDGET
#define ODEN_GAMMO_UI_WIDGETS_INDWIDGET

//#############################################
//  インクルード
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
//  コントロール型を取得
// 
	template< typename T >
	struct indirect_widget
	{
		// ポインタを外し、cv修飾を外す
		typedef typename boost::remove_cv< typename indirect<T>::type >::type raw;
		
		// 
		typedef typename indirect<T>::pointer_ref pointer_ref;
		typedef typename indirect<T>::pointee_ref pointee_ref;
			
		// ウィンドウ型ならそのまま、ウィジェット型ならさらにcontrol_typeを取り出す
		template< typename W >
		struct inspect
		{		
			// widget SFINAE用
			template< typename X >
			static char test_widget( const widget<X>& );

			typedef char( &not_widget )[2];
			static not_widget test_widget( ... );

			static const W& sample; // 実際に中身を参照しないので、これでよし

			// 型を取り出す
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
		
		// 取得する
		static pointee_ref get( pointer_ref p )
		{
			return formula::get(p);
		}
	};

}}

#endif








