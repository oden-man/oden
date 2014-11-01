#pragma once

#ifndef ODEN_GOB_UI_LAYOUT_RATIO
#define ODEN_GOB_UI_LAYOUT_RATIO

//#######################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- stl ------------
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
//  縦横比を指定
//
	class ratio_layout
		: public layout_base< ratio_layout, single_elem_layout >
	{
	public:
	// コンストラクタ
		ratio_layout( int w = 1, int h = 1 )
			: w_(w), h_(h)
		{}
		
	// 比を変更
		void set_ratio( int w, int h )
		{
			w_ = w;
			h_ = h;
		}
		
	// 実装関数
	  // レイアウトを再計算
		void recalc( const rect& this_rect );
	  // 最小値
		size best_size()const;
		
	private:
		int w_, h_;
	};

	namespace layout
	{
		inline ratio_layout& ratio(int w,int h){ return *(new ratio_layout(w,h)); }
	}
}}

#endif
