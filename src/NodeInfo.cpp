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

SetNetwork NodeInfo::_netInfo;

NodeInfo::NodeInfo() :
		_vAIGetInfo(new VctrAIGetInfo), _vVIGetInfo(new VctrVIGetInfo), _vAVEncGetInfo(
				new VctrAVENCGetInfo), _vAVDecGetInfo(new VctrAVDECGetInfo), _vAOGetInfo(
				new VctrAOGetInfo), _vVOGetInfo(new VctrVOGetInfo), _allCodecTaskIDCount(
				0), _mOutCodecTaskIDBeUsed(new MapOutCodecTaskIDBeUsed), _mOutThirdCodecTaskID(
				new MapOutThirdCodecTaskID), _mSwmsChCodecDecInfo(
				new MapOutSWMSChCodecDecInfo), _mThirdIDSrcVideoInfo(
				new MapThirdIDSrcVideoInfo), _mAODevIDCodecID(
				new MapAODevIDCodecID), _mAudioTaskID(new MapServerTaskID), _mVideoTaskID(
				new MapServerTaskID), _mAuViTaskID(new MapServerTaskID), _vWindowPriority(
				new VctrWindowPriority) {
	_netInfo.setIfname(IFNAMEDEV);
	_netInfo.getNetworkConfig();
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
	if (initCodec() != DP_TRUE) {
		LOG_ERROR << "Init failed !";
		return;
	}
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
	//cmd_set_aenc_default（）//0 0 0 0 0
	//cmd_set_venc_default（）//256 1920 1080 4000 0
	//257 1920 1080 4000 1
	//cmd_set_avenc_default（）//512 1920 1080 4000 1 2
	//513 1920 1080 4000 2 3
	DP_M2S_AVENC_GET_INFO_S avEncInfo;
	cmd_set_aenc_default(&avEncInfo, 0, 0, 0, 0, 0);
//	cmd_set_avenc_default_512(&avEncInfo);
//	AVEncInputNodeInfo->push_back(avEncInfo);
//	memset(&avEncInfo, 0, sizeof(DP_M2S_AVENC_GET_INFO_S));
//	cmd_set_avenc_default_513(&avEncInfo);
	AVEncInputNodeInfo->push_back(avEncInfo);
	memset(&avEncInfo, 0, sizeof(DP_M2S_AVENC_GET_INFO_S));
	cmd_set_avenc_default(&avEncInfo, 512, 1920, 1080, 4000, 1, 1);
	AVEncInputNodeInfo->push_back(avEncInfo);
	memset(&avEncInfo, 0, sizeof(DP_M2S_AVENC_GET_INFO_S));
	cmd_set_avenc_default(&avEncInfo, 513, 1920, 1080, 4000, 2, 2);
	AVEncInputNodeInfo->push_back(avEncInfo);

	LOG_INFO << "Setting avenc ------------------";
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
	//// 说明： 获取音频输出信息
	VctrAOGetInfoPtr AOInfo = getAOGetInfo();
	getAVInfoFromCodecInfo<VctrAOGetInfoPtr, DP_M2S_AO_GET_INFO_S>(AOInfo,
			DP_M2S_INFO_TYPE_GET_AO,
			DP_AO_DEV_MAX);
	updateAOGetInfo(AOInfo);
#endif

	///	//
	//	//		[0,256)，为音频编码任务ID，选中此ID范围时，仅可操作音频编码的相关属性；
	//	// *	[256,512)，为音频解码任务ID，选中此ID范围时，仅可操作音频解码的相关属性；
	//	// *	[512,1024)，为视频编码任务ID，选中此ID范围时，仅可操作视频编码的相关属性；
	//	// *	[1024,1280)，为视频解码任务ID，选中此ID范围时，仅可操作视频解码的相关属性；
	//	// *	[1280,1536)，为音视频编码任务ID，选中此ID范围时，仅可操作音视频编码的相关属性；
	//	// *	[1536,1792)，为音视频解码任务ID，选中此ID范围时，仅可操作音视频解码的相关属性；
	//	// *	其他预留，无效；

	//#if (InputDevice)
	//init input node
	//#endif
	//
	//#if (OutputDevice)
	//init output node

	initOutAVEnc();
	initOutAVDec();
	// 获取视频输出信息
	VecVODEV voDev;
	voDev.push_back(DP_M2S_VO_DEV_HDMI0_HI3536);
	VctrVOGetInfoPtr VOInfo = getVOGetInfo();
	getAOVOInfoFromCodec<VctrVOGetInfoPtr, DP_M2S_CMD_VO_GETINFO_S, VecVODEV,
			DP_M2S_CMD_VO_GETINFO_ACK_S>(VOInfo, DP_M2S_CMD_VO_GET, voDev);

	updateVOGetInfo(VOInfo);
//	getAVInfoFromCodecInfo<VctrVOGetInfoPtr, DP_M2S_VO_GET_INFO_S>(VOInfo,
//			DP_M2S_INFO_TYPE_GET_VO, DP_VO_DEV_MAX);
//	LOG_INFO << "VOInfo size:::: === " << VOInfo->size();
//	for_each(VOInfo->begin(), VOInfo->end(), print_DP_M2S_VO_GET_INFO_S_);
//	updateVOGetInfo(VOInfo);
//
////	LOG_INFO << "########################  6  ####################";
//	//// 说明： 获取音频输出信息
//	VctrAOGetInfoPtr AOInfo = getAOGetInfo();
//	getAVInfoFromCodecInfo<VctrAOGetInfoPtr, DP_M2S_AO_GET_INFO_S>(AOInfo,
//			DP_M2S_INFO_TYPE_GET_AO, DP_AO_DEV_MAX);
//	updateAOGetInfo(AOInfo);

}

DP_BOOL NodeInfo::initOutAVEnc() {
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
	getAVInfoFromCodec<VctrAVENCGetInfoPtr, DP_M2S_CMD_AVENC_SETINFO_S>(vTaskID,
			DP_M2S_CMD_AVENC_GET);

	return DP_TRUE;
}

DP_BOOL NodeInfo::initOutAVDec() {
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
	} else
		setOutputTaskIDInMap(AVDecInfo);

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

	getAVInfoFromCodec<VctrAVDECGetInfoPtr, DP_M2S_CMD_AVDEC_SETINFO_S>(codecID,
			DP_M2S_CMD_AVDEC_GET);

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
		LOG_INFO << "getNewCodecTaskID _vVideoTaskID: " << _vVideoTaskID.size()
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
	return -2; //error!!!!!!
}
DP_S32 NodeInfo::findNewID(DP_U32 thirdId, VctrOutCodecTaskID TaskID) {
	LOG_INFO << "findNewID TaskID.size() : " << TaskID.size();
	LOG_INFO << " _mOutCodecTaskIDBeUsed->operator [](TaskID) "
			<< _mOutCodecTaskIDBeUsed->operator [](TaskID);

//check thirdId!
	if (_mOutThirdCodecTaskID->find(thirdId) != _mOutThirdCodecTaskID->end()) {
		LOG_ERROR << "The repetition third id ";
		return -3;
	}

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
void NodeInfo::removeCodecTaskID(DP_U32 thirdId) {
	LOG_INFO << "Remove third task id :" << thirdId;
	MapOutThirdCodecTaskIDPtr thirdCodecID = getOutThirdCodecTaskID();
	DP_S32 id = 0;
	TaskObjectType_E taskType;
	if (thirdCodecID->find(thirdId) != thirdCodecID->end())
		id = thirdCodecID->operator [](thirdId);
	if (id <= 255)
		taskType = _eAudioTask;
	else if (id >= 256 && id <= 511)
		taskType = _eVideoTask;
	else if (id >= 512)
		taskType = _eAudioAndVideoTask;
	LOG_INFO << "codec id in remove :" << id << " taskType: " << taskType;
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
	boost::shared_ptr<DP_M2S_AI_SET_INFO_S> aiInfo(
			new DP_M2S_AI_SET_INFO_S(DP_M2S_AI_DEV_LINEIN0_HI3536, 0,
					DP_M2S_AUDIO_SAMPLE_RATE_48000, DP_M2S_AUDIO_BIT_WIDTH_16,
					DP_M2S_AUDIO_SOUND_MODE_STEREO));

#if 0 // input
	boost::shared_ptr<DP_M2S_CMD_SYS_INIT_S> sysInit(
			new DP_M2S_CMD_SYS_INIT_S(aiInfo.get(), 1, NULL, 0));
#endif
#if 1 // output
	DP_M2S_RECT_S rect;
	DP_M2S_SWMS_ATTR_S swms(DP_M2S_VO_DEV_HDMI0_HI3536, DP_M2S_SWMS_MAX + 1, 0,
			rect);
	DP_M2S_CSC_ATTR_S csc;
	boost::shared_ptr<DP_M2S_VO_INFO_S> voInfo(
			new DP_M2S_VO_INFO_S(DP_M2S_VO_DEV_HDMI0_HI3536, DP_TRUE,
					DP_M2S_VIDEO_SYNC_1080P60, &swms, 1, csc));
	boost::shared_ptr<DP_M2S_CMD_SYS_INIT_S> sysInit(
			new DP_M2S_CMD_SYS_INIT_S(aiInfo.get(), 1, voInfo.get(), 1));
#endif
	try {
		DP_S32 retSend = client.doSendCommand(sysInit.get(),
				sizeof(DP_M2S_CMD_SYS_INIT_S));
		if (retSend == 0)
			return DP_TRUE;
		else {
			return DP_FALSE;
		}
	} catch (SystemException &ex) {
		LOG_ERROR << ex.what();
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
// jhbnote add len tag!!!!!!!
	DP_M2S_CMD_ACK_S *ack = (DP_M2S_CMD_ACK_S*) pData;
	if (ack->u32Success == 0) {
		return 0;
	} else {
		LOG_ERROR << "ACK from codec: " << ack->u32Success;
		return ack->u32Success;
	}
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
	avdec->AvBindAttr.stAudio.stOut.u32DevId = DP_M2S_AO_DEV_HDMI0_HI3536;

	avdec->AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VDEC;
	avdec->AvBindAttr.stVideo.stIn.u32ChnId = chnID;
	avdec->AvBindAttr.stVideo.stIn.u32DevId = 0;
	avdec->AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VO;
	avdec->AvBindAttr.stVideo.stOut.u32ChnId = 0;
	avdec->AvBindAttr.stVideo.stOut.u32DevId = DP_M2S_VO_DEV_HDMI0_HI3536;

	avdec->stStream._rtsp.stRtspClient.bMulticast = 0;
	avdec->stStream._rtsp.stRtspClient.s32ConnTimeout = 0;
	avdec->stStream._rtsp.stRtspClient.bUDP = DP_FALSE;
	avdec->stStream._rtsp.stRtspClient.s8Open = 0;
	avdec->stStream._rtsp.stRtspServer.bMulticast = 0;
	avdec->stStream._rtsp.stRtspServer.bOpen = 0;
	avdec->stStream._rtsp.stRtspServer.s32ConnMax = 0;
	avdec->stStream._rtsp.stRtspServer.s32ConnNums = 0;
	avdec->stStream._rtsp.stRtspServer.s32ConnTimeout = 0;
	avdec->stStream._rtsp.stRtspServer.bUDP = DP_FALSE;

	avdec->stAdec.enAlg = DP_M2S_ALG_AAC_DEC;
}

//
//cmd_set_aenc_default（）//0 0 0 0 0
//cmd_set_venc_default（）//256 1920 1080 4000 0
//257 1920 1080 4000 1
//cmd_set_avenc_default（）//512 1920 1080 4000 1 2
//513 1920 1080 4000 2 3

#if 0
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
	stAttr.AvBindAttr.stAudio.stOut.u32DevId = 0;//not use
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
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
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
	stAttr.AvBindAttr.stAudio.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stAudio.stOut.u32ChnId = AencChn;

	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
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
	stCrop.u32Width = 760;
	stCrop.u32Height = 576;
//	stCrop.u32Width = 3840; //4K
//	stCrop.u32Height = 2160;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = 760;
	stZoom.stRect.u32Height = 576;
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

void NodeInfo::printAVENC(DP_M2S_AVENC_INFO_S *avenc) {
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

void NodeInfo::printAVDEC(DP_M2S_AVDEC_INFO_S *avdec) {
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
