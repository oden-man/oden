#pragma once

#ifndef ODEN_KONBU_CLAZY_DT_UAC
#define ODEN_KONBU_CLAZY_DT_UAC

//#######################################
//
//
// ---------- STL -----------
// ---------- boost ---------
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#include <boost/numeric/conversion/is_subranged.hpp>
// ---------- oden ------------
// ---------- konbu -----------

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
//##################################################################
// ’Êí‚ÌZpŒ^•ÏŠ·
//	
namespace detail
{		
	// •‚“®¬”“_Œ^‚Ì•ÏŠ·
	template< typename X, typename Y, bool FloatingPointsMode >
	struct usual_arithmetic_conversion_impl
	{
		typedef mpl::eval_if< 
					mpl::or_< boost::is_same<X, long double>, boost::is_same<Y, long double> >,
					mpl::identity<long double>,
					mpl::eval_if< 
						mpl::or_< boost::is_same<X, double>, boost::is_same<Y, double> >,
						mpl::identity<double>,
						mpl::identity<float>
					>
				>
			result;
		
		typedef typename result::type type;
	};
	
	// ‚»‚Ì‘¼‚ÌZpŒ^‚Ì•ÏŠ·
	template< typename rawX, typename rawY >
	struct usual_arithmetic_conversion_impl< rawX, rawY, false >
	{
		typedef typename boost::integral_promotion<rawX>::type X;
		typedef typename boost::integral_promotion<rawY>::type Y;
		
		typedef mpl::eval_if< 
					mpl::or_< boost::is_same<X, unsigned long>, boost::is_same<Y, unsigned long> >,
					mpl::identity<unsigned long>,
					mpl::eval_if< 
						mpl::or_< 
							mpl::and_< boost::is_same<X, unsigned int>, boost::is_same<Y, long> >, 
							mpl::and_< boost::is_same<Y, unsigned int>, boost::is_same<X, long> >
						>,
						mpl::eval_if< 
							boost::numeric::is_subranged<unsigned int, long>,
							mpl::identity<long>,
							mpl::identity<unsigned long>
						>,
						mpl::eval_if<
							mpl::or_< boost::is_same<X, long>, boost::is_same<Y, long> >,
							mpl::identity<long>,
							mpl::identity<int>
						>
					>
				>
			result;
			
		typedef typename result::type type;
	};	
		
	// ƒtƒƒ“ƒg
	template< typename X, typename Y >
	struct usual_arithmetic_conversion
		: public usual_arithmetic_conversion_impl< X, Y, mpl::or_< boost::is_floating_point<X>, boost::is_floating_point<Y> >::value >
	{};
}

}
}} /* end of namespace gobou, oden */

#endif




