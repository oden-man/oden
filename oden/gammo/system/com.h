#pragma once

#ifndef ODEN_GAMMO_SYS_COMBASE
#define ODEN_GAMMO_SYS_COMBASE


//#############################################
//  インクルード
//
//-------- defs ---------
#include <oden/gammo/defs.h>
//-------- vc -----------
#include <comdef.h>
#include <comutil.h>
//-------- win ----------
#ifndef STRICT_TYPED_ITEMIDS
	#define STRICT_TYPED_ITEMIDS
#endif
#include <ObjBase.h>
//-------- boost --------
//#include <boost/smart_ptr/intrusive_ptr.hpp>
//-------- oden ---------
//-------- gammo --------

//
//#############################################


namespace oden { namespace gammo
{
//########################################
// typedef 
//


//########################################
// 自作()スマートポインタ
//
	template< typename T >
	class com_ptr
	{
	public:
		typedef T 			element_type;
		typedef T* 			pointer;
		typedef const T* 	const_pointer;
		typedef T& 			reference;
		typedef const T& 	const_reference;
		
		com_ptr()
			: p_(0)
		{}
		com_ptr( boost::none_t )
			: p_(0)
		{}
		com_ptr( const int )
			: p_(0)
		{}
		explicit com_ptr( pointer p, bool bAddref = true )
			: p_(p)
		{
			if( bAddref ) add_ref();
		}
		com_ptr( const com_ptr<T>& other )
			: p_(other.p_)
		{
			add_ref();
		}
		template< typename U >
		com_ptr( const com_ptr<U>& other )
			: p_(other.p_)
		{
			add_ref();
		}
		~com_ptr()
		{
			release();
		}

		int add_ref()
		{ 
			if( p_ ) 
				return p_->AddRef(); 
			return 0;
		}		
		int release()
		{ 
			if( p_ ) 
				return p_->Release();
			return 0;
		}

		void reset()
		{ com_ptr().swap(*this); }
		void reset( pointer p, bool addref = true )
		{ com_ptr(p, addref).swap(*this); }

		pointer get()const { return p_; }
		static const IID& iid() { return __uuidof(T); }
		
		pointer* to_source()
		{
			reset();
			return &p_; 
		}

		void** to_any_source()
		{
			return reinterpret_cast<void**>( to_source() ); 
		}
		
		void swap( com_ptr& other )
		{
			std::swap( p_, other.p_ );
		}
		friend void swap( com_ptr& l, com_ptr& r )
		{
			l.swap(r);
		}

	// 演算子
		com_ptr& operator= ( const com_ptr& other )
		{
			com_ptr(other).swap(*this);
			return *this;
		}
		template< typename U >
		com_ptr& operator= ( com_ptr<U>& other )
		{
			com_ptr(other).swap(*this);
			return *this;
		}

		reference operator* (){ return *p_; }
		const_reference operator* ()const { return *p_; }

		pointer operator-> (){ return p_; }
		const_pointer operator-> ()const { return p_; }

	private:
		typedef void (com_ptr::*unspecified_bool_t)()const;
		void unspecified_true()const{}
	public:
		operator unspecified_bool_t()const { return p_!=0 ? &com_ptr::unspecified_true : 0; }

	private:
		pointer p_;
	};

//########################################
// 自動初期化
//
	class com
	{
	public:
		com()
		{
			ret_ = ::CoInitialize(NULL);
		}
		explicit com( DWORD option )
		{
			ret_ = ::CoInitializeEx(NULL, option);
		}

		typedef struct uninitializer {
			void operator()( com& o )
			{
				if( o.ret_.is_valid() ){ ::CoUninitialize(); }
			}
		} deleter_type;

	private:
		hresult ret_;
	};
	
//########################################
// スマートポインタに橋渡し
//
	template< typename T = void >
	class com_sp_adapter
	{
	public:
		com_sp_adapter()
			: ip_(0)
		{}

		T** to_source(){ return &ip_; }
		void** to_any_source(){ return reinterpret_cast<void**>(&ip_); }
		static const IID& iid() { return __uuidof(T); }

		T* get()const{ return ip_; }

		template< typename Out >
		bool construct( Out &out )
		{
			if( ip_ ) 
			{
				Out( ip_, false ).swap( out );
				return true;
			}
			return false;
		}

	private:
		T* ip_;
	};

	template<>
	class com_sp_adapter<void>
	{
	public:
		com_sp_adapter( const IID& iid )
			: ip_(0), piid_(&iid)
		{}

		void** to_source(){ return &ip_; }
		void** to_any_source(){ return &ip_; }
		const IID& iid()const { return *piid_; }
		void* get()const{ return ip_; }

		template< typename Out >
		bool construct( Out &out )
		{
			if( ip_ ) 
			{
				typedef typename Out::element_type target_type;
				target_type* ip2 = static_cast< target_type* >( ip_ );

				Out( ip2, false ).swap( out );
				return true;
			}
			return false;
		}

	private:
		void* ip_; 
		const IID* piid_;
	};

/*
	com::sp_adapter<> pap( IID_AAAA );
	if( hresult_error hr = ps->QueryInterface( pap.iid(), pap.to_source() ) ) 
		// error

	boost::intrusive_ptr<IHoe> phoe;
	pap.construct( phoe );

	---------------------------------------------

	com::sp_adapter<IFoe> adapter;
	SHget( CLSID, MUKO, IID, IID_ARGV_PP(adapter) );

	boost::intrusive_ptr<IFoe> phoe;
	adapter.construct( phoe );

	---------------------------------------------

	com::sp_adapter<IFoe> adapter;
	if( hresult_error hr = query_interface( ps, adapter ) )

	boost::intrusive_ptr<IFoe> phoe;
	adapter.construct( phoe );
	
*/

//########################################
// pointer, IID
//
  // QueryInterface
	template< typename Source, typename Dest > 
	hresult query_interface( Source& ps, Dest& out )
	{
		hresult err;
		if( ps ) {
			err = ps->QueryInterface( out.iid(), out.to_any_source() );
		}
		else {
			err = E_POINTER;
		}
		return err;
	}

  // CoCreateInstance
	template< typename Dest > 
	hresult create_instance( const CLSID& creator_clsid, DWORD cls_context, Dest& out, LPUNKNOWN pUnkOuter = 0 )
	{
		return ::CoCreateInstance( creator_clsid, pUnkOuter, cls_context, out.iid(), out.to_any_source() );
	}

//#############################################
// COMのデフォルトアロケータで確保されたメモリ
//
	class com_memory_deleter
	{
	public:
		void operator()( void* o )
		{ 
			CoTaskMemFree(o);
		}
	};

	template< typename T >
	class com_memory
	{
	public:
		com_memory() : p_(0)
		{}
		explicit com_memory( int cb ) : p_(0)
		{
			this->allocate(cb);
		}

		typedef com_memory_deleter deleter_type;
	
		void allocate( int cb = sizeof(T) )
		{
			this->free();
			p_ = CoTaskMemAlloc( cb );
		}

		void reallocate( int cb )
		{
			p_ = CoTaskMemRealloc( p_, cb );
		}
		
		void free()
		{
			deleter_type()(p_);
			p_ = 0;
		}

		T* get()const { return p_; }
		operator T*()const { return get(); }
		operator const T*()const { return get(); }

		T** to_source()
		{
			this->free();
			return &p_;
		}
		
		void swap( com_memory<T>& r ) 
		{
			std::swap( p_, r.p_ );
		}
		friend void swap( com_memory<T>& l, com_memory<T>& r ) 
		{ 
			l.swap(r); 
		}
	private:
		T* p_;
	};

	/*
	gammo::scoped< com::task_mem<WCHAR> > flname;
	pShlItm->GetDisplayName( flname->to_source() );
	flname->to_source() : WCHAR**
	flname.to_source()  : com::task_mem<WCHAR> &

	gammo::scoped< WCHAR*, com::task_mem_deleter > flname;
	pShlItm->GetDisplayName( &flname.to_source() );
	flname.to_source()  : WCHAR* &
	*/
	

//#############################################
// IMalloc -> std::allocator アダプタ?
//
	class com_memory_allocator;

	//template< typename Interface >
	//class allocator;

//########################################
// com インターフェースを扱うクラス
//
	template< typename I >
	class com_wrapper
	{
	public:
		typedef I interface_type;
		typedef com_ptr<I> s_pointer;

		com_wrapper():pi_(){}
		
	// メンバ関数
	  // ポインタを操作
		const s_pointer& ptr(){ return pi_; }
		s_pointer& sp(){ return pi_; }

		I* get()const { return pi_.get(); }

		bool fail()const{ return !pi_; }
		
		I** to_source(){ return pi_.to_source(); }
		void** to_any_source(){ return pi_.to_any_source(); }
		
	  // インターフェース操作
		// カウンタ
		int add_ref(){ return pi_->add_ref(); }
		int release(){ return pi_->release(); }
		// 問い合わせ
		template< typename Out >
		hresult query_interface( Out &out )
		{
			return com::query_interface( pi_, out );
		}
		// 構築
		hresult create_instance( const CLSID& creator_clsid, DWORD cls_context = CLSCTX_ALL, LPUNKNOWN pUnkOuter = 0 )
		{
			return com::create_instance( creator_clsid, cls_context, pi_, pUnkOuter );
		}
		
	private:
		com_ptr<I> pi_;
	};

}}

#endif
