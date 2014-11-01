#pragma once

#ifndef ODEN_PROPERTY_INCLUDED
#define ODEN_PROPERTY_INCLUDED

#include <oden/memory/memory_range.h>

namespace oden 
{	
//############################################################################
// �v���p�e�B�����̕⏕�N���X
//	
  // �f�t�H���g��N���X�̒�`
	struct property_default_operators
	{
		struct get {};
		struct set {};
		struct def 
		{
			template< typename Any >
			def( Any& ){}
		};
	};

  // �v���p�e�B�l�𒊏o����N���X
	// �L���X�g
	struct property_cast_value
	{
		template< typename DestT, typename SrcT >
		static DestT& val( SrcT& r ) { return reinterpret_cast<DestT&>(r); }

		template< typename DestT, typename SrcT >
		static const DestT& val( const SrcT& r ){ return reinterpret_cast<const DestT&>(r); }
	};
	// �����o
	template< typename T, typename Class, T Class::*pMv >
	struct property_member_value
	{
		template< typename T >
		static T& val( Class& obj ){ return obj.*pMv; }

		template< typename T >
		static const T& val( const Class& obj ){ return obj.*pMv; }
	};

  // �e�N���X�𑀍삷��֐��Q���
	template< typename Derived, typename ValueType, typename SelfValue >
	struct property_interface
	{
		typedef Derived   derived;
		typedef ValueType value_type;
		typedef SelfValue self_value;

	  // �^�T�C�Y
		static int value_size(){ return sizeof( value_type ); }

	  // �C���X�^���X���l��
		template< typename Inst >
		static derived& 
			me( Inst* inst ) { return *static_cast<derived*>(inst); }

		template< typename Inst >
		static const derived& 
			me( const Inst* inst ) { return *static_cast<const derived*>(inst); }

	  // ���f�[�^����f�[�^���l�� : SelfValue�ŃJ�X�^�}�C�Y
		template< typename T >
		static value_type& 
			custom_val( T& r ) { return self_value::val<value_type>( r ); }

		template< typename T >
		static const value_type& 
			custom_val( const T& r ) { return self_value::val<value_type>( r ); }

		/* �����^�Ȃ炻�̂܂� */
		static value_type& 
			custom_val( value_type& r ) { return r; }

		static const value_type& 
			custom_val( const value_type& r ) { return r; }

	  // �f�[�^���l��
		template< typename Inst >
		static value_type& 
			val( Inst* inst ) { return custom_val( me(inst).get() ); }

		template< typename Inst >
		static const value_type& 
			val( const Inst* inst ) { return custom_val( me(inst).get() ); }

	  // �ʂ̃I�y���[�^���p���p�ɍč\��
		template< typename Set, typename SelfValue2 = property_cast_value >
		struct extends
		{
			typedef typename Set::template implement< derived, SelfValue2 > ops;
			typedef typename ops::get get;
			typedef typename ops::set set;	

			struct def : ops::def
			{
				template< typename T >
				def( T& out ) : ops::def( ops::interface_::self_value(out) ) {}
			};

			typedef typename Set::value_type src_value_type;

			template< src_value_type value_type::*pMv >
			struct member
				: extends< Set, property_member_value<src_value_type,value_type,pMv> >
			{};
		};
	};

//##############################################################################################
// �v���p�e�B�E�Z�b�g��񋟂���
//
  // reference_writer, value_writer, rvalue_writer 
	template< typename T, template< typename, typename, typename > class ImplSet >
	struct property_set
	{
		typedef property_set outer_class;
		typedef T value_type;
		
		typedef value_type const& 	const_reference;
		typedef value_type& 		reference;
		typedef value_type const* 	const_pointer;
		typedef value_type* 		pointer;

	  // �G�C���A�X
		template< typename Me, typename SelfValue = property_cast_value /*dummy*/ >
		struct implement
		{
			typedef property_interface<Me, value_type, SelfValue> interface_;
			typedef ImplSet< Me, value_type, interface_ > type;

			typedef typename type::get get;
			typedef typename type::set set;
			typedef typename type::def def;
		};
		
	//--------------------------------------------------------------
	//   reader �̃Z�b�g
	//
	  // �C���X�^���X����ǂ݂���
		class value_reader
			 : public implement< value_reader >::get
		{
		public:
			typedef outer_class  property;
		// �R���X�g���N�^
			value_reader()
				: data_()
			{
				typename implement< value_reader >::def def_value(data_);
			}
			value_reader( const_reference p )
				: data_( p )
			{}
			
		// �I�[�o�[���C�h
			const_reference get()const { return data_; }
			reference get() { return data_; }
			
			operator reference(){ return get(); }
			operator const_reference()const{ return get(); }		
		private:		
			value_type data_;
		};		
		
	  // �f�[�^�̎Q�Ƃ���ǂ݂���
		class reference_reader
			 : public implement< reference_reader >::get
		{
		public:
			typedef outer_class  property;
		// �R���X�g���N�^
			reference_reader( reference r )
				: pdata_( &r )
			{}
			reference_reader( pointer p )
				: pdata_( p )
			{}

		// �I�[�o�[���C�h
			reference get() { return *pdata_; }			
			const_reference get()const { return *pdata_; }
			
			operator reference(){ return get(); }
			operator const_reference()const{ return get(); }
			
			bool operator!() const { return !pdata_; } 	
		private:
			pointer pdata_;
		};
		
	  // �f�[�^��const�Q�Ƃ���ǂ݂���
		class const_reference_reader
			 : public implement< const_reference_reader >::get
		{
		public:
			typedef outer_class  property;
		// �R���X�g���N�^
			const_reference_reader( const_reference r )
				: pdata_( &r )
			{}
			const_reference_reader( const_pointer p )
				: pdata_( p )
			{}
		
			const_reference get()const { return *pdata_; }
			
			operator const_reference()const{ return get(); }
			
			bool operator!() const { return !pdata_; } 	
		private:
			const_pointer pdata_;
		};

	  //--------------------------------------------------------------
	  //   writer �̃Z�b�g
	  //
		// ���̂ɏ�������
		class value_writer
			: public implement< value_writer >::set
		{
		public:
			typedef outer_class  property;
			// �R���X�g���N�^
			value_writer()
				: data_()
			{
				typename implement< value_writer >::def def_value( data_ );
			}
			value_writer( const_reference p )
				: data_( p )
			{}

			// �I�[�o�[���C�h
			reference get() { return data_; }
			const_reference get()const { return data_; }

			operator reference(){ return get(); }
			operator const_reference()const{ return get(); }

			// getter
			const_reference_reader getter()const { return const_reference_reader( data_ );  }

		private:
			value_type data_;
		};

		// �f�[�^�̎Q�Ƃ֏�������
		class reference_writer
			: public implement< reference_writer >::set
		{
		public:
			typedef outer_class  property;
			// �R���X�g���N�^
			reference_writer( reference r )
				: pdata_( &r )
			{}
			reference_writer( pointer p )
				: pdata_( p )
			{}

			// �I�[�o�[���C�h
			reference get() { return *pdata_; }
			const_reference get()const { return *pdata_; }

			operator reference(){ return get(); }
			operator const_reference()const{ return get(); }

			//
			bool operator!() const { return !pdata_; }

			// getter
			const_reference_reader getter()const { return const_reference_reader( pdata_ ); }

		private:
			pointer pdata_;
		};
		
	//--------------------------------------------------------------
	//   �G�C���A�X
	//
		typedef value_writer			write_val;
		typedef reference_writer		write_ref;

		typedef value_reader			read_val;
		typedef reference_reader		read_ref;
		typedef const_reference_reader	read_cref;

	  // ��M�����p
		static value_writer				set()							{ return value_writer(); }
		static reference_writer			set( reference r )				{ return reference_writer( r ); }
		
		static const_reference_reader	get( const_reference r )		{ return const_reference_reader( r ); }
		static reference_reader			get( reference r )				{ return reference_reader( r ); }
		static const_reference_reader	get( const_pointer r )			{ return const_reference_reader( r ); }
		static reference_reader			get( pointer r )				{ return reference_reader( r ); }
	  
	  // auto�p
		static value_writer				writer()						{ return value_writer(); }
		static reference_reader			writer( reference r )			{ return reference_writer( r ); }

		static const_reference_reader	reader( const_reference r )		{ return const_reference_reader( r ); }
		static reference_reader			reader( reference r )			{ return reference_reader( r ); }
		static const_reference_reader	reader( const_pointer r )		{ return const_reference_reader( r ); }
		static reference_reader			reader( pointer r )				{ return reference_reader( r ); }

		static value_reader				copy_reader( const_reference r ){ return value_reader( r ); }

	//
	// �\�z���֎~
	//
	private:
		property_set(){}
	};
	
//##########################################################################
//�@memory_range -> property �̃T�|�[�g
//
  // cref
	template< typename Prop, typename U >
		typename Prop::const_reference_reader 
			mem_reader( 
				memory_range<U> p 
			)
	{
		const void* vp = p.get();
		
		typedef typename Prop::const_pointer pT;
		return typename Prop::const_reference_reader( static_cast<pT>(vp) );
	}
	
  // mcref
	template< typename Prop, typename U >
		typename Prop::reference_reader 
			mem_reader( 
				buffer_range<U> p 
			)
	{
		void* vp = p.get();
		
		typedef typename Prop::pointer pT;
		return typename Prop::reference_reader( static_cast<pT>( vp ) );
	}

  // ref
	template< typename Prop, typename U >
		typename Prop::reference_writer
			mem_writer( 
				buffer_range<U> p 
			)
	{
		void* vp = p.get();
		
		typedef typename Prop::pointer vT;
		return typename Prop::reference_writer( static_cast<pT>( vp ) );
	}

//##############################################################################################
//  �L�q�p�}�N��
//
  // 
	#define ODEN_PROPERTY_OPERATORS_NAME( name ) \
		name##_operators

	#define ODEN_PROPERTY_OPERATORS_DECL( name ) \
		template< typename Me, typename value_type, typename self > struct ODEN_PROPERTY_OPERATORS_NAME(name)

	#define ODEN_PROPERTY_OPERATORS( name ) \
		ODEN_PROPERTY_OPERATORS_DECL( name ) : oden::property_default_operators

  //
	#define ODEN_PROPERTY_EXTENDS \
		typename self::template extends
		 
	#define ODEN_PROPERTY_EXTENDS_MEMBER( set, pmem ) \
		ODEN_PROPERTY_EXTENDS< set >::template member<pmem>
		
	#define ODEN_PROPERTY_EXTENDS_SELF( set ) \
		ODEN_PROPERTY_EXTENDS< set >

  // 
	#define ODEN_PROPERTY_DECL( name, value_type )\
		ODEN_PROPERTY_OPERATORS_DECL( name ); \
		typedef property_set< value_type, ODEN_PROPERTY_OPERATORS_NAME(name) > name

	#define ODEN_PROPERTY( name, value_type ) \
		ODEN_PROPERTY_DECL( name, value_type ); ODEN_PROPERTY_OPERATORS( name )

	#define ODEN_STRUCT ODEN_PROPERTY
	
	
	
	

} /* end of namespace oden */
	
#endif

