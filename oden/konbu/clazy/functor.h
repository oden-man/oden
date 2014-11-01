
#ifndef ODEN_KONBU_CLAZY_FUNCTOR
#define ODEN_KONBU_CLAZY_FUNCTOR

//#######################################
//
//
// ---------- STL -------------
// ---------- boost.pp --------
// ---------- boost -----------
// ---------- oden ------------
// ---------- konbu -----------

//
//#######################################

namespace oden { namespace konbu { namespace clazy
{
  // �O���錾
	template< typename L, typename R >
	class assign_;
	
	template< typename L, typename R >
	class subscript_;
	
//_____________________________________________________________________________________________________
//
//                               �x���]���̃x�[�X�ƂȂ�
//_____________________________________________________________________________________________________
//
//#######################################################################
// �S�Ẵ����_�t�@���N�^�̃x�[�X�ɂȂ�
//	
	template< typename Derived >
	class functor
	{
	private:
		typedef functor<Derived> self;
	public:
	// ���^��
		const Derived& to_derived()const { return static_cast<const Derived&>(*this); }
	// operators
		template< typename Right >
		assign_< Derived, Right > operator=( const Right& r )const;

		template< typename Right >
		subscript_< Derived, Right > operator[]( const Right& r )const;
	};	

} /* end of namespace clazy */
}}

#endif


