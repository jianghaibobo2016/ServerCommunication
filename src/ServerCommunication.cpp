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
//#include <CurrentThread.h>
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
#include "LightDisplay.h"

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
//	signal(SIGSEGV, signalCB);
//	signal(SIGINT, signalCB);
//	signal(SIGILL, signalCB);
//	signal(SIGKILL, signalCB);
//	signal(SIGABRT, signalCB);
}

int main() {

//	EventLoop planLoop;
//	EventLoopThreadPool pool(&planLoop,"fff");
//	pool.setThreadNum(3);
//	pool.start();
//	pool.getNextLoop();

//	setvbuf(stdout, (char *) NULL, _IOLBF, 0);
	//logging setting
//	g_logFile.reset(new muduo::LogFile(LogFileName, g_LogFileMaxSize));
//	muduo::Logger::setOutput(outputFunc);
//	muduo::Logger::setFlush(flushFunc);
	muduo::Logger::setLogLevel(muduo::Logger::TRACE);
	LOG_TRACE << "Build time : " << BuildTime;
	LOG_TRACE << "Software Version : " << SoftVersion;
	LOG_TRACE
			<< "===========ServerCommunication program starting !==============";
//	muduo::Logger::setLogLevel(muduo::Logger::ERROR);
	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
	setvbuf(stdout, (char *) NULL, _IOLBF, 0);
//	print(&loop);

#if 1
	signalHandle();
	//init
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
	DP_UDRV_LCD_ShowString(0, 1, ip, strlen(ip));// jhbnote add ETH0/1
	DP_UDRV_LCD_ShowString(0, 2, Working, strlen(Working));

//	DP_S32 DP_UDRV_LCD_DeInit();
//	return 0;

#endif

#if 0 // led
	DP_UDRV_LED_Init();
//
	LOG_INFO<<"close all ";
	DP_UDRV_LED_SetValue(DP_UDRV_LED_ALL, 0);
	sleep(2);
	LOG_INFO<<"open all ";
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX0, 1);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 1);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 1);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 1);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX4, 1);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX5, 1);

	sleep(2);
	LOG_INFO<<"close all ";
	DP_UDRV_LED_SetValue(DP_UDRV_LED_ALL, 0);
	sleep(2);
	LOG_INFO<<"open all ";
	DP_UDRV_LED_SetValue(DP_UDRV_LED_ALL, 1);

	sleep(2);
	LOG_INFO<<"close all ";
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX0, 0);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 0);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX4, 0);
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX5, 0);
//
//	return 0;
#endif

	EventLoop loop; // one loop shared by multiple servers
	ServerHandle tcpServer(&loop, InetAddress(5010), 8);
	tcpServer.startServerHandle(2);
	//jhbnote one thread try?
	tcpServer.startHandle(60, 1);

	EventLoopThread UDPServer;
	UDPServerHandle udpServer(UDPServer.startLoop());
	udpServer.startListen();
	EventLoopThread lightThread;
	EventLoop *lightLoop = lightThread.startLoop();
	lightLoop->runInLoop(lightDisplay);

#endif
#if 0
	EventLoopThread ctrlBoardHandle;
	CtrlBoardHandle boardHandle(ctrlBoardHandle.startLoop());
	boardHandle.startRunning();
#endif
	loop.loop();
}

#endif

