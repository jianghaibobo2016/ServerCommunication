/*
 * FastTcpClient.h
 *
 *  Created on: Oct 22, 2018
 *      Author: jhb
 */

#ifndef SRC_FASTTCPCLIENT_H_
#define SRC_FASTTCPCLIENT_H_

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/TcpClient.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <unistd.h>
#include "interactivepro.h"
#include "dp_sd_type.h"
#include "PrintBuff.h"

using namespace muduo;
using namespace muduo::net;

class FastTcpClient: boost::noncopyable {
public:
	FastTcpClient(EventLoop* loop, const InetAddress& serverAddr);
	virtual ~FastTcpClient();

	void connect() {
		_client.connect();
	}

	void send(TcpConnection* conn, Buffer *buff);
	void send(TcpConnection* conn,const void* data, int len);
	void disconnect() {
		_client.disconnect();
	}

	void setTcpRetry() {
		_client.enableRetry();
	}

protected:
	TcpClient _client;
	virtual void onMessage(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, muduo::Timestamp receiveTime)=0;
private:
	void onConnection(const TcpConnectionPtr& conn);

//	void onStringMessage(const TcpConnectionPtr&, const string& message,
//			Timestamp) {
//		printf("<<< %s\n", message.c_str());
//	}
	MutexLock mutex_;
	TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

#endif /* SRC_FASTTCPCLIENT_H_ */
