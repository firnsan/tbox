#ifndef TBOX_TIMESTAMP_H
#define TBOX_TIMESTAMP_H

#include <stdint.h>

namespace tbox
{
	class TimeStamp
	{
	public:
		TimeStamp()
			: microSecSinceEpoch_(0)
		{
		}
		
		explicit TimeStamp(int64_t microSec);

		static int64_t now();

	private:
		int64_t microSecSinceEpoch_;
	};
}

#endif