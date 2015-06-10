#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tbox/Logger.h>
#include <tbox/TimeStamp.h>
#include <tbox/Thread.h>



namespace tbox
{
	const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
		{
			"TRACE ",
			"DEBUG ",
			"INFO  ",
			"WARN  ",
			"ERROR ",
			"FATAL ",
		};

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

		extraInfo();
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

		extraInfo();
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

	void Logger::extraInfo()
	{
		const int microSecPerSec = 1000*1000;

		int64_t microSecSinceEpoch = TimeStamp::now();
		time_t sec = static_cast<time_t>(microSecSinceEpoch/(microSecPerSec));
		int microSec = static_cast<int>(microSecSinceEpoch%(microSecPerSec));

		//根据秒数，获取日期，精确到秒
		struct tm dayAndTime;
		char temp[128];
		localtime_r(&sec, &dayAndTime);
		snprintf(temp, sizeof(temp), "%4d%02d%02d %02d:%02d:%02d",
			dayAndTime.tm_year+1900, dayAndTime.tm_mon+1,
			dayAndTime.tm_mday, dayAndTime.tm_hour,
			dayAndTime.tm_min, dayAndTime.tm_sec);
		stream_ << temp;

		snprintf(temp, sizeof(temp), ".%06d ", microSec);
		stream_ << temp;

		stream_ << Thread::currentTidStr();
		stream_ << LogLevelName[level_];
	}

	void Logger::setLogLevel(LogLevel level)
	{
		globalLevel_ = level;
	}
}