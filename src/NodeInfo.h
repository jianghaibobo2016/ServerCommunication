/*
 * NodeInfo.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_NODEINFO_H_
#define SRC_NODEINFO_H_

//#include <muduo/net/TcpConnection.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <muduo/base/Mutex.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Logging.h>

#include <map>
#include <vector>

#include "interactivepro.h"
#include "dp_m2s_prot.h"

typedef enum _TaskObjectType_E {
	_eAudioTask = 0, _eVideoTask, _eAudioAndVideoTask, _eTaskObjectTypeButt
} TaskObjectType_E;

class NodeInfo: boost::noncopyable {
public:
	NodeInfo();
	~NodeInfo();

	typedef boost::shared_ptr<DP_M2S_CMD_INIT_S> InitPtr;
	typedef boost::shared_ptr<DP_M2S_CMD_DEINIT_S> DeinitPtr;
	typedef boost::shared_ptr<_sAllAoChnInfo> AllAoChInfoPtr;
	typedef boost::shared_ptr<_sAllVencChnInfo> AllVencChInfoPtr;
	typedef boost::shared_ptr<_sAllVoChnInfo> AllVoChInfoPtr;

	//获取音频输入设备信息
	typedef std::vector<DP_M2S_AI_GET_INFO_S> VctrAIGetInfo;
	typedef boost::shared_ptr<VctrAIGetInfo> VctrAIGetInfoPtr;

	//获取视频输入设备信息
	typedef std::vector<DP_M2S_VI_GET_INFO_S> VctrVIGetInfo;
	typedef boost::shared_ptr<VctrVIGetInfo> VctrVIGetInfoPtr;

	//获取音视频编码通道信息
	typedef std::vector<DP_M2S_AVENC_GET_INFO_S> VctrAVENCGetInfo;
	typedef boost::shared_ptr<VctrAVENCGetInfo> VctrAVENCGetInfoPtr;

	//取音视频解码通道信息
	typedef std::vector<DP_M2S_AVDEC_GET_INFO_S> VctrAVDECGetInfo;
	typedef boost::shared_ptr<VctrAVDECGetInfo> VctrAVDECGetInfoPtr;

	// 获取视频输出信息
	typedef std::vector<DP_M2S_VO_GET_INFO_S> VctrVOGetInfo;
	typedef boost::shared_ptr<VctrVOGetInfo> VctrVOGetInfoPtr;

	//id for codec & id for thirdparty
	typedef std::map<DP_U32, DP_U32> MapServerTaskID;
	typedef boost::shared_ptr<MapServerTaskID> MapServerTaskIDPtr;

	//input node URL<<--->>codecTaskID
	typedef std::map<DP_U8*, DP_U32> MapCodecTaskID;
	typedef boost::shared_ptr<MapCodecTaskID> MapCodecTaskIDPtr;

	// third id -->url codeID
	typedef std::map<DP_U32, MapCodecTaskID> MapThirdCodecTaskID;
	typedef boost::shared_ptr<MapThirdCodecTaskID> MapThirdCodecTaskIDPtr;

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
	typedef std::map<DP_S32, DP_M2S_AVDEC_GET_INFO_S> MapOutSWMSChCodecDecInfo;
	typedef boost::shared_ptr<MapOutSWMSChCodecDecInfo> MapOutSWMSChCodecDecInfoPtr;

//	typedef std::map<DP_S32, DP_M2S_AVDEC_GET_INFO_S> MapOutSWMSChCodecDecInfo;
//	typedef boost::shared_ptr<MapOutSWMSChCodecDecInfo> MapOutSWMSChCodecDecInfoPtr;

	// third task id -->  _sSrcVideoInfo
	typedef std::map<DP_U32, _sSrcVideoInfo> MapThirdIDSrcVideoInfo;
	typedef boost::shared_ptr<MapThirdIDSrcVideoInfo> MapThirdIDSrcVideoInfoPtr;

#if 0
	//output node thirdTaskID <<--->>URL
	typedef std::map<DP_U32, muduo::string> MapThirdTaskID;
//	typedef boost::shared_ptr<MapThirdTaskID> MapThirdTaskIDPtr;
	//union third id & codec task id
	typedef std::map<MapThirdTaskID, DP_U32> MapThirdCodecTaskID;
	typedef boost::shared_ptr<MapThirdCodecTaskID> MapThirdCodecTaskIDPtr;
	//output node thirdTaskID-->AOCh-->URL
	typedef std::map<eDeviceAudioChannelID, DP_U32> MapAOCHTaskID;
	typedef boost::shared_ptr<MapAOCHTaskID> MapAOCHTaskIDPtr;
#endif

	//AOCh status
	typedef std::map<eDeviceAudioChannelID, DP_BOOL> MapAOChStatus;
	typedef boost::shared_ptr<MapAOChStatus> MapAOChStatusPtr;

	//----------------------------get --------------------------//
	inline const AllAoChInfoPtr getAllAuChInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _sAllAoChInfo;
	}
	inline const AllVencChInfoPtr getAllVencChStreamInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _sAllVencChInfo;
	}
	inline const AllVoChInfoPtr getAllViChInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _sAllVoChInfo;
	}
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
	inline const VctrVOGetInfoPtr getVOGetInfo() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vVOGetInfo;
	}
//	inline const MapCodecTaskIDPtr getInputCodecTaskID() const {
//		muduo::MutexLockGuard lock(_mutex);
//		return _mInputNodeCodecTaskID;
//	}
//	inline const MapCodecTaskIDPtr getOutputCodecTaskID() const {
//		muduo::MutexLockGuard lock(_mutex);
//		return _mOutputNodeCodecTaskID;
//	}
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
//	inline const VctrAllUsedCodecTaskID getAllUsedCodecTaskID() const {
//		muduo::MutexLockGuard lock(_mutex);
//		return _vAllUseCodecTaskID;
//	}
	inline const MapServerTaskIDPtr getudioTaskIDMap() const {
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
	//----------------------------get --------------------------//

	//----------------------------update --------------------------//
	inline void updateAllAuChInfo(AllAoChInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_sAllAoChInfo.swap(newData);
		}
	}
	inline void updateAllVencChStreamInfo(AllVencChInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_sAllVencChInfo.swap(newData);
		}
	}
	inline void updateAllViChInfo(AllVoChInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_sAllVoChInfo.swap(newData);
		}
	}
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
	inline void updateVOGetInfo(VctrVOGetInfoPtr newData) {
		if (newData) {
			muduo::MutexLockGuard lock(_mutex);
			_vVOGetInfo.swap(newData);
		}
	}
//	inline void updateInputCodecTaskID(MapCodecTaskIDPtr newData) {
//		if (newData) {
//			muduo::MutexLockGuard lock(_mutex);
//			_mInputNodeCodecTaskID.swap(newData);
//		}
//	}
//	inline void updateOutputCodecTaskID(MapCodecTaskIDPtr newData) {
//		if (newData) {
//			muduo::MutexLockGuard lock(_mutex);
//			_mOutputNodeCodecTaskID.swap(newData);
//		}
//	}
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
//	inline void updateAllUsedCodecTaskID(VctrAllUsedCodecTaskID newData) {
//		if (newData) {
//			muduo::MutexLockGuard lock(_mutex);
//			_vAllUseCodecTaskID.swap(newData);
//		}
//	}
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
			}
		}
	}
	//----------------------------update --------------------------//

private:
	InitPtr _sInit;
	DeinitPtr _sDeinit;
	AllAoChInfoPtr _sAllAoChInfo;
	AllVencChInfoPtr _sAllVencChInfo;
	AllVoChInfoPtr _sAllVoChInfo;
	mutable muduo::MutexLock _mutex;
	muduo::MutexLock _mutexForUsedID;
	VctrAIGetInfoPtr _vAIGetInfo;
	VctrVIGetInfoPtr _vVIGetInfo;
	VctrAVENCGetInfoPtr _vAVEncGetInfo;
	VctrAVDECGetInfoPtr _vAVDecGetInfo;
	VctrVOGetInfoPtr _vVOGetInfo;

	//new
//	MapCodecTaskIDPtr _mInputNodeCodecTaskID, _mOutputNodeCodecTaskID;
//	MapThirdCodecTaskIDPtr _mThirdCodecTaskID;

	VctrOutCodecTaskID _vAudioTaskID, _vVideoTaskID, _vAuViTaskID;
	DP_U32 _allCodecTaskIDCount;
	MapOutCodecTaskIDBeUsedPtr _mOutCodecTaskIDBeUsed;
	MapOutThirdCodecTaskIDPtr _mOutThirdCodecTaskID;
	VctrAllUsedCodecTaskID _vAllUseCodecTaskID; // must use mutex
	MapOutSWMSChCodecDecInfoPtr _mSwmsChCodecDecInfo;
	MapThirdIDSrcVideoInfoPtr _mThirdIDSrcVideoInfo;
public:
	//new way
	DP_S32 getNewCodecTaskID(DP_U32 thirdId, TaskObjectType_E taskType);
	DP_S32 findNewID(DP_U32 thirdId, VctrOutCodecTaskID TaskID);
	DP_S32 getUsedCodecTaskID(DP_U32 thirdId);
	void  removeCodecTaskID(DP_U32 thirdId, TaskObjectType_E taskType);

	static int recvCB(void* pData, int len);
private:
	void initLocalInfo();

	template<typename T, typename S>
	DP_BOOL getAVInfoFromCodecInfo(T vctrGetInfo, DP_M2S_INFO_TYPE_E type,
			DP_U32 devMax);

	template<typename T, typename S>
	DP_BOOL setAVInfoToCodec(T vAVEnc, DP_M2S_INFO_TYPE_E type);

public:
	MapServerTaskIDPtr _mAudioTaskID, _mVideoTaskID, _mAuViTaskID;
	DP_U32 setServerTaskID(DP_U32 taskID, TaskObjectType_E taskType);
	void removeServerTaskID(DP_U32 taskID, TaskObjectType_E taskType);

//	MapAOCHTaskIDPtr _mAOCHTaskID;
	//use in output node
//	void setTaskIDInMap(DP_U32 codecTaskID, muduo::string url);
	void setInputTaskIDInMap(VctrAVENCGetInfoPtr avEncInfo);
	void setOutputTaskIDInMap(VctrAVDECGetInfoPtr avEncInfo);

//	DP_U32 getNewCodecTaskID(DP_U32 thirdTaskID, muduo::string url);
//	DP_BOOL removeCodecTaskID(DP_U32 codecTaskID);

	DP_BOOL initCodec();
	DP_BOOL deinitCodec();

	DP_BOOL setAVENCInfoToCodec();
	DP_BOOL setAVDECInfoToCodec();

private:
	DP_U32 setID(MapServerTaskIDPtr mTaskID, DP_U32 taskID, DP_U32 min,
			DP_U32 max, TaskObjectType_E taskType);
	void rmID(DP_U32 taskID, MapServerTaskIDPtr mTaskID,
			TaskObjectType_E taskType);

	DP_BOOL setInfoToCodec(muduo::net::Buffer setInfo);

	DP_S32 cmd_set_avenc_default_512(DP_VOID*pPtr);
	DP_S32 cmd_set_avenc_default_513(DP_VOID*pPtr);
	DP_S32 print_avenc_get_attr(DP_M2S_AVENC_SET_INFO_S info);

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
