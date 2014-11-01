#pragma once

#ifndef ODEN_GAMMO_CMN_ERR
#define ODEN_GAMMO_CMN_ERR

//##############################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>

// --------- boost -----------

// --------- oden -----------
//#include <oden/tsumile/tsumile.h>

//
//
//#############################################



namespace oden { namespace gammo
{
//#############################################################
// �G���[�R�[�h
//
  // �G���[�R�[�h
	typedef nullable< DWORD, singular_null<DWORD,0> > win32_error_code;

  // �N���X
	class win32_error
	{
	public:
	// �����o�֐�
	  // 
		static win32_error last();
		static void set_last( win32_error_code c );
	  // �ŐV�̃G���[�R�[�h���擾
		void update();
	  // �G���[�R�[�h�����b�Z�[�W������
		string format()const;
	  // �v���p�e�B
		win32_error_code get()const { return c_; }
		DWORD value()const { return c_.value(); }
		bool  none()const { return c_.is_null(); }

	// �R���X�g���N�^
		win32_error(){ this->update(); }
		win32_error( win32_error_code dw ) : c_(dw) {}
	private:
		win32_error_code c_;
	};

}
}
	
#endif