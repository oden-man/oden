#ifndef ODEN_MEM_RANGE
#define ODEN_MEM_RANGE

#include <cstddef>
#include <boost/none.hpp>
#include <oden/memory/memory_range_fwd.h>

namespace oden 
{
//##################################################
//  memory_range
//
	template< typename T, bool const_ = true >
	class memory_range_base
	{
	public:
		typedef T value_type;
		
		template< typename X, bool bCst > struct ptr
		{ 
			typedef const X* type;
		};
		template< typename X > struct ptr<X,false> 
		{ 
			typedef X* type;
		};			
		
		typedef typename ptr<T,const_>::type 		pointer;		
		typedef typename ptr<void,const_>::type 	void_pointer;	

		template< bool b >
		struct check { typedef void type; };
		
		template<>
		struct check<false> {}; // non-const -> const NG
		
	// �R���X�g���N�^
		memory_range_base()
			: ptr_( 0 ), len_( 0 )
		{}
		memory_range_base( pointer p, size_t len )
			: ptr_( p ), len_( len )
		{}

	// �����o�֐�
		pointer get()const			{ return ptr_; }
		size_t  length( )const 		{ return len_; }
		size_t  size( )const	  	{ return len_; }
		
		pointer begin( )const 		{ return get(); }		// �[��range
		pointer end( )const 		{ return begin() + size(); }	// 

		bool is_null()const			{ return ptr_==0; }
		
		void set( pointer p, size_t len )
		{ ptr_ = p; len_ = len; }
		
		// �قȂ�|�C���^�^����ϊ�
		template< typename X, bool cst >
		void set( const memory_range_base<X,cst>& other, typename ptr<X,cst>::type = ptr<X,const_>::type(0) )
		{
			ptr_ = static_cast<pointer>( static_cast<void_pointer>( other.get() ) );
			len_ = other.size() * sizeof(X) / sizeof(T);
		}		

		// �Q�Ƃ���f�[�^���r
		template< typename X >
		bool equal( const memory_range_base<X>& src, typename check<sizeof(X)==sizeof(value_type)>::type* = 0 )const
		{
	  		if( this->size() > src.size() )
				return false;

			for( size_t i = 0; i<len_; ++i ) 
			{
				if( ptr_[i] != src.get()[i] ) 
					return false;
			}
			return true;
		}

	// ���Z�q
	  // ==, !=
	  	friend bool operator == ( const memory_range_base& l, const memory_range_base& r ) 
		{
			return (l.size() == r.size()) && (l.get() == r.get());
		}
	  	friend bool operator != ( const memory_range_base& l, const memory_range_base& r ) 
		{ return !(l==r); }
		
	  /*
	  // >
		friend bool operator < ( const memory_range_base& l, const memory_range_base& r )
		{
			return 
		} */
      
      // =
		memory_range_base& operator= ( memory_range_base& p )
		{
			set( p.ptr_, p.len_ );
			return *this;
		}
		memory_range_base& operator= ( boost::none_t )
		{
			set( 0, 0 );
			return *this;
		}

	// swap
		void swap( memory_range_base& o )
		{
			std::swap( ptr_, o.ptr_ );
			std::swap( len_, o.len_ );
		}

	private:
		pointer ptr_;
		size_t  len_;
	};

//##################################################
//  memory_range
//	
	template< typename T >
	class buffer_range
		: public memory_range_base<T,false>
	{
	private:
		typedef memory_range_base<T,false> base;
	public:
		typedef const T* const_pointer;

	// �R���X�g���N�^
		buffer_range( )
			: base()
		{}
		buffer_range( boost::none_t )
			: base()
		{}
		buffer_range( const buffer_range<T>& other )
			: base( other.get(), other.size() )
		{}
		buffer_range( typename base::pointer ptr, size_t size )
			: base( ptr, size )
		{}
		template< typename X >
		buffer_range( const buffer_range<X>& other )
			: base()
		{
			this->set( other );
		}
		
	// �����o�֐�
		const_pointer cget( )const		{ return get(); }
		const_pointer cbegin( )const 	{ return get(); }
		const_pointer cend( )const 		{ return cbegin() + size() + 1; }	
	};

	template< typename T >
	class memory_range
		: public memory_range_base<T>
	{
	private:
		typedef memory_range_base<T> base;
	public:
	// �R���X�g���N�^
		memory_range( )
			: base()
		{}
		memory_range( boost::none_t )
			: base()
		{}
		memory_range( const memory_range<T>& p )
			: base( p.get(), p.size() )
		{}
		memory_range( buffer_range<T> p )
			: base( p.cget(), p.size() )
		{}
		memory_range( typename base::pointer ptr, size_t size )
			: base( ptr, size )
		{}
		template< typename X >
		memory_range( const memory_range<X>& other )
			: base()
		{
			this->set( other );
		}
	};	
	
	typedef memory_range<void> any_memory_range;
	typedef buffer_range<void> any_buffer_range;
	
//##################################################
//�@�쐬
//	
  // �z��
	template< typename T, int N >
	struct container_to_carray< T[N] >
	{
		typedef T elem_type;
	
		template< typename Range, typename Container >
		static Range get( Container& cont )
		{
			return Range( cont, N );
		}
	};
	
namespace detail
{
	template< typename X >
	struct ignore { typedef void type; };
}
	
  // �P��̒l�ł���
	template< typename T, typename C = void >
	struct value_to_carray
	{
		typedef T type;
	};
	
	template< typename T >
	struct value_to_carray< T, typename detail::ignore< typename container_to_carray<T>::elem_type >::type >
	{};
	
//
//  �t���[�֐�
//
  // �l���o�C�g��Ƃ��Č��� ( T, U == POD )
	template< typename U, typename T >
		memory_range<U> 
			mem_range ( const T& val, typename value_to_carray<T>::type* = 0 )
	{
		return memory_range<U>( memory_range<T>(&val,1) );
	}
	
	template< typename U, typename T >
		buffer_range<U> 
			buf_range ( T& val, typename value_to_carray<T>::type* = 0 )
	{
		return buffer_range<U>( buffer_range<T>(&val,1) );
	}
	
  // �R���e�i�^�Ƃ��ă|�C���^���擾
	template< typename T >
		memory_range< typename container_to_carray<T>::elem_type > 
			mem_range ( const T& val )
	{
		typedef memory_range< typename container_to_carray<T>::elem_type > result_type;
		
		return container_to_carray<T>::get< result_type >(val);
	}
	
	template< typename T >
		buffer_range< typename container_to_carray<T>::elem_type > 
			buf_range ( T& val )
	{
		typedef buffer_range< typename container_to_carray<T>::elem_type > result_type;
		
		return container_to_carray<T>::get< result_type >(val);
	}
	
  // �^��ς��ă|�C���^���擾 ( T, U == POD )
	template< typename U, typename T >
		memory_range<U> 
			mem_range ( const T& val, typename container_to_carray<T>::elem_type* = 0 )
	{
		return memory_range<U>( mem_range(val) );
	}
	
	template< typename U, typename T >
		buffer_range<U> 
			buf_range ( T& val, typename container_to_carray<T>::elem_type* = 0 )
	{
		return buffer_range<U>( buf_range(val) );
	}

  // byte��
	template< typename T >
		memory_range<byte_type>
			byte_range( const T& val )
	{
		return mem_range<byte_type>( val );
	}
	
	template< typename T >
		buffer_range<byte_type>
			byte_buf_range( T& val )
	{
		return buf_range<byte_type>( val );
	}
	
//##################################################
//�@begin, end, size
//
  // mem range
	template< typename U, typename T >
		typename memory_range<U>::pointer 
			mem_begin( const T& val )
	{
		return mem_range<U>(val).begin();
	}

	template< typename U, typename T >
		typename memory_range<U>::pointer 
			mem_end( const T& val )
	{
		return mem_range<U>(val).end();
	}
	
  // buf range
	template< typename U, typename T >
		typename buffer_range<U>::pointer 
			buf_begin( T& val )
	{
		return buf_range<U>(val).begin();
	}

	template< typename U, typename T >
		typename buffer_range<U>::pointer 
			buf_end( T& val )
	{
		return buf_range<U>(val).end();
	}
	
  // byte��
	template< typename T >
		typename memory_range<byte_type>::pointer 
			byte_begin( const T& val )
	{
		return byte_range(val).begin();
	}
	
	template< typename T >
		typename memory_range<byte_type>::pointer 
			byte_end( const T& val )
	{
		return byte_range(val).end();
	}
	
	template< typename T >
		typename buffer_range<byte_type>::pointer 
			byte_buf_begin( T& val )
	{
		return byte_buf_range(val).begin();
	}
	
	template< typename T >
		typename buffer_range<byte_type>::pointer 
			byte_buf_end( T& val )
	{
		return byte_buf_range(val).end();
	}
	
  // mem/buf size
	template< typename U, typename T >
		size_t
			mem_range_size( const T& val )
	{
		return memory_range<U>(val).size();
	}
	
//##################################################
//�@�����̃A���S���Y��
//
	
}

#endif

