/*
 * NodeInfo.hpp
 *
 *  Created on: Oct 25, 2018
 *      Author: jhb
 */

#ifndef SRC_NODEINFO_HPP_
#define SRC_NODEINFO_HPP_
#include <boost/smart_ptr.hpp>
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

//VctrAVDECGetInfo  DP_M2S_CMD_AVENC_SETINFO_S
template<typename T, typename S>
DP_BOOL NodeInfo::getAVInfoFromCodec(VecCodecTaskID codecID,
		DP_M2S_CMD_ID_E cmd) {
	vector<DP_S32>::iterator itID;
	T checkAVInfo;
	DP_M2S_CMD_COMMON_GETINFO_S getAVDec(cmd);
	DP_S32 retSend = 0;
	DP_U8 *recvBuff = new DP_U8[BUFFER_SIZE_PIPESOCKET];
	for (itID = codecID.begin(); itID != codecID.end(); itID++) {
		LOG_INFO << "Ask av info  cmd: " << cmd
				<< " ===================task id : " << (DP_S32) *itID;
		getAVDec.s32TskId = (DP_S32) *itID;
		DP_S32 retResult = 0;
		memset(recvBuff, 0, BUFFER_SIZE_PIPESOCKET);
		NodeInfo::sendToCodecAndRecv(retResult, &getAVDec,
				sizeof(DP_M2S_CMD_COMMON_GETINFO_S), recvBuff);
		if (retResult != 0) {
			LOG_ERROR << "Send avdec failed taskiD in ask : "
					<< getAVDec.s32TskId;
		} else {
			if (recvBuff == NULL) {
				LOG_ERROR << "NULL recv back !";
			} else {
				S *getCodecRespond = (S*) recvBuff;
				checkAVInfo.push_back(getCodecRespond->stInfo);
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
		}
	}
	LOG_INFO << "Size of checkAVDec: " << checkAVInfo.size();
	delete[] recvBuff;
	recvBuff = NULL;
	return DP_TRUE;
}

//send vector
template<typename T, typename S> //VctrAVDECGetInfoPtr DP_M2S_CMD_AVDEC_SETINFO_S
DP_BOOL NodeInfo::setAVInfoToCodec(boost::shared_ptr<T> &vAVInfo,
		DP_M2S_CMD_ID_E cmd) {
	boost::shared_ptr<S> setAVInfo(new S(cmd));
	muduo::net::Buffer buffSend;
	DP_S32 ret = 0;
	typename T::iterator it;
	for (it = vAVInfo->begin(); it != vAVInfo->end(); it++) {
		setAVInfo->stInfo = *it;
		buffSend.retrieveAll();
		buffSend.append(setAVInfo.get(), sizeof(S));
		NodeInfo::sendToCodecAndRecv(ret, buffSend.toStringPiece().data(),
				sizeof(S), NULL);
		if (ret != 0) {
			LOG_ERROR << "Recv from codec : " << ret;
			return DP_FALSE;
		} else {
//			LOG_INFO << "Set to codec task id: " << setAVInfo->stInfo.s32TskId;
		}
	}
	return DP_TRUE;
}

//only one
template<typename T, typename S> // DP_M2S_AVDEC_INFO_S  DP_M2S_CMD_AVDEC_SETINFO_S
DP_S32 NodeInfo::sendCodecAVEncDecInfo(T info, DP_U8 isReply,
		DP_M2S_CMD_ID_E cmd) {
	boost::shared_ptr<S> setAVInfo(new S(cmd, isReply));
	muduo::net::Buffer buffSend;
	setAVInfo->stInfo = info;
	buffSend.append(setAVInfo.get(), sizeof(S));
	DP_S32 ret = 0;
	LOG_INFO << "Tid : " << muduo::CurrentThread::tid();
	if (isReply == g_NoNeedReply) {
		sendToCodecOnly(buffSend.toStringPiece().data(), sizeof(S));
	} else if (isReply == g_NeedReply) {
		NodeInfo::sendToCodecAndRecv(ret, buffSend.toStringPiece().data(),
				sizeof(S), NULL);
		if (ret != 0) {
			LOG_ERROR << "Recv from codec : " << ret;
		}
	}
	return ret;
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
//		try {
		LOG_DEBUG << "Send dev id : " << getCmd.enDevId;

		retSend = client.doSendCommand(&getCmd, sizeof(ST));
		if (retSend != 0) {
			LOG_ERROR << "Send avdec failed dev in ask : " << *it;
		} else {
			DP_U8 *recvBuff = client.getRecvBuff();
			ACK* getInfoAck = (ACK*) recvBuff;
			AOVOInfo->push_back(getInfoAck->stInfo);
			LOG_INFO << "Get aovo info dev: " << getInfoAck->stInfo.enDevId;
		}
//		} catch (SystemException &ex) {
//			LOG_ERROR << ex.what() << " dev id :" << *it;
//		}
	}
	LOG_INFO << "Get aovo size: " << AOVOInfo->size();
	return DP_TRUE;
}

template<typename T>   //DP_M2S_AVENC_INFO_S
DP_S32 NodeInfo::batchGetAVInfoFromCodec(VecCodecTaskID taskID,
		DP_M2S_CMD_ID_E cmd, boost::shared_ptr<std::vector<T>> &avInfo) {
	avInfo->clear();
	//batch get
	DP_U32 count = taskID.size();
	if (count == 0) {
		LOG_DEBUG << "Task id vector is empty.";
		return DP_ERR_TASK_PARAM_ILLEGAL;
	}
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_U32) * (count + 1);

	boost::shared_ptr<DP_M2S_CMD_BATCH_COMMON_S> setAVInfo(
			new DP_M2S_CMD_BATCH_COMMON_S(packageLen, cmd, count));
	muduo::net::Buffer buffSend;
	DP_S32 aTaskID;
	buffSend.append(setAVInfo.get(), sizeof(DP_M2S_CMD_BATCH_COMMON_S));
	for (VecCodecTaskID::iterator it = taskID.begin(); it != taskID.end();
			it++) {
		aTaskID = *it;
		LOG_INFO << "Get task id : " << aTaskID;
		buffSend.append(&aTaskID, sizeof(aTaskID));
	}
	DP_S32 retResult = 0;
	boost::shared_array<DP_U8> recvBuff(new DP_U8[BUFFER_SIZE_PIPESOCKET]);

	/*	DP_U8 *recvBuff =*/NodeInfo::sendToCodecAndRecv(retResult,
			buffSend.toStringPiece().data(), packageLen, recvBuff.get());
	if (retResult == 0) {
		LOG_DEBUG << "Batch getting ok, cmd-- " << cmd;
		DP_M2S_CMD_COMMON_GETBATCHINFO_ACK_S* ackBuff =
				(DP_M2S_CMD_COMMON_GETBATCHINFO_ACK_S*) recvBuff.get();
		if (ackBuff->u32Nums == count) {
			T *avEncInfos;
			avEncInfos = (T *) (recvBuff.get()
					+ sizeof(DP_M2S_CMD_COMMON_GETBATCHINFO_ACK_S)
					+ sizeof(DP_U32) * count);
			for (DP_U32 i = 0; i < count; i++) {
				avInfo->push_back(avEncInfos[i]);
			}
		} else {
			LOG_ERROR << "ackBuff->u32Nums != count u32Nums: "
					<< ackBuff->u32Nums << " count: " << count;
			return DP_ERR_PROTOCOL_CONTENT;
		}
	} else {
		LOG_ERROR << "retResult in batch getting : " << retResult;
		//jhbnote todo
		return retResult;
	}
	return 0;
}

//template<typename T>
//void NodeInfo::test(T tmp) {
//	return;
//}

#endif /* SRC_NODEINFO_HPP_ */
