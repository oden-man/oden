#include "../Pch.h"

#include <cmath>

#include <oden/gammo/ui/layout/base.h>
#include <oden/gammo/ui/layout/box.h>
#include <oden/gammo/ui/layout/div.h>
#include <oden/gammo/ui/layout/ratio.h>

#include <oden/gammo/ui/widget/object.h>

namespace oden { namespace gammo
{
//##########################################################
//               
//

//
template< typename T >
long get_size_component( layout_orientation ori, const T& dim )
{
	return ori == layout_horizontal ? dim.width() : dim.height();
}
template< typename T >
long& ref_size_component( layout_orientation ori, T& dim )
{
	return ori == layout_horizontal ? dim.rwidth() : dim.rheight();
}

// 
template< typename E >
int get_layout_elem_size( layout_orientation ori, const layout_container_elem<E>& e )
{
	if( e.has_layout() ) {
		if( e.size_value() == layout_best_size ) {
			return get_size_component( ori, e.layout()->best_size() );
		}
	}
	
	return e.size_value();
}
template< typename E >
size get_layout_elem_dimen( layout_orientation ori, const layout_container_elem<E>& e )
{
	if( e.has_layout() ) {
		if( e.size_value() == layout_best_size ) {
			return e.layout()->best_size();
		}
	}
	
	size dim;
	ref_size_component( ori, dim ) = e.size_value();
	return dim;
}

//##########################################################
//// term_layout
//
void term_layout::recalc( const rect& this_rect )
{
	if(wid_)
		wid_->set_widget_location( this_rect.grown(-space_,-space_) );
}

//
size term_layout::best_size()const
{
	size dm;
	if( wid_ )
		wid_->get_best_size(dm);
	
	dm.rwidth()  += space_ * 2;
	dm.rheight() += space_ * 2;
	
	return dm;
}


//------------------------------------------------------------------------------------------------//
//
//                               レイアウトオブジェクト
//
//------------------------------------------------------------------------------------------------//

//##########################################################
//----------------------------------------------------------------------- FILL
//
void fill_layout::recalc( const rect& this_rect )
{ 
	if( layout_object* p = elems().get() )
		p->recalc(this_rect);
}
size fill_layout::best_size()const
{ 
	if( layout_object* p = elems().get() )
		return p->best_size();
	return size();
}

//##########################################################
//---------------------------------------------------------------------- PAD
//
void pad_layout::recalc( const rect& this_rect )
{
	if( layout_object* p = elems().get() )
		p->recalc( this_rect.adjusted(left_, top_, -right_, -bottom_) );
		
}
size pad_layout::best_size()const
{
	size dim;
	if( layout_object* p = elems().get() )
		dim = p->best_size();

	return dim + size(left_+right_, top_+bottom_);
}

//##########################################################
//---------------------------------------------------------------------- BOX
//
void box_layout::recalc( const rect& this_rect )
{
	rect loca( this_rect );
	
  // 初期ラインを形成
	if( orient_ == layout_horizontal ) 
		loca.set_width(0);
	else	
		loca.set_height(0);
		
	auto eitr = elems().end();
	
  // まず固定幅の合計を求める
	int blocksum = 0;
	for( auto itr = elems().begin(); itr != eitr; ++itr )
	{
		if( itr->value() == box_layout_block )
			blocksum += get_layout_elem_size( orient_, *itr );		
	}
	
  // 変動比率の合計、変動部全長を求める
	int springsum = 0;
	for( auto itr = elems().begin(); itr != eitr; ++itr )
	{
		if( itr->value() == box_layout_spring )
			springsum += itr->size_value();		
	}
	int springlength = std::max( get_size_component(orient_, this_rect) - blocksum, (long)0 );
	
  // 固定幅だけなら中心に寄せる
	if( springsum == 0 )
	{
		divide_space_( loca, springlength/2 );
	}
	
  // 動かす
	for( auto itr=elems().begin(); itr!=eitr; ++itr )
	{
		if( itr->value() == box_layout_spring ) {
			divide_space_( loca, static_cast<long>( static_cast<double>(springlength * itr->size_value()) / springsum ) );
			
			if( boost::next(itr) == eitr )
				loca.rbotright() = this_rect.botright(); // 小数点の誤差を飲み込む
		}
		else {
			divide_space_( loca, get_layout_elem_size(orient_, *itr) );
		}
		
		if( itr->has_layout() )
			itr->layout()->recalc( loca );		
	}
}

// 
void box_layout::divide_space_( rect& out, long length )
{
	if( orient_ == layout_horizontal ) {
		out.rleft() = out.right();
		out.rright() = out.left() + length;
	}
	else {
		out.rtop() = out.bottom();
		out.rbottom() = out.top() + length;
	}
}
		
		
#if 0	
	iterator itr = begin(), eitr = end();
	for(; itr!=eitr; ++itr )
	{
		int propo = itr->second;
		
		if( is_vertical_ ) {
			loca.rtop() = loca.bottom();
			if( boost::next(itr) == eitr )
				loca.rbottom() = this_rect.bottom();
			else
				loca.rbottom() = loca.top() + static_cast<long>( static_cast<double>(this_rect.height() * propo) / proportion_sum_ );
		}
		else {
			loca.rleft() = loca.right();
			if( boost::next(itr) == eitr )
				loca.rright() = this_rect.right();
			else
				loca.rright() = loca.left() + static_cast<long>( static_cast<double>(this_rect.width() * propo) / proportion_sum_ );
		}
		
		if( layout_object* child = itr->first )
			child->recalc( loca );
	}
#endif

// ベストサイズ
size box_layout::best_size( )const
{
	size dim;
	
	auto itr = elems().begin(), eitr = elems().end();
	for(; itr!=eitr; ++itr )
	{
		if( itr->value() == box_layout_block && itr->size_value() != layout_best_size ){
			ref_size_component( orient_, dim ) += itr->size_value(); 
		}
		else if( itr->has_layout() ) {
			dim += itr->layout()->best_size();
		}
		else { /* レイアウトのないスプリング */
			//ref_size_component( orient_, dim ) += 10 //?
		}
	}
	return dim;
}

//##########################################################
//----------------------------------------------------------------------- DIV
//
void div_layout::recalc( const rect& this_rect )
{
  // 現在残っているスペース
	rect space( this_rect );
	
  // 各アイテムが順にスペースを切り出していく
	auto itr  = elems().begin(); 
	auto eitr = elems().end();
	for(; itr!=eitr; ++itr )
	{
		rect items_loca;
		
		div_layout_place place = itr->value();		
		bool no_space = false;

		if( div_layout_rest == place ) {
			items_loca = space;	// 余りスペースの処理
			no_space = true;
		}
		else if( div_layout_top == place ) {
			no_space = !space.divide_widthwise( space.top() + get_layout_elem_size( layout_horizontal, *itr ), items_loca, space );
		}
		else if( div_layout_left == place ) {
			no_space = !space.divide_lengthwise( space.left() + get_layout_elem_size( layout_vertical, *itr ), items_loca, space );
		}
		else if( div_layout_right == place ) {
			no_space = !space.divide_lengthwise( space.right() - get_layout_elem_size( layout_vertical, *itr ), space, items_loca );
		}
		else if( div_layout_bottom == place ) {
			no_space = !space.divide_widthwise( space.bottom() - get_layout_elem_size( layout_horizontal, *itr ), space, items_loca );
		}
		
		if( no_space )
			space = rect();		
		
		// アイテムを移動
		if( itr->has_layout() )
			itr->layout()->recalc( items_loca );
	}
}

size div_layout::best_size( )const
{
	size dim;
	
	auto ritr  = elems().value().rbegin();
	auto reitr = elems().value().rend();
	for(; ritr!=reitr; ++ritr)
	{
		layout_orientation my_orient = layout_horizontal;

		div_layout_place place = ritr->value();
		if( place == div_layout_rest )
			continue;
		else if( place == div_layout_left || place == div_layout_right  ) my_orient = layout_horizontal;
		else if( place == div_layout_top  || place == div_layout_bottom ) my_orient = layout_vertical;
			
		size my_bs = get_layout_elem_dimen( my_orient, *ritr );
		
		if( my_orient == layout_horizontal ) {
			dim.rwidth() += my_bs.width();
			dim.rheight() = std::max( dim.height(), my_bs.height() );
		}
		else {
			dim.rwidth() = std::max( dim.width(), my_bs.width() );
			dim.rheight() += my_bs.height();
		}
	}
	
	return dim;
}


//##########################################################
//---------------------------------------------------------------------- RATIO
//
void ratio_layout::recalc( const rect& this_rect )
{
	rect out;
	
	double rw = this_rect.width()  / static_cast<double>(w_);
	double rh = this_rect.height() / static_cast<double>(h_);
	
	if( std::fabs( rw - rh ) < 0.00000001 ) /*  誤差範囲は適当 */
	{
		out = this_rect;
	}
	else if( rw > rh )
	{
		out.rtop() = this_rect.top();
		out.rbottom() = this_rect.bottom();
		
		long d_w = static_cast<long>( ( this_rect.width() - w_ * rh ) / 2 );
		
		out.rleft() = this_rect.left() + d_w;
		out.rright() = this_rect.right() - d_w;
	}
	else if( rw < rh )
	{
		out.rleft() = this_rect.left();
		out.rright() = this_rect.right();
		
		long d_h = static_cast<long>( ( this_rect.height() - h_ * rw ) / 2 );
		
		out.rtop() = this_rect.top() + d_h;
		out.rbottom() = this_rect.bottom() - d_h;	
	}
	
	if( layout_object* p = elems().get() )
		p->recalc(out);
}

size ratio_layout::best_size()const
{	
	size cbs;
	if( layout_object* p = elems().get() )
		cbs = p->best_size();
	else
		return size();
	
	double rw = cbs.width()  / static_cast<double>(w_);
	double rh = cbs.height() / static_cast<double>(h_);
	if( std::fabs( rw - rh ) < 0.00000001 ) /*  誤差範囲は適当 */
	{
		return cbs;
	}
	else if( rw > rh )
	{
		return size( cbs.width(), static_cast<long>( rh*static_cast<double>(h_) ) );
	}
	else if( rw < rh )
	{
		return size( static_cast<long>( rw*static_cast<double>(w_) ), cbs.height() );
	}
	
	return cbs;
}



} /* end of namespace gammo */
} /* end of namespace oden */
