#include "../Pch.h"
#include <cderr.h>

#include <oden/gammo/ui/dialog/common_dialog.h>

namespace oden { namespace gammo
{

//
// common_dialog error
//
void common_dialog_error::update()
{
	code_ = ::CommDlgExtendedError();
}
string common_dialog_error::name()const
{

#define CASE( err ) case err: return _T(#err)
	switch( code_ )
	{
	CASE( CDERR_STRUCTSIZE );
	CASE( CDERR_REGISTERMSGFAIL );
	CASE( CDERR_NOTEMPLATE );
	CASE( CDERR_NOHOOK );
	CASE( CDERR_NOHINSTANCE );
	CASE( CDERR_MEMLOCKFAILURE );
	CASE( CDERR_MEMALLOCFAILURE );
	CASE( CDERR_DIALOGFAILURE );
	CASE( CDERR_FINDRESFAILURE );
	CASE( CDERR_INITIALIZATION );
	CASE( CDERR_LOADRESFAILURE );
	CASE( CDERR_LOADSTRFAILURE );
	CASE( CDERR_LOCKRESFAILURE );

	CASE( FNERR_BUFFERTOOSMALL );
	CASE( FNERR_INVALIDFILENAME );
	CASE( FNERR_SUBCLASSFAILURE );

	CASE( FRERR_BUFFERLENGTHZERO );
	}
#undef CASE

	return _T("!undefined");
}


//
// start_common_dialog
//
bool show_dialog( open_file_name_info& v )
{ 
	return ::GetOpenFileName(&v) != FALSE; 
}

bool show_dialog( save_file_name_info& v )
{ 
	return ::GetSaveFileName(&v.ofn) != FALSE; 
}

dlg_result show_dialog( message_box_info& v )
{
	 return ::MessageBoxIndirect( &v );
}

//
int show_message_box( window_handle owner, string_ptr caption, string_ptr text, UINT flags )
{ 
	return ::MessageBox( owner.value(), caption.c_str(), text.c_str(), flags ); 
}

int show_ansi_message_box( window_handle owner, narrow_string_ptr caption, narrow_string_ptr text, UINT flags )
{ 
	return ::MessageBoxA( owner.value(), caption.c_str(), text.c_str(), flags ); 
}


















}}