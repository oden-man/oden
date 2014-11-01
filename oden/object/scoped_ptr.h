
#ifndef ODEN_SMP_SCOPED_PTR
#define ODEN_SMP_SCOPED_PTR

#include <ostream>
#include <functional>

#include <oden/object/deleter.h>

namespace oden
{
//################################################################
//
//
	template< class T, class D = pointer_delete<T> >
	class scoped_ptr
	{
	public:
		typedef scoped_ptr<T,U> self_type;
		typedef T				element_type;
		typedef D				deleter_type;
	//
		scoped_ptr():ptr(NULL),deler()
		{
		}
		explicit scoped_ptr(D deleter):ptr(NULL),deler(deleter)
		{
		}
		explicit scoped_ptr(T* p):ptr(p),deler()
		{
		}
		scoped_ptr(T* p,D deleter):ptr(p),deler(deleter)
		{
		}
	// 
		~scoped_ptr()
		{
			if(ptr!=NULL)
				deler(ptr);
		}
	//
		void swap(self_type& other)
		{
			std::swap(this->deler,other.deler);
			std::swap(this->ptr,other.ptr);
		}
		
		void reset()
		{
			self_type().swap(*this);
		}
		void reset(T* p)
		{
			self_type(p).swap(*this);
		}
		void reset(T* p,D deleter)
		{
			self_type(p,deleter).swap(*this);
		}
	
	  // 参照
	  	T& operator*()const
	  	{
	  		return *ptr;	
	  	}
	  	T* operator->()const
	  	{
	  		return ptr;	
	  	} 
		T* get()const
		{
			return ptr;
		}
		
		D& get_deleter() { return deler; }
		const D& get_deleter()const { return deler;}
		
	// コピー禁止	
	private:
		scoped_ptr(const self_type& src);
		operator = (const self_type& src);
		
	//	
		T* ptr;
		D  deler;
	};
}
	
  // 比較演算子
	template<class T, class D, class U, class E>
	inline bool operator==(const scoped_ptr<T,D>& l, const scoped_ptr<U,E>& r)
	{
		return l.get() == r.get();
	}
	template<class T, class D, class U, class E>
	inline bool operator!=(const scoped_ptr<T,D>& l, const scoped_ptr<U,E>& r)
	{
		return l.get() != r.get();
	}
	template<class T, class D, class E>
	inline bool operator<(const scoped_ptr<T,D>& l, const scoped_ptr<T,E>& r)
	{
		return std::less<T*>( l.get(), r.get() );
	}
	template<class T, class D, class U, class E>
	inline bool operator<(const scoped_ptr<T,D>& l, const scoped_ptr<U,E>& r)
	{
		return l.get() < r.get();
	}
	
  // グローバル swap
  	template<class T,class D>
  	inline void swap(scoped_ptr<T,D>& l, scoped_ptr<T,D>& r)
  	{
  		l.swap(r);	
  	}
  	
  // ストリーム出力 operator <<
	template<class E,class T,class Y,class D>
	inline std::basic_ostream<E,T>& operator<< (std::basic_ostream<E,T>& os,const scoped_ptr<Y,D>& src)
	{
		os<<src.get();
		return os;	
	}
	
}

#endif