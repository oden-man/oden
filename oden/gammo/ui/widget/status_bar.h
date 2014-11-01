#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_STATUSBAR
#define ODEN_GAMMO_UI_WIDGETS_STATUSBAR

//#######################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------	
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
// --------- gammo ---------
#include <oden/gammo/ui/control/status_bar.h>
#include <oden/gammo/ui/widget/window.h>

//
//####################################################


namespace oden { namespace gammo
{
//______________________________________________________________________________________________________
//
//          �X�e�[�^�X�o�[�E�E�B�W�F�b�g�E�p�[�c
//______________________________________________________________________________________________________
//
namespace widget_layout
{
//######################################################
// �@�X�e�[�^�X�o�[�̋@�\�����������C�A�E�g
//
	template< typename Base >
	class status_bar
		: public Base
	{
	public:
	// �R���X�g���N�^
		status_bar(){}
		~status_bar(){}

	// �����o�֐�
	  // �p�[�c�̃T�C�Y���i�[
		class parts {
		public:
		  // �����w��
			parts& fix( int w ) 
			{ data_.push_back(w); return *this; }
		  // �������ɐL�т�
			parts& expand() 
			{ return fix(-1); }
		  // �N���A
			void clear()
			{ data_.clear(); }
		  // �p�[�c���Q�b�g
			const std::vector<int>& get()const { return data_; }
		private:
			std::vector<int> data_;
		};	

	  // ���C�A�E�g�N���X��Ԃ�
		parts& describe_parts_layout()
		{ parts_.clear(); return parts_; }

	// ���ʊ֐�
	  // ���̃E�B�W�F�b�g�𓮂���
		bool move_widget();

	private:
		typename status_bar<Base>::parts parts_;
	};

	template< typename B >
	bool status_bar<B>::move_widget()
	{
		if( on_start() ) /* ���������̂� */
		{
			set_part_layout( parts_.get() );  // �p�[�c�𕪊�
		}
		else if( has_parent_widget() )
		{
			rect rc;
			client_rect(rc);
			send_message( WM_SIZE, rc.width(), rc.height() );
			return true;
		}
		return false;
	}
/*
//######################################################
// �@���O�Ńp�[�c�T�C�Y���Čv�Z���郌�C�A�E�g
//
	class status_bar_layout_ex
	{
	public:
	// �R���X�g���N�^
		status_bar_layout_ex(){}
		virtual ~status_bar_layout_ex(){}

	// ���C�A�E�g�p�[�c�N���X
		class part_term
			: public layout_terminal, private boost::noncopyable
		{
		public:
		// �R���X�g���N�^
			part_term( status_bar_layout_ex* pinst, int at )
				: pinst_(pinst), at_(at)
			{}
			~part_term(){}
			
		// �I�[�o�[���C�h�֐�		
		  // layout_terminal - �I�[�o�[���C�h
			void register_location( const rect& lo ) 
			{ 
				pinst_->parts_[at_] = lo.width(); 
			}
		
		private:
			status_bar_layout_ex* pinst_;
			int at_;
		};

	// �����o�֐�	
	  // �p�[�c
		part_term* bar_part()
		{
			parts_.push_back(0);
			return new part_term( this, parts_.size()-1 );
		}
	  // �p�[�c�T�C�Y���Čv�Z
		void recalc_parts( widget_object* wid )
		{
			if( layout_tree_ )
			{
				rect rc;
				wid->get_window().client_rect(rc);

				layout_tree_->recalc(rc);
			}

			status_bar bar = wid->get_window_handle();
			bar.set_part_layout( parts_ );
		}
	  // ���C�A�E�g���Z�b�g
		void set_layout( layout_object_pointer pobj )
		{ layout_tree_.reset( pobj.get() ); }
		
	// ���̃E�B�W�F�b�g�𓮂���
		void move_wid( widget_object* wid )
		{
			if( wid->has_parent() )
			{
				rect rc;
				wid->get_parent()->get_window().client_rect(rc);

				wid->get_window().send_message( WM_SIZE, rc.width(), rc.height() );

				recalc_parts( wid );
			}	
		}

	// �C�x���g�n���h��
	  // WM_CREATE : �X�e�[�^�X�o�[���敪������
		void layout_on_start( widget_object* wid )
		{ recalc_parts( wid ); }

	  // �_�~�[
		void layout_on_size( widget_object* ) {}
		void layout_on_sizing( widget_object*, const window_event& ) {}
		
	private:
		boost::scoped_ptr<layout_object> layout_tree_;	
		std::vector<int> parts_;		
	};
	*/
}

//______________________________________________________________________________________________________
//
//          �E�B�W�F�b�g�p�[�c�E�v���Z�b�g
//______________________________________________________________________________________________________
//
  // �R���g���[��
	typedef new_window< widget< status_bar >, control_window_class<status_bar> > 
		status_bar_widget;
		
	typedef widget_layout::status_bar< status_bar_widget > 
		status_bar_layout_widget;


}} /* end of namespace oden.gammo */

#endif

