#include "../Pch.h"

#include <oden/gammo/ui/widget/window.h>


namespace oden { namespace gammo
{
/*-----------------------------------------------------------------------------------------------------*/
//
// window creator
//
/* ---------------------------------------------------------------------------------------------------- */
// 
bool widget_creator::new_class::create( widget_object* wi )
{
	if( widget_hub* phub = widget_hub::this_thread_hub() ) 
		phub->set_creating_widget( *wi );
	else
		return false;
	
	if( wi->has_parent_widget() )
		param.parent( wi->get_parent_widget()->get_window_handle() );
	
	class_param.procedure( &widget_hub::static_window_proc );
	
	if( window_handle h = create_window( class_param, param ) )
	{
		wi->set_window_handle(h);
		return true;
	}
	return false;
}

// 
bool widget_creator::new_param::create( widget_object* wi )
{
	if( wi->has_parent_widget() )
		param.parent( wi->get_parent_widget()->get_window_handle() );
	
	window_class::value_type wcex;
	if( 0 != ::GetClassInfoEx( get_app_handle().value(), class_id.get(), &wcex ) )
	{
		if( wcex.lpfnWndProc == &widget_hub::static_window_proc )
		{
			if( widget_hub* phub = widget_hub::this_thread_hub() ) 
				phub->set_creating_widget( *wi );
			else
				return false;
		}
	}
	
	if( window_handle h = create_window( class_id, param ) )
	{
		wi->set_window_handle(h);
		return true;
	}
	return false;
}

//
bool widget_creator::system_ctl_impl::create( const window_class_id& cid, widget_object* wi )
{
	if( wi->has_parent_widget() )
		param.parent( wi->get_parent_widget()->get_window_handle() );
	
	if( window_handle h = create_window( cid, param ) )
	{
		wi->set_window_handle(h);
		return true;
	}
	return false;
}

//
bool widget_creator::get_child::create( widget_object* wi )
{
	if( window_handle pa = wi->get_parent_widget()->get_window_handle() )
	{
		if( window_handle h = window( pa ).get_child( child_id ) )
		{
			wi->set_window_handle(h);
			return true;
		}
	}
	return false;
}

//
bool widget_creator::get_parent::create( widget_object* wi )
{
	if( window_handle pa = wi->get_parent_widget()->get_window_handle() )
	{
		wi->set_window_handle(pa);
		return true;
	}
	return false;
}





#if 0
/*-----------------------------------------------------------------------------------------------------*/
//
// window layouter
//
/* ---------------------------------------------------------------------------------------------------- */

//##############################################################
//  window_term_layout
//
// イベント
void window_term_layout::handle_layout_event( widget_object*, window_event& ev )
{
	ev.not_handled();
}
// このウィジェットを動かす
bool window_term_layout::move_wid( widget_object* wid )
{
	// 親のdwpを持っていればそれを利用し、なければ自力で動かす
	if( get_parent_dwp() && get_parent_dwp()->is_started() )
		return get_parent_dwp()->add( wid->get_window_handle(), location_, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER );	
	else 
		return wid->to_window().set_position( location_, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER );
}

//##############################################################
//  window_layout
//
// イベント
void window_layout::handle_layout_event( widget_object* wid, window_event& ev )
{
		switch( ev.code() )
		{
		case WM_SIZE:
			recalc_child_wid(wid);	
			move_child_wid(wid);
			break;
		case WM_SIZING:
			//limit_child_wid(this, size(0,0));
			break;
		default:
			ev.not_handled();
			break;
		}
}

// このレイアウトを動かす
bool window_layout::move_wid( widget_object* wid )
{
	// ターミナルレイアウトとして自分が動く->子供たちを動かす
	if( window_term_layout::move_wid(wid) ) {
		return move_child_wid(wid);
	}
	return false;
}

// 子レイアウトを動かす
bool window_layout::move_child_wid( widget_object* wid )
{
	if( !wid->has_child_widget() )
		return true;

	bool ret = true;

	// dwp はじめ
	get_dwp().start( wid->to_window().count_child() );
	
	// 子ウィジェットを動かす
	widget_object::widget_iterator itr = wid->begin(), end = wid->end();
	for( ; itr!=end; ++itr )
	{
		if( !(*itr)->move_widget() )
		{
			ret = false;
			break;
		}
	}
	
	// dwp を終了、子らを動かす
	get_dwp().end();
	
	return ret;
}

// 子以下を再計算する
bool window_layout::recalc_child_wid( widget_object* wid )
{
	if( layout_tree_ )
	{
		// サイズ取得
		rect rc;
		get_layout_rect(wid,rc); 

		// レイアウト再計算
		layout_tree_->recalc( rc );
	}
	return true;
}

// 最大・最小を計算
/*
// イベントハンドラ
// WM_CREATE : このウィンドウが作成された時、親ウィンドウがあらかじめ計算しておいた位置に移動する
void window_layout::layout_on_start( widget_object* wid )
{
	if( wid->has_parent() )
		move_widget(wid);
}

// WM_SIZING : このウィンドウの大きさが変わっている間、サイズ制限を適用する
void window_layout::layout_on_sizing( widget_object*, window_event& ev )
{
	WPARAM edge = ev.wparam();
	rect rc = *(reinterpret_cast<RECT*>( ev.lparam() ));

	edge = rc.area();
	edge = 0;
	
	//if( children_layout )
	//	min_size = children_layout>minimum();
	// size max_size = layout_->maximum();
	
	if( rc.height() < min_size.height() ) 
	{
		if( edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT )
		{
			point br = rc.botright();
			rc.set_height( min_size.height() ); 
			rc.move_botright(br);
		}
		else
			rc.set_height( min_size.height() ); 
	}
	if( rc.width() < min_size.width() )
	{
		if( edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT )
		{
			point br = rc.botright();
			rc.set_width( min_size.width() );
			rc.move_botright(br);
		}
		else
			rc.set_width( min_size.width() );
	}
}
*/
/*-----------------------------------------------------------------------------------------------------*/
//
// window final handle event
//
/* ---------------------------------------------------------------------------------------------------- */
//##############################################################
//  window_def_event_handler
//
void window_def_event_handler::final_handle_event( window_event& ev )
{
	widget_hub::call_default_window_proc( ev );
}

//##############################################################
//  subclassable_window
//
void subclassable_window::final_handle_event( window_event& ev )
{
	call_original_proc( ev );
}

//
void subclassable_window::subclass_wid( widget_object* wid )
{
	if( is_subclassed() )
		return;
	
	original_proc_ = wid->to_window().procedure_ptr();
	wid->to_window().set_procedure_ptr( &widget_hub::static_window_proc );
	
	// ウィジェットハブに追加する
	if( widget_hub* phub = widget_hub::this_thread_hub() )
		phub->add_widget_to_map( *wid );
}

	// 元に戻す
	void subclassable_window::unsubclass_wid( widget_object* wid )
	{
		if( !is_subclassed() )
			return;
		
		wid->to_window().set_procedure_ptr( original_proc_ );
		original_proc_ = 0;

		// ウィジェットハブから取り除く
		if( widget_hub* phub = widget_hub::this_thread_hub() )
			phub->remove_widget_from_map( wid->to_window().handle_value() );
	}

// 元のプロシージャを呼ぶ
void subclassable_window::call_original_proc( window_event& ev )
{
	ev.result() = ::CallWindowProc( original_proc_, ev.target().handle_value(), ev.code(), ev.wparam(), ev.lparam() ); 
}
#endif

} /* end of namespace gammo */
} /* end of namespace oden */

