
#ifndef ODEN_GAMMO_THRUTIL
#define ODEN_GAMMO_THRUTIL


//####################################################
//  インクルード
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
		
	// メインスレッド側が呼ぶ関数
		// 作業開始
		void Launch();
		// ワーカースレッドが作業を終えるまで待つ
		void Join();
		// 作業の中断を指示
		void Suspend();
		// 作業の再開を指示
		void Resume();
		// 作業の中止を指示
		void Abort();
		// スレッドが稼働中か
		bool IsRunning()const;
		
	protected:
	// ワーカースレッド内部でよぶ関数
		// 実際にスレッドを停止する
		void CheckSuspend();
		// スレッド本体の関数
		virtual bool ThreadProc() = 0;
		
	public:		
	// コンストラクタ・デストラクタ
		WorkerThread();
		virtual ~WorkerThread(){}
	
	private:	 
	// 共有変数
		volatile bool 	   isSuspended;	// 停止中かどうか
	// メンバ変数
		thread_ptr         thr;			// ワーカースレッドポインタ
		boost::condition   thr_cond;	// スレッドの状態を制御	
	protected:
		boost::mutex       mx;			// ロック用のミューテックス
	};	
	
}
}

#endif