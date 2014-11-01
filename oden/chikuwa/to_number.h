#ifndef ODEN_CHKW_PARSE_TONUM
#define ODEN_CHKW_PARSE_TONUM


#include <oden/chikuwa/std_funcs.h>
#include <oden/chikuwa/string_ptr.h>

#include <boost/optional.hpp>

namespace oden { namespace chikuwa
{
//
//#####################################################
// 数値変換関数
//
namespace lex_cast_detail
{  
  // 型ディスパッチタグ
	template< int n >
	struct tag
	{ enum { value = n }; };
	
	typedef tag<1> long_tag;
	typedef tag<2> ulong_tag;
	typedef tag<3> double_tag;
	typedef tag<4> float_tag;
	typedef tag<5> longlong_tag;
	typedef tag<6> ulonglong_tag;
	typedef tag<7> longdouble_tag;
	typedef tag<10> other_tag;
	
	template< typename T >
	struct tester
	{ typedef char(&type)[T::value]; };
	
	#if defined(_MSC_VER)
		typedef unsigned __int64 ulonglong_t;
		typedef __int64 longlong_t;
	#else
		typedef unsigned long long ulonglong_t;
		typedef long long longlong_t;
	#endif

	tester<long_tag>::type        test( int );
	tester<ulong_tag>::type       test( unsigned int );
	tester<double_tag>::type      test( double );
	tester<long_tag>::type        test( long );
	tester<ulong_tag>::type       test( unsigned long );
	tester<float_tag>::type       test( float );
	tester<longlong_tag>::type    test( longlong_t );
	tester<ulonglong_tag>::type   test( ulonglong_t );
	tester<longdouble_tag>::type  test( long double );
	tester<other_tag>::type       test( ... ); //sfinae
	
  // 文字列から変換
	template< typename T > 
	struct cannot_parse_to {};
	
	template< typename T, typename Ch >
	struct parse_function
	{
		const Ch* ptr;
		int base;
		Ch** endpos;

		T operator()( long_tag )const { return static_cast<T>( str::to_long( ptr, base, endpos ) ); }

		T operator()( ulong_tag )const { return static_cast<T>( str::to_ulong( ptr, base, endpos ) ); } 

		T operator()( double_tag )const { return static_cast<T>( str::to_double( ptr, endpos ) ); }	
	
#if !defined ODEN_CHIKUWA_NO_FLOAT 
		T operator()( float_tag )const  { return static_cast<T>( str::to_float( ptr, endpos ) ); }
#endif
#if !defined ODEN_CHIKUWA_NO_LONGDOUBLE
		T operator()( longdouble_tag )const { return static_cast<T>( str::to_longdouble( ptr, endpos ) ); }	 
#endif
#if !defined ODEN_CHIKUWA_NO_LONGLONG 
		T operator()( longlong_tag )const { return static_cast<T>( str::to_longlong( ptr, base, endpos ) ); }
#endif
#if !defined ODEN_CHIKUWA_NO_ULONGLONG 
		T operator()( ulonglong_tag )const { return static_cast<T>( str::to_ulonglong( ptr, base, endpos ) ); }
#endif
		template< typename Other >
		T operator()( Other )const { return cannot_parse_to<T>(); /* generate compile error */ }
	
	};
	
	template< typename T, typename Ch >
	T parse_dispatch( const Ch* p, int base, Ch* &errpos )
	{
		parse_function<T, Ch> functor = { p, base, &errpos };
		return functor( tag<sizeof( test(T()) )>() );
	}
}

  // 文字列を数値に変換
    //  任意の進数
	template< typename T, typename Str >
	boost::optional<T> to_number( const Str& s, int base = 10 )
	{
		typename string_traits<Str>::const_pointer	str = string_get(s);
		typename string_traits<Str>::pointer		errstr = 0;
		T val = lex_cast_detail::parse_dispatch<T>( str, base, errstr );
		
		if( val != 0 || ( errstr && *errstr == 0 && *str != 0 ) )
		{
			return boost::optional<T>(val);
		}
		return boost::none;
	}
	
	// 16進数エイリアス
	template< typename T, typename Str >
	boost::optional<T> to_hex( const Str& str )
	{
		return to_number<T>( str, 16 );
	} 
  
}}

#endif

