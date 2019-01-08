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
#include <BuildTime.h>

#include "ServerHandle.h"
#include "UDPServerHandle.h"
#include "NodeInfo.h"
#include "GlobalProfile.h"
#include "CtrlBoardHandle.h"
#include "LogicHandle.h"
#include "TaskRestore.h"

//led & lcd
#include "dp_udrv_led.h"
#include "dp_udrv_lcd.h"

//#include "testhead.h"

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
	//jhbnote deinit free
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSEGV, signalCB);
	signal(SIGINT, signalCB);
	signal(SIGILL, signalCB);
	signal(SIGKILL, signalCB);
	signal(SIGABRT, signalCB);
}

int main() {
	//logging setting
	g_logFile.reset(new muduo::LogFile(LogFileName, g_LogFileMaxSize));
	muduo::Logger::setOutput(outputFunc);
	muduo::Logger::setFlush(flushFunc);
	muduo::Logger::setLogLevel(muduo::Logger::TRACE);
	LOG_TRACE << "Build time : " << BuildTime;
	LOG_TRACE << "Software Version : " << SoftVersion;
	LOG_TRACE
			<< "===========ServerCommunication program starting !==============";
	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
	setvbuf(stdout, (char *) NULL, _IOLBF, 0);
//	print(&loop);

#if 1
	signalHandle();
	//init
	sleep(2);
	muduo::Singleton<NodeInfo>::instance();
	muduo::Singleton<LogicHandle>::instance();
	GlobalProfile::getInstance();
	muduo::Singleton<TaskRestore>::instance().startRunning(32, 2);

#if 0
	DP_UDRV_LCD_Init();
	const DP_CHAR *ip =
			muduo::Singleton<NodeInfo>::instance().getNetInfo().getNetConfStruct().ipAddr.c_str();
//	DP_UDRV_LCD_SetBackLight(1);

	DP_UDRV_LCD_ShowString(0, 0, Welcome, strlen(Welcome));
	DP_UDRV_LCD_ShowString(0, 1, ip, strlen(ip)); // jhbnote add ETH0/1
	DP_UDRV_LCD_ShowString(0, 2, Working, strlen(Working));

//	DP_S32 DP_UDRV_LCD_DeInit();
//	return 0;

#endif

#if 0 // led
	DP_UDRV_LED_Init();
//
//	LOG_INFO<<"close all ";
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_ALL, 0);
//	sleep(2);
//	LOG_INFO<<"open all ";
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX0, 1);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 1);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 1);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 1);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX4, 1);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX5, 1);
//
//	sleep(2);
//	LOG_INFO<<"close all ";
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_ALL, 0);
//	sleep(2);
//	LOG_INFO<<"open all ";
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_ALL, 1);

//	sleep(2);
//	LOG_INFO<<"close all ";
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX0, 0);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 0);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX4, 0);
//	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX5, 0);
//
//	return 0;
#endif

//	std::cout << "00000000000000000000" << std::endl;
//	OpenAndMoveWindow_S winData;
//	muduo::Singleton<TaskRestore>::instance().setDataToJson(winData );
//	std::cout << "777777777777" << std::endl;

	EventLoop loop; // one loop shared by multiple servers
	ServerHandle tcpServer(&loop, InetAddress(5010));
	tcpServer.startServerHandle(5);
	//jhbnote one thread try?
	tcpServer.startHandle(20, 3);

	EventLoopThread UDPServer;
	UDPServerHandle udpServer(UDPServer.startLoop());
	udpServer.startListen();
#endif
#if 0
	EventLoopThread ctrlBoardHandle;
	CtrlBoardHandle boardHandle(ctrlBoardHandle.startLoop());
	boardHandle.startRunning();
#endif
	loop.loop();
}

#endif

#if 0
#include <muduo/base/BlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <memory>
#include <string>
#include <stdio.h>
#include <unistd.h>

class Test {
public:
	Test(int numThreads) :
	latch_(numThreads), threads_(numThreads) {
		for (int i = 0; i < numThreads; ++i) {
			char name[32];
			snprintf(name, sizeof name, "work thread %d", i);
			threads_.push_back(
					new muduo::Thread(boost::bind(&Test::threadFunc, this),
							muduo::string(name)));
		}
		for_each(threads_.begin(), threads_.end(),
				boost::bind(&muduo::Thread::start, _1));
	}

	void run(int times) {
		printf("waiting for count down latch\n");
		latch_.wait();
		printf("all threads started\n");
		for (int i = 0; i < times; ++i) {
			char buf[32];
			snprintf(buf, sizeof buf, "hello %d", i);
			queue_.put(buf);
			printf("tid=%d, put data = %s, size = %zd\n",
					muduo::CurrentThread::tid(), buf, queue_.size());
		}
	}

	void joinAll() {
		for (size_t i = 0; i < threads_.size(); ++i) {
			queue_.put("stop");
		}

		for_each(threads_.begin(), threads_.end(),
				boost::bind(&muduo::Thread::join, _1));
	}

private:

	void threadFunc() {
		printf("tid=%d, %s started\n", muduo::CurrentThread::tid(),
				muduo::CurrentThread::name());

//		sleep(1);
		latch_.countDown();
		bool running = true;
		while (running) {
//			usleep(1000);
			std::string d(queue_.take());
			printf("tid=%d, get data = %s, size = %zd\n",
					muduo::CurrentThread::tid(), d.c_str(), queue_.size());
			running = (d != "stop");
		}

		printf("tid=%d, %s stopped\n", muduo::CurrentThread::tid(),
				muduo::CurrentThread::name());
	}

	muduo::BlockingQueue<std::string> queue_;
	muduo::CountDownLatch latch_;
	boost::ptr_vector<muduo::Thread> threads_;
};

void testMove() {
//#ifdef __GXX_EXPERIMENTAL_CXX0X__

// std::unique_ptr requires gcc 4.4 or later
//#if __GNUC_PREREQ (4,4)
	muduo::BlockingQueue<std::unique_ptr<int>> queue;
	queue.put(std::unique_ptr<int>(new int(42)));
	std::unique_ptr<int> x = queue.take();
	printf("took %d\n", *x);
	*x = 123;
	queue.put(std::move(x));
	std::unique_ptr<int> y = queue.take();
	printf("took %d\n", *y);
//#endif
//
//#endif
}

int main() {
	printf("pid=%d, tid=%d\n", ::getpid(), muduo::CurrentThread::tid());
	Test t(5);
	t.run(100);
//	sleep(100);
	t.joinAll();
	testMove();

	printf("number of created threads %d\n", muduo::Thread::numCreated());
}
#endif
