#pragma once

#ifndef ODEN_GAMMO_UI_WND_WND
#define ODEN_GAMMO_UI_WND_WND

//#############################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ----------
// --------- win ----------
// -------- boost ---------
// -------- oden -----------
#include <oden/property.h>
// -------- gammo -----------
#include <oden/gammo/ui/window/message.h>

//
//############################################


//
// 前方宣言
//

namespace oden { namespace gammo
{
	class window;
	class paint_dc;

//___________________________________________________________________________________
//
//
//           ウ ィ ン ド ウ ク ラ ス
//
//___________________________________________________________________________________
//		
//###########################################################
// WNDCLASSEXのプロパティ
//
	ODEN_STRUCT( window_class, WNDCLASSEX )
	{
		struct get
		{
			WNDPROC			procedure		 ( )const { return self::val(this).lpfnWndProc; }
			DWORD 			style			 ( )const { return self::val(this).style; }
			icon_handle		class_icon		 ( )const { return self::val(this).hIcon; }
			icon_handle		small_icon		 ( )const { return self::val(this).hIconSm; }
			size_t 			extra_mem		 ( )const { return self::val(this).cbClsExtra; }
			size_t  		window_extra_mem ( )const { return self::val(this).cbWndExtra; }
			brush_handle	background		 ( )const { return self::val(this).hbrBackground; }
			window_class_id identifier		 ( )const {	return window_class_id( self::val(this).lpszClassName ); }
		};
		
		struct set
		{
	  	  // ID
			Me& identifier( window_class_id class_id ) {		
				self::val(this).lpszClassName = class_id.get();
				return self::me(this);
			}
		  // プロシージャ
			Me& procedure( WNDPROC proc ) {
				self::val(this).lpfnWndProc = proc; 
				return self::me(this);	
			}
		  // スタイル
			Me& style( DWORD sty ) {
				self::val(this).style |= sty; 
				return self::me(this);				
			}
		  //  クラスメモリ
			Me& extra_mem( size_t bytes ) {
				self::val(this).cbClsExtra=bytes;
				return self::me(this);				
			}
		  // ウィンドウごとのメモリ
			Me& window_extra_mem( size_t bytes ) {
				self::val(this).cbWndExtra=bytes;
				return self::me(this);					
			}
		  // 大きいアイコン
			Me& class_icon( icon_handle hi ) {
				self::val(this).hIcon = hi.value();
				self::val(this).hIconSm = self::val(this).hIcon;
				return self::me(this);				
			}
		  // 小さいアイコン
			Me& class_icon_small( icon_handle hi ) {
				self::val(this).hIconSm = hi.value();
				return self::me(this);				
			}
		  // 背景ブラシ
		  	Me& background( brush_handle bh ) {
				self::val(this).hbrBackground = br.value();
				return self::me(this);				
			}
		  // 背景色
		  	Me& background( int color ) {
				self::val(this).hbrBackground = reinterpret_cast< HBRUSH >( color + 1 );
				return self::me(this);				
			}
		  // アプリケーションハンドル
			Me& instance( instance_handle ih ) {
				self::val(this).hInstance = ih.value();
				return self::me(this);				
			}
		};
		
	  // デフォルト値
		struct def
		{ 
			def( value_type& out )
			{
				out.cbSize = self::value_size();

				out.style = CS_HREDRAW|CS_VREDRAW;
				out.hInstance = get_app_handle().value();

				out.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW+1 );
				out.hCursor = reinterpret_cast<HCURSOR>( ::LoadImage( NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE| LR_SHARED ) );
			}
		};
	};

//###########################################################
//  フリー関数
//
  // ウィンドウクラスを登録
 	window_class_id_val register_window_class( window_class::value_type& desc );
 	
  // ウィンドウクラスを登録から外す
 	bool unregister_window_class( window_class_id id, instance_handle h = boost::none );
	
//______________________________________________________________________________________
//
//
//           ウ ィ ン ド ウ
//
//______________________________________________________________________________________
//
//##############################################################
// ウィンドウを作るパラメータ
//
	ODEN_STRUCT( window_create_info, CREATESTRUCT )
  	{
		struct get
  		{};

		struct set
  		{
		  // タイトルを設定
			Me& text( string_ptr str ) {
				self::val(this).lpszName = str.c_str(); 
				return self::me(this);
			}
		  // ウィンドウのサイズ
			Me& size( const size& sz ){ 
				return size( sz.width(), sz.height() ); 
			}
			Me& size( long cx, long cy ) {			
				self::val(this).cx = cx;  
				self::val(this).cy = cy;  
				return self::me(this);
			}
		  // ウィンドウの位置
			Me& position( const point& pos ){ 
				return position( pos.x(), pos.y() ); 
			}
			Me& position( long x, long y ){			
				self::val(this).x = x; 
				self::val(this).y = y;
				return self::me(this);
			}
		  // アプリケーションハンドル
			Me& instance( instance_handle ins ) { 
				self::val(this).hInstance = h.value(); 
				return self::me(this);
			}
		  // スタイル
			Me& style( DWORD sty ) {
				self::val(this).style |= sty; 
				return self::me(this); 
			}
		  // EXスタイル
			Me& style_ex( DWORD sty ) { 
				self::val(this).dwExStyle |= sty; 
				return self::me(this); 
			} 
		  // 親ウィンドウ
			Me& parent( window pw ) { 
				self::val(this).style |= WS_CHILD;
				self::val(this).hwndParent = pw.handle_value();
				return self::me(this);
			}
		  // 子ID
			Me& child_id( int ID ){				
				self::val(this).style |= WS_CHILD;
				self::val(this).hMenu = reinterpret_cast< HMENU >( ID );
				return self::me(this);
			}
		  // メニュー  
			Me& menu( menu_handle mh ) {
				self::val(this).hMenu = mh.value();
				return self::me(this);
			}
		  // 追加パラメータ
			Me& lparam( LPVOID param ) {
				self::val(this).lpCreateParams = param; 
				return self::me(this); 
			}

		  // 既存のウィンドウから設定
			Me& inherit_from( window other ) {
				other.update_window_longs();
				
				self::val(this).lpszName = 0; // 名前はコピーできない...
				
				self::val(this).x = 0;
				self::val(this).y = 0;
				self::val(this).hInstance = other.app_handle().value();
				
				size clidimn = other.client_rect().dimension();
				self::val(this).cx = clidimn.width();
				self::val(this).cy = clidimn.height();
				
				if( other.is_top_level() ) {
					self::val(this).hwndParent = NULL;
					self::val(this).hMenu = other.menu().value();
				}
				else { // 親を共有する
					parent( other.get_parent_or_owner() );
				}

				return self::me(this);
			}
		};

	  // デフォルト		
		struct def
		{ 
			def( value_type& out )
			{
				out.x = CW_USEDEFAULT;
				out.y = 1;
				
				out.cx = CW_USEDEFAULT;
				out.cy = 1;

				out.hInstance = NULL;
			}
		};
  	};

//##############################################################
//  操作インターフェース
//
	class window
		: public handle_interface<window_handle>
	{	
	private:
		typedef window self;

	public:
	// コンストラクタ
		window( const window_handle& wh = boost::none )
			: handle_interface<window_handle>( wh )
		{}
		
	// デリータ
		typedef struct destroyer {
			void operator()( self& obj )const { obj.destroy(); }
		} deleter_type;
	
	// ハンドル
	//	
 		bool create( window_class::value_type& cls_desc, window_create_info::value_type& desc );
		bool create( const window_class_id &cls_id, window_create_info::value_type& desc );
	
		bool destroy();
		bool destroy_handle()const;
	
	// ウィンドウ描画関数
	//
		void show( int nCmdShow = SW_SHOWNORMAL )const;
    
		bool update( )const;
		bool show_update( int nCmdShow = SW_SHOWNORMAL )const
		{
			show(nCmdShow);
			return update();
		}
    
		void invalidate( bool erase = true )const;
		void invalidate( const rect &fr, bool erase = true )const;
	    
		bool redraw( UINT flags )const;
		bool redraw( const rect &fr, UINT flags )const;
		
	// ウィンドウ状態変更・取得
	//
		void enable( bool b = true )const;		
		void disable( )const;
		
		window_handle activate( )const;
		
		bool close( )const;
	
	// ほかのウィンドウとの関係・ハンドル取得
	//
		bool has_parent( )const;
		bool is_top_level( )const{ return !has_parent(); }
		bool has_child( )const;

		bool is_ancestor_of( window_handle child )const;
		bool is_descendant_of( window_handle parent )const;
		
		bool is_parent_of( window child )const;
		bool is_child_of( window parent )const;
		
		bool is_owned_by( window owner )const;
		bool is_owner_of( window ownee )const;
		
		window_handle get_parent( )const;
		window_handle get_parent_or_owner( )const;
		window_handle get_owner( )const;
		window parent( )const 
		{ return get_parent(); }
		window parent_or_owner( )const 
		{ return get_parent_or_owner(); }
		window owner( )const 
		{ return get_owner(); }
		
		window_handle get_child( int id )const;
		window child( int id )const 
		{ return get_child(id); }
		template< typename Wnd >
		Wnd child( int id )const
		{ return get_child(id); }
		window_handle get_child_from_point( const point& pt, UINT types )const;
		
		int count_child( )const;
	
	// ウィンドウの座標取得・変換
	//		
		bool screen_rect( rect& out )const;		
		rect screen_rect( )const;
		
		bool client_rect( rect& out )const;		
		rect client_rect( )const;
		
		bool screen_to_client( point& out )const;		
		point to_client( const point& scrpt )const;
		bool screen_to_client( rect& out )const;
		
		bool client_to_screen( point& out )const;		
		point to_screen( const point& clipt )const;	
		bool client_to_screen( rect& out )const;
		
	// ウィンドウのサイズ変更
	//		
		bool adjust_rect( const size& client_size, rect& out, bool hasMenu )const;
		
		bool move( const rect& new_rc, bool repaint = false )const;
		
		bool set_position(
			const rect&	new_rc,
			DWORD flags = SWP_NOZORDER,
			window_handle insert_after = boost::none 
		) const;		
		
	// ウィンドウのデータ取得
	//		
	  // classlong
		bool get_class_name( string& out )const;
		string class_name( )const;

		window_class_atom class_atom( )const;
		window_class_id   class_id( )const{ return class_atom().value(); }
	
	  // windowlong
		LONG_PTR set_window_long( int i, LONG_PTR val )const;
		LONG_PTR window_long( int i )const;

		DWORD set_style( DWORD flags )const;		
		DWORD set_style_ex( DWORD flags )const;		
		WNDPROC set_procedure_ptr( WNDPROC )const;		
		LONG_PTR set_child_id( LONG_PTR )const;		
		LONG_PTR set_user_data( LONG_PTR )const;		
		LONG_PTR set_extra_data( int, LONG_PTR )const;
		
		DWORD style( )const;		
		DWORD style_ex( )const;		
		WNDPROC procedure_ptr( )const;		
		instance_handle app_handle( )const;		
		LONG_PTR child_id( )const;		
		LONG_PTR user_data( )const;		
		LONG_PTR extra_data( int )const;
		
	  // window longを反映させる
	  	void update_window_longs()const;
		
	  // テキスト
		bool get_text( string& out )const;
		string text( )const;
		
		int text_length( )const;
		
		bool set_text( string_ptr text )const;	

	  // メニュー
		menu_handle menu( )const;
		menu_handle set_menu( menu_handle mh )const;
		bool draw_menu_bar( )const;
		
	  // システムメニュー
		menu_handle system_menu()const;
		void reset_system_menu()const;
	
	// ウィンドウ間のメッセージ送信
	//	
	  /// メッセージを送り、終わるまで制御を返さない
		LRESULT send( const window_message& m )const;
		LRESULT send_message( UINT code, WPARAM w = 0, LPARAM l = 0 )const;
		
	  /// 他スレッドのウィンドウへメッセージを送る。タイムアウト指定ができる
		bool send_timeout( const window_message& m, UINT flags, DWORD timeoutMilisec, LRESULT& retOut)const;
		
	  /// メッセージを送り、すぐに制御を返す		
		bool send_notify( const window_message& m )const;
		
	  /// メッセージを送り、すぐに制御を返す。送り先の処理が終わった時、コールバック関数を呼び出す
		//bool    SendMsgCallBack(const_window_ptr p, const window_message& m)const;
		
	  /// メッセージをポストし、すぐに制御を返す
		bool post( const window_message& m )const;
		bool post_message( UINT code, WPARAM w = 0, LPARAM l = 0 )const;
		
	// マウス
	//
	  // ドラッグ
		bool drag_detect( const point& scr_start )const;
	  //  このウィンドウ上でのマウスの動きを伝えるメッセージを送る(一度のみ)
		bool track_mouse_event( DWORD flags, DWORD time = HOVER_DEFAULT )const;
	
	// その他
	//	
	  // アニメーション
		bool animate( DWORD Dura, DWORD AnimateType )const;
      // ウィンドウパーツのシステムカラーを取得
		static colorref sys_color( int dw );
		static bool set_sys_color( memory_range<int> i, memory_range<COLORREF> cols );
	  // 最適サイズを返す：デフォルト
		void calc_best_size( size& out )const { out.set(10,10); }

	}; /* end of class window */

//##############################################################
// 作成関数(プロシージャ登録なし)
//	
  // クラスID + ウィンドウパラメータ
 	window_handle create_window( const window_class_id &cls_id, window_create_info::value_type& desc );
	
  // ウィンドウクラスパラメータ + ウィンドウパラメータ
 	window_handle create_window( window_class::value_type& cls_desc, window_create_info::value_type& desc );
	
	
//##############################################################
// DeferWindowPos関連の操作
//	
	class defer_window_pos
		: public handle_interface<defer_window_pos_handle>
	{
	public:
	// コンストラクタ
		defer_window_pos( defer_window_pos_handle h = boost::none )
			: handle_interface<defer_window_pos_handle>(h)
		{}
		
	// 終了処理
		typedef struct ender {
			void operator()( defer_window_pos& o ){ o.end(); }
		} deleter_type;
		
	// 開始
		bool is_started()const { return handle().is_valid(); }
		bool start( int reserve_size );
	// 反映させる
		bool end();
	// 操作を追加
		bool add( 
			window_handle wnd, 
			const rect& fr, 
			DWORD flags = SWP_NOZORDER,
			window_handle insert_after = boost::none
		);
	};

//___________________________________________________________________________________
//
//
//           ウ ィ ン ド ウ イ ベ ン ト
//
//___________________________________________________________________________________
//
//#############################################################
// 文字列の取得に使う
//
	template< typename Ch = char_t >
	class string_getter
	{
	public:
		typedef std::basic_string<Ch> string_type;

		string_getter( size_t sz )
			: buf_( sz, (Ch)0 )
		{}

		string_type& buffer() { return buf_; }

		const string_type& get()const { return buf_; }
		operator string_type()const   { return buf_; }	
	private:
		string_type buf_;
	};

	/*
	template< typename Ch = char_t >
	class ntstring_getter
	{
	public:
		typedef std::basic_string<Ch> string_type;

		ntstring_getter( size_t def_sz = 32 )
			: buf_( def_sz, (Ch)0 )
		{}

		Ch* buffer(){ return buf_begin<Ch>( buf_ ); }

		const string_type& get()const { return buf_; }
		operator string_type()const   { return buf_; }	
	private:
		string_type buf_;
	};*/

//###################################################################
//  構造体
//
  // NMHDR
	ODEN_STRUCT( nm_header, NMHDR )
	{
		struct get
		{
			window sender()const	{ return self::val(this).hwndFrom; }
			UINT sender_id()const	{ return self::val(this).idFrom; }
			UINT code()const		{ return self::val(this).code; }
		};
	};

//###################################################################
//  特殊ブランチ判定
//
  // 分岐引数から子IDを取り出し判定
	inline bool branch_child_id( UINT src, UINT id )
	{ 
		return src == id;
	}
	inline bool branch_child_id( UINT src, const window& ctl )
	{
		if( ctl.fail() )
			return false;
		return src == static_cast<UINT>(ctl.child_id()); 
	}
	inline bool branch_child_id( UINT src, const window* ctl )
	{
		if( ctl && !ctl->fail() )
			return false;
		return src == static_cast<UINT>(ctl->child_id()); 
	}
	template< typename WidPtr >
	bool branch_child_id( UINT src, const WidPtr& wid )
	{
		return src == static_cast<UINT>(wid.get()->get_widget_child_id());
	}
	
  // WM_COMMAND
	template< typename Sub >
	struct command_def
	{
		enum { code = WM_COMMAND };
		enum { sub_code = Sub::value };
	};

	template< typename Sub >
	struct event_arg< command_def<Sub> >
		: event::wparam_lparam< event::value<int>, event::value<int>, event::value_object<window_handle> >
	{};

	template< typename Sub >
	struct event_branch< command_def<Sub> >
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& arg1 )
		{
			if( branch( ev ) )
			{
				return branch_child_id( ev.wparam_low(), arg1 ) && ev.wparam_high() == Sub::value;
			}
			return false;
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_COMMAND;
		}
	};


  // WM_NOTIFY
	template< typename Sub >
	struct notify_def
	{
		enum { code = WM_NOTIFY };
		enum { sub_code = Sub::value };
	};

	template< typename Sub >
	struct event_arg< notify_def<Sub> >
		: event::wparam_lparam< event::value<UINT>, event::property_cref<nm_header> >
	{};

	template< typename Sub >
	struct event_branch< notify_def<Sub> >
	{
		template< typename T >
		static bool branch( const window_event& ev, const T& arg1 )
		{
			if( branch(ev) )
			{
				nm_header::read_cref nmhdr( get_event_arg< notify_def<void>, event::_2 >(ev) ); // genericなnotify_defを呼び出す
				return branch_child_id( nmhdr.sender_id(), arg1 ) && nmhdr.code() == static_cast<UINT>(Sub::value);
			}
			return false;
		}

		static bool branch( const window_event& ev )
		{
			return ev.code() == WM_NOTIFY;
		}
	};

//#############################################################
// イベント定義
//
  // create
	namespace event 
	{
		//  返り値の定数
		typedef limen< int, 0, singular_null<int,-1> > create_result;		
	}
	
	GAMMO_DEFINE_EVENT( event_def, create, WM_CREATE, 1 )
		
	template<>
	struct event_arg<event::create> :
		event::result_< event::value_object<event::create_result> >, 
		event::lparam_< event::property_cref<window_create_info> > 
	{};
	
  // 通知イベント
	GAMMO_DEFINE_NULL_EVENT( event_def, destroy, WM_DESTROY )
	GAMMO_DEFINE_NULL_EVENT( event_def, close, WM_CLOSE )
	
  // 
	
  // set font
	GAMMO_DEFINE_EVENT( event_def, set_font, WM_SETFONT, 2 )
	
	template<>
	struct event_arg<event::set_font> :
		event::wparam_lparam< event::value_object<font_handle>, event::bool_<> >
    {};
	  
  // get font
	GAMMO_DEFINE_EVENT( event_def, get_font, WM_GETFONT, 0 )
	
	template<>
	struct event_arg<event::get_font> :
		event::result_< event::value_object<font_handle> >, 
		event::noparam
	{};
	
  // get text
	GAMMO_DEFINE_EVENT( event_def, get_text, WM_GETTEXT, 1 )
	
	template<>
	struct event_arg<event::get_text> :
		event::result_< event::value<size_t> >, 
		event::wparam_lparam< event::string_out<> >
	{};	
	
  // set text
	GAMMO_DEFINE_EVENT( event_def, set_text, WM_SETTEXT, 1 )

	template<>
	struct event_arg<event::set_text> :
		event::result_< event::bool_<> >, 
		event::lparam_< event::string_pointer<> >
	{};	  

	/*
  // command
	GAMMO_DEFINE_WM_EVENT( command, WM_COMMAND, 3 )
		
	template<>
	struct event_arg<event::command> :
		event::wparam_lparam< event::value<int>, event::value<int>, event::value_object<window_handle> > 
	{};

  // notify
	GAMMO_DEFINE_WM_EVENT( notify, WM_NOTIFY, 2 ) 
	
	template<>
	struct event_arg<event::notify> :
		event::wparam_lparam< event::value<WPARAM>, event::property_ref< nm_header > >
	{};	  
	
	
  // set text
	GAMMO_DEFINE_WM_EVENT( set_text, WM_SETTEXT, 1 ) : 
		event::result_< event::bool_<> >, 
		event::lparam_< event::string_pointer<> >
	{};	  
	
  // get text
	GAMMO_DEFINE_WM_EVENT( get_text, WM_GETTEXT, 1 ) :
		event::result_< event::value<size_t> >, 
		event::wparam_lparam< event::string_out<> >
	{};	
	
  // get text length
	GAMMO_DEFINE_WM_EVENT( get_text_length, WM_GETTEXTLENGTH, 0 ) : 
		event::result_< event::value<size_t> >, 
		event::noparam
	{};	

  // set font
	GAMMO_DEFINE_WM_EVENT( set_font, WM_SETFONT, 2 ) : 
		event::wparam_lparam< event::value_object<font_handle>, event::bool_<> >
    {};
	  
  // get font
	GAMMO_DEFINE_WM_EVENT( get_font, WM_GETFONT, 0 ) : 
		event::result_< event::value_object<font_handle> >, 
		event::noparam
	{};
	
  // redraw
	GAMMO_DEFINE_WM_EVENT( redraw, WM_SETREDRAW, 1 ) : 
		event::wparam_< event::bool_<> >
	{};
	
*/
}} /* end of namespace oden.gammo */

#endif








