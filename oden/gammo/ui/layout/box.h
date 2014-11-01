#pragma once

#ifndef ODEN_GOB_UI_LAYOUT_BOX
#define ODEN_GOB_UI_LAYOUT_BOX

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
//###########################################################
//  一方向上にアイテムを詰め込む
//
	enum box_layout_elem
	{
		box_layout_spring,
		box_layout_block
	};
	
	class box_layout
		: public layout_base< box_layout, box_layout_elem >
	{
	public:	
	// コンストラクタ(起点)
		box_layout( layout_orientation ori = layout_vertical )
			: orient_( ori )
		{}
		
	// アイテムを追加する
	  // 固定幅
		box_layout& block( int w = layout_best_size )
		{
			return this->elem( w, box_layout_block );
		}
	  // 伸び縮みする
		box_layout& spring( int proportion = 1 )
		{
			return this->elem( proportion, box_layout_spring );
		}

	// オーバーライド
		void recalc( const rect& this_rect );
		size best_size()const;

	private:
	// 内部関数
		void divide_space_( rect& out, long length );
	
		layout_orientation orient_;
	};
	
	namespace layout
	{
		inline box_layout& box( layout_orientation ori ){ return *(new box_layout(ori)); }
		inline box_layout& vbox(){ return box(layout_vertical); }
		inline box_layout& hbox(){ return box(layout_horizontal); }
	}
	
	/*
	class box_layout
		: public container_layout<int>
	{
	public:	
	// コンストラクタ(起点)
		box_layout( layout_orientation ori = layout_vertical )
			: proportion_sum_(0), is_vertical_( ori == layout_vertical )
		{}
		
	// アイテムを追加する
		box_layout& box( int proportion, layout_object_pointer child = 0 )
		{
			add( child, proportion );
			proportion_sum_ += proportion;
			return *this;
		}
		
	// 比率の変更
		void set_proportion_at( int index, int value )
		{
			proportion_sum_	-= ( at(index).second - value );	// 比率の合計を修正する
			at(index).second = value;
		}
	
	// オーバーライド
		void recalc( const rect& this_rect );
		size minimum( )const;

	private:
		int proportion_sum_;
		bool is_vertical_;
	};
	*/

}}

#endif
