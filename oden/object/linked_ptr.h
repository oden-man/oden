#pragma once

// リンクリスト方式スマートポインタ

/*
*
* SwapPtrに非対応
* WeakPtrに非対応（循環参照に注意）
*
*/
#pragma once

#ifndef ODEN_OBJ_LINKED_P
#define ODEN_OBJ_LINKED_P

// ----------- std -----------------
#include <ostream>
#include <cstddef>

// ----------- boost -----------------
#include <boost/compressed_pair.hpp>
#include <boost/checked_delete.hpp>

// ----------- oden -----------------
#include <oden/object/link_ref.h>

namespace oden
{
//_____________________________________________________________________________
//
//
//_____________________________________________________________________________
//
//######################################################
// スマートポインタ
//
	template< typename T, typename D = boost::checked_deleter<T> >
	class linked_ptr
	{
		template< typename, typename > friend class linked_ptr;
	public:	
		typedef linked_ptr<T,D> self;
		typedef D deleter_type;

		typedef T 	element_type;
		typedef T 	value_type;
		typedef T* 	pointer;
		typedef T& 	reference;

	private:
		typedef boost::compressed_pair<pointer,deleter_type> inner_data_type;
	public:
		
	// コンストラクタ
		linked_ptr()
			: data_(pointer(0)), cn_()
		{}

		explicit linked_ptr( pointer pNew, const deleter_type& del = deleter_type() )
			: data_( pNew, del ), cn_() 
		{}
		
		template< typename Y >
		explicit linked_ptr( Y* pNew, const deleter_type& del = deleter_type() )
			: data_( pNew, del ), cn_() 
		{}

		linked_ptr( const self &src )
			: data_( src.data_ ), cn_( src.cn_ )
		{}		
		
		template< typename Y,typename E >
		linked_ptr( const linked_ptr<Y,E> &src )
			: data_( src.get(), src.get_deleter() ), cn_( src.cn_ )
		{}

	// デストラクタ
		~linked_ptr() 
		{ this->deletes(); set_pointer(0); }
		
	  // 代入
	  	self& operator=( const self& other )
		{
			if( this->get() == other.get() )
				return *this;
			
			this->deletes();

			cn_ = other.cn_;
			data_ = other.data_;
			return *this;
	  	}
	  	template< typename Y, typename E >
	  	self& operator=( const linked_ptr<Y,E>& src )
	  	{
			if( this->get() == other.get() )
				return *this;
			
			this->deletes();

			cn_ = other.cn_;
			data_ = other.data_;
			return *this;
	  	}
	  	
	  // reset
	  	void reset()
	  	{
			reset(pointer(0));
	  	} 
		template< typename Y >
		void reset( Y* pNew )
		{
			if( get() != pNew )
			{
				this->deletes();

				cn_.depart();
				set_pointer(pNew);		
			}
		}
	  	
	  // 参照
	  	reference operator*()const
	  	{
	  		return *get();	
	  	}
	  	pointer operator->()const
	  	{
	  		return get();	
	  	} 
		
	  // get
	  	pointer get()const { return data_.first(); }
	  // deleter
		deleter_type& get_deleter() { return data_.second(); }
		const deleter_type& get_deleter()const { return data_.second(); }

	  // empty
		bool empty()const { return get()==0; }
		
	  // use_count==1
	  	bool unique()const
	  	{
	  		return ( cn_.alone() && get()!=0 );	
	  	}
		
	  // カウンタの取得(遅いです。)
	  	unsigned long use_count()const
	  	{
	  		if( get() == 0 )
	  			return 0;
	  		return cn_.node_count();	
	  	} 
	  	
	  // safe-bool
	  	typedef bool (linked_ptr::*bool_type)()const;
	  	operator bool_type ()const
	  	{
	  		return (get()!=0 ? &linked_ptr::unique : 0);
	  	}

	  // !
		bool operator!()const 
		{
			return empty();
		}
		
	  // データをそっくり入れ替える
		void swap( self& r )
		{
			cn_.swap(r.cn_);
			std::swap( data_, r.data_ );
		}
		friend void swap( self& l, self& r )
		{
			l.swap(r);
		}
		
	private:
		void set_pointer( pointer p ){ data_.first() = p; }
		void deletes()
		{
			 if( cn_.alone() )
			 	get_deleter()( get() );
		}

		inner_data_type data_;
		link_ref cn_;
	};	
	
  // 比較演算子
  	template< typename T, typename D, typename U, typename E >
	inline bool operator==(const linked_ptr<T,D>& l, const linked_ptr<U,E>& r)
	{
		return l.get() == r.get();
	}
  	template< typename T, typename D, typename U, typename E >
	inline bool operator!=(const linked_ptr<T,D>& l, const linked_ptr<U,E>& r)
	{
		return l.get() != r.get();
	}
  	template< typename T, typename D, typename U, typename E >
	inline bool operator<(const linked_ptr<T,D>& l, const linked_ptr<U,E>& r)
	{
		return l.get() < r.get();
	}
  	
  // ストリーム出力 operator <<
	template< typename E,typename X,typename T,typename D>
	inline std::basic_ostream<E,X>& operator<< ( std::basic_ostream<E,X>& os, const linked_ptr<T,D>& src )
	{
		os<<src.get();
		return os;	
	}

	template< typename T >
	linked_ptr<T> make_link_shared( T* pNew )
	{
		return linked_ptr<T>(pNew);
	}

}

#endif