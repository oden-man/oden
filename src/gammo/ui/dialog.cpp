#include "../Pch.h"

#include <oden/gammo/ui/dialog/dialog.h>
#include <oden/gammo/ui/widget/base.h> // static_dialog_proc �̂���

namespace oden { namespace gammo
{
/*-----------------------------------------------------------------------------------------------------*/
//
// �_�C�A���O
//
/* ---------------------------------------------------------------------------------------------------- */	
//####################################################
//  dialog
//
//
bool dialog::create( res_id dlg_template, window_handle parent, LPARAM init_param, instance_handle ih )
{
	if( !ih )
		ih = get_app_handle();

	if( window_handle wh = ::CreateDialogParam( 
			ih.value(), 
			dlg_template.get(), 
			parent.value(), 
			&widget_hub::static_dialog_proc, 
			init_param 
			) )
	{
		this->attach( wh );
		return true;
	}
	return false;
}

// ���[�_���_�C�A���O���J�n
dlg_result dialog::do_modal( res_id dlg_template, window_handle parent, LPARAM init_param, instance_handle ih )const
{
	if( !ih )
		ih = get_app_handle();

	return ::DialogBoxParam( ih.value(), dlg_template.get(), parent.value(), &widget_hub::static_dialog_proc, init_param );
}

// ���[�_�����I��
bool dialog::end_modal( dlg_result Ret )const
{
	return FALSE != ::EndDialog( handle_value(), Ret.value() );	
}

// �_�C�A���O�ނ��̃��b�Z�[�W����������
bool dialog::process_dlg_message( raw_window_message& out )const
{
	if( FALSE!=::IsDialogMessage( handle_value(), &out ) )
	{
		// debug_string(32,L"hwnd=%x,code=%x\n",out.hwnd,out.message); 
		return true;
	}
	return false;	
}

// �A�C�e�����擾
window_handle dialog::get_item( int i )const
{
	return this->get_child( i );	
}
window dialog::item( int i )const
{
	return this->child( i );	
}

// �A�C�e���̃e�L�X�g���擾
bool	dialog::get_item_text( int item, string &out )const
{
	return window( get_item( item ) ).get_text( out );
}
string dialog::item_text( int item )const
{
	string out;
	get_item_text( item, out );
	return out;	
}

// �A�C�e���̃e�L�X�g��ݒ�
bool dialog::set_item_text( int item, string_ptr str )const
{
	return FALSE!=::SetDlgItemText( handle_value(), item, str.c_str() );
}

//
UINT dialog::item_value( int item, bool s, bool* pbool )const
{
	BOOL b = 0;
	UINT ui = ::GetDlgItemInt( handle_value(), item, &b, s?TRUE:FALSE );

	if( pbool )
		*pbool = ( b != FALSE );
	return ui;
}

//
bool	dialog::set_item_value( int i, UINT val, bool s )const
{
	return FALSE!=::SetDlgItemInt( handle_value(), i, val, s?TRUE:FALSE ); 
}

// �O���[�v�A�C�e�����擾
window_handle dialog::prior_group_item( window_handle begin )const
{
	return ::GetNextDlgGroupItem( handle_value(), begin.value(), TRUE );
}
window_handle dialog::next_group_item( window_handle begin )const
{
	return ::GetNextDlgGroupItem( handle_value(), begin.value(), FALSE );
}

// �^�u�O���[�v�A�C�e�����擾
window_handle dialog::prior_tab_item( window_handle begin )const
{
	return ::GetNextDlgTabItem( handle_value(), begin.value(), TRUE );
}
window_handle dialog::next_tab_item( window_handle begin )const
{
	return ::GetNextDlgTabItem( handle_value(), begin.value(), FALSE );
}

// �_�C�A���O�P�ʂ̍��W
LONG dialog::base_unit()
{
	return ::GetDialogBaseUnits();
}
bool dialog::base_unit_to_screen( rect& out )const
{
	RECT rc;
	get_rect( out, rc );
	if( FALSE == ::MapDialogRect( handle_value(), &rc ) )
		return false;

	out = make_rect(rc);
	return true;
}

// WindowLong
DLGPROC dialog::set_dialog_procedure_ptr( DLGPROC p )const
{
	return reinterpret_cast<DLGPROC>( set_window_long( DWL_DLGPROC, reinterpret_cast<LONG_PTR>(p) ) );
}
LRESULT dialog::set_message_result( LRESULT l )const
{
	return set_window_long( DWL_MSGRESULT, l );
}
LONG_PTR dialog::set_dialog_data( LONG_PTR v )const
{
	return set_window_long( DWL_USER, v );
}

DLGPROC dialog::dialog_procedure_ptr()const 
{
	return reinterpret_cast<DLGPROC>( window_long( DWL_DLGPROC ) );
}
LRESULT dialog::message_result()const
{
	return window_long( DWL_MSGRESULT );
}
LONG_PTR dialog::dialog_data()const
{
	return window_long( DWL_USER );
}

//	
//void dialog_init_focus( event_result &e, bool b )
//{
//	e.set<event::dialog_init>( int_bool(b) );
//}


} /* end of namespace gammo */
} /* end of namespace oden */


