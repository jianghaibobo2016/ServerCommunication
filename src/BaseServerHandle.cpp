/*
 * BaseServerHandle.cpp
 *
 *  Created on: Oct 16, 2018
 *      Author: jhb
 */
#include "BaseServerHandle.h"
#include <boost/bind.hpp>

BaseServerHandle::BaseServerHandle(EventLoop *loop,
		const InetAddress &listenAddr) :
		_server(loop, listenAddr, "BaseServerHandle") {
	_server.setConnectionCallback(
			boost::bind(&BaseServerHandle::onConnection, this, _1));
	_server.setMessageCallback(
			boost::bind(&BaseServerHandle::onMessage, this, _1, _2, _3));
//	_server.setThreadNum(2);

}

BaseServerHandle::~BaseServerHandle() {
}

void BaseServerHandle::startServerHandle(int numThreads) {
	_server.setThreadNum(numThreads);
	_server.start();
}

