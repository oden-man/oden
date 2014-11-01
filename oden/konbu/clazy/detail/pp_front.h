

//_____________________________________________________________________________________________________
//
//                               本体定義
//_____________________________________________________________________________________________________
//
//------------------------------------------------------------------------
// ファイルに対する引数類を定義
#define EXPR_NAME   	BOOST_PP_ARRAY_ELEM(0,CLAZY_EXPR)
#define EXPR_IMPL_TAG 	BOOST_PP_ARRAY_ELEM(1,CLAZY_EXPR)
#define N 				BOOST_PP_ARRAY_ELEM(2,CLAZY_EXPR)
#define EXPR_IMPL		BOOST_PP_ARRAY_ELEM(3,CLAZY_EXPR)

// 追加の型パラメータマクロを調整する
/* 型パラメータ */
#ifndef CLAZY_EXPR_EXTRA_T
#	define T_N 0
#else
#	define T_N CLAZY_EXPR_EXTRA_T
#endif
/* 非型パラメータ */
#ifndef CLAZY_EXPR_EXTRA_NT
#	define CLAZY_EXPR_EXTRA_NT
#	define NT_N 0
#else
#	define NT_N BOOST_PP_SEQ_SIZE(CLAZY_EXPR_EXTRA_NT)
#endif

// 追加メンバ
#ifndef CLAZY_EXPR_MEMBER_1
#	define CLAZY_EXPR_MEMBER_1
#endif
#ifndef CLAZY_EXPR_MEMBER_2
#	define CLAZY_EXPR_MEMBER_2
#endif
#ifndef CLAZY_EXPR_MEMBER_3
#	define CLAZY_EXPR_MEMBER_3
#endif

//------------------------------------------------------------------------
// コンマ
#define EXPR_SEP_COMMA(a,b)		   BOOST_PP_COMMA_IF( BOOST_PP_AND(a,b) )

// エクストラテンプレート引数を展開
#define EXPR_EXTRA_NT_DECL(z,n,_)  BOOST_PP_SEQ_ELEM(n, CLAZY_EXPR_EXTRA_NT) ExtraNT##n
#define EXPR_EXTRA_NT_WRAP(z,n,_)  detail::nttp<BOOST_PP_SEQ_ELEM(n, CLAZY_EXPR_EXTRA_NT), ExtraNT##n>

// テンプレート引数宣言、実際のファンクタ型、ラップ済みの追加テンプレート引数リスト
#if !(N==0 && T_N==0 && NT_N==0)
#	define EXPR_TEMPLATE_DECL(def,n) \
		template<																					\
			BOOST_PP_ENUM_PARAMS(T_N, typename Extra)  	EXPR_SEP_COMMA(T_N, BOOST_PP_ADD(NT_N,n))	\
			BOOST_PP_ENUM(NT_N, EXPR_EXTRA_NT_DECL, _) 	EXPR_SEP_COMMA(NT_N, n)						\
			BOOST_PP_ENUM_BINARY_PARAMS(n, typename Arg, def)										\
		>
#	define EXPR_SELF_TYPE(n) \
		EXPR_NAME<																					\
			BOOST_PP_ENUM_PARAMS(T_N, Extra)  			EXPR_SEP_COMMA(T_N, BOOST_PP_ADD(NT_N,n))	\
			BOOST_PP_ENUM_PARAMS(NT_N, ExtraNT)			EXPR_SEP_COMMA(NT_N, n)						\
			BOOST_PP_ENUM_PARAMS(n, Arg)															\
		>
#	define EXPR_TEMPLATE_EXTRA_ARGS_WRAPPED \
		BOOST_PP_CAT(mpl::vector, BOOST_PP_ADD(T_N,NT_N))<											\
			BOOST_PP_ENUM_PARAMS(T_N, Extra) 			EXPR_SEP_COMMA(T_N, NT_N)					\
			BOOST_PP_ENUM(NT_N, EXPR_EXTRA_NT_WRAP, _)												\
		>
#	define EXPR_AS_TEMPLATE_DECL \
		template<																					\
			BOOST_PP_ENUM_PARAMS(T_N, typename Extra)  	EXPR_SEP_COMMA(T_N, BOOST_PP_ADD(NT_N,1))	\
			BOOST_PP_ENUM(NT_N, EXPR_EXTRA_NT_DECL, _) 	EXPR_SEP_COMMA(NT_N, 1)						\
			typename Operands = mpl::vector0<>														\
		>
#else
#	define EXPR_TEMPLATE_DECL(def,n)
#	define EXPR_SELF_TYPE(n)								EXPR_NAME
#	define EXPR_TEMPLATE_EXTRA_ARGS_WRAPPED					mpl::vector0<>
#	define EXPR_AS_TEMPLATE_DECL							template< typename Dummy = void >
#endif

// ローカルテンプレート引数を展開
#ifdef CLAZY_EXPR_LOCAL_T
#	define EXPR_TEMPLATE_LOCAL_ARGS 		BOOST_PP_CAT(mpl::vector, BOOST_PP_SEQ_SIZE(CLAZY_EXPR_LOCAL_T))< BOOST_PP_SEQ_ENUM(CLAZY_EXPR_LOCAL_T) >
#else
#	define EXPR_TEMPLATE_LOCAL_ARGS			mpl::vector0<>
#endif

// 非ファンクタ型をvalueでラップする
#ifdef CLAZY_VALUE_BEING_INVOKED
#	define EXPR_OPERAND_VALUE_WRAPPER		mpl::identity<mpl::_1>
#else
#	define EXPR_OPERAND_VALUE_WRAPPER		detail::value_wrapper<mpl::_1>
#endif 

//------------------------------------------------------------------------
// 参照型リスト
#define EXPR_CREF_VECTOR(n,name) fusion::vector<BOOST_PP_ENUM_BINARY_PARAMS(n, name, const& BOOST_PP_INTERCEPT)>
	
//###########################################################################
// ファンクタひな形
//
	EXPR_TEMPLATE_DECL(= detail::unspecified BOOST_PP_INTERCEPT, N)
	class EXPR_NAME
		: public detail::EXPR_IMPL<
				  /* self */
					EXPR_SELF_TYPE(N),
				  /* tag */
				  	EXPR_IMPL_TAG,
				  /* template parameter */
				  	custom_template_parameter<
				  		EXPR_IMPL_TAG,
						typename mpl::transform<
							typename mpl::remove< BOOST_PP_CAT(mpl::vector,N)< BOOST_PP_ENUM_PARAMS(N, Arg) >, detail::unspecified >::type,
							EXPR_OPERAND_VALUE_WRAPPER
						>::type,
					  	typename mpl::copy<
					  		EXPR_TEMPLATE_EXTRA_ARGS_WRAPPED,				// エキストラ
							mpl::back_inserter< EXPR_TEMPLATE_LOCAL_ARGS >	// ローカル
						>::type
					>
				>
	{
	private:
	// 自分の型
		typedef EXPR_SELF_TYPE(N) self;
	// ベース型
		typedef typename self::implement_type	base;
	public:
	// エイリアス
		typedef typename self::action_tag		action_tag;	
		typedef typename self::operands			operands;	
		typedef typename self::extra_args		extra_args;
		
	// 暗黙に変換可能な型を調べる
		template< typename T >
		struct is_constructible_with
			: public mpl::if_< 
						typename is_constructor_argument< T, action_tag, operands, extra_args >::type,
						mpl::true_,
						boost::is_convertible< T, self >
					>::type
		{};

	// デフォルトコンストラクタ
		EXPR_NAME()
			: implement_type()
		{}
		
	// コピーコンストラクタ
		EXPR_NAME( const self& other )
			: implement_type( other )
		{}

	// 値を最初に受けるコンストラクタ(1~MAX)
	#define CTOR(z, n, _) \
		template< BOOST_PP_ENUM_PARAMS(n, typename A) >											\
		explicit EXPR_NAME( 																	\
					BOOST_PP_ENUM_BINARY_PARAMS(n, const A, &a), 								\
					typename ctor_mode::disable_if_tag<A0>::type* = 0							\
				)																				\
			: implement_type( 																	\
				typename detail::select_ctor_mode<operands, EXPR_CREF_VECTOR(n, A)>::type(),	\
				EXPR_CREF_VECTOR(n, A)( BOOST_PP_ENUM_PARAMS(n,a) ) 							\
			)																					\
		{}
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(KONBU_CLAZY_CTOR_LIMIT), CTOR, _)
	#undef CTOR
	
	// 値を下に転送するコンストラクタ
		template< typename Any >
		EXPR_NAME( ctor_mode::transfer_arg tag, Any& arg )
			: implement_type( tag, arg )
		{}
		
	// 値をオペランドそのものとしてコピーするコンストラクタ（明示的にタグを指定）
		template< typename Operands >
		EXPR_NAME( ctor_mode::copy_operand tag, const Operands& operand )
			: implement_type( tag, operand )
		{}
	
	// 演算子を実装クラスからよびこむ
		using base::operator=;
		using base::operator[];
		
	// その他のメンバ
		CLAZY_EXPR_MEMBER_1
		CLAZY_EXPR_MEMBER_2
		CLAZY_EXPR_MEMBER_3
	};
	
//###########################################################################
// ファンクタ型をオペランド(変換前)のリストから生成
//
	EXPR_AS_TEMPLATE_DECL
	struct BOOST_PP_CAT(as_,EXPR_NAME)
	{
	private:
	#if (N>0)
		typedef mpl::vector<BOOST_PP_ENUM_PARAMS(N, detail::unspecified BOOST_PP_INTERCEPT)> unspecifieds;
		
		typedef 
		typename mpl::copy< 
					mpl::iterator_range< 
						typename mpl::begin<unspecifieds>::type,
						typename mpl::advance_c< typename mpl::begin<unspecifieds>::type, (N-mpl::size<Operands>::value) >::type
					>,
					mpl::back_inserter< Operands >
				>::type
			real_operands;
		
	#define ARGAT(z,n,_) typedef typename mpl::at_c<real_operands,n>::type Arg##n;
		BOOST_PP_REPEAT_FROM_TO(0, N, ARGAT, _)
	#undef  ARGAT
	#endif

	public:
 		typedef EXPR_SELF_TYPE(N) type;		
	};	
	
//#################################################################
// 生成関数(または演算子オーバーロード)
//
#define SYMBOL_WRAP_VALUE(z,n,_) 	detail::wrap_value(a##n)
#define SYMBOL_PARAM(z,n,seq)		BOOST_PP_SEQ_ELEM(n,seq)(n) const &a##n

// 普通の関数
#define SYMBOL_FUNC_SIMPLE(z,n,name) \
	EXPR_TEMPLATE_DECL(BOOST_PP_INTERCEPT, n)								\
	EXPR_SELF_TYPE(n)														\
	name ( BOOST_PP_ENUM_BINARY_PARAMS(n, const Arg, &a) ) 					\
	{																		\
		return 			 													\
		EXPR_SELF_TYPE(n) ( BOOST_PP_ENUM(n, SYMBOL_WRAP_VALUE, _) );		\
	}

// 演算子用
#define SYMBOL_FUNC_PROD(n,prod,enabler) \
	EXPR_TEMPLATE_DECL(BOOST_PP_INTERCEPT, n)								\
	typename enabler<														\
		EXPR_SELF_TYPE(n), mpl::vector<BOOST_PP_ENUM_PARAMS(n, Arg)>		\
	>::type																	\
	CLAZY_SYMBOL_OPERATOR ( BOOST_PP_ENUM(n, SYMBOL_PARAM, prod) ) 			\
	{																		\
		return 			 													\
		EXPR_SELF_TYPE(n) ( BOOST_PP_ENUM(n, SYMBOL_WRAP_VALUE, _) );		\
	}
	
// 値 ・ファンクタどちらでも受け取れる
#  if defined(CLAZY_SYMBOL)
	SYMBOL_FUNC_SIMPLE(_, N, CLAZY_SYMBOL)	
	
// 値 ・ファンクタどちらでも受け取れる、複数のシグネチャの関数
#elif defined(CLAZY_SYMBOL_VARIADIC)
	BOOST_PP_REPEAT_FROM_TO( 0, BOOST_PP_INC(N), SYMBOL_FUNC_SIMPLE, CLAZY_SYMBOL_VARIADIC)
#elif defined(CLAZY_SYMBOL_VARIADIC_R)
	BOOST_PP_REPEAT_FROM_TO( BOOST_PP_ARRAY_ELEM(1,CLAZY_SYMBOL_VARIADIC_R), BOOST_PP_ARRAY_ELEM(2,CLAZY_SYMBOL_VARIADIC_R), SYMBOL_FUNC_SIMPLE, BOOST_PP_ARRAY_ELEM(0,CLAZY_SYMBOL_VARIADIC_R))

// 演算子オーバーロード：少なくとも一つは引数がファンクタで、複数のシグネチャを持つ
#elif defined(CLAZY_SYMBOL_OPERATOR)

#define VAL_PARAM(n)  	Arg##n
#define FUN_PARAM(n)  	functor<Arg##n>

#  if (N==1)
	SYMBOL_FUNC_PROD(1, (FUN_PARAM), detail::enable_always)
#elif (N==2)
	SYMBOL_FUNC_PROD(2, (FUN_PARAM)(FUN_PARAM), detail::enable_always)
	SYMBOL_FUNC_PROD(2, (FUN_PARAM)(VAL_PARAM), detail::disable_if_right_func)
	SYMBOL_FUNC_PROD(2, (VAL_PARAM)(FUN_PARAM), detail::disable_if_left_func)
#endif

#undef VAL_PARAM
#undef FUN_PARAM

// 関数を生成しない
#else

#endif
	
#undef SYMBOL_WRAP_VALUE
#undef SYMBOL_PARAM
#undef SYMBOL_FUNC_SIMPLE
#undef SYMBOL_FUNC_PROD

//###################################################################
// マクロ後始末
//
#undef CLAZY_EXPR
#undef CLAZY_EXPR_EXTRA_T
#undef CLAZY_EXPR_EXTRA_NT
#undef CLAZY_EXPR_LOCAL_T

#undef N
#undef T_N
#undef NT_N

#undef EXPR_NAME
#undef EXPR_IMPL
#undef EXPR_ARITY

#undef EXPR_EXTRA_NT_DECL
#undef EXPR_EXTRA_NT_WRAP

#undef EXPR_SEP_COMMA

#undef EXPR_TEMPLATE_DECL
#undef EXPR_SELF_TYPE
#undef EXPR_TEMPLATE_EXTRA_ARGS_WRAPPED
#undef EXPR_AS_TEMPLATE_DECL

#undef EXPR_TEMPLATE_LOCAL_ARGS
#undef EXPR_OPERAND_VALUE_WRAPPER

#undef EXPR_CREF_VECTOR

#undef CLAZY_EXPR_MEMBER_1
#undef CLAZY_EXPR_MEMBER_2
#undef CLAZY_EXPR_MEMBER_3

#undef CLAZY_SYMBOL
#undef CLAZY_SYMBOL_VARIADIC
#undef CLAZY_SYMBOL_VARIADIC_R
#undef CLAZY_SYMBOL_OPERATOR





