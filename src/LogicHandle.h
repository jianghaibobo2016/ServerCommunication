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
	void videoOutput(const muduo::net::TcpConnectionPtr connPtr,
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

//	template<typename S>
//	DP_BOOL getInfoFromCodec(DP_M2S_INFO_TYPE_E infoType,
//			boost::shared_ptr<DP_S8> recvBuff, DP_U32 countOfSt);
//	typedef boost::function0<void()> getInfoPtrCallBack;
//	template<typename PTR, typename SALL,typename SSIN>
//	void replyGottenInfoToThird(getInfoPtrCallBack cb,eDeviceType devType,eRemoteCommand cmd);
//	void GetInputNodeInfoCB(const muduo::net::TcpConnectionPtr& conn,
//			muduo::net::Buffer* buf, muduo::Timestamp receiveTime);
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

};
#include "LogicHandle.hpp"
#endif /* SRC_LOGICHANDLE_H_ */
