#pragma once
#ifndef ODEN_CHKW_BU_DEF
#define ODEN_CHKW_BU_DEF

#include <Shiny.h>

namespace oden { namespace chikuwa
{
namespace bu
{
//#######################################################
//  型
//
#ifdef ODEN_CHIKUWA_BU_USE_SMALL_INDEX
	typedef unsigned short		generic_index_t;
	typedef unsigned int		promoted_index_t;

#	define ODEN_CHIKUWA_BU_INDEX_MAX 0xFFFF
#else
	typedef unsigned int		generic_index_t;
	typedef unsigned int		promoted_index_t;

#	define ODEN_CHIKUWA_BU_INDEX_MAX 0x7FFFFFFF
#endif

	typedef generic_index_t		node_index, joint_index;
	typedef generic_index_t		node_size, joint_size;


//#######################################################
//  定数
//	
  // 結合優先順位の定数
	enum joint_priority
	{ 
		highest_priority = 0, 
		lowest_priority  = 0x00FFFFFF 
	};
	
  // 結合方向の定数
	enum joint_assoc_type
	{
		left_assoc,
		right_assoc
	};
	
  // 結合関数
	enum joint_function
	{
		join_func_null = 0,
		join_func_adjucent,
		join_func_interval,
		join_func_fixed
	};
	
  // 結合関数の引数定数
	enum joint_function_arg
	{
		join_arg_false = 0,
		join_arg_true,
		join_arg_forward,
		join_arg_backward
	};

  // IDの定数
	enum definition_id
	{
		null_def_id = -1,
		user_def_id = 0		
	};
	
  // コンテクストID
	enum context_id
	{ 
		temp_context_id    = -1, 
		default_context_id = 0, 
		user_context_id    = 1 
	};
	
  // マッチャー返り値
	enum matcher_result 
	{ 
		matcher_error = -1,
		matcher_fail  = 0
	};
	
  // ルールの挙動
	enum rule_flags
	{
		first_match    = 0x01,
		longest_match  = 0x02,
		shortest_match = 0x04
	};
	
  // matcher_result と同じ
	enum rule_set_result
	{ 
		rule_set_error = matcher_error 
	};
	
	
	
	
	
} // end of namespace bu
}}

#endif
