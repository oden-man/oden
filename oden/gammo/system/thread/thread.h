
#ifndef ODEN_GAMMO_THRUTIL
#define ODEN_GAMMO_THRUTIL


//####################################################
//  �C���N���[�h
//

// --------- defs -----------
#ifndef ODEN_GOB_DEFS_INCLUDED
	#include <oden/gammo/detail/general/defs.h>
#define ODEN_GOB_DEFS_INCLUDED
#endif

// --------- boost ----------
#ifndef ODEN_BST_SCOPED_PTR_INCLUDED
	#include <boost/scoped_ptr.hpp>
#define ODEN_BST_SCOPED_PTR_INCLUDED
#endif
#ifndef ODEN_BST_THREAD_THREAD_INCLUDED
	#include <boost/thread/thread.hpp>
#define ODEN_BST_THREAD_THREAD_INCLUDED
#endif
#ifndef ODEN_BST_THREAD_MUTEX_INCLUDED
	#include <boost/thread/mutex.hpp>
#define ODEN_BST_THREAD_MUTEX_INCLUDED
#endif
#ifndef ODEN_BST_THREAD_CONDITION_INCLUDED
	#include <boost/thread/condition.hpp>
#define ODEN_BST_THREAD_CONDITION_INCLUDED
#endif

//
//
//####################################################


namespace oden { namespace gammo
{
	
//############################################################
//
//
	class WorkerThread
	{
	public:
		typedef boost::scoped_ptr<boost::thread> thread_ptr;
		
	// ���C���X���b�h�����ĂԊ֐�
		// ��ƊJ�n
		void Launch();
		// ���[�J�[�X���b�h����Ƃ��I����܂ő҂�
		void Join();
		// ��Ƃ̒��f���w��
		void Suspend();
		// ��Ƃ̍ĊJ���w��
		void Resume();
		// ��Ƃ̒��~���w��
		void Abort();
		// �X���b�h���ғ�����
		bool IsRunning()const;
		
	protected:
	// ���[�J�[�X���b�h�����ł�Ԋ֐�
		// ���ۂɃX���b�h���~����
		void CheckSuspend();
		// �X���b�h�{�̂̊֐�
		virtual bool ThreadProc() = 0;
		
	public:		
	// �R���X�g���N�^�E�f�X�g���N�^
		WorkerThread();
		virtual ~WorkerThread(){}
	
	private:	 
	// ���L�ϐ�
		volatile bool 	   isSuspended;	// ��~�����ǂ���
	// �����o�ϐ�
		thread_ptr         thr;			// ���[�J�[�X���b�h�|�C���^
		boost::condition   thr_cond;	// �X���b�h�̏�Ԃ𐧌�	
	protected:
		boost::mutex       mx;			// ���b�N�p�̃~���[�e�b�N�X
	};	
	
}
}

#endif