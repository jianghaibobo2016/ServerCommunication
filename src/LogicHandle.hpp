/*
 * LogicHandle.hpp
 *
 *  Created on: Oct 27, 2018
 *      Author: jhb
 */
#ifndef SRC_LOGICHANDLE_HPP_
#define SRC_LOGICHANDLE_HPP_

#include "LogicHandle.h"

template<typename S, typename T>
void LogicHandle::sendCMD(const muduo::net::TcpConnectionPtr connPtr,
		const S *data, T &reply) {
	DP_U32 stSize = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + stSize;

	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, stSize);
	+sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + stSize;
	muduo::net::Buffer sendBuff;
	sendBuff.append(&setInfo, packageLen - stSize);
	sendBuff.append(data, stSize);

	if (sendAckToCodec(sendBuff.toStringPiece().data(),
			setInfo.stHeader.u16PacketLen, 0x01)) {
	} else {
		reply.u32Success = 1;
	}
	reply.u32Success = 0;
	sendBuff.retrieveAll();
	sendBuff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&sendBuff);
}

#endif /* SRC_LOGICHANDLE_HPP_ */

