#include "../../Pch.h"

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <oden/gammo/ui/window/window.h>
#include <oden/gammo/ui/window/notifications.h>
#include <oden/gammo/ui/window/procedure/expression.h>

namespace oden { namespace gammo
{
namespace wm
{
//########################################################################
//  ウィンドウのcondition, fuse
//
/* --------------------------------------------------------- */
//   ウィンドウで分岐 : WM_DESTROY後、切断
/* -------------------------------------------------------- */
rule_node_ptr message_from(const window& wnd)
{
	rule_node_ptr node = 
	  delay_fuse<message>(
		equal_( code(), const_(WM_DESTROY) )
		&& equal_( hwnd(), boost::bind(&window::get_handle, boost::cref(wnd)) )
	) >>= condition<message>(
		equal_( hwnd(), boost::bind(&window::get_handle, boost::cref(wnd)) )
	);
	return node;
}

/* ------------------------------------ */
//   WM_CREATE
/* ------------------------------------ */
rule_node_ptr creating()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_CREATE) )
	);
}

/* ------------------------------------ */
//   WM_SIZE
/* ------------------------------------ */
rule_node_ptr resized()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_SIZE) )
	);
}

/* ------------------------------------ */
//   WM_SIZE - wParamフラグ分岐
/* ------------------------------------ */
rule_node_ptr size::restored()
{
	return 
	condition<message>(
		equal_( wparam<UINT>(), const_(SIZE_RESTORED) )
	);
}
rule_node_ptr size::maximized()
{
	return 
	condition<message>(
		equal_( wparam<UINT>(), const_(SIZE_MAXIMIZED) )
	);
}
rule_node_ptr size::minimized()
{
	return 
	condition<message>(
		equal_( wparam<UINT>(), const_(SIZE_MINIMIZED) )
	);
}

/* ------------------------------------ */
//   WM_MOVE
/* ------------------------------------ */
rule_node_ptr moved()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_MOVE) )
	);
}

/* ------------------------------------ */
//   WM_SHOWWINDOW
/* ------------------------------------ */
rule_node_ptr showing()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_SHOWWINDOW) )
		&& equal_( wparam<>(), const_(TRUE) )
	);
}
rule_node_ptr hiding()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_SHOWWINDOW) )
		&& equal_( wparam<>(), const_(FALSE) )
	);
}

/* ------------------------------------ */
//   WM_SHOWWINDOW - lparam原因フラグ
/* ------------------------------------ */
rule_node_ptr show_reason::showwindow_called()
{
	return 
	condition<message>(
		equal_( lparam<>(), const_(0) )
	);
}
rule_node_ptr show_reason::others_unzoomed()
{
	return 
	condition<message>(
		equal_( lparam<>(), const_(SW_OTHERUNZOOM) )
	);
}
rule_node_ptr show_reason::others_zoomed()
{
	return 
	condition<message>(
		equal_( lparam<>(), const_(SW_OTHERZOOM) )
	);
}
rule_node_ptr show_reason::parent_restoring()
{
	return 
	condition<message>(
		equal_( lparam<>(), const_(SW_PARENTOPENING) )
	);
}
rule_node_ptr show_reason::parent_closing()
{
	return 
	condition<message>(
		equal_( lparam<>(), const_(SW_PARENTCLOSING) )
	);
}

/* ------------------------------------ */
//   WM_ENABLE
/* ------------------------------------ */
rule_node_ptr enabled()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_ENABLE) )
		&& equal_( wparam<>(), const_(TRUE) )
	);
}
rule_node_ptr disabled()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_ENABLE) )
		&& equal_( wparam<>(), const_(FALSE) )
	);
}

/* ------------------------------------ */
//   WM_DESTROY
/* ------------------------------------ */
rule_node_ptr dying()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_DESTROY) )
	);
}

/* ------------------------------------ */
//   WM_CLOSE
/* ------------------------------------ */
rule_node_ptr closing()
{
	return 
	condition<message>(
		equal_( code(), const_(WM_CLOSE) )
	);
}

} /* end of namespace wm */
}} /* end of namespace gammo, oden */
