#pragma once

#ifndef ODEN_GAMMO_UI_CTL_RCHEDIT
#define ODEN_GAMMO_UI_CTL_RCHEDIT

//#######################################################
//  �C���N���[�h
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
//            �G�f�B�b�g �R���g���[�� �N���X
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
		  // �}�X�N
			Me& mask( DWORD m ) {
				self::val(this).dwMask |= mask;
				return self::me(this);
			}
		  // �����G�t�F�N�g
			Me& effect( DWORD target, DWORD val ) {
				mask( target );
				return effect( val );
			}
			Me& effect( DWORD dw ) {
				self::val(this).dwEffects |= dw;
				return self::me(this);
			}
		  // �T�C�Y
	  		Me& height( LONG l ) {
				self::val(this).dwMask |= CFM_SIZE;
				self::val(this).yHeight = l;
				return self::me(this);
			}
		  // �I�t�Z�b�g
	  		Me& offset( LONG l ) {
				self::val(this).dwMask |= CFM_OFFSET;
				self::val(this).yOffset = l;
				return self::me(this);
			}
		  // �F
	  		Me& color( colorref c ) {
				self::val(this).dwMask |= CFM_COLOR;
				self::val(this).crTextColor = c.value();
				return self::me(this);
			}
		  // �w�i�F
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
				// CHARFORMAT2 ��POD�łȂ�(���N���X������)�̂ŁA�C�j�V�����C�U�Ń[���N���A����Ȃ�
				// �ł������������͘A�����Ă���悤�Ȃ̂ŁAmemset�͎g����?
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
//  �R���g���[���N���X
//
	class rich_edit
		: public edit
	{
	private:
		typedef rich_edit self;
	public:	
	// �R���X�g���N�^
		rich_edit( window_handle wh = boost::none )
			: edit( wh )
		{}
		
	// �����o�֐�
	  // ���C�u���������[�h����
		static module_handle load_library( int var = 2 );

	  // ��{�֐�
		using edit::select;
		void select( const char_range::value_type& rng )const;
		void get_selection( char_range::value_type& out )const;

	  // �X�^�C������
		// �����̃t�H�[�}�b�g��ς���
		bool set_char_format( UINT target, const char_format::value_type& fmt )const;
		// �����̔w�i��ς���
		colorref set_background( colorref c = boost::none )const;
		
	  // �I���ʒu�E�L�����b�g�ʒu	 
		
	  // �����s�̑���
		
	  // �����̐���
		
	  // ���C�A�E�g
		
	  // �R���g���[���̑���
		// �Ǝ����@�œǂݏ�������
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
