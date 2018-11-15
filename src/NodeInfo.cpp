/*
 * NodeInfo.cpp
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#include <assert.h>
#include <string.h>
#include "NodeInfo.h"
#include "interactivepro.h"
#include "UnixSockClientData.h"
NodeInfo::NodeInfo() :
		_sInit(new DP_M2S_CMD_INIT_S()), _sDeinit(new DP_M2S_CMD_DEINIT_S()), _vAIGetInfo(
				new VctrAIGetInfo), _vVIGetInfo(new VctrVIGetInfo), _vAVEncGetInfo(
				new VctrAVENCGetInfo), _vAVDecGetInfo(new VctrAVDECGetInfo), _vAOGetInfo(
				new VctrAOGetInfo), _vVOGetInfo(new VctrVOGetInfo), _allCodecTaskIDCount(
				0), _mOutCodecTaskIDBeUsed(new MapOutCodecTaskIDBeUsed), _mOutThirdCodecTaskID(
				new MapOutThirdCodecTaskID), _mSwmsChCodecDecInfo(
				new MapOutSWMSChCodecDecInfo), _mThirdIDSrcVideoInfo(
				new MapThirdIDSrcVideoInfo), _mAODevIDCodecID(
				new MapAODevIDCodecID), _mAudioTaskID(new MapServerTaskID), _mVideoTaskID(
				new MapServerTaskID), _mAuViTaskID(new MapServerTaskID), _vWindowPriority(
				new VctrWindowPriority) {
	initLocalInfo();
}
NodeInfo::~NodeInfo() {
}

//////////////////////////////////////////& type can modify info
void print_DP_M2S_VO_GET_INFO_S_(DP_M2S_VO_GET_INFO_S voInfo) {
	LOG_INFO << "print_DP_M2S_VO_GET_INFO_S_　devid: " << voInfo.s32DevId
			<< " enable: " << voInfo.bEnable << " sync: " << voInfo.enSync;
	voInfo.s32DevId = 10;
	LOG_INFO << "print_DP_M2S_VO_GET_INFO_S_　 stsw[0]::" << " stw.void: "
			<< voInfo.stSwms[0].s32VoDevId << " chn: "
			<< voInfo.stSwms[0].s32SwmsChn << " priority: "
			<< voInfo.stSwms[0].u32Priority << " x "
			<< voInfo.stSwms[0].stRect.s32X << " y: "
			<< voInfo.stSwms[0].stRect.s32Y << " width: "
			<< voInfo.stSwms[0].stRect.u32Width << " height: "
			<< voInfo.stSwms[0].stRect.u32Height;
	LOG_INFO << "print_DP_M2S_VO_GET_INFO_S_　 stsw[1]::" << " stw.void: "
			<< voInfo.stSwms[1].s32VoDevId << " chn: "
			<< voInfo.stSwms[1].s32SwmsChn << " priority: "
			<< voInfo.stSwms[1].u32Priority << " x "
			<< voInfo.stSwms[1].stRect.s32X << " y: "
			<< voInfo.stSwms[1].stRect.s32Y << " width: "
			<< voInfo.stSwms[1].stRect.u32Width << " height: "
			<< voInfo.stSwms[1].stRect.u32Height;

}
void NodeInfo::initLocalInfo() {
	LOG_INFO << "Codec init begin...";
	if (initCodec() != DP_TRUE)
		return;
	LOG_INFO << "Codec init end...";
#if (InputDevice)
	LOG_INFO << "########################  1  ####################";
	//获取输入节点的音频输入的通道信息
	VctrAIGetInfoPtr aiInfo = getAIGetInfo();
	getAVInfoFromCodecInfo<VctrAIGetInfoPtr, DP_M2S_AI_GET_INFO_S>(aiInfo,
			DP_M2S_INFO_TYPE_GET_AI,
			DP_AI_DEV_MAX);
	updateAIGetInfo(aiInfo);
//
	LOG_INFO << "########################  2  ####################";
	VctrVIGetInfoPtr viInfo = getVIGetInfo();//获取输入节点的视频采集通道信息
	getAVInfoFromCodecInfo<VctrVIGetInfoPtr, DP_M2S_VI_GET_INFO_S>(viInfo,
			DP_M2S_INFO_TYPE_GET_VI,
			DP_M2S_VI_DEV_MAX);
	updateVIGetInfo(viInfo);

	LOG_INFO << "########################  3  ####################";

	VctrAVENCGetInfoPtr AVEncInputNodeInfo = getAVEncGetInfo();
	//for_each!!!!!!!
	AVEncInputNodeInfo->clear();
	DP_M2S_AVENC_GET_INFO_S avEncInfo;
	cmd_set_avenc_default_512(&avEncInfo);
	AVEncInputNodeInfo->push_back(avEncInfo);
	memset(&avEncInfo, 0, sizeof(DP_M2S_AVENC_GET_INFO_S));
	cmd_set_avenc_default_513(&avEncInfo);
	AVEncInputNodeInfo->push_back(avEncInfo);

	setAVInfoToCodec<VctrAVENCGetInfo, DP_M2S_AVENC_SET_INFO_S>(
			*AVEncInputNodeInfo.get(), DP_M2S_INFO_TYPE_SET_AVENC);
	updateAVEncGetInfo(AVEncInputNodeInfo);

	LOG_INFO << "########################  4  ####################";

	VctrAVENCGetInfoPtr AVEncInfo = getAVEncGetInfo();
	getAVInfoFromCodecInfo<VctrAVENCGetInfoPtr, DP_M2S_AVENC_GET_INFO_S>(
			AVEncInfo, DP_M2S_INFO_TYPE_GET_AVENC,
			DP_VI_DEV_MAX);
	for (VctrAVENCGetInfo::iterator it = AVEncInfo->begin();
			it != AVEncInfo->end(); it++) {
		LOG_INFO << "print ------------------";
		print_avenc_get_attr(*it);
	}

#elif (OutputDevice)
// input / output  node -->enc info //获取输入节点的视频采集通道的流信息(output node echo video)
	VctrAVENCGetInfoPtr AVEncInfo = getAVEncGetInfo();
	LOG_INFO << "########################  1  ####################";
	getAVInfoFromCodecInfo<VctrAVENCGetInfoPtr, DP_M2S_AVENC_GET_INFO_S>(
			AVEncInfo, DP_M2S_INFO_TYPE_GET_AVENC,
			DP_VI_DEV_MAX);
//	for (VctrAVENCGetInfo::iterator it = AVEncInfo->begin();
//			it != AVEncInfo->end(); it++) {
//		LOG_INFO << "1111111111it url: "
//				<< it->stStream._rtsp.stRtspServer.au8Url << " task ID: "
//				<< it->TskId << " enType : " << it->stStream.enType;
//	}
//	LOG_INFO << "avenc size: " << AVEncInfo->size() << " avenc get url : "
//			<< AVEncInfo->begin()->stStream._rtsp.stRtspServer.au8Url;

	for (VctrAVENCGetInfo::iterator it = AVEncInfo->begin();
			it != AVEncInfo->end(); it++) {
		it->stStream._rtsp.stRtspServer.bOpen = DP_TRUE;
	}
	LOG_INFO
			<< "########################  2  #################### AVEncInfo size:: "
			<< AVEncInfo->size();
	setAVInfoToCodec<VctrAVENCGetInfo, DP_M2S_AVENC_SET_INFO_S>(
			*AVEncInfo.get(), DP_M2S_INFO_TYPE_SET_AVENC);

	LOG_INFO << "########################  3  ####################";
	getAVInfoFromCodecInfo<VctrAVENCGetInfoPtr, DP_M2S_AVENC_GET_INFO_S>(
			AVEncInfo, DP_M2S_INFO_TYPE_GET_AVENC,
			DP_VI_DEV_MAX);
//	for (VctrAVENCGetInfo::iterator it = AVEncInfo->begin();
//			it != AVEncInfo->end(); it++) {
//		LOG_INFO << "333333333333333it url: "
//				<< it->stStream._rtsp.stRtspServer.au8Url << " task ID: "
//				<< it->TskId << " enType : " << it->stStream.enType;
//	}
	updateAVEncGetInfo(AVEncInfo);

//	initCodec();
// output node -->dec info
	//取音视频解码通道信息
	VctrAVDECGetInfoPtr AVDecInfo = getAVDecGetInfo();
	LOG_INFO
			<< "########################  4  #################### AVDecInfo size: "
			<< AVDecInfo->size();
	getAVInfoFromCodecInfo<VctrAVDECGetInfoPtr, DP_M2S_AVDEC_GET_INFO_S>(
			AVDecInfo, DP_M2S_INFO_TYPE_GET_AVDEC,
			DP_VO_DEV_MAX);
	LOG_INFO << " get avedc by out node : size: " << AVDecInfo->size();
//	for (VctrAVDECGetInfo::iterator it = AVDecInfo->begin();
//			it != AVDecInfo->end(); it++) {
//		it->stStream._rtsp.stRtspClient.s8Open = DP_TRUE;
//	}
//	setAVInfoToCodec<VctrAVDECGetInfo, DP_M2S_AVDEC_SET_INFO_S>(
//			*AVDecInfo.get(), DP_M2S_INFO_TYPE_SET_AVDEC);
//	LOG_DEBUG << "AVDecInfo size:: in fun initLocalInfo : " << AVDecInfo->size()
//			<< " DP_M2S_AVDEC_GET_INFO_S sizeof : "
//			<< sizeof(DP_M2S_AVDEC_GET_INFO_S);

	setOutputTaskIDInMap(AVDecInfo);
	updateAVDecGetInfo(AVDecInfo);

	// output node -->vo get info
	// 获取视频输出信息
	VctrVOGetInfoPtr VOInfo = getVOGetInfo();
	LOG_INFO << "########################  5  ####################";
	getAVInfoFromCodecInfo<VctrVOGetInfoPtr, DP_M2S_VO_GET_INFO_S>(VOInfo,
			DP_M2S_INFO_TYPE_GET_VO,
			DP_VO_DEV_MAX);
	LOG_INFO << "VOInfo size:::: === " << VOInfo->size();
	for_each(VOInfo->begin(), VOInfo->end(), print_DP_M2S_VO_GET_INFO_S_);
	updateVOGetInfo(VOInfo);

//	LOG_INFO << "########################  6  ####################";
//	//// 说明： 获取音频输出信息
//	VctrAOGetInfoPtr AOInfo = getAOGetInfo();
//	getAVInfoFromCodecInfo<VctrAOGetInfoPtr, DP_M2S_AO_GET_INFO_S>(AOInfo,
//			DP_M2S_INFO_TYPE_GET_AO,
//			DP_AO_DEV_MAX);
//	updateAOGetInfo(AOInfo);
#endif
}

DP_S32 NodeInfo::getNewCodecTaskID(DP_U32 thirdId, TaskObjectType_E taskType) {
	DP_S32 codecID;
	switch (taskType) {
	case _eAudioTask:
		codecID = findNewID(thirdId, _vAudioTaskID);
		return codecID;
		break;
	case _eVideoTask:
		codecID = findNewID(thirdId, _vVideoTaskID);
		LOG_INFO << "getNewCodecTaskID _vVideoTaskID: " << _vVideoTaskID.size()
				<< " and codecID: " << codecID;
		return codecID;
		break;
	case _eAudioAndVideoTask:
		codecID = findNewID(thirdId, _vAuViTaskID);
		return codecID;
		break;
	case _eTaskObjectTypeButt:
		break;
	}
	return -2; //error!!!!!!
}
DP_S32 NodeInfo::findNewID(DP_U32 thirdId, VctrOutCodecTaskID TaskID) {
	LOG_INFO << "findNewID TaskID.size() : " << TaskID.size();
	LOG_INFO << " _mOutCodecTaskIDBeUsed->operator [](TaskID) "
			<< _mOutCodecTaskIDBeUsed->operator [](TaskID);

	//check thirdId!
	if (_mOutThirdCodecTaskID->find(thirdId) != _mOutThirdCodecTaskID->end())
		return 0;

	if (TaskID.size() <= _mOutCodecTaskIDBeUsed->operator [](TaskID))
		return -1; //error for full !!!!!!
	else {
		muduo::MutexLockGuard lock(_mutexForUsedID);
		for (VctrOutCodecTaskID::iterator it = TaskID.begin();
				it != TaskID.end(); it++) {
			if (find(_vAllUseCodecTaskID.begin(), _vAllUseCodecTaskID.end(),
					*it) == _vAllUseCodecTaskID.end()) {
//				TaskID.push_back(*it);
				_vAllUseCodecTaskID.push_back(*it);
				LOG_INFO << "_vAllUseCodecTaskIDsize1111: "
						<< _vAllUseCodecTaskID.size();
				_mOutCodecTaskIDBeUsed->operator [](TaskID) += 1;
				LOG_INFO
						<< "_mOutCodecTaskIDBeUsed->operator [](TaskID)second : "
						<< _mOutCodecTaskIDBeUsed->operator [](TaskID);
				_mOutThirdCodecTaskID->insert(
						MapOutThirdCodecTaskID::value_type(thirdId, *it));
				return *it;
			}
		}
	}
	return -2;
}
DP_S32 NodeInfo::getUsedCodecTaskID(DP_U32 thirdId) {
	muduo::MutexLockGuard lock(_mutexForUsedID);
	if (_mOutThirdCodecTaskID->find(thirdId) != _mOutThirdCodecTaskID->end())
		return _mOutThirdCodecTaskID->operator [](thirdId);
	else
		return -2;
}
void NodeInfo::removeCodecTaskID(DP_U32 thirdId, TaskObjectType_E taskType) {
	switch (taskType) {
	case _eAudioTask:
		break;
	case _eVideoTask: {
		LOG_INFO << "_mOutCodecTaskIDBeUsed->operator [](TaskID)in remove  : "
				<< _mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID)
				<< " size:: " << _mOutCodecTaskIDBeUsed->size();
		DP_U32 codecID = _mOutThirdCodecTaskID->operator [](thirdId);
		_mOutThirdCodecTaskID->erase(thirdId);
		_vAllUseCodecTaskID.erase(
				find(_vAllUseCodecTaskID.begin(), _vAllUseCodecTaskID.end(),
						codecID));
//		LOG_INFO << "_vAllUseCodecTaskIDsize2222: "
//				<< _vAllUseCodecTaskID.size();
		/////thread safe
//		_vVideoTaskID.erase(
//				find(_vVideoTaskID.begin(), _vVideoTaskID.end(), codecID));
		if (_mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID) > 0)
			_mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID) -= 1;
	}
		break;
	case _eAudioAndVideoTask:
		break;
	case _eTaskObjectTypeButt:
		break;
	}

}

/*
 * 范围定义：
 * 	[0~255]，为音频任务ID，选中此ID范围时，仅可操作音频的相关属性；
 *	[256~511]，为视频任务ID，选中此ID范围时，仅可操作音频的相关属性；
 *	[512~767]，为音视频任务ID，选中此ID范围时，可操作音频和视频的相关属性；
 *	其他预留，无效；
 */
DP_U32 NodeInfo::setServerTaskID(DP_U32 taskID) {
	MapOutThirdCodecTaskIDPtr thirdCodecID = getOutThirdCodecTaskID();
	if (thirdCodecID->find(taskID) != thirdCodecID->end()) {
		return thirdCodecID->operator [](taskID);
	}
	return 0xffff;
}

DP_U32 NodeInfo::setID(MapServerTaskIDPtr mTaskID, DP_U32 taskID, DP_U32 min,
		DP_U32 max, TaskObjectType_E taskType) {
	std::vector<DP_U32> taskIDUsed;
	for (MapServerTaskID::iterator iter = mTaskID->begin();
			iter != mTaskID->end(); iter++) {
		if (iter->second == taskID)
			return iter->first;
		else
			taskIDUsed.push_back(iter->first);
	}
	bool finded = true;
	for (DP_U32 i = min; i <= max; i++) {
		for (std::vector<DP_U32>::iterator it = taskIDUsed.begin();
				it != taskIDUsed.end(); it++) {
			if (*it == i) {
				finded = true;
				break;
			} else
				finded = false;
		}
		if (!finded) {
			mTaskID->insert(MapServerTaskID::value_type(i, taskID));
			//			efficientAddOrUpdate(*mTaskID.get(), i, taskID);
			updateTaskID(mTaskID, taskType);
			return i;
		}
	}
	if (finded)
		return 0xffff;
	return 0xffff;
}

void NodeInfo::removeServerTaskID(DP_U32 taskID, TaskObjectType_E taskType) {
	MapServerTaskIDPtr mTaskID;
	switch (taskType) {
	case _eAudioTask:
		mTaskID = getAudioTaskIDMap();
		rmID(taskID, mTaskID, taskType);
		break;
	case _eVideoTask:
		mTaskID = getVideoTaskIDMap();
		rmID(taskID, mTaskID, taskType);
		break;
	case _eAudioAndVideoTask:
		mTaskID = getAuViTaskIDMap();
		rmID(taskID, mTaskID, taskType);
		break;
	case _eTaskObjectTypeButt:
		break;
	}
}

//stl
void NodeInfo::rmID(DP_U32 taskID, MapServerTaskIDPtr mTaskID,
		TaskObjectType_E taskType) {
	for (MapServerTaskID::iterator iter = mTaskID->begin();
			iter != mTaskID->end();) {
		if (taskID == iter->first) {
			mTaskID->erase(iter++);
			break;
		} else
			++iter;
	}
	updateTaskID(mTaskID, taskType);
}

DP_BOOL NodeInfo::initCodec() {
	UnixSockClientData client(NodeInfo::recvCB);
	try {
		if (client.doSendCommand(_sInit.get(), sizeof(DP_M2S_CMD_INIT_S))
				== 0) {
			return DP_TRUE;
		} else {
			return DP_FALSE;
		}
	} catch (const std::string& selfreason) {
		LOG_FATAL << selfreason;
		return DP_FALSE;
	}
	return DP_TRUE;
}

DP_BOOL NodeInfo::deinitCodec() {
	UnixSockClientData client(NodeInfo::recvCB);
	if (client.doSendCommand(_sDeinit.get(), sizeof(DP_M2S_CMD_INIT_S)) == 0)
		return DP_TRUE;
	else
		return DP_FALSE;
}

void NodeInfo::setOutputTaskIDInMap(VctrAVDECGetInfoPtr avDecInfo) {
	MapOutCodecTaskIDBeUsedPtr mTaskIDUsed = getOutCodecTaskIDBeUsed();
//	assert(avDecInfo->empty());
	vector<DP_S32> vSwmsChn;
	for (DP_U32 i = 0; i < 64; i++)
		vSwmsChn.push_back(i);

	for (VctrAVDECGetInfo::iterator it = avDecInfo->begin();
			it != avDecInfo->end(); it++) {
		LOG_INFO << "AVDecInfo  in fun setOutputTaskIDInMap size : "
				<< avDecInfo->size() << "  enBindType : "
				<< it->AvBindAttr.enBindType << " taskID : " << it->TskId
				<< " swms chn : " << it->stVdec.stSwms.s32SwmsChn;

		if (it->stVdec.bSwms == DP_TRUE) {
			LOG_INFO << "bSwms is true ";
			_mSwmsChCodecDecInfo->insert(
					MapOutSWMSChCodecDecInfo::value_type(
							it->stVdec.stSwms.s32SwmsChn, *it));

			vSwmsChn.erase(
					find(vSwmsChn.begin(), vSwmsChn.end(),
							it->stVdec.stSwms.s32SwmsChn));

			_vWindowPriority->push_back(it->stVdec.stSwms.u32Priority);
		}
		switch (it->AvBindAttr.enBindType) {
		case DP_M2S_AVBIND_ADEC2AO:
			_vAudioTaskID.push_back(it->TskId);
			// without audio
//			_allCodecTaskIDCount++;
			break;
		case DP_M2S_AVBIND_VDEC2VO:
			_vVideoTaskID.push_back(it->TskId);
//			LOG_INFO << "push back :::::::::::::::::::::::: " << it->TskId;
			_allCodecTaskIDCount++;
			break;
		case DP_M2S_AVBIND_ADEC2AO_VDEC2VO:
			_vAuViTaskID.push_back(it->TskId);
			_allCodecTaskIDCount++;
			break;
		case DP_M2S_AVBIND_AI2AO:
			break;
		case DP_M2S_AVBIND_VI2VENC:
			break;
		case DP_M2S_AVBIND_AI2AENC:
			break;
		case DP_M2S_AVBIND_VI2VO:
			break;
		case DP_M2S_AVBIND_AI2AENC_VI2VENC:
			break;
		case DP_M2S_AVBIND_BUTT:
			break;
		}
	}

	sort(_vWindowPriority->begin(), _vWindowPriority->end());
	for (VctrWindowPriority::iterator it = _vWindowPriority->begin();
			it != _vWindowPriority->end(); it++)
		LOG_INFO << "_vWindowPriority: " << *it;
//	LOG_INFO << "vSwmsChn size: " << vSwmsChn.size() << " vSwmsChn[0]: "
//			<< *vSwmsChn.begin() << " vSwmsChn[1] " << vSwmsChn[1]
//			<< " vSwmsChn[2] " << vSwmsChn[2];

	for (VctrAVDECGetInfo::iterator it = avDecInfo->begin();
			it != avDecInfo->end(); it++) {
		if (it->stVdec.bSwms != DP_TRUE) {
			it->stVdec.stSwms.s32SwmsChn = *vSwmsChn.begin();
			vSwmsChn.erase(vSwmsChn.begin());
		}
		LOG_INFO << "after given swms chn " << it->stVdec.stSwms.s32SwmsChn;
	}

	_mOutCodecTaskIDBeUsed->insert(
			MapOutCodecTaskIDBeUsed::value_type(_vAudioTaskID, 0));
	_mOutCodecTaskIDBeUsed->insert(
			MapOutCodecTaskIDBeUsed::value_type(_vVideoTaskID, 0));
	_mOutCodecTaskIDBeUsed->insert(
			MapOutCodecTaskIDBeUsed::value_type(_vAuViTaskID, 0));
	LOG_INFO << "_allCodecTaskIDCount num ::           " << _allCodecTaskIDCount
			<< " _mSwmsChCodecDecInfo size " << _mSwmsChCodecDecInfo->size()
			<< " _vVideoTaskID: " << _vVideoTaskID.size()
			<< " _mOutCodecTaskIDBeUsed size: "
			<< _mOutCodecTaskIDBeUsed->size()
			<< " _mOutCodecTaskIDBeUsed[video]"
			<< _mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID);

}

int NodeInfo::recvCB(void* pData, int len) {
	DP_M2S_CMD_ACK_S *ack = (DP_M2S_CMD_ACK_S*) pData;
	if (ack->u32Success == 0) {
		return 0;
	} else {
		return -1;
	}
}
//
//
//cmd_set_aenc_default（）//0 0 0 0 0
//cmd_set_venc_default（）//256 1920 1080 4000 0
//257 1920 1080 4000 1
//cmd_set_avenc_default（）//512 1920 1080 4000 1 2
//513 1920 1080 4000 2 3

DP_S32 NodeInfo::cmd_set_aenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn) {
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = s32TskId;    //0

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_AI2AENC;/* no video */

	stAttr.AvBindAttr.stAudio.stIn.ModId = DP_M2S_MOD_AI;
	stAttr.AvBindAttr.stAudio.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stAudio.stIn.u32ChnId = 0;    //not use

	stAttr.AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	stAttr.AvBindAttr.stAudio.stOut.u32DevId = 0;    //not use
	stAttr.AvBindAttr.stAudio.stOut.u32ChnId = AencChn;

	//stAttr.AvBindAttr.stVideo = NULL;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */
	stAttr.stAenc.enAlg = DP_M2S_ALG_AAC_ENC;
	stAttr.stAenc.s32Bitrate = 48000;

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = u32Width;
	stCrop.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = u32Width;
	stZoom.stRect.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	DP_M2S_OSD_ATTR_S stOsd;
	stOsd.enType = DP_M2S_OSD_BUTT;
	memcpy(&stAttr.stVenc.stOsd, &stOsd, sizeof(DP_M2S_OSD_ATTR_S));

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = u32Bitrate;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;    //udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;    //not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}
DP_S32 NodeInfo::cmd_set_venc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 VencChn) {
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = s32TskId;										//256~257

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;

	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;					//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;					//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = VencChn;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = u32Width;
	stCrop.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = u32Width;
	stZoom.stRect.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	DP_M2S_OSD_ATTR_S stOsd;
	stOsd.enType = DP_M2S_OSD_BUTT;
	memcpy(&stAttr.stVenc.stOsd, &stOsd, sizeof(DP_M2S_OSD_ATTR_S));

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = u32Bitrate;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;										//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;									//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}

DP_S32 NodeInfo::cmd_set_avenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn,
		DP_S32 VencChn) {
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = s32TskId;											//513

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_AI2AENC_VI2VENC;/* AV */

	stAttr.AvBindAttr.stAudio.stIn.ModId = DP_M2S_MOD_AI;
	stAttr.AvBindAttr.stAudio.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stAudio.stIn.u32ChnId = 0;					//not use

	stAttr.AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	stAttr.AvBindAttr.stAudio.stOut.u32DevId = 0;					//not use
	stAttr.AvBindAttr.stAudio.stOut.u32ChnId = AencChn;

	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;					//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;					//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = VencChn;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */
	stAttr.stAenc.enAlg = DP_M2S_ALG_AAC_ENC;
	stAttr.stAenc.s32Bitrate = 48000;

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = u32Width;
	stCrop.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = u32Width;
	stZoom.stRect.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	DP_M2S_OSD_ATTR_S stOsd;
	stOsd.enType = DP_M2S_OSD_BUTT;
	memcpy(&stAttr.stVenc.stOsd, &stOsd, sizeof(DP_M2S_OSD_ATTR_S));

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = u32Bitrate;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;										//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;									//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}
#if 0
DP_S32 NodeInfo::cmd_set_avenc_default_512(DP_VOID*pPtr) {
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = 512; //512~513

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;
	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 0;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = 1920;
	stCrop.u32Height = 1080;
//	stCrop.u32Width = 3840; //4K
//	stCrop.u32Height = 2160;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = 1920;
	stZoom.stRect.u32Height = 1080;
//	stZoom.stRect.u32Width = 3840; //4K
//	stZoom.stRect.u32Height = 2160;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	/*
	 DP_M2S_OSD_ATTR_S stOsd;//not use
	 memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
	 */

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = 20 * 1024;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}
//
//
DP_S32 NodeInfo::cmd_set_avenc_default_513(DP_VOID*pPtr) {
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = 513;	//512~513

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;
	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 1;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = 1920;
	stCrop.u32Height = 1080;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = 1920;
	stZoom.stRect.u32Height = 1080;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	/*
	 DP_M2S_OSD_ATTR_S stOsd;//not use
	 memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
	 */

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = 20 * 1024;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}
#endif
DP_S32 NodeInfo::print_avenc_get_attr(DP_M2S_AVENC_SET_INFO_S info) {
	printf(
			"#################################################################\n");
	printf("task id:%d\n", info.TskId);
	printf("-------------------------avbind\n");
	printf("av bind type:%d\n", info.AvBindAttr.enBindType);
	printf("av bind audio src mode id:%d\n",
			info.AvBindAttr.stAudio.stIn.ModId);
	printf("av bind audio src dev id:%d\n",
			info.AvBindAttr.stAudio.stIn.u32DevId);
	printf("av bind audio src chn id:%d\n",
			info.AvBindAttr.stAudio.stIn.u32ChnId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.ModId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.u32DevId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.u32ChnId);
	printf("av bind video src mode id:%d\n",
			info.AvBindAttr.stVideo.stIn.ModId);
	printf("av bind video src dev id:%d\n",
			info.AvBindAttr.stVideo.stIn.u32DevId);
	printf("av bind video src chn id:%d\n",
			info.AvBindAttr.stVideo.stIn.u32ChnId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.ModId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.u32DevId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.u32ChnId);
	printf("-------------------------stream\n");
	printf("stream type:%d\n", info.stStream.enType);
	printf("stream rtsp server open:%d\n",
			info.stStream._rtsp.stRtspServer.bOpen);
	printf("stream rtsp server multicast:%d\n",
			info.stStream._rtsp.stRtspServer.bMulticast);
	printf("stream rtsp server transtype:%d\n",
			info.stStream._rtsp.stRtspServer.s32TransType);
	printf("stream rtsp server conntimeout :%d\n",
			info.stStream._rtsp.stRtspServer.s32ConnTimeout);
	printf("stream rtsp server connmax :%d\n",
			info.stStream._rtsp.stRtspServer.s32ConnMax);
	printf("stream rtsp server connnums :%d\n",
			info.stStream._rtsp.stRtspServer.s32ConnNums);
	printf("stream rtsp server au8url :%s\n",
			info.stStream._rtsp.stRtspServer.au8Url);
	LOG_INFO << "url length: "
			<< strlen((DP_CHAR*) info.stStream._rtsp.stRtspServer.au8Url);
	printf("-------------------------stAenc\n");
	printf("aenc alg :%d \n", info.stAenc.enAlg);
	printf("aenc s32bitrate :%d\n", info.stAenc.s32Bitrate);
	printf("-------------------------stVenc\n");
	printf("venc is bCrop :%d\n", info.stVenc.bCrop);
	printf("venc crop status x:%d\n", info.stVenc.stCrop.s32X);
	printf("venc crop status y:%d\n", info.stVenc.stCrop.s32Y);
	printf("venc crop status widget:%d\n", info.stVenc.stCrop.u32Width);
	printf("venc crop status height:%d\n", info.stVenc.stCrop.u32Height);
	printf("venc is zoom :%d\n", info.stVenc.bZoom);
	printf("venc zoom type:%d\n", info.stVenc.stZoom.enType);
	if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RECT) {
		printf("venc zoom rect x:%d\n", info.stVenc.stZoom.stRect.s32X);
		printf("venc zoom rect y:%d\n", info.stVenc.stZoom.stRect.s32Y);
		printf("venc zoom rect widget:%d\n",
				info.stVenc.stZoom.stRect.u32Width);
		printf("venc zoom rect height:%d\n",
				info.stVenc.stZoom.stRect.u32Height);
	} else if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RATIO) {
		printf("venc zoom ratio x:%d\n", info.stVenc.stZoom.stRatio.u32XRatio);
		printf("venc zoom ratio y:%d\n", info.stVenc.stZoom.stRatio.u32YRatio);
		printf("venc zoom ratio widget:%d\n",
				info.stVenc.stZoom.stRatio.u32WRatio);
		printf("venc zoom ratio height:%d\n",
				info.stVenc.stZoom.stRatio.u32HRatio);
	}
	printf("venc is osd:%d\n", info.stVenc.bOsd);
	printf("venc osd type :%d\n", info.stVenc.stOsd.enType);
	if (info.stVenc.stOsd.enType == DP_M2S_OSD_PIC) {
		printf("venc osd pic path :%s\n", info.stVenc.stOsd.au8PicPath);
	} else if (info.stVenc.stOsd.enType == DP_M2S_OSD_STRING) {
		printf("venc osd str path :%s\n", info.stVenc.stOsd.stStr.au8Str);
		printf("venc osd str color :%d\n", info.stVenc.stOsd.stStr.u32Color);
	}
	printf("venc osd display mode :%d\n", info.stVenc.stOsd.enDispMode);
	printf("venc osd point x:%d\n", info.stVenc.stOsd.stPoint.s32X);
	printf("venc osd point y:%d\n", info.stVenc.stOsd.stPoint.s32Y);
	printf("venc alg type :%d\n", info.stVenc.stAlg.enAlg);
	printf("venc alg frmrate:%d\n", info.stVenc.stAlg.stH264Enc.u32FrmRate);
	printf("venc alg stSize Widget:%d\n",
			info.stVenc.stAlg.stH264Enc.stSize.u32Width);
	printf("venc alg stSize Height:%d\n",
			info.stVenc.stAlg.stH264Enc.stSize.u32Height);
	printf("venc alg RcMode:%d\n", info.stVenc.stAlg.stH264Enc.enRcMode);
	printf("venc alg BitRate:%d\n", info.stVenc.stAlg.stH264Enc.u32Bitrate);
	printf("venc alg Profile:%d\n", info.stVenc.stAlg.stH264Enc.enProfile);
	printf("venc alg gop:%d\n", info.stVenc.stAlg.stH264Enc.u32Gop);
	printf("venc alg sf:%d\n", info.stVenc.stAlg.stH264Enc.u16SF);
	printf("venc alg tf:%d\n", info.stVenc.stAlg.stH264Enc.u16TF);
	printf(
			"#################################################################\n");
	return 0;
}
