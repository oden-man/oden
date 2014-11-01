#pragma once
#ifndef ODEN_CHKW_BU_JOINT
#define ODEN_CHKW_BU_JOINT

#include <oden/chikuwa/bu/parser/defs.h>

namespace oden { namespace chikuwa 
{ 
namespace bu
{

//#######################################################
//  結合規則の記述
// 	
  //	定義
	class joint_def
	{
	public:
		joint_def( 
			int id = null_def_id, 
			int prio = lowest_priority, joint_assoc_type assoc = left_assoc, 
			joint_function function = join_func_null, int arg1 = 0, int arg2 = 0
		) : id_(id), prio_(prio)
		{
			assoc_ = static_cast<char>( static_cast<int>(assoc) );
		
			func_ = static_cast<char>(function);
			arg1_ = static_cast<char>(arg1);
			arg2_ = static_cast<char>(arg2);
		}
	
		int id()const { return id_; }
	
		int priority()const { return prio_; }
		int assoc()const    { return assoc_; }
		
		int function()const 	 { return func_; }
		int function_arg1()const { return arg1_; }
		int function_arg2()const { return arg2_; }
		
	private:
		int  id_;
		int  prio_;
		char assoc_;
		char func_, arg1_, arg2_;
	};

//#######################################################
//  結合エントリ
// 	
  // 実行用エントリ
	class joint_entry
	{
	public:
	// コンストラクタ
		joint_entry() // 既定のコンストラクタが必要
			: def_(), node_(0)
		{}

		joint_entry( joint_def d, node_index ni, std::size_t place )
			: def_(d), node_(ni), place_(place)
		{}

	// メンバ関数
	  // アクセス
		const joint_def& 	definiton()const { return def_; }
		int					def_id()const    { return def_.id(); }

		std::size_t 		place()const { return place_; }
		node_index			node()const { return node_; }
		
		void set_node( int ni ) { node_ = ni; }
		
	  // ソート用
		bool higher( const joint_entry& r )const
		{
			int lp = def_.priority();
			int rp = r.def_.priority();
		
			if( lp < rp ) {
				return true;
			}
			else if( lp == rp )
			{
				bool left = false;
				if( node_ == r.node_ )
					left = (place_ < r.place_);
				else
					left = (node_ < r.node_);			
			
				if( def_.assoc() == left_assoc )
					return left;
				else
					return !left;
			}
			return false;
		}

	 // アクション実行
		template< typename Index >
		bool get_target( Index cxt_first, Index& first, Index& tail )
		{
			PROFILE_BLOCK( act_invoke );
			
			// 結合範囲を取得
			int fn = def_.function();
			if( fn == join_func_adjucent )
			{
				/* 隣同士を結合 */

				int l = def_.function_arg1();
				int r = def_.function_arg2();

				// 範囲を決定
				if( l == join_arg_true && r == join_arg_true )
				{
					if( node_ == 0 )
						return false;

					first = node_ - 1;
					tail = node_;
				}
				else if( l == join_arg_true )
				{
					if( node_ == 0 )
						return false;

					first = node_ - 1;
					tail = first;
				}
				else if( r == join_arg_true )
				{
					first = node_;
					tail = first;
				}
			}
			else if( fn == join_func_fixed )
			{
				/* 固定数のノードを結合 */

				unsigned int num = def_.function_arg2();
				if( def_.function_arg1() == join_arg_forward )
				{
					if( num == 0 )
						return false;

					first = node_;
					tail = node_ + num - 1;
				}
				else
				{
					if( node_ < num || node_ < 1 )
						return false;

					first = node_ - num;
					tail = node_ - 1;
				}
			}
			else if( fn == join_func_interval )
			{
				if( cxt_first >= node_ || node_ == 0 )
					return false;

				first = cxt_first;
				tail  = node_ - 1;
			}
			else 
			{
				return false;
			}

			return true;
		}

	  // 先読みするノード
		int look_ahead() const
		{
			int fn = def_.function();
			int offs = 0;

			if( fn == join_func_adjucent && def_.function_arg2() == join_arg_true )
				offs = 1;
			else if( fn == join_func_fixed && def_.function_arg1() == join_arg_forward )
				offs = def_.function_arg2();

			return offs;
		}

	  //
		node_index node_ahead() const
		{
			promoted_index_t n = node_ + look_ahead();
			if( n == 0 )
				return 0;
			else if( n == ODEN_CHIKUWA_BU_INDEX_MAX )
				return ODEN_CHIKUWA_BU_INDEX_MAX;

			return static_cast<node_index>(n - 1);
		}
		
	  // 左側に必要なノード
		int look_back() const
		{
			int fn = def_.function();
			int offs = 0;

			if( fn == join_func_adjucent && def_.function_arg1() == join_arg_true )
				offs = 1;
			else if( fn == join_func_fixed && def_.function_arg1() == join_arg_backward )
				offs = def_.function_arg2();

			return offs;
		}
		
	private:	
		joint_def			def_;
		std::size_t			place_; // 文字の位置
		node_index			node_;  // 右側のノード
	};
	
} // end of namespace parser
}}

#endif
