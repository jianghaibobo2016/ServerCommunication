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
	BaseServerHandle(EventLoop *loop, const InetAddress &listenAddr);
	virtual ~BaseServerHandle();

//	TcpServer &getTcpServer() {
//		return _server;
//	}

	void startServerHandle(int numThreads = 0);

private:

	virtual void onConnection(const TcpConnectionPtr &connPtr)=0;

	virtual void onMessage(const TcpConnectionPtr &connPtr, Buffer *buff,
			Timestamp time) = 0;

private:
	TcpServer _server;
};

#endif /* BASESERVERHANDLE_H_ */
