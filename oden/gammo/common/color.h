#pragma once

#ifndef ODEN_GAMMO_CMN_COL
#define ODEN_GAMMO_CMN_COL

//################################################
//   インクルード
//
// --------- defs -----------
#include <oden/gammo/defs.h>

// ----------- stl --------------
#include <cmath>

// ----------- win --------------
// ----------- boost ------------
//#include <boost/integer.hpp>

//
//################################################

namespace oden { namespace gammo
{
//#########################################################
//  8bitのRGB
//
	class rgb8
	{
	public:
		typedef unsigned char element_type;
	
	// コンストラクタ
		rgb8( )
		 : r_( 0 ), g_( 0 ), b_( 0 )
		{}
		rgb8( element_type r, element_type g, element_type b )
		 : r_( r ), g_( g ), b_( b )
		{}
		rgb8( colorref ref )
		 : r_( 0 ), g_( 0 ), b_( 0 )
		{
			set(ref);
		}
		
	// プロパティ
		element_type red()const   { return r_; }
		element_type green()const { return g_; }
		element_type blue()const  { return b_; }
		
		element_type& rred()	  { return r_; }
		element_type& rgreen()	  { return g_; }
		element_type& rblue()	  { return b_; }
		
		void set( element_type r, element_type g, element_type b ) {
			r_ = r; 
			g_ = g; 
			b_ = b;
		}
		
		colorref get_colorref()const 
		 { return RGB( r_, g_, b_ ); }
		void set( colorref ref ) 
		{
			r_ = static_cast<element_type>(GetRValue(ref.value()));
			g_ = static_cast<element_type>(GetGValue(ref.value()));
			b_ = static_cast<element_type>(GetBValue(ref.value()));
		}
			
		RGBQUAD get_quad()const  
		{
			RGBQUAD q_; 
			get_quad(q_);
			return q_; 
		}
		void get_quad( RGBQUAD& out )const 
		{
			 out.rgbRed    = static_cast<BYTE>(r_);
			 out.rgbGreen  = static_cast<BYTE>(g_);
			 out.rgbBlue   = static_cast<BYTE>(b_);
			 out.rgbReserved = 0;
		}
		void set( const RGBQUAD& quad ) 
		{
			r_ = static_cast<element_type>(quad.rgbRed);
			g_ = static_cast<element_type>(quad.rgbGreen);
			b_ = static_cast<element_type>(quad.rgbBlue);			
		}
		
	// 変換
		operator colorref()const 
		  { return get_colorref(); }
		operator RGBQUAD()const 
		  { return get_quad(); }
		
	private:
		element_type r_;
		element_type g_;
		element_type b_;
	};
	
//#########################################################
//  8bitのRGB
//
	class rgba8;
	
	class hsv8;
	
	class hsl8;
	
	class cmyk8;

}
}


#endif