/*
 * CircleBoundedQueue.h
 *
 *  Created on: Dec 13, 2018
 *      Author: jhb
 */

#ifndef SRC_CIRCLEBOUNDEDQUEUE_H_
#define SRC_CIRCLEBOUNDEDQUEUE_H_

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <assert.h>

#include <stdio.h>
using namespace muduo;

template<typename T>
class CircleBoundedQueue: boost::noncopyable {
public:
	explicit CircleBoundedQueue(int maxSize) :
			mutex_(), notEmpty_(mutex_), notFull_(mutex_), queue_(maxSize) {
	}

	void put(const T& x) {
		MutexLockGuard lock(mutex_);
//		while (queue_.full()) {
//			notFull_.wait();
//		}
//		assert(!queue_.full());
//		queue_.push_back(x);
//		notEmpty_.notify();

		queue_.push_back(x);
		if (!queue_.full()) {
			notEmpty_.notify();
		}

//		printf("queue_[queue_.capacity() - 1]: %s\n",
//				queue_[queue_.capacity() - 1].irRcvCode);
//
//		printf("rrrrrrrrrrrrrrrrcv IR Code len %d\r\n",
//				queue_[queue_.capacity() - 1].irRcvLen);
//		for (unsigned int i = 0; i < queue_[queue_.capacity() - 1].irRcvLen;
//				i++) {
//			printf("0x%0.2x ", queue_[queue_.capacity() - 1].irRcvCode[i]);
//			if ((i + 1) % 16 == 0)
//				printf("\r\n");
//		}
//		printf("\r\n");
//		std::cout << "" << queue_[queue_.capacity() - 1] << std::endl;

	}

	T take() {
		MutexLockGuard lock(mutex_);
		while (queue_.empty()) {
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		notFull_.notify();
		return front;
	}

	bool empty() const {
		MutexLockGuard lock(mutex_);
		return queue_.empty();
	}

	bool full() const {
		MutexLockGuard lock(mutex_);
		return queue_.full();
	}

	size_t size() const {
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

	size_t capacity() const {
		MutexLockGuard lock(mutex_);
		return queue_.capacity();
	}

private:
	mutable MutexLock mutex_;
	Condition notEmpty_ GUARDED_BY(mutex_);
	Condition notFull_ GUARDED_BY(mutex_);
	boost::circular_buffer<T> queue_ GUARDED_BY(mutex_);
};

#endif /* SRC_CIRCLEBOUNDEDQUEUE_H_ */
