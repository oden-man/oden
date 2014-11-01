#pragma once

#ifndef ODEN_GAMMO_SYS_FILE_FILE
#define ODEN_GAMMO_SYS_FILE_FILE

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
// --------- boost ----------
// --------- oden ----------
#include <oden/property.h>
//----------- gammo ----------
#include <oden/gammo/common/large_int.h>
#include <oden/gammo/system/kernel_object.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            ファイル クラス
//
//________________________________________________________________________________
//

// typedef
//
//################################################################
//  
//
	ODEN_STRUCT( file_time, FILETIME )
	{
		struct get {
			DWORD low()const { return self::val(this).dwLowDateTime; }
			DWORD high()const{ return self::val(this).dwHighDateTime; }
		};

		struct set {
			Me& low( DWORD dw )  { self::val(this).dwLowDateTime = dw; return self::me(this); }
			Me& high( DWORD dw ) { self::val(this).dwHighDateTime = dw; return self::me(this); }
		};
	};

//################################################################
//  
//
	ODEN_STRUCT( by_handle_file_info, BY_HANDLE_FILE_INFORMATION )
	{
		struct get
		{
			DWORD attribute()const 	{ return self::val(this).dwFileAttributes; }
			DWORD volume()const 	{ return self::val(this).dwVolumeSerialNumber; }
			DWORD size_high()const 	{ return self::val(this).nFileSizeHigh; }
			DWORD size_low()const 	{ return self::val(this).nFileSizeLow; }
			DWORD link_count()const { return self::val(this).nNumberOfLinks; }
			DWORD id_high()const 	{ return self::val(this).nFileIndexHigh; }
			DWORD id_low()const 	{ return self::val(this).nFileIndexLow; }
			file_time::read_cref creation_time()	const { return self::val(this).ftCreationTime; }
			file_time::read_cref last_access_time()	const { return self::val(this).ftLastAccessTime; }
			file_time::read_cref last_write_time()	const { return self::val(this).ftLastWriteTime; }
		};
	};
		
//################################################################
//  コントロールクラス
//
	class file
		: public kernel_obj
	{
	public:	
	// コンストラクタ
		file( file_handle h = boost::none )
			: kernel_obj(h)
		{}
		
	// ハンドル
		bool create( string_ptr filename, DWORD access, DWORD sharemode, DWORD disposition = OPEN_EXISTING, DWORD attr = FILE_ATTRIBUTE_NORMAL );
		//bool create( file_handle orig, DWORD access, DWORD sharemode, DWORD flags )const;

	// 入出力操作
		bool read( buffer_range<BYTE> buf, DWORD& r_size )const;
		bool write( memory_range<BYTE> data, DWORD& w_size )const;
		bool seek( const large_int& offs, DWORD dir = FILE_BEGIN )const;
		bool tell( large_int& out )const;
		bool flush()const;

		bool lock( const large_int& start, const large_int& length )const;
		bool unlock( const large_int& start, const large_int& length )const;

	// 属性取得
		bool get_size( large_int& out )const;
		large_int size()const{ large_int out; get_size(out); return out; }
		bool set_size()const;
		bool set_size( const large_int& sz )const { seek(sz); return set_size(); }

		DWORD type()const;
	};

}}

#endif
