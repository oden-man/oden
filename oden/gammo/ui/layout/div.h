#pragma once

#ifndef ODEN_GOB_UI_LAYOUT_DIVISON
#define ODEN_GOB_UI_LAYOUT_DIVISON

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
#include <vector>

// --------- win ------------
// --------- boost ----------
// --------- oden -----------
//---------- gammo ----------
#include <oden/gammo/ui/layout/base.h>

//
//####################################################


namespace oden { namespace gammo
{
	
	enum div_layout_place 
	{
		div_layout_rest = 0,
		div_layout_left,
		div_layout_right,
		div_layout_top,
		div_layout_bottom
	};

//###########################################################
//  上下左右にアイテムを詰める
//
	class div_layout
		: public layout_base< div_layout, div_layout_place >
	{
	public:	
	// コンストラクタ
		div_layout(){}
		
	// アイテムを追加する
		div_layout& div( div_layout_place add_at, int size )
		{
			this->elem( size, add_at );			
			return *this;
		}
		div_layout& left( int s = layout_best_size )
		{ return div( div_layout_left, s ); }

		div_layout& top( int s = layout_best_size )
		{ return div( div_layout_top, s ); }

		div_layout& right( int s = layout_best_size )
		{ return div( div_layout_right, s ); }

		div_layout& bottom( int s = layout_best_size )
		{ return div( div_layout_bottom, s ); }

		div_layout& rest( )
		{ return div( div_layout_rest, 0 ); }
	
	// オーバーライド
	  // レイアウトを再計算
		void recalc( const rect& this_rect );
	  // 最小値
		size best_size()const;
	};
	
	namespace layout
	{
		inline div_layout& div(){ return *(new div_layout()); }
	}
}}

#endif
