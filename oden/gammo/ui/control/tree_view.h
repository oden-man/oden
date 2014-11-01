#pragma once

#ifndef ODEN_GAMMO_UI_CTL_TV
#define ODEN_GAMMO_UI_CTL_TV

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
//            �c���[�r���[ �R���g���[�� �N���X
//
//________________________________________________________________________________
//

// typedef
//
	typedef nullable< HTREEITEM, singular_null<HTREEITEM,0> > tree_item_handle;

//################################################################
//  �A�C�e��
//
	ODEN_STRUCT( tree_view_item, TVITEMEX )
	{
		struct get
		{};

		struct set
		{
			Me& mask( UINT m )
			{ self::val(this).mask |= m; return self::me(this); }

			Me& state( UINT mask, UINT u )
			{ self::val(this).mask |= TVIF_STATE;
			  self::val(this).state |= u;
			  self::val(this).stateMask |= mask; return self::me(this); }

			Me& text( string_ptr s )
			{ self::val(this).mask |= TVIF_TEXT;
			  self::val(this).pszText = const_cast<LPTSTR>(s.c_str()); return self::me(this); }

			Me& image( index i )
			{ self::val(this).mask |= TVIF_IMAGE;
			  self::val(this).iImage = i.value(); return self::me(this); }

			Me& selected_image( index i )
			{ self::val(this).mask |= TVIF_SELECTEDIMAGE;
			  self::val(this).iSelectedImage = i.value(); return self::me(this); }

			Me& param( LPARAM val )
			{ self::val(this).mask |= TVIF_PARAM;
			  self::val(this).lParam = val; return self::me(this); }

			Me& child( bool b, int value = 1 ) /* true, I_CHILDCALLBACK�Ɛݒ�ł��� */
			{ 
				self::val(this).mask |= TVIF_CHILDREN;
				self::val(this).cChildren = (b ? value : 0);
				return self::me(this);
			}

		  	Me& target( tree_item_handle h )
			{ self::val(this).mask |= TVIF_HANDLE;
			  self::val(this).hItem = h.value(); return self::me(this); }

		  	Me& text_out( string& buf )
			{ text( buf ); self::val(this).cchTextMax = buf.size(); return self::me(this); }

			Me& height_scale( int v = 1 )
			{ self::val(this).mask |= TVIF_INTEGRAL;
			  self::val(this).iIntegral = v; return self::me(this); }

		#if (_WIN32_WINNT >= 0x0600)
			Me& state_ex( UINT u )
			{ self::val(this).mask |= TVIF_STATEEX;
			  self::val(this).uStateEx |= u; return self::me(this); }
		#endif
		};
	};
	typedef tree_view_item tv_item;
	
//################################################################
//  �A�C�e���}��
//
	ODEN_STRUCT( tree_view_inserter, TVINSERTSTRUCT )
	{
	private:
		struct item_value { // TVINSERTSTRUCT�͖������p�̂������A�����o�|�C���^�����Ȃ�
			static TVITEMEX& val( TVINSERTSTRUCT& obj ){ return obj.itemex; }
			static const TVITEMEX& val( const TVINSERTSTRUCT& obj ){ return obj.itemex; }
		};

	public:
		typedef ODEN_PROPERTY_EXTENDS<tree_view_item, item_value> extends;

		struct set : extends::set
		{
			Me& parent( tree_item_handle h )
			{ self::val(this).hParent = h.value(); return self::me(this); }

			Me& insert_after( tree_item_handle h )
			{ self::val(this).hInsertAfter = h.value(); return self::me(this); }
		};
	};
	typedef tree_view_inserter tv_inserter;

//#############################################################
//  �q�b�g�e�X�g
//
	ODEN_STRUCT( tree_view_hittest_info, TVHITTESTINFO )
	{
		struct get
		{
			point position()const	{ return self::val(this).pt; }
			UINT  state()const		{ return self::val(this).flags; }
			tree_item_handle hit()const{ return self::val(this).hItem; }
		};
		
		struct set
		{
			Me& position( const point& pt ) 
			{ pt.get_point( self::val(this).pt ); return self::me(this); }
		};
	};
	typedef tree_view_hittest_info tv_hittest_info;

//#############################################################
//  �\�[�g
//
	typedef konbu::callback< PFNTVCOMPARE, 2 > tree_view_compare_callback, tv_compare_callback;

	ODEN_STRUCT( tree_view_sort, TVSORTCB )
	{		
		struct set
		{
			Me& parent( tree_item_handle h )
			{ self::val(this).hParent = h.value(); return self::me(this); }

			template< typename F >
			Me& comparer( const F& fn )
			{ self::val(this).lpfnCallBack = &tv_compare_callback::set<F>::entry; 
			  self::val(this).lParam = reinterpret_cast<LPARAM>(&fn); return self::me(this); }
		};
	};
	typedef tree_view_sort tv_sort;
		
//################################################################
//  �R���g���[���N���X
//
	class tree_view
		: public window
	{
	private:
		typedef tree_view self;
	public:	
	// �R���X�g���N�^
		tree_view( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �A�C�e���ҏW
		// �}��
		tree_item_handle insert( const tv_inserter::value_type &tvIns )const;
		bool set( const tv_item::value_type& itm )const;
		bool erase( tree_item_handle h )const;
		bool clear()const;
		bool get_item( tv_item::value_type& out )const;
		int count()const;
		// �q�b�g�e�X�g
		tree_item_handle hit_test( tv_hittest_info::value_type& out )const;
		// �q���\�[�g����
		bool sort_child( tree_item_handle h, bool recursive )const;
		bool sort_child( tv_sort::value_type& desc, bool recursive )const;

	  // �A�C�e���擾
		tree_item_handle get_next_item( UINT flag, tree_item_handle h = boost::none )const;
		tree_item_handle selection()const;
		tree_item_handle root_item()const;
		tree_item_handle parent_item( tree_item_handle h )const;
		tree_item_handle child_item( tree_item_handle h )const;
		tree_item_handle prior_item( tree_item_handle h )const;
		tree_item_handle next_item( tree_item_handle h )const;

	  // �A�C�e�����
		// �I����Ԃɂ���
		bool select( tree_item_handle h, UINT flags = TVGN_CARET )const;
		bool select_drop_target( tree_item_handle h )const 			{ return select( h, TVGN_DROPHILITE ); }
		bool set_top_visible( tree_item_handle h )const 				{ return select( h, TVGN_FIRSTVISIBLE ); }
		// �߂��L����E����
		bool expand( tree_item_handle h, UINT flags = TVE_EXPAND )const;
		bool expand_toggle( tree_item_handle h )const				{ return expand( h, TVE_TOGGLE ); }
		bool collapse( tree_item_handle h, bool reset = false )const	{ return expand( h, TVE_COLLAPSE|(reset?TVE_COLLAPSERESET:0) ); }
		// �ҏW���[�h
		window_handle edit_label( tree_item_handle h )const;
		bool end_edit_label( bool bsave = false )const;		
		// ���̃A�C�e����������悤�ɊJ��
		bool ensure_visible( tree_item_handle h )const;
		// �`�F�b�N
		bool get_item_check( tree_item_handle h )const;
		void set_item_check( tree_item_handle h, bool b = true )const;
		// ��ԃt���O
		void set_item_state( tree_item_handle h, UINT mask, UINT state )const;
		UINT get_item_state( tree_item_handle h, UINT mask )const;

	  // �C���[�W���X�g
		image_list_handle set_image_list( int i, image_list_handle h )const;
		image_list_handle image_list( int i )const;
		image_list_handle create_drag_image_list( tree_item_handle h )const;

	  // ���̑�
		// �F
		colorref set_line_color( colorref c )const;
		colorref line_color()const;
		colorref set_back_color( colorref c )const;
		colorref back_color()const;
		colorref set_text_color( colorref c )const;
		colorref text_color()const;
		// �}�[�N
		bool  insert_mark( tree_item_handle h, bool after = true )const;
		colorref mark_color()const;
		colorref set_mark_color( colorref c )const;
		// �g���X�^�C��
		DWORD extended_style()const;
		hresult set_extended_style( DWORD mask, DWORD style )const;	// mask�͈̔͂�style�̒l�ɂ���

      // class id
		static window_class_id get_control_class() { return window_class_id(WC_TREEVIEW); }
	};
}}

#endif
