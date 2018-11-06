/*
 * MyFastTcpClient.h
 *
 *  Created on: Oct 22, 2018
 *      Author: jhb
 */

#ifndef SRC_MYFASTTCPCLIENT_H_
#define SRC_MYFASTTCPCLIENT_H_

#include <boost/function.hpp>
#include "FastTcpClient.h"


typedef boost::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;

class MyFastTcpClient: public FastTcpClient {
public:
	MyFastTcpClient(EventLoop* loop, const InetAddress& serverAddr,
			const MessageCallback &cb = MessageCallback()) :
			FastTcpClient(loop, serverAddr) {
		_client.setMessageCallback(cb);
	}
	virtual ~MyFastTcpClient() {
	}

	virtual void onMessage(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, muduo::Timestamp receiveTime) {
		muduo::PrintBuff::printBufferByHex("recv:(onmessage)",
				buf->toStringPiece().data(), buf->toStringPiece().size());

	}
};

#endif /* SRC_MYFASTTCPCLIENT_H_ */
