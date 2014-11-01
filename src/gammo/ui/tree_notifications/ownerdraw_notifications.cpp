#include "../../../Pch.h"

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <oden/gammo/ui/window/window.h>
#include <oden/gammo/ui/control/general/od_notifications.h>

namespace oden { namespace gammo
{
namespace wm
{
//########################################################################
//  �I�[�i�[�h���[
//
/* --------------------------------------------------------- */
//   WM_DRAWITEM + ID�ŕ���  :
/* -------------------------------------------------------- */
	rule_node_ptr control_owner_drawing(int ctlID)
	{
		return 
		condition<message>( 
			equal_( code(), const_(WM_DRAWITEM) )
			&&  equal_( member_of_lparam(&DRAWITEMSTRUCT::CtlID), const_(ctlID) )
			&&  not_equal_(  member_of_lparam(&DRAWITEMSTRUCT::CtlType), const_(ODT_MENU) )
		);
	}	
	rule_node_ptr control_owner_drawing(const window& from)
	{
		return 
		condition<message>( 
			equal_( code(), const_(WM_DRAWITEM) )
			&&  equal_( member_of_lparam(&DRAWITEMSTRUCT::CtlID), boost::bind(&window::child_id, boost::cref(from)) )
			&&  not_equal_(  member_of_lparam(&DRAWITEMSTRUCT::CtlType), const_(ODT_MENU) )
		);
	}
	
/* --------------------------------------------------------- */
//   ����ɍׂ������򂷂�
/* -------------------------------------------------------- */
	namespace od
	{
	  // item_action�ŕ���
		rule_node_ptr draw_entire_needed() 
		{
			return 
			condition<message>(
				equal_( member_of_lparam(&DRAWITEMSTRUCT::itemAction), const_(ODA_DRAWENTIRE) )
			);
		}
		rule_node_ptr focus_changed()
		{
			return 
			condition<message>(
				equal_( member_of_lparam(&DRAWITEMSTRUCT::itemAction), const_(ODA_FOCUS) )
			);
		}
		rule_node_ptr selection_changed()
		{
			return 
			condition<message>(
				equal_( member_of_lparam(&DRAWITEMSTRUCT::itemAction), const_(ODA_SELECT) )
			);
		}
	  // item_state�ŕ���
		rule_node_ptr item_focused();
		rule_node_ptr item_default();
		rule_node_ptr item_disabled();
		rule_node_ptr item_selected();
	  // item_id�ŕ���
		rule_node_ptr item_id(UINT id);	
	}
	
} /* end of namespace wm */
}} 

/*
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/bind/bind_member_variable.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/operator/comparision.hpp>
#include <boost/phoenix/operator/logical.hpp>
#include <boost/phoenix/operator/bitwise.hpp>
#include <boost/phoenix/object/reinterpret_cast.hpp>

namespace phnx = boost::phoenix;
namespace phnx_arg = boost::phoenix::arg_names;
using namespace phnx;
using namespace phnx_arg;
	// ->* ���g���ƁA�Ȃ����X�^�b�N�j�󂪋N����(phoenix v2 �ł͋N���Ȃ��H)
	//(&arg1) ->* &message::hwnd == ((&cref(wnd)) ->* &gammo::window::get_handle)()
*/