#include "../../../Pch.h"

#include <oden/gammo/ui/control/general/notifications.h>
#include <oden/gammo/ui/control/text_box/notifications.h>

namespace oden { namespace gammo
{
namespace en
{
//########################################################################
//  ウィンドウのcondition, fuse
//
/* --------------------------------------------------------- */
//   EN_CHANGE
/* -------------------------------------------------------- */
wm::rule_node_ptr	text_changed()
{
	return wm::commanded(EN_CHANGE);
}

/* --------------------------------------------------------- */
//   EN_UPDATE
/* -------------------------------------------------------- */
wm::rule_node_ptr	text_updating()
{
	return wm::commanded(EN_UPDATE);
}

/* --------------------------------------------------------- */
//   EN_KILLFOCUS
/* -------------------------------------------------------- */
wm::rule_node_ptr	lost_keyfocus()
{
	return wm::commanded(EN_KILLFOCUS);
}

/* --------------------------------------------------------- */
//   EN_SETFOCUS
/* -------------------------------------------------------- */
wm::rule_node_ptr	got_keyfocus()
{
	return wm::commanded(EN_SETFOCUS);
}

/* --------------------------------------------------------- */
//   EN_MAXTEXT
/* -------------------------------------------------------- */
wm::rule_node_ptr	reached_textmax()
{
	return wm::commanded(EN_MAXTEXT);
}

/* --------------------------------------------------------- */
//   EN_HSCROLL
/* -------------------------------------------------------- */
wm::rule_node_ptr	hscrolling()
{
	return wm::commanded(EN_HSCROLL);
}

/* --------------------------------------------------------- */
//   EN_VSCROLL
/* -------------------------------------------------------- */
wm::rule_node_ptr	vscrolling()
{
	return wm::commanded(EN_VSCROLL);
}

/* --------------------------------------------------------- */
//   EN_ERRSPACE
/* -------------------------------------------------------- */
wm::rule_node_ptr	space_exhausted()
{
	return wm::commanded(EN_ERRSPACE);
}

} /* end of namespace en */
}} /* end of namespace gammo, oden */

