#pragma once

#ifndef ODEN_GAMMO_UI_WIDGETS_OBJECT
#define ODEN_GAMMO_UI_WIDGETS_OBJECT

//#######################################################
//  �C���N���[�h
//

// --------- defs -----------
#include <oden/gammo/defs.h>
// --------- stl ------------	
#include <bitset>
// --------- win ------------
// --------- boost ----------
// --------- oden  ----------
#include <oden/object/container.h>
// --------- gammo ----------

//
//####################################################


namespace oden { namespace gammo
{
	class window;
	class window_event;
	class preprocessing_event;
	class move_widget_context;

//_____________________________________________________________________________________________________________
//
//		�C���^�[�t�F�[�X
//_____________________________________________________________________________________________________________
//

// �E�B�W�F�b�g�E�|�C���^
//
	template< typename T >
	class wid_ptr
	{
	public:
		typedef T  value_type;
		typedef T  element_type;
		typedef T* pointer;
		typedef T& reference;

		explicit wid_ptr( T* p = 0 )
			: p_( p )
		{}

	// �����o�֐�
 	  // �ύX
		void reset( pointer p ) { p_ = p; }
		pointer get()const { return p_; }

	  // ���Z�q
		pointer operator->()const { return p_; }
		reference operator*()const { return *p_; }

		operator pointer()const { return p_; }

	private:
		pointer p_;
	};

// ��ԃt���O
//
	enum widget_flags
	{
		widget_ready,

		widget_no_update_after_init,
		widget_invisible,

		widget_flags_max
	};

//#################################################
//  �E�B�W�F�b�g�E�C���^�[�t�F�[�X
//
	class widget_object
		  : private boost::noncopyable
	{
	public:
		typedef object_container< widget_object > 	widget_container;
		typedef widget_container::element			widget_vector_elem;
		typedef std::vector< widget_vector_elem >	widget_vector;
		
		typedef widget_vector::iterator				widget_iterator;
		typedef widget_vector::const_iterator		widget_const_iterator;
		
	// �R���X�g���N�^
		widget_object()
			: parent_(0), id_(0)
		{}
		virtual ~widget_object(){}
	
	// �E�B���h�E
	  // ���̂��̂ɃL���X�g
		virtual window& to_window() = 0;
		virtual const window& to_window() const = 0;
	  // �n���h��
		window_handle get_window_handle() const;
		void set_window_handle( window_handle wh );
		
	// �C�x���g����
	  // �S�̗̂���
		virtual void handle_event( window_event& ) = 0;
	  // ���C�A�E�g�֘A�̃C�x���g�ɑΉ�
		virtual void handle_layout_event( window_event& ev ) = 0;
	  // ���[�U�[��`�̃C�x���g�n���h�����N�� 
		virtual void my_handle_event( window_event& ev ) = 0;
	  // �c����̂ɑΉ�
		virtual void final_handle_event( window_event& ) = 0;
	  // �����Ŏ��R�ȏ������`
		virtual bool preprocess_event( preprocessing_event& ) = 0;
	  // �C�x���g�V�X�e���Ɏ��M��ǉ�
		virtual bool attach_event_loop() = 0;
	  // �C�x���g�V�X�e������O���
		virtual bool detach_event_loop() = 0;

	// ���C�A�E�g�֘A����
	  // �w��ʒu�ֈړ�
		virtual bool move_widget ( move_widget_context& c ) = 0;
	  // �ʒu���w�肷��
		virtual void set_widget_location ( const rect& r ) = 0;
		virtual const rect& widget_location ()const = 0;
	  // �œK�T�C�Y
		virtual void get_best_size		( size& out )const = 0;
		virtual void get_best_size_now	( size& out )const = 0;
	  // �œK�T�C�Y�ɂȂ�悤�ʒu�w��
		virtual void fit_widget() = 0;
	  // �t�H���g�̕ύX�ȂǂōœK�T�C�Y���ς����
		virtual void best_size_changed() = 0;

	// �쐬�t���[
	  // ������
		bool start();
	  // �j��
		bool destroy();

	private:
	  // �������X�e�b�v
		// ���[�U�[��`�̏�����(�쐬�O)
		virtual void describe(){}
		// �쐬�O�����̂܂Ƃ�
		void precreate();

		// �R���g���[���쐬���@
		virtual bool create_control(){ return true; }
		// ������
		virtual bool init_widget();
		// ���[�U�[��`�̏�����(�쐬��)
		virtual bool initialize(){ return true; }
		// �쐬�����̂܂Ƃ�
		bool docreate();


	/* widget_object �ŗL�֐� */
	public:
	// �t���O����
	  // �J�n�������s������?
		bool is_ready()const { return flags_[widget_ready]; }

	  // �쐬��Aupdate���Ă΂Ȃ�
		void no_update_after_init()
		{ flags_.set( widget_no_update_after_init ); }

	  // �t���O�𑀍�
		bool test_widget_flag ( widget_flags f )const { return flags_[f]; }
		bool set_widget_flag  ( widget_flags f, bool b = true ) { flags_.set( f, b ); }

	// �e�q
	  // �e
		widget_object* get_parent_widget()const { return parent_; }
		void set_parent_widget( widget_object* p ) 
		{ 
			parent_ = p; 
			id_ = p->count_child_widget();
		}

	  // �q��ǉ�
		template< typename Wid >
		Wid* add_child( Wid* pNew ) 
		{
			if( pNew )
			{
				widget_container::push_back( children_, pNew );
				pNew->set_parent_widget( this );
			}
			return pNew;
		}
		template< typename Wid >
		Wid* add_new_child()
		{
			return this->add_child( new Wid() );
		}

		template< typename Wid >
		Wid* new_child( wid_ptr<Wid>& w )
		{
			w.reset( this->add_new_child<Wid>() );
			return w.get();
		}
		
	  // ���ׂ�
		bool has_parent_widget()const { return get_parent_widget() != 0; }
		bool has_child_widget()const  { return count_child_widget() != 0; }
		int count_child_widget()const { return children_.size(); }
		
	  // �C�e���[�^�擾
		widget_iterator begin() { return children_.begin(); }
		widget_const_iterator begin()const { return children_.begin(); }
		
		widget_iterator end() { return children_.end(); }
		widget_const_iterator end()const { return children_.end(); }
		
	  // �qID
		void widget_child_id( int ID ){ id_ = ID; }
		int get_widget_child_id()const { return id_; }
		
	private:
		widget_object* parent_;
		widget_vector  children_;
		int			   id_;
		std::bitset<widget_flags_max> flags_;
	};

}} /* end of namespace oden.gammo */


#endif

