#include "../Pch.h"

#include <oden/gammo/ui/control/progress_bar.h>

namespace oden { namespace gammo
{

//############################################
// ProgressBar
//

// バーの先端位置を設定
int progress_bar::set_progress( int pos )const
{
	return static_cast<int>( send_message( PBM_SETPOS, pos, 0 ) );
}
int progress_bar::progression()const
{
	return static_cast<int>( send_message( PBM_GETPOS, 0, 0 ) );
}

// バーの先端位置を進める
int progress_bar::advance()const
{
	return static_cast<int>( send_message( PBM_STEPIT, 0, 0 ) );
}
int progress_bar::advance( int delta )const
{
	return static_cast<int>( send_message( PBM_DELTAPOS, delta, 0 ) );
}
int progress_bar::set_advance_step( int step )const
{
	return static_cast<int>( send_message( PBM_SETSTEP, step, 0 ) );
}

//
void progress_bar::clear()const
{
	set_progress( range_low() );
}
void progress_bar::fill()const
{
	set_progress( range_high() );
}

//
void progress_bar::set_range( int min, int max )const
{
	send_message( PBM_SETRANGE32, min, max );
}
void progress_bar::set_range_high( int v )const
{
	set_range( range_low(), v );
}
void progress_bar::set_range_low( int v )const
{
	set_range( v, range_high() );
}

//
std::pair<int,int> progress_bar::range()const
{
	PBRANGE rng;
	send_message( PBM_GETRANGE, 0, reinterpret_cast<LPARAM>(&rng) );
	return std::pair<int,int>( rng.iLow, rng.iHigh );
}
int progress_bar::range_high()const
{
	return send_message(PBM_GETRANGE, FALSE, 0);
}
int progress_bar::range_low()const
{
	return send_message(PBM_GETRANGE, TRUE, 0);
}

// バーの色を設定
colorref progress_bar::set_bar_color( colorref col )const
{
	return send_message( PBM_SETBARCOLOR, 0, col.value() );	
}
colorref progress_bar::bar_color()const
{
	return send_message( PBM_GETBARCOLOR, 0, 0 );	
}

// 背景の色を設定
colorref progress_bar::set_back_color( colorref col )const
{
	return send_message( PBM_SETBKCOLOR, 0, col.value() );	
}
colorref progress_bar::back_color()const
{
	return send_message( PBM_GETBKCOLOR, 0, 0 );	
}

//
void progress_bar::set_progress_state( int state )const
{
	send_message( PBM_SETSTATE, state, 0 );
}
int progress_bar::progress_state()const
{
	return static_cast<int>( send_message( PBM_GETSTATE, 0, 0 ) );	
}

// マーキーモード
void progress_bar::start_marquee( int speed )const
{
	send_message( PBM_SETMARQUEE, TRUE, speed );
}
void progress_bar::end_marquee()const
{
	send_message( PBM_SETMARQUEE, FALSE, 0 );	
}

// 
//"""""""""""""""""""""""""""""""""""""""""""""
/*
//
void progress_bar::SetSmoothMode()const
{
	LONG style=GetWindowLong(hWnd,GWL_STYLE);
	if(style & PBS_MARQUEE)
		style&=(~PBS_MARQUEE);
	style|=PBS_SMOOTH;
	SetWindowLong(hWnd,GWL_STYLE,style);
}

//
void progress_bar::SetMarqueeMode()const
{
	LONG style=GetWindowLong(hWnd,GWL_STYLE);
	if(style & PBS_SMOOTH)
		style&=(~PBS_SMOOTH);
	style|=PBS_MARQUEE;
	SetWindowLong(hWnd,GWL_STYLE,style);
}*/


}}