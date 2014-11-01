#pragma once
#ifndef ODEN_CHKW_BU_ACT
#define ODEN_CHKW_BU_ACT

#include <oden/chikuwa/bu/parser/joint.h>

namespace oden { namespace chikuwa
{
namespace bu
{
	
//____________________________________________________________________________________________
//
// 				記 述 オ ブ ジ ェ ク  ト	・ アクション部		
//____________________________________________________________________________________________
//	
//#######################################################
//  アクション部
//
  // アクション・ファイアウォール	
	template< typename D >
	struct action {};
	
  // 端末生成：今すぐ生成
	class terminal
		: public action< terminal >
	{
	public:
		explicit terminal( int id )
			: id_(id)
		{}
	
		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter first, Iter last )const
		{
			return pser.create_terminal( id_, first, last );
		}
		
	private:
		int id_;
	};

  // 端末結合：スタックに追加し後で実行  
	class joint
		: public action< joint >
	{
	public:	
		joint( const joint_def& d = joint_def() )
			: def(d)
		{}
		
		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter itr, Iter )const
		{
			return pser.create_joint( def, itr );
		}
		
		joint_def def;
	};
	
  // push
	class context_open
		: public action< context_open >
	{
	public:
		explicit context_open( int cxt )
			: cxt_(cxt)
		{}

		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter itr, Iter )const
		{
			pser.open_context( cxt_, itr );
			return true;
		}
	private:
		int cxt_;
	};
	
  // pop
	class context_close
		: public action< context_close >
	{
	public:
		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter itr, Iter )const
		{
			return pser.close_context( itr );
		}
	};	

  // なにもしません
	class eps_action
		: public action< eps_action >
	{
	public:
		template< typename Pser, typename Iter >
		bool eval( Pser&, Iter, Iter )const
		{
			return true;
		}
	};
	
  // いずれか一つが成立する結合
	template< typename L, typename R >
	class action_candidates
		: public action< action_candidates<L,R> >
	{
	public:
		action_candidates( const L& l, const R& r )
			: l_(l), r_(r)
		{}

		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter first, Iter last )const
		{
			return l_.eval( pser, first, last ) || r_.eval( pser, first, last );
		}
	private:
		L l_; R r_;
	};	
	
    // 演算子オーバーロード
	template< typename L, typename R >
	action_candidates<L,R> operator/ ( const action<L>& l, const action<R>& r )
	{
		return action_candidates<L,R>( static_cast<const L&>(l), static_cast<const R&>(r) );
	}
	
  // 連続してアクションを実行する
	template< typename L, typename R >
	class action_sequence
		: public action< action_sequence<L,R> >
	{
	public:
		action_sequence( const L& l, const R& r )
			: l_(l), r_(r)
		{}
		
		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter first, Iter last )const
		{
			return l_.eval( pser, first, last ) && r_.eval( pser, first, last );
		}
	private:
		L l_; R r_;
	};

    // 演算子オーバーロード
	template< typename L, typename R >
	action_sequence<L,R> operator>> ( const action<L>& l, const action<R>& r )
	{
		return action_sequence<L,R>( static_cast<const L&>(l), static_cast<const R&>(r) );
	}


  // 失敗してもよい
	template< typename T >
	class action_optional
		: public action< action_optional<T> >
	{
	public:
		explicit action_optional( const T& a )
			: a_(a)
		{}
	
		template< typename Pser, typename Iter >
		bool eval( Pser& pser, Iter itr, Iter eitr )const
		{
			a_.eval( pser, itr, eitr );
			return true;
		}
	
		T a_;
	};
	
    // 演算子オーバーロード
	template< typename T >
	action_optional<T> operator- ( const action<T>& a )
	{
		return action_optional<T>( static_cast<const T&>(a) );
	}

//#######################################################
//  エイリアス
//	
  // ターミナル
	inline terminal term( int id )
	{
		return terminal(id);
	}

  // ジョイント
	inline joint left_right
	 ( int id, int p, joint_assoc_type as = left_assoc )
	{
		return joint( joint_def( id, p, as, join_func_adjucent, join_arg_true, join_arg_true ) );
	}
	
	inline joint left
	 ( int id, int p, joint_assoc_type as = left_assoc )
	{
		return joint( joint_def( id, p, as, join_func_adjucent, join_arg_true, join_arg_false ) );
	}
	
	inline joint right
	 ( int id, int p, joint_assoc_type as = right_assoc )
	{
		return joint( joint_def( id, p, as, join_func_adjucent, join_arg_false, join_arg_true ) );
	}
	
	inline joint interval
	 ( int id, int p = lowest_priority, joint_assoc_type as = right_assoc )
	{
		return joint( joint_def( id, p, as, join_func_interval ) );
	}
	
	template< int N > joint forward
	 ( int id, int p, joint_assoc_type as = right_assoc )
	{
		return joint( joint_def( id, p, as, join_func_fixed, join_arg_forward, N ) );
	}
	
  // コンテクスト操作
	// 開始
	inline context_open push( int cid = temp_context_id )
	{
		return context_open( cid );
	}	
	// 終了
	inline context_close reduce()
	{
		return context_close();
	}

  // スキップに使える
	inline eps_action nothing()
	{
		return eps_action();
	}

} // end of namespace parser
}}
	
#undef BU_RAISE_EVENT

#endif
