#include "../../../Pch.h"

#include <oden/gammo/ui/resource/icon.h>
#include <oden/gammo/ui/resource/icon_selector.h>

namespace oden { namespace gammo
{
//#############################################################
// 並び替えPredicate
//

// サイズが大きい順
bool icon_larger::operator()( const icon::element& left, const icon::element& right )const
{
	return left.dimension() > right.dimension();
}
// 深度が大きい順
bool icon_more_vivid::operator()( const icon::element& left, const icon::element& right )const
{
	return left.depth() > right.depth();
}
// サイズ・深度が大きい順
bool icon_better::operator()( const icon::element& left, const icon::element& right )const
{
	if(left.depth() > right.depth())
		return true;
	else if(left.depth() == right.depth())
		return left.dimension() > right.dimension();
	return false;
}
// サイズ・深度が小さい順
bool icon_lighter::operator()( const icon::element& left, const icon::element& right )const
{
	if(left.depth() < right.depth())
		return true;
	else if(left.depth() == right.depth())
		return left.dimension() < right.dimension();
	return false;
}

//#############################################################
// 
//
bool icon_criteria::operator()(const icon::element& target)const
{
	// iconからBITMAPINFOHEADERを作成する
	icon_criteria_info info;
	info.dimension = target.dimension();
	info.bitCount = target.depth();
	
	// 処理にかける
	return (*this)(info);
}

}}








