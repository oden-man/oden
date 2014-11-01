#pragma once

#ifndef ODEN_GAMMO_UI_CTL_LV
#define ODEN_GAMMO_UI_CTL_LV

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
	class header_ctl;
//________________________________________________________________________________
//
//
//            ���X�g�r���[ �R���g���[�� �N���X
//
//________________________________________________________________________________
//
//################################################################
//  �J����
//
	ODEN_STRUCT( list_view_column, LVCOLUMN )
	{		
		struct set
		{
			Me& format( int fmt ) {
				self::val(this).fmt = fmt;
				self::val(this).mask|=LVCF_FMT;
				return self::me(this);	
			}
			Me& width( int w ) {
				self::val(this).cx=w;
				self::val(this).mask|=LVCF_WIDTH;
				return self::me(this);	
			}
			Me& text( string_ptr str ) {
				self::val(this).pszText = const_cast<LPTSTR>( str.c_str() ); // �L���X�g����OK
				self::val(this).mask|=LVCF_TEXT;
				return self::me(this);
			}
			Me& text_buf( string& out ) {	// �o�b�t�@�̐ݒ�
				self::val(this).cchTextMax = out.size();
				return text(out);
			}
			Me& sub_item( index i ) {
				self::val(this).iSubItem = i.value();
				self::val(this).mask|=LVCF_SUBITEM;
				return self::me(this);	
			}
			Me& image_index( index i ) {
				self::val(this).iImage = i.value();
				self::val(this).mask|=LVCF_IMAGE;
				return self::me(this);	
			}
			Me& offset( int val ) {
				self::val(this).iOrder = val;
				self::val(this).mask|=LVCF_ORDER;
				return self::me(this);	
			}
		#if (_WIN32_WINNT >= 0x0600)
			Me& minimum_width( int w ) {
				self::val(this).cxMin = w;
				self::val(this).mask|=LVCF_MINWIDTH;
				return self::me(this);	
			}
			Me& default_width( int w ) {
				self::val(this).cxDefault = w;
				self::val(this).mask|=LVCF_DEFAULTWIDTH;
				return self::me(this);	
			}
		#endif	
		};		
	};
	typedef list_view_column lv_column;
	
	/*
	list_view_column::value_reader ri
	if( !lv.get_column( list_view_column::write(ri).text_buf(out) ) )
		return false;
	ri.text();
	*/
	
//################################################################
//  �A�C�e��
//
	ODEN_STRUCT( list_view_item, LVITEM )
	{
		struct get
		{
			int group_id()const { return self::val(this).iGroupId; }
		};
		
		struct set
		{
		  //
			Me& text( string_ptr str ) {
				self::val(this).mask|=LVIF_TEXT;
				self::val(this).pszText = const_cast<LPTSTR>( str.c_str() );
				return self::me(this);
			}
		  // �o�b�t�@�̐ݒ�
			Me& text_buf( string& out ) {
				self::val(this).cchTextMax = out.size();
				return text(out);
			}
		  //
			Me& param( LPARAM lParam ) {
				self::val(this).mask|=LVIF_PARAM;
				self::val(this).lParam = lParam;
				return self::me(this);
			}
			Me& icon_index( index i ) { 
				self::val(this).mask|=LVIF_IMAGE;	
				self::val(this).iImage = i.value();
				return self::me(this);
			}
			Me& at( index i ) {
				self::val(this).iItem = i.value();
				return self::me(this);
			}
			Me& sub_at( index i ) {
				self::val(this).iSubItem = i.value();
				return self::me(this);
			}
			Me& group_id( index i ) {
				self::val(this).mask|=LVIF_GROUPID;
				self::val(this).iGroupId = i.value();
				return self::me(this);
			}

			Me& mask( UINT m ) {
				self::val(this).mask = m;
				return self::me(this);
			}
		#if (_WIN32_WINNT >= 0x0600)	  	
		#endif
		};
	};
	typedef list_view_item lv_item;
	
//################################################################
//  �O���[�v
//
	ODEN_STRUCT( list_view_group, LVGROUP )
	{
		struct get
		{};
		
		struct set
		{
			Me& id( index i ) {
				self::val(this).mask|=LVGF_GROUPID;
				self::val(this).iGroupId = i.value();
				return self::me(this);
			}
			Me& header( string_ptr str ) {
				self::val(this).mask|=LVGF_HEADER;
				self::val(this).pszHeader = const_cast<LPTSTR>( str.c_str() );
				return self::me(this);
			}
			Me& align( UINT Align ) {
				self::val(this).mask|=LVGF_ALIGN;
				self::val(this).uAlign=Align;
				return self::me(this);
			}
			Me& footer( string_ptr str ) {
				self::val(this).mask|=LVGF_FOOTER;
				self::val(this).pszFooter = const_cast<LPTSTR>( str.c_str() );
				return self::me(this);
			}

		#if (_WIN32_WINNT >= 0x0600)
			Me& task( string_ptr str ) {
				self::val(this).mask|=LVGF_TASK;
				self::val(this).pszTask = const_cast<LPTSTR>( str.c_str() );
				return self::me(this);
			}
			Me& subtitle( string_ptr str ) {
				self::val(this).mask|=LVGF_SUBTITLE;
				self::val(this).pszSubtitle = const_cast<LPTSTR>( str.c_str() );
				return self::me(this);
			}
		#endif
		};
			
		struct def
		{
			def( value_type& out )
			{
				out.cbSize = self::value_size();
			}
		};
	};
	typedef list_view_group lv_group;

//################################################################
//  ��������
//
	ODEN_STRUCT( list_view_find_info, LVFINDINFO )
	{
		struct get
		{};
		
		struct set
		{
		  // �e�L�X�g�̈�v
			Me& text( string_ptr str ) 
			{ self::val(this).flags|=(LVFI_STRING|LVFI_PARTIAL); self::val(this).psz = str.c_str(); return self::me(this); }
		  // �e�L�X�g�̊��S�Ȉ�v
			Me& text_exact( string_ptr str ) 
			{
				self::val(this).flags|=LVFI_STRING;
				self::val(this).flags&=~LVFI_PARTIAL;
				self::val(this).psz = str.c_str();
				return self::me(this);
			}
		  // wrap����(�������)
			Me& wrap() 
			{ self::val(this).flags|=LVFI_WRAP; return self::me(this); }
		  // lParam�̈�v
			Me& lparam( LPARAM lParam ) 
			{ self::val(this).lParam = lParam; self::val(this).flags |= LVFI_PARAM; return self::me(this); }
		  // �ʒu�ƕ����ł�����
			Me& near_at( const point& pos, UINT vk_direct ) 
			{				
				self::val(this).flags |= LVFI_NEARESTXY;
				pos.get_point(self::val(this).pt);
				self::val(this).vkDirection = vk_direct;
				return self::me(this);
			}
		};
	};
	typedef list_view_find_info lv_find_info;
	
//#############################################################
//  �q�b�g�e�X�g
//
	ODEN_STRUCT( list_view_hittest_info, LVHITTESTINFO )
	{
		struct get
		{
			point position()const	{ return self::val(this).pt; }
			index item()const		{ return self::val(this).iItem; }
			index sub_item()const	{ return self::val(this).iSubItem; }
			index group()const		{ return self::val(this).iGroup; }
			UINT  state()const		{ return self::val(this).flags; }
		};
		
		struct set
		{
			Me& position( const point& pt ) 
			{
				pt.get_point( self::val(this).pt );
				return self::me(this);
			}
		};
	};
	typedef list_view_hittest_info lv_hittest_info;
	
	
//################################################################
//  �R���g���[���N���X
//
	class list_view
		: public window
	{
	private:
		typedef list_view self;
	public:	
	// �R���X�g���N�^
		list_view( window_handle wh = boost::none )
			: window( wh )
		{}
		
	// �����o�֐�
	  // �J��������
		// �V�����ǉ�
		index insert( index at, const list_view_column::value_type &desc )const;
		index append( const list_view_column::value_type &desc )const;
		// ��폜
		bool erase_column( index iColm )const;
		// �f�[�^���擾
		bool get( index iColm, list_view_column::value_type &out )const;
		// �f�[�^���Z�b�g
		bool set( index iColm, const list_view_column::value_type &desc )const;
		
		// �J�����̕����擾
		UINT column_width( index iColm )const;
		// �J�����̕���ݒ�
		bool set_column_width( index iColm, UINT width = LVSCW_AUTOSIZE )const;
		
		// ������E�̏��ɁA�J������ID���܂Ƃ߂Ď擾
		bool get_column_order( std::vector<int> &out )const;
		// �J�����̕��я���ID�Őݒ�
		bool set_column_order( memory_range<int> order )const;
		
	  //  �A�C�e������
		// �A�C�e������ǉ�
		index insert( const list_view_item::value_type& data )const;
		// �A�C�e�����폜
		bool erase( index iItem )const;
		// �A�C�e����S�č폜
		void clear()const;
		// �A�C�e���f�[�^���擾
		bool get( list_view_item::value_type& out )const;
		string  item_text( index i, index sub = 0 )const;
		rect    item_rect( index i, int code )const;
		rect    item_rect( index i, index sub, int code )const;
		// �A�C�e���f�[�^��ݒ�
		bool set( const list_view_item::value_type &data )const;
		// �A�C�e���̏�Ԃ�ݒ�
		void set_item_state( index i, UINT mask, UINT val )const;
		void clear_item_state( index i, UINT mask )const;
		void select( index i )const   { set_item_state( i, LVIS_SELECTED, LVIS_SELECTED ); }
		void deselect( index i )const { clear_item_state( i, LVIS_SELECTED ); }
		// �A�C�e�������擾
		int count()const;
		// �����ɍ����A�C�e������������
		index find( const list_view_find_info::value_type& cond, index start = boost::none )const;
		// �����ɍ����A�C�e�����擾
		index next( UINT flags, index start = boost::none )const;
		index selection( index start = boost::none )const { return next( LVIS_SELECTED, start ); }
		index focused( index start = boost::none )const   { return next( LVIS_FOCUSED, start ); }
		// �A�C�e�����\�[�g���� ( index, index )
		template< typename Pred >
		bool sort( const Pred &pred )const;	
		// �A�C�e�����\�[�g���� ( lparam, lparam )
		template< typename Pred >
		bool sort_item_param( const Pred &pred )const;
		// �q�b�g�e�X�g
		index hit_test( list_view_hittest_info::value_type& out, bool ex_mode = false )const;
		index sub_item_hit_test( list_view_hittest_info::value_type& out, bool ex_mode = false )const;
		// ��������
		bool ensure_visible(index at)const;
	  
	  // �O���[�v����
		// �A�C�e���O���[�v��ǉ�
		index insert( const list_view_group::value_type& data, index iBefore = -1 )const;
		// �O���[�v���폜
		bool erase_group( index iGroup )const;
		// �O���[�v��S�č폜
		void clear_group()const;
		// �O���[�v�����擾
		int count_group()const;
		// �O���[�v�r���[���[�h�ɂ���
		bool enable_group_view( bool b = true )const;
		
	  // �C���[�W���X�g
		// �A�C�e���p�ɐݒ�(�V�X�e�����Ŏ����j������܂��B)
		bool set_image_list( image_list_handle imgh, int type )const;
		image_list_handle image_list( int type )const;
		// �h���b�O�p�̂𐶐�( �A�v���P�[�V�������j�����ׂ� )
		image_list_handle create_drag_image( index at, const point& pt )const;
	
	  // ���̑�
	    // �r���[
		DWORD view_type()const;
	    bool set_view_type( DWORD val )const;
		// �A�C�e���̈ʒu�𒲐�
		bool arrange( int option = LVA_DEFAULT )const;
		// 
		size approximate_view_size( const size& proposed = size(), integer itemNum = boost::none )const;
		// �w�萔�̃A�C�e���p���������蓖�Ă��s��
		void reserve( int number, DWORD ex_mode_option = 0 )const;
		// �w�b�_�[�R���g���[�����擾
		header_ctl header()const;
		// �g���X�^�C��
		DWORD exstyle()const;
		void set_exstyle( DWORD style )const;
		void set_exstyle( DWORD mask, DWORD style )const;	// mask�͈̔͂�style�̒l�ɂ���
		void add_exstyle( DWORD style )const{ return set_exstyle( style, style ); }
		
      // class id
		static window_class_id get_control_class() { return window_class_id(WC_LISTVIEW); }

	  //
		void calc_best_size( size& out )const;
	};
	
  // sort_item ����
	typedef konbu::callback<PFNLVCOMPARE, 3> list_view_compare_pred;
	
	template< typename Pred >
	bool list_view::sort( const Pred &pred )const
	{
		return 
		FALSE!=ListView_SortItemsEx( 
			handle_value(), 
			&list_view_compare_pred::set<Pred>::entry, 
			reinterpret_cast<LPARAM>( &pred )
		);
	}
	template< typename Pred >
	bool list_view::sort_item_param( const Pred &pred )const
	{
		return 
		FALSE!=ListView_SortItems( 
			handle_value(), 
			&list_view_compare_pred::set<Pred>::entry, 
			reinterpret_cast<LPARAM>( &pred )
		);	
	}

//#########################################################
//  �C�x���g
//
  // NMLISTVIEW
	ODEN_STRUCT( nm_listview, NMLISTVIEW )
	{
		struct get
		{
			nm_header::read_cref header		()const	{ return self::val(this).hdr; }
			index			at				()const	{ return self::val(this).iItem; }
			index			sub_at			()const	{ return self::val(this).iSubItem; }
			UINT			new_state		()const	{ return self::val(this).uNewState; }
			UINT			old_state		()const	{ return self::val(this).uOldState; }
			UINT			changed_state	()const	{ return new_state() ^ old_state(); }
			UINT			changed			()const	{ return self::val(this).uChanged; }
			point			event_location	()const { return self::val(this).ptAction; }
			LPARAM			item_param		()const { return self::val(this).lParam; }
		};
	};

  // NMITEMACTIVATE
	ODEN_STRUCT( nm_item_activate, NMITEMACTIVATE )
	{
		typedef ODEN_PROPERTY_EXTENDS< nm_listview > extends;

		struct get : extends::get
		{
			UINT modifier_key()const 	{ return self::val(this).uKeyFlags; }
		};
	};

  // NMLVCUSTOMDRAW
	ODEN_STRUCT( lv_custom_draw, NMLVCUSTOMDRAW )
	{
		typedef ODEN_PROPERTY_EXTENDS< custom_draw >::template member< &NMLVCUSTOMDRAW::nmcd > extends;
		
		struct get : extends::get
		{
			colorref			text_color		()const	{ return self::val( this ).clrText;  }
			colorref			back_color		()const	{ return self::val( this ).clrTextBk;  }
			index				sub_at			()const { return self::val( this ).iSubItem; }
		};
	};


//
// ���O
//
namespace event {

	typedef notify_def< event_code<NM_DBLCLK> >					list_view_double_click;
	typedef notify_def< event_code<LVN_ITEMCHANGED> >			list_view_item_change;
	typedef notify_def< event_code<NM_CUSTOMDRAW,list_view> >	list_view_custom_draw;

}

  // �G�C���A�X����
	template<> struct event_alias<event::double_click, list_view> { typedef event::list_view_double_click type; };
	template<> struct event_alias<event::item_change, list_view>  { typedef event::list_view_item_change type; };
	template<> struct event_alias<event::custom_draw, list_view>  { typedef event::list_view_custom_draw type; };

//
// ����
//
	template<>
	struct event_arg<event::list_view_double_click> : 
			event::lparam_< event::property_ref< nm_item_activate > >
	  {};

	template<>
	struct event_arg<event::list_view_item_change> :
			event::lparam_< event::property_ref< nm_listview > >
	  {};

	template<>
	struct event_arg<event::list_view_custom_draw> : 
		event::result_< event::value_object< brush_handle > >, event::lparam_< event::property_ref< lv_custom_draw > >
	 {};

}}

#endif
