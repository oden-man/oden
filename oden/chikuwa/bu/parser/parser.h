#pragma once
#ifndef ODEN_CHKW_BU_PSER_PSER
#define ODEN_CHKW_BU_PSER_PSER

#include <oden/object/container.h>
#include <oden/buf_vector.h>

#include <algorithm>

#include <oden/chikuwa/bu/parser/defs.h>
#include <oden/chikuwa/bu/parser/event.h>
#include <oden/chikuwa/bu/parser/syntax_tree.h>
#include <oden/chikuwa/bu/parser/rule.h>

namespace oden { namespace chikuwa 
{ 
namespace bu
{
//____________________________________________________________________________________________
//
// 				�{  ��	
//____________________________________________________________________________________________
//	
//###############################################################################
//  �X�^�b�N�g�p�R���e�i
//	
	template< typename N, typename J, typename C >
	struct parser_stackT
	{
		template< int n > struct context
		: parser_stackT< N,J, buf_use_array<n> > {};
	
		template< int n > struct node
		: parser_stackT< buf_use_array<n>, J,C > {};
		
		template< int n > struct joint
		: parser_stackT< N, buf_use_array<n>, C > {}; 
		
		template< int n, int m = n > struct tree
		: parser_stackT< buf_use_array<n>, buf_use_array<m>, C > {};
		
		typedef N node_t;
		typedef J joint_t;
		typedef C context_t;
	};
	
  // �f�t�H���g�͂��ׂē��I
	typedef parser_stackT< buf_use_vector, buf_use_vector, buf_use_vector >
	parser_stack;

//###############################################################################
// �C�e���[�^�ړ��N���X
//	
	template< typename Iterator >
	class parser_range
	{
	public:
		parser_range( Iterator beg = Iterator(), Iterator end = Iterator() )
			: beg_(beg), end_(end), cur_(beg), cur_pos_(0)
		{}

	// �����o�֐�
	  // �i��
		void advance( int num )
		{
			std::advance( cur_, num );
			cur_pos_ += num;
		}

	  // �`�F�b�N
		bool empty()const{ return cur_ >= end_; }

	  // �Ԃ�l�`�F�b�N��
		bool advance_processed( int ret )
		{
			if( ret == rule_set_error )
				return false;
			else if( ret == 0 )
				ret = 1;

			this->advance( ret );
			return true;
		}

	// �A�N�Z�T
		Iterator begin()const	{ return beg_; }
		Iterator end()const		{ return end_; }
		Iterator cur()const		{ return cur_; }
		int		 cur_pos()const { return cur_pos_;  }

	private:
		Iterator beg_, end_;
		Iterator cur_;
		int		 cur_pos_;
	};

//###############################################################################
//  �f�t�H���g�̃X�L���i : ���݂̃��[�����Ăяo�����s���邾��
//
	struct default_scanner
	{
		template< typename Parser, typename Range >
		bool scan( Parser& pser, Range& rng )const
		{
			while( !rng.empty() )
			{
				typename Parser::rule_set_type const* pRule = pser.current_rule();
				if( !pRule )
					return false;

				if( !rng.advance_processed( pRule->process( pser, rng.cur() ) ) )
					return false;
			}

			return true;
		}
	};

//###############################################################################
//  �p�[�T�{��
//	
	template< typename Iterator, typename Result, typename Stacks = parser_stack >
	class parser
	{
	public:
		typedef parser 									self;
		typedef Iterator								token_iterator;
		typedef Result									result_data;
		typedef Stacks									stack_types;	
	
		typedef parser_event< self >					event_type;
		typedef syntax_tree< Result, stack_types >		tree_type;

		typedef parser_range<token_iterator>			target_range;
	
		struct context_entry 
		{ 
			int 			id;				  // �R���e�N�X�g���ʎq
			token_iterator 	first;			  // �J�n�ꏊ
			syntax_tree_context tree_cxt;	  // �c���[�̃R���e�L�X�g
		};
		typedef typename stack_types::context_t			context_container;
		typedef buf_vector< context_entry, 
							context_container >			context_stack;	

		typedef rule_set<self>							rule_set_type;
		typedef object_container<rule_set_type>			rule_set_container;
		typedef typename rule_set_container::element	rule_set_ptr;
		typedef std::vector<rule_set_ptr>				rule_set_list;

		typedef buf_vector< rule_set_type* >			rule_set_srack;

	// �����o�֐�
	  // �I�[�ƃA�N�V����
		// �ǉ�
		bool create_terminal( int tid, token_iterator first, token_iterator last )
		{
			if( !tree_.add_node( tid, first, last, event_ ) )
				return false;
			
			event_.raise( event_term_success, first, tid );
			return true;
		}

		// �ǉ�
		bool create_joint( joint_def def, token_iterator itr )
		{
			if( !tree_.add_join_entry( def, target_.cur_pos(), event_ ) )
				return false;
			
			event_.raise( event_join_expect, itr, def.id() );
			return true;
		}

	  // �R���e�L�X�g����
		// �J�n
		bool open_context( int n, token_iterator itr )
		{			
			context_entry dat = { n, itr, tree_.new_context() };
			
			if( !cxt_stack_.push_back(dat) )
			{
				event_.raise( error_stack_overflow, itr );
				return false;
			}
	
			event_.raise( event_context_begin, itr );
			return true;
		}

		// �Ҍ�
		bool close_context( token_iterator itr )
		{
			event_.raise( event_join_reduce, itr );

			// �c���Ă���G���g�������s������
			if( !tree_.invoke_join_entry( event_ ) )
				return false;
				
			// ����
			
			event_.raise( event_context_end, itr );

			// �O��̃R���e�N�X�g�֕��A����
			if( !tree_.restore_context( this->context().tree_cxt ) )
				return false;

			// �R���e�N�X�g���E
			cxt_stack_.pop_back();

			return true;
		}
		
		// �R���e�N�X�g�ɃA�N�Z�X
		context_entry& context() { return cxt_stack_.back(); }
		const context_entry& context()const { return cxt_stack_.back(); }

		bool get_context( int at, context_entry& out )const
		{
			if( at < 0 || cxt_stack_.size() <= at )
				return false;

			out = cxt_stack_.back(at);
			return true;
		}
		
	  // �A�N�Z�T
		// �c���[
		tree_type& tree() { return tree_; }
		const tree_type& tree()const { return tree_; }
		
		// �v�Z�I�u�W�F�N�g
		const result_data& result() { return result_; }
		
		// ����
		const typename result_data::value_type& get_result()const { return result_.get(); }
		
		// �J�n�E�I���ʒu
		token_iterator begin()const { return target_.begin(); }
		token_iterator end()const   { return target_.end(); }
		const target_range& target()const { return target_; }

	  // �C�x���g
		// ���ׂẴC�x���g��񍐂���
		void report_all_event()
		{
			event_level_ = event_level_all;
		}
		void report_only_error()
		{
			event_level_ = event_level_error;
		}
		// �L�^���x�����e�X�g
		bool test_event_level( event_code c )const
		{
			return event_level_ < c;
		}
		// �C�x���g�n���h�����s
		void handle_event( event_type& eve )
		{
			result_.on_event( eve );
		}
		
	  // ���[��
		// �؂�ւ�
		bool switch_rule( const rule_set_type* p )
		{
			cur_rule_ = p;

			event_.raise( event_rule_switched, target_.cur() );
			return true;
		}
		bool switch_rule( int idx )
		{
			if( idx >= rule_libs_.size() ) {
				return false;
			}
			
			this->switch_rule( rule_libs_[idx].get() );
			return true;
		}
		
		// �擾
		const rule_set_type* current_rule()const { return cur_rule_; }

		// �V�K�쐬 + ���[���W�����C�u�����ɒǉ� 
		rule_set_expr<self> create_rule()
		{
			rule_set_type* pNew = new rule_set_type();			
			rule_set_container::push_back( rule_libs_, pNew );
			
			return rule_set_expr<self>( *pNew );
		}

	  // �p�[�T�N��		
		// ����
		template< typename Iterator, typename Scanner >
		bool run( Iterator sfirst, Iterator slast, const Scanner& scanner )
		{
			target_ = target_range( sfirst, slast );
			
			// �`�F�b�N
			if( target_.empty() )
				return true;
				
			if( rule_libs_.empty() )
				return true;
		
			// �N���A
			result_.clear();
			tree_.clear();
			cxt_stack_.clear();
			
			// �ŏ��̃��[���I��
			if( cur_rule_ == 0 ) 
				cur_rule_ = rule_libs_.front().get();
		
			// �R���e�N�X�g�J�n
			this->open_context( default_context_id, target_.begin() );

			// �X�L����
			if( !scanner.scan( *this, target_ ) )
				return false;

			// �X�L�����I����
			token_iterator itr = target_.end();
			--itr;
			
			// �c�����R���e�N�X�g��S�ĕ���
			if( cxt_stack_.empty() )
			{
				event_.raise( error_main_context_closed, itr );
				return false;			
			}
			while( cxt_stack_.size() > 1 )
			{
				event_.raise( error_context_not_closed, itr );
				
				if( !this->close_context( itr ) )
					return false;
			}
			
			// ���C���R���e�N�X�g
			if( !this->close_context( itr ) )
				return false;			

			return true;
		}
		
		// �f�t�H���g�X�L���i���g�p
		template< typename Iterator >
		bool run( Iterator itr, Iterator eitr )
		{
			default_scanner sa;
			return this->run( itr, eitr, sa );
		}
		
	  // �R���X�g���N�^
		parser( const Result& res = Result() )
			: result_(res), tree_(result_), event_(this), event_level_(event_level_error), cur_rule_(0)
		{}
		
	private:
		result_data		result_;

		tree_type		tree_;	
		context_stack   cxt_stack_;
		
		target_range	target_;

		event_type		event_;
		int				event_level_;

		rule_set_list			rule_libs_;
		const rule_set_type*	cur_rule_;
	}; 
	
//#######################################################
//  �Z�}���e�B�b�N�A�N�V����
//	
	template< typename T >
	class semantic_action
	{
	public:
		typedef T value_type;
		
		semantic_action()
			: val_()
		{}
		
	  // �c���[�\�z
		const T& val()const { return val_; }
		T& val() { return val_; }	
		
		void clear(){ val_ = T(); }
		
		const T& get()const { return val_; }

	  // �G���[�n���h��
		template< typename E >
		void on_event( E& ){}

	private:
		T val_;
	};

} // end of namespace parser
}}
	
#undef BU_RAISE_EVENT

#endif
