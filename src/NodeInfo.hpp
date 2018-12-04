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
//#include <st>

// 使用偏特化判断模板是否为指定类型，这里用于判断模板类型是否为double型
template<typename T>
struct isAVEncOrDec {
	operator bool() {
		return false;
	}
};
template<>
struct isAVEncOrDec<DP_M2S_AVENC_INFO_S> {
	operator bool() {
		return true;
	}
};
template<>
struct isAVEncOrDec<DP_M2S_AVDEC_INFO_S> {
	operator bool() {
		return true;
	}
};

//利用 c++模板 类型 推导思想，实现最简单的 判断两个类型 是否一样的 方法
template<typename T1, typename T2>
struct is__same {
	operator bool() {
		return false;
	}
};

template<typename T1>
struct is__same<T1, T1> {
	operator bool() {
		return true;
	}
};

//VctrAVDECGetInfoPtr  DP_M2S_CMD_AVENC_SETINFO_S
template<typename T, typename S>
DP_BOOL NodeInfo::getAVInfoFromCodec(VecCodecTaskID codecID,
		DP_M2S_CMD_ID_E cmd) {
	vector<DP_S32>::iterator itID;
	typename T checkAVInfo;
	DP_M2S_CMD_COMMON_GETINFO_S getAVDec(cmd);
	DP_S32 retSend = 0;
	UnixSockClientData client(NodeInfo::recvCB);
	for (itID = codecID.begin(); itID != codecID.end(); itID++) {
		LOG_INFO << "Ask av info  cmd: " << cmd
				<< " ======================================task id : "
				<< (DP_S32) *itID;
		getAVDec.s32TskId = (DP_S32) *itID;
		try {
			retSend = client.doSendCommand(&getAVDec,
					sizeof(DP_M2S_CMD_COMMON_GETINFO_S));
			if (retSend != 0)
				LOG_ERROR << "Send avdec failed taskiD in ask : "
						<< getAVDec.s32TskId;
				else {
					DP_U8 *recvBuff = client.getRecvBuff();
					S *getCodecRespond = (S*) recvBuff;
					checkAVInfo->push_back(getCodecRespond->stInfo);
					LOG_INFO << "Ask av task id : "
							<< getCodecRespond->stInfo.s32TskId;
					if (is__same<S, DP_M2S_AVENC_INFO_S>()) {
						LOG_INFO << "S is same to DP_M2S_AVENC_INFO_S";
						NodeInfo::printAVENC(&getCodecRespond->stInfo);
					}
					if (is__same<S, DP_M2S_AVDEC_INFO_S>()) {
						LOG_INFO << "S is same to DP_M2S_AVDEC_INFO_S";
						NodeInfo::printAVDEC(&getCodecRespond->stInfo);
					}
				}
		} catch (SystemException &ex) {
			LOG_ERROR << ex.what() << " task id :" << getAVDec.s32TskId;
		}
	}
	LOG_INFO << "Size of checkAVDec: " << checkAVInfo->size();
}

template<typename T, typename S> //VctrAVDECGetInfoPtr DP_M2S_CMD_AVDEC_SETINFO_S
DP_BOOL NodeInfo::setAVInfoToCodec(boost::shared_ptr<T> vAVInfo,
		DP_M2S_CMD_ID_E cmd) {
	try {
		boost::shared_ptr<S> setAVInfo(new S(cmd));
		muduo::net::Buffer buffSend;
		UnixSockClientDataPtr client(new UnixSockClientData(NodeInfo::recvCB));
		DP_S32 ret = 0;
		for (typename T::iterator it = vAVInfo.begin(); it != vAVInfo.end();
				it++) {
			setAVInfo->stInfo = *it;
			buffSend.retrieveAll();
			buffSend.append(setAVInfo.get(), sizeof(S));
			ret = client->doSendCommand(buffSend.toStringPiece().data(),
					sizeof(S));
			if (ret != 0) {
				LOG_ERROR << "Recv from codec : " << ret;
				return DP_FALSE;
			} else {
				LOG_INFO << "Set to codec task id: " << it->stInfo.s32TskId;
			}
		}
	} catch (const std::string& selfreason) {
		std::cout << "Error: " << selfreason << endl;
		return DP_FALSE;
	}
	return DP_TRUE;
}

//VctrVOGetInfoPtr DP_M2S_CMD_VO_GETINFO_S VecVODEV DP_M2S_CMD_VO_GETINFO_ACK_S
template<typename T, typename ST, typename V, typename ACK>
DP_BOOL NodeInfo::getAOVOInfoFromCodec(T AOVOInfo, DP_M2S_CMD_ID_E cmd,
		V aovoDev) {
	AOVOInfo->clear();
	DP_S32 retSend = 0;
	UnixSockClientData client(NodeInfo::recvCB);
	typename V::iterator it;
	for (it = aovoDev.begin(); it != aovoDev.end(); it++) {
		ST getCmd(cmd, *it);
		try {
			retSend = client.doSendCommand(&getCmd, sizeof(ST));
			if (retSend != 0) {
				LOG_ERROR << "Send avdec failed dev in ask : " << *it;
			} else {
				DP_U8 *recvBuff = client.getRecvBuff();
				ACK* getInfoAck = (ACK*) recvBuff;
				AOVOInfo->push_back(getInfoAck->stInfo);
				LOG_INFO << "Get aovo info dev: " << getInfoAck->stInfo.enDevId;
			}
		} catch (SystemException &ex) {
			LOG_ERROR << ex.what() << " dev id :" << *it;
		}
	}
	LOG_INFO << "Get aovo size: " << AOVOInfo->size();
}

#endif /* SRC_NODEINFO_HPP_ */
