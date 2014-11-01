#ifndef ODEN_CHIKUWA_TMPSTR
#define ODEN_CHIKUWA_TMPSTR

#include <algorithm>
#include <oden/chikuwa/string_ptr.h>

namespace oden { namespace chikuwa
{	
//##################################################
// 
//	
	template< typename T = char, int Cap = 32 >
	class temp_string
	{
	public:
		typedef T 				value_type;
		typedef T* 				pointer;
		typedef const T* 		const_pointer;
		typedef pointer 		iterator;
		typedef const_pointer 	const_iterator;
	
		template< typename Iter >
		bool allocate( Iter itr, Iter eitr )
		{
			int nsz = std::distance( itr, eitr );
			if( nsz > Cap )
				return false;
			
			std::copy( itr, eitr, buf_ );
			
			// ヌル文字のチェック
			if( buf_[nsz-1] != value_type(0) )
			{
				if( nsz+1 > Cap )
					return false;
					
				buf_[nsz] = value_type(0);
				++nsz;
			}
			
			size_ = nsz;	
			return true;			
		}
		
		template< typename Iter >
		bool allocate( Iter itr, Iter eitr )
		{
			
			
		}
		
		iterator begin() { return buf_; }
		const_iterator begin()const { return buf_; }
		
		iterator end() { return buf_+size_; }
		const_iterator end()const { return buf_+size_; }
		
		pointer get() { return buf_; }
		const_pointer get()const { return buf_; }
		
		const_pointer c_str()const { return buf_; }
		int size()const { return size_; }		
		
	private:
		int size_;
		T buf_[Cap];
	};
	
}}

#endif

