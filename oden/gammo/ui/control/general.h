#pragma once
#pragma warning ( disable: 4503 ) 

#ifndef ODEN_GAMMO_UI_CTL_GEN
#define ODEN_GAMMO_UI_CTL_GEN

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>

// --------- stl ------------

// --------- win ------------
#include <commctrl.h>

// --------- boost ----------
// --------- oden ----------
//----------- gammo ----------
#include <oden/gammo/ui/window/window.h>

//
//#################################################



namespace oden { namespace gammo
{

//________________________________________________________________________________
//
//
//            コントロールクラス共通要素
//
//________________________________________________________________________________
//	
//#############################################################
// typedef 
//	
	typedef nullable< HIMAGELIST, singular_null<HIMAGELIST,0> >	image_list_handle;

//#############################################################
//  InitCommonControl
//	
	inline bool init_common_controls( DWORD classes = ICC_WIN95_CLASSES )
	{
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof( INITCOMMONCONTROLSEX );
		icc.dwICC = classes;

		return FALSE != InitCommonControlsEx(&icc);
	}

//___________________________________________________________________________________
//
//
//           共通イベント
//
//___________________________________________________________________________________
//
//###################################################################
//  構造体
//
  // NMCHAR
	ODEN_STRUCT( nm_char, NMCHAR )
	{
		struct set
		{};

		struct get
		{
			nm_header::read_cref header()const { return self::val(this).hdr; }
			UINT  char_code()const { return self::val(this).ch; }
			DWORD prev_item()const { return self::val(this).dwItemPrev; }	
			DWORD next_item()const { return self::val(this).dwItemNext; }
		};
	};

  // NMCUSTOMDRAW
	ODEN_STRUCT( custom_draw, NMCUSTOMDRAW )
	{
		struct set
		{};

		struct get
		{
			nm_header::read_cref header()	const { return self::val( this ).hdr; }
			DWORD			stage()			const { return self::val( this ).dwDrawStage; }
			dc_handle		dc()			const { return self::val( this ).hdc; }
			rect			rect()			const { return make_rect( self::val( this ).rc ); }
			DWORD			item_spec()		const { return self::val( this ).dwItemSpec; }
			UINT			item_state()	const { return self::val( this ).uItemState; }
			LPARAM			item_data()		const { return self::val( this ).litemlParam; }
		};
	};
	
  // MEASUREITEMSTRUCT
	ODEN_STRUCT( measure_item, MEASUREITEMSTRUCT )
	{
		struct set
		{
			Me& item_size( const size& dim ) 
			{ self::val(this).itemWidth = dim.width(); self::val(this).itemHeight = dim.height(); return self::me(this); }
		};

		struct get
		{
			UINT control_type()		const { return self::val(this).CtlType; }
			UINT control_id()		const { return self::val(this).CtlID; }
			index item_id()			const { return static_cast<index::value_type>( self::val(this).itemID ); }
			ULONG_PTR item_data()	const { return self::val(this).itemData; }
		};
	};

  // DRAWITEMSTRUCT
	ODEN_STRUCT( draw_item, DRAWITEMSTRUCT )
	{
		struct set
		{};

		struct get
		{
			UINT control_type()			const { return self::val(this).CtlType; }
			UINT control_id()			const { return self::val(this).CtlID; }
			index item_id()				const { return static_cast<index::value_type>( self::val(this).itemID ); }
			UINT item_action()			const { return self::val(this).itemAction; }
			UINT item_state()			const { return self::val(this).itemState; }
			window_handle control()		const { return self::val(this).hwndItem; }
			dc_handle dc()				const { return self::val(this).hDC; }
			rect item_rect()			const { return make_rect( self::val(this).rcItem ); }			
			ULONG_PTR item_data()		const { return self::val(this).itemData; }
		};
	};
	
//###############################################################
//  イベント定義
//
  //
	GAMMO_DEFINE_EVENT( notify_def, nm_char, NM_CHAR, 1 )

	template<>
	struct event_arg<event::nm_char> : 
		event::lparam_< event::property_cref< nm_char > >
	{};
	  
  //
	GAMMO_DEFINE_EVENT( notify_def, nm_custom_draw, NM_CUSTOMDRAW, 1 )	 

	template<>
	struct event_arg<event::nm_custom_draw> :
		event::result_< event::value<DWORD> >, 
		event::lparam_< event::property_cref< custom_draw > >
	{};
	
  // 
	GAMMO_DEFINE_EVENT( event_def, draw_item, WM_DRAWITEM, 1 )	

	template<>
	struct event_arg<event::draw_item> : 
		event::lparam_< event::property_cref< draw_item > >
	{};
	
	template<>
	struct event_branch<event::draw_item>
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& arg1 )
		{
			if( branch(ev) )
			{
				int id = static_cast<int>( get_event_arg< event::draw_item, event::_1 >(ev).CtlID );
				return branch_child_id( id, arg1 );
			}
			return false;
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_DRAWITEM;
		}
	};
	
  //
	GAMMO_DEFINE_EVENT( event_def, measure_item, WM_MEASUREITEM, 1 )	
	
	template<>
	struct event_arg<event::measure_item> : 
		event::lparam_< event::property_ref< measure_item > >
	{};
	
	template<>
	struct event_branch<event::measure_item>
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& arg1 )
		{
			if( branch(ev) )
			{
				int id = static_cast<int>( get_event_arg< event::measure_item, event::_1 >(ev).CtlID );
				return branch_child_id( id, arg1 );
			}
			return false;
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_MEASUREITEM;
		}
	};
	

  //
  // エイリアス
  //
	namespace event 
	{
		struct click;
		struct double_click;

		struct change;
		struct sel_change;  // 
		struct item_change; // list_view, tree_view

		struct color;

		struct custom_draw;
	}

// エイリアス解決
  // NM_CUSTOMDRAW その他
	template< typename Other > 
	struct event_alias< event::custom_draw, Other > { typedef event::nm_custom_draw type; };

//
//  返り値
//	
  // 色を設定
	//inline void control_color_brush( event_result& e, brush_handle bh )
	//{
	//	e.set<...>( bh );
	//}
	
  // カスタムドロー
	//inline void custom_draw_result( event_result& e, DWORD val )
	//{
	//	e.set<
	//}
	
	
	/*
	class control_color : event_result
	{
	public:
		control_color( const event_result& r )
			: event_result(r)
		{}
	
		void set_brush( brush_handle bh )
		{
			this->set_handle( bh.value() );
		}
	};
	
  // CUSTOMDRAW
	class custom_draw_result : protected event_result
	{
	public:
		custom_draw_result( const event_result& r )
			: event_result(r)
		{}
		
		void set( DWORD val )const
		{
			this->set_value(val);
		}
	};
	*/
	
}}

#endif
