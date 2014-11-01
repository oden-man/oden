#ifndef ODEN_CHKW_STR_STRPTR
#define ODEN_CHKW_STR_STRPTR

#include <string>
#include <oden/memory/memory_range.h>
#include <oden/chikuwa/std_funcs.h>

namespace oden { namespace chikuwa
{
//##################################################
//  string_ptr_base 
//
	template< typename ChT, bool const_ >
	class string_ptr_base
		: public memory_range_base< ChT, const_ >
	{
	public:
		typedef ChT char_type;
	protected:
		typedef memory_range_base< ChT, const_ > base;
		
	// コンストラクタ ( protected )
		string_ptr_base()
		{}
		string_ptr_base( typename base::pointer p, size_t len )
			: base( p, len )
		{}

	public:		
	// 演算子
	  // =
		template< typename X >
	  	string_ptr_base& operator= ( const X& x ) 
		{ set(x); return *this; }

	// メンバ関数
		const char_type* c_str( )const { return get(); }

		void set( typename base::pointer p ){ string_ptr_base(p).swap(*this); }

		template< typename String >
		void to_string( String& out )
		{
			if( !is_null() )
				out.assign( this->get(), this->size() );
		}
		
		//void remeasure() { this->set( this->get() ); }
	};
	
//##################################################
//  string_ptr (const CharT)
//
	template< typename T >
	class string_ptr
		: public string_ptr_base<T, true>
	{
	private:
		typedef string_ptr_base<T, true> base;
	public:		
	// コンストラクタ
		string_ptr()
		{}
		string_ptr( boost::none_t )
		{}
		string_ptr( const int )
		{}
		string_ptr( typename base::pointer p )
			: base( p, ( p ? str::length(p) : 0 ) )
		{}
		template< typename String >
		string_ptr( const String& str, const typename String::iterator* = 0 )
			: base( str.c_str(), str.size() )
		{}
	};
	
//##################################################
//  関数Ver
//
  // traits
	template< typename T >
	struct string_traits
	{
		typedef typename T::value_type	  char_type;
		typedef typename T::pointer		  pointer;
		typedef typename T::const_pointer const_pointer;
	};
	
	template< typename T >
	struct string_traits<T*>
	{
		typedef T	  char_type;
		typedef T*	  pointer;
		typedef const T* const_pointer;
	};
	template< typename T >
	struct string_traits<const T*> : string_traits<T*>
	{};

	template< typename T, std::size_t N >
	struct string_traits<T [N]>
	{
		typedef T	  char_type;
		typedef T*	  pointer;
		typedef const T* const_pointer;
	};
	template< typename T, std::size_t N >
	struct string_traits<const T[N]> : string_traits<T[N]>
	{};

  // c_strを呼び出す : ポインタならそのまま
	template< typename T >
	typename string_traits<T>::const_pointer string_get( const T& t )
	{
		return t.c_str();
	}
	template< typename T >
	T* string_get( T* p )
	{
		return p;
	}
	template< typename T >
	const T* string_get( const T* p )
	{
		return p;
	}

  // sizeを呼び出す : ポインタなら計測する
	template< typename T >
	int string_length( const T& t )
	{
		return t.size();
	}
	template< typename T >
	int string_length( const T* p )
	{
		return str::length(p);
	}
}}

#endif

