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
#include "LogicHandle.h"

NodeInfo::NodeInfo() :
		_vAIGetInfo(new VctrAIGetInfo), _vVIGetInfo(new VctrVIGetInfo), _vAVEncGetInfo(
				new VctrAVENCGetInfo), _vAVDecGetInfo(new VctrAVDECGetInfo), _vAOGetInfo(
				new VctrAOGetInfo), _vVOGetInfo(new VctrVOGetInfo), _allCodecTaskIDCount(
				0), _mOutCodecTaskIDBeUsed(new MapOutCodecTaskIDBeUsed), _mOutThirdCodecTaskID(
				new MapOutThirdCodecTaskID), _mSwmsChCodecDecInfo(
				new MapOutSWMSChCodecDecInfo), _mThirdIDSrcVideoInfo(
				new MapThirdIDSrcVideoInfo), _mAODevIDCodecID(
				new MapAODevIDCodecID), _mAODevAudioInfo(new MapAODevAudioInfo), _mAudioTaskID(
				new MapServerTaskID), _mVideoTaskID(new MapServerTaskID), _mAuViTaskID(
				new MapServerTaskID), _vWindowPriority(new VctrWindowPriority), _bCodecInited(
				DP_FALSE) {
	_netInfo.setIfname(g_IFNAMEDEV);
	_netInfo.getNetworkConfig();
	initLocalInfo();

}
NodeInfo::~NodeInfo() {
}

//////////////////////////////////////////& type can modify info
void print_DP_M2S_VO_GET_INFO_S_(DP_M2S_VO_INFO_S voInfo) {
//	LOG_DEBUG << "print_DP_M2S_VO_GET_INFO_S_　devid: " << voInfo.enDevId
//			<< " enable: " << voInfo.bEnable << " sync: " << voInfo.enSync;
//	LOG_DEBUG << "print_DP_M2S_VO_GET_INFO_S_　 stsw[0]::" << " stw.void: "
//			<< voInfo.stSwms[0].enVoDevId << " chn: "
//			<< voInfo.stSwms[0].u32SwmsChn << " priority: "
//			<< voInfo.stSwms[0].u32Priority << " x "
//			<< voInfo.stSwms[0].stRect.s32X << " y: "
//			<< voInfo.stSwms[0].stRect.s32Y << " width: "
//			<< voInfo.stSwms[0].stRect.u32Width << " height: "
//			<< voInfo.stSwms[0].stRect.u32Height;
//	LOG_DEBUG << "print_DP_M2S_VO_GET_INFO_S_　 stsw[1]::" << " stw.void: "
//			<< voInfo.stSwms[1].enVoDevId << " chn: "
//			<< voInfo.stSwms[1].u32SwmsChn << " priority: "
//			<< voInfo.stSwms[1].u32Priority << " x "
//			<< voInfo.stSwms[1].stRect.s32X << " y: "
//			<< voInfo.stSwms[1].stRect.s32Y << " width: "
//			<< voInfo.stSwms[1].stRect.u32Width << " height: "
//			<< voInfo.stSwms[1].stRect.u32Height;

}

void print_DP_M2S_VI_GET_INFO_S(DP_M2S_VI_GET_INFO_S viInfo) {
	LOG_DEBUG << "viInfo.bConn" << viInfo.bConn;
	LOG_DEBUG << "viInfo.bSignal" << viInfo.bSignal;
	LOG_DEBUG << "viInfo.enDevId" << viInfo.enDevId;
	LOG_DEBUG << "viInfo.u32FrmRate" << viInfo.u32FrmRate;
	LOG_DEBUG << "viInfo.stCap.s32X" << viInfo.stCap.s32X;
	LOG_DEBUG << "viInfo.stCap.s32Y" << viInfo.stCap.s32Y;
	LOG_DEBUG << "viInfo.stCap.u32Height" << viInfo.stCap.u32Height;
	LOG_DEBUG << "viInfo.stCap.u32Width" << viInfo.stCap.u32Width;
	LOG_DEBUG << "viInfo.stDst.u32Height" << viInfo.stDst.u32Height;
	LOG_DEBUG << "viInfo.stDst.u32Width" << viInfo.stDst.u32Width;
}

void print_DP_M2S_AI_GET_INFO_S(DP_M2S_AI_GET_INFO_S aiInfo) {
	LOG_DEBUG << "aiInfo.bSignal" << aiInfo.bSignal;
	LOG_DEBUG << "aiInfo.bsConn" << aiInfo.bsConn;
	LOG_DEBUG << "aiInfo.enDevId" << aiInfo.enDevId;
	LOG_DEBUG << "aiInfo.u8Vol" << aiInfo.u8Vol;
	LOG_DEBUG << "aiInfo.stCommAttr.enBitwidth" << aiInfo.stCommAttr.enBitwidth;
	LOG_DEBUG << "aiInfo.stCommAttr.enSamplerate"
			<< aiInfo.stCommAttr.enSamplerate;
	LOG_DEBUG << "aiInfo.stCommAttr.enSoundmode"
			<< aiInfo.stCommAttr.enSoundmode;
}

void NodeInfo::initLocalInfo() {
	///	//
	//	//		[0,256)，为音频编码任务ID，选中此ID范围时，仅可操作音频编码的相关属性；
	//	// *	[256,512)，为音频解码任务ID，选中此ID范围时，仅可操作音频解码的相关属性；
	//	// *	[512,1024)，为视频编码任务ID，选中此ID范围时，仅可操作视频编码的相关属性；
	//	// *	[1024,1280)，为视频解码任务ID，选中此ID范围时，仅可操作视频解码的相关属性；
	//	// *	[1280,1536)，为音视频编码任务ID，选中此ID范围时，仅可操作音视频编码的相关属性；
	//	// *	[1536,1792)，为音视频解码任务ID，选中此ID范围时，仅可操作音视频解码的相关属性；
	//	// *	其他预留，无效；
	LOG_INFO << "Codec init begin...";
	if (initCodec() != DP_TRUE) {
		LOG_ERROR << "Init failed !";
		return;
	}

#if (InputDevice)
//init input node
	initInAVEnc();
	initInGetVI();
	initInGetAI();

#elif (OutputDevice)
	//init output node

	initOutAVEnc();
	initOutAVDec();
	initOutGetVO();
	initOutGetAO();
#endif
	_bCodecInited = DP_TRUE;
	LOG_INFO << "Codec init end...";
}

DP_BOOL NodeInfo::initOutAVEnc() {
#if (OutputDevice)
	VctrAVENCGetInfoPtr AVEncInfo = getAVEncGetInfo();
	LOG_INFO
			<< "[ out ] ###################### [ init ] [ 1 ] [ set avenc ]  ###################";

	boost::shared_ptr<DP_M2S_AVBIND_ATTR_S> avBind(new DP_M2S_AVBIND_ATTR_S);
	avBind->enBindType = DP_M2S_AVBIND_VI2VENC;
	avBind->stAudio.stIn.ModId = DP_M2S_MOD_AI;
	avBind->stAudio.stIn.u32DevId = DP_M2S_AI_DEV_LINEIN0_HI3536;
	avBind->stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	avBind->stAudio.stOut.u32ChnId = 0;
	avBind->stVideo.stIn.ModId = DP_M2S_MOD_VO;
	avBind->stVideo.stIn.u32DevId = DP_M2S_VO_DEV_HDMI0_HI3536;
	avBind->stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	avBind->stVideo.stOut.u32ChnId = 0;

	boost::shared_ptr<DP_M2S_AENC_ATTR_S> aenc(new DP_M2S_AENC_ATTR_S);
	aenc->stAlg.enAlg = DP_M2S_ALG_AAC_ENC;
	aenc->stAlg.stAACEnc.bAdts = DP_TRUE;
	aenc->stAlg.stAACEnc.u32Bitrate = 128000;

	boost::shared_ptr<DP_M2S_VENC_ATTR_S> venc(new DP_M2S_VENC_ATTR_S);
	venc->bCrop = DP_FALSE;
	venc->bOsd = DP_FALSE;
	venc->bZoom = DP_FALSE;

	boost::shared_ptr<DP_M2S_RTSP_SERVER_ATTR_S> streamServer(
			new DP_M2S_RTSP_SERVER_ATTR_S);
	DP_CHAR url[DP_M2S_URL_LEN] = { 0 };
	const DP_CHAR*ip = _netInfo.getNetConfStruct().ipAddr.c_str();
	DP_U32 len1 = strlen("rtsp://");
	DP_U32 len2 = strlen(ip);
	DP_U32 len3 = strlen(":50000/Media");
	memcpy(url, "rtsp://", len1);
	memcpy(url + len1, ip, len2);
	memcpy(url + len1 + len2, ":50000/Media", len3);

	memset(streamServer->au8Url, 0, DP_M2S_URL_LEN);
	strcpy((DP_CHAR*) streamServer->au8Url, url);
	streamServer->bMulticast = DP_FALSE;
	streamServer->bOpen = DP_TRUE;
	streamServer->bUDP = DP_FALSE;

	DP_S32 taskID = 1300;

	boost::shared_ptr<DP_M2S_AVENC_INFO_S> avEnc(
			new DP_M2S_AVENC_INFO_S(taskID, *avBind.get(), *aenc.get(),
					*venc.get(), DP_M2S_STREAM_RTSP_SERVER,
					*streamServer.get()));

	AVEncInfo->clear();
	AVEncInfo->push_back(*avEnc.get());

	updateAVEncGetInfo(AVEncInfo);
	printAVENC(avEnc.get());
	if (setAVInfoToCodec<VctrAVENCGetInfo, DP_M2S_CMD_AVENC_SETINFO_S>(
			AVEncInfo, DP_M2S_CMD_AVENC_SET) != DP_TRUE) {
		LOG_ERROR << "Set avEnc failed !";
		return DP_FALSE;
	}

	VecCodecTaskID vTaskID;
	vTaskID.push_back(taskID);
	getAVInfoFromCodec<VctrAVENCGetInfo, DP_M2S_CMD_AVENC_SETINFO_S>(vTaskID,
			DP_M2S_CMD_AVENC_GET);

#endif
	return DP_TRUE;
}

DP_BOOL NodeInfo::initOutAVDec() {
#if (OutputDevice)
	LOG_INFO
			<< "[ out ] ###################### [ init ] [ 2 ] [ set avdec ]  ###################";
	DP_S32 taskID = 0;
	DP_U32 chnID = 0;
	VctrAVDECGetInfoPtr AVDecInfo = getAVDecGetInfo();
	UnixSockClientData client(NodeInfo::recvCB);
	DP_S32 retSend = 0;
	boost::shared_ptr<DP_M2S_AVDEC_INFO_S> avDec(new DP_M2S_AVDEC_INFO_S());
//	boost::shared_ptr<DP_M2S_CMD_AVDEC_SETINFO_S> sendAVDec(
//			new DP_M2S_CMD_AVDEC_SETINFO_S(DP_M2S_CMD_AVDEC_SET));

	AVDecInfo->clear();
	for (taskID = 256; taskID <= 257; taskID++) {
		initAVDec(avDec.get(), taskID, chnID++);
		AVDecInfo->push_back(*avDec.get());
//		sendAVDec->stInfo = *avDec.get();
//		try {
//			retSend = client.doSendCommand(sendAVDec.get(),
//					sizeof(DP_M2S_CMD_AVDEC_SETINFO_S));
//			if (retSend != 0)
//				LOG_ERROR << "Send avdec failed taskiD: " << taskID;
//		} catch (SystemException &ex) {
//			LOG_ERROR << ex.what() << " task id :" << taskID;
//		}
	}
	//}
	for (taskID = 1536; taskID <= 1541; taskID++) {
		initAVDec(avDec.get(), taskID, chnID++);
		AVDecInfo->push_back(*avDec.get());
//		sendAVDec->stInfo = *avDec.get();
//		try {
//			retSend = client.doSendCommand(sendAVDec.get(),
//					sizeof(DP_M2S_CMD_AVDEC_SETINFO_S));
//			if (retSend != 0)
//				LOG_ERROR << "Send avdec failed taskiD: " << taskID;
//		} catch (SystemException &ex) {
//			LOG_ERROR << ex.what() << " task id :" << taskID;
//		}
	}
	if (setAVInfoToCodec<VctrAVDECGetInfo, DP_M2S_CMD_AVDEC_SETINFO_S>(
			AVDecInfo, DP_M2S_CMD_AVDEC_SET) != DP_TRUE) {
		LOG_ERROR << "Set av Dec failed !";
		// jhbnote will restart prog or not ?
	} else {
		setOutputTaskIDInMap(AVDecInfo);
	}

	updateAVDecGetInfo(AVDecInfo);

	//re get avdec for checking avdec setting is correct or not.
	vector<DP_S32> codecID;
	VctrOutCodecTaskID::iterator it;
	if (!_vAudioTaskID.empty())
		for (it = _vAudioTaskID.begin(); it != _vAudioTaskID.end(); it++)
			codecID.push_back(*it);
	if (!_vVideoTaskID.empty())
		for (it = _vVideoTaskID.begin(); it != _vVideoTaskID.end(); it++)
			codecID.push_back(*it);
	if (!_vAuViTaskID.empty())
		for (it = _vAuViTaskID.begin(); it != _vAuViTaskID.end(); it++)
			codecID.push_back(*it);

	getAVInfoFromCodec<VctrAVDECGetInfo, DP_M2S_CMD_AVDEC_SETINFO_S>(codecID,
			DP_M2S_CMD_AVDEC_GET);

#endif
	return DP_TRUE;
}

DP_BOOL NodeInfo::initOutGetVO() {
#if (OutputDevice)
	LOG_INFO
			<< "[ out ] ###################### [ init ] [ 3 ] [ get VO ]  ###################";
	// 获取视频输出信息
	VecVODEV voDev;
	voDev.push_back(DP_M2S_VO_DEV_HDMI0_HI3536);
	VctrVOGetInfoPtr VOInfo = getVOGetInfo();

	getAOVOInfoFromCodec<VctrVOGetInfoPtr, DP_M2S_CMD_VO_GETINFO_S, VecVODEV,
			DP_M2S_CMD_VO_GETINFO_ACK_S>(VOInfo, DP_M2S_CMD_VO_GET, voDev);

	for_each(VOInfo->begin(), VOInfo->end(), print_DP_M2S_VO_GET_INFO_S_);
	LOG_INFO << "Get aovo size获取视频输出信息 : " << VOInfo->size();
	updateVOGetInfo(VOInfo);
#endif
	return DP_TRUE;
}
DP_BOOL NodeInfo::initOutGetAO() {
#if (OutputDevice)
	LOG_INFO
			<< "[ out ] ###################### [ init ] [ 4 ] [ get AO ]  ###################";
	//	//// 说明： 获取音频输出信息
	VecAODEV aoDev;
	aoDev.push_back(DP_M2S_AO_DEV_LINEOUT0_HI3536);
	aoDev.push_back(DP_M2S_AO_DEV_HDMI0_HI3536);
	VctrAOGetInfoPtr AOInfo = getAOGetInfo();
	getAOVOInfoFromCodec<VctrAOGetInfoPtr, DP_M2S_CMD_AO_GETINFO_S, VecAODEV,
			DP_M2S_CMD_AO_GETINFO_ACK_S>(AOInfo, DP_M2S_CMD_AO_GET, aoDev);
	LOG_INFO << "Get aovo size获取音频输出信息 : " << AOInfo->size();
	updateAOGetInfo(AOInfo);
#endif
	return DP_TRUE;
}

DP_BOOL NodeInfo::initInAVEnc() {
#if (InputDevice)
	LOG_INFO
	<< "[ input ] ###################### [ init ] [ 1 ] [ set AEnc ]  ###################";
	boost::shared_ptr<DP_M2S_AVENC_INFO_S> aEnc(new DP_M2S_AVENC_INFO_S);
	VctrAVENCGetInfoPtr AVEncInfo = getAVEncGetInfo();
	AVEncInfo->clear();
	cmd_set_aenc_default(aEnc.get(), 0, 0, 0, 0, 0);
	AVEncInfo->push_back(*aEnc.get());
	LOG_INFO
	<< "[ input ] ###################### [ init ] [ 2 ] [ set VEnc ]  ###################";
	cmd_set_venc_default(aEnc.get(), 512, 1920, 1080, 4000, 0);
	AVEncInfo->push_back(*aEnc.get());
	cmd_set_venc_default(aEnc.get(), 513, 1920, 1080, 4000, 1);
	AVEncInfo->push_back(*aEnc.get());
	LOG_INFO
	<< "[ input ] ###################### [ init ] [ 3 ] [ set VEnc ]  ###################";
	cmd_set_avenc_default(aEnc.get(), 1280, 1920, 1080, 4000, 1, 2);
	AVEncInfo->push_back(*aEnc.get());
	cmd_set_avenc_default(aEnc.get(), 1281, 1920, 1080, 4000, 2, 3);
	AVEncInfo->push_back(*aEnc.get());

	if (setAVInfoToCodec<VctrAVENCGetInfo, DP_M2S_CMD_AVENC_SETINFO_S>(
					AVEncInfo, DP_M2S_CMD_AVENC_SET) != DP_TRUE) {
		LOG_ERROR << "Set AV Enc failed !";
		return DP_FALSE;
		// jhbnote will restart prog or not ?
	} else {
		updateAVEncGetInfo(AVEncInfo);
		return DP_TRUE;
	}
#endif
	return DP_TRUE;
}

DP_BOOL NodeInfo::initInGetVI() {
#if (InputDevice)
	LOG_INFO
	<< "[ input ] ###################### [ init ] [ 4 ] [ get VI ]  ###################";
	VctrVIGetInfoPtr viInfo = getVIGetInfo();	//获取输入节点的视频采集通道信息
	VecVIDEV viDev;
	viDev.push_back(DP_M2S_VI_DEV_HDMI0_ITE6801);
	getAOVOInfoFromCodec<VctrVIGetInfoPtr, DP_M2S_CMD_VI_GETINFO_S, VecVIDEV,
	DP_M2S_CMD_VI_GETINFO_ACK_S>(viInfo, DP_M2S_CMD_VO_GET, viDev);

	for_each(viInfo->begin(), viInfo->end(), print_DP_M2S_VI_GET_INFO_S);
	LOG_INFO << "Get aovo size获取输入节点的视频采集通道信息 : " << viInfo->size();
	updateVIGetInfo(viInfo);
#endif
	return DP_TRUE;
}

DP_BOOL NodeInfo::initInGetAI() {
#if (InputDevice)
	LOG_INFO
	<< "[ input ] ###################### [ init ] [ 5 ] [ get AI ]  ###################";
	//获取输入节点的音频输入的通道信息
	VctrAIGetInfoPtr aiInfo = getAIGetInfo();
	VecAIDEV aiDev;
	aiDev.push_back(DP_M2S_AI_DEV_LINEIN0_HI3536);
	aiDev.push_back(DP_M2S_AI_DEV_HDMI0_ITE6801);
	getAOVOInfoFromCodec<VctrAIGetInfoPtr, DP_M2S_CMD_AI_GETINFO_S, VecAIDEV,
	DP_M2S_CMD_AI_GETINFO_ACK_S>(aiInfo, DP_M2S_CMD_VO_GET, aiDev);
	for_each(aiInfo->begin(), aiInfo->end(), print_DP_M2S_AI_GET_INFO_S);
	LOG_INFO << "Get aovo size获取输入节点的音频输入的通道信息 : " << aiInfo->size();
	updateAIGetInfo(aiInfo);
#endif
	return DP_TRUE;
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
		LOG_DEBUG << "getNewCodecTaskID _vVideoTaskID: " << _vVideoTaskID.size()
				<< " and codecID: " << codecID;
		return codecID;
		break;
	case _eAudioAndVideoTask:
		codecID = findNewID(thirdId, _vAuViTaskID);
		return codecID;
		break;
	case _eUnknowTask:
		break;
	case _eTaskObjectTypeButt:
		break;
	}
	return DP_ERR_PROTOCOL_PRASE_PACKET_TYPE; //error!!!!!!
}

DP_S32 NodeInfo::findNewID(DP_U32 thirdId, VctrOutCodecTaskID TaskID) {
	LOG_INFO << "findNewID TaskID.size() : " << TaskID.size();
	LOG_INFO << " _mOutCodecTaskIDBeUsed->operator [](TaskID) "
			<< _mOutCodecTaskIDBeUsed->operator [](TaskID);

//check thirdId!
	if (_mOutThirdCodecTaskID->find(thirdId) != _mOutThirdCodecTaskID->end()) {
		LOG_ERROR << "The repetition third id ";
		return DP_ERR_THIRD_TASK_ID;
	}

	if (TaskID.size() <= _mOutCodecTaskIDBeUsed->operator [](TaskID))
		return DP_ERR_FULL_CODEC_TASK_ID; //error for full !!!!!!
	else {
		muduo::MutexLockGuard lock(_mutexForUsedID);
		for (VctrOutCodecTaskID::iterator it = TaskID.begin();
				it != TaskID.end(); it++) {
			if (find(_vAllUseCodecTaskID.begin(), _vAllUseCodecTaskID.end(),
					*it) == _vAllUseCodecTaskID.end()) {
				_vAllUseCodecTaskID.push_back(*it);
				LOG_DEBUG << "_vAllUseCodecTaskIDsize1111: "
						<< _vAllUseCodecTaskID.size();
				_mOutCodecTaskIDBeUsed->operator [](TaskID) += 1;
				LOG_DEBUG
						<< "_mOutCodecTaskIDBeUsed->operator [](TaskID)second : "
						<< _mOutCodecTaskIDBeUsed->operator [](TaskID);
				_mOutThirdCodecTaskID->insert(
						MapOutThirdCodecTaskID::value_type(thirdId, *it));
				return *it;
			}
		}
	}
	return DP_ERR_TASK_SOURCE_NOTENOUGH;
}
DP_S32 NodeInfo::getUsedCodecTaskID(DP_U32 thirdId) {
	muduo::MutexLockGuard lock(_mutexForUsedID);
	if (_mOutThirdCodecTaskID->find(thirdId) != _mOutThirdCodecTaskID->end())
		return _mOutThirdCodecTaskID->operator [](thirdId);
	else
		return -2;
}

//*	[0,256)，为音频编码任务ID，选中此ID范围时，仅可操作音频编码的相关属性；
//*	[256,512)，为音频解码任务ID，选中此ID范围时，仅可操作音频解码的相关属性；
//*	[512,1024)，为视频编码任务ID，选中此ID范围时，仅可操作视频编码的相关属性；
//*	[1024,1280)，为视频解码任务ID，选中此ID范围时，仅可操作视频解码的相关属性；
//*	[1280,1536)，为音视频编码任务ID，选中此ID范围时，仅可操作音视频编码的相关属性；
//*	[1536,1792)，为音视频解码任务ID，选中此ID范围时，仅可操作音视频解码的相关属性；

void NodeInfo::removeCodecTaskID(DP_U32 thirdId) {
	LOG_INFO << "Remove third task id :" << thirdId;
	MapOutThirdCodecTaskIDPtr thirdCodecID = getOutThirdCodecTaskID();
	DP_S32 id = 0;
	TaskObjectType_E taskType;
	if (thirdCodecID->find(thirdId) != thirdCodecID->end())
		id = thirdCodecID->operator [](thirdId);
	if (id >= 256 && id < 512)
		taskType = _eAudioTask;
	else if (id >= 1024 && id < 1280)
		taskType = _eVideoTask;
	else if (id >= 1536 && id < 1792)
		taskType = _eAudioAndVideoTask;
	LOG_INFO << "codec id in remove :" << id << " taskType: " << taskType;
	muduo::MutexLockGuard lock(_mutexForUsedID);
	switch (taskType) {
	case _eAudioTask: {
		LOG_INFO << "_mOutCodecTaskIDBeUsed->operator [](TaskID)in remove  : "
				<< _mOutCodecTaskIDBeUsed->operator [](_vAudioTaskID)
				<< " size:: " << _mOutCodecTaskIDBeUsed->size()
				<< " _mOutCodecTaskIDBeUsed->operator [](_vAudioTaskID): "
				<< _mOutCodecTaskIDBeUsed->operator [](_vAudioTaskID);
		if (_mOutThirdCodecTaskID->find(thirdId)
				== _mOutThirdCodecTaskID->end()) {
			LOG_ERROR << "Can not find thirdId: " << thirdId
					<< " in _mOutThirdCodecTaskID !";
			return;
		}
		DP_U32 codecID = _mOutThirdCodecTaskID->operator [](thirdId);
		_mOutThirdCodecTaskID->erase(thirdId);
		_vAllUseCodecTaskID.erase(
				find(_vAllUseCodecTaskID.begin(), _vAllUseCodecTaskID.end(),
						codecID));
		if (_mOutCodecTaskIDBeUsed->operator [](_vAudioTaskID) > 0)
			_mOutCodecTaskIDBeUsed->operator [](_vAudioTaskID) -= 1;
	}
		break;
	case _eVideoTask: {
		LOG_INFO << "_mOutCodecTaskIDBeUsed->operator [](TaskID)in remove  : "
				<< _mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID)
				<< " size:: " << _mOutCodecTaskIDBeUsed->size()
				<< " _mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID): "
				<< _mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID);
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
	case _eAudioAndVideoTask: {
		LOG_INFO << "_mOutCodecTaskIDBeUsed->operator [](TaskID)in remove  : "
				<< _mOutCodecTaskIDBeUsed->operator [](_vAuViTaskID)
				<< " size:: " << _mOutCodecTaskIDBeUsed->size()
				<< " _mOutCodecTaskIDBeUsed->operator [](_vAuViTaskID): "
				<< _mOutCodecTaskIDBeUsed->operator [](_vAuViTaskID);
		DP_U32 codecID = _mOutThirdCodecTaskID->operator [](thirdId);
		_mOutThirdCodecTaskID->erase(thirdId);
		_vAllUseCodecTaskID.erase(
				find(_vAllUseCodecTaskID.begin(), _vAllUseCodecTaskID.end(),
						codecID));
		if (_mOutCodecTaskIDBeUsed->operator [](_vAuViTaskID) > 0)
			_mOutCodecTaskIDBeUsed->operator [](_vAuViTaskID) -= 1;
	}
		break;
	case _eUnknowTask:
		break;
	case _eTaskObjectTypeButt:
		break;
	}

}

void NodeInfo::updateThirdTaskIDCodecTaskid(DP_U32 thirdId, DP_S32 codecID) {
	LOG_INFO << "Update third task id :" << thirdId;
	MapOutThirdCodecTaskIDPtr thirdCodecID = getOutThirdCodecTaskID();
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;
	it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
			bind2nd(LogicHandle::findThirdIDByCodecID(), codecID));
	if (it_ID != thirdCodecID->end()) {
		thirdCodecID->erase(it_ID);
		thirdCodecID->insert(
				MapOutThirdCodecTaskID::value_type(thirdId, codecID));
		updateMapOutThirdCodecTaskID(thirdCodecID);
	} else {
		LOG_ERROR << "Can not find third task id : " << thirdId;
		return;
	}
}

DP_U32 NodeInfo::setServerTaskID(DP_U32 taskID) {
	MapOutThirdCodecTaskIDPtr thirdCodecID = getOutThirdCodecTaskID();
	if (thirdCodecID->find(taskID) != thirdCodecID->end()) {
		return thirdCodecID->operator [](taskID);
	}
	return DP_ERR_TASK_ID_NOTEXIST;
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
	case _eUnknowTask:
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

	DP_M2S_AI_SET_INFO_S aiSet(DP_M2S_AI_DEV_LINEIN0_HI3536, 100,
			DP_M2S_AUDIO_SAMPLE_RATE_48000, DP_M2S_AUDIO_BIT_WIDTH_16,
			DP_M2S_AUDIO_SOUND_MODE_STEREO);

	DP_M2S_RECT_S rect;
	DP_M2S_SWMS_ATTR_S swms(DP_M2S_VO_DEV_HDMI0_HI3536, DP_M2S_SWMS_MAX + 1, 0,
			rect);
	DP_M2S_VO_INFO_S voSet(DP_M2S_VO_DEV_HDMI0_HI3536, DP_TRUE,
			DP_M2S_VIDEO_SYNC_1080P60, &swms, 1, DP_M2S_CSC_ATTR_S());

#if (InputDevice)
	DP_M2S_AI_SET_INFO_S aiSet_2 = aiSet;
	aiSet_2.enDevId = DP_M2S_AI_DEV_HDMI0_ITE6801;

	DP_M2S_AI_SET_INFO_S aiInfo[2] = {aiSet, aiSet_2};

	boost::shared_ptr<DP_M2S_CMD_SYS_INIT_S> sysInit(
			new DP_M2S_CMD_SYS_INIT_S(aiInfo, DP_M2S_AI_DEV_MAX, &voSet, 1));

#elif (OutputDevice)
	boost::shared_ptr<DP_M2S_CMD_SYS_INIT_S> sysInit(
			new DP_M2S_CMD_SYS_INIT_S(&aiSet, 1, &voSet, 1));
#endif
//	try {
	DP_S32 retSend = client.doSendCommand(sysInit.get(),
			sizeof(DP_M2S_CMD_SYS_INIT_S));
	if (retSend == 0)
		return DP_TRUE;
	else {
		return DP_FALSE;
	}
//	} catch (SystemException &ex) {
//		LOG_ERROR << ex.what();
//		return DP_FALSE;
//	}
	return DP_TRUE;
}

DP_BOOL NodeInfo::deinitCodec() {
	DP_M2S_INF_PROT_HEAD_S deinit(sizeof(DP_M2S_INF_PROT_HEAD_S),
			DP_M2S_CMD_SYS_DEINIT, g_NeedReply);
	DP_S32 retSend = 0;
	NodeInfo::sendToCodecAndRecv(retSend, &deinit,
			sizeof(DP_M2S_INF_PROT_HEAD_S));
	if (retSend != 0) {
		LOG_ERROR << "Deinit failed.";
		return DP_FALSE;
	} else {
		LOG_WARN << "Deinit ok.";
		return DP_TRUE;
	}
}

void NodeInfo::setOutputTaskIDInMap(VctrAVDECGetInfoPtr avDecInfo) {
	vector<DP_S32> vSwmsChn;
	for (DP_U32 i = 0; i < 64; i++)
		vSwmsChn.push_back(i);

	for (VctrAVDECGetInfo::iterator it = avDecInfo->begin();
			it != avDecInfo->end(); it++) {
		LOG_DEBUG << "AVDecInfo in func setOutputTaskIDInMap size : "
				<< avDecInfo->size() << "  enBindType : "
				<< it->AvBindAttr.enBindType << " taskID : " << it->s32TskId
				<< " swms chn : " << it->stVdec.stSwms.u32SwmsChn;

		//used swms == true
		if (it->stVdec.bSwms == DP_TRUE) {
			LOG_DEBUG << "bSwms is true ";
			_mSwmsChCodecDecInfo->insert(
					MapOutSWMSChCodecDecInfo::value_type(
							it->stVdec.stSwms.u32SwmsChn, *it));

			vector<DP_S32>::iterator it_find = find(vSwmsChn.begin(),
					vSwmsChn.end(), it->stVdec.stSwms.u32SwmsChn);
			if (it_find != vSwmsChn.end()) {
				vSwmsChn.erase(it_find);
			} else {
				LOG_WARN << "Can not find swms chn id : "
						<< it->stVdec.stSwms.u32SwmsChn << " in vSwmsChn";
			}
			_vWindowPriority->push_back(it->stVdec.stSwms.u32Priority);
		}

		DP_M2S_AVBIND_TYPE_E bindType = it->AvBindAttr.enBindType;
		if (bindType == DP_M2S_AVBIND_ADEC2AO) {
			_vAudioTaskID.push_back(it->s32TskId);
			_allCodecTaskIDCount++;
		} else if (bindType == DP_M2S_AVBIND_VDEC2VO) {
			_vVideoTaskID.push_back(it->s32TskId);
			_allCodecTaskIDCount++;
		} else if (bindType == DP_M2S_AVBIND_ADEC2AO_VDEC2VO) {
			_vAuViTaskID.push_back(it->s32TskId);
			_allCodecTaskIDCount++;
		} else {
			LOG_WARN << "Another bind type: " << bindType;
		}
	}

	sort(_vWindowPriority->begin(), _vWindowPriority->end());
	for (VctrWindowPriority::iterator it = _vWindowPriority->begin();
			it != _vWindowPriority->end(); it++)
		LOG_INFO << "_vWindowPriority: " << *it;

	for (VctrAVDECGetInfo::iterator it = avDecInfo->begin();
			it != avDecInfo->end(); it++) {
		if (it->stVdec.bSwms != DP_TRUE) {
			it->stVdec.stSwms.u32SwmsChn = *vSwmsChn.begin();
			vSwmsChn.erase(vSwmsChn.begin());
		}
		LOG_INFO << "after given swms chn " << it->stVdec.stSwms.u32SwmsChn;
	}

	//jhbnote if used swms is true
	_mOutCodecTaskIDBeUsed->insert(
			MapOutCodecTaskIDBeUsed::value_type(_vAudioTaskID, 0));
	_mOutCodecTaskIDBeUsed->insert(
			MapOutCodecTaskIDBeUsed::value_type(_vVideoTaskID, 0));
	_mOutCodecTaskIDBeUsed->insert(
			MapOutCodecTaskIDBeUsed::value_type(_vAuViTaskID, 0));
	LOG_INFO << "_allCodecTaskIDCount num : " << _allCodecTaskIDCount
			<< " _mSwmsChCodecDecInfo size: " << _mSwmsChCodecDecInfo->size()
			<< " _vVideoTaskID: " << _vVideoTaskID.size()
			<< " _mOutCodecTaskIDBeUsed size: "
			<< _mOutCodecTaskIDBeUsed->size()
			<< " _mOutCodecTaskIDBeUsed[video]: "
			<< _mOutCodecTaskIDBeUsed->operator [](_vVideoTaskID);
}

DP_S32 NodeInfo::recvCB(void* pData, int len) {
	if (pData == NULL && len <= 0) {
		return DP_ERR_PROTOCOL_PRASE;
	}
	DP_M2S_CMD_ACK_S *ack = (DP_M2S_CMD_ACK_S*) pData;
	if (ack->stHeader.u16HeadTag == DP_M2S_INF_PROT_PKT_HEAD) {
		if (ack->stHeader.u16PacketLen == len) {
			if (ack->stHeader.u8CommandID != DP_M2S_CMD_BUTT) {
				if (ack->u32Success == 0) {
					return DP_SUCCESS;
				} else if (ack->u32Success
						== DP_ERR_COMMUNICATE_ABNORMAL_TIMEOUT) {
					LOG_ERROR << "Wait from codec timeout: " << ack->u32Success;
				} else {
					LOG_ERROR << "ACK from codec: " << ack->u32Success;
					return ack->u32Success;
				}
			} else {
				return DP_ERR_PROTOCOL_PARAM_CMD;
			}
		} else {
			return DP_ERR_PROTOCOL_TOTALLEN;
		}
	} else {
		return DP_ERR_PROTOCOL_PRASE_HEAD;
	}
	return DP_ERR_PROTOCOL_PRASE;
}

DP_U32 NodeInfo::batchSetting(DP_M2S_CMD_ID_E cmd, VecCodecTaskID &vTaskID,
		VecErrInfo &errInfo) {
	DP_U32 count = vTaskID.size();
	if (count == 0) {
		return 0;
	}
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_U32) * (count + 1);
	boost::shared_ptr<DP_M2S_CMD_BATCH_COMMON_S> setAVInfo(
			new DP_M2S_CMD_BATCH_COMMON_S(packageLen, cmd, count));
	muduo::net::Buffer buffSend;
	DP_S32 taskID;
	buffSend.append(&setAVInfo, sizeof(DP_M2S_CMD_BATCH_COMMON_S));
	for (VecCodecTaskID::iterator it = vTaskID.begin(); it != vTaskID.end();
			it++) {
		taskID = *it;
		buffSend.append(&taskID, sizeof(taskID));
	}
	DP_S32 retResult = 0;
	DP_U8 *recvBuff = NodeInfo::sendToCodecAndRecv(retResult,
			buffSend.toStringPiece().data(), packageLen);
	if (retResult == 0) {
		LOG_DEBUG << "Batch setting ok, cmd-- " << cmd;
		return retResult;
	} else {
		LOG_ERROR << "Set info to codec failed : " << retResult;
		DP_M2S_CMD_BATCH_ACK_S *ack = (DP_M2S_CMD_BATCH_ACK_S*) recvBuff;
		DP_U32 numsRecv = ack->u32Nums;
		if (numsRecv != count) {
			LOG_ERROR << "numsRecv!=count! numsRecv: " << numsRecv << " count: "
					<< count;
			return DP_ERR_PROTOCOL_CONTENT;
		} else {
			recvBuff += sizeof(DP_M2S_CMD_BATCH_ACK_S);
			DP_U32 *errCode = (DP_U32*) recvBuff;
			for (DP_U32 i = 0; i < count; i++) {
				errInfo.push_back(errCode[i]);
			}
		}
	}
	return retResult;
}

//
//		[0,256)，为音频编码任务ID，选中此ID范围时，仅可操作音频编码的相关属性；
// *	[256,512)，为音频解码任务ID，选中此ID范围时，仅可操作音频解码的相关属性；
// *	[512,1024)，为视频编码任务ID，选中此ID范围时，仅可操作视频编码的相关属性；
// *	[1024,1280)，为视频解码任务ID，选中此ID范围时，仅可操作视频解码的相关属性；
// *	[1280,1536)，为音视频编码任务ID，选中此ID范围时，仅可操作音视频编码的相关属性；
// *	[1536,1792)，为音视频解码任务ID，选中此ID范围时，仅可操作音视频解码的相关属性；
// *	其他预留，无效；
void NodeInfo::initAVDec(DP_M2S_AVDEC_INFO_S *avdec, DP_S32 taskID,
		DP_U32 chnID) {
	DP_M2S_AVBIND_TYPE_E bindType;
	if (taskID >= 0 && taskID < 256)
		bindType = DP_M2S_AVBIND_AI2AENC;
	else if (taskID >= 256 && taskID < 512)
		bindType = DP_M2S_AVBIND_ADEC2AO;
	else if (taskID >= 512 && taskID < 1024)
		bindType = DP_M2S_AVBIND_VI2VENC;
	else if (taskID >= 1024 && taskID < 1280)
		bindType = DP_M2S_AVBIND_VDEC2VO;
	else if (taskID >= 1280 && taskID < 1536)
		bindType = DP_M2S_AVBIND_AI2AENC_VI2VENC;
	else if (taskID >= 1536 && taskID < 1792)
		bindType = DP_M2S_AVBIND_ADEC2AO_VDEC2VO;
	else
		return;

	memset(avdec, 0, sizeof(DP_M2S_AVDEC_INFO_S));

	avdec->s32TskId = taskID;

	avdec->AvBindAttr.enBindType = bindType;
	avdec->AvBindAttr.stAudio.stIn.ModId = DP_M2S_MOD_ADEC;
	avdec->AvBindAttr.stAudio.stIn.u32ChnId = chnID;
	avdec->AvBindAttr.stAudio.stIn.u32DevId = 0;
	avdec->AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AO;
	avdec->AvBindAttr.stAudio.stOut.u32ChnId = 0;
#if (InputDevice)
	avdec->AvBindAttr.stAudio.stOut.u32DevId = DP_M2S_AO_DEV_LINEOUT0_HI3536;
#elif (OutputDevice)
	avdec->AvBindAttr.stAudio.stOut.u32DevId = DP_M2S_AO_DEV_HDMI0_HI3536;
#endif
	avdec->AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VDEC;
	avdec->AvBindAttr.stVideo.stIn.u32ChnId = chnID;
	avdec->AvBindAttr.stVideo.stIn.u32DevId = 0;
	avdec->AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VO;
	avdec->AvBindAttr.stVideo.stOut.u32ChnId = 0;
#if (InputDevice)
//	avdec->AvBindAttr.stVideo.stOut.u32DevId = DP_M2S_VO_DEV_HDMI0_HI3536;
#elif (OutputDevice)
	avdec->AvBindAttr.stVideo.stOut.u32DevId = DP_M2S_VO_DEV_HDMI0_HI3536;
#endif

	avdec->stStream._rtsp.stRtspClient.bMulticast = DP_FALSE;
	avdec->stStream._rtsp.stRtspClient.s32ConnTimeout = 0;
	avdec->stStream._rtsp.stRtspClient.bUDP = DP_FALSE;
	avdec->stStream._rtsp.stRtspClient.s8Open = DP_FALSE;
	avdec->stStream._rtsp.stRtspServer.bMulticast = DP_FALSE;
	avdec->stStream._rtsp.stRtspServer.bOpen = DP_FALSE;
	avdec->stStream._rtsp.stRtspServer.s32ConnMax = 0;
	avdec->stStream._rtsp.stRtspServer.s32ConnNums = 0;
	avdec->stStream._rtsp.stRtspServer.s32ConnTimeout = 0;
	avdec->stStream._rtsp.stRtspServer.bUDP = DP_FALSE;

	avdec->stAdec.enAlg = DP_M2S_ALG_AAC_DEC;
}
int NodeInfo::cmd_set_aenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn) //0 0 0 0 0
		{
	DP_M2S_AVENC_INFO_S *pstAttr = (DP_M2S_AVENC_INFO_S *) pPtr;
	DP_M2S_AVENC_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_INFO_S));

	/* s1 TskId*/
	stAttr.s32TskId = s32TskId; //0~255

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.stAvBind.enBindType = DP_M2S_AVBIND_AI2AENC;/* no video */

	stAttr.stAvBind.stAudio.stIn.ModId = DP_M2S_MOD_AI;
	stAttr.stAvBind.stAudio.stIn.u32DevId = 0;
	stAttr.stAvBind.stAudio.stIn.u32ChnId = 0; //not use

	stAttr.stAvBind.stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	stAttr.stAvBind.stAudio.stOut.u32DevId = 0; //not use
	stAttr.stAvBind.stAudio.stOut.u32ChnId = AencChn;

	//stAttr.AvBindAttr.stVideo = NULL;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */
	stAttr.stAenc.stAlg.enAlg = DP_M2S_ALG_AAC_ENC;
	stAttr.stAenc.stAlg.stAACEnc.u32Bitrate = 48000;
	stAttr.stAenc.stAlg.stAACEnc.bAdts = DP_FALSE;

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
	stRtspServer.bUDP = DP_FALSE; //udp
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0; //not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_INFO_S));
	return 0;
}

int NodeInfo::cmd_set_venc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 VencChn)
//512 1920 1080 4000 0
		{												//513 1920 1080 4000 1
	DP_M2S_AVENC_INFO_S *pstAttr = (DP_M2S_AVENC_INFO_S *) pPtr;
	DP_M2S_AVENC_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_INFO_S));

	/* s1 TskId*/
	stAttr.s32TskId = s32TskId;										//512~1023

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.stAvBind.enBindType = DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;

	stAttr.stAvBind.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.stAvBind.stVideo.stIn.u32DevId = 0;
	stAttr.stAvBind.stVideo.stIn.u32ChnId = 0;						//not use

	stAttr.stAvBind.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.stAvBind.stVideo.stOut.u32DevId = 0;						//not use
	stAttr.stAvBind.stVideo.stOut.u32ChnId = VencChn;

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
	stRtspServer.bUDP = DP_FALSE;										//udp
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;									//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_INFO_S));
	return 0;
}

int NodeInfo::cmd_set_avenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn,
		DP_S32 VencChn)    //1280 1920 1080 4000 1 2
		{											//1281 1920 1080 4000 2 3
	DP_M2S_AVENC_INFO_S *pstAttr = (DP_M2S_AVENC_INFO_S *) pPtr;
	DP_M2S_AVENC_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_INFO_S));

	/* s1 TskId*/
	stAttr.s32TskId = s32TskId;										//1280~1535

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.stAvBind.enBindType = DP_M2S_AVBIND_AI2AENC_VI2VENC;/* AV */

	stAttr.stAvBind.stAudio.stIn.ModId = DP_M2S_MOD_AI;
	stAttr.stAvBind.stAudio.stIn.u32DevId = 0;
	stAttr.stAvBind.stAudio.stIn.u32ChnId = 0;						//not use

	stAttr.stAvBind.stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	stAttr.stAvBind.stAudio.stOut.u32DevId = 0;						//not use
	stAttr.stAvBind.stAudio.stOut.u32ChnId = AencChn;

	stAttr.stAvBind.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.stAvBind.stVideo.stIn.u32DevId = 0;
	stAttr.stAvBind.stVideo.stIn.u32ChnId = 0;						//not use

	stAttr.stAvBind.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.stAvBind.stVideo.stOut.u32DevId = 0;						//not use
	stAttr.stAvBind.stVideo.stOut.u32ChnId = VencChn;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */
	stAttr.stAenc.stAlg.enAlg = DP_M2S_ALG_AAC_ENC;
	stAttr.stAenc.stAlg.stAACEnc.u32Bitrate = 48000;
	stAttr.stAenc.stAlg.stAACEnc.bAdts = DP_FALSE;

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_TRUE;

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
	stOsd.enType = DP_M2S_OSD_STRING;
	memcpy(stOsd.stStr.au8Str, "Hello �yԇ`1234567890~!@", sizeof(char) * 32);
	stOsd.stStr.u32Color = 0xEEEE00;
	stOsd.enDispMode = DP_M2S_OSD_DISPLAY_MODE_LTOP;
	stOsd.stPoint.s32X = 0;
	stOsd.stPoint.s32Y = 0;
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
	stRtspServer.bUDP = DP_FALSE;										//udp
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;									//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_INFO_S));
	return 0;
}

void NodeInfo::printAVENC(void *args) {
	DP_M2S_AVENC_INFO_S *avenc = (DP_M2S_AVENC_INFO_S *) args;
	LOG_DEBUG << "avenc=========================================:";
	LOG_DEBUG << "avenc->stAvBind.enBindType " << avenc->stAvBind.enBindType;
	LOG_DEBUG << "avenc->stAvBind.stAudio.stIn.ModId "
			<< avenc->stAvBind.stAudio.stIn.ModId;
	LOG_DEBUG << "avenc->stAvBind.stAudio.stIn.u32ChnId "
			<< avenc->stAvBind.stAudio.stIn.u32ChnId;
	LOG_DEBUG << "avenc->stAvBind.stAudio.stIn.u32DevId "
			<< avenc->stAvBind.stAudio.stIn.u32DevId;
	LOG_DEBUG << "avenc->stAvBind.stAudio.stOut.ModId "
			<< avenc->stAvBind.stAudio.stOut.ModId;
	LOG_DEBUG << "avenc->stAvBind.stAudio.stOut.u32ChnId "
			<< avenc->stAvBind.stAudio.stOut.u32ChnId;
	LOG_DEBUG << "avenc->stAvBind.stAudio.stOut.u32DevId "
			<< avenc->stAvBind.stAudio.stOut.u32DevId;
	LOG_DEBUG << "avenc->stAvBind.stVideo.stIn.ModId "
			<< avenc->stAvBind.stVideo.stIn.ModId;
	LOG_DEBUG << "avenc->stAvBind.stVideo.stIn.u32ChnId "
			<< avenc->stAvBind.stVideo.stIn.u32ChnId;
	LOG_DEBUG << "avenc->stAvBind.stVideo.stIn.u32DevId "
			<< avenc->stAvBind.stVideo.stIn.u32DevId;
	LOG_DEBUG << "avenc->stAvBind.stVideo.stOut.ModId "
			<< avenc->stAvBind.stVideo.stOut.ModId;
	LOG_DEBUG << "avenc->stAvBind.stVideo.stOut.u32ChnId "
			<< avenc->stAvBind.stVideo.stOut.u32ChnId;
	LOG_DEBUG << "avenc->stAvBind.stVideo.stOut.u32DevId "
			<< avenc->stAvBind.stVideo.stOut.u32DevId;
	LOG_DEBUG << "avenc->s32TskId " << avenc->s32TskId;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspClient.au8Url "
			<< avenc->stStream._rtsp.stRtspClient.au8Url;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspClient.bMulticast "
			<< avenc->stStream._rtsp.stRtspClient.bMulticast;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspClient.s32ConnTimeout "
			<< avenc->stStream._rtsp.stRtspClient.s32ConnTimeout;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspClient.bUDP "
			<< avenc->stStream._rtsp.stRtspClient.bUDP;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspClient.s8Open "
			<< avenc->stStream._rtsp.stRtspClient.s8Open;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.au8Url "
			<< avenc->stStream._rtsp.stRtspServer.au8Url;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.bMulticast "
			<< avenc->stStream._rtsp.stRtspServer.bMulticast;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.bOpen "
			<< avenc->stStream._rtsp.stRtspServer.bOpen;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.s32ConnMax "
			<< avenc->stStream._rtsp.stRtspServer.s32ConnMax;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.s32ConnNums "
			<< avenc->stStream._rtsp.stRtspServer.s32ConnNums;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.s32ConnTimeout "
			<< avenc->stStream._rtsp.stRtspServer.s32ConnTimeout;
	LOG_DEBUG << "avenc->stStream._rtsp.stRtspServer.bUDP "
			<< avenc->stStream._rtsp.stRtspServer.bUDP;

	LOG_DEBUG << "avenc->stAenc.stAlg.enAlg " << avenc->stAenc.stAlg.enAlg;
	LOG_DEBUG << "avenc->stVenc.bCrop " << avenc->stVenc.bCrop;
	LOG_DEBUG << "avenc->stVenc.bOsd " << avenc->stVenc.bOsd;
	LOG_DEBUG << "avenc->stVenc.bZoom " << avenc->stVenc.bZoom;
	LOG_DEBUG << "avenc->stVenc.stAlg.enAlg " << avenc->stVenc.stAlg.enAlg;
	LOG_DEBUG << "avenc->stVenc.stCrop.s32X " << avenc->stVenc.stCrop.s32X;
	LOG_DEBUG << "avenc->stVenc.stCrop.s32Y " << avenc->stVenc.stCrop.s32Y;
	LOG_DEBUG << "avenc->stVenc.stCrop.u32Height "
			<< avenc->stVenc.stCrop.u32Height;
	LOG_DEBUG << "avenc->stVenc.stCrop.u32Width "
			<< avenc->stVenc.stCrop.u32Width;
	LOG_DEBUG << "avenc->stVenc.stOsd.au8PicPath "
			<< avenc->stVenc.stOsd.au8PicPath;
	LOG_DEBUG << "avenc->stVenc.stOsd.enDispMode "
			<< avenc->stVenc.stOsd.enDispMode;
	LOG_DEBUG << "avenc->stVenc.stOsd.enType " << avenc->stVenc.stOsd.enType;
	LOG_DEBUG << "avenc->stVenc.stOsd.stStr.au8Str "
			<< avenc->stVenc.stOsd.stStr.au8Str;
	LOG_DEBUG << "avenc->stVenc.stOsd.stStr.u32Color "
			<< avenc->stVenc.stOsd.stStr.u32Color;
	LOG_DEBUG << "avenc->stVenc.stOsd.stPoint.s32X "
			<< avenc->stVenc.stOsd.stPoint.s32X;
	LOG_DEBUG << "avenc->stVenc.stOsd.stPoint.s32Y "
			<< avenc->stVenc.stOsd.stPoint.s32Y;
	LOG_DEBUG << "avenc->stVenc.stZoom.enType " << avenc->stVenc.stZoom.enType;
	LOG_DEBUG << "avenc->stVenc.stZoom.stRatio.u32HRatio "
			<< avenc->stVenc.stZoom.stRatio.u32HRatio;
	LOG_DEBUG << "avenc->stVenc.stZoom.stRatio.u32WRatio "
			<< avenc->stVenc.stZoom.stRatio.u32WRatio;
	LOG_DEBUG << "avenc->stVenc.stZoom.stRatio.u32XRatio "
			<< avenc->stVenc.stZoom.stRatio.u32XRatio;
	LOG_DEBUG << "avenc->stVenc.stZoom.stRatio.u32YRatio "
			<< avenc->stVenc.stZoom.stRatio.u32YRatio;
	LOG_DEBUG << "avenc->stVenc.stZoom.stRect.s32X "
			<< avenc->stVenc.stZoom.stRect.s32X;
	LOG_DEBUG << "avenc->stVenc.stZoom.stRect.s32Y "
			<< avenc->stVenc.stZoom.stRect.s32Y;

}

void NodeInfo::printAVDEC(void *args) {
	DP_M2S_AVDEC_INFO_S *avdec = (DP_M2S_AVDEC_INFO_S*) args;
	LOG_DEBUG << "avdec===================================:";
	LOG_DEBUG << "avdec->AvBindAttr.enBindType "
			<< avdec->AvBindAttr.enBindType;
	LOG_DEBUG << "avdec->AvBindAttr.stAudio.stIn.ModId "
			<< avdec->AvBindAttr.stAudio.stIn.ModId;
	LOG_DEBUG << "avdec->AvBindAttr.stAudio.stIn.u32ChnId "
			<< avdec->AvBindAttr.stAudio.stIn.u32ChnId;
	LOG_DEBUG << "avdec->AvBindAttr.stAudio.stIn.u32DevId "
			<< avdec->AvBindAttr.stAudio.stIn.u32DevId;
	LOG_DEBUG << "avdec->AvBindAttr.stAudio.stOut.ModId "
			<< avdec->AvBindAttr.stAudio.stOut.ModId;
	LOG_DEBUG << "avdec->AvBindAttr.stAudio.stOut.u32ChnId "
			<< avdec->AvBindAttr.stAudio.stOut.u32ChnId;
	LOG_DEBUG << "avdec->AvBindAttr.stAudio.stOut.u32DevId "
			<< avdec->AvBindAttr.stAudio.stOut.u32DevId;
	LOG_DEBUG << "avdec->AvBindAttr.stVideo.stIn.ModId "
			<< avdec->AvBindAttr.stVideo.stIn.ModId;
	LOG_DEBUG << "avdec->AvBindAttr.stVideo.stIn.u32ChnId "
			<< avdec->AvBindAttr.stVideo.stIn.u32ChnId;
	LOG_DEBUG << "avdec->AvBindAttr.stVideo.stIn.u32DevId "
			<< avdec->AvBindAttr.stVideo.stIn.u32DevId;
	LOG_DEBUG << "avdec->AvBindAttr.stVideo.stOut.ModId "
			<< avdec->AvBindAttr.stVideo.stOut.ModId;
	LOG_DEBUG << "avdec->AvBindAttr.stVideo.stOut.u32ChnId "
			<< avdec->AvBindAttr.stVideo.stOut.u32ChnId;
	LOG_DEBUG << "avdec->AvBindAttr.stVideo.stOut.u32DevId "
			<< avdec->AvBindAttr.stVideo.stOut.u32DevId;
	LOG_DEBUG << "avdec->s32TskId " << avdec->s32TskId;
	LOG_DEBUG << "avdec->stAdec.enAlg " << avdec->stAdec.enAlg;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspClient.au8Url "
			<< avdec->stStream._rtsp.stRtspClient.au8Url;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspClient.bMulticast "
			<< avdec->stStream._rtsp.stRtspClient.bMulticast;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspClient.s32ConnTimeout "
			<< avdec->stStream._rtsp.stRtspClient.s32ConnTimeout;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspClient.bUDP "
			<< avdec->stStream._rtsp.stRtspClient.bUDP;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspClient.s8Open "
			<< avdec->stStream._rtsp.stRtspClient.s8Open;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.au8Url "
			<< avdec->stStream._rtsp.stRtspServer.au8Url;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.bMulticast "
			<< avdec->stStream._rtsp.stRtspServer.bMulticast;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.bOpen "
			<< avdec->stStream._rtsp.stRtspServer.bOpen;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.s32ConnMax "
			<< avdec->stStream._rtsp.stRtspServer.s32ConnMax;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.s32ConnNums "
			<< avdec->stStream._rtsp.stRtspServer.s32ConnNums;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.s32ConnTimeout "
			<< avdec->stStream._rtsp.stRtspServer.s32ConnTimeout;
	LOG_DEBUG << "avdec->stStream._rtsp.stRtspServer.bUDP "
			<< avdec->stStream._rtsp.stRtspServer.bUDP;
	LOG_DEBUG << "avdec->stVdec.bCrop " << avdec->stVdec.bCrop;
	LOG_DEBUG << "avdec->stVdec.bOsd " << avdec->stVdec.bOsd;
	LOG_DEBUG << "avdec->stVdec.bSwms " << avdec->stVdec.bSwms;
	LOG_DEBUG << "avdec->stVdec.bZoom " << avdec->stVdec.bZoom;
	LOG_DEBUG << "avdec->stVdec.stAlg.enAlg " << avdec->stVdec.stAlg.enAlg;
	LOG_DEBUG << "avdec->stVdec.stCrop.s32X " << avdec->stVdec.stCrop.s32X;
	LOG_DEBUG << "avdec->stVdec.stCrop.s32Y " << avdec->stVdec.stCrop.s32Y;
	LOG_DEBUG << "avdec->stVdec.stCrop.u32Height "
			<< avdec->stVdec.stCrop.u32Height;
	LOG_DEBUG << "avdec->stVdec.stCrop.u32Width "
			<< avdec->stVdec.stCrop.u32Width;
	LOG_DEBUG << "avdec->stVdec.stOsd.au8PicPath "
			<< avdec->stVdec.stOsd.au8PicPath;
	LOG_DEBUG << "avdec->stVdec.stOsd.enDispMode "
			<< avdec->stVdec.stOsd.enDispMode;
	LOG_DEBUG << "avdec->stVdec.stOsd.enType " << avdec->stVdec.stOsd.enType;
	LOG_DEBUG << "avdec->stVdec.stOsd.stStr.au8Str "
			<< avdec->stVdec.stOsd.stStr.au8Str;
	LOG_DEBUG << "avdec->stVdec.stOsd.stStr.u32Color "
			<< avdec->stVdec.stOsd.stStr.u32Color;
	LOG_DEBUG << "avdec->stVdec.stOsd.stPoint.s32X "
			<< avdec->stVdec.stOsd.stPoint.s32X;
	LOG_DEBUG << "avdec->stVdec.stOsd.stPoint.s32Y "
			<< avdec->stVdec.stOsd.stPoint.s32Y;
	LOG_DEBUG << "avdec->stVdec.stSwms.u32SwmsChn "
			<< avdec->stVdec.stSwms.u32SwmsChn;
	LOG_DEBUG << "avdec->stVdec.stSwms.enVoDevId "
			<< avdec->stVdec.stSwms.enVoDevId;
	LOG_DEBUG << "avdec->stVdec.stSwms.stRect.s32X "
			<< avdec->stVdec.stSwms.stRect.s32X;
	LOG_DEBUG << "avdec->stVdec.stSwms.stRect.s32Y "
			<< avdec->stVdec.stSwms.stRect.s32Y;
	LOG_DEBUG << "avdec->stVdec.stSwms.stRect.u32Height "
			<< avdec->stVdec.stSwms.stRect.u32Height;
	LOG_DEBUG << "avdec->stVdec.stSwms.stRect.u32Width "
			<< avdec->stVdec.stSwms.stRect.u32Width;
	LOG_DEBUG << "avdec->stVdec.stSwms.u32Priority "
			<< avdec->stVdec.stSwms.u32Priority;
	LOG_DEBUG << "avdec->stVdec.stZoom.enType " << avdec->stVdec.stZoom.enType;
	LOG_DEBUG << "avdec->stVdec.stZoom.stRatio.u32HRatio "
			<< avdec->stVdec.stZoom.stRatio.u32HRatio;
	LOG_DEBUG << "avdec->stVdec.stZoom.stRatio.u32WRatio "
			<< avdec->stVdec.stZoom.stRatio.u32WRatio;
	LOG_DEBUG << "avdec->stVdec.stZoom.stRatio.u32XRatio "
			<< avdec->stVdec.stZoom.stRatio.u32XRatio;
	LOG_DEBUG << "avdec->stVdec.stZoom.stRatio.u32YRatio "
			<< avdec->stVdec.stZoom.stRatio.u32YRatio;
	LOG_DEBUG << "avdec->stVdec.stZoom.stRect.s32X "
			<< avdec->stVdec.stZoom.stRect.s32X;
	LOG_DEBUG << "avdec->stVdec.stZoom.stRect.s32Y "
			<< avdec->stVdec.stZoom.stRect.s32Y;
}
