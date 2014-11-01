
#ifndef ODEN_GAMMO_SYS_THR_TLS
#define ODEN_GAMMO_SYS_THR_TLS


//####################################################
//  インクルード
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- oden ----------

//
//####################################################


namespace oden { namespace gammo
{
//#############################################################
// TLSインデックス
//	
	typedef nullable< DWORD, singular_null<DWORD, TLS_OUT_OF_INDEXES> > tls_index;

  // 確保
	tls_index allocate_tls();	
	
//#############################################################
// TLSを操作するクラス
//	
	class thread_local_stroage
	{
	public:
	// コンストラクタ
		thread_local_stroage( tls_index idx )
			: index_(idx)
		{}
		
	// デリータ
		typedef struct releaser {
			void operator()( thread_local_stroage& s )const { s.release(); }
		} deleter_type;
		
	// ハンドル
		tls_index index()const { return index_; }
		
		bool fail()const { return index_.is_null(); }
		
		bool release( );
		
	// 値操作
		LONG_PTR value()const;
		bool	 set( LONG_PTR val )const;
		
	private:
		tls_index index_;
	};
}}

#endif