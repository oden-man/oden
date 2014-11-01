#pragma once
#ifndef ODEN_CHKW_BU_NODE
#define ODEN_CHKW_BU_NODE

#include <vector>
#include <string>
#include <algorithm>
#include <ostream>
#include <oden/object/linked_ptr.h>
//#include <boost/shared_ptr.hpp>

#include <oden/chikuwa/bu/parser.h>
#include <oden/gammo/common/string/conv.h>

namespace oden { namespace chikuwa
{
namespace bu
{	
//#######################################################
//  �\���c���[
//	
	template< typename CharT >
	class node
	{
	public:
		typedef node<CharT>						self;	
		typedef CharT 							char_type;
		typedef std::basic_string<CharT>		string_type;
		typedef oden::linked_ptr< self >		smart_ptr;

	// �R���X�g���N�^
		explicit node( int id )
			: id_(id), content_()
		{}
		template< typename Iter >
		node( int id, Iter f, Iter l )
			: id_(id), content_(f,l)
		{}

	// �����o�֐�
	  // �q��ǉ�
		void add( const smart_ptr& p ){ child_.push_back(p); }
	  // �f�[�^
		int id()const { return id_; }
		const string_type& content()const { return content_; }
		
	  // �X�g���[���ɕ\��
		template< typename Os >
		struct shower 
		{ 
			Os* pos; int depth; 
			template< typename X > void operator()( const X& chi )const { chi->show(*pos, depth); } 
		};
		void show( std::ostream& os, int dep = 0 )const
		{			
			for( int i = 0; i<dep-1; ++i ){	os << "  "; }
			
			if( dep > 0 ) 
				os << "��";

			os << "[" << id_;
			if( !content_.empty() ) {
				os << " value:" << oden::gammo::narrow(content_);
			}
			os << "]" << std::endl;

			shower<std::ostream> swr = { &os, dep+1 };
			std::for_each( child_.begin(), child_.end(), swr );
		}
		
	private:
		int id_;
		string_type content_;
		std::vector<smart_ptr> child_; 
	};

	typedef node<char> 		cnode;
	typedef node<wchar_t> 	wnode;
	
//#######################################################
//  �A�N�V����
//	
	template< typename Ch, typename Smp = typename node<Ch>::smart_ptr >
	struct tree_creator
		: public semantic_action< Smp >
	{
		typedef node<Ch> node_type;
		typedef Smp      node_ptr;
	
		node_ptr temp_node_;

		template< typename Iter >
		void on_term( int id, Iter itr, Iter eitr )
		{		
			val().reset( new node_type( id, itr, eitr ) ); // new Term(itr,eitr);
		};
		
		void on_node_child( int id, const node_ptr& chi, int argc )
		{
			if( argc == 1 ) {
				temp_node_.reset( new node_type(id) );
			}
		
			temp_node_->add( chi );
		}
		
		void on_node( int )
		{
			val() = temp_node_;
			temp_node_.reset();
		}

		template< typename Event >
		void on_event( Event& e )
		{
			std::cout << "at "<< std::distance(e.begin(), e.here()) << " \"" << *e.here() << "\" ";

			switch( e.code() )
			{
			case bu::event_term_success:
				std::cout << "�I�[����:" << e.id();
				break;
			case bu::event_join_success:
				std::cout << "��������:" << e.id();
				break;
			case bu::event_join_skip:
				std::cout << "�������X�L�b�v:" << e.id();
				break;
			case bu::event_join_reduce:
				std::cout << "���O�܂ł̌������Ҍ�";
				break;
			case bu::event_join_expect:
				std::cout << "������\�z:" << e.id();
				break;
			case bu::event_context_begin:
				std::cout << "�R���e�N�X�g�J�n:" << e.context();
				break;
			case bu::event_context_end:
				std::cout << "�R���e�N�X�g�I��:" << e.context();
				break;
			case bu::event_rule_switched:
				std::cout << "���[����؂�ւ��܂���";
				break;
			case bu::error_unexpected_token:
				std::cout << "!!�\�����Ȃ��g�[�N��������܂�";
				break;
			case bu::error_invalid_joint:
				std::cout << "!!�c���[�̍\����A�����Ȍ����ł�";
				break;
			case bu::error_index_overflow:
				std::cout << "!!�C���f�b�N�X���I�[�o�[�t���[���܂��� �ő�l=" << ODEN_CHIKUWA_BU_INDEX_MAX;
				break;
			}

			std::cout << "\n";
		}
	};
	
} // end of namespace bu
}}

#endif
