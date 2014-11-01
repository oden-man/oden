
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
		thr->join();  // スレッド実行終了を待つ
		thr.reset();  // スレッドオブジェクトの破棄
	}
}

// 作業一時停止を指示
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

// 作業を一時停止
//"""""""""""""""""""""""""""""""""""""""
void gammo::WorkerThread::CheckSuspend()
{
	boost::unique_lock<boost::mutex> lk(mx);
	if(isSuspended)
	{
		// 待機フラグがあるなら待つ
		DebugString(_T(" --- 待機します\n"));
		thr_cond.wait(lk);
		DebugString(_T(" --- 待機を抜けました\n"));
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
		thr.reset();	   // スレッドオブジェクトを破棄
	}	
}

//
//"""""""""""""""""""""""""""""""""""""""
bool gammo::WorkerThread::IsRunning()const
{
	return static_cast<bool>(thr);	
}