/*
 * TcpProtocol_1.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: jhb
 */

#if 1
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Singleton.h>
#include <boost/bind.hpp>
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

int main() {
	signal(SIGPIPE, SIG_IGN);
//	std::cout<< "sizeof ::::"<<sizeof(DP_M2S_AVDEC_GET_INFO_S)<<std::endl;

#if 1
	muduo::Singleton<NodeInfo>::instance();

	GlobalProfile::getInstance();

//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
	EventLoop loop; // one loop shared by multiple servers
	print(&loop);
	ServerHandle tcpServer(&loop, InetAddress(5010));
	tcpServer.startServerHandle(1);
	tcpServer.startHandle(10, 3);

	EventLoopThread UDPServer;
	UDPServerHandle udpServer(UDPServer.startLoop());
	udpServer.startListen();

	loop.loop();
#endif
}

#endif

#if 0
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

EventLoop* g_loop;

void callback()
{
	printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	EventLoop anotherLoop;
}

void threadFunc()
{
	printf("threadFunc(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

	assert(EventLoop::getEventLoopOfCurrentThread() == NULL);
	EventLoop loop;
	assert(EventLoop::getEventLoopOfCurrentThread() == &loop);
	loop.runAfter(1.0, callback);
	loop.loop();
}

int main()
{
	printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

	assert(EventLoop::getEventLoopOfCurrentThread() == NULL);
	EventLoop loop;
	assert(EventLoop::getEventLoopOfCurrentThread() == &loop);

	Thread thread(threadFunc);
	thread.start();

	loop.loop();
}
#endif

#if 0
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/CountDownLatch.h>

#include <boost/bind.hpp>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

void print(EventLoop* p = NULL)
{
	printf("print: pid = %d, tid = %d, loop = %p\n",
			getpid(), CurrentThread::tid(), p);
}

void quit(EventLoop* p)
{
	print(p);
	p->quit();
}

int main()
{
	print();

	{
		EventLoopThread thr1;  // never start
	}

	{
		// dtor calls quit()
		EventLoopThread thr2;
		EventLoop* loop = thr2.startLoop();
		loop->runInLoop(boost::bind(print, loop));
		CurrentThread::sleepUsec(1000 * 1000);
	}

	{
		// quit() before dtor
		EventLoopThread thr3;
		EventLoop* loop = thr3.startLoop();
		loop->runInLoop(boost::bind(quit, loop));
		CurrentThread::sleepUsec(1000 * 1000);
	}
}

#endif

#if 0
#include <muduo/base/ThreadPool.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>
#include <stdio.h>
#include <unistd.h>  // usleep
#include <iostream>

void print()
{
	printf("tid=%d\n", muduo::CurrentThread::tid());
}

void printString(const std::string& str)
{
	LOG_INFO << str;
	usleep(100*1000);
}

void test(int maxSize)
{
	LOG_WARN << "Test ThreadPool with max queue size = " << maxSize;
	muduo::ThreadPool pool("MainThreadPool");
	pool.setMaxQueueSize(maxSize);
	pool.start(5);

	LOG_WARN << "Adding";
	pool.run(print);
	pool.run(print);
	for (int i = 0; i < 100; ++i)
	{
		char buf[32];
		snprintf(buf, sizeof buf, "task %d", i);
		pool.run(boost::bind(printString, std::string(buf)));
	}
	LOG_WARN << "Done";

	muduo::CountDownLatch latch(2);
	pool.run(boost::bind(&muduo::CountDownLatch::countDown, &latch));
	latch.wait();
	std::cout<<"111"<<std::endl;
	pool.stop();
}

int main()
{
	test(10);
//  test(1);
//  test(5);
//  test(10);
//  test(50);
}

#endif

#if 0
#include <muduo/base/ThreadLocalSingleton.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test : boost::noncopyable
{
public:
	Test()
	{
		printf("tid=%d, constructing %p\n", muduo::CurrentThread::tid(), this);
	}

	~Test()
	{
		printf("tid=%d, destructing %p %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
	}

	const muduo::string& name() const {return name_;}
	void setName(const muduo::string& n) {name_ = n;}

private:
	muduo::string name_;
};

void threadFunc(const char* changeTo)
{
	printf("tid=%d, %p name=%s\n",
			muduo::CurrentThread::tid(),
			&muduo::ThreadLocalSingleton<Test>::instance(),
			muduo::ThreadLocalSingleton<Test>::instance().name().c_str());
//  muduo::ThreadLocalSingleton<Test>::instance().setName(changeTo);
	printf("tid=%d, %p name=%s\n",
			muduo::CurrentThread::tid(),
			&muduo::ThreadLocalSingleton<Test>::instance(),
			muduo::ThreadLocalSingleton<Test>::instance().name().c_str());

	// no need to manually delete it
	// muduo::ThreadLocalSingleton<Test>::destroy();
}

int main()
{
	muduo::ThreadLocalSingleton<Test>::instance().setName("main one");
	muduo::Thread t1(boost::bind(threadFunc, "thread1"));
	muduo::Thread t2(boost::bind(threadFunc, "thread2"));
	t1.start();
	t2.start();
	t1.join();
	printf("tid=%d, %p name=%s\n",
			muduo::CurrentThread::tid(),
			&muduo::ThreadLocalSingleton<Test>::instance(),
			muduo::ThreadLocalSingleton<Test>::instance().name().c_str());
	t2.join();

	pthread_exit(0);
}
#endif

#if 0
#include <muduo/base/Singleton.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Thread.h>

#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test: boost::noncopyable {
public:
	Test() {
		printf("tid=%d, constructing %p\n de: %lu\n", muduo::CurrentThread::tid(), this,sizeof(a));
	}

	~Test() {
		printf("tid=%d, destructing %p %s\n", muduo::CurrentThread::tid(), this,
				name_.c_str());
	}
//
	const muduo::string& name() const {
		return name_;
	}
	void setName(const muduo::string& n) {
		name_ = n;
	}

	int no_destroy() {
		printf("\n");
		return 1;
	}
//	void no_destroy11() {
//			printf("\n");
//		}
private:
	typeof(&Test::no_destroy) a;
	muduo::string name_;
//	int no_destroy;
};

class TestNoDestroy: boost::noncopyable {
public:
	// Tag member for Singleton<T>
	void no_destroy() {
	}

	TestNoDestroy() {
		printf("tid=%d, constructing TestNoDestroy %p\n",
				muduo::CurrentThread::tid(), this);
	}

	~TestNoDestroy() {
		printf("tid=%d, destructing TestNoDestroy %p\n",
				muduo::CurrentThread::tid(), this);
	}
private:
	muduo::string name_;
};

void threadFunc() {
	printf("tid=%d, %p name=%s\n", muduo::CurrentThread::tid(),
			&muduo::Singleton<Test>::instance(),
			muduo::Singleton<Test>::instance().name().c_str());
	muduo::Singleton<Test>::instance().setName("only one, changed");
}

int main() {
	muduo::Singleton<Test>::instance();
//  muduo::Thread t1(threadFunc);
//  t1.start();
//  t1.join();
//  printf("tid=%d, %p name=%s\n",
//         muduo::CurrentThread::tid(),
//         &muduo::Singleton<Test>::instance(),
//         muduo::Singleton<Test>::instance().name().c_str());
	muduo::Singleton<TestNoDestroy>::instance();
//  muduo::Singleton<TestNoDestroy>::instance().no_destroy();
//  muduo::Singleton<Test>::instance();
//  printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
}

#endif
