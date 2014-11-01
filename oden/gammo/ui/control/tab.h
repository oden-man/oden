#pragma once

#ifndef ODEN_GAMMO_UI_CTL_TAB
#define ODEN_GAMMO_UI_CTL_TAB

//#######################################################
//  �C���N���[�h
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden ----------
#include <oden/konbu/callback.h>
//----------- gammo ----------
#include <oden/gammo/ui/control/general.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            �^�u�R���g���[�� �N���X
//
//________________________________________________________________________________
//
//################################################################
//  �R���g���[���N���X
//
	class tab
		: public window
	{
	private:
		typedef tab self;
	public:	
	// �R���X�g���N�^
		tab( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_TABCONTROL); }

	  //
		// void calc_best_size( size& out )const;
	};

//################################################################
//  �A�C�e��
//
	ODEN_STRUCT( tab_item, LVCOLUMN )
	{		
		struct set
		{
			Me& text( string_ptr str ) 
			{
				self::val(this).pszText = const_cast<LPTSTR>( str.c_str() ); // �L���X�g����OK
				self::val(this).mask |= TCIF_TEXT;
				return self::me(this);
			}
			Me& text_buf( string& out ) // �o�b�t�@�̐ݒ�
			{	
				self::val(this).cchTextMax = out.size();
				return text(out);
			}
			Me& image_index( index i ) 
			{
				self::val(this).iImage = i.value();
				self::val(this).mask |= TCIF_IMAGE;
				return self::me(this);	
			}
			Me& param( LPARAM lparam ) 
			{
				self::val(this).lParam = lparam;
				self::val(this).mask |= TCIF_PARAM;
				return self::me(this);	
			}
		};		
	};
	
//#############################################################
//  �q�b�g�e�X�g
//
	ODEN_STRUCT( tab_hittest_info, TCHITTESTINFO )
	{
		struct get 
		{
			point position()const	{ return self::val(this).pt; }
			UINT  state()const		{ return self::val(this).flags; }
		};
		
		struct set 
		{
			Me& position( const point& pt ) 
			{
				get_point( pt, self::val(this).pt );
				return self::me(this);
			}
		};
	};	

//#########################################################
//  �C�x���g
//
  // �A�C�e���}��
	GAMMO_DEFINE_EVENT( notify_def, tab_insert_item, TCM_INSERTITEM, 2 )
	
	template<>
	struct event_arg<event::tab_insert_item> :
		event::result_< event::value_object<index> >,
		event::wparam_lparam< event::value_object<index>, event::property_cref<tab_item> >
	{};

  // �A�C�e����
	GAMMO_DEFINE_EVENT( notify_def, tab_count_item, TCM_GETITEMCOUNT, 0 )
	
	template<>
	struct event_arg<event::tab_count_item> :
		event::result_< event::value<size_t> >,
		event::noparam
	{};

  // �A�C�e���擾
	GAMMO_DEFINE_EVENT( notify_def, tab_get_item, TCM_GETITEM, 2 )
	
	template<>
	struct event_arg<event::tab_get_item> :
		event::result_< event::bool_<> >,
		event::wparam_lparam< event::value_object<index>, event::property_ref<tab_item> >
	{};
	
}}

#endif
