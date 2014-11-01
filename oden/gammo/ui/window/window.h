#pragma once

#ifndef ODEN_GAMMO_UI_WND_WND
#define ODEN_GAMMO_UI_WND_WND

//#############################################
//  �C���N���[�h
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
// �O���錾
//

namespace oden { namespace gammo
{
	class window;
	class paint_dc;

//___________________________________________________________________________________
//
//
//           �E �B �� �h �E �N �� �X
//
//___________________________________________________________________________________
//		
//###########################################################
// WNDCLASSEX�̃v���p�e�B
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
		  // �v���V�[�W��
			Me& procedure( WNDPROC proc ) {
				self::val(this).lpfnWndProc = proc; 
				return self::me(this);	
			}
		  // �X�^�C��
			Me& style( DWORD sty ) {
				self::val(this).style |= sty; 
				return self::me(this);				
			}
		  //  �N���X������
			Me& extra_mem( size_t bytes ) {
				self::val(this).cbClsExtra=bytes;
				return self::me(this);				
			}
		  // �E�B���h�E���Ƃ̃�����
			Me& window_extra_mem( size_t bytes ) {
				self::val(this).cbWndExtra=bytes;
				return self::me(this);					
			}
		  // �傫���A�C�R��
			Me& class_icon( icon_handle hi ) {
				self::val(this).hIcon = hi.value();
				self::val(this).hIconSm = self::val(this).hIcon;
				return self::me(this);				
			}
		  // �������A�C�R��
			Me& class_icon_small( icon_handle hi ) {
				self::val(this).hIconSm = hi.value();
				return self::me(this);				
			}
		  // �w�i�u���V
		  	Me& background( brush_handle bh ) {
				self::val(this).hbrBackground = br.value();
				return self::me(this);				
			}
		  // �w�i�F
		  	Me& background( int color ) {
				self::val(this).hbrBackground = reinterpret_cast< HBRUSH >( color + 1 );
				return self::me(this);				
			}
		  // �A�v���P�[�V�����n���h��
			Me& instance( instance_handle ih ) {
				self::val(this).hInstance = ih.value();
				return self::me(this);				
			}
		};
		
	  // �f�t�H���g�l
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
//  �t���[�֐�
//
  // �E�B���h�E�N���X��o�^
 	window_class_id_val register_window_class( window_class::value_type& desc );
 	
  // �E�B���h�E�N���X��o�^����O��
 	bool unregister_window_class( window_class_id id, instance_handle h = boost::none );
	
//______________________________________________________________________________________
//
//
//           �E �B �� �h �E
//
//______________________________________________________________________________________
//
//##############################################################
// �E�B���h�E�����p�����[�^
//
	ODEN_STRUCT( window_create_info, CREATESTRUCT )
  	{
		struct get
  		{};

		struct set
  		{
		  // �^�C�g����ݒ�
			Me& text( string_ptr str ) {
				self::val(this).lpszName = str.c_str(); 
				return self::me(this);
			}
		  // �E�B���h�E�̃T�C�Y
			Me& size( const size& sz ){ 
				return size( sz.width(), sz.height() ); 
			}
			Me& size( long cx, long cy ) {			
				self::val(this).cx = cx;  
				self::val(this).cy = cy;  
				return self::me(this);
			}
		  // �E�B���h�E�̈ʒu
			Me& position( const point& pos ){ 
				return position( pos.x(), pos.y() ); 
			}
			Me& position( long x, long y ){			
				self::val(this).x = x; 
				self::val(this).y = y;
				return self::me(this);
			}
		  // �A�v���P�[�V�����n���h��
			Me& instance( instance_handle ins ) { 
				self::val(this).hInstance = h.value(); 
				return self::me(this);
			}
		  // �X�^�C��
			Me& style( DWORD sty ) {
				self::val(this).style |= sty; 
				return self::me(this); 
			}
		  // EX�X�^�C��
			Me& style_ex( DWORD sty ) { 
				self::val(this).dwExStyle |= sty; 
				return self::me(this); 
			} 
		  // �e�E�B���h�E
			Me& parent( window pw ) { 
				self::val(this).style |= WS_CHILD;
				self::val(this).hwndParent = pw.handle_value();
				return self::me(this);
			}
		  // �qID
			Me& child_id( int ID ){				
				self::val(this).style |= WS_CHILD;
				self::val(this).hMenu = reinterpret_cast< HMENU >( ID );
				return self::me(this);
			}
		  // ���j���[  
			Me& menu( menu_handle mh ) {
				self::val(this).hMenu = mh.value();
				return self::me(this);
			}
		  // �ǉ��p�����[�^
			Me& lparam( LPVOID param ) {
				self::val(this).lpCreateParams = param; 
				return self::me(this); 
			}

		  // �����̃E�B���h�E����ݒ�
			Me& inherit_from( window other ) {
				other.update_window_longs();
				
				self::val(this).lpszName = 0; // ���O�̓R�s�[�ł��Ȃ�...
				
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
				else { // �e�����L����
					parent( other.get_parent_or_owner() );
				}

				return self::me(this);
			}
		};

	  // �f�t�H���g		
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
//  ����C���^�[�t�F�[�X
//
	class window
		: public handle_interface<window_handle>
	{	
	private:
		typedef window self;

	public:
	// �R���X�g���N�^
		window( const window_handle& wh = boost::none )
			: handle_interface<window_handle>( wh )
		{}
		
	// �f���[�^
		typedef struct destroyer {
			void operator()( self& obj )const { obj.destroy(); }
		} deleter_type;
	
	// �n���h��
	//	
 		bool create( window_class::value_type& cls_desc, window_create_info::value_type& desc );
		bool create( const window_class_id &cls_id, window_create_info::value_type& desc );
	
		bool destroy();
		bool destroy_handle()const;
	
	// �E�B���h�E�`��֐�
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
		
	// �E�B���h�E��ԕύX�E�擾
	//
		void enable( bool b = true )const;		
		void disable( )const;
		
		window_handle activate( )const;
		
		bool close( )const;
	
	// �ق��̃E�B���h�E�Ƃ̊֌W�E�n���h���擾
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
	
	// �E�B���h�E�̍��W�擾�E�ϊ�
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
		
	// �E�B���h�E�̃T�C�Y�ύX
	//		
		bool adjust_rect( const size& client_size, rect& out, bool hasMenu )const;
		
		bool move( const rect& new_rc, bool repaint = false )const;
		
		bool set_position(
			const rect&	new_rc,
			DWORD flags = SWP_NOZORDER,
			window_handle insert_after = boost::none 
		) const;		
		
	// �E�B���h�E�̃f�[�^�擾
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
		
	  // window long�𔽉f������
	  	void update_window_longs()const;
		
	  // �e�L�X�g
		bool get_text( string& out )const;
		string text( )const;
		
		int text_length( )const;
		
		bool set_text( string_ptr text )const;	

	  // ���j���[
		menu_handle menu( )const;
		menu_handle set_menu( menu_handle mh )const;
		bool draw_menu_bar( )const;
		
	  // �V�X�e�����j���[
		menu_handle system_menu()const;
		void reset_system_menu()const;
	
	// �E�B���h�E�Ԃ̃��b�Z�[�W���M
	//	
	  /// ���b�Z�[�W�𑗂�A�I���܂Ő����Ԃ��Ȃ�
		LRESULT send( const window_message& m )const;
		LRESULT send_message( UINT code, WPARAM w = 0, LPARAM l = 0 )const;
		
	  /// ���X���b�h�̃E�B���h�E�փ��b�Z�[�W�𑗂�B�^�C���A�E�g�w�肪�ł���
		bool send_timeout( const window_message& m, UINT flags, DWORD timeoutMilisec, LRESULT& retOut)const;
		
	  /// ���b�Z�[�W�𑗂�A�����ɐ����Ԃ�		
		bool send_notify( const window_message& m )const;
		
	  /// ���b�Z�[�W�𑗂�A�����ɐ����Ԃ��B�����̏������I��������A�R�[���o�b�N�֐����Ăяo��
		//bool    SendMsgCallBack(const_window_ptr p, const window_message& m)const;
		
	  /// ���b�Z�[�W���|�X�g���A�����ɐ����Ԃ�
		bool post( const window_message& m )const;
		bool post_message( UINT code, WPARAM w = 0, LPARAM l = 0 )const;
		
	// �}�E�X
	//
	  // �h���b�O
		bool drag_detect( const point& scr_start )const;
	  //  ���̃E�B���h�E��ł̃}�E�X�̓�����`���郁�b�Z�[�W�𑗂�(��x�̂�)
		bool track_mouse_event( DWORD flags, DWORD time = HOVER_DEFAULT )const;
	
	// ���̑�
	//	
	  // �A�j���[�V����
		bool animate( DWORD Dura, DWORD AnimateType )const;
      // �E�B���h�E�p�[�c�̃V�X�e���J���[���擾
		static colorref sys_color( int dw );
		static bool set_sys_color( memory_range<int> i, memory_range<COLORREF> cols );
	  // �œK�T�C�Y��Ԃ��F�f�t�H���g
		void calc_best_size( size& out )const { out.set(10,10); }

	}; /* end of class window */

//##############################################################
// �쐬�֐�(�v���V�[�W���o�^�Ȃ�)
//	
  // �N���XID + �E�B���h�E�p�����[�^
 	window_handle create_window( const window_class_id &cls_id, window_create_info::value_type& desc );
	
  // �E�B���h�E�N���X�p�����[�^ + �E�B���h�E�p�����[�^
 	window_handle create_window( window_class::value_type& cls_desc, window_create_info::value_type& desc );
	
	
//##############################################################
// DeferWindowPos�֘A�̑���
//	
	class defer_window_pos
		: public handle_interface<defer_window_pos_handle>
	{
	public:
	// �R���X�g���N�^
		defer_window_pos( defer_window_pos_handle h = boost::none )
			: handle_interface<defer_window_pos_handle>(h)
		{}
		
	// �I������
		typedef struct ender {
			void operator()( defer_window_pos& o ){ o.end(); }
		} deleter_type;
		
	// �J�n
		bool is_started()const { return handle().is_valid(); }
		bool start( int reserve_size );
	// ���f������
		bool end();
	// �����ǉ�
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
//           �E �B �� �h �E �C �x �� �g
//
//___________________________________________________________________________________
//
//#############################################################
// ������̎擾�Ɏg��
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
//  �\����
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
//  ����u�����`����
//
  // �����������qID�����o������
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
				nm_header::read_cref nmhdr( get_event_arg< notify_def<void>, event::_2 >(ev) ); // generic��notify_def���Ăяo��
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
// �C�x���g��`
//
  // create
	namespace event 
	{
		//  �Ԃ�l�̒萔
		typedef limen< int, 0, singular_null<int,-1> > create_result;		
	}
	
	GAMMO_DEFINE_EVENT( event_def, create, WM_CREATE, 1 )
		
	template<>
	struct event_arg<event::create> :
		event::result_< event::value_object<event::create_result> >, 
		event::lparam_< event::property_cref<window_create_info> > 
	{};
	
  // �ʒm�C�x���g
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








