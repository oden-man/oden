#pragma once

#ifndef ODEN_GAMMO_CMN_GEO
#define ODEN_GAMMO_CMN_GEO

//#############################################
// インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// -------- win -----------
// -------- boost ---------
// -------- oden ----------
#include <oden/geometry.h>

//
//
//#############################################


namespace oden { namespace gammo
{
//#######################################################
// xy座標点を表すDWORD値
//
	class dword_point
	{
	public:
	// コンストラクタ
		dword_point( )
			: dw_(0)
		{}
		explicit dword_point( DWORD dw )
			: dw_(dw)
		{}
		dword_point( WORD x, WORD y )
			: dw_(0)
		{
			set( x, y );
		}
		
	// 取得
		WORD x()const { return LOWORD(dw_); }
		WORD y()const { return HIWORD(dw_); }
		DWORD get()const { return dw_; }

	// 設定
		void set( WORD x, WORD y ){ dw_ = static_cast<DWORD>(MAKELONG( x, y )); }
		void set_x( WORD v ){ set( v, y() ); }
		void set_y( WORD v ){ set( x(), v ); }

	// 演算子
		operator DWORD() const { return dw_; }

	private:
		DWORD dw_;
	};


//#######################################################
// 四角形の寸法を表すDWORD値
//
	class dword_size
	{
	public:
	// コンストラクタ
		dword_size( )
			: dw_(0)
		{}
		explicit dword_size( DWORD dw )
			: dw_(dw)
		{}
		dword_size( WORD w, WORD h )
			: dw_(0)
		{
			set( w, h );
		}

	// 取得
		WORD  width ()const { return LOWORD(dw_); }
		WORD  height()const { return HIWORD(dw_); }

		DWORD get()const { return dw_; }

	// 設定
		void set( WORD w, WORD h ){ dw_ = static_cast<DWORD>(MAKELONG( w, h )); }

		void set_width( WORD w )  { set( w, height() ); }
		void set_height( WORD h ) { set( width(), h ); }

	// 演算子
		operator DWORD() const { return dw_; }

	private:
		DWORD dw_;
	};
	
//#######################################################
//  変換関数
//
  // POINT <-> point
	inline oden::point make_point( const POINT& pt )
	{
		return oden::point( pt.x, pt.y );
	}
	inline oden::point make_point( dword_point pt )
	{
		return oden::point( pt.x(), pt.y() );
	}
	
	inline void get_point( const oden::point& po, POINT& out )
	{
		out.x = po.x();
		out.y = po.x();
	}
	inline POINT get_point( const oden::point& po )
	{
		POINT pt = { po.x(), po.y() };
		return pt;
	}
	
  // SIZE <-> size
	inline oden::size make_size( const SIZE& sz )
	{
		return oden::size( sz.cx, sz.cy );
	}
	inline oden::size make_size( dword_size sz )
	{
		return oden::size( sz.width(), sz.height() );
	}
	
	inline void get_size( const oden::size& sz, SIZE& out )
	{
		out.cx = sz.width();
		out.cy = sz.height();
	}
	inline SIZE get_size( const oden::size& sz )
	{
		SIZE sze = { sz.width(), sz.height() };
		return sze;
	}
		
  // RECT <-> rect
	inline oden::rect make_rect( const RECT& r )
	{
		return oden::rect( r.left, r.top, r.right, r.bottom );
	}
	
	inline void get_rect( const oden::rect& r, RECT& out )
	{
		out.left = r.left();
		out.top = r.top();
		out.right = r.right();
		out.bottom = r.bottom();
	}
	inline RECT get_rect( const oden::rect& r )
	{
		RECT rt = { r.left(), r.top(), r.right(), r.bottom() };
		return rt;
	}
	
//#######################################################
//  変換バッファとしてPOINT, SIZE, RECTを持つ
//
namespace detail
{
	template< typename T, typename D, T (*pConv)( const D& ) >
	class value_getter
	{
	public:
		value_getter() : data() {}
		
		T get()const { return (*pConv)(data_); }
		operator T()const { return get(); }
		
		D& value()   { return data_; }		
	private:
		D data_;
	};
}
	typedef detail::value_getter< point, POINT, &make_point > 	point_getter;
	typedef detail::value_getter< size,  SIZE,  &make_size > 	size_getter;
	typedef detail::value_getter< rect,  RECT,  &make_rect > 	rect_getter;

}
}


#endif
