#ifndef ODEN_CHKW_PARSE_TONUM
#define ODEN_CHKW_PARSE_TONUM

//#include <string>

#include <oden/chikuwa/std_funcs.h>
#include <oden/chikuwa/string_ptr.h>

#include <boost/optional.hpp>
//#include <boost/type_traits/is_signed.hpp>

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
	tester<other_tag>::type       test( ... );
	
  // 文字列から変換
	template< typename T > 
	struct cannot_parse_to {};
	
	template< typename Ch >
	struct parse_function
	{
		const Ch* ptr;
		int base;
		Ch** endpos;

		template< typename T > T operator()( long_tag ) 
		 { return str::to_long( ptr, base, endpos ); }

		template< typename T > T operator()( ulong_tag ) 
		 { return str::to_ulong( ptr, base, endpos ); } 

		template< typename T > T operator()( double_tag ) 
		 { return str::to_double( ptr, endpos ); }	
	
#if !defined ODEN_CHIKUWA_NO_FLOAT 
		template< typename T > T operator()( float_tag ) 
		 { return str::to_float( ptr, endpos ); }
#endif
#if !defined ODEN_CHIKUWA_NO_LONGDOUBLE
		template< typename T > T operator()( longdouble_tag ) 
		 { return str::to_longdouble( ptr, endpos ); }	 
#endif
#if !defined ODEN_CHIKUWA_NO_LONGLONG 
		template< typename T > T operator()( longlong_tag ) 
		 { return str::to_longlong( ptr, base, endpos ); }
#endif
#if !defined ODEN_CHIKUWA_NO_ULONGLONG 
		template< typename T > T operator()( ulonglong_tag ) 
		 { return str::to_ulonglong( ptr, base, endpos ); }
#endif
		template< typename T, typename Other >
		T operator()( Other ) 
		 { return cannot_parse_to<T>(); /* make compile error */ }
	
	};
	
	template< typename T, typename Ch >
	T parse_dispatch( const Ch* p, int base, Ch* &errpos )
	{
		parse_function<Ch> functor = { p, base, &errpos };
		return functor<T>( tag<sizeof( test(T()) )>() );
	}
	
  // 文字列へ変換
	template< typename T > 
	struct cannot_make_string_from {};
	
	template< typename Ch, typename T >
	struct make_string_function
	{
		T val;

		template< typename T > bool operator()( long_tag ) 
		 { return str::to_long( ptr, base, endpos ); }

		template< typename T > T impl( ulong_tag ) 
		 { return str::to_ulong( ptr, base, endpos ); } 

		template< typename T > T impl( double_tag ) 
		 { return str::to_double( ptr, endpos ); }	
	
#if !defined ODEN_CHIKUWA_NO_FLOAT 
		template< typename T > T impl( float_tag ) 
		 { return str::to_float( ptr, endpos ); }
#endif
#if !defined ODEN_CHIKUWA_NO_LONGDOUBLE
		template< typename T > T impl( longdouble_tag ) 
		 { return str::to_longdouble( ptr, endpos ); }	 
#endif
#if !defined ODEN_CHIKUWA_NO_LONGLONG 
		template< typename T > T impl( longlong_tag ) 
		 { return str::to_longlong( ptr, base, endpos ); }
#endif
#if !defined ODEN_CHIKUWA_NO_ULONGLONG 
		template< typename T > T impl( ulonglong_tag ) 
		 { return str::to_ulonglong( ptr, base, endpos ); }
#endif
		template< typename T, typename Other >
		T impl( Other ) 
		 { return cannot_stringize_from<T>(); /* make compile error */ }
	
	};  
}

  // 文字列を数値に変換
	template< typename T, typename Str >
	boost::optional<T> parse_num( const Str& s, int base = 10 )
	{
		typename string_traits<Str>::pointer errpos = 0;
		T val = lex_cast_detail::parse_dispatch<T>( string_cstr(s), base, errpos );

		return boost::optional<T>( (errpos && *errpos == 0), val );
	}
	
	template< typename T, typename Str >
	boost::optional<T> parse_hex( const Str& str )
	{
		return parse_num<T>( str, 16 );
	}
	
  // 数値などを文字列に変換
	template< typename Ch, typename T >
	std::basic_string<Ch> to_string( const T& val )
	{
		
		
	}
	
  
  
}}

#endif

