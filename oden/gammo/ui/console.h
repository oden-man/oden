#pragma once

#ifndef ODEN_GAMMO_UI_CONSOLE
#define ODEN_GAMMO_UI_CONSOLE

//#####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// ------ STL ---------
// ----- boost --------
// ------- oden ---------
// ------- gammo --------
#include <oden/gammo/system/kernel_object.h>

//
//
//###############################################################

namespace oden { namespace gammo
{	
//____________________________________________________________________________
//
//           �R���\�[��
//____________________________________________________________________________
//		
//########################################################################
//  �S�̂̊��蓖�āE�ݒ�Ȃ�
//
	class console
	{
	public:
		static bool allocate();
		static bool free();
		
		typedef struct freer {
			void operator()( console& ){ console::free(); }
		} deleter_type;
		
		static void set_as_stdio();
		
		
	};

//########################################################################
// �o��  
//
	class console_out
		: public kernel_obj
	{
	public:
	// �R���X�g���N�^
		console_out( kernel_obj_handle h = boost::none )
			: kernel_obj(h)
		{}

	// �����o�֐�

	};

}}

#endif

