/*
 * UDPServerHandle.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: jhb
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/bind.hpp>
#include <muduo/base/Singleton.h>
#include <muduo/base/Logging.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include "UDPServerHandle.h"
#include "LocalInfo.h"

UDPServerHandle::UDPServerHandle(EventLoop *loop) :
		NetTrans(UNIXAF, DATAGRAM, 0), _loop(loop) {
}

UDPServerHandle::~UDPServerHandle() {
}

void UDPServerHandle::startListen() {
	_loop->runInLoop(boost::bind(&UDPServerHandle::runUdp, this));
}

void UDPServerHandle::runUdp() {
	socketBind(MultiCastRecvPort);
	fd_set readfd; //读文件描述符集合
	DP_U32 ret_select = 0;
	DP_U32 ret_recv = 0;
	DP_U8 buffer[UDPBufferSizeMax] = { 0 };
	struct sockaddr_in recvSendAddr;
	DP_S32 tmp_server_addr_len = sizeof(struct sockaddr_in);
	while (1) {
		FD_ZERO(&readfd);
		FD_SET(m_socket, &readfd);
		ret_select = select(m_socket + 1, &readfd, NULL, NULL, NULL);
		if (ret_select < 0) {
			break;
		} else if (ret_select == 0) {
			printf("timeout\n");
			break;
		}
		memset(buffer, 0, UDPBufferSizeMax);
		ret_recv = recvfrom(m_socket, buffer, UDPBufferSizeMax, 0,
				(struct sockaddr *) &recvSendAddr,
				(socklen_t *) &tmp_server_addr_len); //recv_addr为服务器端地址
		if (ret_recv <= 0) {
			break;
		}
		if (FD_ISSET(m_socket, &readfd)) {
			_loop->runInLoop(
					boost::bind(&UDPServerHandle::parserDataRecv, this, buffer,
							ret_recv, recvSendAddr));
		}
	}
}
void UDPServerHandle::parserDataRecv(DP_U8 *buff, DP_U32 len,
		struct sockaddr_in recvAddr) {
	_sRemote_Search * search = (_sRemote_Search*) buff;
	//jhbnote
	printBufferByHex("UDP recv :", buff, len);
	if (search->header.stFunctionMsg.u8CommandID == Command_MulticastSearch) {
		devSearchHandle((DP_BOOL) DP_TRUE, recvAddr);
	} else if (search->header.stFunctionMsg.u8CommandID
			== Command_UnicastSearch) {
		devSearchHandle((DP_BOOL) DP_FALSE, recvAddr);
	} else {
		LOG_ERROR << "CMD id error: "
				<< search->header.stFunctionMsg.u8CommandID;
//		printBufferByHex("UDP recv 2 ", buff, len);
	}
}

void UDPServerHandle::devSearchHandle(DP_BOOL isMulticast,
		struct sockaddr_in recvAddr) {
	LocalInfo::RRSPtr localInfo =
			muduo::Singleton<LocalInfo>::instance().getLocalInfo();

	DP_S32 tmp_server_addr_len = sizeof(struct sockaddr_in);
	struct sockaddr_in sendAddr;
	if (isMulticast) {
		memset(&sendAddr, 0, sizeof(struct sockaddr_in));
		sendAddr.sin_family = AF_INET;
		sendAddr.sin_addr.s_addr = inet_addr(MultiCastAddrSEND); //任何主机地址
		sendAddr.sin_port = htons(MultiCastSendPort);
		sendAddr.sin_addr.s_addr = inet_addr(MultiCastAddrSEND); //任何主机地址
	} else {
		localInfo->header.stFunctionMsg.u8CommandID = Command_UnicastSearch;
		if (&recvAddr) {
			memcpy(&sendAddr, &recvAddr, sizeof(struct sockaddr_in));
		} else {
			LOG_ERROR << "recvAddr is NULL!";
			return;
		}
	}

	DP_U8 sendBuf[UDPBufferSizeMax] = { 0 };
	memcpy(sendBuf, localInfo.get(), sizeof(_sRemote_Reply_Search));

	if (g_DevType == Type_DeviceOutput) {
		_sRemote_Reply_Search_ExtendDeviceOutput info(1, 1, 1);
		memcpy(sendBuf + sizeof(_sRemote_Reply_Search), &info,
				sizeof(_sRemote_Reply_Search_ExtendDeviceOutput));
	} else if (g_DevType == Type_DeviceInput) {
		_sRemote_Reply_Search_ExtendDeviceInput info(1, 1, 1);
		memcpy(sendBuf + sizeof(_sRemote_Reply_Search), &info,
				sizeof(_sRemote_Reply_Search_ExtendDeviceInput));
	}
	int ret = 0;
	ret = sendto(m_socket, sendBuf, localInfo->header.u16PackageLen, 0,
			(struct sockaddr *) &sendAddr, tmp_server_addr_len);
	if (ret > 0)
		LOG_DEBUG << "Dev search ok, return " << ret << " bytes.";

}
