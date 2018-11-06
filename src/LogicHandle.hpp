/*
 * LogicHandle.hpp
 *
 *  Created on: Oct 27, 2018
 *      Author: jhb
 */
#ifndef SRC_LOGICHANDLE_HPP_
#define SRC_LOGICHANDLE_HPP_

#include "LogicHandle.h"

#if 0
template<typename S>
DP_BOOL getInfoFromCodec(DP_M2S_INFO_TYPE_E infoType,
		boost::shared_ptr<DP_S8> recvBuff, DP_U32 countOfSt) {
	DP_M2S_CMD_GETINFO_S getInfoCodec(sizeof(DP_M2S_CMD_GETINFO_S),
			DP_M2S_CMD_GETINFO, 0x01, DP_M2S_INFO_TYPE_GET_AVENC);

	UnixSocketClient client;
	client.clientConnect();
	client.clientSend(&getInfoCodec, sizeof(DP_M2S_CMD_GETINFO_S));
	DP_U32 offset = sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
	+ sizeof(DP_U32) + sizeof(DP_U32);
	boost::shared_ptr<DP_S8> recvFromCodec(
			new DP_S8[offset + DP_VO_DEV_MAX * sizeof(S)]);
	client.clientRecv(recvFromCodec.get(), sizeof(DP_M2S_CMD_ACK_S), 1);

	DP_M2S_CMD_GETINFO_RESPOND_S *getCodecRespond =
	(DP_M2S_CMD_GETINFO_RESPOND_S*) recvFromCodec.get();
	if (DP_M2S_INFO_TYPE_GET_AVENC != getCodecRespond->enInfoTYpe
			|| getCodecRespond->u32Success != 0) {
		return DP_FALSE;
	}
	return DP_TRUE;
}
#endif

//template<typename T>
//bool LogicHandle::compareDevID(const T *vInfo, DP_U32 devID) {
//	if ((vInfo->AvBindAttr.enBindType == DP_M2S_AVBIND_AI2AENC
//			|| vInfo->AvBindAttr.enBindType == DP_M2S_AVBIND_AI2AENC_VI2VENC)
//			&& vInfo->AvBindAttr.stAudio.stIn.u32DevId == devID)
//		return true;
//	else
//		return false;
//}


#endif /* SRC_LOGICHANDLE_HPP_ */

