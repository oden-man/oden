#pragma once

#ifndef ODEN_GAMMO_UI_MENU
#define ODEN_GAMMO_UI_MENU

//#######################################################
//  �C���N���[�h
//
// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
// --------- win ------------

// --------- boost ----------
// --------- oden ----------
#include <oden/property.h>
#include <oden/gammo/ui/window/message.h>
//----------- gammo ----------

//
//###############################################


//___________________________________________________
//
//  �O���錾
//___________________________________________________
//
namespace oden { namespace gammo
{		
//_______________________________________________________________________________________
//
//
//     ���j���[�N���X
//
//_______________________________________________________________________________________
//
//########################################################
// ���j���[�A�C�e��
//
	ODEN_STRUCT( menu_item_info, MENUITEMINFO )
	{
		struct set
		{ 
		  // ���
			Me& state( UINT v ) 
			{
				self::val(this).fMask |= MIIM_STATE;
				self::val(this).fState |= v;
				return self::me(this);
			}
			Me& id( UINT v ) 
			{
				self::val(this).fMask |= MIIM_ID;
				self::val(this).wID = v;
				return self::me(this);
			}
			Me& sub( menu_handle mh ) 
			{
				self::val(this).fMask |= MIIM_SUBMENU;
				self::val(this).hSubMenu = mh.value();
				return self::me(this);
			}
			Me& check_mark( bool checked, bitmap_handle bh ) 
			{
				self::val(this).fMask |= MIIM_CHECKMARKS;
				if( checked ) {
					self::val(this).hbmpChecked = bh.value();
				}else{
					self::val(this).hbmpUnchecked = bh.value();
				}
				return self::me(this);				
			}
			Me& lparam( ULONG_PTR v ) 
			{
				self::val(this).fMask |= MIIM_DATA;
				self::val(this).dwItemData = v;
				return self::me(this);
			}
			
		  // �A�C�e���̎�� 
			Me& type( UINT v, LPTSTR data = 0 ) 
			{
				self::val(this).fMask |= MIIM_FTYPE;
				self::val(this).fType |= v;
				self::val(this).dwTypeData = data;
				return self::me(this);
			}
			
			Me& separator() 
			{ return type( MFT_SEPARATOR ); }
			
			Me& radiocheck() 
			{ return type( MFT_RADIOCHECK ); }

			Me& text( string_ptr str ) 
			{
				self::val(this).fMask |= MIIM_STRING;
				return type( MFT_STRING, const_cast<LPTSTR>(str.c_str()) );
			}
			Me& text_buf( string& out )
			{
				self::val(this).cch = out.size();
				return text( out.c_str() );
			}
		
			Me& ownerdraw( ULONG_PTR lparam = 0 ) 
			{ return type( MFT_OWNERDRAW, reinterpret_cast<LPTSTR>(lparam) ); }
			
			Me& new_line( bool bar = false )
			{ return type( bar ? MFT_MENUBARBREAK : MFT_MENUBREAK ); }

		  // �r�b�g�}�b�v
			Me& bitmap( bitmap_handle bh ) 
			{
				self::val(this).fMask |= MIIM_BITMAP;
				self::val(this).hbmpItem = bh.value();
				self::me(this);
			}

			Me& bitmap_ownerdraw() // �I�[�i�[�h���[�Ńr�b�g�}�b�v�`��
			{ return bitmap( HBMMENU_CALLBACK ); }

			Me& window_icon( window_handle wh ) // �w��E�B���h�E�̃A�C�R����`��
			{
				bitmap( HBMMENU_SYSTEM );
				return lparam( reinterpret_cast<LPARAM>( wh.value() ) );
			}
		};

		
		struct def
		{ 
			def( value_type& out )
			{
				out.cbSize = self::value_size();
			}
		};
	};
	typedef menu_item_info menu_item;

//########################################################
// ���j���[�R���g���[��
//
	class menu
		: public handle_interface<menu_handle>
	{
	public:
	// �R���X�g���N�^
		menu( menu_handle mh = boost::none )
			: handle_interface<menu_handle>(mh)
		{}

	// �f���[�^
		typedef struct destroyer {
			void operator()( menu& o )const { o.destroy(); }
		} deleter_type;

	// �����o�֐�
      // ���j���[
		// ���̃��j���[��j������
		bool destroy()const;
		// �T�u���j���[���擾
		menu_handle sub_at( index pos )const;
		// �w��̃X�N���[���ӏ��ɕ\������
		positive track_popup( window_handle owner, const point& screen_pt, UINT flag = 0 )const;
		positive track_popup( window_handle owner, const point& screen_pt, const rect& ex_rect, UINT flag = 0  )const;

	  // �A�C�e��
		// �ǉ�
		bool insert ( const menu_item::value_type& mifo, menu_item_id id = boost::none )const;
		bool insert_at ( const menu_item::value_type& mifo, index at )const;
		// �ύX
		bool set ( const menu_item::value_type& mifo, index at = boost::none )const;
		// �擾
		bool get ( menu_item::value_type& out, index at = boost::none )const;
		// �j�󂷂�
		bool erase( menu_item_id id, bool by_index = false )const;
		bool erase_at ( index at )const { return erase( at.value(), true ); }
		// �T�u���j���[��؂藣��
		bool detach ( menu_item_id id, bool by_index = false )const ;
		bool detach_at ( index at )const { return detach( at.value(), true ); }
		// �n�C���C�g����
		bool hilite ( window_handle wh, menu_item_id id, bool on = true, bool by_index = false )const;
		bool hilite_at ( window_handle wh, index at, bool on = true )const { return hilite( wh, at.value(), on, true ); }
		// �A�C�e���̐�
		integer count()const;

		// �f�t�H���g�őI������Ă���A�C�e��
		bool set_default ( menu_item_id id, bool by_index = false )const;
		bool set_default_at ( index at )const { return set_default( at.value(), true ); }
		
		menu_item_id get_default ( UINT flags = 0, bool by_index = false )const;
		index get_default_index ( UINT flags = 0 )const { return static_cast<int>( get_default(flags, true).value() ); }

	};
		
//########################################################
// ���j���[�쐬�֐�
//	
	//  ��̃��j���[������
	menu create_menu();
	menu create_popup_menu();

	// ���\�[�X����ǂݏo��
	menu load_menu( res_id id, instance_handle ih = boost::none );

//########################################################
// ���j���[�C�x���g
//
  // ���j���[���N���b�N���ꂽ��
	GAMMO_DEFINE_EVENT_ALT( event_def, menu_click, WM_COMMAND, 1000, 3 )

	template<>
	struct event_arg<event::menu_click> :
		event::wparam_< event::value_object<integer> >
	{};

	template<>
	struct event_branch<event::menu_click>
	{
		static bool branch( window_event& ev )
		{
			return ev.code() == WM_COMMAND && ev.wparam_high() == 0;
		}

		static bool branch( window_event& ev, UINT id )
		{
			if( branch( ev ) )
				return ev.wparam_low() == id;
			return false;
		}	
	};

  // ���j���[�ɑI���J�[�\�����ڂ�����
	GAMMO_DEFINE_EVENT( event_def, menu_select, WM_MENUSELECT, 3 )

	template<>
	struct event_arg<event::menu_select> :
		event::wparam_lparam< event::value_object<index>, event::value<UINT>, event::value_object<menu_handle> >
	{};

  // ���j���[�ɑI���J�[�\�����ڂ������A����Ƀ|�b�v�A�b�v���j���[�������Ȃ��ꍇ
	GAMMO_DEFINE_EVENT_ALT( event_def, menu_cmd_select, WM_MENUSELECT, 1000, 3 )

	template<>
	struct event_arg<event::menu_cmd_select> :
		event::wparam_lparam< event::value_object<integer>, event::value<UINT>, event::value_object<menu_handle> >
	{};


  // ����
	namespace event
	{
		// ���j���[������ꂽ�ꍇ���H
		static bool is_menu_closed( window_event& ev )
		{
			if( get_event_arg<event::menu_select, event::_3>( ev ) == menu_handle() )
				return true;
			return false;
		}

		static bool compare_menu_id( window_event& ev, int index, bool by_index )
		{
			int src = get_event_arg<event::menu_select, event::_1>( ev ).value();
			if( index == src ) {
				return by_index == ((get_event_arg<event::menu_select, event::_2>( ev ) & MF_POPUP) != 0);
			}
			return false;
		}
	}

	template<>
	struct event_branch<event::menu_select> 
	{
		static bool branch( window_event& ev )
		{
			return ev.code() == WM_MENUSELECT && !event::is_menu_closed( ev );
		}
	
		static bool branch( window_event& ev, int idx )
		{
			if( branch(ev) )
				return event::compare_menu_id( ev, idx, true );
			return false;
		}
	};

	template<>
	struct event_branch<event::menu_cmd_select>
	{
		static bool branch( window_event& ev )
		{
			return ev.code() == WM_MENUSELECT && !event::is_menu_closed( ev );
		}

		static bool branch( window_event& ev, int id )
		{
			if( branch( ev ) )
				return event::compare_menu_id( ev, id, false );
			return false;
		}
	};




}}

#endif
