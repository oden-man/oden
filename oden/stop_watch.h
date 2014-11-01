#pragma once

#ifndef ODEN_SWCH
#define ODEN_SWCH

//################################################
//   インクルード
//

// ------------ boost -----------------
#include <boost/chrono.hpp>

//
//################################################


namespace oden
{
	template< typename Reso = boost::chrono::milliseconds >
	class stop_watch
	{
	public:		
		typedef boost::chrono::high_resolution_clock::time_point tp;
		typedef typename Reso::rep count_type;
		
		stop_watch(){ restart(); }
	
		void restart() 
		{
			beg_ = boost::chrono::high_resolution_clock::now();
		}
		count_type stop()
		{ 
			tp end_ = boost::chrono::high_resolution_clock::now();
			return boost::chrono::duration_cast<Reso>( end_-beg_ ).count();
		}
		
	private:
		 tp beg_;
	}; 
	
	typedef stop_watch<boost::chrono::milliseconds>  milli_stop_watch;
	typedef stop_watch<boost::chrono::microseconds>  micro_stop_watch;
}


#endif
