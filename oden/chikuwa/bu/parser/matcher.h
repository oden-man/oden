#pragma once
#ifndef ODEN_CHKW_BU_MATCHER
#define ODEN_CHKW_BU_MATCHER

#include <cctype>
#include <string>

#include <oden/chikuwa/bu/parser/defs.h>

namespace oden { namespace chikuwa
{
namespace bu
{

//____________________________________________________________________________________________
//
// 				記 述 オ ブ ジ ェ ク  ト	・ マッチ部		
//____________________________________________________________________________________________
//
  // ファイアウォール
	template< typename D >
	struct matcher
	{};
	
//#######################################################
//  文字列のマッチ
//
	template< typename Src, typename ValType = typename Src::value_type >
	class string_match
		: public matcher< string_match<Src,ValType> >
	{
	public:
		typedef ValType value_type;
	
		string_match( const Src& src )
			: source_(src)
		{}

		template< typename Pser, typename Iter >
		int match( Pser&, Iter itr, Iter eitr )const
		{
			int cnt = 0;
			for( ;; ++cnt )
			{ 
				value_type src = source_[cnt];

				if( src == (value_type)0 )
					break;
				else if( itr == eitr )
					return 0;
			
				if( src != *itr ) 
					return 0;				
				
				++itr;
			}
			return cnt;			
		}
	private:
		Src source_;
	};
	
	typedef string_match<std::string> 				str_match;
	typedef string_match<std::wstring> 				wstr_match;
	typedef string_match<const char*, char> 		lit_match;
	typedef string_match<const wchar_t*, wchar_t> 	wlit_match;

	inline str_match str( const std::string& s )
	{
		return str_match( s );
	}
	inline wstr_match str( const std::wstring& s )
	{
		return wstr_match( s );
	}
	inline lit_match cstr( const char* s )
	{
		return lit_match( s );
	}
	inline wlit_match cstr( const wchar_t* s )
	{
		return wlit_match( s );
	}

//#######################################################
//  文字分類のマッチ
//
namespace detail
{
  // charTをunsignedな型へキャストする
	template< typename T >
	struct char_to_code
	{
		template< typename Src >
		struct ret
		{};
		template<>
		struct ret<char> { typedef unsigned char type; };
		template<>
		struct ret<wchar_t> { typedef wchar_t type; }; // VC++ではunsigned

		typedef typename ret<T>::type result_type;

		static result_type cast( T ch ){ return ch; }
	};

  // 一文字ごとに対するプレディケート
	template< typename Pred >
	struct char_predicate_holder
	{
		char_predicate_holder( const Pred& pr )
			: pr_( pr )
		{}

		template< typename Ch >
		bool operator()( Ch ch )const { return pr_( char_to_code<Ch>::cast(ch) ); }

		Pred pr_;
	};
	
	template< typename D >
	struct char_predicate
	{};	
}

  // 1文字を判定
	template< typename Pred > // bool(int)
	class single_char_match
		: public matcher< single_char_match<Pred> >
	{
	public:
		single_char_match( const Pred& pr = Pred() )
			:pred_(pr)
		{}

		template< typename Pser, typename Iter >
		int match( Pser&, Iter itr, Iter )const
		{
			return pred_( *itr ) ? 1 : 0;
		}

	private:
		detail::char_predicate_holder<Pred> pred_;
	};
	
  // 複数文字を判定
	template< typename Pred > // bool(int)
	class serial_char_match
		: public matcher< serial_char_match<Pred> >
	{
	public:
		serial_char_match( const Pred& pr = Pred() )
			:pred_(pr)
		{}

		template< typename Pser, typename Iter >
		int match( Pser&, Iter itr, Iter eitr )const
		{
			int cnt = 0;
			for( ; itr != eitr; ++itr )
			{
				if( !pred_( *itr ) )
					break;
				++cnt;
			}

			return cnt;
		}

	private:
		detail::char_predicate_holder<Pred> pred_;
	};

  // 述語関数  
	// ある一文字
	template< typename CharT >
	struct is_same_char
		: detail::char_predicate< is_same_char<CharT> >
	{
		is_same_char( CharT ch )
			: ch_(ch)
		{}

		bool operator()( int ch )const { return ch == detail::char_to_code<CharT>::cast(ch_); }

		CharT ch_;
	};
	
	template< typename CharT >
	is_same_char<CharT> is( CharT ch )
	{
		return is_same_char<CharT>(ch);
	}

	typedef single_char_match< is_same_char<char> >    char_match;
	typedef single_char_match< is_same_char<wchar_t> > wchar_match;

	inline char_match ch( char c )
	{
		return char_match( is(c) );
	}
	inline wchar_match ch( wchar_t c )
	{
		return wchar_match( is(c) );
	}
	
	template< typename Pred >
	single_char_match< Pred > ch( const Pred& pred )
	{
		return single_char_match< Pred >( pred );
	}
	template< typename Pred >
	serial_char_match< Pred > chars( const Pred& pred )
	{
		return serial_char_match< Pred >( pred );
	}
	
	// isxxx関数のラッパ
	#define BU_STD_CHAR_CLASS( name, func ) \
	struct is_##name														\
		: detail::char_predicate< is_##name >								\
	{																		\
		bool operator()( int ch )const { return std::##func( ch ) != 0; }	\
	};																		\
	typedef serial_char_match<is_##name> name##_match;						\
																			\
	inline name##_match														\
		name (){ return name##_match(); }									
	
	BU_STD_CHAR_CLASS( alpha, isalpha );
	BU_STD_CHAR_CLASS( digit, isdigit );
	BU_STD_CHAR_CLASS( alnum, isalnum );
	
	// or結合
	template< typename L, typename R >
	struct is_char_or
		: detail::char_predicate< is_char_or<L,R> >
	{
		is_char_or( const L& l, const R& r )
			: l_(l), r_(r)
		{}
	
		bool operator()( int ch )const { return l_(ch) || r_(ch); }		
		L l_; R r_;
	};
	
	template< typename L, typename R >
	is_char_or<L,R> operator | ( const detail::char_predicate<L>& l, const detail::char_predicate<R>& r )
	{
		return is_char_or<L,R>( static_cast<const L&>(l), static_cast<const R&>(r) );	
	}	

//#######################################################
//  その他のマッチ
//
  // 常にマッチ
	class eps_match
		: public matcher< eps_match >
	{
	public:		
		eps_match( int c = 1 )
			: c_(c)
		{}
		
		template< typename Pser, typename Iter >
		int match( Pser&, Iter, Iter )const
		{
			return c_;
		}
	private:
		int c_;
	};	
	
  // コンテキストIDを比べつつマッチ
	template< typename C = eps_match >
	class context_match
		: public matcher< context_match<C> >
	{
	public:		
		context_match( int cid, const C& c = C() )
			: cid_(cid), cond_(c)
		{}
		
		template< typename Pser, typename Iter >
		int match( Pser& pser, Iter itr, Iter eitr )const
		{	
			return cond_.match( pser, itr, eitr );	
		}
		bool do_context_match( int cid )const
		{
			return cid == cid_;
		}
	private:
		int cid_;
		C   cond_;
	};

	inline context_match<> context( int cid )
	{
		return context_match<>( cid );
	}
	
//#######################################################
//  型から適当なスキャナを選択する
//	
  // deduction
	template< typename P >
	struct deduce_matcher
	{
		typedef P type;
	};
	
	template<>
	struct deduce_matcher<char> { typedef char_match type; };
	
	template<>
	struct deduce_matcher<wchar_t> { typedef wchar_match type; };
	
	template<>
	struct deduce_matcher<const char*> { typedef lit_match type; };
	
	template<>
	struct deduce_matcher<const wchar_t*> { typedef wlit_match type; };
	
	template<int N>
	struct deduce_matcher<const char[N]> { typedef lit_match type; };
	
	template<int N>
	struct deduce_matcher<const wchar_t[N]> { typedef wlit_match type; };

	template<>
	struct deduce_matcher<int> { typedef context_match<> type; };

	// 構築
	template< typename Param, typename matcher >
	struct matcher_type_impl
	{
		static matcher construct( const Param& p )
		{
			return matcher( p );
		}
	};
	template< typename Param >
	struct matcher_type_impl<Param, Param>
	{
		static const Param& construct( const Param& p )
		{
			return p;
		}
	};

	template< typename T >
	struct matcher_type
		: matcher_type_impl<T, typename deduce_matcher<T>::type>
	{
		typedef typename deduce_matcher<T>::type type;
		typedef T parameter;
	};

 //
 // コンテキスト
 //
	template< typename Pa >
		context_match< typename matcher_type<Pa>::type >
			context( int cid, const Pa& cond_param )
	{
		return
		context_match< typename matcher_type<Pa>::type >( 
			cid, 
			matcher_type<Pa>::construct( cond_param ) 
		);
	}
	
//#######################################################
//  マッチ条件のスタック
//
  // スタックをチェック	
	template< typename C, typename Next >
	class stack_match
		: public matcher< stack_match<C,Next> >
	{
	public:
		stack_match( const C& sc, const Next& n )
			: cond_(sc), next_(n)
		{}
		
		template< typename Pser, typename Iter >
		int match( Pser& pser, Iter itr, Iter eitr )const
		{	
			int len = cond_.match( pser, itr, eitr );
			if( len == 0 )
				return 0;
				
			if( !next_.tail_match( pser, eitr, 0 ) )
				return 0;
				
			return len;			
		}
		
		template< typename Pser, typename Iter >
		bool tail_match( Pser& pser, Iter eitr, int ci )const
		{
			typename Pser::context_entry ce;
			if( !pser.get_context( ci, ce ) ) 
				return false;
				
			if( !this->do_context_match( cond_, ce.id ) || 0==cond_.match( pser, ce.first, eitr ) )
				return false;
		
			return next_.tail_match( pser, eitr, ci+1 );			
		}
		
		template< typename X >
		bool do_context_match( const context_match<X>& c, int cid )const
		{
			return c.do_context_match( cid );
		}
		template< typename X >
		bool do_context_match( const matcher<X>&, int )const
		{
			return true;
		}
		
		C    cond_;
		Next next_;
	};	
	
	struct stack_match_nil
	{};
	
	template< typename C >
	class stack_match<C, stack_match_nil>
		: public matcher< stack_match<C, stack_match_nil> >
	{
	public:
		stack_match( const C& sc, stack_match_nil )
			: cond_(sc)
		{}
		
		template< typename Pser, typename Iter >
		int match( Pser& pser, Iter itr, Iter eitr )const
		{
			return cond_.match( pser, itr, eitr );
		}
		
		template< typename Pser, typename Iter >
		bool tail_match( Pser& pser, Iter eitr, int ci )const
		{
			return true;			
		}
		
		C cond_;
	};
	
  // セルを作成
	// mpl.identity
	template< typename T >
	struct stack_match_identity
	{
		typedef T type;
	};

	// インターフェース
	template< typename Param, typename Next = stack_match_identity<stack_match_nil> >
	struct stack_match_type
	{
		typedef stack_match_type<Param, Next>			self;
		typedef matcher_type<Param>						matcherT;
		typedef typename matcherT::type					matcher;
		typedef	typename Next::type						next;
		typedef stack_match< matcher, next >			type;

		static type construct( const Param& pa, const next& n = next() )
		{
			return type( matcherT::construct(pa), n );
		}
	};
	
	template< typename P, typename N >
	typename stack_match_type< P, stack_match_identity<N> >::type pushed( const P& p, const N& n )
	{
		return stack_match_type< P, stack_match_identity<N> >::construct( p, n );
	}	
	template< typename P >
	typename stack_match_type< P >::type pushed( const P& p )
	{
		return stack_match_type< P >::construct( p );
	}
	
} // end of namespace bu
}}

#endif
