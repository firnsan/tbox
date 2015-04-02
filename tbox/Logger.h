#ifndef TBOX_LOGGER_H
#define TBOX_LOGGER_H

#include "LogStream.h"

namespace tbox
{

	class Logger
	{
	public:
		enum LogLevel
		{
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL,
			NUM_LOG_LEVELS,
		};

		Logger(const char *filename, int line);
		Logger(const char *filename, int line, LogLevel level);
		~Logger();

		LogStream &stream() { return stream_; }

		static LogLevel logLevel() { return globalLevel_; }
		static void setLogLevel(LogLevel level);

		typedef void (*OutputFunc)(const char *msg, int len);
		typedef void (*FlushFunc)();
		static void setOutput(OutputFunc);
		static void setFlush(FlushFunc);

	private:
		void finish();
		void extraInfo(); //输出时间、线程id

		LogStream stream_;
		const char *basename_;
		int line_;
		LogLevel level_; //每次打log时的暂时level

		static LogLevel globalLevel_; //全局level
		static OutputFunc output_;
		static FlushFunc flush_;
	};

#define LOG_INFO if (tbox::Logger::logLevel() <= tbox::Logger::INFO) \
		tbox::Logger(__FILE__,  __LINE__).stream()
#define LOG_FATAL if (tbox::Logger::logLevel() <= tbox::Logger::FATAL) \
		tbox::Logger(__FILE__,  __LINE__, tbox::Logger::FATAL).stream()
}
#endif