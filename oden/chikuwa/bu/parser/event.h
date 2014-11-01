#pragma once
#ifndef ODEN_CHKW_BU_EVENT
#define ODEN_CHKW_BU_EVENT

#include <oden/chikuwa/bu/parser/defs.h>

namespace oden { namespace chikuwa 
{ 
namespace bu
{
//#######################################################
//  エラー・イベントレポート
// 	
	enum event_code
	{
		event_level_all,
		event_join_expect,
		event_join_skip,
		event_join_reduce,
		event_join_success,
		event_context_begin,
		event_context_end,
		event_term_success,
		event_rule_switched,

		event_level_error,
		error_unexpected_token,
		error_invalid_joint,
		error_main_context_closed,
		error_context_not_closed,
		
		error_stack_overflow,
		error_index_overflow,

		event_none
	};

	template< typename Parser >
	class parser_event
	{
	public:
		typedef Parser parser_type;
		typedef typename parser_type::token_iterator	token_iterator;
		typedef typename parser_type::tree_type			tree_type;
		
		parser_event( Parser* pPser )
			: pser_(pPser)
		{}
		
		int  code()     const { return code_; }
		bool is_error() const { return code > bu::event_level_error; }

		const parser_type& parser()const { return *pser_;  }
		const tree_type&   tree()const   { return pser_->tree(); }
		
		int id() const { return def_id_; }
		int context() const 
		{ 
			return pser_->context().id; 
		}
		
		token_iterator here()			const { return pos_; }
		token_iterator begin()			const { return pser_->begin(); }
		token_iterator end()			const { return pser_->end(); }
		token_iterator context_begin() const 
		{
			return pser_->context().first; 
		}
		
	  // test
		bool raise( event_code code, token_iterator itr, int jid = null_def_id )
		{
			if( pser_->test_event_level(code) )
			{		
				code_ = code;
				def_id_ = jid;
				pos_ = itr;

				pser_->handle_event( *this );
				return true;
			}
			return false;
		}
		template< typename Entry >
		bool raise( event_code code, const Entry& ent )
		{
			if( pser_->test_event_level(code) )
			{		
				return this->raise( code, this->begin()+ent.place(), ent.def_id() );
			}
			return false;
		}

	  // デバッグ表示文字列
		std::string tree_debug_display()const
		{
			const char chs[] = "Oo-";
			return this->tree().debug_display( chs );
		}

	private:
		Parser* pser_;	
		
		event_code code_;
		int def_id_;
		token_iterator pos_;	
	};
	
} // end of namespace parser
}}

#endif
