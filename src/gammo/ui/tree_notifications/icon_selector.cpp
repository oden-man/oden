#include "../../../Pch.h"

#include <oden/gammo/ui/resource/icon.h>
#include <oden/gammo/ui/resource/icon_selector.h>

namespace oden { namespace gammo
{
//#############################################################
// ���ёւ�Predicate
//

// �T�C�Y���傫����
bool icon_larger::operator()( const icon::element& left, const icon::element& right )const
{
	return left.dimension() > right.dimension();
}
// �[�x���傫����
bool icon_more_vivid::operator()( const icon::element& left, const icon::element& right )const
{
	return left.depth() > right.depth();
}
// �T�C�Y�E�[�x���傫����
bool icon_better::operator()( const icon::element& left, const icon::element& right )const
{
	if(left.depth() > right.depth())
		return true;
	else if(left.depth() == right.depth())
		return left.dimension() > right.dimension();
	return false;
}
// �T�C�Y�E�[�x����������
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
	// icon����BITMAPINFOHEADER���쐬����
	icon_criteria_info info;
	info.dimension = target.dimension();
	info.bitCount = target.depth();
	
	// �����ɂ�����
	return (*this)(info);
}

}}








