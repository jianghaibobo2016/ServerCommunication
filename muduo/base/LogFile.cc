#include <muduo/base/LogFile.h>

#include <muduo/base/FileUtil.h>
#include <muduo/base/ProcessInfo.h>
#include <muduo/base/Logging.h>
#include "PrintBuff.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>  // delete dir rmdir
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace muduo;

LogFile::LogFile(const string& basename, off_t rollSize, bool threadSafe,
		int flushInterval, int checkEveryN) :
		basename_(basename), rollSize_(rollSize), flushInterval_(flushInterval), checkEveryN_(
				checkEveryN), count_(0), mutex_(
				threadSafe ? new MutexLock : NULL), startOfPeriod_(0), lastRoll_(
				0), lastFlush_(0) {
//	assert(basename.find('/') == string::npos);
	rollFile();
}

LogFile::~LogFile() {
}

void LogFile::append(const char* logline, int len) {

	if (mutex_) {
		MutexLockGuard lock(*mutex_);
		append_unlocked(logline, len);
	} else {
		append_unlocked(logline, len);
	}
}

void LogFile::flush() {
	if (mutex_) {
		MutexLockGuard lock(*mutex_);
		file_->flush();
	} else {
		file_->flush();
	}
}

void LogFile::append_unlocked(const char* logline, int len) {

//	file_->append(logline, len);

//	std::cout << "file_->writtenBytes(): " << file_->writtenBytes()
//			<< "roolsie: " << rollSize_ << " basename_ : " << basename_
//			<< std::endl;
	if (file_->writtenBytes() > rollSize_) {
//		std::cout
//				<< "(file_->writtenBytes() > rollSize_(file_->writtenBytes() > rollSize_(file_->writtenBytes!!!!!"
//				<< std::endl;

//		LOG_DEBUG << "Log file will clean -> 0";

//		std::cout << "cleannnnnnnnnnn!!!!!" << std::endl;
		file_->cleanFile();
//		std::cout << "Cleaned file !" << std::endl;
//		LOG_DEBUG << "Log file cleaned .";

//		rollFile();
	} else {

//		std::cout
//				<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<!!!!!file_->writtenBytes(): "
//				<< file_->writtenBytes() << "rollSize_: " << rollSize_
//				<< std::endl;
//		++count_;
//		if (count_ >= checkEveryN_) {
//			count_ = 0;
//			time_t now = ::time(NULL);
//			time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
//			if (thisPeriod_ != startOfPeriod_) {
//				rollFile();
//			} else if (now - lastFlush_ > flushInterval_) {
//				lastFlush_ = now;
//				file_->flush();
//			}
//		}
	}
	file_->append(logline, len);
}

bool LogFile::rollFile() {
	time_t now = 0;
	string filename = getLogFileName(basename_, &now);
	time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

	if (now > lastRoll_) {
		lastRoll_ = now;
		lastFlush_ = now;
		startOfPeriod_ = start;

		file_.reset(new FileUtil::AppendFile(filename));

		return true;
	}
	return false;
}

//check file existed!
//	if (0 != access(filename.c_str(), 0)) {
//
//	}

string LogFile::getLogFileName(const string& basename, time_t* now) {
	string filename;
	string pathName;
	filename.reserve(basename.size() + 64);

	filename = basename;

	int pos = filename.rfind('/');

	pathName = filename.substr(0, pos);

	if (0 != access(pathName.c_str(), 0)) {
		int ret = mkdir(pathName.c_str(), S_IRWXO);
		if (ret == -1) {
			LOG_SYSERR<<"Can not create log path: "<<pathName;
		}
	}

	char timebuf[32];
	struct tm tm;
	*now = time(NULL);
	gmtime_r(now, &tm); // FIXME: localtime_r ?
	strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
//	filename += timebuf;

//	filename += ProcessInfo::hostname();

	char pidbuf[32];
	snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::pid());
//	filename += pidbuf;

	filename += ".log";

	return filename;
}

