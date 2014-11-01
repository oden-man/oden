#ifndef ODEN_BUFFER_SBUF
#define ODEN_BUFFER_SBUF

#include <istream>
#include <vector>
#include <oden/oage/defs.h>
#include <oden/oage/algorithm.h>

namespace oden { namespace oage 
{

//####################################################
//  ファイルバッファリング
//
	template< int StaticBuf = -1, typename Offs = offset_type >
	class stream_buffer
	{
	// バッファクラス
		// スタック
		template< int Size > 
		struct inner_buffer 
		{ 
			typedef byte_type* type;
			byte_type buf[Size]; 
			
			byte_type* ptr() { return &buf; }
			const byte_type* ptr()const { return &buf; }
			int size()const { return Size; }
			
			inner_buffer( int ){}
		};
		
		// フリーストア
		template<>
		struct inner_buffer<-1> 
		{ 
			typedef std::vector<byte_type> type;
			type buf; 
			
			byte_type* ptr() { return &buf[0]; }
			const byte_type* ptr()const { return &buf[0]; }
			int size()const { return buf.size(); }
			
			inner_buffer( int sz ) : buf(sz) {}
		};
		
		inner_buffer<StaticBuf> buf_;
	
	public:
	// typedef
		typedef inner_buffer<StaticBuf> buffer_type;
		typedef Offs offset_type;
		
	//  コンストラクタ
		stream_buffer( std::istream& fi, int unit = 1, int v = 1024 )
			: fi_(fi), buf_(v), buf_offs_next_(0), head_(0), unit_(unit)
		{}

	// メンバ関数
	  // バッファ単位で読む:
		bool read()
		{
			return this->load_next();
		}
		
	  // 1単位ぶん読み進める : 
		bool read_one()
		{
			if( this->is_buffer_full() )
			{
				if( !this->load_next() )
					return false;
			}
			buf_offs_next_ += unit_;

			return true;
		}	
		template< typename X > 
		bool read_one( X& out )
		{
			if( !this->read_one() )
				return false;

			return this->get(out);
		}
		
      // オフセット位置まで読み込む:
		bool seek( Offs offs )
		{
			while( offs-head_ >= buf_.size() )
			{
				if( !this->load_next() )
					return false;
			}
			buf_offs_next_ = offs-head_ + unit_;

			return true;
		}
		
	  // 現在位置の値を読み取る
		template< typename X > 
		X get()const
		{
			T val = 0; this->get(val); 
			return val;
		}
		template< typename X > 
		bool get( X& out )const
		{ 
			if( buffer_pos() >= size_ )
				return false;
			
			oage::write( buf_.ptr()+buffer_pos(), unit_, reinterpret_cast<byte_type*>(&out), sizeof(X) );
			return true;
		}		

	  // バッファの現在位置に値を書き込む
		template< typename X > 
		bool put( const X& src )
		{
			if( buffer_pos() >= size_ )
				return false;
			
			oage::write( reinterpret_cast<const byte_type*>(&src), sizeof(X), buf_.ptr()+buffer_pos(), unit_ );
			return true;
		}
		
	  // reset
		void reset()
		{
			fi.seekg(0);
			buf_offs_next_ = 0;
			head_ = 0;
			size_ = 0;			
		}

	// アクセス
	  // バッファ
		const typename buffer_type::type& buffer()const { return buf_.buf; }
		byte_type*       buffer_ptr()           		{ return buf_.ptr(); }
		const byte_type* buffer_ptr()const      		{ return buf_.ptr(); }
		int              buffer_size()const     		{ return size_; }
		int              buffer_capacity()const 		{ return buf_.size(); }
	  // 位置
		bool is_buffer_full ()const { return buf_offs_next_ >= (Offs)size_; }
		Offs buffer_pos ()const
		{ 
			if( buf_offs_next_ < unit_ )
				return 0;
			return buf_offs_next_ - unit_;
		}
		Offs nextpos()const { return head_+buf_offs_next_; }
		Offs pos()const     { return head_+buffer_pos(); }
		Offs readed()const  { return head_+size_; }
	  // ストリーム
		std::istream& stream()const { return fi_; }

	private:
	// 内部
	  // バッファに読み込む
		bool load_next()
		{
			if( fi_.eof() )
				return false;

			if( !fi_.read( reinterpret_cast<char*>(buf_.ptr()), buf_.size() ) && !fi_.eof() )
				return false;
			
			head_ += size_;
			size_ = static_cast<int>(fi_.gcount());			
			buf_offs_next_ = 0;
			
			return true;
		}
		
	// メンバ変数
		Offs buf_offs_next_;
		Offs head_;
		int size_;
		
		std::istream& fi_;
		int unit_;
	};

}}

#endif

