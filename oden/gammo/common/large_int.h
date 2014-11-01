#pragma once

#ifndef ODEN_GAMMO_CMN_LINT
#define ODEN_GAMMO_CMN_LINT

//#############################################
// インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// -------- win -----------
// -------- boost ---------
// -------- oden ----------

//
//
//#############################################


namespace oden { namespace gammo
{
//#######################################################
// LARGE_INTEGER
//
	template< typename T, typename High, typename Low, typename Quad >
	class large_int_T
	{
	public:
	// コンストラクタ
		large_int_T()
			: lint_()
		{}
		large_int_T( High h, Low l )
			: lint_()
		{
			set(h,l);
		}
		large_int_T( Quad v )
			: lint_()
		{
			set(v);
		}	
		large_int_T( const T& v )
			: lint_(v)
		{}
		
	// プロパティ
		High high()const { return lint_.HighPart; }
		Low  low ()const { return lint_.LowPart; }
		Quad value()const { return lint_.QuadPart; }

		T& data() { return lint_; }
		const T& data()const { return lint_; }
		
		High& rhigh() { return lint_.HighPart; }
		Low&  rlow () { return lint_.LowPart; }
		Quad& rvalue() { return lint_.QuadPart; }

		void set( High h, Low l )
		{ 
			lint_.HighPart = h;
			lint_.LowPart = l;
		}
		void set( Quad v )
		{
			lint_.QuadPart = v;
		}
		void set( const T& v )
		{
			lint_ = v;
		}
	
	// 演算子オーバーロード
		large_int_T& operator=( Quad q )
		{
			set(q);
			return (*this);
		}
		large_int_T& operator=( const T& src )
		{
			set(src);
			return (*this);
		}

	  // order
		bool operator==( const large_int_T& other )const
		{
			return lint_.QuadPart == other.lint_.QuadPart;
		}
		
	// スワップ
		void swap( large_int_T& other )
		{
			std::swap( lint_, other.lint_ );
		}
		friend void swap( large_int_T& l, large_int_T& r )
		{
			l.swap(r);
		}
		
	private:
		T lint_;
	};

//
// typedef
//
	typedef large_int_T<LARGE_INTEGER, LONG, DWORD, LONGLONG> 		large_int;
	typedef large_int_T<ULARGE_INTEGER, DWORD, DWORD, ULONGLONG> 	ularge_int;
	
}
}


#endif
