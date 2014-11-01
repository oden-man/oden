#pragma once

#ifndef ODEN_INDEX
#define ODEN_INDEX

#include <oden/nullable.h>

namespace oden 
{

//#################################################################
//  nullable typedef
//
	typedef nullable< int, 	  less_eq_null<int,-1> > 			integer;		/* -1 エラー 0 から正の整数 */
	typedef	integer									 			index;			/* = integer */
	typedef nullable< int, 	  less_eq_null<int,0> > 			positive;		/* 0 エラー 0 をふくまない正の整数 */
	typedef nullable< unsigned int,   
							  singular_null<unsigned int,
							  static_cast<unsigned int>(-1)> >	uinteger;		/*  0xffffffff がエラーになる UINT */

  // インデックスをつめる
	inline index clamp_index( index i, int mi, int mx )
	{
		if( i < mi )
			return mi;
		if( mx < i )
			return mx;
		return i;
	}	
	template< typename Cont >
	index clamp_index( const Cont& cont, index i )
	{
		return clamp_index( i, 0, cont.size() );		
	}
	template< typename T, int N >
	index clamp_index( T (&)[N], index i )
	{
		return clamp_index( i, 0, N );	
	}
	
  // インデックスチェック
	inline bool check_index( index i, int mi, int mx )
	{
		return mi <= i && i <= mx;		
	}	
	template< typename Cont >
	bool check_index( const Cont& cont, index i )
	{
		return i && i < cont.size();		
	}
	template< typename T, int N >
	bool check_index( T (&)[N], index i )
	{
		return i && i < N;		
	}
	
  // インデックスの範囲
	class index_range
	{
	public:
		index_range()
			: b_(), e_()
		{}
		index_range( index b )
			: b_(b), e_(b)
		{}
		index_range( index b, index e )
			: b_(b), e_(e)
		{}

		index begin()const{ return b_; }
		index tail()const { return e_ - 1; }
		index end()const{ return e_; }

		int distance()const 
		{ return (b_ < e_ ? e_ - b_ : b_ - e_).value_or(0); }

		bool is_valid()const { return b_ && e_; }
		bool is_empty()const { return !is_valid() || (b_ == e_); }

		void resize( size_t len ) { e_ = b_ + len; }
		index_range resized( size_t len )const 
		{
			index_range n(b_);
			n.resize(len);
			return n;
		}

		bool operator==( const index_range& r )const{ return (b_==r.b_) && (e_==r.e_); }
		bool operator!=( const index_range& r )const{ return !(*this==r); }
	private:
		index b_, e_;
	};
	
}

#endif
