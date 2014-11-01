#pragma once

#ifndef ODEN_GEO
#define ODEN_GEO

#include <algorithm>
#include <boost/operators.hpp>


namespace oden 
{

//#######################################################
// 補助関数
//
  // 二つの値の差分をとる
	template< typename T >
	T difference( const T& a, const T& b )
	{
		if( a > b ) 
			return a - b;
		return b - a;
	}

//#######################################################
// xy座標点を表す
//
	template< typename T >
	class pointT
		: boost::additive< pointT<T>,				// + -
		  boost::multiplicative< pointT<T>,	T,		// * /
		  boost::equality_comparable< pointT<T> > >	> // ==, !=
	{
	public:
		typedef pointT<T>	self;
		typedef T			value_type;
	// コンストラクタ
		pointT()
			: x_(0), y_(0)
		{}
		pointT( value_type X, value_type Y )
			: x_(X), y_(Y)
		{}	
		pointT( const pointT& src )
			: x_(src.x_), y_(src.y_)
		{}
		
	// プロパティ
		value_type x()const { return x_; }
		value_type y()const { return y_; }
		
		value_type& rx() { return x_; }
		value_type& ry() { return y_; }
	
		void set( value_type x, value_type y )
		{
			x_ = x; 
			y_ = y;
		}
		
	// メンバ関数
		bool is_null()const 
		{
			return x_==0 && y_==0;
		}
		void shift( value_type x, value_type y ) 
		{
			x_ += x;
			y_ += y;
		}
		void shift( const self& src ) 
		{
			x_ += src.x_;
			y_ += src.y_;
		}
		self shifted( value_type x, value_type y )const
		{
			return self( x_ + x, y_ + y );
		}
		self shifted( const self& src )const
		{
			return self( x_ + src.x_, y_ + src.y_ );
		}
		self distance( const self& src )const
		{
			return self( oden::difference( x_, src.x_ ), oden::difference( y_, src.y_ ) );
		}
		self offset( const self& src )const
		{
			return self( x_ - src.x_, y_ - src.y_ );
		}
	
	// 演算子オーバーロード
	  // 単項-
	  	self operator-()const
	  	{
	  		return self(-x_,-y_);	
	  	}
	  // +-
		self& operator+=( const self& src )
		{
			x_ += src.x_; y_ += src.y_;
			return (*this);	
		}
		self& operator-=( const self& src )
		{
			x_ -= src.x_; y_ -= src.y_;
			return (*this);	
		}
	  // */
		self& operator*=( value_type val )
		{
			x_ *= val; y_ *= val;
			return (*this);	
		}
		self& operator/=( value_type val )
		{
			x_ /= val; y_ /= val;
			return (*this);	
		}
	  // order
		bool operator==( const self& src )const
		{
			return ((x_ == src.x_) && (y_ == src.y_));
		}
		
	// スワップ
		void swap( self& other )
		{
			std::swap( x_, other.x_ );	
			std::swap( y_, other.y_ );	
		}
		friend void swap( self& l, self& r )
		{
			l.swap(r);
		}
		
	public:
		value_type x_;
		value_type y_;
	};
	
//#######################################################
// 四角形の寸法:縦と横
//
	template< typename T >
	class sizeT
		: boost::additive< sizeT<T>,
		  boost::additive< sizeT<T>, T,
		  boost::totally_ordered< sizeT<T> > > >
		  		  
	{
		typedef sizeT<T> self;
	public:	
		typedef T value_type;

	// コンストラクタ
		sizeT()
			: width_( 0 ), height_( 0 )
		{}
		sizeT( value_type cx, value_type cy )
			: width_( cx ), height_( cy )
		{}
		sizeT( const self& src )
			: width_( src.width_ ), height_( src.height_ )
		{}
		
	// プロパティ
		value_type width()const 	{ return width_;  }
		value_type height()const	{ return height_; }
		
		value_type& rwidth()		{ return width_;  }
		value_type& rheight()		{ return height_; }
		
		void set( value_type cx, value_type cy )
		{
			width_ = cx;
			height_ = cy;
		}
		
	// メンバ関数
	  // 面積
		value_type area()const { return width_ * height_; }
		
	  // 拡大・縮小
		void scale( double x, double y ) 
		{
			width_ = static_cast<value_type>(width_*x);
			height_ = static_cast<value_type>(height_*y);
		}
	    self scaled( double x, double y )const 
		{ 
			self ret( *this );
	    	ret.scale( x, y );
	    	return ret;
	    }
	    
	  // アスペクト比を守ったまま大きさを変更
		void set_width_keep_ratio( value_type w ) 
		{
			height_ = static_cast<value_type>( w * static_cast<double>(height_) / width_ );
			width_  = w;
		}
		void set_height_keep_ratio( value_type h ) 
		{
			width_  = static_cast<value_type>( h * static_cast<double>(width_) / height_ );
			height_ = h;
		}
		
	  // 縦横を入れ替える
		void transpose() 
		{
			std::swap( width_, height_ );
		}
		self transposed( )const 
		{
			self ret( *this );
		    ret.transpose();
		    return ret;
		}

	  // 対象サイズを含みきれる大きさか
		bool contains( const self& src )const
		{
			if( this->is_valid() ) {
				if( src.is_null() )
					return true;
			}else
				return false;

			return std::max( width_, height_ ) >= std::max( src.width_, src.height_ );
		}
		
	  // 有効性のチェック
		bool is_empty()const { return !is_valid(); }
		bool is_null()const  { return width_==0 && height_==0; }
		bool is_valid()const { return width_>0 && height_>0; }
		
	// 演算子オーバーロード
	  // +-
		self& operator+=( const self& src )
		{
			width_ += src.width_;
			height_ += src.height_;
			return (*this);	
		}
		self& operator-=( const self& src )
		{
			width_ -= src.width_;
			height_ -= src.height_;
			return (*this);	
		}
		self& operator+=( value_type v )
		{
			width_ += v;
			height_ += v;
			return (*this);	
		}
		self& operator-=( value_type v )
		{
			width_ -= v;
			height_ -= v;
			return (*this);	
		}
	  // ==
		bool operator==( const self& src )const
		{
			return ( (width_==src.width_) && (height_==src.height_) );
		}
		bool operator<( const self& src )const
		{
			return this->area() < src.area();
		}
			
	// スワップ
		void swap( self& out )
		{
			std::swap(width_, out.width_);
			std::swap(height_, out.height_);
		}
		friend void swap( self& l, self& r )
		{
			l.swap(r);
		}
		
	private:
		value_type width_;
		value_type height_;
	};

//#######################################################
// 一つの四角形を表す
//
	template< typename T >
	class rectT
		: boost::additive< rectT<T>,
		  boost::equality_comparable< rectT<T> > >
	{
	public:	
		typedef rectT<T>	self;
		typedef T			value_type;
		typedef pointT<T>	point_type;
		typedef sizeT<T>	size_type;

	// コンストラクタ
		rectT()
			: tl_(), br_()
		{}
		rectT( value_type Left, value_type Top, value_type Right, value_type Bottom )
			: tl_( Left, Top ), br_( Right, Bottom )
		{}			
		rectT( const point_type &topleft, const point_type &botright )
			: tl_( topleft ), br_( botright )
		{}
		rectT( const size_type &di, const point_type &topleft = point_type() )
			: tl_(), br_()
		{
			this->set( di, topleft );
		}
		
	// メンバ関数
	  // 取得
		value_type left  ()const	{ return tl_.x(); }
		value_type right ()const	{ return br_.x(); }
		value_type top   ()const	{ return tl_.y(); }
		value_type bottom()const	{ return br_.y(); }
		
		value_type& rleft  ()  { return tl_.rx(); }
		value_type& rright ()  { return br_.rx(); }
		value_type& rtop   ()  { return tl_.ry(); }
		value_type& rbottom()  { return br_.ry(); }
		
		const point_type& topleft()const{return tl_;}
		point_type& rtopleft(){return tl_;}
		
		const point_type& botright()const {return br_;}
		point_type& rbotright(){return br_;}
		
		point_type topright(){return point_type(br_.x(), tl_.y());}
		
		point_type botleft() {return point_type(tl_.x(), br_.y());}
		
		point_type center()const{return (tl_ + br_) / 2;}
		
		size_type dimension() { return size_type( width(), height() ); }
		
		value_type width()  const{ return oden::difference( tl_.x(), br_.x() ); }
		value_type height() const{ return oden::difference( tl_.y(), br_.y() ); }
		
	  // 調べる
		bool is_empty()const { return !is_valid(); }
		bool is_null()const  { return (tl_.x() == br_.x() && tl_.y() == br_.y()); }
		bool is_valid()const { return (tl_.x() < br_.x()) && (tl_.y() < br_.y()); }
		
	  // 長方形が裏返っていれば元に戻す
		self normalized()const 
		{
			if( is_empty() && !is_null() ) {
				return self(br_,tl_);
			}
			return *this;
		}

	  // 平行移動
		void shift( value_type dx, value_type dy )
		{
			tl_.shift( dx, dy );
			br_.shift( dx, dy );
		}
		void shift( const point_type& offs )
		{
			tl_.shift( offs );
			br_.shift( offs );
		}
		self shifted( value_type dx, value_type dy )const
		{
			return self( tl_.shifted( dx, dy ), br_.shifted( dx, dy ) );
		}
		self shifted( const point_type& offs )const
		{
			return self( tl_.shifted( offs ), br_.shifted( offs ) );
		}
			
	  // 拡大
		void grow( value_type d )
		{
			grow( d, d );
		}
		void grow( value_type dx, value_type dy )
		{
			tl_.shift( -dx, -dy );
			br_.shift( dx, dy );
		}
		self grown( value_type dx, value_type dy )const
		{
			self rc( *this );
			rc.grow( dx, dy );
			return rc;
		}
		
	  // 調整
		void adjust( value_type dl, value_type dt, value_type dr, value_type db )
		{
			tl_.shift( dl, dt );
			br_.shift( dr, db );
		}
		self adjusted( value_type dl, value_type dt, value_type dr, value_type db )const
		{
			self rc(*this);
			rc.adjust( dl, dt, dr, db );
			return rc;
		}
		
	  // 包含判定
		bool contains( const point_type& point )const
		{
			return (tl_.x() <= point.x() && point.x() <= br_.x()) &&
				   (tl_.y() <= point.y() && point.y() <= br_.y());
		}
		bool contains( const self& rect )const
		{
			return contains(rect.tl_) &&
				   contains(rect.br_);
		}
		bool contains_proper( const point_type& point )const
		{
			return (tl_.x() < point.x() && point.x() < br_.x()) &&
				   (tl_.y() < point.y() && point.y() < br_.y());
		}
		bool contains_proper( const self& rect )const
		{
			return contains_proper(rect.tl_) &&
				   contains_proper(rect.br_);
		}
		
	  // 設定
		void set( const point_type &topleft, const point_type &botright )
		{
			tl_ = topleft;
			br_ = botright;
		}
		void set( value_type l, value_type t, value_type r, value_type b )
		{
			tl_.set( l, t );
			br_.set( r, b );
		}
		void set( const size_type &sz, const point_type &topleft )
		{
			tl_ = topleft;
			set_dimension( sz );
		}

		void set_left( value_type x ) 
		{
			rleft() = x;	
		}
		void set_top( value_type y )
		{
			rtop() = y;	
		}
		void set_right( value_type x ) 
		{
			rright() = x;	
		}
		void set_bottom( value_type y ) 
		{
			rbottom() = y;	
		}
		
		void set_topleft( const point_type &point )
		{
			tl_ = point;
		}
		void set_botright( const point_type &point )
		{
			br_ = point;
		}
		void set_topright( const point_type &point )
		{
			tl_.set( point.x(), tl_.y() );
			br_.set( br_.x(), point.y() );
		}
		void set_botleft( const point_type &point )
		{
			tl_.set( tl_.x(), point.y() );
			br_.set( point.x(), br_.y() );
		}
		
	  // 左上は固定で幅・高さを変化
		void set_width( value_type w )
		{
			br_.rx() = tl_.x() + w;
		}
		void set_height( value_type h ) 
		{
			br_.ry() = tl_.y() + h;
		}
		void set_dimension( const size_type& dm )
		{
			set_width( dm.width() );
			set_height( dm.height() );
		}
		
	  // 各成分を移動させる
		void move_topleft( const point_type& to ) 
		{
			shift( to - topleft() );
		}
		void move_botright( const point_type& to ) 
		{
			shift( to - botright() );
		}
		void move_topright( const point_type& to )
		{
			shift( to - topright() );
		}
		void move_botleft( const point_type& to ) 
		{
			shift( to - botleft() );
		}
		void move_center( const point_type& to ) 
		{
			shift( to - center() );
		}
		void move_top( value_type dy )
		{
			shift( 0, dy );
		}
		void move_bottom( value_type dy ) 
		{
			shift( 0, -dy );
		}
		void move_left( value_type dx ) 
		{
			shift( dx, 0 );
		}
		void move_right( value_type dx )
		{
			shift( -dx, 0 );
		}
			
	  // OR結合
		self united( const self& rc )const
		{
			return self(
				std::min( left(), rc.left() ),
				std::min( top(), rc.top() ),
				std::max( right(), rc.right() ),
				std::max( bottom(), rc.bottom() )
				);
		}
		
	  // 交差判定
		bool intersects( const self& rect )const
		{
			return (left()  <= rect.right()  ) &&
			       (right() >= rect.left()   ) &&
				   (top()   <= rect.bottom() ) && 
				   (bottom()>= rect.top()    );
		}
		
	  // AND結合
		self intersected( const self& rc )const
		{
			if( !intersects( rc ) )
				return self();
			return self(
				std::max( left(),   rc.left()   ),
				std::max( top(),    rc.top()    ),
				std::min( right(),  rc.right()  ),
				std::min( bottom(), rc.bottom() )
				);
		}
		
	  // 縦割りにする
	  	bool divide_lengthwise( value_type at, self& part1, self& part2 )const
	  	{
			if( at < left() || right() < at )
				return false;
		
	  		self tmp1;
	  		tmp1.set_topleft( tl_ );
	  		tmp1.set_right( at );
	  		tmp1.set_bottom( bottom() );
	  		
	  		part2.set_topleft( tmp1.topright() );
	  		part2.set_botright( br_ );
	  		
	  		part1 = tmp1;			
			return true;
	  	}
	  	
	  // 横割りにする
	  	bool divide_widthwise( value_type at, self& part1, self& part2 )const
	  	{
			if( at < top() || bottom() < at )
				return false;
				
	  		self tmp1;
	  		tmp1.set_topleft( tl_ );
	  		tmp1.set_right( right() );
	  		tmp1.set_bottom( at );
	  		
	  		part2.set_topleft( tmp1.botleft() );
	  		part2.set_botright( br_ );
	  		
	  		part1 = tmp1;
			return true;
	  	}
		
	// 演算子オーバーロード
		bool operator==( const self& src )const
		{
			return ((tl_ == src.tl_) && (br_ == src.br_));
		}
		self& operator+=( const self& src )
		{
			tl_ += src.tl_; br_ += src.br_;
			return (*this);
		}
		self& operator-=( const self& src )
		{
			tl_ -= src.tl_; br_ -= src.br_;
			return (*this);
		}
		
	// swap
		void swap( self& other )
		{
			tl_.swap( other.tl_ );
			br_.swap( other.br_ );	
		}
		friend void swap( self& l, self& r )
		{
			l.swap(r);
		}		
		
	private:
		point_type tl_;
		point_type br_;
	};


//
//
//
	typedef pointT<long>	point;
	typedef sizeT<long>		size;
	typedef rectT<long>		rect;

	typedef pointT<float>	pointF;
	typedef sizeT<float>	sizeF;
	typedef rectT<float>	rectF;

	typedef pointT<double>	pointR;
	typedef sizeT<double>	sizeR;
	typedef rectT<double>	rectR;
}


#endif
