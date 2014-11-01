#ifndef ODEN_OBJ_DEL
#define ODEN_OBJ_DEL

#include <boost/checked_delete.hpp>
//#include <boost/optional.hpp>

namespace oden
{	
//###########################################################################
//  �Ȃɂ����܂���
//	
	struct null_deleter
	{
		template< typename T >
		void operator()(T&){}
	};
	
//###########################################################################
//  �I�u�W�F�N�g����f���[�^�����o��
//	
	template< typename T = void >
	struct inner_deleter
		: T::deleter_type
	{};
	
	template<>
	struct inner_deleter<void>
	{
		template< typename T >
		void operator()( T& o )
		{
			typename T::deleter_type deler;
			deler( o );
		}
	};

//###########################################################################
//  �C�ӂ̃f���[�^
//	
	template< typename T, typename Ret = void >
	struct unary_function_deleter
	{
		typedef Ret (*delete_func_t)(T);
		
		void operator()( const T& r )
		{
			pfn(r);
		}
	private:
		unary_function_deleter( delete_func_t pDeleter )
			: pfn(pDeleter)
		{}
		
		delete_func_t pfn;
	};

//###########################################################################
//  �R���e�i�Ȃǂɑ΂��A�S�̂�Deleter��K�p
//	
	template< typename Deleter >
	struct deleter_foreach
	{
		template< typename Cont >
		void operator()( Cont& cont )
		{
			typename Cont::iterator itr = cont.begin(), eitr = cont.end();
			for(; itr!=eitr; ++itr)
				deler_(*itr);
		}
		
		deleter_foreach()
			: deler_()
		{}
		deleter_foreach( const Deleter& deler )
			: deler_(deler)
		{}
		
	private:
		Deleter deler_;
	};
	
	template< typename T = void >
	struct inner_deleter_foreach
		: deleter_foreach< inner_deleter<T> >
	{};
	
//###########################################################################
//  �R���e�i�Ȃǂɑ΂��A�S�̂�Deleter��K�p
//	
	struct any_deleter
	{
		template< typename T >
		struct the_holder
		{
			template< typename X >
			void operator()( X& x )
			{
				instance_( x );
			}
			
			T instance_;
		};
	};
/*
	// 
	template<class D>
	struct lazy_init_deleter
	{
		lazy_init_deleter():deler(){}
		
		template<class U>
		void operator()(const U& p)
		{
			if(deler)
				(deler.get())(p);
		}
	// ������
		template<class E>
		void construct(const E& expr)
		{
			deler = expr;	
		}
	private:
		boost::optional<D> deler;
	};
	
	// �؂�ւ����ł���deleter
	template<class Deleter1, class Deleter2>
	struct dual_deleter
	{
		typedef Deleter1	deleter1;
		typedef Deleter2	deleter2;
		typedef dual_deleter<deleter1,deleter2> self_type;
		
		template<class T>
		void operator()(T& src) {
			if(use1_)
				deler1_(src);
			else	
				deler2_(src);
		}
		
		void activate_first()	{ use1_ = true; }
		void activate_second()	{ use1_ = false; }
		void switching()		{ use1_ = !use1_; }
				
		self_type(const deleter1& d1 = deleter1(), const deleter2& d2 = deleter2())
			: deler1_(d1), deler2_(d2), use1_(true)
		{}
	private:
		bool	 use1_;
		deleter1 deler1_;
		deleter2 deler2_;			
	};
	*/
}




#endif