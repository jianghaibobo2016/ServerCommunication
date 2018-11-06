/*
 * ServerHandle.h
 *
 *  Created on: Oct 16, 2018
 *      Author: jhb
 */

#ifndef SERVERHANDLE_H_
#define SERVERHANDLE_H_

#include "BaseServerHandle.h"
#include <muduo/base/ThreadPool.h>

//class ThreadPool;

class ServerHandle: public BaseServerHandle {
public:
	ServerHandle(EventLoop *loop, const InetAddress &listenAddr);
	virtual ~ServerHandle();

	void startHandle(int maxQueueSize = 0, int numThreads = 0);

private:
	virtual void onConnection(const TcpConnectionPtr &connPtr);

	virtual void onMessage(const TcpConnectionPtr &connPtr, Buffer *buff,
			Timestamp time);

	bool isUsingThreadPool;
	muduo::ThreadPool _threadPool;

};

#endif /* SERVERHANDLE_H_ */
