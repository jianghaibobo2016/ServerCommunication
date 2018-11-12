/*
 * NodeInfo.hpp
 *
 *  Created on: Oct 25, 2018
 *      Author: jhb
 */

#ifndef SRC_NODEINFO_HPP_
#define SRC_NODEINFO_HPP_

#include "NodeInfo.h"
#include "UnixSockClientData.h"
#include "PrintBuff.h"

template<typename T, typename S>
DP_BOOL NodeInfo::getAVInfoFromCodecInfo(T vctrGetInfo, DP_M2S_INFO_TYPE_E type,
		DP_U32 devMax) {
	DP_M2S_CMD_GETINFO_S getInfo(sizeof(DP_M2S_CMD_GETINFO_S),
			DP_M2S_CMD_GETINFO, 0x01, type);
	try {
		UnixSockClientDataPtr client(new UnixSockClientData());
		client->doSendCommand(&getInfo, sizeof(DP_M2S_CMD_GETINFO_S));
		DP_U32 offset = sizeof(DP_M2S_INF_PROT_HEAD_S)
				+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + sizeof(DP_U32);
		DP_U8 *recvBuff = client->getRecvBuff();
		DP_M2S_CMD_GETINFO_RESPOND_S *getCodecRespond =
				(DP_M2S_CMD_GETINFO_RESPOND_S*) recvBuff;
		if (type != getCodecRespond->enInfoTYpe
				|| getCodecRespond->u32Success != 0) {
			LOG_WARN << "Return error in getAVInfoFromCodecInfo : type : "
					<< getCodecRespond->enInfoTYpe << " sucess: "
					<< getCodecRespond->u32Success;
			return DP_FALSE;
		}
//		muduo::PrintBuff::printBufferByHex(" recv buff getAVInfoFromCodecInfo ",
//				recvBuff, getCodecRespond->u32InfoLen);
		DP_U32 numOfVencCh = 0;
		numOfVencCh = getCodecRespond->u32InfoLen / sizeof(S);
		LOG_INFO << "u32InfoLen : sizeof (S) : " << getCodecRespond->u32InfoLen
				<< " // " << sizeof(S) << " numOfVencCh:  " << numOfVencCh;

		vctrGetInfo->clear();
		for (DP_U32 i = 0; i < numOfVencCh; i++) {
			S tmp = *(S *) (recvBuff + offset + i * sizeof(S));
			vctrGetInfo->push_back(tmp);
		}
	} catch (const std::string& selfreason) {
		LOG_FATAL << selfreason;
		return DP_FALSE;
	}
	return DP_TRUE;
}

//send all
//template<typename T, typename S>
//DP_BOOL NodeInfo::setAVInfoToCodec(T vAVEnc, DP_M2S_INFO_TYPE_E type) {
//	try {
//		DP_U32 infoLen = vAVEnc.size() * sizeof(S);
//		DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
//				+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + infoLen;
//		DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01, type,
//				infoLen);
//		muduo::net::Buffer buffSend;
//		buffSend.append(&setInfo, packageLen - infoLen);
//		for (typename T::iterator it = vAVEnc.begin(); it != vAVEnc.end();
//				it++) {
//			buffSend.append((DP_U8*) &(*it), sizeof(S));
//		}
//		UnixSockClientDataPtr client(new UnixSockClientData(NodeInfo::recvCB));
//		if (client->doSendCommand(buffSend.toStringPiece().data(), packageLen)
//				== 0)
//			return DP_TRUE;
//		else
//			return DP_FALSE;
//
//	} catch (SystemException& ex) {
//		std::cout << "Error:" << ex.what() << endl;
//	}
//	return DP_TRUE;
//}

template<typename T, typename S>
DP_BOOL NodeInfo::setAVInfoToCodec(T vAVEnc, DP_M2S_INFO_TYPE_E type) {
	try {
		DP_U32 infoLen = sizeof(S);
		DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
				+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + infoLen;
		DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01, type,
				infoLen);
		muduo::net::Buffer buffSend;

		UnixSockClientDataPtr client(new UnixSockClientData(NodeInfo::recvCB));
		for (typename T::iterator it = vAVEnc.begin(); it != vAVEnc.end();
				it++) {
			buffSend.retrieveAll();
			buffSend.append(&setInfo, packageLen - infoLen);
			buffSend.append((DP_U8*) &(*it), sizeof(S));
			LOG_INFO << " it -> task id : " << it->TskId;
			if (client->doSendCommand(buffSend.toStringPiece().data(),
					packageLen) == 0) {
//			return DP_TRUE;

			}
		}

	} catch (const std::string& selfreason) {
		std::cout << "Error:" << selfreason << endl;
		return DP_FALSE;
	}
	return DP_TRUE;
}

#endif /* SRC_NODEINFO_HPP_ */
