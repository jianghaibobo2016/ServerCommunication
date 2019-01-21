/*
 * BaseServerHandle.h
 *
 *  Created on: Oct 16, 2018
 *      Author: jhb
 */

#ifndef BASESERVERHANDLE_H_
#define BASESERVERHANDLE_H_

#include <muduo/net/TcpServer.h>
//#include "../muduo/net/TcpServer.h"
#include <boost/noncopyable.hpp>
#include <muduo/base/Logging.h>
#include <boost/weak_ptr.hpp>
//#include <memory>
//#include <iostream>
#include <list>
using namespace muduo;
using namespace muduo::net;

class BaseServerHandle: boost::noncopyable {
public:
//#pragma pack(push)
//#pragma pack(1)
//	typedef struct _Msg_S_ {
//		char str[5];
//		int num;
//		int count;
//	} _Msg_S;
//#pragma pack(pop)
	BaseServerHandle(EventLoop *loop, const InetAddress &listenAddr,
			int idleSeconds);
	virtual ~BaseServerHandle();

//	TcpServer &getTcpServer() {
//		return _server;
//	}

	void startServerHandle(int numThreads = 0);

protected:

	virtual void onConnection(const TcpConnectionPtr &connPtr)=0;

	virtual void onMessage(const TcpConnectionPtr &connPtr, Buffer *buff,
			Timestamp time) = 0;

protected:

	void onTimer();

	void dumpConnectionList() const;

	typedef boost::weak_ptr<TcpConnection> WeakTcpConnectionPtr;
	typedef std::list<WeakTcpConnectionPtr> WeakConnectionList;

	struct Node: public muduo::copyable {
		Timestamp lastReceiveTime;
		WeakConnectionList::iterator position;
	};

	int _idleSeconds;
	TcpServer _server;
	WeakConnectionList connectionList_;
};

#endif /* BASESERVERHANDLE_H_ */
