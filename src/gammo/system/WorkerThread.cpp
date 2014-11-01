
#include "../Pch.h"
#include <boost/thread/locks.hpp>
#include <oden/gammo/ThreadUtil.h>

//############################################################
// WorkerThread
//

//
//""""""""""""""""""""""""""""""""""""""""
gammo::WorkerThread::WorkerThread():isSuspended(false)
{
}

//
//""""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::Launch()
{
	thr.reset(new boost::thread(boost::bind(&WorkerThread::ThreadProc,this)));	
}

//
//""""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::Join()
{
	if(thr)
	{
		thr->join();  // �X���b�h���s�I����҂�
		thr.reset();  // �X���b�h�I�u�W�F�N�g�̔j��
	}
}

// ��ƈꎞ��~���w��
//"""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::Suspend()
{
	if(thr)
	{
		boost::mutex::scoped_lock(mx);
		isSuspended = true;
	}
}

//
//"""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::Resume()
{
	boost::mutex::scoped_lock(mx);
	if(thr && isSuspended==true)
	{
		thr_cond.notify_one();
		isSuspended = false;
	}
}

// ��Ƃ��ꎞ��~
//"""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::CheckSuspend()
{
	boost::unique_lock<boost::mutex> lk(mx);
	if(isSuspended)
	{
		// �ҋ@�t���O������Ȃ�҂�
		DebugString(_T(" --- �ҋ@���܂�\n"));
		thr_cond.wait(lk);
		DebugString(_T(" --- �ҋ@�𔲂��܂���\n"));
	}

}

//
//"""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::Abort()
{
	if(thr)
	{
		thr->interrupt();
		thr->detach();
		thr.reset();	   // �X���b�h�I�u�W�F�N�g��j��
	}	
}

//
//"""""""""""""""""""""""""""""""""""""""
bool gammo::WorkerThread::IsRunning()const
{
	return static_cast<bool>(thr);	
}