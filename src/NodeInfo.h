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

	typedef boost::shared_ptr<DP_M2S_CMD_SYS_INIT_S> InitPtr;
	typedef boost::shared_ptr<DP_M2S_CMD_DEINIT_S> DeinitPtr;

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

	// window present priority (sort)
	typedef std::vector<DP_U32> VctrWindowPriority;
	typedef boost::shared_ptr<VctrWindowPriority> VctrWindowPriorityPtr;

	//AO Dev id -- codec Task id
	typedef std::map<DP_U8, DP_U32> MapAODevIDCodecID;
	typedef boost::shared_ptr<MapAODevIDCodecID> MapAODevIDCodecIDPtr;

	typedef std::vector<DP_S32> VecCodecTaskID;

	typedef std::vector<DP_M2S_VO_DEV_E> VecVODEV;
	typedef std::vector<DP_M2S_AO_DEV_E> VecAODEV;

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
		return _allCodecTaskIDCount;
	}
	inline const VctrWindowPriorityPtr getVctrWindowPriority() {
		return _vWindowPriority;
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
//	InitPtr _sInit;
//	DeinitPtr _sDeinit;
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
public:
	//new method
	DP_S32 getNewCodecTaskID(DP_U32 thirdId, TaskObjectType_E taskType);
	DP_S32 findNewID(DP_U32 thirdId, VctrOutCodecTaskID TaskID);
	DP_S32 getUsedCodecTaskID(DP_U32 thirdId);
	void removeCodecTaskID(DP_U32 thirdId);
	void updateThirdTaskIDCodecTaskid(DP_U32 thirdId, DP_S32 codecID);

	static int recvCB(void* pData, int len);
private:
	void initLocalInfo();

	template<typename T, typename S>
	DP_BOOL getAVInfoFromCodec(VecCodecTaskID codecID, DP_M2S_CMD_ID_E cmd);

	template<typename T, typename S>
	DP_BOOL setAVInfoToCodec(boost::shared_ptr<T> vAVEnc, DP_M2S_CMD_ID_E cmd);

	template<typename T, typename ST, typename V, typename ACK>
	DP_BOOL getAOVOInfoFromCodec(T AOVOInfo, DP_M2S_CMD_ID_E cmd, V aovoDev);

public:
	MapServerTaskIDPtr _mAudioTaskID, _mVideoTaskID, _mAuViTaskID;
	DP_U32 setServerTaskID(DP_U32 taskID);
	void removeServerTaskID(DP_U32 taskID, TaskObjectType_E taskType);

private:
	VctrWindowPriorityPtr _vWindowPriority;

	static SetNetwork _netInfo;

	//use in output node
	void setInputTaskIDInMap(VctrAVENCGetInfoPtr avEncInfo);
	void setOutputTaskIDInMap(VctrAVDECGetInfoPtr avEncInfo);

	DP_BOOL initCodec();
	DP_BOOL deinitCodec();

	DP_BOOL initOutAVEnc();
	DP_BOOL initOutAVDec();

private:
	DP_U32 setID(MapServerTaskIDPtr mTaskID, DP_U32 taskID, DP_U32 min,
			DP_U32 max, TaskObjectType_E taskType);
	void rmID(DP_U32 taskID, MapServerTaskIDPtr mTaskID,
			TaskObjectType_E taskType);

	void initAVDec(DP_M2S_AVDEC_INFO_S *avdec, DP_S32 taskID, DP_U32 chnID);

	DP_S32 cmd_set_aenc_default(DP_VOID *pPtr, DP_S32 s32TskId, DP_U32 u32Width,
			DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn);
	DP_S32 cmd_set_venc_default(DP_VOID *pPtr, DP_S32 s32TskId, DP_U32 u32Width,
			DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 VencChn);
	DP_S32 cmd_set_avenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
			DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate,
			DP_S32 AencChn, DP_S32 VencChn);
//	DP_S32 cmd_set_avenc_default_512(DP_VOID*pPtr);
//	DP_S32 cmd_set_avenc_default_513(DP_VOID*pPtr);
//	DP_S32 print_avenc_get_attr(DP_M2S_AVENC_INFO_S info);

	//sync
	void syncToJson();
	void syncFromJson();

public:
	static void printAVDEC(DP_M2S_AVDEC_INFO_S *avdec);
	static void printAVENC(DP_M2S_AVENC_INFO_S *avenc);

//	inline void print_DP_M2S_VO_GET_INFO_S_(DP_M2S_VO_GET_INFO_S *voInfo) {
//		LOG_INFO << "print_DP_M2S_VO_GET_INFO_S_　devid: " << voInfo->s32DevId
//				<< " enable: " << voInfo->bEnable << " sync: " << voInfo->enSync;
////		LOG_INFO << "print_DP_M2S_VO_GET_INFO_S_　 stsw[0]::" << " stw.void: "
////				<< voInfo.stSwms[0].s32VoDevId << " chn: "
////				<< voInfo.stSwms[0].s32SwmsChn << " priority: "
////				<< voInfo.stSwms[0].u32Priority << " x "
////				<< voInfo.stSwms[0].stRect.s32X << " y: "
////				<< voInfo.stSwms[0].stRect.s32Y << " width: "
////				<< voInfo.stSwms[0].stRect.u32Width << " height: "
////				<< voInfo.stSwms[0].stRect.u32Height;
////		LOG_INFO << "print_DP_M2S_VO_GET_INFO_S_　 stsw[1]::" << " stw.void: "
////				<< voInfo.stSwms[1].s32VoDevId << " chn: "
////				<< voInfo.stSwms[1].s32SwmsChn << " priority: "
////				<< voInfo.stSwms[1].u32Priority << " x "
////				<< voInfo.stSwms[1].stRect.s32X << " y: "
////				<< voInfo.stSwms[1].stRect.s32Y << " width: "
////				<< voInfo.stSwms[1].stRect.u32Width << " height: "
////				<< voInfo.stSwms[1].stRect.u32Height;
//
//	}
};

#include "NodeInfo.hpp"
#endif /* SRC_NODEINFO_H_ */
