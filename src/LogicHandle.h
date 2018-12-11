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

#include "NodeInfo.h"
#include "dp_m2s_prot.h"
//#include "NodeInfo.h"

class LogicHandle {
public:
	LogicHandle();
	~LogicHandle();

	//jhbnote third communication after terminal initialization.

	void getInfo(const muduo::net::TcpConnectionPtr connPtr, std::string data);
	//in out node
	void setInfo(const muduo::net::TcpConnectionPtr connPtr, std::string data);

	void createWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

	void moveWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void closeWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void clearAllTask(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void openAudio(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void closeAudio(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);
	void setAudioInfo(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

	void OpenWinsBatch(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

private:

	void Get_InputVideoChnInfo(const muduo::net::TcpConnectionPtr connPtr);
	void Get_VideoChnVencInfo(const muduo::net::TcpConnectionPtr connPtr);
	void Get_InputAudioChnInfo(const muduo::net::TcpConnectionPtr connPtr);
	void Get_OutputAudioChnInfo(const muduo::net::TcpConnectionPtr connPtr);
	void Get_OutputVideoChnInfo(const muduo::net::TcpConnectionPtr connPtr);

	DP_S32 getNewCodecTaskID(DP_U32 thirdTaskID, TaskObjectType_E type);

	void commonReplyToThird(eRemoteCommand cmd, DP_U32 taskID, DP_U32 success,
			const muduo::net::TcpConnectionPtr connPtr);
	void replyClearToThird(DP_U32 success, NodeInfo::VecErrInfo errTaskID,
			NodeInfo::VecErrInfo errCode,
			const muduo::net::TcpConnectionPtr connPtr);

	void replyGetInfoToThird(eRemotePropertyName proterty, DP_U32 success,
			muduo::net::Buffer buffSend,
			const muduo::net::TcpConnectionPtr connPtr);

//	template<typename S, typename T>
//	void sendCMD(const muduo::net::TcpConnectionPtr connPtr, const S *data,
//			T &reply);

	template<typename T>
	struct compareDevID: public std::binary_function<T, DP_U32, bool> {
		bool operator()(const T &vInfo, const DP_U32 &devID) const {
			if ((vInfo.stAvBind.enBindType == DP_M2S_AVBIND_AI2AENC)
					&& vInfo.stAvBind.stAudio.stIn.u32DevId == devID)
				return true;
			else
				return false;
		}
	};

	///change if terminal changed
public:
	/* remain */
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
private:
	struct findAVDecInfoByCodecID: public std::binary_function<
			DP_M2S_AVDEC_INFO_S, DP_U32, bool> {
		bool operator()(const DP_M2S_AVDEC_INFO_S &avDec,
				const DP_S32 &codecID) const {
			if (avDec.s32TskId == codecID)
				return true;
			else
				return false;
		}
	};
	/* remain */
	template<typename S>
	struct findEchoRtspURL: public std::binary_function<S, DP_M2S_VO_DEV_E, bool> {
		bool operator()(const S & lhs, const DP_M2S_VO_DEV_E devID) const {
			if (lhs.stAvBind.stVideo.stOut.u32DevId == devID)
				return true;
			else
				return false;
		}
	};

	template<typename S, typename E>
	struct findAVDevID: public std::binary_function<S, E, bool> {
		bool operator()(const S & lhs, const E devID) const {
			if (lhs.enDevId == devID)
				return true;
			else
				return false;
		}
	};

//	typedef enum _AOChnID_E {
//		_pureAudioChn = 0x00, _videoAudioChn
//	} AOChnID_E;

	DP_U8 DP_MediaClient_CheckCropDateIsIegitimate(DP_M2S_CROP_ATTR_S crop,
			DP_M2S_VIDEO_SYNC_E enSync);

};
#include "LogicHandle.hpp"
#endif /* SRC_LOGICHANDLE_H_ */
