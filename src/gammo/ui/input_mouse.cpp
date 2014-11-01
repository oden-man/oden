#include "../Pch.h"

#include <oden/gammo/ui/input/mouse.h>
#include <oden/gammo/ui/widget/object.h>

namespace oden { namespace gammo
{
//##########################################################
//                 
//
UINT double_click_time()
{
	return ::GetDoubleClickTime();
}
bool set_double_click_time(UINT c)
{
	return 0!=::SetDoubleClickTime(c);
}

bool swap_mouse_button(bool b)
{
	return 0!=::SwapMouseButton(b?TRUE:FALSE);
}

//##########################################################
//                  mouse_capture
//
mouse_capture::mouse_capture( window_handle w )
{
	this->start(w);
}
mouse_capture::mouse_capture( widget_object* p )
{
	if(p)
		this->start( p->get_window_handle() );
}

window_handle mouse_capture::start( window_handle h )
{
	return ::SetCapture(h.value());
}
bool mouse_capture::release()
{
	return 0!=::ReleaseCapture();
}

window_handle mouse_capture::get()
{
	return ::GetCapture();
}

} /* end of namespace gammo */
} /* end of namespace oden */
