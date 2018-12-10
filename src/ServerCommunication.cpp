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
#include <stdlib.h>

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

void signalCB(int sig) {
	if (sig == SIGSEGV) {
		LOG_ERROR << "[SIGSEGV] Got a interrupt signal  =====prog exit !";
	} else if (sig == SIGINT) {  //<2>2 中断（等同 Ctrl + C）
		LOG_ERROR
				<< "[SIGINT] Got a segmentation violation signal  =====prog exit !";
	} else if (sig == SIGILL) {
		LOG_ERROR
				<< "[SIGILL] Got an illegal instruction signal  =====prog exit !";
	} else if (sig == SIGKILL) {  //9 强制终止
		LOG_ERROR << "[SIGKILL] Got an kill 9 signal  =====prog exit !";
	} else if (sig == SIGABRT) { //abort : stack
		LOG_ERROR << "[SIGABRT] Got an abort signal  =====prog exit !";
	} else {
		LOG_ERROR << "Got an unknow signal  =====prog exit ! sig: " << sig;
	}
	exit(0);
}

void signalHandle() {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSEGV, signalCB);
	signal(SIGINT, signalCB);
	signal(SIGILL, signalCB);
	signal(SIGKILL, signalCB);
	signal(SIGABRT, signalCB);
}

int main() {
	//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
	LOG_ERROR
			<< "===========ServerCommunication program starting !==============";
	signalHandle();
	//logging setting
	g_logFile.reset(new muduo::LogFile(LogFileName, g_LogFileMaxSize));
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

