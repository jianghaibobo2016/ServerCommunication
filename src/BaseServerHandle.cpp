/*
 * BaseServerHandle.cpp
 *
 *  Created on: Oct 16, 2018
 *      Author: jhb
 */
#include "BaseServerHandle.h"
#include <boost/bind.hpp>
#include <muduo/net/EventLoop.h>

BaseServerHandle::BaseServerHandle(EventLoop *loop,
		const InetAddress &listenAddr, int idleSeconds) :
		_idleSeconds(idleSeconds), _server(loop, listenAddr, "BaseServerHandle") {
	_server.setConnectionCallback(
			boost::bind(&BaseServerHandle::onConnection, this, _1));
	_server.setMessageCallback(
			boost::bind(&BaseServerHandle::onMessage, this, _1, _2, _3));
	  loop->runEvery(1.0, std::bind(&BaseServerHandle::onTimer, this));
//	  dumpConnectionList();
//	_server.setThreadNum(2);

}

BaseServerHandle::~BaseServerHandle() {
}

void BaseServerHandle::startServerHandle(int numThreads) {
	_server.setThreadNum(numThreads);
	_server.start();
}

void BaseServerHandle::onTimer() {
//	dumpConnectionList();
	Timestamp now = Timestamp::now();
	for (WeakConnectionList::iterator it = connectionList_.begin();
			it != connectionList_.end();) {
		TcpConnectionPtr conn = it->lock();
		if (conn) {
			Node* n = boost::any_cast<Node>(conn->getMutableContext());
			double age = timeDifference(now, n->lastReceiveTime);
			if (age > _idleSeconds) {
				if (conn->connected()) {
					conn->shutdown();
					LOG_INFO << "shutting down " << conn->name();
					conn->forceCloseWithDelay(3.5); // > round trip of the whole Internet.
				}
			} else if (age < 0) {
				LOG_WARN << "Time jump";
				n->lastReceiveTime = now;
			} else {
				break;
			}
			++it;
		} else {
			LOG_WARN << "Expired";
			it = connectionList_.erase(it);
		}
	}
}

void BaseServerHandle::dumpConnectionList() const {
	LOG_INFO << "size = " << connectionList_.size();

	for (WeakConnectionList::const_iterator it = connectionList_.begin();
			it != connectionList_.end(); ++it) {
		TcpConnectionPtr conn = it->lock();
		if (conn) {
			printf("conn %p\n", get_pointer(conn));
			const Node& n = boost::any_cast<const Node&>(conn->getContext());
			printf("    time %s\n", n.lastReceiveTime.toString().c_str());
		} else {
			printf("expired\n");
		}
	}
}
