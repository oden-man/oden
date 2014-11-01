#pragma once

#ifndef ODEN_SMP_LINKCNT
#define ODEN_SMP_LINKCNT

namespace oden
{
	
// リンク式カウンタ
//""""""""""""""""""""""""""""""""""
	class link_ref
	{
	public:
		typedef link_ref self;
		
	// コンストラクタ
		link_ref()
			: prev(0), next(0)
		{
			prev = this;
			next = this;
		}
		link_ref(const self& other)
			: prev(&other), next(other.next)
		{
			next->prev = this;
			prev->next = this;
		}
	  // デストラクタ
		~link_ref()
		{
			prev->next = next;   
			next->prev = prev;
		}
		
	  // 代入
	  	self& operator=( const self& other )
		{
			if( &other == this )
				return *this;
			
			if( !alone() )
			{
				prev->next = next;   
				next->prev = prev;
			}

			(next = other.next)->prev = this;
			(prev = &other    )->next = this;
			return *this;
		}
		
	// メンバ関数
	  // 
		void depart()
		{
			prev->next = next;   
			next->prev = prev;
			this->isolate();
		}

	  // 参照者が自分ひとり
		bool alone()const
		{
			return next==this;	
		}
		
	  // リストのノード数を取得
		unsigned long node_count()const
		{
			unsigned long res = 0;
			const self* pnode = this;
			do {
				++res;
				pnode = pnode->next;
			}
			while( pnode != this );
			return res;
		}
		
	  // 入れ替える
		void swap( self& other )
		{
			self* that = &other;
			bool this_isolated  = this->alone();
			bool that_isolated  = that->alone();

			if ( this_isolated )
			{                                // if both isolated, do nothing
				if ( !that_isolated )         // but if only this is:
				{
					next = that->next;
					prev = that->prev;

					next->prev = this;         // adjust back pointers
					prev->next = this;

					that->isolate();
				}
			}
			else if ( that_isolated )        // and this isn't
			{                                
				that->next = next;
				that->prev = prev;
         
				next->prev = that;            // adjust back pointers
				prev->next = that;
				
				this->isolate();
			}
			else                             // neither isolated - full blown swap
			{
				next->prev = that;            // adjust back pointers
				prev->next = that;

				std::swap( next, that->next );
				std::swap( prev, that->prev );

				next->prev = this;            // adjust back pointers
				prev->next = this;
			}
		}
		friend void swap( self& l, self& r )
		{
			l.swap(r);
		}		
	private:
		void isolate()
		{
			next = this;
			prev = this;
		}

		mutable const self* prev;
		mutable const self* next;		
	};
}
	
#endif