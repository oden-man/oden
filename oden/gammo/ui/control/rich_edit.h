#pragma once

#ifndef ODEN_GAMMO_UI_CTL_RCHEDIT
#define ODEN_GAMMO_UI_CTL_RCHEDIT

//#######################################################
//  インクルード
//

//-------- defs ---------
#include <oden/gammo/defs.h>
// --------- stl ------------
// --------- win ------------
#include <richedit.h>
// --------- boost ----------
// --------- oden ----------
#include <oden/konbu/callback.h>
//----------- gammo ----------
#include <oden/gammo/ui/control/edit.h>

//
//#################################################

namespace oden { namespace gammo
{
//________________________________________________________________________________
//
//
//            エディット コントロール クラス
//
//________________________________________________________________________________
//	
//################################################################
//  
//	
	ODEN_STRUCT( char_format, CHARFORMAT2 )
  	{		
		struct set
  		{
		  // マスク
			Me& mask( DWORD m ) {
				self::val(this).dwMask |= mask;
				return self::me(this);
			}
		  // 文字エフェクト
			Me& effect( DWORD target, DWORD val ) {
				mask( target );
				return effect( val );
			}
			Me& effect( DWORD dw ) {
				self::val(this).dwEffects |= dw;
				return self::me(this);
			}
		  // サイズ
	  		Me& height( LONG l ) {
				self::val(this).dwMask |= CFM_SIZE;
				self::val(this).yHeight = l;
				return self::me(this);
			}
		  // オフセット
	  		Me& offset( LONG l ) {
				self::val(this).dwMask |= CFM_OFFSET;
				self::val(this).yOffset = l;
				return self::me(this);
			}
		  // 色
	  		Me& color( colorref c ) {
				self::val(this).dwMask |= CFM_COLOR;
				self::val(this).crTextColor = c.value();
				return self::me(this);
			}
		  // 背景色
	  		Me& back_color( colorref c ) {
				self::val(this).dwMask |= CFM_BACKCOLOR;
				self::val(this).crBackColor = c.value();
				return self::me(this);
			}
	  		Me& auto_back_color() {
				return effect(CFE_AUTOBACKCOLOR);
			}
			
		};
		
		struct def
		{ 
			def( value_type& out )
			{
				// CHARFORMAT2 はPODでない(基底クラスがある)ので、イニシャライザでゼロクリアされない
				// でも実質メモリは連続しているようなので、memsetは使える?
				memset( &out, 0, self::value_size() );

				out.cbSize = self::value_size();
			}
		};
	};

  //
	typedef konbu::callback<EDITSTREAMCALLBACK, 1> edit_stream_callback;

	ODEN_STRUCT( edit_stream, EDITSTREAM )
	{
		struct get
		{
			DWORD error()const { return self::val(this).dwError; }
		};

		struct set
		{
			Me& callback( edit_stream_callback::pointer p, LONG_PTR l )
			{ self::val(this).pfnCallBack = p;
			  self::val(this).dwCookie = l; return self::me(this); }

			template< typename F >
			Me& callback( const F& f )
			{ return callback( &edit_stream_callback::set<F>::entry, reinterpret_cast<LONG_PTR>(f) ); }
		};
	};

  //
	ODEN_STRUCT( char_range, CHARRANGE )
	{
		struct get
		{
			index begin()const { return self::val(this).cpMin; }
			index end()const { return self::val(this).cpMax; }
		};

		struct set
		{
			Me& from( index i )
			{ self::val(this).cpMin = i.value(); return self::me(this); }

			Me& to( index i )
			{ self::val(this).cpMax = i.value(); return self::me(this); }

			Me& all()
			{ return from(0).to(-1); }
		};
	};
	
//################################################################
//  コントロールクラス
//
	class rich_edit
		: public edit
	{
	private:
		typedef rich_edit self;
	public:	
	// コンストラクタ
		rich_edit( window_handle wh = boost::none )
			: edit( wh )
		{}
		
	// メンバ関数
	  // ライブラリをロードする
		static module_handle load_library( int var = 2 );

	  // 基本関数
		using edit::select;
		void select( const char_range::value_type& rng )const;
		void get_selection( char_range::value_type& out )const;

	  // スタイル操作
		// 文字のフォーマットを変える
		bool set_char_format( UINT target, const char_format::value_type& fmt )const;
		// 文字の背景を変える
		colorref set_background( colorref c = boost::none )const;
		
	  // 選択位置・キャレット位置	 
		
	  // 複数行の操作
		
	  // 文字の制限
		
	  // レイアウト
		
	  // コントロールの操作
		// 独自方法で読み書きする
		int input ( edit_stream::value_type& stm, UINT opt = SF_TEXT|SF_UNICODE )const;
		int output( edit_stream::value_type& stm, UINT opt = SF_TEXT|SF_UNICODE )const;
		
	  // XP later
	#if (_WIN32_WINNT >= 0x0501)	
	#endif

	  // Vista later
	#if (_WIN32_WINNT >= 0x0501)	
	#endif
		
      // class id
		static window_class_id get_control_class() { return window_class_id(RICHEDIT_CLASS); }
	};

}}
#endif
