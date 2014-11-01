
#ifndef ODEN_KONBU_CLAZY_OBJ_CTOR
#define ODEN_KONBU_CLAZY_OBJ_CTOR

//#######################################
//

// ---------- STL -----------
// ---------- boost ---------
// ---------- konbu ----------
#include "../detail/callable_expression.h"

//
//#######################################


//________________________________________________________________________________________________________________
//
// 
//________________________________________________________________________________________________________________
//	
namespace oden { namespace konbu { namespace clazy
{
//#######################################################################
// ctor共通	
//	
  // value_types
	template< typename Operands, typename Context, typename Extra >
	struct value_types_raw< tag::construct, Operands, Context, Extra >
		: public detail::value_types_all< as_any_tag, Operands >
	{};
	
  // return_type
	template< typename T >
	struct return_type_static< tag::construct, mpl::vector1<T> >
	{
		typedef return_rvalue<T> type;
	};

  //  action_impl
	template< typename T > 						
	struct action_impl< tag::construct, mpl::vector1<T> >					
	{
	// コンストラクタ
		action_impl(){}
		template< typename X > explicit action_impl( X& ){}
		
	#define APPLY(z,n,_) \
		template< 																		\
			typename Ret, 																\
			BOOST_PP_ENUM_PARAMS(n, typename Operand) BOOST_PP_COMMA_IF(n)				\
			typename Context															\
		>																				\
		Ret apply( BOOST_PP_ENUM_BINARY_PARAMS(n, Operand, opr) BOOST_PP_COMMA_IF(n) 	\
			const Context&																\
		)const 																			\
		{																				\
			return T( BOOST_PP_ENUM_PARAMS(n, opr) );									\
		}
		BOOST_PP_REPEAT_FROM_TO(0, ODEN_KONBU_CLAZY_CTOR_EXPR_MAX_ARITY)
	#undef APPLY
	
	};
	
	//////// 生成 ////////////
  	#define CLAZY_EXPR 		   CLAZY_VARIADIC_CALLABLE(construct_, tag::construct)
	#define CLAZY_EXPR_EXTRA_T 1
	#define CLAZY_SYMBOL	   construct
  	#include "../detail/pp_front.h"
	
}}}
#endif


