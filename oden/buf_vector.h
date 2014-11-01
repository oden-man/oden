#pragma once
#ifndef ODEN_BUFCONT
#define ODEN_BUFCONT

#include <cassert>
#include <vector>

namespace oden
{
//#######################################################
//  スタック
// 	
	template< int N >
	struct buf_use_array;	
	
	struct buf_use_vector;

  // traits
	// コンテナ
	template< typename T, typename Cont = buf_use_vector >
	struct buf_container_traits
	{		
		typedef Cont							container_type;
		typedef typename Cont::iterator 		iterator;
		typedef typename Cont::const_iterator 	const_iterator;

		template< typename C >
		static bool extend( C& cont, unsigned int size )
		{
			if( size > cont.size() )
				cont.resize(size); 
			return true;
		}
		
		template< typename C >
		static void shrink( C& cont, unsigned int size )
		{
			cont.resize( size );
			C(cont).swap(cont);
		}
		
		template< typename C >
		static iterator begin( C& cont ) { return cont.begin(); }
		
		template< typename C >
		static const_iterator begin( const C& cont ) { return cont.begin(); }
	};
 
	// 配列
	template< typename T, int N >
	struct buf_container_traits< T, buf_use_array<N> >
	{
		typedef T 					container_type [N];
		typedef T* 					iterator;
		typedef const T* 			const_iterator;
		
		template< typename C >
		static bool extend( C&, unsigned int size )
		{
			if( size > N )
				return false;
			return true; 
		}
		
		template< typename C >
		static void shrink( C&, unsigned int ) {}
		
		template< typename C >
		static iterator begin( C& cont ) { return &cont[0]; }
		
		template< typename C >
		static const_iterator begin( const C& cont ) { return &cont[0]; }
	};
	
	template< typename T >
	struct buf_container_traits< T, buf_use_vector >
		 : buf_container_traits< T, std::vector<T> >
	{};
	

  // インターフェース
	template< typename T, typename Cont = buf_container_traits<T> >
	class buf_vector
	{
	public:
		typedef buf_vector<T, Cont>							self;
		typedef buf_container_traits<T, Cont>				container_traits;
		typedef typename container_traits::container_type	container_type;
		typedef typename container_traits::iterator			iterator;
		typedef typename container_traits::const_iterator	const_iterator;
	
		buf_vector()
			: back_(-1)
		{}
		
		bool push_back( const T& e )
		{
			int idx = back_ + 1;
			
			if( !container_traits::extend( cont_, idx+1 ) )
				return false;

			cont_[idx] = e;
			back_ = idx;
			
			return true;
		}
		void pop_back()
		{
			if( back_ >= 0 )
				--back_;
		}
		
		T& back() { assert(back_>=0); return cont_[back_]; }
		const T& back()const { assert(back_>=0); return cont_[back_]; }
		
		T& back( int i ) { assert(back_>=i); return cont_[back_-i]; }
		const T& back( int i )const { assert(back_>=i); return cont_[back_-i]; }
		
		T& front() { assert(back_>=0); return cont_[0]; }
		const T& front()const { assert(back_>=0); return cont_[0]; }
		
		T& front( int i ) { assert(back_>=i); return cont_[i]; }
		const T& front( int i )const { assert(back_>=i); return cont_[i]; }
		
		T& operator[]( int i ) { return front(i); }
		const T& operator[]( int i )const { return front(i); }
		
		void set_back( int i )
		{
			if( back_>=i )
				back_ = i; 
		}
		void set_size( int i )
		{
			this->set_back( i-1 );
		}
		
		iterator begin() { return container_traits::begin( cont_ ); }
		const_iterator begin()const { return container_traits::begin( cont_ ); }
		
		iterator end() { return this->begin() + (back_ + 1); }
		const_iterator end()const { return this->begin() + (back_ + 1); }
		
		int size()const { return back_ + 1; }
		bool empty()const { return back_ == -1; }
		void clear() 
		{
			back_ = -1;
		}
		
		void shrink_to_fit()
		{
			container_traits::shrink( cont_, back_+1 );
		}
		
		const container_type& container()const { return cont_; }
		
		void swap( self& r )
		{
			using std::swap;
			swap( cont_, r.cont_ );
			swap( back_, r.back_ );
		}
		
	private:
		container_type cont_;
		int  back_;
	};

}

#endif
