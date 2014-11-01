#pragma once
#ifndef ODEN_CHKW_BU_STXTREE
#define ODEN_CHKW_BU_STXTREE

#include <algorithm>

#include <oden/buf_vector.h>

#include <oden/chikuwa/bu/parser/defs.h>
#include <oden/chikuwa/bu/parser/joint.h>
#include <oden/chikuwa/bu/parser/event.h>

namespace oden { namespace chikuwa 
{ 
namespace bu
{	
//#######################################################
//  �\����
// 
/*
	node_index �͈���炷�B
	joint_index �͂��̂܂܁B
*/

  // �R���e�N�X�g�ێ�
	struct syntax_tree_context
	{
		node_index  	first;				// �J�n�m�[�h�ʒu
		node_index      ahead;				// �W���C���g���v������m�[�h�ʒu
		int 			joint_stack_pos;	// �J�n���̃X�^�b�N�g�b�v�ʒu
		int 			lowest_entry; 		// �ŉ��ʂ̃G���g��
	};
		
  // �c���[
	template< typename Result, typename Stacks >
	class syntax_tree
	{
	public:
		typedef syntax_tree<Result,Stacks>					self;
		
		typedef Result										result_data;
		typedef typename result_data::value_type			data_type;
			
		typedef Stacks										stack_types;
			
		typedef joint_entry						 			entry_type;
		typedef typename Stacks::joint_t					entry_container;	
		typedef buf_vector< entry_type, entry_container > 	entry_list;
		
		struct node
		{
			int        sibling_offs;
			data_type  data;
		};
		typedef typename Stacks::node_t						node_container;
		typedef buf_vector< node, node_container >			node_map;
		
	// �����o�֐�	
	  // �\������		
		// �t
		template< typename Iterator, typename Event >
		bool add_node( int term_id, Iterator first, Iterator last, Event& event )
		{
			PROFILE_BLOCK( add_terminal );

			// �f�[�^���v�Z
			result_->on_term( term_id, first, last );

			// �m�[�h���쐬
			node ne = { 0, result_->get() };		

			if( !nodes_.push_back(ne) )
			{
				event.raise( error_stack_overflow, first, term_id );
				return false;
			}
			
			return true;
		}
		
		// �Ҍ�
		bool join_node( int term_id, node_index first_hint, node_index tail_hint )
		{
			PROFILE_BLOCK( add_joint );			
			assert( first_hint <= tail_hint && 0 <= first_hint && tail_hint < (unsigned int)nodes_.size() );
			
			node_index head = this->node_siblings_head(first_hint);
			node_index tail = this->node_siblings_tail(tail_hint);

			// ���ԂɃf�[�^�����o��
			int child_cnt = 0;
			for( unsigned int i = head; i<=tail; ++i )
			{
				data_type& dat = nodes_[i].data;
				
				// �e�ɓo�^
				++child_cnt;
				result_->on_node_child( term_id, dat, child_cnt );
				
				// �u���b�N������
				i = this->node_siblings_tail(i);
			}
			
			// �Ō�̏���������΂�����
			result_->on_node( term_id );

			// �f�[�^���ł����̃m�[�h�ɕۑ�
			nodes_[head].data = result_->get();
			
			// �m�[�h������
			promoted_index_t sibling_dist = tail - head; 
			if( sibling_dist > 0 )
			{
				nodes_[head].sibling_offs = sibling_dist;
				nodes_[tail].sibling_offs = -static_cast<int>( sibling_dist );
			}
			
			return true;
		}

	  // �m�[�h����		
		// �m�[�h�����݂��邩
		bool node_exists( node_index i )const
		{
			if( (unsigned int)nodes_.size() <= i )
				return false;
			return true;		
		}
		
	    // �A���u���b�N�̖������擾
		node_index node_siblings_tail( node_index i )const
		{
			assert( this->node_exists(i) );
			
			if( nodes_[i].sibling_offs <= 0 )
				return i;
			
			return static_cast<node_index>( i + nodes_[i].sibling_offs );
		}
		
	    // �A���u���b�N�̐擪���擾
		node_index node_siblings_head( node_index i )const
		{
			assert( this->node_exists(i) );
			
			if( nodes_[i].sibling_offs >= 0 )
				return i;
			
			return static_cast<node_index>( i + nodes_[i].sibling_offs );
		}
		
	  // �m�[�h�̏�Ԃ𕶎���
		template< typename Ch >
		std::basic_string<Ch> debug_display( const Ch (&chars)[4] )const
		{
			std::basic_string<Ch> disp;			
			return disp;
		}
		
	  // �\�z���̃c���[���E���ɏI�[��v�����Ă��邩
		bool require_right_term()const
		{
			if( nodes_.empty() ) 
				return true;   // �I�[���Ȃ�

			if( entries_.empty() )
				return false;  // �I�[�݂̂�����
				
			const entry_type& ent = entries_.back();
			promoted_index_t last_node = static_cast<promoted_index_t>(nodes_.size() - 1);

			if( ent.node() <= last_node )
				return false;  // ���O���I�[

			if( ent.node_ahead() > last_node )
				return true;   // �m�[�h������Ȃ�
			
			if( cxt_.ahead > last_node )
				return true;   // �m�[�h���܂�����Ȃ�
			
			return false;			
		}
		
	  // �G���g���̒ǉ�
		template< typename Event >
		bool add_join_entry( joint_def def, std::size_t place, Event& event )
		{
			// �G���g���쐬
			promoted_index_t ni = nodes_.size();  // �E���m�[�h�̃C���f�b�N�X���̂�
			entry_type ent( def, static_cast<node_index>(ni), place ); // higher�̂��߂ɂ�����node index����x�ݒ肷��
			
			// �C���f�b�N�X�I�[�o�[�t���[�𒲂ׂ�
			if( ni >= ODEN_CHIKUWA_BU_INDEX_MAX )
			{
				event.raise( error_index_overflow, ent );
				return false;
			}
			
			// ������open/closed ���m�F
			int left_num = ent.look_back();
			if( ni - left_num < 0 || this->require_right_term() == (left_num!=0) )
			{
				//! �X�L�b�v
				event.raise( event_join_skip, ent );
				return false;
			}
			
			// ���̃G���g�����ł��Ⴂ�D�揇�ʂȂ�A�O���̃G���g�����Ҍ�
			if( cxt_.lowest_entry == -1 )
			{
				cxt_.lowest_entry = entries_.size();
			}
			else
			{
				const entry_type& lowest_entry = entries_[cxt_.lowest_entry];

				if( !ent.higher( lowest_entry ) )
				{
					if( cxt_.ahead <= ni ) // �O���̃m�[�h��������Ă邩
					{
						event.raise( event_join_reduce, ent );

						if( !this->invoke_join_entry( event ) )
							return false;
					}

					// �X�y�[�X�J����̃m�[�h�C���f�b�N�X���Đݒ�
					ni = nodes_.size();
					ent.set_node( static_cast<node_index>(ni) );

					cxt_.lowest_entry = entries_.size();
				}
			}
			
			// �ǉ�
			if( !entries_.push_back( ent ) )
			{
				event.raise( error_stack_overflow, ent );
				return false;
			}
			
			// �K�v�m�[�h���̍X�V
			node_index ahead_node = ent.node_ahead();
			if( cxt_.ahead < ahead_node ) {
				cxt_.ahead = ahead_node;
			}

			return true;
		}
		
	  // �G���g���̎��s
		template< typename Event >
		bool invoke_join_entry( Event& event )
		{			
			entry_list::iterator jitr  = entries_.begin() + ( cxt_.joint_stack_pos + 1 );
			entry_list::iterator jeitr = entries_.end();
			
			if( jitr == jeitr ) { // �G���g������������
				return true;
			}
			
			// �R���e�N�X�g�͈͂̃G���g����D�揇�ʂŃ\�[�g
			std::sort( jitr, jeitr, []( const entry_type& l, const entry_type& r ){ return l.higher(r); } );
			
			// ���̕����̂݌���
			for(; jitr != jeitr; ++jitr )
			{
				// �����͈͂��擾
				promoted_index_t first = 0, tail = 0;
				if( !jitr->get_target( cxt_.first, first, tail ) ) 
					break;

				// ���݂���m�[�h��?
				assert( first <= tail );
				if( static_cast<promoted_index_t>(nodes_.size()) <= tail )
					break;

				// �m�[�h������
				if( !this->join_node( jitr->def_id(), static_cast<node_index>(first), static_cast<node_index>(tail) ) )
					break;
			}

			// ���s�`�F�b�N
			if( jitr != jeitr )
			{
				//! �G���[ : �G���g�����e����������				
				event.raise( error_invalid_joint, *jitr );
				return false;
			}

			/* �g��Ȃ��Ȃ����̈���J������ */
			
			// �m�[�h
			this->cleanup_nodes();
			
			// �G���g��
			entries_.set_back( cxt_.joint_stack_pos );
			
			return true;
		}
		
	  // �m�[�h���܂Ƃ߂ė̈���ė��p
		void cleanup_nodes()
		{
			if( nodes_.empty() )
				return;

			node_index       first = cxt_.first;
			promoted_index_t last  = nodes_.size();
			promoted_index_t back  = first;
			for( promoted_index_t i = first; i < last; ++i )
			{
				node_index ta = this->node_siblings_tail(i);
				
				if( ta == i )
					continue;
				
				if( i != first ) 
				{
					back += 1;					
					nodes_[back].data = nodes_[i].data;
				}
				
				nodes_[back].sibling_offs = 0;
				
				i = ta;
			}
			
			nodes_.set_back( back );
		}
		
	  // �R���e�N�X�g
		// �ۑ�
		syntax_tree_context new_context()
		{
			syntax_tree_context ocxt = cxt_;
			syntax_tree_context ncxt = { nodes_.size(), nodes_.size(), entries_.size() - 1, -1 };

			cxt_ = ncxt;

			return ocxt;
		}
		
		// ��Ԃ𕜋A
		bool restore_context( const syntax_tree_context& oldcxt )
		{			
			cxt_ = oldcxt;	
			return true;
		}

		// �擾
		node_index context_first_node()const { return cxt_.first; }
		
	  // �N���A
		void clear()
		{
			nodes_.clear();	
			entries_.clear();
			
			syntax_tree_context nullcxt = {0};
			cxt_ = nullcxt;
		}
		
	// �R���X�g���N�^
		syntax_tree( result_data& ru )
			: result_(&ru)
		{}

	private:	
		node_map	 			nodes_;
		entry_list	 			entries_;
		
		syntax_tree_context		cxt_;
		
		result_data*			result_;
	};

} // end of namespace parser
}}

#endif
