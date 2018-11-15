/*
 * LogicHandle.h
 *
 *  Created on: Oct 19, 2018
 *      Author: jhb
 */

#ifndef SRC_LOGICHANDLE_H_
#define SRC_LOGICHANDLE_H_

#include <muduo/net/Buffer.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Condition.h>
#include <muduo/net/TcpConnection.h>
//#include <boost/noncopyable.hpp>
#include <map>

#include "SetNetwork.h"
#include "NodeInfo.h"
#include "dp_m2s_prot.h"
//#include "NodeInfo.h"

class LogicHandle {
public:
	LogicHandle();
	~LogicHandle();

	void getInfo(const muduo::net::TcpConnectionPtr connPtr, std::string data);
	//in out node
	void setInfo(const muduo::net::TcpConnectionPtr connPtr, std::string data);

	void createWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

	void moveWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void closeWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void openAudio(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void closeAudio(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void setAudioInfo(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

private:

	bool sendAckToCodec(const void *data, DP_S32 dataLen, DP_U8 needReply);

	DP_S32 getNewCodecTaskID(DP_U32 thirdTaskID, TaskObjectType_E type,
			DP_U32 &reply);

	void closeAAudio(DP_U8 AOChnID, DP_U8 voChnID,
			DP_M2S_CMD_SETINFO_S &setInfo,
			NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo);

	template<typename S, typename T>
	void sendCMD(const muduo::net::TcpConnectionPtr connPtr, const S *data,
			T &reply);

	template<typename T>
	struct compareDevID: public std::binary_function<T, DP_U32, bool> {
		bool operator()(const T &vInfo, const DP_U32 &devID) const {
			if ((vInfo.AvBindAttr.enBindType == DP_M2S_AVBIND_AI2AENC
					|| vInfo.AvBindAttr.enBindType
							== DP_M2S_AVBIND_AI2AENC_VI2VENC)
					&& vInfo.AvBindAttr.stAudio.stIn.u32DevId == devID)
				return true;
			else
				return false;
		}
	};

	muduo::MutexLock _mutex;
	muduo::Condition _cond;

	///change if terminal changed
	SetNetwork _netInfo;
private:
	/* remainnnnnnnnnnnnnnnn*/
	struct findThirdIDByCodecID: public std::binary_function<
			std::map<DP_U32, DP_U32>::value_type, DP_U32, bool> {
		bool operator()(const std::map<DP_U32, DP_U32>::value_type &pair,
				const DP_U32 &codecID) const {
			if (pair.second == codecID)
				return true;
			else
				return false;
		}
	};
	struct findAVDecInfoByCodecID: public std::binary_function<
			DP_M2S_AVDEC_GET_INFO_S, DP_U32, bool> {
		bool operator()(const DP_M2S_AVDEC_GET_INFO_S &avDec,
				const DP_U32 &codecID) const {
			if (avDec.TskId == codecID)
				return true;
			else
				return false;
		}
	};
	/* remainnnnnnnnnnnnnnnn*/
	template<typename S>
	struct findEchoRtspURL: public std::binary_function<S, DP_U32, bool> {
		bool operator()(const S & lhs, const DP_U32 devID) const {
			if (lhs.AvBindAttr.stVideo.stOut.u32DevId == devID)
				return true;
			else
				return false;
		}
	};

	typedef enum _AOChnID_E {
		_pureAudioChn = 0x00, _videoAudioChn
	} AOChnID_E;

};
#include "LogicHandle.hpp"
#endif /* SRC_LOGICHANDLE_H_ */
