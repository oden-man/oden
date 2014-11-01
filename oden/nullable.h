#pragma once

#ifndef ODEN_NULLABLE
#define ODEN_NULLABLE

//################################################
//
//

// ----------- std --------------
#include <cassert>

// ----------- boost -------------
#include <boost/none.hpp>
#include <boost/utility/swap.hpp>

// ----------- oden --------------
//
//###########################################


namespace oden
{
//#######################################################################
//  空の状態を持ったオブジェクトのラッパ 
//
	template< typename T, typename Traits >
	class nullable
	{
	private:
		typedef nullable< T, Traits > self;
	public:
		typedef T 			value_type;
		typedef T& 		 	reference;
		typedef const T& 	const_reference;
		
		typedef Traits 		traits;
		
	public:
	// 無効値
		static value_type null_value() { return traits::null_value(); }
		
	// コンストラクタ
		nullable( )
			: data_( self::null_value() )
		{}
		nullable( boost::none_t )
			: data_( self::null_value() )
		{}
	#ifndef ODEN_NO_NULLPTR
		nullable( std::nullptr_t )
			: data_( self::null_value() )
		{}
	#endif
		nullable( const_reference p )
			: data_( p )
		{}

	// メンバ関数
	  // 値を取得
		const_reference value()const { return data_; }
		reference value() { return data_; }

	  // nullかどうか確かめる
	  	bool is_null()const { return traits::is_null( value() ); }	
		bool is_valid()const{ return !is_null(); }

	  // value_or
		const_reference value_or( const_reference r )const { return is_valid() ? value() : r; }
		reference value_or( reference r ) { return is_valid() ? value() : r; }
	 	
	// 演算子
	  // unspecified bool idiom
	private:
		typedef void ( self::*unspecified_bool_type )( )const;
		void unspecified_bool_true()const{}
	public:
	  	operator unspecified_bool_type()const { return !is_null() ? &self::unspecified_bool_true : 0; }
	  // !
		bool operator!()const { return is_null(); }
		
	  // =
		nullable& operator=( const value_type& r ) 
		{
			data_ = r;
			return *this;
		}

	// swap
		void swap( nullable& other ) 
		{ boost::swap( data_, other.data_ ); }
		
		friend void swap( nullable& l, nullable& r ) 
		{ l.swap(r); }

	private:
		value_type data_;
	};
	
//#######################################################################
//  定数
//
	// boost::noneのパクリ
	namespace detail 
	{ 
		struct failure_helper {}; 
		struct success_helper {}; 
	}

	typedef int detail::failure_helper::*failure_t;
	typedef int detail::success_helper::*success_t;	
	
	const failure_t failure = ( static_cast<failure_t>(0) ) ;
	const success_t success = ( static_cast<success_t>(0) ) ;
	
//#######################################################################
//  空の状態を持ったオブジェクトのラッパ 
//	
	template< typename T, T succ, typename Traits, typename Initial = success_t >
	class limen
		: public nullable< T, Traits >
	{
	private:
		typedef limen< T, succ, Traits, Initial > 	self;
		typedef nullable< T, Traits > 	base;
	public:		
		typedef Traits 					traits;
		
	// 定数値
		static value_type success_value() { return succ; }
		static value_type failure_value() { return self::null_value(); }
		
		static value_type value( success_t ) { return self::success_value(); }
		static value_type value( failure_t ) { return self::failure_value(); }
	  // 初期値
		static value_type initial_value() { return self::value( Initial() ); }
		
	// コンストラクタ
		limen( )
			: base( self::initial_value() )
		{}
		limen( boost::none_t )
			: base( self::initial_value() )
		{}
		limen( failure_t )
			: base( self::failure_value() )
		{}
		limen( success_t )
			: base( self::success_value() )
		{}
		limen( const_reference p )
			: base( p )
		{}

	// メンバ関数
		using base::value;

	// 演算子		
	  // =
		self& operator=( typename self::const_reference r ) 
		{
			this->value() = r;
			return *this;
		}
		self& operator= ( failure_t ) 
		{
			this->value() = self::failure_value();
			return *this;
		}
		self& operator= ( success_t ) 
		{
			this->value() = self::success_value();
			return *this;
		}

	// swap
		void swap( self& other ) 
		{ this->base::swap(other); }
		
		friend void swap( self& l, self& r ) 
		{ l.swap(r); }
	};
	
//###############################################################################
//  null値の性質・プリセット (trueで無効)
//
  // 無効値が一つある
	template< typename T, T n >
	struct singular_null
	{
		static bool is_null( const T& v ){ return v == n; }	/* is_null :値がnull範囲にあればtrue,なければfalse */
		static T null_value() { return n; }					/* value    :代表となるnull値を返す */
	};

  // カスタマイズ可能
	template< typename T, T n, typename Pred >
	struct custom_null
	{
		static bool is_null( const T& v ){ return Pred()( v ); }
		static T null_value() { return n; }
	};
	
  // 無効値を含め、開いた範囲を無効とする
	template< typename T, T nu >
	struct less_eq_null
	{
		static bool is_null( const T& v ){ return v <= nu; }
		static T null_value() { return nu; }
	};
	template< typename T, T nu >
	struct greater_eq_null
	{
		static bool is_null( const T& v ){ return v >= nu; }
		static T null_value() { return nu; }
	};

  // null範囲を結合
	template< typename T, T n, typename Ln, typename Rn >
	struct null_ice_or
	{
		static bool is_null( const T& v ){ return Ln::is_null(v) || Rn::is_null(v); }
		static T null_value() { return n; }
	};
	template< typename T, T n, typename Ln, typename Rn >
	struct null_ice_and
	{
		static bool is_null( const T& v ){ return Ln::is_null(v) && Rn::is_null(v); }
		static T null_value() { return n; }
	};
	
  // 無効値が2つある
  	template< typename T, T n, T n2 >
  	struct singular_null2
  	{
		static bool is_null( const T& v ){ return v == n || v == n2; }
		static T null_value() { return n; }
	};
	
  // 無効値が3つある
  	template< typename T, T n, T n2, T n3 >
  	struct singular_null3
  	{
		static bool is_null( const T& v ){ return singular_null2<T,n,n2>::is_null(v) || v == n3; }
		static T null_value() { return n; }
	};

//###############################################################################
//  nullableの演算
//
	// 比較演算子
	// ==
	template< typename T, typename Tr >
	bool operator ==( const nullable<T,Tr>& l, const nullable<T,Tr>& r )																	\
	{
		if(l && r)
			return l.value() == r.value();
		else if(!l && !r)
			return true;
		return false;
	}

	template< typename T, typename Tr >
	bool operator ==( const nullable<T,Tr>& l, typename nullable<T,Tr>::const_reference r ) 
	{ return l == nullable<T,Tr>(r); }

	template< typename T, typename Tr >
	bool operator ==( typename nullable<T,Tr>::const_reference l, const nullable<T,Tr>& r ) 
	{ return nullable<T,Tr>(l) == r; }

	template< typename T, typename Tr >
	bool operator ==( const nullable<T,Tr>& l, boost::none_t )
	{ return l == nullable<T,Tr>(boost::none); }

	template< typename T, typename Tr >
	bool operator ==( boost::none_t, const nullable<T,Tr>& r ) 
	{ return nullable<T,Tr>(boost::none) == r; }

	// <
	template< typename T, typename Tr >
	bool operator <( const nullable<T,Tr>& l, const nullable<T,Tr>& r )
	{
		if(l && r)
			return l.value() < r.value();
		else if(!l)
			return true;
		return false;
	}

	template< typename T, typename Tr >
	bool operator <( const nullable<T,Tr>& l, typename nullable<T,Tr>::const_reference r )	
	{ return l < nullable<T,Tr>(r); }

	template< typename T, typename Tr >
	bool operator <( typename nullable<T,Tr>::const_reference l, const nullable<T,Tr>& r )
	{ return nullable<T,Tr>(l) < r; }

	template< typename T, typename Tr >
	bool operator <( const nullable<T,Tr>& l, boost::none_t ) 
	{ return l < nullable<T,Tr>(boost::none); }

	template< typename T, typename Tr >
	bool operator <( boost::none_t, const nullable<T,Tr>& r ) 
	{ return nullable<T,Tr>(boost::none) < r; }

	// !=
	template< typename T, typename Tr >
	bool operator !=( const nullable<T,Tr>& l, const nullable<T,Tr>& r ) 
	{ return !(l==r); }

	template< typename T, typename Tr >
	bool operator !=( const nullable<T,Tr>& l, typename nullable<T,Tr>::const_reference r ) 
	{ return !(l==r); }

	template< typename T, typename Tr >
	bool operator !=( typename nullable<T,Tr>::const_reference l, const nullable<T,Tr>& r ) 
	{ return !(l==r); }

	template< typename T, typename Tr >
	bool operator !=( const nullable<T,Tr>& l, boost::none_t r ) 
	{ return !(l==r); }

	template< typename T, typename Tr >
	bool operator !=( boost::none_t l, const nullable<T,Tr>& r )
	{ return !(l==r); }

	// >
	template< typename T, typename Tr >
	bool operator >( const nullable<T,Tr>& l, const nullable<T,Tr>& r ) 
	{ return r<l; }

	template< typename T, typename Tr >
	bool operator >( const nullable<T,Tr>& l, typename nullable<T,Tr>::const_reference r )
	{ return r<l; }

	template< typename T, typename Tr >
	bool operator >( typename nullable<T,Tr>::const_reference l, const nullable<T,Tr>& r )
	{ return r<l; }

	template< typename T, typename Tr >
	bool operator >( const nullable<T,Tr>& l, boost::none_t r ) 
	{ return r<l; }

	template< typename T, typename Tr >
	bool operator >( boost::none_t l, const nullable<T,Tr>& r )
	{ return r<l; }

	// <=
	template< typename T, typename Tr >
	bool operator <=( const nullable<T,Tr>& l, const nullable<T,Tr>& r ) 
	{ return !(r<l); }

	template< typename T, typename Tr >
	bool operator <=( const nullable<T,Tr>& l, typename nullable<T,Tr>::const_reference r )
	{ return !(r<l); }

	template< typename T, typename Tr >
	bool operator <=( typename nullable<T,Tr>::const_reference l, const nullable<T,Tr>& r )
	{ return !(r<l); }
	
	template< typename T, typename Tr >
	bool operator <=( const nullable<T,Tr>& l, boost::none_t r ) 
	{ return !(r<l); }

	template< typename T, typename Tr >
	bool operator <=( boost::none_t l, const nullable<T,Tr>& r )
	{ return !(r<l); }

	// >=
	template< typename T, typename Tr >
	bool operator >=( const nullable<T,Tr>& l, const nullable<T,Tr>& r ) 
	{ return !(l<r); }

	template< typename T, typename Tr >
	bool operator >=( const nullable<T,Tr>& l, typename nullable<T,Tr>::const_reference r )
	{ return !(l<r); }

	template< typename T, typename Tr >
	bool operator >=( typename nullable<T,Tr>::const_reference l, const nullable<T,Tr>& r )
	{ return !(l<r); }
	
	template< typename T, typename Tr >
	bool operator >=( const nullable<T,Tr>& l, boost::none_t r ) 
	{ return !(l<r); }

	template< typename T, typename Tr >
	bool operator >=( boost::none_t l, const nullable<T,Tr>& r )
	{ return !(l<r); }

	// 数学的演算子
	#define nullable_NUMERIC_OP2( symbol ) \
		template< typename T, typename Tr >									\
		nullable<T,Tr> operator symbol(										\
			const nullable<T,Tr>& l,										\
			const nullable<T,Tr>& r											\
		)																	\
		{																	\
			if( l && r )													\
				return l.value() symbol r.value();							\
			else if( l )													\
				return r;													\
			else if( r )													\
				return l;													\
			return l;														\
		}																	\
																			\
		template< typename T, typename Tr >									\
		nullable<T,Tr> operator symbol(										\
			const nullable<T,Tr>& l,										\
			typename nullable<T,Tr>::const_reference r						\
		)																	\
		{																	\
			return l ? l.value() symbol r : l;								\
		}																	\
																			\
		template< typename T, typename Tr >									\
		nullable<T,Tr> operator symbol(										\
			typename nullable<T,Tr>::const_reference l,						\
			const nullable<T,Tr>& r											\
		)																	\
		{																	\
			return r ? l symbol r.value() : r;								\
		}
	
	nullable_NUMERIC_OP2(+)
	nullable_NUMERIC_OP2(-)
	nullable_NUMERIC_OP2(*)
	nullable_NUMERIC_OP2(/)
	nullable_NUMERIC_OP2(%)
	nullable_NUMERIC_OP2(&)
	nullable_NUMERIC_OP2(|)
	nullable_NUMERIC_OP2(^)
	nullable_NUMERIC_OP2(<<)
	nullable_NUMERIC_OP2(>>)
	#undef nullable_NUMERIC_OP2

	// その他の演算子
	template< typename T, typename Tr >
	nullable<T,Tr> operator++( nullable<T,Tr>& l )
	{
		return l ? ++l.value() : l;
	}
	template< typename T, typename Tr >
	nullable<T,Tr> operator--( nullable<T,Tr>& l )
	{
		return l ? --l.value() : l;
	}
	template< typename T, typename Tr >
	nullable<T,Tr> operator++( nullable<T,Tr>& l, int )
	{
		return l ? l.value()++ : l;
	}
	template< typename T, typename Tr >
	nullable<T,Tr> operator--( nullable<T,Tr>& l, int )
	{
		return l ? l.value()-- : l;
	}

	template< typename T, typename Tr >
	nullable<T,Tr> operator~( const nullable<T,Tr>& l )
	{
		return l ? ~l.value() : l;
	}

}



#endif