#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Logger.h"

namespace tbox
{
	Logger::LogLevel initLogLevel()
	{
		if (::getenv("LOGGER_DEBUG"))
			return Logger::DEBUG;
		else
			return Logger::INFO;
	}

	void defaultOutput(const char *msg, int len)
	{
		size_t n = fwrite(msg, len, 1, stdout);
		(void)n; //skip -Werror=unused-variable
	}

	void defaultFlush()
	{
		fflush(stdout);
	}


	Logger::LogLevel Logger::globalLevel_ = initLogLevel(); //不是常量表达式也行
	Logger::OutputFunc Logger::output_ = defaultOutput;
	Logger::FlushFunc Logger::flush_ = defaultFlush;

	Logger::Logger(const char *filename, int line)
		: stream_(),
		  basename_(filename),
		  line_(line),
		  level_(INFO)
	{
		//new(this) Logger(...);
		const char *slash = strrchr(filename, '/');
		if (slash) {
			basename_ = slash + 1;
		}
	}


	Logger::Logger(const char *filename, int line, LogLevel level)
		: stream_(),
		  basename_(filename),
		  line_(line),
		  level_(level)
	{
		const char *slash = strrchr(filename, '/');
		if (slash) {
			basename_ = slash + 1;
		}
	}

	Logger::~Logger()
	{
		finish();
		const LogStream::Buffer &buf = stream().buffer();
		output_(buf.data(), buf.length());

		if (level_ == FATAL) {
			flush_();
			abort();
		}
	}

	void Logger::finish()
	{
		stream_ << " - " << basename_ << ":" << line_ << "\n";
	}


	void Logger::setLogLevel(LogLevel level)
	{
		globalLevel_ = level;
	}
}