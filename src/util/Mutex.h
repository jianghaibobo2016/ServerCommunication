/*
 * Mutex.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_MUTEX_H_
#define UTIL_MUTEX_H_
#include <assert.h>
#include <pthread.h>
#include <iostream>
using namespace std;
#include "SystemException.h"
class Mutex {
public:
	Mutex() {
		int ret;
		ret = pthread_mutex_init(&_mutex, 0);
		assert(ret == 0);
		if (ret != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		//cout<<"create mutex"<<endl;
	}
	virtual ~Mutex() {
		int ret = pthread_mutex_destroy(&_mutex);
		assert(ret == 0);
		if (ret != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		//cout<<"delete mutex"<<endl;
	}
	void lock() {
		int ret = pthread_mutex_lock(&_mutex);
		if (ret != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);

	}
	void unlock() {
		int ret = pthread_mutex_unlock(&_mutex);
		if (ret != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
	}
private:
	pthread_mutex_t _mutex;
};

#endif /* UTIL_MUTEX_H_ */
