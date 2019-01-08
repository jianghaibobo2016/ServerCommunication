/*
 * NodeInfo.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_NODEINFO_H_
#define SRC_NODEINFO_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <muduo/base/Mutex.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Logging.h>

#include <map>
#include <vector>
#include "SetNetwork.h"
#include "interactivepro.h"
#include "dp_m2s_prot.h"
#include "ErrorCode.h"
#include "UnixSockClientData.h"

typedef enum _TaskObjectType_E {
	_eAudioTask = 0,
	_eVideoTask,
	_eAudioAndVideoTask,
	_eUnknowTask,
	_eTaskObjectTypeButt
} TaskObjectType_E;

class NodeInfo: boost::noncopyable {
public:
	NodeInfo();
	~NodeInfo();

//获取音频输入设备信息
	typedef std::vector<DP_M2S_AI_GET_INFO_S> VctrAIGetInfo;
	typedef boost::shared_ptr<VctrAIGetInfo> VctrAIGetInfoPtr;

	//获取视频输入设备信息
	typedef std::vector<DP_M2S_VI_GET_INFO_S> VctrVIGetInfo;
	typedef boost::shared_ptr<VctrVIGetInfo> VctrVIGetInfoPtr;

	//获取音视频编码通道信息
	typedef std::vector<DP_M2S_AVENC_INFO_S> VctrAVENCGetInfo;
	typedef boost::shared_ptr<VctrAVENCGetInfo> VctrAVENCGetInfoPtr;

	//取音视频解码通道信息
	typedef std::vector<DP_M2S_AVDEC_INFO_S> VctrAVDECGetInfo;
	typedef boost::shared_ptr<VctrAVDECGetInfo> VctrAVDECGetInfoPtr;

	// 说明： 获取音频输出信息
	typedef std::vector<DP_M2S_AO_INFO_S> VctrAOGetInfo;
	typedef boost::shared_ptr<VctrAOGetInfo> VctrAOGetInfoPtr;

	// 获取视频输出信息
	typedef std::vector<DP_M2S_VO_INFO_S> VctrVOGetInfo;
	typedef boost::shared_ptr<VctrVOGetInfo> VctrVOGetInfoPtr;

	//id for codec & id for thirdparty
	typedef std::map<DP_U32, DP_U32> MapServerTaskID;
	typedef boost::shared_ptr<MapServerTaskID> MapServerTaskIDPtr;

	//out node codec task id vector data structure
	typedef std::vector<DP_U32> VctrOutCodecTaskID;

	//task id & its used
	typedef std::map<VctrOutCodecTaskID, DP_U32> MapOutCodecTaskIDBeUsed;
	typedef boost::shared_ptr<MapOutCodecTaskIDBeUsed> MapOutCodecTaskIDBeUsedPtr; //!
	// third id -->> codecID
	typedef std::map<DP_U32, DP_U32> MapOutThirdCodecTaskID;
	typedef boost::shared_ptr<MapOutThirdCodecTaskID> MapOutThirdCodecTaskIDPtr; //!
	//all used codec ID
	typedef std::vector<DP_U32> VctrAllUsedCodecTaskID;

	// SWMS chn  num -->> avdec
	typedef std::map<DP_S32, DP_M2S_AVDEC_INFO_S> MapOutSWMSChCodecDecInfo;
	typedef boost::shared_ptr<MapOutSWMSChCodecDecInfo> MapOutSWMSChCodecDecInfoPtr;

	// third task id -->  _sSrcVideoInfo
	typedef std::map<DP_U32, _sSrcVideoInfo> MapThirdIDSrcVideoInfo;
	typedef boost::shared_ptr<MapThirdIDSrcVideoInfo> MapThirdIDSrcVideoInfoPtr;

	// window present priority (sorted)
	typedef std::vector<DP_U32> VctrWindowPriority;
	typedef boost::shared_ptr<VctrWindowPriority> VctrWindowPriorityPtr;

	//AO Dev id -- codec Task id (audio)
	typedef std::map<DP_M2S_AO_DEV_E, DP_U32> MapAODevIDCodecID;
	typedef boost::shared_ptr<MapAODevIDCodecID> MapAODevIDCodecIDPtr;

	typedef std::vector<DP_S32> VecCodecTaskID;

	//err code info array
	typedef std::vector<DP_U32> VecErrInfo;

	typedef struct _AOAudioInfo_S {
		DP_U8 u8AoChnMute;	 ///<是否静音 0否 1是
		DP_U8 u8AoChnVolume;   ///<音量 0~100
	} AOAudioInfo_S;

	//AO ------- AO info (audio)
	typedef std::map<DP_M2S_AO_DEV_E, AOAudioInfo_S> MapAODevAudioInfo;
	typedef boost::shared_ptr<MapAODevAudioInfo> MapMapAODevAudioInfoPtr;

	typedef std::vector<DP_M2S_VO_DEV_E> VecVODEV;
	typedef std::vector<DP_M2S_AO_DEV_E> VecAODEV;

	typedef std::vector<DP_M2S_VI_DEV_E> VecVIDEV;
	typedef std::vector<DP_M2S_AI_DEV_E> VecAIDEV;

	//----------------------------get --------------------------//
	inline const VctrAIGetInfoPtr getAIGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vAIGetInfo;
	}
	inline const VctrVIGetInfoPtr getVIGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vVIGetInfo;
	}
	inline const VctrAVENCGetInfoPtr getAVEncGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vAVEncGetInfo;
	}
	inline const VctrAVDECGetInfoPtr getAVDecGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vAVDecGetInfo;
	}
	inline const VctrAOGetInfoPtr getAOGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vAOGetInfo;
	}
	inline const VctrVOGetInfoPtr getVOGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vVOGetInfo;
	}
	inline const MapOutCodecTaskIDBeUsedPtr getOutCodecTaskIDBeUsed() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mOutCodecTaskIDBeUsed;
	}
	inline const MapOutThirdCodecTaskIDPtr getOutThirdCodecTaskID() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mOutThirdCodecTaskID;
	}
	inline const MapOutSWMSChCodecDecInfoPtr getOutSWMSChCodecDecInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mSwmsChCodecDecInfo;
	}
	inline const MapThirdIDSrcVideoInfoPtr getThirdIDSrcVideoInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mThirdIDSrcVideoInfo;
	}
	inline const MapAODevIDCodecIDPtr getAODevIDCodecID() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mAODevIDCodecID;
	}
	inline const MapMapAODevAudioInfoPtr AODevAudioInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mAODevAudioInfo;
	}
	inline const MapServerTaskIDPtr getAudioTaskIDMap() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mAudioTaskID;
	}
	inline const MapServerTaskIDPtr getVideoTaskIDMap() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mVideoTaskID;
	}
	inline const MapServerTaskIDPtr getAuViTaskIDMap() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mAuViTaskID;
	}
	inline const DP_U32 getCodecTaskIDCount() {
		muduo::MutexLockGuard lock(_mutex);
		return _allCodecTaskIDCount;
	}
	inline const VctrWindowPriorityPtr getVctrWindowPriority() {
		muduo::MutexLockGuard lock(_mutex);
		return _vWindowPriority;
	}
	inline const SetNetwork getNetInfo() {
		muduo::MutexLockGuard lock(_mutex);
		return _netInfo;
	}
	inline const DP_BOOL getCodecInited() {
		muduo::MutexLockGuard lock(_mutex);
		return _bCodecInited;
	}
	//----------------------------get --------------------------//

	//----------------------------update --------------------------//
	inline void updateAIGetInfo(VctrAIGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vAIGetInfo.swap(newData);
		}
	}
	inline void updateVIGetInfo(VctrVIGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vVIGetInfo.swap(newData);
		}
	}
	inline void updateAVEncGetInfo(VctrAVENCGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vAVEncGetInfo.swap(newData);
		}
	}
	inline void updateAVDecGetInfo(VctrAVDECGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vAVDecGetInfo.swap(newData);
		}
	}
	inline void updateAOGetInfo(VctrAOGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vAOGetInfo.swap(newData);
		}
	}
	inline void updateVOGetInfo(VctrVOGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vVOGetInfo.swap(newData);
		}
	}
	inline void updateOutCodecTaskIDBeUsed(MapOutCodecTaskIDBeUsedPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_mOutCodecTaskIDBeUsed.swap(newData);
		}
	}
	inline void updateMapOutThirdCodecTaskID(
			MapOutThirdCodecTaskIDPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_mOutThirdCodecTaskID.swap(newData);
		}
	}
	inline void updateMapOutSWMSChCodecDecInfo(
			MapOutSWMSChCodecDecInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_mSwmsChCodecDecInfo.swap(newData);
		}
	}
	inline void updateMapThirdIDSrcVideoInfo(
			MapThirdIDSrcVideoInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_mThirdIDSrcVideoInfo.swap(newData);
		}
	}
	inline void updateAODevIDCodecID(MapAODevIDCodecIDPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_mAODevIDCodecID.swap(newData);
		}
	}
	inline void updateAODevAudioInfo(MapMapAODevAudioInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_mAODevAudioInfo.swap(newData);
		}
	}
	inline void updateVctrWindowPriority(VctrWindowPriorityPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vWindowPriority.swap(newData);
		}
	}
	inline void updateTaskID(MapServerTaskIDPtr newData,
			TaskObjectType_E taskType) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			switch (taskType) {
			case _eAudioTask:
				_mAudioTaskID.swap(newData);
				break;
			case _eVideoTask:
				_mVideoTaskID.swap(newData);
				break;
			case _eAudioAndVideoTask:
				_mAuViTaskID.swap(newData);
				break;
			case _eTaskObjectTypeButt:
				break;
			case _eUnknowTask:
				break;
			}
		}
	}
	//----------------------------update --------------------------//

private:
	mutable muduo::MutexLock _mutex;
	muduo::MutexLock _mutexForUsedID;
	VctrAIGetInfoPtr _vAIGetInfo;
	VctrVIGetInfoPtr _vVIGetInfo;
	VctrAVENCGetInfoPtr _vAVEncGetInfo;
	VctrAVDECGetInfoPtr _vAVDecGetInfo;
	VctrAOGetInfoPtr _vAOGetInfo;
	VctrVOGetInfoPtr _vVOGetInfo;

	VctrOutCodecTaskID _vAudioTaskID, _vVideoTaskID, _vAuViTaskID;
	DP_U32 _allCodecTaskIDCount;
	MapOutCodecTaskIDBeUsedPtr _mOutCodecTaskIDBeUsed;
	MapOutThirdCodecTaskIDPtr _mOutThirdCodecTaskID;
	VctrAllUsedCodecTaskID _vAllUseCodecTaskID; // must use mutex
	MapOutSWMSChCodecDecInfoPtr _mSwmsChCodecDecInfo;
	MapThirdIDSrcVideoInfoPtr _mThirdIDSrcVideoInfo;
	MapAODevIDCodecIDPtr _mAODevIDCodecID;
	MapMapAODevAudioInfoPtr _mAODevAudioInfo;
public:
	//new method
	DP_S32 getNewCodecTaskID(DP_U32 thirdId, TaskObjectType_E taskType);
	DP_S32 findNewID(DP_U32 thirdId, VctrOutCodecTaskID TaskID);
	DP_S32 getUsedCodecTaskID(DP_U32 thirdId);
	void removeCodecTaskID(DP_U32 thirdId);
	void updateThirdTaskIDCodecTaskid(DP_U32 thirdId, DP_S32 codecID);

	static DP_S32 recvCB(void* pData, int len);

	template<typename T, typename S>
	static DP_S32 sendCodecAVEncDecInfo(T info, DP_U8 isReply,
			DP_M2S_CMD_ID_E cmd);

	static DP_U32 batchSetting(DP_M2S_CMD_ID_E cmd, VecCodecTaskID &vTaskID,
			VecErrInfo &errInfo);

	template<typename S>
	static DP_U32 batchSetting(DP_M2S_CMD_ID_E cmd, VecCodecTaskID &vTaskID,
			std::vector<S> &info, VecErrInfo &errInfo);

	template<typename S>
	static DP_U32 batchGetting(DP_M2S_CMD_ID_E cmd, VecCodecTaskID &vTaskID,
			std::vector<S> &info, VecErrInfo &errInfo);

private:

	inline static void sendToCodecAndRecv(DP_S32 &retResult, const void* data,
			DP_S32 len, DP_U8* recvBuff) {
		UnixSockClientData client(NodeInfo::recvCB);
//		try {
		retResult = client.doSendCommand(data, len);
//		} catch (SystemException &ex) {
//			LOG_ERROR << ex.what();
//			retResult = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
//			return NULL;
//		}
		if (retResult != 0) {
//			LOG_ERROR << "Send and recv failed. ";
//			return NULL;
			return;
		} else {
			//jhbnote can not return a deleted buff
//			return client.getRecvBuff();
			if (recvBuff != NULL) {
				//notice buff len >= BUFFER_SIZE_PIPESOCKET
				memcpy(recvBuff, client.getRecvBuff(), BUFFER_SIZE_PIPESOCKET);
				return;
			}
		}
		return;
	}

	inline static DP_S32 sendToCodecOnly(const void* data, DP_S32 len) {
		UnixSockClientData client;
//		try {
		return client.onlySendMsg(data, len);
//		} catch (SystemException &ex) {
//			LOG_ERROR << ex.what();
//			return DP_ERR_COMMUNICATE_ABNORMAL_INNER;
//		}
		return 0;
	}

	void initLocalInfo();

	template<typename T, typename S>
	DP_BOOL getAVInfoFromCodec(VecCodecTaskID codecID, DP_M2S_CMD_ID_E cmd);

	template<typename T, typename S>
	DP_BOOL setAVInfoToCodec(boost::shared_ptr<T> &vAVInfo,
			DP_M2S_CMD_ID_E cmd);

	template<typename T, typename ST, typename V, typename ACK>
	DP_BOOL getAOVOInfoFromCodec(T AOVOInfo, DP_M2S_CMD_ID_E cmd, V aovoDev);

	template<typename T>
	DP_S32 batchGetAVInfoFromCodec(VecCodecTaskID taskID, DP_M2S_CMD_ID_E cmd,
			boost::shared_ptr<std::vector<T>> &avInfo);

//	template<typename T>
//	void test(T tmp);

public:
	MapServerTaskIDPtr _mAudioTaskID, _mVideoTaskID, _mAuViTaskID;
	DP_U32 setServerTaskID(DP_U32 taskID);
	void removeServerTaskID(DP_U32 taskID, TaskObjectType_E taskType);

private:
	VctrWindowPriorityPtr _vWindowPriority;

	SetNetwork _netInfo;
	DP_BOOL _bCodecInited;

	//use in output node
	void setInputTaskIDInMap(VctrAVENCGetInfoPtr avEncInfo);
	void setOutputTaskIDInMap(VctrAVDECGetInfoPtr avEncInfo);

	DP_BOOL initCodec();
	DP_BOOL deinitCodec();

	DP_BOOL initOutAVEnc();
	DP_BOOL openOutAVEnc();
	DP_BOOL initOutAVDec();
	DP_BOOL initOutGetVO();
	DP_BOOL initOutGetAO();

	DP_BOOL initInAVEnc();

	DP_BOOL initInGetVI();
	DP_BOOL initInGetAI();

private:
	DP_U32 setID(MapServerTaskIDPtr mTaskID, DP_U32 taskID, DP_U32 min,
			DP_U32 max, TaskObjectType_E taskType);
	void rmID(DP_U32 taskID, MapServerTaskIDPtr mTaskID,
			TaskObjectType_E taskType);

	void initAVDec(DP_M2S_AVDEC_INFO_S *avdec, DP_S32 taskID, DP_U32 chnID);

	static int cmd_set_aenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
			DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate,
			DP_S32 AencChn);
	static int cmd_set_venc_default(DP_VOID *pPtr, DP_S32 s32TskId,
			DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate,
			DP_S32 VencChn);
	static int cmd_set_avenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
			DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate,
			DP_S32 AencChn, DP_S32 VencChn);
	static DP_S32 cmd_set_adec_default(DP_VOID *pPtr, DP_S32 s32TskId,
			DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate,
			DP_S32 AdecChn);
	//sync
	void syncToJson();
	void syncFromJson();

public:
	static void printAVDEC(void *args);
	static void printAVENC(void *args);

};

#include "NodeInfo.hpp"
#endif /* SRC_NODEINFO_H_ */
