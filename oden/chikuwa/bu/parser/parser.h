#pragma once
#ifndef ODEN_CHKW_BU_PSER_PSER
#define ODEN_CHKW_BU_PSER_PSER

#include <oden/object/container.h>
#include <oden/buf_vector.h>

#include <algorithm>

#include <oden/chikuwa/bu/parser/defs.h>
#include <oden/chikuwa/bu/parser/event.h>
#include <oden/chikuwa/bu/parser/syntax_tree.h>
#include <oden/chikuwa/bu/parser/rule.h>

namespace oden { namespace chikuwa 
{ 
namespace bu
{
//____________________________________________________________________________________________
//
// 				本  体	
//____________________________________________________________________________________________
//	
//###############################################################################
//  スタック使用コンテナ
//	
	template< typename N, typename J, typename C >
	struct parser_stackT
	{
		template< int n > struct context
		: parser_stackT< N,J, buf_use_array<n> > {};
	
		template< int n > struct node
		: parser_stackT< buf_use_array<n>, J,C > {};
		
		template< int n > struct joint
		: parser_stackT< N, buf_use_array<n>, C > {}; 
		
		template< int n, int m = n > struct tree
		: parser_stackT< buf_use_array<n>, buf_use_array<m>, C > {};
		
		typedef N node_t;
		typedef J joint_t;
		typedef C context_t;
	};
	
  // デフォルトはすべて動的
	typedef parser_stackT< buf_use_vector, buf_use_vector, buf_use_vector >
	parser_stack;

//###############################################################################
// イテレータ移動クラス
//	
	template< typename Iterator >
	class parser_range
	{
	public:
		parser_range( Iterator beg = Iterator(), Iterator end = Iterator() )
			: beg_(beg), end_(end), cur_(beg), cur_pos_(0)
		{}

	// メンバ関数
	  // 進む
		void advance( int num )
		{
			std::advance( cur_, num );
			cur_pos_ += num;
		}

	  // チェック
		bool empty()const{ return cur_ >= end_; }

	  // 返り値チェックつき
		bool advance_processed( int ret )
		{
			if( ret == rule_set_error )
				return false;
			else if( ret == 0 )
				ret = 1;

			this->advance( ret );
			return true;
		}

	// アクセサ
		Iterator begin()const	{ return beg_; }
		Iterator end()const		{ return end_; }
		Iterator cur()const		{ return cur_; }
		int		 cur_pos()const { return cur_pos_;  }

	private:
		Iterator beg_, end_;
		Iterator cur_;
		int		 cur_pos_;
	};

//###############################################################################
//  デフォルトのスキャナ : 現在のルールを呼び出し実行するだけ
//
	struct default_scanner
	{
		template< typename Parser, typename Range >
		bool scan( Parser& pser, Range& rng )const
		{
			while( !rng.empty() )
			{
				typename Parser::rule_set_type const* pRule = pser.current_rule();
				if( !pRule )
					return false;

				if( !rng.advance_processed( pRule->process( pser, rng.cur() ) ) )
					return false;
			}

			return true;
		}
	};

//###############################################################################
//  パーサ本体
//	
	template< typename Iterator, typename Result, typename Stacks = parser_stack >
	class parser
	{
	public:
		typedef parser 									self;
		typedef Iterator								token_iterator;
		typedef Result									result_data;
		typedef Stacks									stack_types;	
	
		typedef parser_event< self >					event_type;
		typedef syntax_tree< Result, stack_types >		tree_type;

		typedef parser_range<token_iterator>			target_range;
	
		struct context_entry 
		{ 
			int 			id;				  // コンテクスト識別子
			token_iterator 	first;			  // 開始場所
			syntax_tree_context tree_cxt;	  // ツリーのコンテキスト
		};
		typedef typename stack_types::context_t			context_container;
		typedef buf_vector< context_entry, 
							context_container >			context_stack;	

		typedef rule_set<self>							rule_set_type;
		typedef object_container<rule_set_type>			rule_set_container;
		typedef typename rule_set_container::element	rule_set_ptr;
		typedef std::vector<rule_set_ptr>				rule_set_list;

		typedef buf_vector< rule_set_type* >			rule_set_srack;

	// メンバ関数
	  // 終端とアクション
		// 追加
		bool create_terminal( int tid, token_iterator first, token_iterator last )
		{
			if( !tree_.add_node( tid, first, last, event_ ) )
				return false;
			
			event_.raise( event_term_success, first, tid );
			return true;
		}

		// 追加
		bool create_joint( joint_def def, token_iterator itr )
		{
			if( !tree_.add_join_entry( def, target_.cur_pos(), event_ ) )
				return false;
			
			event_.raise( event_join_expect, itr, def.id() );
			return true;
		}

	  // コンテキスト操作
		// 開始
		bool open_context( int n, token_iterator itr )
		{			
			context_entry dat = { n, itr, tree_.new_context() };
			
			if( !cxt_stack_.push_back(dat) )
			{
				event_.raise( error_stack_overflow, itr );
				return false;
			}
	
			event_.raise( event_context_begin, itr );
			return true;
		}

		// 還元
		bool close_context( token_iterator itr )
		{
			event_.raise( event_join_reduce, itr );

			// 残っているエントリを実行させる
			if( !tree_.invoke_join_entry( event_ ) )
				return false;
				
			// 検証
			
			event_.raise( event_context_end, itr );

			// 前回のコンテクストへ復帰する
			if( !tree_.restore_context( this->context().tree_cxt ) )
				return false;

			// コンテクスト離脱
			cxt_stack_.pop_back();

			return true;
		}
		
		// コンテクストにアクセス
		context_entry& context() { return cxt_stack_.back(); }
		const context_entry& context()const { return cxt_stack_.back(); }

		bool get_context( int at, context_entry& out )const
		{
			if( at < 0 || cxt_stack_.size() <= at )
				return false;

			out = cxt_stack_.back(at);
			return true;
		}
		
	  // アクセサ
		// ツリー
		tree_type& tree() { return tree_; }
		const tree_type& tree()const { return tree_; }
		
		// 計算オブジェクト
		const result_data& result() { return result_; }
		
		// 結果
		const typename result_data::value_type& get_result()const { return result_.get(); }
		
		// 開始・終了位置
		token_iterator begin()const { return target_.begin(); }
		token_iterator end()const   { return target_.end(); }
		const target_range& target()const { return target_; }

	  // イベント
		// すべてのイベントを報告する
		void report_all_event()
		{
			event_level_ = event_level_all;
		}
		void report_only_error()
		{
			event_level_ = event_level_error;
		}
		// 記録レベルをテスト
		bool test_event_level( event_code c )const
		{
			return event_level_ < c;
		}
		// イベントハンドラ実行
		void handle_event( event_type& eve )
		{
			result_.on_event( eve );
		}
		
	  // ルール
		// 切り替え
		bool switch_rule( const rule_set_type* p )
		{
			cur_rule_ = p;

			event_.raise( event_rule_switched, target_.cur() );
			return true;
		}
		bool switch_rule( int idx )
		{
			if( idx >= rule_libs_.size() ) {
				return false;
			}
			
			this->switch_rule( rule_libs_[idx].get() );
			return true;
		}
		
		// 取得
		const rule_set_type* current_rule()const { return cur_rule_; }

		// 新規作成 + ルール集合ライブラリに追加 
		rule_set_expr<self> create_rule()
		{
			rule_set_type* pNew = new rule_set_type();			
			rule_set_container::push_back( rule_libs_, pNew );
			
			return rule_set_expr<self>( *pNew );
		}

	  // パーサ起動		
		// 実装
		template< typename Iterator, typename Scanner >
		bool run( Iterator sfirst, Iterator slast, const Scanner& scanner )
		{
			target_ = target_range( sfirst, slast );
			
			// チェック
			if( target_.empty() )
				return true;
				
			if( rule_libs_.empty() )
				return true;
		
			// クリア
			result_.clear();
			tree_.clear();
			cxt_stack_.clear();
			
			// 最初のルール選択
			if( cur_rule_ == 0 ) 
				cur_rule_ = rule_libs_.front().get();
		
			// コンテクスト開始
			this->open_context( default_context_id, target_.begin() );

			// スキャン
			if( !scanner.scan( *this, target_ ) )
				return false;

			// スキャン終了後
			token_iterator itr = target_.end();
			--itr;
			
			// 残ったコンテクストを全て閉じる
			if( cxt_stack_.empty() )
			{
				event_.raise( error_main_context_closed, itr );
				return false;			
			}
			while( cxt_stack_.size() > 1 )
			{
				event_.raise( error_context_not_closed, itr );
				
				if( !this->close_context( itr ) )
					return false;
			}
			
			// メインコンテクスト
			if( !this->close_context( itr ) )
				return false;			

			return true;
		}
		
		// デフォルトスキャナを使用
		template< typename Iterator >
		bool run( Iterator itr, Iterator eitr )
		{
			default_scanner sa;
			return this->run( itr, eitr, sa );
		}
		
	  // コンストラクタ
		parser( const Result& res = Result() )
			: result_(res), tree_(result_), event_(this), event_level_(event_level_error), cur_rule_(0)
		{}
		
	private:
		result_data		result_;

		tree_type		tree_;	
		context_stack   cxt_stack_;
		
		target_range	target_;

		event_type		event_;
		int				event_level_;

		rule_set_list			rule_libs_;
		const rule_set_type*	cur_rule_;
	}; 
	
//#######################################################
//  セマンティックアクション
//	
	template< typename T >
	class semantic_action
	{
	public:
		typedef T value_type;
		
		semantic_action()
			: val_()
		{}
		
	  // ツリー構築
		const T& val()const { return val_; }
		T& val() { return val_; }	
		
		void clear(){ val_ = T(); }
		
		const T& get()const { return val_; }

	  // エラーハンドラ
		template< typename E >
		void on_event( E& ){}

	private:
		T val_;
	};

} // end of namespace parser
}}
	
#undef BU_RAISE_EVENT

#endif
