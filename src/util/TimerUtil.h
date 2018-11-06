/*
 * Timer.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_TIMER_H_
#define UTIL_TIMER_H_
#include <ThreadUtil.h>
#include "DateTime.h"
#include "DebugFuncCall.h"
#include <vector>
using namespace std;
class TimerUtil: virtual public ThreadUtil {
public:
	TimerUtil() {
		_bExit = false;
		//DebugFuncCall::DebugConstruct(DEBUGFUNCCALL);
	}
	virtual ~TimerUtil() {
		//DebugFuncCall::DebugDestruct(DEBUGFUNCCALL);
		destroy();
	}
	void destroy() {
		if (isAlive()) {
			DebugFuncCall::DebugFunc(DEBUGFUNCCALL);
			_bExit = true;
			waitForExit();
			_timer.clear();
		}
	}
	virtual void onTimer(int nID) = 0;
	void setTimer(int nID, int msec, bool immediateRun = false) {
		sTimer timer;
		timer.nID = nID;
		timer.nInterval = msec;
		if (immediateRun)
			timer.lastTime = DateTime::milliSeconds(0);
		else
			timer.lastTime = DateTime::now();
		{
			AutoLock lock(&_mutex);
			for (size_t i = 0; i < _timer.size(); i++) {
				if (_timer[i].nID == nID)
					return;
			}
			_timer.push_back(timer);
		}
		if (!isAlive())
			start();
	}
	void killTimer(int nID) {
		vector<sTimer>::iterator it = _timer.begin();
		while (it != _timer.end()) {
			if (it->nID == nID) {
				_timer.erase(it);
				return;
			}
			it++;
		}
	}
	virtual void run() {
		while (!_bExit) {
			vector<int> vIDTemp;
			{
				AutoLock lock(&_mutex);
				for (size_t i = 0; i < _timer.size(); i++) {
					DateTime interval = DateTime::now() - _timer[i].lastTime;
					if (interval.toMilliSeconds() >= _timer[i].nInterval) {
						//time is can execute
						_timer[i].lastTime = DateTime::now();
						vIDTemp.push_back(_timer[i].nID);
					} //end if
				} //end for
			}
			for (size_t i = 0; i < vIDTemp.size(); i++) {
				onTimer(vIDTemp[i]);
			}
			ThreadUtil::Sleep(20);
		}
	}
private:
	struct sTimer {
		int nID;
		uint64_t nInterval;
		DateTime lastTime;
	};
	Mutex _mutex;
	vector<sTimer> _timer;
	bool _bExit;
};

#endif /* UTIL_TIMER_H_ */
