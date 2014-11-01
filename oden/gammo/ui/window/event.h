#pragma once

#ifndef ODEN_GAMMO_UI_WINDOW_EVENT
#define ODEN_GAMMO_UI_WINDOW_EVENT

//#############################################
//  �C���N���[�h
//
// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl -------------
// --------- win ------------
// --------- boost ----------
#include <boost/ref.hpp>
// --------- oden  ----------
// --------- gammo --------
#include <oden/gammo/common/mini_mpl.h>

//
//############################################


namespace oden { namespace gammo
{
//#######################################################################
//  �E�B���h�E�C�x���g
//
	class window_event
	{
	public:
	// �R���X�g���N�^
		window_event()
			: code_(0), wparam_(0), lparam_(0), p_ret_(0), handled_cnt_(0)
		{}

	// �����o�֐�
		window_handle target()const { return target_; }
		template< typename T >
		T target()const { return target_; }
		
		UINT code()const { return code_; }
		WPARAM wparam()const { return wparam_; }
		LPARAM lparam()const { return lparam_; }

		WORD wparam_low ()const { return LOWORD(wparam_); }
		WORD wparam_high()const { return HIWORD(wparam_); }
		WORD lparam_low ()const { return LOWORD(lparam_); }
		WORD lparam_high()const { return HIWORD(lparam_); }

		template< typename T >
		T* lparam_to_ptr()const { return reinterpret_cast<T*>(lparam_); }
		
		LRESULT& result(){ return *p_ret_; }
		const LRESULT& result()const { return *p_ret_; }

	  // �X�V
		void set( HWND h, UINT c, WPARAM w, LPARAM l, LRESULT* pr, int calldepth )
		{
			target_ = h;
			code_ = c;
			wparam_ = w;
			lparam_ = l;
			p_ret_ = pr;
			handled_cnt_ = 0;
			call_depth_ = calldepth;
		}

	  // �������ꂽ���H�t���O�̑���
		// �P�̂Ŏg��
		int handle_addref()   { return ++handled_cnt_; }
		int handle_release()  { return --handled_cnt_; }
		// handle_start -> not_handled
		void handled() 	      { this->handle_addref(); }
		void not_handled()    { this->handle_release(); }
		// �`�F�b�N����
		bool is_handled()const { return 0 < handled_cnt_; }	

	  // �v���V�[�W���Ăяo����
		int call_depth()const { return call_depth_; }		

	private:
		window_handle	target_;
		UINT   	 		code_;
		WPARAM	 		wparam_;
		LPARAM	 		lparam_;
		LRESULT* 		p_ret_;

		int handled_cnt_;
		int call_depth_;
	};

//###########################################################
// �C�x���g�R�[�h�����߂�
//
	template< typename Code > 
	struct event_def
	{
		enum { code = Code::value };
	};

	struct any_event_def
	{};

	// or : trigger �̂ݍ�p
	template< typename L, typename R >
	struct event_def_or
	{};
	
  // �ʏ�͂�����g�p... ���b�Z�[�W�R�[�h�������ŋ�����ς���K�v������Εʂ̂��g��
	template< int C, typename Alter = void >
	struct event_code
	{
		enum { value = C };
	};

namespace event
{
	typedef any_event_def any;

//##############################################
// �C�x���g�����C���f�b�N�X�N���X
//
	template< int I >
	struct placeholder
	{
		enum { index = I };
	};

	typedef placeholder<-1>		args;  			// result ~ �S�������܂Ƃ߂��^�v��

	typedef placeholder<0>		result, ret;	// 0 result
	typedef placeholder<1>		arg1, _1;		// 1 �C�x���g����1
	typedef placeholder<2>		arg2, _2;		// 2 �C�x���g����2
	typedef placeholder<3>		arg3, _3;		// 3 �C�x���g����3
	typedef placeholder<4>		arg4, _4;		// 4 �C�x���g����4

	typedef placeholder<100>	raw;  			// �C�x���g�\����
	typedef placeholder<101>	target;			// target
	typedef placeholder<102>	code;  			// code
	typedef placeholder<103>	wparam;			// wparam
	typedef placeholder<104>	lparam;			// lparam
	
	typedef boost::reference_wrapper<window_event> event_ref;
	
	struct wlparam
	{
		WPARAM w;
		LPARAM l;

		explicit wlparam( WPARAM wp = 0, LPARAM lp = 0 )
			: w(wp), l(lp)
		{}
	};

//################################################
//  window_message�𕪉����A�����̂��ƂƂ���
//
  // �^�O
	struct src_wparam		{};
	struct src_lparam		{};
	struct src_wparam_low	{};
	struct src_wparam_high	{};
	struct src_lparam_low	{};
	struct src_lparam_high	{};
	struct src_wlparam		{};
	struct src_result		{};
	struct src_target		{};
	struct src_code			{};
	struct src_event		{};

  // �^�����߂�
	template< typename Tag >
	struct source;
	
	template<> 
	struct source<src_wparam> 
	{ 
		typedef WPARAM type; 
		static type get( window_event& ev )	{ return ev.wparam(); }
	};
	
	template<> 
	struct source<src_lparam> 
	{ 
		typedef LPARAM type; 
		static type get( window_event& ev )	{ return ev.lparam(); }
	};	
	
	template<> 
	struct source<src_wparam_low> 
	{
		typedef WORD type;
		static type get( window_event& ev )	{ return ev.wparam_low(); }
	};	
	
	template<> 
	struct source<src_wparam_high> 
	{
		typedef WORD type;
		static type get( window_event& ev )	{ return ev.wparam_high(); }
	};
	
	template<> 
	struct source<src_lparam_low> 
	{
		typedef WORD type;
		static type get( window_event& ev )	{ return ev.lparam_low(); }
	};	
	
	template<> 
	struct source<src_lparam_high> 
	{
		typedef WORD type;
		static type get( window_event& ev )	{ return ev.lparam_high(); }
	};
	
	template<> 
	struct source<src_result> 
	{ 
		typedef LRESULT type; 
		static type get( window_event& ev )	{ return ev.result(); }
	};
	
	template<> 
	struct source<src_target> 
	{ 
		typedef window_handle type; 
		static type get( window_event& ev )	{ return ev.target(); }
	};	
	
	template<> 
	struct source<src_code> 
	{ 
		typedef UINT type; 
		static type get( window_event& ev )	{ return ev.code(); }
	};	
	
	template<> 
	struct source<src_wlparam> 
	{ 
		typedef wlparam type; 
		static type get( window_event& ev )	{ return wlparam( ev.wparam(), ev.lparam() ); }
	};
	
	template<> 
	struct source<src_event> 
	{ 
		typedef event_ref type; 
		static type get( window_event& ev )	{ return event_ref(ev); }
	};
	
//#############################################################
//   �ϐ��̒�`
//
	template< typename Var, typename SourceTag >
	struct arg_def
	{
		// �f�R�[�h�^�̒�`
		typedef typename Var::value_type  		value_type;

		// �f�R�[�h�ϐ����󂯂�^
		template< typename T, bool Small = (sizeof(T) <= 8) >
		struct value_param_type
		{
			typedef value_type type;
		};
		template< typename T >
		struct value_param_type<T,false>
		{
			typedef const value_type& type;
		};
		template< typename X, bool B >
		struct value_param_type< boost::reference_wrapper<X>, B >
		{
			typedef X& type;
		};
		typedef typename 
			value_param_type<value_type>::type  param_type;
		
		// �f�[�^��
		typedef SourceTag 						source_tag;
		typedef source<source_tag>				source;
		typedef typename source::type			source_type;

		// �G���R�[�h�����^
		typedef typename Var::input_type		input_type;
		
		/* �G���R�[�_�E�f�R�[�_�̐ݒ� */
		typedef Var var;
		
		/* static YYY decode( window_event& ev ){ return ~~~; } �\�z�֐��̒�` */
		static param_type decode_message( window_event& ev )
		{
			return var::decode( source::get(ev) ); 
		}			
		static param_type decode_message( source_type so )
		{
			return var::decode( so ); 
		}
		
		/* static ZZZ encode( type t ){ return ~~~; }  �G���R�[�h�֐��̒�` */
		static source_type encode_message( input_type x )
		{
			return var::template encode<source_type>( x ); 
		}
	};	
	
  // �ϐ��������ĕۊ�
	template< typename Def >
	class arg_cradle
	{ 
	public:
		explicit arg_cradle( window_event& ev )
			: value( Def::decode_message(ev) ) 
		{}
	
		typename Def::value_type value;
	};
	
//##################################################################
// �l�ϊ����@�̎w�� - 
//
  // �P�ɃL���X�g���邾��
	template< typename T, typename In = T >
	struct value
	{
		typedef T  value_type;
		typedef In input_type;
	
		template< typename Src >
		static T decode( Src s ){ return (T)(s); }
		
		template< typename Src >
		static Src encode( In t ){ return (Src)(t); }
	};
	
  // �������l�������L���X�g������value�i�����Ȃ���W/LPARAM -> ��������̌^�֕ϊ�����ꍇ�j
	template< typename T, typename In = T >
	struct signed_value
	{
		typedef T  value_type;
		typedef In input_type;
	
		template< typename Src > // �����Ȃ� -> ��������
		static T decode( Src s ){ return static_cast<T>( signed_value::cast(s) ); }
		
		template< typename Src > // �������� -> �����Ȃ��Ȃ̂œ��ʂȏ����͂���Ȃ�
		static Src encode( In t ){ return static_cast<Src>(t); }
		
	private:
		// �����Ȃ� -> ��������i���Ƃ͕��j�̕ϊ���K�؂ɍs���i�����n�ˑ��j	
		// BYTE -> char
		static char cast( BYTE wo ) { return static_cast<char>(wo); }
		
		// WORD -> short
		static short cast( WORD wo ) { return static_cast<short>(wo); }
		
		// DWORD -> int
		static int cast( DWORD wo ) { return static_cast<int>(wo); }
		
	#if defined(_WIN64)
		// DWORD_PTR -> int
		static long cast( ULONG_PTR wo ){ return static_cast<long>(wo); }
	#endif
	
		// �Ȃɂ����Ȃ�
		template< typename X >
		static X cast( X x ) { return x; }
	};
	
  // bool
	template< typename T = BOOL, T False = FALSE, T True = TRUE >
	struct bool_
	{
		typedef bool value_type;
		typedef bool input_type;
		
		template< typename S >
		static value_type decode( S s ) { return s!=False; }
		
		template< typename S >
		static S encode( bool b ) { return (b? True:False); }		
	};
  
  // 1�����̃I�u�W�F�N�g
	template< typename Ctor, typename C1, typename Extract1, typename T = typename Ctor::type, typename In = const T& >
	struct object1
	{
		typedef T	value_type;
		typedef In	input_type;
		
		template< typename S >
		static value_type decode( S s ) 
		{ 
			typename C1::value_type a1 = C1::decode(s);
			return Ctor::eval( a1 ); 
		}
		
		template< typename S >
		static S encode( In x ) 
		{ 
			return C1::template encode<S>( Extract1::eval(x) ); 
		}
	};			
  
  // �f�[�^�����I�u�W�F�N�g���\�z
	template< typename Ctor, 
			  typename C1, typename Extract1, 
			  typename C2, typename Extract2, typename T = typename Ctor::type, typename In = const T& >
	struct object2
	{
		typedef T	value_type;
		typedef In	input_type;
	
		// ���ʃ��[�h�A��ʃ��[�h��������
		static value_type decode( DWORD_PTR s )
		{
			typename C1::value_type a1 = C1::decode( LOWORD(s) );
			typename C2::value_type a2 = C2::decode( HIWORD(s) );
			return Ctor::eval( a1, a2 );
		}
		template< typename So >
		static So encodeus( In in, So )
		{
			return (So)MAKELONG( C1::template encode<S1>( Extract1::eval(in) ), 
			                     C2::template encode<S2>( Extract2::eval(in) ) );
		}
		
		// WPARAM, LPARAM��������
		static value_type decode( wlparam s )
		{
			return Ctor::eval( s.w, s.l );
		}
		static wlparam encodeus( In in, wlparam )
		{
			return wlparam( C1::template encode<WPARAM>( Extract1::eval(in) ),
			                C2::template encode<LPARAM>( Extract2::eval(in) ) );
		}
		
		template< typename So >
		static So encode( In in )
		{
			return encodeus( in, So() );
		}
	};	
	
  // �|�C���^���Q�Ƃ��I�u�W�F�N�g���\�z
	template< typename Ctor, typename Extract, 
			  typename Ptr = typename Ctor::type::value_type*, typename T = typename Ctor::type, typename In = const T& >
	struct object_pointer
	{
		typedef T		value_type;
		typedef In		input_type;
		typedef Ptr		pointer_type;
		
		template< typename S >
		static value_type decode( S s )
		{ 
			pointer_type p = reinterpret_cast< pointer_type >(s);
			return Ctor::eval( *p ); 
		}
		
		template< typename S >
		static S encode( In in )
		{ 
			return reinterpret_cast<S>( ptr_cast( &(Extract::eval(in)) ) );
		}

		template< typename P > 
		static void* ptr_cast( P* p ) { return static_cast<void*>(p); }
		
		template< typename P > 
		static const void* ptr_cast( const P* p ) { return static_cast<const void*>(p); }
	};	
	
  // �\�z�݂̂̃I�u�W�F�N�g
	template< typename T >
	struct spec_object
	{
		typedef T value_type;
		typedef T input_type; // �_�~�[
		
		template< typename S >
		static T decode( S s ){ return T( s ); }
		
		// encode�͂Ȃ�
	};	
	
  // �I�u�W�F�N�g�̃f�[�^�����o��
	// �R���X�g���N�^
	template< typename T >
	struct ctor1
	{
		typedef T type;
		
		template< typename X >
		static type eval( X& x ){ return type(x); }
	};
	template< typename T >
	struct ctor2
	{
		typedef T type;
		
		template< typename X, typename Y >
		static type eval( X& x, Y& y ){ return type(x,y); }
	};  
  
	// �e���v�� decltype���g����
	template< typename Type, Type Ptr >
	struct field;
	
	template< typename Class, typename Ret, Ret (Class::*pMemFn)()const > // �����o�֐�
	struct field< Ret (Class::*)()const, pMemFn >
	{
		typedef Ret type;
		
		static Ret eval( const Class& cl ){ return (cl.*pMemFn)(); }
	};
	
	template< typename Class, typename Ret, Ret Class::*pMem > // �����o�ϐ�
	struct field< Ret Class::*, pMem >
	{
		typedef Ret type;
		
		static Ret eval( const Class& cl ){ return (cl.*pMem); }
	};
	
	template< typename Ret, typename T, Ret (*pGFn)(T) > // �O���[�o���֐��i�����ɃI�u�W�F�N�g���Ƃ�j
	struct field< Ret (*)(T), pGFn >
	{
		typedef Ret type;
		
		template< typename X >
		static Ret eval( X& x ){ return (*pGFn)(x); }
	};	
	
	template< typename X > X field_identity( X );
	#define GAMMO_FIELD( a ) field< decltype( field_identity(a) ), a >
	
	// �Ȃɂ����Ȃ�
	template< typename T = void >
	struct nothing
	{
		typedef T type;
		
		template< typename X >
		static X& eval( X& x ){ return x; }
	};
	
	/*
	object1< ctor1<index>, value<int>, field<int (index::*)()const, &index::value> >
	
	object2< ctor2<size>,
		value<long>, field<long (size::*)()const, &size::width>,
		value<long>, field<long (size::*)()const, &size::height> >
		
	object2< ctor2<wstring_ptr>,
		value<wchar_t*>, field<wchar_t* (*)( wstring&), &buf_begin<wchar_t> >,
		value<int>,           field<int (wstring::*)()const, &wstring::size> >,
		boost::reference_wrapper<wstring>
		>
	object_pointer< raw<window_create_info>, raw<> >
	object_pointer< event::FIELD(&make_size), event::FIELD(&get_size) >	
	
	*/
	
  //
  // �v���Z�b�g ���낢��
  // 
	// nullable�Ȃ� X::value_type X::const_reference x.value �������OK
	template< typename T, typename Va = typename T::value_type >
	struct value_object
			: object1< ctor1<T>, value<Va>, field< typename T::const_reference (T::*)()const, &T::value > >
	{};

	// �����̎Q��
	template< typename T >
	struct object_cref
			: object_pointer< nothing<>, nothing<>, T*, const T&, const T& >
	{};  
	template< typename PropSet >
	struct property_cref
			: object_cref< typename PropSet::value_type >
	{};  
	
	// �Q�Ƃ킽��
	template< typename T >
	struct object_ref
			: object_pointer< nothing<>, nothing<>, T*, T&, T& >
	{};  
	template< typename PropSet >
	struct property_ref
			: object_ref< typename PropSet::value_type >
	{};  
  
	// point 
	typedef object2< ctor2<point>, 
				signed_value< point::value_type >, GAMMO_FIELD( &point::x ),
				signed_value< point::value_type >, GAMMO_FIELD( &point::y )				
			>
			point_;
			
	// size 
	typedef object2< ctor2<size>,
				signed_value< size::value_type >, GAMMO_FIELD( &size::width ),
				signed_value< size::value_type >, GAMMO_FIELD( &size::height )			
			>
			size_;
			
	// size ref
	typedef object_pointer< 
				field< size (*)( const SIZE& ),  &gammo::make_size >, 
				field< SIZE& (size_getter::*)(), &size_getter::value >,
				SIZE*, size, size_getter
			>
			size_out;
 
	// pair
	template< typename C1, typename C2, typename Pair = std::pair< typename C1::value_type, typename C2::value_type > >
	struct pair_
			: object2< ctor2<Pair>, 
				C1, GAMMO_FIELD( &Pair::first ), 
				C2, GAMMO_FIELD( &Pair::second )
			>
	{};	
	
	// ������̃|�C���^
	template< typename Ch = char_t, typename Ptr = chikuwa::string_ptr<Ch> >
	struct string_pointer
			: object1< ctor1< Ptr >, 
				value< const Ch* >, GAMMO_FIELD( &Ptr::c_str )
			>
	{};
	
	// �o�b�t�@��n��
	template< typename Ch = char_t, typename Ptr = chikuwa::string_ptr<Ch>, typename Str = std::basic_string<Ch> >
	struct string_out
			: object2< ctor2< Ptr >, 
				value< size_t >,    field< size_t (Str::*)()const, &Str::size >,		// �T�C�Y
				value< Ch* >,		field< Ch* (*)( Str& ),  &oden::buf_begin<Ch> >,	// �|�C���^�̕���
				Ptr, Str&
			>
	{};
			
	// array
	/*
	template< typename T, typename Ptr = string_ptr<Ch> >
	struct array_
			: object2< ctor2< SPtr >, 
				value< const Ch* >, GAMMO_FIELD( &SPtr::c_str ),
				value< size_t >, GAMMO_FIELD( &SPtr::size ),
				boost::reference_wrapper< std::basic_string<Ch> >
			>
			*/	
	
  //  arg_def�̃v���Z�b�g
    // ���̂܂�
	template< typename Tag, typename T = typename source<Tag>::type >
	struct arg_def_raw
		: arg_def< value<T>, Tag >
	{};	
	
  
//##############################################
// �C�x���g�������т̒�`
//	
  // WPARAM
	// ����
	template< typename C1, typename C2 = void >
	struct wparam_
	{
		typedef arg_def< C1, src_wparam_low >  arg_def1;
		typedef arg_def< C2, src_wparam_high > arg_def2;
		typedef typelist< arg_def1, arg_def2 > 		   arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1, typename arg_def2::input_type a2 )
		{ 
			return wlparam( MAKEWPARAM( arg_def1::encode_message(a1), arg_def2::encode_message(a2) ), 0 );
		}
	};	
	
	// ���̂܂�
	template< typename C1 >
	struct wparam_<C1,void>
	{
		typedef arg_def< C1, src_wparam >      arg_def1;
		typedef typelist< arg_def1 >	 		   arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1 )
		{ 
			return wlparam( arg_def1::encode_message(a1), 0 );
		}
	};
	
  // LPARAM
	// ����
	template< typename C1, typename C2 = void >
	struct lparam_
	{
		typedef arg_def< C1, src_lparam_low >  arg_def1;
		typedef arg_def< C2, src_lparam_high > arg_def2;	
		typedef typelist< arg_def1, arg_def2 > arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1, typename arg_def2::input_type a2 )
		{ 
			return wlparam( 0, MAKELPARAM( arg_def1::encode_message( a1 ), arg_def2::encode_message( a2 ) ) );
		}
	};	
	
	// ���̂܂�
	template< typename C1 >
	struct lparam_<C1,void>
	{
		typedef arg_def< C1, src_lparam >      arg_def1;
		typedef typelist< arg_def1 >	 		arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1 )
		{
			return wlparam( 0, arg_def1::encode_message( a1 ) );
		}
	};
	
  // WPARAM �� LPARAM
	// WPARAM������LPARAM
	template< typename C1, typename C2 = void, typename C3 = void >
	struct wparam_lparam
	{
		typedef arg_def< C1, src_wparam_low >				arg_def1;
		typedef arg_def< C2, src_wparam_high >				arg_def2;	
		typedef arg_def< C3, src_lparam >					arg_def3;	
		typedef typelist< arg_def1, arg_def2, arg_def3 >	arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1, typename arg_def2::input_type a2, typename arg_def2::input_type a3 )
		{
			return wlparam( MAKEWPARAM( arg_def1::encode_message( a1 ), arg_def2::encode_message( a2 ) ), arg_def3::encode_message( a3 ) );
		}
	};	
	
	// ���̂܂ܓ��
	template< typename C1, typename C2 >
	struct wparam_lparam<C1,C2>
	{
		typedef arg_def< C1, src_wparam >			arg_def1;
		typedef arg_def< C2, src_lparam >			arg_def2;	
		typedef typelist< arg_def1, arg_def2 >      arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1, typename arg_def2::input_type a2 )
		{ 
			return wlparam( arg_def1::encode_message( a1 ), arg_def2::encode_message( a2 ) );
		}
	};	
  
	// �����ɂ܂Ƃ߂�
	template< typename C1 >
	struct wparam_lparam<C1>
	{
		typedef arg_def< C1, src_wlparam >			arg_def1;
		typedef typelist< arg_def1 >				arg_defs;
		
		static wlparam encode_whole( typename arg_def1::input_type a1 )
		{ 
			return arg_def1::encode_message( a1 );
		}
	};	

	// �Ȃ��ł�
	struct noparam
	{
		typedef typelist<> arg_defs;

		static wlparam encode_whole() {	return wlparam(0,0); }
	};
	
  // result	
	template< typename C >
	struct result_
	{
		typedef arg_def< C, src_result > result_def;
	};
	
} /* end of namespace event */

//#############################################################
// �C�x���g�����̌^�A���������@���`
//	
//
  // template - ������C�x���g���Ƃɓ��ꉻ event_sig���p������
	template< typename EvDef >
	struct event_arg
	{
		typedef typelist0 arg_defs; // �f�t�H���g�ŋ�
	};

//#############################################################
// �C�x���g�����̒�`�𑀍삷��
//	
  // def�����o��
	template< typename EvDef >
	struct event_arg_defs
	{
		typedef typename event_arg<EvDef>::arg_defs type;
	};

  // �v���[�X�z���_�[���Ƃ�arg_def�����o��
	template< typename EvDef, typename At >
	struct event_arg_def;
	
	// �C�x���g�ŗL�̈���
	template< typename EvDef >
	struct event_arg_def< EvDef, event::arg1 >
		: typelist_at< typename event_arg_defs<EvDef>::type, 0 >
	{};

	template< typename EvDef >
	struct event_arg_def< EvDef, event::arg2 >
		: typelist_at< typename event_arg_defs<EvDef>::type, 1 >
	{};

	template< typename EvDef >
	struct event_arg_def< EvDef, event::arg3 >
		: typelist_at< typename event_arg_defs<EvDef>::type, 2 >
	{};

	template< typename EvDef >
	struct event_arg_def< EvDef, event::arg4 >
		: typelist_at< typename event_arg_defs<EvDef>::type, 3 >
	{};	

	// �ėp�̈���
	template< typename EvDef >
	struct event_arg_def< EvDef, event::raw >
		: identity< event::arg_def_raw< event::src_event > >
	{};

	template< typename EvDef >
	struct event_arg_def<EvDef, event::target>
		: identity< event::arg_def_raw<event::src_target> >
	{};

	template< typename EvDef >
	struct event_arg_def<EvDef, event::code>
		: identity< event::arg_def_raw<event::src_code> >
	{};

	template< typename EvDef >
	struct event_arg_def<EvDef, event::wparam>
		: identity< event::arg_def_raw<event::src_wparam> >
	{};

	template< typename EvDef >
	struct event_arg_def<EvDef, event::lparam>
		: identity< event::arg_def_raw<event::src_lparam> >
	{};	
	
  // �^�݂̂����o���Ԃ�
	template< typename EvDef, typename At >
	struct event_arg_type
	{
		typedef typename event_arg_def< EvDef, At >::type def;
		typedef typename def::value_type type;
	};
	template< typename EvDef, typename At >
	struct event_arg_param_type
	{
		typedef typename event_arg_def< EvDef, At >::type def;
		typedef typename def::param_type type;
	};

  // �����̐�
	template< typename EvDef >
	struct event_arity
	{
		typedef typename event_arg_defs< EvDef >::type defs;
		
		enum { value = defs::size };
	};

//#############################################################
// �C�x���g�����̌^�A���������@���`
//
  // �Ԃ�l���C�x���g�ɂ��邩�H
	template< typename Sig >
	struct event_has_result_tester
	{
		template< typename X, typename Test = void >
		struct tester
		{
			enum { value = 0 };
		};
		template< typename X >
		struct tester< X, typename ignore< typename event_arg<X>::result_def >::type >
		{
			enum { value = 1 };
		};

		typedef tester<Sig> type;
	};

	template< typename Sig >
	struct event_has_result
	{
		enum { value = event_has_result_tester<Sig>::type::value };
	};
	
  // �擾
	template< typename Sig >
	struct event_result_def
	{
		typedef typename event_arg<Sig>::result_def type;
	};

  // ���ۂ̕Ԃ�l�^ - arg_def�ɋL�q���ꂽ�^�ł͂Ȃ��R���i���b�p�[�j
	template< typename EvDef, int Enable = event_has_result<EvDef>::value >
	class event_result
	{
	public:
		typedef typename event_result_def<EvDef>::type 	def;		
		typedef typename def::value_type	   			value_type;
		typedef typename def::param_type	  			param_type;
		typedef event_result				   			self;
		
		event_result( window_event& r )
			: p_( &r )
		{}

		template< typename T >
		void set( T t )const
		{
			p_->result() = def::encode_message( t );
		}

		template< typename T >
		self& operator= ( T t )
		{
			this->set( t );
			return *this;
		}

		window_event& event()const { return *p_; }
	private:
		window_event* p_;
	};

	template< typename E >
	class event_result<E, 0>
	{
	public:
		event_result( window_event& ){}
	};

    // arg_def��`
	template< typename EvDef >
	struct event_arg_def<EvDef, event::result>
		: identity< 
			event::arg_def< event::spec_object< event_result<EvDef> >, event::src_event >
		> {};

//##################################################
// �S�ẴC�x���g�����E�Ԃ�l���܂ރ^�v��
//
namespace detail
{
    // ����
	template< typename Defs, int N >
	class arg_tuple;

    // �ǂ�ǂ�p�����Ă���
	#define GENarg_tuple( N, ARG ) \
	template< typename EvDef >												\
	class arg_tuple< EvDef, N >	: public arg_tuple< EvDef, N-1 >			\
	{																		\
	public:																	\
		typedef typename event_arg_def< EvDef, event::##ARG >::type			\
															def##N;			\
		typedef typename def##N::value_type					value_type##N;	\
		typedef typename def##N::param_type					param_type##N;	\
																			\
		explicit arg_tuple( event::event_ref ev )							\
			: ARG##_( ev ), arg_tuple< EvDef, N-1 >( ev ) {}				\
																			\
		param_type##N ARG ()const { return ARG##_.value; }					\
	private:																\
		event::arg_cradle<def##N> ARG##_;									\
	};
	
	// ����
	template< typename ED >
	class arg_tuple<ED, -1> {
	public:
		explicit arg_tuple( event::event_ref ) {}
	};	

	// 0~3
	GENarg_tuple( 0, result )
	GENarg_tuple( 1, _1 )
	GENarg_tuple( 2, _2 )
	GENarg_tuple( 3, _3 )

	#undef GENarg_tuple
}

  // �C���^�[�t�F�[�X
	template< typename EvDef, int Arity = event_arity<EvDef>::value >
	class event_args
		: public detail::arg_tuple< EvDef, Arity >
	{
	public:
		typedef detail::arg_tuple< EvDef, Arity > base;
		typedef typename base::value_type0		  result_type;

		explicit event_args( event::event_ref e )
			: base( e ) 
		{}
	};	

    // arg_def���`
	template< typename EvDef >
	struct event_arg_def< EvDef, event::args >
		: identity< 
			event::arg_def< event::spec_object< event_args<EvDef> >, event::src_event > 
		> {};
	
//##################################################
//  �C�x���g�̓���ȕ���
//
  // ���ꉻ�Œ�`
	template< typename EvDef >
	struct event_branch;

  // ���b�Z�[�W�R�[�h �݂̂Ŕ��f
	template< typename Code >
	struct event_branch< event_def<Code> >
	{
		static bool branch( const window_event& ev )
		{
			return ev.code() == Code::value;
		}
	};

	template<>
	struct event_branch< any_event_def >
	{
		static bool branch( const window_event& )
		{
			return true;
		}
	};

  // 
	template< typename L, typename R >
	struct event_branch< event_def_or<L,R> >
	{
		static bool branch( const window_event& ev )
		{
			return event_branch<L>::branch( ev ) || event_branch<R>::branch( ev );
		}
		
		template< typename A1 > 
		static bool branch( const window_event& ev, A1& a1 )
		{
			return event_branch<L>::branch( ev, a1 ) || event_branch<R>::branch( ev, a1 );
		}

		template< typename A1, typename A2 > 
		static bool branch( const window_event& ev, A1& a1, A2& a2 )
		{
			return event_branch<L>::branch( ev, a1, a2 ) || event_branch<R>::branch( ev, a1, a2 );
		}
	};

//##################################################
//	�C�x���g�^�O�̃G�C���A�X����������N���X
//     �P�ɏ����₷����ڎw������
//
  // ���ꉻ���Ē�`
	template< typename Src, typename A1 = unused, typename A2 = unused >
	struct event_alias
	{
		typedef Src type;
	};

//##################################################
//  �C�x���g�E�G���R�[�_
//
  // ��{��event_arg�𗬗p
  // construct���g�p����ꍇ�ȂǂőΉ�������Ȃ���Γ��ꉻ
	template< typename EvDef >
	struct event_message_arg : event_arg<EvDef>
	{};	

	template< typename EvDef >
	struct event_message_arity
	{
		typedef typename event_message_arg<EvDef>::arg_defs defs;
		enum { value = defs::size };
	};
	
  // �G���R�[�_�E�^�O�G�C���A�X
  //
  // �����������Ƃ�C�x���g���܂Ƃ߂���
  // event_alias�Ƃ͕ʂɒ�`����
	template< typename Src, typename Obj >
	struct event_message_alias
	{
		typedef Src type;
	};
  
  // ���b�Z�[�W�����^���擾
	template< typename EvDef, int At >
	struct event_message_input_type
	{
		typedef event_message_arg<EvDef>  	margs;
		typedef typename margs::arg_defs 	defs;	
		typedef typename typelist_at< defs, At >::type def;
		
		typedef typename def::input_type type;
	};
	
//##################################################
//  �o���o���ɋ@�\�𗘗p
//
	// decode
	template< typename EvDef, typename At >
	typename event_arg_type<EvDef, At>::type 
		get_event_arg( window_event& e )
	{
		return event_arg_def<EvDef,At>::type::decode_message(e);
	}
	
	// encode
	template< typename EvDef, int At >
	typename event_arg_def<EvDef, event::placeholder<At> >::source_type
		make_event_param( typename event_message_input_type<EvDef,At>::type arg )
	{
		return event_arg_def<EvDef, event::placeholder<At> >::type::encode_message(e);
	}
	
//##################################################
//  �}�N��
//

// �C�x���g�^�O�E���b�Z�[�W���`
#define GAMMO_DEFINE_EVENT_DETAIL( Define, Name, Code, Alt, ParamNum )  \
																		\
	namespace event   {													\
		typedef Define< event_code< Code, Alt > > Name;					\
	}																	\
																	    \
	namespace message {												    \
		typedef event_message<event::Name, ParamNum> Name;			    \
	}

#define GAMMO_DEFINE_EVENT( Define, Name, Code, ParamNum ) \
		GAMMO_DEFINE_EVENT_DETAIL( Define, Name, Code, void, ParamNum )

#define GAMMO_DEFINE_EVENT_ALT( Define, Name, Code, AltCode, ParamNum ) \
		GAMMO_DEFINE_EVENT_DETAIL( Define, Name, Code, event_code<AltCode>, ParamNum )

#define GAMMO_DEFINE_NOTIFY_EVENT( Define, Name, Code ) \
		GAMMO_DEFINE_EVENT( Define, Name, Code, event_message_arity< Define< event_code<0> > >::value )

#define GAMMO_DEFINE_NULL_EVENT( Define, Name, Code ) \
		GAMMO_DEFINE_EVENT( Define, Name, Code, 0 )			\
		template<>											\
		struct event_arg<event::Name> : event::noparam {};


}}

#endif








