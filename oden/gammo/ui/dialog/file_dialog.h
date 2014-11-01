#pragma once

#ifndef ODEN_GAMMO_UI_DLG_FILEDLG
#define ODEN_GAMMO_UI_DLG_FILEDLG

//###############################################
//
//
// --------- def -----------
#include <oden/gammo/defs.h>
// --------- win -----------
#include <boost/optional.hpp>
// --------- oden ----------
#include <oden/gammo/ui/dialog/common_dialog.h>

//
//
//#################################################


namespace oden { namespace gammo
{	
//##############################################################
// ダイアログボックスの記述
//
	ODEN_STRUCT( open_file_name, open_file_name_info )
  	{
		struct get
  		{};

		struct set
  		{
		  // オーナーウィンドウ
			Me& owner( window_handle h ) 
			{ self::val(this).hwndOwner = h.value(); return self::me(this); }
		  // フィルター
			Me& filter( string_ptr str ) 
			{ self::val(this).lpstrFilter = str.c_str(); return self::me(this); }
			Me& filter_select( index n ) 
			{ self::val(this).nFilterIndex = n.value(); return self::me(this); }
		  // ファイルネームバッファ
			Me& filepath_buf( string& out ) 
			{
				self::val(this).nMaxFile = out.size();
				self::val(this).lpstrFile = buf_begin<char_t>(out);
				return self::me(this);
			}
		  // ファイルタイトルバッファ
			Me& filename_buf( string& out ) 
			{
				self::val(this).lpstrFileTitle =  buf_begin<char_t>(out);
				self::val(this).nMaxFileTitle = out.size();
				return self::me(this);
			}
		  // タイトルを設定
			Me& title( string_ptr str ) 
			{
				self::val(this).lpstrTitle = str.c_str(); 
				return self::me(this);
			}
		  // 初期ディレクトリ
			Me& initial_dir( string_ptr s ) 
			{
				self::val(this).lpstrInitialDir = s.c_str(); 
				return self::me(this);
			}
		  // スタイル
			Me& flag( DWORD dw ) 
			{
				self::val(this).Flags |= dw; 
				return self::me(this); 
			}
		  // EXスタイル
			Me& flag_ex( DWORD dw ) 
			{ 
				self::val(this).FlagsEx |= dw; 
				return self::me(this); 
			}

		};

	  // デフォルト
		struct def
		{ 
			def( value_type& out )
			{
				out.lStructSize = self::value_size();
			}
		};
	};

	ODEN_STRUCT( save_file_name, save_file_name_info )
  	{
		typedef ODEN_PROPERTY_EXTENDS_MEMBER( open_file_name, &save_file_name_info::ofn ) extends;

		struct get : extends::get {};
		struct set : extends::set {};
		struct def : extends::def
		{
			def( value_type& out ) : extends::def(out) {}
		};
	};
	
//##############################################################
// 関数
//
  // openfilename
	bool show_open_dialog( string& pathbuf, DWORD flags = OFN_FILEMUSTEXIST, boost::optioanl<string&> namebuf = boost::none )
	{
		open_file_name::read_val ofn;
		ofn.filepath_buf(pathbuf);
		ofn.flag( flags );
		
		if( namebuf )
			ofn.filename_buf(namebuf.get());

		return show_dialog( ofn );
	}

//##############################################################
// イベント
//

//
// 構造体
//
	ODEN_STRUCT( open_file_notify, OFNOTIFY )
	{
		struct get 
		{
			nm_header::read_cref 		header()const { return self::val(this).hdr; }
			open_file_name::read_cref 	dialog()const { return self::val(this).lpOfn; }
			string						error_file_name()const { return self::val(this).pszFile; }
		};
	};
	
//
// イベント記述
//
	GAMMO_DEFINE_NOTIFY_EVENT( notify_def, dialog_file_ok, CDN_FILEOK )

	template<>
	struct event_arg<event::dialog_file_ok> : 
		event::result_< event::bool_<> >,
		event::lparam_< event::property_cref<open_file_notify> >
	{};
	
	// 返り値
	//inline void open_file_reject( event_result &e, bool b = true )
	// { e.set<event::dialog_file_ok>( int_bool(b) ); }

}}

#endif