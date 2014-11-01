#pragma once
#ifndef ODEN_CHKW_BU_STXTREE
#define ODEN_CHKW_BU_STXTREE

#include <algorithm>

#include <oden/buf_vector.h>

#include <oden/chikuwa/bu/parser/defs.h>
#include <oden/chikuwa/bu/parser/joint.h>
#include <oden/chikuwa/bu/parser/event.h>

namespace oden { namespace chikuwa 
{ 
namespace bu
{	
//#######################################################
//  構文木
// 
/*
	node_index は一つずらす。
	joint_index はそのまま。
*/

  // コンテクスト保持
	struct syntax_tree_context
	{
		node_index  	first;				// 開始ノード位置
		node_index      ahead;				// ジョイントが要求するノード位置
		int 			joint_stack_pos;	// 開始時のスタックトップ位置
		int 			lowest_entry; 		// 最下位のエントリ
	};
		
  // ツリー
	template< typename Result, typename Stacks >
	class syntax_tree
	{
	public:
		typedef syntax_tree<Result,Stacks>					self;
		
		typedef Result										result_data;
		typedef typename result_data::value_type			data_type;
			
		typedef Stacks										stack_types;
			
		typedef joint_entry						 			entry_type;
		typedef typename Stacks::joint_t					entry_container;	
		typedef buf_vector< entry_type, entry_container > 	entry_list;
		
		struct node
		{
			int        sibling_offs;
			data_type  data;
		};
		typedef typename Stacks::node_t						node_container;
		typedef buf_vector< node, node_container >			node_map;
		
	// メンバ関数	
	  // 構文操作		
		// 葉
		template< typename Iterator, typename Event >
		bool add_node( int term_id, Iterator first, Iterator last, Event& event )
		{
			PROFILE_BLOCK( add_terminal );

			// データを計算
			result_->on_term( term_id, first, last );

			// ノードを作成
			node ne = { 0, result_->get() };		

			if( !nodes_.push_back(ne) )
			{
				event.raise( error_stack_overflow, first, term_id );
				return false;
			}
			
			return true;
		}
		
		// 還元
		bool join_node( int term_id, node_index first_hint, node_index tail_hint )
		{
			PROFILE_BLOCK( add_joint );			
			assert( first_hint <= tail_hint && 0 <= first_hint && tail_hint < (unsigned int)nodes_.size() );
			
			node_index head = this->node_siblings_head(first_hint);
			node_index tail = this->node_siblings_tail(tail_hint);

			// 順番にデータを取り出し
			int child_cnt = 0;
			for( unsigned int i = head; i<=tail; ++i )
			{
				data_type& dat = nodes_[i].data;
				
				// 親に登録
				++child_cnt;
				result_->on_node_child( term_id, dat, child_cnt );
				
				// ブロック末尾へ
				i = this->node_siblings_tail(i);
			}
			
			// 最後の処理があればここで
			result_->on_node( term_id );

			// データを最も左のノードに保存
			nodes_[head].data = result_->get();
			
			// ノードを結合
			promoted_index_t sibling_dist = tail - head; 
			if( sibling_dist > 0 )
			{
				nodes_[head].sibling_offs = sibling_dist;
				nodes_[tail].sibling_offs = -static_cast<int>( sibling_dist );
			}
			
			return true;
		}

	  // ノード操作		
		// ノードが存在するか
		bool node_exists( node_index i )const
		{
			if( (unsigned int)nodes_.size() <= i )
				return false;
			return true;		
		}
		
	    // 連続ブロックの末尾を取得
		node_index node_siblings_tail( node_index i )const
		{
			assert( this->node_exists(i) );
			
			if( nodes_[i].sibling_offs <= 0 )
				return i;
			
			return static_cast<node_index>( i + nodes_[i].sibling_offs );
		}
		
	    // 連続ブロックの先頭を取得
		node_index node_siblings_head( node_index i )const
		{
			assert( this->node_exists(i) );
			
			if( nodes_[i].sibling_offs >= 0 )
				return i;
			
			return static_cast<node_index>( i + nodes_[i].sibling_offs );
		}
		
	  // ノードの状態を文字列化
		template< typename Ch >
		std::basic_string<Ch> debug_display( const Ch (&chars)[4] )const
		{
			std::basic_string<Ch> disp;			
			return disp;
		}
		
	  // 構築中のツリーが右側に終端を要求しているか
		bool require_right_term()const
		{
			if( nodes_.empty() ) 
				return true;   // 終端がない

			if( entries_.empty() )
				return false;  // 終端のみがある
				
			const entry_type& ent = entries_.back();
			promoted_index_t last_node = static_cast<promoted_index_t>(nodes_.size() - 1);

			if( ent.node() <= last_node )
				return false;  // 直前が終端

			if( ent.node_ahead() > last_node )
				return true;   // ノードが足りない
			
			if( cxt_.ahead > last_node )
				return true;   // ノードがまだ足りない
			
			return false;			
		}
		
	  // エントリの追加
		template< typename Event >
		bool add_join_entry( joint_def def, std::size_t place, Event& event )
		{
			// エントリ作成
			promoted_index_t ni = nodes_.size();  // 右側ノードのインデックスを採る
			entry_type ent( def, static_cast<node_index>(ni), place ); // higherのためにここでnode indexを一度設定する
			
			// インデックスオーバーフローを調べる
			if( ni >= ODEN_CHIKUWA_BU_INDEX_MAX )
			{
				event.raise( error_index_overflow, ent );
				return false;
			}
			
			// 左側のopen/closed を確認
			int left_num = ent.look_back();
			if( ni - left_num < 0 || this->require_right_term() == (left_num!=0) )
			{
				//! スキップ
				event.raise( event_join_skip, ent );
				return false;
			}
			
			// このエントリが最も低い優先順位なら、前方のエントリを還元
			if( cxt_.lowest_entry == -1 )
			{
				cxt_.lowest_entry = entries_.size();
			}
			else
			{
				const entry_type& lowest_entry = entries_[cxt_.lowest_entry];

				if( !ent.higher( lowest_entry ) )
				{
					if( cxt_.ahead <= ni ) // 前方のノード数が足りてるか
					{
						event.raise( event_join_reduce, ent );

						if( !this->invoke_join_entry( event ) )
							return false;
					}

					// スペース開放後のノードインデックスを再設定
					ni = nodes_.size();
					ent.set_node( static_cast<node_index>(ni) );

					cxt_.lowest_entry = entries_.size();
				}
			}
			
			// 追加
			if( !entries_.push_back( ent ) )
			{
				event.raise( error_stack_overflow, ent );
				return false;
			}
			
			// 必要ノード数の更新
			node_index ahead_node = ent.node_ahead();
			if( cxt_.ahead < ahead_node ) {
				cxt_.ahead = ahead_node;
			}

			return true;
		}
		
	  // エントリの実行
		template< typename Event >
		bool invoke_join_entry( Event& event )
		{			
			entry_list::iterator jitr  = entries_.begin() + ( cxt_.joint_stack_pos + 1 );
			entry_list::iterator jeitr = entries_.end();
			
			if( jitr == jeitr ) { // エントリが無かった
				return true;
			}
			
			// コンテクスト範囲のエントリを優先順位でソート
			std::sort( jitr, jeitr, []( const entry_type& l, const entry_type& r ){ return l.higher(r); } );
			
			// その部分のみ結合
			for(; jitr != jeitr; ++jitr )
			{
				// 結合範囲を取得
				promoted_index_t first = 0, tail = 0;
				if( !jitr->get_target( cxt_.first, first, tail ) ) 
					break;

				// 存在するノードか?
				assert( first <= tail );
				if( static_cast<promoted_index_t>(nodes_.size()) <= tail )
					break;

				// ノードを結合
				if( !this->join_node( jitr->def_id(), static_cast<node_index>(first), static_cast<node_index>(tail) ) )
					break;
			}

			// 失敗チェック
			if( jitr != jeitr )
			{
				//! エラー : エントリ内容がおかしい				
				event.raise( error_invalid_joint, *jitr );
				return false;
			}

			/* 使わなくなった領域を開放する */
			
			// ノード
			this->cleanup_nodes();
			
			// エントリ
			entries_.set_back( cxt_.joint_stack_pos );
			
			return true;
		}
		
	  // ノードをまとめて領域を再利用
		void cleanup_nodes()
		{
			if( nodes_.empty() )
				return;

			node_index       first = cxt_.first;
			promoted_index_t last  = nodes_.size();
			promoted_index_t back  = first;
			for( promoted_index_t i = first; i < last; ++i )
			{
				node_index ta = this->node_siblings_tail(i);
				
				if( ta == i )
					continue;
				
				if( i != first ) 
				{
					back += 1;					
					nodes_[back].data = nodes_[i].data;
				}
				
				nodes_[back].sibling_offs = 0;
				
				i = ta;
			}
			
			nodes_.set_back( back );
		}
		
	  // コンテクスト
		// 保存
		syntax_tree_context new_context()
		{
			syntax_tree_context ocxt = cxt_;
			syntax_tree_context ncxt = { nodes_.size(), nodes_.size(), entries_.size() - 1, -1 };

			cxt_ = ncxt;

			return ocxt;
		}
		
		// 状態を復帰
		bool restore_context( const syntax_tree_context& oldcxt )
		{			
			cxt_ = oldcxt;	
			return true;
		}

		// 取得
		node_index context_first_node()const { return cxt_.first; }
		
	  // クリア
		void clear()
		{
			nodes_.clear();	
			entries_.clear();
			
			syntax_tree_context nullcxt = {0};
			cxt_ = nullcxt;
		}
		
	// コンストラクタ
		syntax_tree( result_data& ru )
			: result_(&ru)
		{}

	private:	
		node_map	 			nodes_;
		entry_list	 			entries_;
		
		syntax_tree_context		cxt_;
		
		result_data*			result_;
	};

} // end of namespace parser
}}

#endif
