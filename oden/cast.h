#pragma once

#ifndef ODEN_CAST
#define ODEN_CAST

//################################################
//   インクルード
//

// ------------ boost -----------------
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

//
//################################################



namespace oden
{
//________________________________________________________________________________________________
//
//
//  キャスト
//
//________________________________________________________________________________________________
//
//##########################################################################################
// static_cast とreinterpret_cast を使い分ける - LPARAMでよく使う
//	
  // パラメータから取り出し
	template< typename To, typename From>
	To param_pack_cast(
		From from,
		typename boost::enable_if< boost::is_convertible<From,To> >::type* = 0 )
	{
		return static_cast<To>(from);	
	}
	template< typename To, typename From>
	To param_pack_cast(
		From from,
		typename boost::enable_if< boost::is_pointer<From> ( sizeof(To) > sizeof(void*) ) >::type* = 0 )
	{
		return reinterpret_cast<To>(from);	
	}
	
  // パラメータから取り出し
	template< typename To, typename From>
	To param_unpack_cast(
		From from,
		typename boost::enable_if< boost::is_convertible<From,To> >::type* = 0 )
	{
		return static_cast<To>(from);	
	}
	template< typename To, typename From>
	To param_unpack_cast(
		From from,
		typename boost::enable_if< boost::is_pointer<To> >::type* = 0 )
	{
		return reinterpret_cast<To>(from);	
	}

//##########################################################################################
// 明示的暗黙型変換
//	
	template<typename To, typename From>
	inline To implicit_cast(const From &x)
	{
		return x;
	}

//##########################################################################################
// ポインタ間の比較的安全な型変換
//	
	template<typename PtrTo, typename From>
	inline PtrTo pointer_cast(From* x)
	{
		return static_cast<PtrTo>(static_cast<void*>(x));
	}
	template<typename PtrTo, typename From>
	inline PtrTo pointer_cast(const From* x)
	{
		return static_cast<PtrTo>(static_cast<const void*>(x));
	}

}


#endif
