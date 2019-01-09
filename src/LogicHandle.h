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
#include <muduo/base/CountDownLatch.h>
#include <map>
#include <pthread.h>
#include "NodeInfo.h"
#include "dp_m2s_prot.h"
#include <json/json.h>

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
	void openAndMoveWindow(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

	//
	void updateBatch(const muduo::net::TcpConnectionPtr connPtr,
			std::string data);

private:
	inline DP_BOOL getIsClearTask() {
//		printf("mu1111: %p,this :%p \n", &Mutex, &*this);
		muduo::MutexLockGuard lock(Mutex);
		return _isClearTasking;
	}

	inline void updateIsClearTask(DP_BOOL newData) {
		LOG_DEBUG << "test ABORT error 1";
		muduo::MutexLockGuard lock(Mutex);
		LOG_DEBUG << "test ABORT error 2";
		_isClearTasking = newData;
		LOG_DEBUG << "test ABORT error 3";
	}

private:
	muduo::MutexLock Mutex;
	muduo::MutexLock _mutex;
	muduo::MutexLock _prioMutex;
//	pthread_mutex_t _mutex;
	DP_BOOL _isClearTasking;

	typedef boost::shared_ptr<muduo::CountDownLatch> CountDownLatchPtr;

	CountDownLatchPtr _latch;

	void Get_InputVideoChnInfo(const muduo::net::TcpConnectionPtr connPtr,
			DP_U32 requestID);
	void Get_VideoChnVencInfo(const muduo::net::TcpConnectionPtr connPtr,
			DP_U32 requestID);
	void Get_InputAudioChnInfo(const muduo::net::TcpConnectionPtr connPtr,
			DP_U32 requestID);
	void Get_OutputAudioChnInfo(const muduo::net::TcpConnectionPtr connPtr,
			DP_U32 requestID);
	void Get_OutputVideoChnInfo(const muduo::net::TcpConnectionPtr connPtr,
			DP_U32 requestID);

	DP_S32 getNewCodecTaskID(DP_U32 thirdTaskID, TaskObjectType_E type);

	void commonReplyToThird(eRemoteCommand cmd, DP_U32 taskID, DP_U32 success,
			const muduo::net::TcpConnectionPtr connPtr);
	void replyClearToThird(DP_U32 success, NodeInfo::VecErrInfo errTaskID,
			NodeInfo::VecErrInfo errCode,
			const muduo::net::TcpConnectionPtr connPtr);

	void replyGetInfoToThird(eRemotePropertyName proterty, DP_U32 success,
			muduo::net::Buffer buffSend,
			const muduo::net::TcpConnectionPtr connPtr);

	void replySetInfoToThird(const muduo::net::TcpConnectionPtr connPtr,
			vector<string> &vKey, vector<DP_U32> &result);

	DP_BOOL parseJsonData(Json::Value &JsonValue, vector<string> &vKey,
			vector<DP_U32> &result);
	DP_U32 setInfo_AVENCToCodec(DP_M2S_AVENC_INFO_S &avEncInfo);

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

	template<typename T>
	struct compareVencChID: public std::binary_function<T, DP_U32, bool> {
		bool operator()(const T &vInfo, const DP_U32 &chID) const {
			if (vInfo.stAvBind.stVideo.stOut.u32ChnId == chID)
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

	template<typename S, typename E>
	struct findAudioVolume: public std::binary_function<S, E, bool> {
		bool operator()(const S & lhs, const E vol) const {
			if (lhs.enDevId == vol)
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

#pragma pack(push)
#pragma pack(1)

typedef struct _SET_AVENC_INFO_S {
	DP_U32 _u32VideoVencCh;
	DP_U32 _u32AILINE0Volume;
	DP_U32 _u32AIHDMI0Volume;
	DP_U32 _u32AOVolume;
	DP_U32 _u32CropX;
	DP_U32 _u32CropY;
	DP_U32 _u32CropWidth;
	DP_U32 _u32CropHeight;
	DP_U32 _u32ZoomWidth;
	DP_U32 _u32ZoomHeight;
	DP_U32 _u32OSDType; //union
	DP_U8 _pu8OSDStr[DP_M2S_OSD_STRING_LEN];
	DP_U32 _u32OSDStrColor;
	DP_U8 _pu8OSDPic[DP_M2S_OSD_PIC_PATH_LEN];
	DP_U32 _u32OSDPresentModel;
	DP_U32 _u32OSDPointX;
	DP_U32 _u32OSDPointY;
	DP_U32 _u32FrameRate;
	DP_U32 _u32BitRate;
	DP_U32 _u32Cast;
} SET_AVENC_INFO_S;

#pragma pack(pop)

static const DP_CHAR *Input_Enc_Info_Str = "输入编码信息";
static const DP_CHAR *Input_Enc_VencCh_Str = "视频编码通道";
static const DP_CHAR *Input_Enc_AILINEIN0Volume_Str = "AI音量LINEIN0";
static const DP_CHAR *Input_Enc_AIHDMI0Volume_Str = " AI音量HDMI0";
static const DP_CHAR *Input_Enc_AOVolume_Str = "音量AO";
static const DP_CHAR *Input_Enc_CropX_Str = "裁剪X";
static const DP_CHAR *Input_Enc_CropY_Str = " 裁剪Y";
static const DP_CHAR *Input_Enc_CropWidth_Str = "裁剪Width";
static const DP_CHAR *Input_Enc_CropHeight_Str = "裁剪Height";
static const DP_CHAR *Input_Enc_ZOOMWidth_Str = "缩放Width";
static const DP_CHAR *Input_Enc_ZOOMHeight_Str = "缩放Height";
static const DP_CHAR *Input_Enc_OSDType_Str = "OSD类型";
static const DP_CHAR *Input_Enc_OSDStr_Str = "OSD字符串";
static const DP_CHAR *Input_Enc_OSDStrColor_Str = "OSD字符串颜色";
static const DP_CHAR *Input_Enc_OSDPic_Str = "OSD图片";
static const DP_CHAR *Input_Enc_OSDPresentModel_Str = "OSD显示模式";
static const DP_CHAR *Input_Enc_OSDPointX_Str = "OSD坐标X";
static const DP_CHAR *Input_Enc_OSDPointY_Str = "OSD坐标Y";
static const DP_CHAR *Input_Enc_FrameRate_Str = "输入帧率";
static const DP_CHAR *Input_Enc_BitRate_Str = "输入码率";
static const DP_CHAR *Input_Enc_Cast_Str = "单组播";

#include "LogicHandle.hpp"
#endif /* SRC_LOGICHANDLE_H_ */
