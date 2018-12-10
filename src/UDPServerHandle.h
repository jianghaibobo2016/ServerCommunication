/*
 * UDPServerHandle.h
 *
 *  Created on: Oct 29, 2018
 *      Author: jhb
 */

#ifndef SRC_UDPSERVERHANDLE_H_
#define SRC_UDPSERVERHANDLE_H_
#include <muduo/net/EventLoop.h>
#include "NetTrans.h"
using namespace muduo;
using namespace muduo::net;

class UDPServerHandle: public NetTrans {
public:
	UDPServerHandle(EventLoop *loop);
	~UDPServerHandle();

	void startListen();

private:
	EventLoop *_loop;
	void runUdp();
	void parserDataRecv(DP_U8 *buff, DP_U32 len, struct sockaddr_in recvAddr);
	void devSearchHandle(DP_BOOL isMulticast, struct sockaddr_in recvAddr);

};

#endif /* SRC_UDPSERVERHANDLE_H_ */
