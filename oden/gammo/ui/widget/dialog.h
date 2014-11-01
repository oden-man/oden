#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_DLG
#define ODEN_GAMMO_UI_WIDGETS_DLG

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------	
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
// --------- gammo ---------
#include <oden/gammo/ui/dialog/dialog.h>
#include <oden/gammo/ui/widget/window.h>

//
//####################################################


namespace oden { namespace gammo
{
//______________________________________________________________________________________________________
//
//          ウィジェット・パーツ
//______________________________________________________________________________________________________
//
//#################################################
//  クラスを新しくつくる
// 
	template< typename Base >
	class new_dialog
		: public Base
	{
	public:
		new_dialog()
			: tmpl_id_(), init_param_(0), hInst_()
		{}
		
		bool create_control();

		// ダイアログ内容記述
		void describe_dialog( res_id tmpl_id, LPARAM lparam = 0, instance_handle hResModule = boost::none, bool processDlgKey = true ) 
		{
			tmpl_id_ = tmpl_id;
			init_param_ = lparam;
			hInst_ = hResModule;
			
			if( processDlgKey )
			{
				if( widget_hub* phub = widget_hub::this_thread_hub() )
					phub->add_event_filter(this);
			}
		}

	// オーバーライド
	  // キーコード処理をする
		bool preprocess_event( preprocessing_event& ev )
		{
			if( this->process_dlg_message( ev.msg() ) )
				ev.set_processed();
			return true;
		}
	private:
		res_id tmpl_id_;
		LPARAM init_param_;
		instance_handle hInst_;
	};
	
	template< typename B >
	bool new_dialog<B>::create_control()
	{
		if( widget_hub* phub = widget_hub::this_thread_hub() ) 
			phub->set_creating_widget( *static_cast<widget_object*>(this) );
		else
			return false;
		
		window_handle parent = boost::none;
		if( has_parent_widget() )
			parent = get_parent_widget()->get_window_handle();
		
		gammo::dialog ndlg;
		if( ndlg.create( tmpl_id_, parent, init_param_, hInst_ ) ) 
		{
			set_window_handle( ndlg.handle() );
			return true;
		}
		return false;
	}

//______________________________________________________________________________________________________
//
//          ウィジェットパーツ・プリセット
//______________________________________________________________________________________________________
//
  // モードレスダイアログ
	typedef new_dialog< dialog_widget > 	new_dialog_widget;


}} /* end of namespace oden.gammo */

#endif

