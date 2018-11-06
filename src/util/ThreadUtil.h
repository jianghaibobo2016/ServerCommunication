/*
 * ThreadUtil.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_THREAD_H_
#define UTIL_THREAD_H_
#include "Shared.h"
#include "Mutex.h"
#include "AutoLock.h"
#include <pthread.h>
#include <unistd.h>
class ThreadUtil : virtual public Shared{
public:
	ThreadUtil(){
		_thread = 0;
		_running = false;
	}
	virtual ~ThreadUtil(){
	}

	virtual void run() = 0;

	//開始線程
	void start(){
		int ret;
		{
			AutoLock lock(&_stateMutex);
			if (_running)
				throw SystemException("error:thread is running");
			_running = true;
		}
		ret = pthread_create(&_thread, 0, doThread, this);
		if (ret != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
	}

	bool isAlive(){
		AutoLock lock(&_stateMutex);
		return _running;
	}

	//等待線程推出
	void waitForExit() {
		if (isAlive()) {
			pthread_join(_thread, 0);
		}
	}

	static void* doThread(void* pVoid){
		ThreadUtil* pThis = (ThreadUtil*) pVoid;
		pThis->run();
		pThis->_done();
		return 0;
	}

	void _done(){
		AutoLock lock(&_stateMutex);
		_running = false;
	}

	static void Sleep(int ms){
		usleep(ms*1000);
	}
private:
	 pthread_t _thread;
	 Mutex _stateMutex;
	 bool _running;
};

#endif /* UTIL_THREAD_H_ */
