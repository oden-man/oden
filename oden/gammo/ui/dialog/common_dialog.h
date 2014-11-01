#pragma once

#ifndef ODEN_GAMMO_UI_DLG_COMMDLG
#define ODEN_GAMMO_UI_DLG_COMMDLG

//###############################################
//
//

// --------- def -----------
#include <oden/gammo/defs.h>
// --------- win -----------
#include <Commdlg.h>
// --------- oden ----------
#include <oden/gammo/ui/dialog/dialog.h>

//
//
//#################################################


//______________________________________________________
//
//           コモンダイアログ(ComDlg.cpp)
//______________________________________________________
//

namespace oden { namespace gammo
{

//###########################################
// コモンダイアログ
//
	struct save_file_name_info
	{
		OPENFILENAME ofn;
	};

	typedef OPENFILENAME open_file_name_info;
	typedef MSGBOXPARAMS message_box_info;

//###########################################
// コモンダイアログのエラー
//
	class common_dialog_error
		: public nullable< DWORD, singular_null<DWORD, 0> >
	{
	public:
		common_dialog_error()
			: code_(0)
		{
			update();
		}

		void update();

		DWORD code()const { return code_; }
		string name()const;
	private:
		DWORD code_;
	};

//###########################################
// メッセージボックス
//
	ODEN_STRUCT( message_box, message_box_info )
	{
		struct set
		{
			Me& owner( window_handle h )
			{ self::val(this).hwndOwner = h.value(); return self::me(this); }

			Me& instance( instance_handle h )
			{ self::val(this).hInstance = h.value(); return self::me(this); }

			Me& text( string_ptr s )
			{ self::val(this).lpszText= s.c_str(); return self::me(this); }

			Me& caption( string_ptr s )
			{ self::val(this).lpszCaption = s.c_str(); return self::me(this); }

			Me& icon( res_id id )
			{ self::val(this).lpszIcon = id.get(); return self::me(this); }

			Me& style( DWORD dw )
			{ self::val(this).dwStyle = dw; return self::me(this); }

			Me& help( MSGBOXCALLBACK pfn, DWORD_PTR cxt_id )
			{ self::val(this).lpfnMsgBoxCallback = pfn; 
			  self::val(this).dwContextHelpId = cxt_id; return self::me(this); }

			Me& language( DWORD dw )
			{ self::val(this).dwLanguageId = dw; return self::me(this); }
		};
		
		struct def
		{
			def( value_type& out )
			{
				out.cbSize = self::value_size();
			}
		};
	};

//###########################################
// コモンダイアログの呼び出し関数
//
  // 構造体でオーバーロード
	bool show_dialog( open_file_name_info& v );
	bool show_dialog( save_file_name_info& v );
	dlg_result show_dialog( message_box_info& v );

  // 個別 - メッセージボックス
	int show_message_box( window_handle owner, string_ptr caption, string_ptr text, UINT flags = MB_OK );
	inline 
	int show_message_box( string_ptr caption, string_ptr text, UINT flags = MB_OK )
	{
		return show_message_box( boost::none, caption, text, flags ); 
	}

	int show_ansi_message_box( window_handle owner, narrow_string_ptr caption, narrow_string_ptr text, UINT flags = MB_OK );
	inline 
	int show_ansi_message_box( narrow_string_ptr caption, narrow_string_ptr text, UINT flags = MB_OK )
	{ 
		return show_ansi_message_box( boost::none, caption, text, flags ); 
	}

	// gammo::show_common_dialog( gammo::open_file_name().filter(_T("")).flags(OFN_).file_name(out).file_title(out) );

#if 0
//####################################################
// 色を選択するダイアログ
//
	class SelectColDialog : public ComDlgExtendedError
	{
	private:
		CHOOSECOLOR ChCol;
		COLORREF    CustCols[16];
	public:
	//"
		SelectColDialog()
		{
			ZeroMemory(&ChCol,sizeof(CHOOSECOLOR));
			ZeroMemory(CustCols,sizeof(CustCols));
		}
		~SelectColDialog(){}
	//"
		bool Create(
			const window* pOwner,
			const dword_flags& Style = 0
			);

		DWORD GetSelectedColor()const{return ChCol.rgbResult;}
	
	// フラグ
		// 
		DW_FLAGS_IMP(Style, \
			((CC_FULLOPEN,			InitiallyFullOpen )) 	\
			((CC_PREVENTFULLOPEN,	DisableFullOpen)) 		\
			((CC_ANYCOLOR,			ShowAllColor)) 			\
			((CC_SOLIDCOLOR,		ShowOnlySolidColor)) 	\
			((CC_RGBINIT,			SetInitColor)) 			\
			((CC_SHOWHELP,			ShowHelp)) 				\
			)
		// 
		DW_FLAGS_IMP(CustomizeFlags, \
			((CC_ENABLEHOOK,			EnableHook	)) \
			((CC_ENABLETEMPLATE,		EnableTemplate)) \
			((CC_ENABLETEMPLATEHANDLE,	EnableTemplateHandle)) \
			)
	};
#endif

}}


#endif