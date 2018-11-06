/*
 * DateTime.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_DATETIME_H_
#define UTIL_DATETIME_H_
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <assert.h>
//#include <string>
//using namespace std;
class DateTime {
public:
	DateTime() {
		_usec = 0;
	}

	virtual ~DateTime() {

	}

	static DateTime now() {
		struct timeval tv;
		if (gettimeofday(&tv, 0) < 0) {
			assert(0);
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		}
		return DateTime(tv.tv_sec * uint64_t(1000000) + tv.tv_usec);
	}
	static DateTime seconds(uint64_t sec) {
		return DateTime(sec * uint64_t(1000000));
	}
	static DateTime milliSeconds(uint64_t msec) {
		return DateTime(msec * uint64_t(1000));
	}
	static DateTime microSeconds(uint64_t usec) {
		return DateTime(usec);
	}
	std::string toDateString() const{
		time_t time = static_cast<long>(_usec / 1000000);
		struct tm* t;
		struct tm tr;
		localtime_r(&time, &tr);
		t = &tr;
		char buf[100];
		if (strftime(buf, sizeof(buf), "%Y-%m-%d", t) == 0) {
			return std::string();
		}
		return std::string(buf);
	}
	std::string toTimeString() const{
		time_t time = static_cast<long>(_usec / 1000000);
		struct tm* t;
		struct tm tr;
		localtime_r(&time, &tr);
		t = &tr;
		char buf[100];
		if (strftime(buf, sizeof(buf), "%H:%M:%S", t) == 0) {
			return std::string();
		}
		return std::string(buf);
	}
	std::string toDateTimeString() const{
		time_t time = static_cast<long>(_usec / 1000000);
		struct tm* t;
		struct tm tr;
		localtime_r(&time, &tr);
		t = &tr;
		char buf[100];
		if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t) == 0) {
			return std::string();
		}
		return std::string(buf);
	}

	uint64_t toSeconds() const {
		return _usec / uint64_t(1000000);
	}
	uint64_t toMilliSeconds() const {
		return _usec / uint64_t(1000);
	}
	uint64_t toMicroSeconds() const {
		return _usec;
	}

	DateTime operator-() const {
		return DateTime(-_usec);
	}

	DateTime operator-(const DateTime& rhs) const {
		return DateTime(_usec - rhs._usec);
	}

	DateTime operator+(const DateTime& rhs) const {
		return DateTime(_usec + rhs._usec);
	}

	DateTime& operator+=(const DateTime& rhs) {
		_usec += rhs._usec;
		return *this;
	}

	DateTime& operator-=(const DateTime& rhs) {
		_usec -= rhs._usec;
		return *this;
	}

	bool operator<(const DateTime& rhs) const {
		return _usec < rhs._usec;
	}

	bool operator<=(const DateTime& rhs) const {
		return _usec <= rhs._usec;
	}

	bool operator>(const DateTime& rhs) const {
		return _usec > rhs._usec;
	}

	bool operator>=(const DateTime& rhs) const {
		return _usec >= rhs._usec;
	}

	bool operator==(const DateTime& rhs) const {
		return _usec == rhs._usec;
	}

	bool operator!=(const DateTime& rhs) const {
		return _usec != rhs._usec;
	}

private:
	DateTime(uint64_t usec) {
		_usec = usec;
	}
	uint64_t _usec;
};

#endif /* UTIL_DATETIME_H_ */
