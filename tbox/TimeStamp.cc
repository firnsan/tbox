#include <sys/time.h>
#include <stdio.h>


#include <tbox/TimeStamp.h>

namespace tbox
{
	TimeStamp::TimeStamp(int64_t microSecs)
		: microSecSinceEpoch_(microSecs)
	{
	}

	int64_t TimeStamp::now()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);

		//time_t seconds = tv.tv_sec; //time_t是long类型，在32位下不够保存micro
		int64_t seconds = tv.tv_sec;
		suseconds_t microSecs = tv.tv_usec;

		return static_cast<int64_t>(seconds*1000*1000 + microSecs);

	}
}
