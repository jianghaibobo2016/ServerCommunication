/*
 * AutoLock.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_AUTOLOCK_H_
#define UTIL_AUTOLOCK_H_
#include <assert.h>
#include "Mutex.h"
class AutoLock {
public:
	AutoLock(Mutex* pMutex){
		_pMutex = pMutex;
		assert(_pMutex);
		_pMutex->lock();
	}
	virtual ~AutoLock(){
		_pMutex->unlock();
	}
private:
	Mutex* _pMutex;
};

#endif /* UTIL_AUTOLOCK_H_ */
