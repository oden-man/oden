#pragma once

#ifndef ODEN_GAMMO_CMN_DUALTID
#define ODEN_GAMMO_CMN_DUALTID

//#####################################################
//  インクルード
//

// ------ defs -----------
// ------ std ---------
//#include <cstdio>
#include <cassert>
// ----- boost --------
#include <boost/operators.hpp>
#include <boost/static_assert.hpp>
// ------ oden --------
// ------- gammo --------

//
//
//###############################################################

namespace oden { namespace gammo
{
//########################################################################
// ポインタと数値両方入るIDクラス
//
	template<
		typename ChT,
		typename ValT,
		typename Classi,
		typename StrT = const ChT*
		>
	class dual_id
		: private 
		          boost::equality_comparable< dual_id< ChT, ValT, Classi, StrT >, ValT,
		  		  boost::equality_comparable< dual_id< ChT, ValT, Classi, StrT >, StrT,
		  		  boost::equality_comparable< dual_id< ChT, ValT, Classi, StrT > 
		  		  > > >
	{
	public:
		typedef Classi		classifier;
		typedef ChT			char_type;
		typedef StrT 		string_pointer;
		typedef ValT 		value_type;
	private:
		BOOST_STATIC_ASSERT( sizeof(value_type) <= sizeof(string_pointer) );

		typedef dual_id<ChT,ValT,Classi,StrT> self;

	public:
	// コンストラクタ
		dual_id()
			:data_( )
		{}
		dual_id( value_type id )
			:data_( reinterpret_cast< string_pointer >( id ) )
		{}
		dual_id( string_pointer cop )
			:data_( cop )
		{}
		
	// メンバ関数
	  // 取得
		string_pointer  get ( )const { return data_; }
		
		bool is_value	( )const { return classifier()( get() ); }
		bool is_string	( )const { return !is_value(); }
		bool is_valid	( )const { return get() != 0; }
		
		string_pointer as_string_ptr ( )const { assert(is_string()); return get(); }
		value_type     as_value      ( )const { return reinterpret_cast<value_type>( get() ); }
		
		template< typename Container >
		void to_string( Container& out )const
		{
			if( is_string( ) ) {
				out = as_string_ptr();
			}/*
			else {
				char_type buf[32];
				detail::num_to_string( buf, as_value() );
				out = buf;
			}*/
		}
		
	  // 設定
	  	void set( value_type l ) 
		{ self( l ).swap(*this); }

		void set( string_pointer cop )
		{ self( cop ).swap(*this); }

	// 演算子
	  // =
		self& operator = ( value_type l ) 			{ set(l); return *this; }
		self& operator = ( string_pointer cop ) 	{ set(cop); return *this; }
	  // ==
	  	bool operator == ( value_type id )const
		{ return is_value() && ( as_value() == id ); }
		
	  	bool operator == ( string_pointer cop )const 
		{ return *this == self( cop ); }
		
	  	bool operator == ( const dual_id& other )const 
		{
			if( is_string() != other.is_string() )  // IDタイプが違けりゃ当然違う
				return false;
			else if ( is_string() )  // 名前の比較
			{
				string_pointer left  = this->as_string_ptr();
				string_pointer right = other.as_string_ptr();
				
				while( *left == *right )
				{
					if( *left == 0 )
						return true;
						
					++left;	++right;
				}
				return false;
			}
			else
				return *this == other.as_value( );	 // ID値の比較
			return false; 
		}
		
	// swap
		void swap( dual_id& other ) 
		{ boost::swap( other.data_, this->data_ ); }
		
		friend void swap( dual_id& l, dual_id& r )
		{ l.swap(r); }
		
	private:
		string_pointer data_;
	};

//########################################################################
// 文字列をバッファを設けて保持するバージョン
//
	template< typename IDType >
	class dual_id_val
		: private 
		          boost::equality_comparable< dual_id_val< IDType >, IDType,
		  		  boost::equality_comparable< dual_id_val< IDType > 
		  		  > >
	{
	private:
		typedef dual_id_val< IDType > self;
	public:
		typedef IDType id_type;
		typedef typename IDType::classifier 	classifier;
		typedef typename IDType::char_type 		char_type;
		typedef typename IDType::value_type 	value_type;
		typedef typename IDType::string_pointer string_pointer;
		
		typedef std::basic_string< char_type > 	string_type;
		
	// コンストラクタ
		dual_id_val()
			: buffer_( ), id_( )
		{}
		dual_id_val( const self& other )
			: buffer_( ), id_( )
		{ 
			set( other );
		}
		dual_id_val( const id_type& id )
			: buffer_( ), id_( )
		{
			set( id );
		}
	// メンバ関数
	  // 取得
		string_pointer get_id( )		const { return id_.get(); }
		
		id_type& get( )					  	  { return id_; }
		const id_type& get( )			const { return id_; }
		
		bool is_value( )				const { return id_.is_value(); }
		bool is_string( )				const { return id_.is_string(); }
		bool is_valid( )				const { return id_.is_valid(); }
		
		value_type as_value( )			const { return id_.as_value(); }
		const string_type& as_string( )	const { assert(is_string()); return buffer_; }
		
		void to_string( string_type& out )const	 
		{
			id_.to_string( out );	
		} 
		string_type to_string( )const 
		{ 
			string_type r;
			id_.to_string( r );
			return r; 
		}
		
	  // 設定
	  	void set( const id_type& id ) 
		{ 
			if( id.is_string() ) {
				buffer_.assign( id.get() );
				id_ = buffer_.c_str();
			} else {
				id_ = id;
			}
		}
		void set( const self& other ) 
		{
			set( other.id_ );
		}
		void set_string( const string_type& str )
		{
			this->set( str.c_str() );
		}

	// 演算子
	  // =
		self& operator = ( const id_type& other ) 		{ set(other); return *this; }
		self& operator = ( const self& other ) 			{ set(other); return *this; }
	  // ==
	  	bool operator == ( const id_type& id )const 	{ return id_ == id; }
	  	bool operator == ( const self& other )const 	
		{
			if( id_.is_string() != other.id_.is_string() )
				return false;
			else if( id_.is_string() )
				return buffer_ == other.buffer_;
			else
				return this->as_value() == other.as_value(); 
		}

	// swap
		void swap( self& other )
		{
			boost::swap( other.buffer_, buffer_ );			
			boost::swap( other.id_, id_ );
		}		
		friend void swap( self& l, self& r )
		{
			l.swap(r);
		}

	private:
		string_type buffer_;
		id_type     id_;
	};
	
	
}}
	
#endif
