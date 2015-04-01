#include <stdio.h>

#include "LogStream.h"

namespace tbox
{
	namespace detail
	{
		//template class FixedBuffer<LogStream::kSmallBuffer>;
		template class FixedBuffer<kSmallBuffer>;
	}

	LogStream &LogStream::operator<<(const char *str)
	{
		if (str) {
			buffer_.append(str, strlen(str));
		}
		else {
			buffer_.append("(NULL)", 6);
		}

		return *this;
	}

	LogStream &LogStream::operator<<(int num)
	{
		if (buffer_.avail() >= kMaxNumericSize) {
			int n = snprintf(buffer_.cur(), kMaxNumericSize, "%d", num);
			buffer_.add(n);
		}
		return *this;
	}
}