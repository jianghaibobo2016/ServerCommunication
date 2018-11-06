/*
 * FastTcpClient.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: jhb
 */

#include <src/FastTcpClient.h>

FastTcpClient::FastTcpClient(EventLoop* loop, const InetAddress& serverAddr) :
		_client(loop, serverAddr, "MyFastTcpClient") {
	_client.setConnectionCallback(
			boost::bind(&FastTcpClient::onConnection, this, _1));
//	_client.setMessageCallback(
//			boost::bind(&FastTcpClient::onMessage, this, _1, _2, _3));
}

FastTcpClient::~FastTcpClient() {
}

void FastTcpClient::send(TcpConnection* conn, Buffer *buff) {
	muduo::PrintBuff::printBufferByHex("send: ", buff->toStringPiece().data(),
			buff->toStringPiece().size());
	conn->send(buff);
}

void FastTcpClient::send(TcpConnection* conn, const void* data, int len) {
	conn->send(StringPiece(static_cast<const char*>(data), len));
}

void FastTcpClient::onConnection(const TcpConnectionPtr& conn) {
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
			<< conn->peerAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");

	MutexLockGuard lock(mutex_);
	if (conn->connected()) {
		connection_ = conn;
	} else {
		connection_.reset();
	}
}
