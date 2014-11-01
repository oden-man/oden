#pragma once

#ifndef ODEN_SMP_LINKCNT
#define ODEN_SMP_LINKCNT

namespace oden
{
	
// �����N���J�E���^
//""""""""""""""""""""""""""""""""""
	class link_ref
	{
	public:
		typedef link_ref self;
		
	// �R���X�g���N�^
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
	  // �f�X�g���N�^
		~link_ref()
		{
			prev->next = next;   
			next->prev = prev;
		}
		
	  // ���
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
		
	// �����o�֐�
	  // 
		void depart()
		{
			prev->next = next;   
			next->prev = prev;
			this->isolate();
		}

	  // �Q�Ǝ҂������ЂƂ�
		bool alone()const
		{
			return next==this;	
		}
		
	  // ���X�g�̃m�[�h�����擾
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
		
	  // ����ւ���
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