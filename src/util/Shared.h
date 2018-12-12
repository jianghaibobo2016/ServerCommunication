/*
 * Shared.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_SHARED_H_
#define UTIL_SHARED_H_

#include "AutoLock.h"
#include "Mutex.h"
#include <iostream>
class Shared {
public:
	Shared() :
			_ref(0) {

	}
	Shared(const Shared&) :
			_ref(0) {

	}
	virtual ~Shared() {

	}
	Shared& operator=(const Shared&) {
		return *this;
	}
	void __incRef() {
		AutoLock lock(&_mutex);
		_ref++;
	}
	void __decRef() {
		{
			AutoLock lock(&_mutex);
			_ref--;
		}

		if (_ref <= 0) {
			delete this;
			//::std::cout<<"delete shared object"<<::std::endl;
		}
	}
	int __getRef() const {
		return _ref;
	}
private:
	Mutex _mutex;
	int _ref;

};

#endif /* UTIL_SHARED_H_ */
