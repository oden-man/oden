#pragma once
#ifndef ODEN_CHKW_BU_NODE
#define ODEN_CHKW_BU_NODE

#include <vector>
#include <string>
#include <algorithm>
#include <ostream>
#include <oden/object/linked_ptr.h>

namespace oden { namespace chikuwa
{
namespace bu
{
//#######################################################
//  構文ツリー
//	
	class node_impl
	
	func( context, a1, a2,... );
	
	class value_node
	class string_node
	
	class unary_operation_node
	class binary_operation_node
	class nary_operation_node
	
	数値ID -> 処理, 型, 結合
	
	bu::node_factory fac;
	bu::terminal lit   = fac.terminal ( node_value_string );
	bu::joint    adder = fac.joint    ( bu::left_right(6,2), node_value_int, std::bind(...) );
	
//#######################################################
//  超簡易的な万能型
//	
  // 識別
	enum node_value_type
	{
		node_value_bool,
		node_value_int,
		node_value_double,
		
		node_value_string,
		
		node_value_null
	};

//#######################################################
//  構文ツリー
//	
	class node
	{
	public:
		typedef node<DataT>			self;	
		
		typedef boost::any			data_type;
		typedef self*				pointer_type;
		
		virtual ~node(){}
		
		const data_type& raw_data()const { return data_; }
	};
	
	class node_parent
		: public node
	{
	public:
	
	private:
		std::vector<smart_ptr> child_; 
	};
		
		virtual int id()

		int id()const { return id_; }
	// コンストラクタ
		explicit node( int id )
			: id_(id), content_()
		{}
		template< typename Iter >
		node( int id, Iter f, Iter l )
			: id_(id), content_(f,l)
		{}

	// メンバ関数
	  // 子を追加
		void add( const smart_ptr& p ){ child_.push_back(p); }
	  // データ
		const string_type& content()const { return content_; }
		
	  // ストリームに表示
		struct shower 
		{ 
			std::ostream* pos; int depth; 
			template< typename X > void operator()( const X& chi )const { chi->show(*pos, depth); } 
		};
		void show( std::ostream& os, int dep = 0 )const
		{			
			for( int i = 0; i<dep-1; ++i ){	os << "  "; }
			
			if( dep > 0 ) 
				os << "└";

			os << "[" << id_;
			if( !content_.empty() )
				os << " value:" << content_;
			os << "]" << std::endl;

			shower swr = { &os, dep+1 };
			std::for_each( child_.begin(), child_.end(), swr );
		}
		
	private:
		int id_;
	};
	
//#######################################################
//  構文ツリー
//	
	template< typename CharT >
	class node
	{
	public:
		typedef node<CharT>						self;	
		typedef CharT 							data_type;
		typedef oden::linked_ptr< self >		smart_ptr;

	// コンストラクタ
		explicit node( int id )
			: id_(id), content_()
		{}
		template< typename Iter >
		node( int id, Iter f, Iter l )
			: id_(id), content_(f,l)
		{}

	// メンバ関数
	  // 子を追加
		void add( const smart_ptr& p ){ child_.push_back(p); }
	  // データ
		int id()const { return id_; }
		const string_type& content()const { return content_; }
		
	  // ストリームに表示
		struct shower 
		{ 
			std::ostream* pos; int depth; 
			template< typename X > void operator()( const X& chi )const { chi->show(*pos, depth); } 
		};
		void show( std::ostream& os, int dep = 0 )const
		{			
			for( int i = 0; i<dep-1; ++i ){	os << "  "; }
			
			if( dep > 0 ) 
				os << "└";

			os << "[" << id_;
			if( !content_.empty() )
				os << " value:" << content_;
			os << "]" << std::endl;

			shower swr = { &os, dep+1 };
			std::for_each( child_.begin(), child_.end(), swr );
		}
		
	private:
		int id_;
		string_type content_;
		std::vector<smart_ptr> child_; 
	};

	typedef node<char> 		cnode;
	typedef node<wchar_t> 	wnode;
	
} // end of namespace bu
}}

#endif
