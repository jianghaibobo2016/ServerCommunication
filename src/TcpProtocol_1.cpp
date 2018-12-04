#if 1
/*
 * TcpProtocol_1.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: jhb
 */

#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Singleton.h>
#include <boost/bind.hpp>
#include <muduo/base/LogFile.h>
#include <muduo/base/FileUtil.h>
#include <signal.h>

#include <stdio.h>
#include <unistd.h>
#include "ServerHandle.h"
#include "UDPServerHandle.h"
#include "NodeInfo.h"
#include "GlobalProfile.h"
#include "testhead.h"

using namespace muduo;
using namespace muduo::net;

void print(EventLoop* p = NULL) {
	printf("main(): pid = %d, tid = %d, loop = %p\n", getpid(),
			CurrentThread::tid(), p);
}

void init(EventLoop* p) {
	printf("init(): pid = %d, tid = %d, loop = %p\n", getpid(),
			CurrentThread::tid(), p);
}

boost::scoped_ptr<muduo::LogFile> g_logFile;

void outputFunc(const char* msg, int len) {
	g_logFile->append(msg, len);
}

void flushFunc() {
	g_logFile->flush();
}

int main() {
	//jhbnote todo : signal handle :sync data
	signal(SIGPIPE, SIG_IGN);

	//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
	//logging setting
	g_logFile.reset(new muduo::LogFile(LogFileName, LogFileMaxSize));
	muduo::Logger::setOutput(outputFunc);
	muduo::Logger::setFlush(flushFunc);

	//init
	muduo::Singleton<NodeInfo>::instance();
	GlobalProfile::getInstance();

	EventLoop loop; // one loop shared by multiple servers
	print(&loop);
	ServerHandle tcpServer(&loop, InetAddress(5010));
	tcpServer.startServerHandle(1);
	tcpServer.startHandle(10, 3);

	EventLoopThread UDPServer;
	UDPServerHandle udpServer(UDPServer.startLoop());
	udpServer.startListen();

	loop.loop();
}

#endif

