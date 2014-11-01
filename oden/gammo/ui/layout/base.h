#pragma once

#ifndef ODEN_GAMMO_UI_LAYOUT_BASE
#define ODEN_GAMMO_UI_LAYOUT_BASE

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
#include <vector>

// --------- win ------------
// --------- boost ----------
#include <boost/scoped_ptr.hpp>
#include <boost/next_prior.hpp>

// --------- oden ----------	
//---------- gammo ----------
#include <oden/gammo/ui/window/window.h>
#include <oden/gammo/common/geometry.h>

//
//####################################################

namespace oden { namespace gammo
{
	class widget_object;
//__________________________________________________________________________________________________________
//
//        レイアウトクラス・共通
//__________________________________________________________________________________________________________
//
//###########################################################
// レイアウトインターフェース
//
	class layout_object
	{
	public:		
	// コンストラクタ
		layout_object(){}
		virtual ~layout_object(){}
	
	public:
	// 仮想関数
	  // 計算を行う
		virtual void recalc( const rect& this_rect ) = 0;
	  // ベストサイズ
		virtual size best_size()const = 0;
	};
	
//	typedef object_ptr<layout_object> layout_object_ptr;

//###########################################################
// サイズ出力インターフェース
//		
	class term_layout
		: public layout_object
	{
	public:
	// コンストラクタ
		term_layout( widget_object* wid )
			: wid_(wid), space_(2)
		{}
	
	// オーバーライド
	  // 計算を行う
		void recalc( const rect& this_rect );
	  // ベストサイズ
		size best_size()const;
		
	// メンバ関数	
	  // スペース
		term_layout& space( int spa ) 
		{ space_ = spa; return *this; }
		int space()const { return space_; }

	private:
		int space_;
		widget_object* wid_;
	};
	
	// ウィジェットをレイアウトでくるむ( 明示的に指定するなら )
	namespace layout
	{
		inline term_layout& term( widget_object* w ){ return *(new term_layout(w)); }
	}

//###########################################################
// レイアウトコンテナクラス
//
  // コンテナ要素
	template< typename T >
	class layout_container_elem_base
	{
	public:
		layout_container_elem_base()
			:value_()
		{}
		layout_container_elem_base( const T& t )
			:value_(t)
		{}
		
		T& rvalue(){ return value_; }
		const T& value()const { return value_; }
	private:
		T value_;
	};
	
	template<>
	class layout_container_elem_base<boost::none_t>
	{};
	
	template< typename T >
	class layout_container_elem
		: public layout_container_elem_base<T>
	{
	public:
		layout_container_elem()
			:p_(0), size_(0), layout_container_elem_base<T>()
		{}
		layout_container_elem( int sz, const T& t )
			:p_(0), size_(sz), layout_container_elem_base<T>(t)
		{}
		
		int size_value()const{ return size_; }
		int& rsize_value(){ return size_; }
		
		layout_object* layout()const{ return p_; }
		
		bool has_layout()const {
			return p_!=0;
		}
		void destroy_layout() { 
			delete p_; p_ = 0; 
		}
		void reset_layout( layout_object* p ) {
			if(p_) 
				destroy_layout();
			p_ = p;
		}
		
	private:
		layout_object* p_;
		int size_;
	};
	
  // コンテナ
	template< typename Elem >
	class layout_container
	{
	public:
		typedef layout_container_elem<Elem> 		elem_type;
		typedef std::vector<elem_type>				value_type;		
		typedef typename value_type::iterator 		iterator;
		typedef typename value_type::const_iterator const_iterator;
	//
		layout_container(){}
		
	// メンバ
		value_type& value(){ return v_; }
		const value_type& value()const { return v_; }
		
		void add( const elem_type& v ){ v_.push_back(v); }
	
		elem_type* last()
		{
			if(!v_.empty()) {
				elem_type& r = *boost::prior(v_.end());
				return &r;
			}
			return 0;
		}
		
		// メモリを解放
		void clear()
		{
			iterator itr = begin(), eitr = end();
			for(; itr!=eitr; ++itr)
				itr->destroy_layout();
		}
		
	  // 
		iterator begin() { return v_.begin(); }
		iterator end() 	{ return v_.end(); }
		const_iterator begin()const { return v_.begin(); }
		const_iterator end()const   { return v_.end(); }

	private:
		value_type v_;
	};
	
	struct single_elem_layout;
	
	template<>
	class layout_container<single_elem_layout>
	{
	public:
		typedef layout_container_elem<boost::none_t> elem_type;
		typedef elem_type value_type;
	
		value_type& value(){ return v_; }
		const value_type& value()const { return v_; }
		
		void add( const elem_type& v )
		{ 
			v_.rsize_value() = v.size_value();
		}
	
		elem_type* last() { return &v_; }
		
		void clear() { v_.destroy_layout(); }
		
	  // 単一限定のメソッド
		void set( layout_object* p ){ v_.reset_layout(p); }
		layout_object* get()const { return v_.has_layout() ? v_.layout() : 0; }
		
	private:		
		value_type v_;
	};
	
//###########################################################
// 子を複数持つレイアウトクラス
//
	template< typename Me, typename Element = boost::none_t >
	class layout_base
		: public layout_object
	{
	private:
		Me& me(){ return *static_cast<Me*>(this); }
		const Me& me()const{ return *static_cast<const Me*>(this); }

	public:
		typedef layout_container<Element>			container;
		typedef typename container::value_type		real_container_type;
		typedef typename container::elem_type		elem_type;

		// コンストラクタ
		layout_base(){}
		virtual ~layout_base()
		{
			clear();
		}

		// メンバ関数
		// レイアウト追加
		Me& elem( int sz, const Element& e = Element() )
		{
			container_.add( elem_type( sz, e ) );
			return me();
		}

		// レイアウトコンテナ・イテレータ
		container& elems() { return container_; }
		const container& elems()const { return container_; }

		// 要素を全て削除し、解放する
		void clear() { container_.clear(); }

		// 最後の子レイアウトを変更する
		void set_last_layout( layout_object* l )
		{
			if( elem_type* p = container_.last() )
				p->reset_layout( l );
		}

		// 演算子
		// []
		Me& operator[]( layout_object& r )
		{
			this->set_last_layout( &r );
			return me();
		}
		Me& operator[]( layout_object* p )
		{
			this->set_last_layout(p);
			return me();
		}
		Me& operator[]( widget_object* w )
		{
			this->set_last_layout(new term_layout(w));
			return me();
		}
		
	private:
		container container_;
	};

//###########################################################
// レイアウトの構文
//
/* 例 */
#if 0
	gammo::layout::hbox().spring()[
		gammo::layout::vbox().block()[ /*値を指定しなければ最適サイズに合わせる*/
			ass->space(1)
		]
		.block(10)
		.spring()
		.block(200)[
			 
		]
	]
	.block(10)[
	
	]
#endif

/*
//###########################################################
// サイズを固定する：固定しない場合は最適サイズに合わせる
//
	class cage_layout
		: public single_layout<cage_layout>
	{
	public:
	// コンストラクタ
		cage_layout( int w = -1, int h = -1, layout_object_ptr p = 0 )
			: dim_(w,h)
		{
			set_layout(p);
		}
		
	// 設定
		cage_layout& size( int w, int h )  { dim_.set(w,h); return *this; }
		cage_layout& soften()              { return this->size(-1,-1); }
		
	// 実装関数
	  // レイアウトを再計算
		void do_recalc( const rect& this_rect, rect& out )
		{
			out.rtopleft() = this_rect.topleft();
			if( dim_.is_valid() )
				out.set_size( dim_ );
			else if( has_layout() )
				out.set_size( layout()->best_size() );
			else
				out.rbotright() = this_rect.botright();
		}
	  // 最小値
		size get_best_size( const size& cdim )const 
		{
			if( dim_.is_valid() )
				return dim_;
			return cdim;
		}
		
	private:
		size dim_;
	};
	
	namespace layout
	{
		inline cage_layout& soft_cage(){ return *(new cage_layout()); }
		inline cage_layout& cage(int w, int h){ return *(new cage_layout(w,h)); }
	}
*/
//###########################################################
//  縦横比を指定
//
	class fill_layout
		: public layout_base< fill_layout, single_elem_layout >
	{
	public:
		fill_layout(){}
		
	// 実装関数
	  // レイアウトを再計算
		void recalc( const rect& this_rect );
	  // 最小値
		size best_size()const;
	};
	
	namespace layout
	{
		inline fill_layout& fill(){ return *(new fill_layout()); }
	}

//###########################################################
//  縦横比を指定
//
	class pad_layout
		: public layout_base< pad_layout, single_elem_layout >
	{
	public:
	// コンストラクタ
		pad_layout()
			: top_(0), right_(0), bottom_(0), left_(0)
		{}
		pad_layout( int tate, int yoko )
			: top_(tate), right_(yoko), bottom_(tate), left_(yoko)
		{}
		pad_layout( int t, int r, int b, int l )
			: top_(t), right_(r), bottom_(b), left_(l)
		{}
		
	// 値を変更
		pad_layout& pad( int value )
		{
			return pad( value, value );
		}
		pad_layout& pad( int t, int y )
		{
			return pad( t, y, t, y );
		}
		pad_layout& pad( int t, int r, int b, int l )
		{
			top_ = t;    right_ = r;
			bottom_ = b; left_ = l;
			return *this;
		}
		
	// 実装関数
	  // レイアウトを再計算
		void recalc( const rect& this_rect );
	  // 最小値
		size best_size()const;
		
	private:
		int top_, right_, bottom_, left_;
	};
	
	namespace layout
	{
		inline pad_layout& pad(int t,int y){ return *(new pad_layout(t,y)); }
		inline pad_layout& pad(int t,int r,int b,int l){ return *(new pad_layout(t,r,b,l)); }
	}

}}

#endif
