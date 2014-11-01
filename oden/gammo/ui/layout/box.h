#pragma once

#ifndef ODEN_GOB_UI_LAYOUT_BOX
#define ODEN_GOB_UI_LAYOUT_BOX

//#######################################################
//  �C���N���[�h
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
//  �������ɃA�C�e�����l�ߍ���
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
	// �R���X�g���N�^(�N�_)
		box_layout( layout_orientation ori = layout_vertical )
			: orient_( ori )
		{}
		
	// �A�C�e����ǉ�����
	  // �Œ蕝
		box_layout& block( int w = layout_best_size )
		{
			return this->elem( w, box_layout_block );
		}
	  // �L�яk�݂���
		box_layout& spring( int proportion = 1 )
		{
			return this->elem( proportion, box_layout_spring );
		}

	// �I�[�o�[���C�h
		void recalc( const rect& this_rect );
		size best_size()const;

	private:
	// �����֐�
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
	// �R���X�g���N�^(�N�_)
		box_layout( layout_orientation ori = layout_vertical )
			: proportion_sum_(0), is_vertical_( ori == layout_vertical )
		{}
		
	// �A�C�e����ǉ�����
		box_layout& box( int proportion, layout_object_pointer child = 0 )
		{
			add( child, proportion );
			proportion_sum_ += proportion;
			return *this;
		}
		
	// �䗦�̕ύX
		void set_proportion_at( int index, int value )
		{
			proportion_sum_	-= ( at(index).second - value );	// �䗦�̍��v���C������
			at(index).second = value;
		}
	
	// �I�[�o�[���C�h
		void recalc( const rect& this_rect );
		size minimum( )const;

	private:
		int proportion_sum_;
		bool is_vertical_;
	};
	*/

}}

#endif
