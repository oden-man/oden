#pragma once

#ifndef ODEN_GOB_UI_LAYOUT_RATIO
#define ODEN_GOB_UI_LAYOUT_RATIO

//#######################################################
//  �C���N���[�h
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
//  �c������w��
//
	class ratio_layout
		: public layout_base< ratio_layout, single_elem_layout >
	{
	public:
	// �R���X�g���N�^
		ratio_layout( int w = 1, int h = 1 )
			: w_(w), h_(h)
		{}
		
	// ���ύX
		void set_ratio( int w, int h )
		{
			w_ = w;
			h_ = h;
		}
		
	// �����֐�
	  // ���C�A�E�g���Čv�Z
		void recalc( const rect& this_rect );
	  // �ŏ��l
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
