#include "../../../Pch.h"

#include <oden/gammo/ui/control/general/notifications.h>
#include <oden/gammo/ui/control/combo_box/notifications.h>

namespace oden { namespace gammo
{
namespace cbn
{
//########################################################################
//  ウィンドウのcondition, fuse
//
/* --------------------------------------------------------- */
//   CBN_CLOSEUP
/* -------------------------------------------------------- */
wm::rule_node_ptr	closed_up()
{
	return wm::commanded(CBN_CLOSEUP);
}
/* --------------------------------------------------------- */
//   CBN_DBLCLK
/* -------------------------------------------------------- */
wm::rule_node_ptr	item_double_clicked();
{
	return wm::commanded(CBN_DBLCLK);
}
/* --------------------------------------------------------- */
//   CBN_DROPDOWN
/* -------------------------------------------------------- */
wm::rule_node_ptr	dropping_down();
{
	return wm::commanded(CBN_DROPDOWN);
}
/* --------------------------------------------------------- */
//   CBN_EDITCHANGE
/* -------------------------------------------------------- */
wm::rule_node_ptr	edit_text_changed();
{
	return wm::commanded(CBN_EDITCHANGED);
}
/* --------------------------------------------------------- */
//   CBN_EDITUPDATE
/* -------------------------------------------------------- */
wm::rule_node_ptr	edit_text_updating();
{
	return wm::commanded(CBN_EDITUPDATE);
}
/* --------------------------------------------------------- */
//   CBN_ERRSPACE
/* -------------------------------------------------------- */
wm::rule_node_ptr	space_exhausted();
{
	return wm::commanded(CBN_ERRSPACE);
}

/* --------------------------------------------------------- */
//   CBN_KILLFOCUS
/* -------------------------------------------------------- */
wm::rule_node_ptr	lost_keyfocus()
{
	return wm::commanded(CBN_KILLFOCUS);
}
/* --------------------------------------------------------- */
//   CBN_SETFOCUS
/* -------------------------------------------------------- */
wm::rule_node_ptr	got_keyfocus()
{
	return wm::commanded(CBN_SETFOCUS);
}

/* --------------------------------------------------------- */
//   CBN_CHANGE
/* -------------------------------------------------------- */
wm::rule_node_ptr	select_changed()
{
	return wm::commanded(CBN_SELCHANGE);
}
/* --------------------------------------------------------- */
//   CBN_SELENDOK
/* -------------------------------------------------------- */
wm::rule_node_ptr	select_end_accepted()
{
	return wm::commanded(CBN_SELENDOK);
}
/* --------------------------------------------------------- */
//   CBN_SELENDCANCEL
/* -------------------------------------------------------- */
wm::rule_node_ptr	select_end_canceled()
{
	return wm::commanded(CBN_SELENDCANCEL);
}

}
}} /* end */










