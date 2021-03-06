#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include <muduo/base/LogStream.h>
#include <muduo/base/Timestamp.h>

namespace muduo {
#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
#define RED "\033[31m" /* Red */
#define GREEN "\033[32m" /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BLUE "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m" /* Cyan */
#define WHITE "\033[37m" /* White */
#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */
#define BOLDRED "\033[1m\033[31m" /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m" /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m" /* Bold White */

class TimeZone;

class Logger {
public:
	enum LogLevel {
		TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NUM_LOG_LEVELS,
	};

	// compile time calculation of basename of source file
	class SourceFile {
	public:
		template<int N>
		inline SourceFile(const char (&arr)[N]) :
				data_(arr), size_(N - 1) {
			const char* slash = strrchr(data_, '/'); // builtin function
			if (slash) {
				data_ = slash + 1;
				size_ -= static_cast<int>(data_ - arr);
			}
		}

		explicit SourceFile(const char* filename) :
				data_(filename) {
			const char* slash = strrchr(filename, '/');
			if (slash) {
				data_ = slash + 1;
			}
			size_ = static_cast<int>(strlen(data_));
		}

		const char* data_;
		int size_;
	};

	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level, const char* func,
			int nonmean);
	Logger(SourceFile file, int line, LogLevel level, const char* func);
	Logger(SourceFile file, int line, bool toAbort);
	~Logger();

	LogStream& stream() {
		return impl_.stream_;
	}

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);

	typedef void (*OutputFunc)(const char* msg, int len);
	typedef void (*FlushFunc)();
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);
	static void setTimeZone(const TimeZone& tz);

private:

	class Impl {
	public:
		typedef Logger::LogLevel LogLevel;
		Impl(LogLevel level, int old_errno, const SourceFile& file, int line,
				const char* func);
		void formatTime();
		void finish();

		Timestamp time_;
		LogStream stream_;
		LogLevel level_;
		int line_;
		SourceFile basename_;
		const char* _func;
	};

	Impl impl_;
	// static LogLevel g_logLevel;
};

// Logger::LogLevel Logger::g_logLevel;
extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel() {
	return g_logLevel;
}

//
// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define LOG_TRACE if (muduo::Logger::logLevel() <= muduo::Logger::TRACE) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (muduo::Logger::logLevel() <= muduo::Logger::DEBUG) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__,0).stream()
#define LOG_INFO if (muduo::Logger::logLevel() <= muduo::Logger::INFO) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::INFO,__func__,0).stream()
#define LOG_WARN if (muduo::Logger::logLevel() <= muduo::Logger::WARN) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN,__func__,0).stream()
#define LOG_ERROR if (muduo::Logger::logLevel() <= muduo::Logger::ERROR) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR,__func__,0).stream()
#define LOG_FATAL if (muduo::Logger::logLevel() <= muduo::Logger::FATAL) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL,__func__,0).stream()
#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::muduo::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template<typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr) {
	if (ptr == NULL) {
		Logger(file, line, Logger::FATAL).stream() << names;
	}
	return ptr;
}

}

#endif  // MUDUO_BASE_LOGGING_H
