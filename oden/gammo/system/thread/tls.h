
#ifndef ODEN_GAMMO_SYS_THR_TLS
#define ODEN_GAMMO_SYS_THR_TLS


//####################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- oden ----------

//
//####################################################


namespace oden { namespace gammo
{
//#############################################################
// TLS�C���f�b�N�X
//	
	typedef nullable< DWORD, singular_null<DWORD, TLS_OUT_OF_INDEXES> > tls_index;

  // �m��
	tls_index allocate_tls();	
	
//#############################################################
// TLS�𑀍삷��N���X
//	
	class thread_local_stroage
	{
	public:
	// �R���X�g���N�^
		thread_local_stroage( tls_index idx )
			: index_(idx)
		{}
		
	// �f���[�^
		typedef struct releaser {
			void operator()( thread_local_stroage& s )const { s.release(); }
		} deleter_type;
		
	// �n���h��
		tls_index index()const { return index_; }
		
		bool fail()const { return index_.is_null(); }
		
		bool release( );
		
	// �l����
		LONG_PTR value()const;
		bool	 set( LONG_PTR val )const;
		
	private:
		tls_index index_;
	};
}}

#endif