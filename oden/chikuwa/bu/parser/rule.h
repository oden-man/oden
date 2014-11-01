#pragma once
#ifndef ODEN_CHKW_BU_RULE
#define ODEN_CHKW_BU_RULE

#include <oden/chikuwa/bu/parser/matcher.h>
#include <oden/chikuwa/bu/parser/action.h>

namespace oden { namespace chikuwa
{
namespace bu
{
//____________________________________________________________________________________________
//
// 				記 述 オ ブ ジ ェ ク  ト	
//____________________________________________________________________________________________
//		
//#######################################################
//  ルール
//	
  // インターフェース
	template< typename Parser >
	class rule_entry
	{
	public:
		typedef typename Parser::token_iterator iter;
	
		virtual ~rule_entry(){}
		
		virtual int  match ( Parser&, iter )const = 0;
		virtual bool eval  ( Parser&, iter, iter )const = 0;
		virtual int  get_flag()const = 0;
	};
	
  // テンプレート
	template< typename Parser, typename Cond, typename Body >
	class rule
		: public rule_entry<Parser>
	{
	public:
		typedef Parser parser_type;
		typedef typename rule_entry<Parser>::token_iter iter;
	
		rule( int f, const Cond& c, const Body& b )
			: flag_(f), cond_(c), body_(b)
		{}
		
	  // マッチ
		int match( Parser& pser, token_iter current )const 
		{
			return cond_.match( pser, current, pser.end() );
		}
		int get_flag()const 
		{ 
			return flag_; 
		}
		
	  // 実行
		bool eval( Parser& pser, token_iter first, token_iter last )const 
		{
			return body_.eval( pser, first, last );
		}
		
	private:
		const int flag_;
		Cond  cond_;
		Body  body_;
	};
		
//#######################################################
//  ルール集合
//	
  // ルールの集合
	template< typename Parser >
	class rule_set
	{
	public:
		typedef rule_entry<Parser>						rule_entry_type;
		typedef object_container<rule_entry_type>		rule_container;
		typedef typename rule_container::element		rule_ptr;
		typedef std::vector<rule_ptr> 					rule_list;
		
	// メンバ関数
	  // マッチするルールを調べて返す
		
	  
	  
	  
		template< typename Iterator >
		int process( Parser& pser, Iterator itr )const
		{
			int last_len = 0, len = 0;
			
			// 当てはまるルールを探す
			rule_list::const_iterator ruitr = rules_.begin(), rueitr = rules_.end(), matched_rule = rueitr;
			for( ; ruitr != rueitr; ++ruitr )
			{
				// マッチするか?
				len = (*ruitr)->match( pser, itr );
				if( len == matcher_error )
					return rule_set_error;
				if( len == 0 )
					continue;
				
				int flag = (*ruitr)->get_flag();
				if( (flag & longest_match  && last_len < len) || 
					(flag & shortest_match && last_len > len) || 
					(flag & first_match) )
				{ 
					matched_rule = ruitr;
					last_len = len;
				}
				
				if( flag & first_match ) 
					break;
			}
			
			if( matched_rule != rueitr ) 
			{
				Iterator mlast = itr;
				std::advance( mlast, last_len );
				
				if( !(*matched_rule)->eval( pser, itr, mlast ) )
					return rule_set_error;
			}
			
			return last_len;
		}
		
	  // ルールの追加
		template< typename C, typename A >
		void add( int flags, const C& cond, const A& act )
		{
			rule_container::push_back( rules_, new rule<Parser, C, A>(flags, cond, act) );
		}
	
	private:
		rule_list rules_;
	};
	
//#######################################################
//  シンタックス生成 [ ] ( )
//	
  // [アクション]
	template< typename Set, typename Matcher >
	class rule_composer
	{
	public:
		typedef rule_composer<Set, Matcher> self;
		
		rule_composer( Set* pset, const Matcher& m, int f )
			: set_(pset), flag_(f), matcher_(m)
		{}
		
		// アクション
		template< typename A >
		void action( const A& act )
		{
			assert( set_!=0 );
			set_->add( flag_, matcher_, act );
		}
		
		template< typename A >
		self& operator=( const A& a )
		{
			this->action(a);
			return *this;
		}		
		
	private:
		Set*      set_;
		int		  flag_;
		Matcher   matcher_;
	};
	
  // ( 条件)
	template< typename Parser >
	class rule_set_expr
	{
		typedef rule_set_expr<Parser> 	self;
		typedef rule_set<Parser>		set_type;
		
		set_type* set_;
		
	  // () の返す型を求める		
		template< typename C >
		struct next_type
		{
			typedef typename C::type matcher;
			typedef rule_composer< set_type, matcher > type;			
		};
		
		template< typename P1 = void, typename P2 = void, typename P3 = void, typename P4 = void >
		struct next
		{};		
		template< typename P1 >
		struct next<P1>
			: next_type< matcher_type<P1> >
		{};		
		template< typename P1, typename P2 >
		struct next<P1, P2>
			: next_type< stack_match_type< P1, stack_match_type<P2> > >
		{};		
		template< typename T1, typename T2 >
		struct next_raw
			: next_type< stack_match_identity< stack_match<T1,T2> > >
		{};
		
	  // rule_set を一つのrule扱いにするラッパー
		struct merge_wrapper
		{			
			set_type* p_;
			
			template< typename Parser, typename Iterator >
			int match( Parser& pser, Iterator itr, Iterator )const
			{
				// matcher_result と rule_set_resultは同じなのでそのまま返せる
				return p_->process( pser, itr ); 
			}
		};
		
	public:
		rule_set_expr( set_type& p )
			: set_(&p)
		{}
		
	// アクセス
		set_type* get()const { return set_; }
		
	// 条件追加	
	  // 内部関数
		template< typename R, typename P1 >
		R match_( int flag, const P1& p1 )
		{
			return R( set_, matcher_type<P1>::construct(p1), flag );
		}
		template< typename R, typename P1, typename P2 >
		R match_( int flag, const P1& p1, const P2& p2 )
		{
			return R( set_, bu::pushed( p2, bu::pushed(p1)), flag );
		}
		template< typename R, typename T1, typename T2 >
		R match_( int flag, const stack_match<T1,T2>& m )
		{
			return R( set_, m, flag );
		}
		
	  // 最速マッチ
		template< typename P1 >
			typename next<P1>::type 
				match( const P1& p1 ) 
		{
			return this->match_< typename next<P1>::type >( first_match, p1 );
		}
		template< typename P1, typename P2 > 
			typename next<P2,P1>::type 
				match( const P1& p1, const P2& p2 )
		{
			return this->match_< typename next<P2,P1>::type >( first_match, p1, p2 );
		}
		template< typename T1, typename T2 > 
			typename next_raw<T1,T2>::type 
				match( const stack_match<T1,T2>& s )
		{
			return this->match_< typename next_raw<T1,T2> >( first_match, s );
		}
		
	  // 最長マッチ
		template< typename P1 >
			typename next<P1>::type 
				match_l( const P1& p1 )
		{
			return this->match_< typename next<P1>::type >( longest_match, p1 );
		}	
		
	// マージ	
	  // ラッパに包んでmatcherに入れる
		void merge( const self& right )
		{
			assert( right.get()!=0 );
			merge_wrapper wrap = { right.get() };
			
			assert( set_!=0 );
			set_->add( first_match, wrap, bu::nothing() );
		}
		
	// 無視する
		template< typename P1 >
		void ignore( const P1& p1 )
		{
			this->match(p1).action(bu::nothing());
		}
		template< typename P1, typename P2 >
		void ignore( const P1& p1, const P2& p2 )
		{
			this->match(p1,p2).action(bu::nothing());
		}
		
	};	
	
//#######################################################
//  ルール集合切り替えを行うアクション
//	
	template< typename Set >
	class rule_set_switcher
		: public action< rule_set_switcher<Set> >
	{
	public:		
		explicit rule_set_switcher( const Set& s )
			: set_(s)
		{}
	
		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter, Iter )const
		{
			return pser.switch_rule( set_ );
		}
	
	private:
		Set set_;
	};
	
	template< typename Pser >
	rule_set_switcher< const rule_set<Pser>* > 
		switch_to( 
			const rule_set_expr<Pser>& e 
		)
	{
		return rule_set_switcher< const rule_set<Pser>* >( e.get() );	
	}
	rule_set_switcher< int > 
		switch_to( int idx )
	{
		return rule_set_switcher< int >( idx );	
	}
		
#if 0
	
	bu::parser< oden::tstring::const_iterator, calc > pser;
	auto rule1 = pser.create_rule();
	auto rule2 = pser.create_rule();
	auto rule3 = pser.create_rule();
	auto rule4 = pser.create_rule();
	auto rule5 = pser.create_rule(); 
	
	rule1.match( '+' ) 
		= pluser / adder;
	rule1.match( '-' ) 
		= minuser / suber;
	rule1.match( '/' ) 
		= divider;
	rule1.match( '(' ) 
		= bu::push();
	rule1.match( '(', ')' ) 
		= bu::reduce();
	rule1.match_l( bu::alnum ) 
		= lit;
		
	rule5.merge( rule1 );
	rule5.merge( rule2 );
		
	pser.run( first, last );
	
	pser.get_result();
	
	
	
	rules rule1 = 	
	('+')[
		
	]
	('-')[
			
	]
	('(')[
		
	]
	('(', ')')[
		
	]
	( eol )[
		
	]
	
	bu::rules<parser_t> rule1;
	
	rule1.match( "if" ) 
		= if_expr >> bu::switch_to( rule5 );
	rule1.match_l( bu::alnum ) 
		= lit;
	rule1.match( 
	
	parser.add_rule(rule1);
	
	if_cond
	( '(' )[
		bu::push() >> bu::set(expr)
	]
	( '(', ')' )[
		bu::reduce() >> bu::set(if_body)
	]
	
	if_body
	( '{' )[
		bu::push() >> bu::set(expr)
	]
	( '{', '}' )[
		bu::reduce() >> bu::
	]
	
	rule2 =
	
	
	bu::parse( itr, eitr, rule1 );
	
#endif



} // end of namespace parser
}}

#endif
