#pragma once

#ifndef ODEN_GAMMO_CMN_TLIST
#define ODEN_GAMMO_CMN_TLIST

//#############################################
// インクルード
//

// --------- defs -----------
// -------- win -----------
// -------- boost ---------
// -------- oden ----------

//
//
//#############################################


namespace oden { namespace gammo
{
//
// mpl
// 
  // unused
	struct unused;

  // ignore
	template< typename >
	struct ignore 
	 { typedef void type; };
	
  // identity
	template< typename T >
	struct identity
	 { typedef T type; };

  // if_	 
	template< bool B, typename Y, typename N >
	struct if_c { typedef N type; };
	
	template< typename Y, typename N >
	struct if_c<true,Y,N> { typedef Y type; };

	template< bool B, typename Y, typename N >
	struct eval_if_c { typedef typename N::type type; };
	
	template< typename Y, typename N >
	struct eval_if_c<true,Y,N> { typedef typename Y::type type; };

//
// かんたん型リスト
//
  // 不使用型
	struct typelist_empty
	{};

  // 型リスト	
	struct typelist0
	{
		enum { size = 0 };
	};

	template< typename T1 >
	struct typelist1
	{
		enum { size = 1 };
		typedef T1 type1;
	};

	template< typename T1, typename T2 >
	struct typelist2
	{
		enum { size = 2 };
		typedef T1 type1;
		typedef T2 type2;
	};

	template< typename T1, typename T2, typename T3 >
	struct typelist3
	{
		enum { size = 3 };
		typedef T1 type1;
		typedef T2 type2;
		typedef T3 type3;
	};

	template< typename T1, typename T2, typename T3, typename T4 >
	struct typelist4
	{
		enum { size = 4 };
		typedef T1 type1;
		typedef T2 type2;
		typedef T3 type3;
		typedef T4 type4;
	};

  // エイリアス
	template< typename T1 = typelist_empty, typename T2 = typelist_empty, typename T3 = typelist_empty, typename T4 = typelist_empty >
	struct typelist : typelist4<T1,T2,T3,T4>
	{};
	
	template<>
	struct typelist<> : typelist0
	{};

	template< typename T1 >
	struct typelist<T1> : typelist1<T1>
	{};

	template< typename T1, typename T2 >
	struct typelist<T1,T2> : typelist2<T1,T2>
	{};

	template< typename T1, typename T2, typename T3 >
	struct typelist<T1,T2,T3>: typelist3<T1,T2,T3>
	{};


  // 取り出し
	template< typename Li, int I >
	struct typelist_at;

	template< typename Li >
	struct typelist_at<Li, 0> { typedef typename Li::type1 type; };

	template< typename Li >
	struct typelist_at<Li, 1> { typedef typename Li::type2 type; };

	template< typename Li >
	struct typelist_at<Li, 2> { typedef typename Li::type3 type; };

	template< typename Li >
	struct typelist_at<Li, 3> { typedef typename Li::type4 type; };

	
  // 取り出し(emptyを返す)
	template< typename Li, int I, bool Ok = true >
	struct typelist_get_impl
	{};

	template< typename Li >
	struct typelist_get_impl<Li, 0> { typedef typename Li::type1 type; };

	template< typename Li >
	struct typelist_get_impl<Li, 1> { typedef typename Li::type2 type; };

	template< typename Li >
	struct typelist_get_impl<Li, 2> { typedef typename Li::type3 type; };

	template< typename Li >
	struct typelist_get_impl<Li, 3> { typedef typename Li::type4 type; };

	template< typename Li, int I >
	struct typelist_get_impl<Li, I, false> { typedef typelist_empty type; };

	template< typename Li, int I >
	struct typelist_get
		: typelist_get_impl<Li, I, (I<Li::size)>
	{};
		
		
  // サイズ
	template< typename Li >
	struct typelist_size
	{
		enum { value = Li::size };
	};

	
	
}
}


#endif
