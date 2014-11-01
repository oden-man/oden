#pragma once

#ifndef ODEN_GAMMO_CMN_ERR
#define ODEN_GAMMO_CMN_ERR

//##############################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- boost -----------

// --------- oden -----------
//#include <oden/tsumile/tsumile.h>

//
//
//#############################################



namespace oden { namespace gammo
{
//#############################################################
// エラーコード
//
  // エラーコード
	typedef nullable< DWORD, singular_null<DWORD,0> > win32_error_code;

  // クラス
	class win32_error
	{
	public:
	// メンバ関数
	  // 
		static win32_error last();
		static void set_last( win32_error_code c );
	  // 最新のエラーコードを取得
		void update();
	  // エラーコードをメッセージ化する
		string format()const;
	  // プロパティ
		win32_error_code get()const { return c_; }
		DWORD value()const { return c_.value(); }
		bool  none()const { return c_.is_null(); }

	// コンストラクタ
		win32_error(){ this->update(); }
		win32_error( win32_error_code dw ) : c_(dw) {}
	private:
		win32_error_code c_;
	};

}
}
	
#endif