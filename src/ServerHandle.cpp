/*
 * ServerHandle.cpp
 *
 *  Created on: Oct 16, 2018
 *      Author: jhb
 */

#include <boost/bind.hpp>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include "DataPackageParser.h"
//#include <muduo/base/ThreadPool.h>

#include "ServerHandle.h"
#include "interactivepro.h"
#include "LocalInfo.h"
#include "DevSearch.h"
#include "NodeInfo.h"
#include "PrintBuff.h"
#include "LogicHandle.h"
#include "LocalInfo.h"//non use
#include <muduo/base/Singleton.h> //non use

ServerHandle::ServerHandle(EventLoop *loop, const InetAddress &listenAddr) :
		BaseServerHandle(loop, listenAddr), isUsingThreadPool(false), _threadPool(
				"ServerHandlePoll") {

}
ServerHandle::~ServerHandle() {
}

void ServerHandle::startHandle(int maxQueueSize, int numThreads) {
	_threadPool.setMaxQueueSize(maxQueueSize);
	_threadPool.start(numThreads);
	isUsingThreadPool = true;
}

void ServerHandle::onConnection(const TcpConnectionPtr &connPtr) {
	LOG_INFO << "ServerHandle - " << connPtr->peerAddress().toIpPort() << " -> "
			<< connPtr->localAddress().toIpPort() << " is "
			<< (connPtr->connected() ? "UP" : "DOWN");
}

void ServerHandle::onMessage(const TcpConnectionPtr &connPtr, Buffer *buff,
		Timestamp time) {
	std::cout << "ip " << connPtr->peerAddress().toIpPort() << " "
			<< CurrentThread::tid() << std::endl;
//	non block
	//use thread pool
	if (!isUsingThreadPool) {
		LOG_FATAL << "WHY NOT USE THREAD POOL !";
	}

	//run model judge
	//msg head parser
	//msg body parser
	muduo::PrintBuff::printBufferByHex("Recv TCP data: ",
			buff->toStringPiece().data(), buff->toStringPiece().size());
	std::string data(buff->retrieveAllAsString());
	LOG_INFO << connPtr->name() << " gets " << data.size()
			<< " bytes, data received at " << time.toString();
	DevSearch devSearch;
//	NodeInfo nodeInfo;
	LogicHandle logicHandle;
//	if (strcmp(data.c_str(), "厉害厉害")==0) {
//		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << std::endl;
//	}else
//		std::cout << "!!!!!!!!~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << std::endl;
	switch (DataPackageParser::parserDataPackage(data)) {
	case _eSearch:
//		_threadPool.run(boost::bind(&DevSearch::devSearch, devSearch, connPtr));
		break;
	case _eCreateWindow:
		_threadPool.run(
				boost::bind(&LogicHandle::createWindow, &logicHandle, connPtr,
						data));
		break;
	case _eGetInfo:
		_threadPool.run(
				boost::bind(&LogicHandle::getInfo, &logicHandle, connPtr,
						data));
		break;
	case _eSetInfo:
		_threadPool.run(
				boost::bind(&LogicHandle::setInfo, &logicHandle, connPtr,
						data));
		break;
	case _eMoveWindow:
		_threadPool.run(
				boost::bind(&LogicHandle::moveWindow, &logicHandle, connPtr,
						data));
		break;
	case _eCloseWindow:
		_threadPool.run(
				boost::bind(&LogicHandle::closeWindow, &logicHandle, connPtr,
						data));
		break;
	case _eOpenAudio:
		_threadPool.run(
				boost::bind(&LogicHandle::openAudio, &logicHandle, connPtr,
						data));
		break;
	case _eCloseAudio:
		_threadPool.run(
				boost::bind(&LogicHandle::closeAudio, &logicHandle, connPtr,
						data));
		break;
	case _eSetAudio:
		_threadPool.run(
				boost::bind(&LogicHandle::setAudioInfo, &logicHandle, connPtr,
						data));
		break;
	case _eClearTask:
		_threadPool.run(
				boost::bind(&LogicHandle::clearAllTask, &logicHandle, connPtr,
						data));
		break;
	default:
		break;
	}
}

