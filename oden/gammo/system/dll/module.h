#pragma once

#ifndef ODEN_GAMMO_SYS_DLL_MOD
#define ODEN_GAMMO_SYS_DLL_MOD

//#############################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>

//-------- boost --------

//-------- oden ---------

//-------- gammo ---------

//
//#############################################


namespace oden { namespace gammo
{	
//########################################
// 自動キャストクラス
//
namespace detail
{
	class proc_caster
	{
	public:
		proc_caster( void* pfn )
			: ptr_( pfn )
		{}
		
		template< typename FuncPtr >
		operator FuncPtr()const
		{
			return static_cast< FuncPtr >( ptr_ );
		}
	private:
		void* ptr_;
	};
}

//########################################
// モジュール・基本クラス
//
	class module
		: public handle_interface<module_handle>
	{
	private:
		typedef module self;
	public:
	// コンストラクタ・デストラクタ
		module( module_handle h = boost::none )
			: handle_interface<module_handle>( h )
		{}
		
	// デリータ
		typedef struct releaser {
			void operator()( self& mod )const{ mod.release(); }
		} deleter_type;
		
	// メンバ関数
	  // 関数を取りだす
		void* proc_address( narrow_string_ptr proc_name )const;
		void* proc_address( int ordinal )const;
	  // キャスト機能付きで関数をとりだす
		detail::proc_caster get_proc_address( narrow_string_ptr proc_name )const 
		 { return detail::proc_caster( this->proc_address( proc_name ) ); }
		detail::proc_caster get_proc_address( int ordinal )const 
		 { return detail::proc_caster( this->proc_address( ordinal ) ); }
	  // 解放
		bool release();
	  // モジュールのパスを取得
		bool get_path( string& out )const;
		string path()const { string out; this->get_path(out); return out; }

	};
	
//########################################
// コンストラクト関数
//		
    // フラグなしでロードする
	module_handle load_library ( string_ptr path );
	
    // フラグを指定してロードする
	module_handle load_library ( string_ptr path, DWORD option );
	
}}

#endif
