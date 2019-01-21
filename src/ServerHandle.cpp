/*
 * ServerHandle.cpp
 *
 *  Created on: Oct 16, 2018
 *      Author: jhb
 */

#include <boost/bind.hpp>
#include <muduo/base/Thread.h>
#include <muduo/base/Singleton.h>
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

ServerHandle::ServerHandle(EventLoop *loop, const InetAddress &listenAddr,
		int idleSeconds) :
		BaseServerHandle(loop, listenAddr, idleSeconds), isUsingThreadPool(
				false), _threadPool("ServerHanPoll") {
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

	if (connPtr->connected()) {
		Node node;
		node.lastReceiveTime = Timestamp::now();
		connectionList_.push_back(connPtr);
		node.position = --connectionList_.end();
		connPtr->setContext(node);
	} else {
		assert(!connPtr->getContext().empty());
		const Node& node = boost::any_cast<const Node&>(connPtr->getContext());
		connectionList_.erase(node.position);
	}
//	dumpConnectionList();
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
	std::string recvData(buff->retrieveAllAsString());
	LOG_INFO << connPtr->name() << " gets " << recvData.size()
			<< " bytes, data received at " << time.toString();
	DevSearch devSearch;
//	NodeInfo nodeInfo;

	LogicHandle &logicHandle = muduo::Singleton<LogicHandle>::instance();
//	if (strcmp(data.c_str(), "厉害厉害")==0) {
//		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << std::endl;
//	}else
//		std::cout << "!!!!!!!!~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << std::endl;
//	DP_U32 count = recvData.size();
	DP_U32 count = 0;
	DP_U32 packLen = 0;
	std::string data;
//	LOG_ERROR << "1111";
	while (count < recvData.size()) {
//		LOG_ERROR << "222222";
		_E_RemoteCMD cmd = DataPackageParser::parserDataPackage(packLen,
				recvData);
		if (packLen == 0) {
			LOG_DEBUG << "pack len == 0";
			break;
		} else {
			LOG_DEBUG << "pack len == " << packLen << " cmd: " << "cmd";
		}
//		LOG_ERROR << "1111cmd: " << cmd << " packLen: " << packLen << " count "
//				<< count;
		data.clear();
		data = recvData.substr(count, packLen);
		count += packLen;
		if (data.size() < packLen) {
			LOG_DEBUG << "data.size() < packLen : " << data.size() << " : "
					<< packLen;
			continue;
		} else {
			LOG_DEBUG << "data.size() !< packLen : " << data.size() << " : "
					<< packLen;
		}
//		LOG_ERROR << "1222221cmd: " << cmd << " packLen: " << packLen << " count "
//				<< count;
		switch (cmd) {
		case _eSearch:
//		_threadPool.run(boost::bind(&DevSearch::devSearch, devSearch, connPtr));
			break;
		case _eCreateWindow:
			_threadPool.run(
					boost::bind(&LogicHandle::createWindow, &logicHandle,
							connPtr, data));
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
					boost::bind(&LogicHandle::closeWindow, &logicHandle,
							connPtr, data));
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
					boost::bind(&LogicHandle::setAudioInfo, &logicHandle,
							connPtr, data));
			break;
		case _eClearTask:
			_threadPool.run(
					boost::bind(&LogicHandle::clearAllTask, &logicHandle,
							connPtr, data));
			break;
		case _eGetJsonInfo:
			_threadPool.run(
					boost::bind(&LogicHandle::getJsonInfo, &logicHandle,
							connPtr, data));
			break;
		case _eSetJsonInfo:
			_threadPool.run(
					boost::bind(&LogicHandle::setJsonInfo, &logicHandle,
							connPtr, data));
			break;
		case _eOpenAndMoveWindow:
			_threadPool.run(
					boost::bind(&LogicHandle::openAndMoveWindow, &logicHandle,
							connPtr, data));
			break;
		case _eWindowBatchControl:
			_threadPool.run(
					boost::bind(&LogicHandle::windowBatchControl, &logicHandle,
							connPtr, data));
			break;
		case _eUpdateBatch:
			_threadPool.run(
					boost::bind(&LogicHandle::updateBatch, &logicHandle,
							connPtr, data));
			break;
		case _eButt:
			break;
		default:
			break;
		}
	}

	assert(!connPtr->getContext().empty());
	Node* node = boost::any_cast<Node>(connPtr->getMutableContext());
	node->lastReceiveTime = time;
	connectionList_.splice(connectionList_.end(), connectionList_,
			node->position);
	assert(node->position == --connectionList_.end());

//	dumpConnectionList();
}

