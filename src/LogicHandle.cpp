/*
 * LogicHandle.cpp
 *
 *  Created on: Oct 19, 2018
 *      Author: jhb
 */

#include <muduo/base/Singleton.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <string.h>
//
#include <functional>

#include "LogicHandle.h"
#include "interactivepro.h"
#include "MyFastTcpClient.h"
#include "DevStaticConfigure.h"
#include "UnixSockClientData.h"
#include "LocalInfo.h"
#include "PrintBuff.h"

LogicHandle::LogicHandle() :
		_mutex(), _cond(_mutex) {
	_netInfo.setIfname(IFNAMEDEV);
	_netInfo.getNetworkConfig();
}

LogicHandle::~LogicHandle() {
}

void LogicHandle::getInfo(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "Get info cmd...";
	_sRemote_GetInfo *getInfoData = (_sRemote_GetInfo*) data.c_str();

	switch (getInfoData->u32Proterty) {
	case Property_Get_InputVideoChnInfo: { //获取输入节点的视频采集通道信息

		NodeInfo::VctrVIGetInfoPtr viGetInfo =
				muduo::Singleton<NodeInfo>::instance().getVIGetInfo();
		DP_U32 propertyLen = sizeof(_sAllViChnInfo)
				+ viGetInfo->size() * sizeof(_sAllViChnInfo::_sSingleViChnInfo);
		DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName)
				+ sizeof(DP_U32) + sizeof(DP_U16) + propertyLen;
		DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;
		_sRemote_Reply_GetInfo replyGetInfo(_netInfo.ip2U32(), g_DevType, 0x01,
				packageLen, Command_GetInfo, cmdLen,
				muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
				Property_Get_InputVideoChnInfo, 0x00, propertyLen);
		DP_U8 viChCount = viGetInfo->size();
		_sAllViChnInfo allViChInfo(viChCount);
		muduo::net::Buffer buffSend;
		buffSend.append(&replyGetInfo, packageLen - propertyLen);
		buffSend.append(&allViChInfo, sizeof(_sAllAiChnInfo));
		for (NodeInfo::VctrVIGetInfo::iterator it = viGetInfo->begin();
				it != viGetInfo->end(); it++) {
			boost::shared_ptr<_sAllViChnInfo::_sSingleViChnInfo> singleViCh(
					new _sAllViChnInfo::_sSingleViChnInfo((DP_S8) it->bSignal,
							it->stCap.u32Width, it->stCap.u32Height,
							it->s32FrmRate));
			buffSend.append(singleViCh.get(),
					sizeof(_sAllViChnInfo::_sSingleViChnInfo));
		}
		connPtr->send(&buffSend);
	}
		break;
	case Property_Get_VideoChnVencInfo: { //获取输入节点的视频采集通道的流信息

		NodeInfo::VctrAVENCGetInfoPtr vAVEncInNodeInfo = muduo::Singleton<
				NodeInfo>::instance().getAVEncGetInfo();

		DP_U32 propertyLen = sizeof(_sAllVencChnInfo)
				+ vAVEncInNodeInfo->size()
						* sizeof(_sAllVencChnInfo::_sSingleVencChnInfo);
		DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName)
				+ sizeof(DP_U32) + sizeof(DP_U16) + propertyLen;
		DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;

		_sRemote_Reply_GetInfo replyGetInfo(_netInfo.ip2U32(), g_DevType, 0x01,
				packageLen, Command_GetInfo, cmdLen,
				muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
				Property_Get_VideoChnVencInfo, 0x00, propertyLen);

		DP_U8 avEncChCount = vAVEncInNodeInfo->size();
		_sAllVencChnInfo allAiChInfo(DP_VI_DEV_MAX, avEncChCount);

		muduo::net::Buffer buffSend;
		buffSend.append(&replyGetInfo, packageLen - propertyLen);
		buffSend.append(&allAiChInfo, sizeof(_sAllVencChnInfo));
		for (NodeInfo::VctrAVENCGetInfo::iterator it =
				vAVEncInNodeInfo->begin(); it != vAVEncInNodeInfo->end();
				it++) {

			boost::shared_ptr<_sAllVencChnInfo::_sSingleVencChnInfo> singleVencCh(
					new _sAllVencChnInfo::_sSingleVencChnInfo(
							it->AvBindAttr.stVideo.stIn.u32DevId == 0 ?
									ID_VI_CHN_VIDEOIN1 : ID_VI_CHN_VIDEOIN2,
							it->AvBindAttr.stVideo.stOut.u32ChnId, 0x0000,
							it->stVenc.stCrop.u32Width,
							it->stVenc.stCrop.u32Height, it->stVenc.stCrop.s32X,
							it->stVenc.stCrop.s32Y,
							it->stStream._rtsp.stRtspServer.au8Url,
							DP_M2S_URL_LEN, 0x00, _sSrcAudioInfo()));
			LOG_WARN << "check url 1: and len:  "
					<< it->stStream._rtsp.stRtspServer.au8Url << " "
					<< strlen((DP_CHAR*) it->stStream._rtsp.stRtspServer.au8Url)
					<< " Check url 2: and len : "
					<< singleVencCh->au8PreviewRtspURL << " "
					<< strlen((DP_CHAR*) singleVencCh->au8PreviewRtspURL);
			if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_VI2VENC) //视频输入绑到视频编码
					{
				buffSend.append(singleVencCh.get(),
						sizeof(_sAllVencChnInfo::_sSingleVencChnInfo));
			} else if (it->AvBindAttr.enBindType
					== DP_M2S_AVBIND_AI2AENC_VI2VENC) { //音频输入绑到音频编码，同时，视频输入绑到视频编码
				singleVencCh->u8AudioIn = 0x01;
				NodeInfo::VctrAIGetInfoPtr aiInfo =
						muduo::Singleton<NodeInfo>::instance().getAIGetInfo();
				DP_M2S_AI_GET_INFO_S aiGetInfo;
				for (NodeInfo::VctrAIGetInfo::iterator iter = aiInfo->begin();
						iter != aiInfo->end(); iter++) {
					if (iter->u32DevId
							== it->AvBindAttr.stAudio.stIn.u32DevId) {
						aiGetInfo = *iter;
						break;
					}
				}
				_sSrcAudioInfo srcAiInfo(aiGetInfo.stCommAttr.enSamplerate,
						aiGetInfo.stCommAttr.enBitwidth,
						aiGetInfo.stCommAttr.enSoundmode, AUDIO_ENCODE_AAC); //AAC default
				singleVencCh->srcAudioInfo = srcAiInfo;

				buffSend.append(singleVencCh.get(),
						sizeof(_sAllVencChnInfo::_sSingleVencChnInfo));
			}
		}
		connPtr->send(&buffSend);
//
	}

		break;
	case Property_Get_InputAudioChnInfo:
		//获取输入节点的音频输入的通道信息
	{
		NodeInfo::VctrAIGetInfoPtr aiGetInfo =
				muduo::Singleton<NodeInfo>::instance().getAIGetInfo();
		DP_U32 propertyLen = sizeof(_sAllAiChnInfo)
				+ aiGetInfo->size() * sizeof(_sAllAiChnInfo::_sSingleAiChnInfo);
		DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName)
				+ sizeof(DP_U32) + sizeof(DP_U16) + propertyLen;
		DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;

		_sRemote_Reply_GetInfo replyGetInfo(_netInfo.ip2U32(), g_DevType, 0x01,
				packageLen, Command_GetInfo, cmdLen,
				muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
				Property_Get_InputAudioChnInfo, 0x00, propertyLen);
		DP_U8 aiChCount = aiGetInfo->size();
		_sAllAiChnInfo allAiChInfo(aiChCount);
		muduo::net::Buffer buffSend;
		buffSend.append(&replyGetInfo, packageLen - propertyLen);

		buffSend.append(&allAiChInfo, sizeof(_sAllAiChnInfo));
		NodeInfo::VctrAVENCGetInfoPtr vAVEncInNodeInfo = muduo::Singleton<
				NodeInfo>::instance().getAVEncGetInfo();
		for (NodeInfo::VctrAIGetInfo::iterator it = aiGetInfo->begin();
				it != aiGetInfo->end(); it++) {
			DP_U32 devID = it->u32DevId;
			NodeInfo::VctrAVENCGetInfo::iterator it_cam = std::find_if(
					vAVEncInNodeInfo.get()->begin(),
					vAVEncInNodeInfo.get()->end(),
					bind2nd(compareDevID<DP_M2S_AVENC_GET_INFO_S>(), devID));
			if (it_cam != vAVEncInNodeInfo.get()->end()) {
				boost::shared_ptr<_sAllAiChnInfo::_sSingleAiChnInfo> singleAICh(
						new _sAllAiChnInfo::_sSingleAiChnInfo(it->bSignal,
								it_cam->stStream._rtsp.stRtspServer.au8Url,
								DP_M2S_URL_LEN, it->stCommAttr.enSamplerate,
								it->stCommAttr.enBitwidth,
								it->stCommAttr.enSoundmode, AUDIO_ENCODE_AAC));
				buffSend.append(singleAICh.get(),
						sizeof(_sAllAiChnInfo::_sSingleAiChnInfo));
			}
		}
		connPtr->send(&buffSend);
	}
		break;
	case Property_Get_OutputVideoChnInfo: /// real time
		//获取输出节点的显示视频通道信息
	{
		NodeInfo::VctrVOGetInfoPtr VOInfo =
				muduo::Singleton<NodeInfo>::instance().getVOGetInfo();
		NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsChInfo = muduo::Singleton<
				NodeInfo>::instance().getOutSWMSChCodecDecInfo();
		DP_U32 taskCount = swmsChInfo->size();
		DP_U32 propertyLen = sizeof(_sAllVoChnInfo)
				+ VOInfo->size() * sizeof(_sSingleVoChnInfo)
				+ taskCount * sizeof(_sVideoTaskInfo);

		DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName)
				+ sizeof(DP_U32) + sizeof(DP_U16) + propertyLen;

		DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;
		LOG_INFO << "VOInfo->size()  : " << VOInfo->size() << " taskCount　: "
				<< taskCount << " propertyLen : " << propertyLen << " cmdLen : "
				<< cmdLen << " packageLen: " << packageLen;

		_sRemote_Reply_GetInfo replyGetInfo(_netInfo.ip2U32(), g_DevType, 0x01,
				packageLen, Command_GetInfo, cmdLen,
				muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
				Property_Get_OutputVideoChnInfo, 0x00, propertyLen);

		_sAllVoChnInfo allVOChInfo(VOInfo->size(), taskCount);
		muduo::net::Buffer buffSend;
		buffSend.append(&replyGetInfo, packageLen - propertyLen);
		buffSend.append(&allVOChInfo, sizeof(_sAllVoChnInfo));
		NodeInfo::VctrAVENCGetInfoPtr AVEncInfo =
				muduo::Singleton<NodeInfo>::instance().getAVEncGetInfo();
		NodeInfo::VctrAVENCGetInfo::iterator iter_AVEnc;
		//append _sSingleVoChnInfo
		for (NodeInfo::VctrVOGetInfo::iterator it = VOInfo->begin();
				it != VOInfo->end(); it++) {
			iter_AVEnc = find_if(AVEncInfo->begin(), AVEncInfo->end(),
					bind2nd(findEchoRtspURL<DP_M2S_AVENC_GET_INFO_S>(),
							it->s32DevId));
			if (it->s32DevId == 0) {
				boost::shared_ptr<_sSingleVoChnInfo> singleVoInfo(
						new _sSingleVoChnInfo(it->bEnable, ID_VO_CHN_VIDEOOUT1,
						NULL, 0, 0, 0, 0)); /////////////////// null
				if (iter_AVEnc != AVEncInfo->end()) {
					memset(singleVoInfo->au8PreviewRtspURL, 0, DP_URL_LEN);
					strcpy((DP_CHAR*) singleVoInfo->au8PreviewRtspURL,
							(DP_CHAR*) iter_AVEnc->stStream._rtsp.stRtspServer.au8Url);
//					LOG_WARN << "check url 1 and len: "
//							<< iter_AVEnc->stStream._rtsp.stRtspServer.au8Url
//							<< " "
//							<< strlen(
//									(DP_CHAR*) iter_AVEnc->stStream._rtsp.stRtspServer.au8Url)
//							<< " Check url 2: and len :"
//							<< singleVoInfo->au8PreviewRtspURL << " "
//							<< strlen(
//									(DP_CHAR*) singleVoInfo->au8PreviewRtspURL);
				}
				buffSend.append(singleVoInfo.get(), sizeof(_sSingleVoChnInfo));
				LOG_INFO << "singleVoInfo attr: u8VoChnOpenStatus: "
						<< singleVoInfo->u8VoChnOpenStatus << " VOchiD : "
						<< singleVoInfo->u8VoChnID << " URL : "
						<< singleVoInfo->au8PreviewRtspURL << " ao chid : "
						<< singleVoInfo->u8RelateAoChnID << " ao volume : "
						<< singleVoInfo->u8AoChnVolume;
				printf("url : %s\n", singleVoInfo->au8PreviewRtspURL);
			} else if (it->s32DevId == 0x01) {
				boost::shared_ptr<_sSingleVoChnInfo> singleVoInfo(
						new _sSingleVoChnInfo(it->bEnable, ID_VO_CHN_VIDEOOUT2,
						NULL, 0, 0, 0, 0)); /////////////////// null
				if (iter_AVEnc != AVEncInfo->end()) {
					memset(singleVoInfo->au8PreviewRtspURL, 0, DP_URL_LEN);
					strcpy((DP_CHAR*) singleVoInfo->au8PreviewRtspURL,
							(DP_CHAR*) iter_AVEnc->stStream._rtsp.stRtspServer.au8Url);
//					LOG_WARN << "check url 1: and len:  "
//							<< iter_AVEnc->stStream._rtsp.stRtspServer.au8Url
//							<< " "
//							<< strlen(
//									(DP_CHAR*) iter_AVEnc->stStream._rtsp.stRtspServer.au8Url)
//							<< " Check url 2: and len : "
//							<< singleVoInfo->au8PreviewRtspURL
//							<< strlen(
//									(DP_CHAR*) singleVoInfo->au8PreviewRtspURL);
				}
				buffSend.append(singleVoInfo.get(), sizeof(_sSingleVoChnInfo));
			}
		}
		boost::shared_ptr<_sVideoTaskInfo> videoInfo(new _sVideoTaskInfo);

		NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID = muduo::Singleton<
				NodeInfo>::instance().getOutThirdCodecTaskID();

		DP_U32 codecTaskID = 0;
		NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;
		LOG_INFO << "swmsChInfo size :: " << swmsChInfo->size();
		if (!swmsChInfo->empty())
			for (NodeInfo::MapOutSWMSChCodecDecInfo::iterator it =
					swmsChInfo->begin(); it != swmsChInfo->end(); it++) {
				// without audio
				if (it->second.AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
					continue;
				codecTaskID = it->second.TskId;
				it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
						bind2nd(findThirdIDByCodecID(), codecTaskID));
				if (it_ID != thirdCodecID->end()) {
					videoInfo->u32TaskID = it_ID->first;
				}
				if (it->second.stVdec.bSwms) {
					videoInfo->u8VoChnID =
							it->second.stVdec.stSwms.s32VoDevId == 0x00 ?
									ID_VO_CHN_VIDEOOUT1 : ID_VO_CHN_VIDEOOUT2;
					videoInfo->u8WinZIndex =
							it->second.stVdec.stSwms.u32Priority;
//				videoInfo->u8VStreamStatus  // none
					videoInfo->dstVideoInfo.u16StartX =
							it->second.stVdec.stSwms.stRect.s32X;
					videoInfo->dstVideoInfo.u16StartY =
							it->second.stVdec.stSwms.stRect.s32Y;
					videoInfo->dstVideoInfo.u16VideoHeight =
							it->second.stVdec.stSwms.stRect.u32Height;
					videoInfo->dstVideoInfo.u16VideoWidth =
							it->second.stVdec.stSwms.stRect.u32Width;
				}
				memset(videoInfo->au8InputRtspURL, 0, DP_URL_LEN);
				strcpy((DP_CHAR*) videoInfo->au8InputRtspURL,
						(DP_CHAR*) it->second.stStream._rtsp.stRtspClient.au8Url);
				LOG_WARN << "check url 1: and len:  "
						<< it->second.stStream._rtsp.stRtspClient.au8Url << " "
						<< strlen(
								(DP_CHAR*) it->second.stStream._rtsp.stRtspClient.au8Url)
						<< " Check url 2: and len : "
						<< videoInfo->au8InputRtspURL
						<< strlen((DP_CHAR*) videoInfo->au8InputRtspURL);
				LOG_INFO << "videoInfo->au8InputRtspURL: "
						<< videoInfo->au8InputRtspURL << " from : "
						<< it->second.stStream._rtsp.stRtspClient.au8Url;
//				muduo::PrintBuff::printBufferByHex("stRtspClient",
//						it->second.stStream._rtsp.stRtspClient.au8Url,
//						DP_M2S_URL_LEN);
//				muduo::PrintBuff::printBufferByHex("videoInfo",
//						videoInfo->au8InputRtspURL,
//						DP_M2S_URL_LEN);
				if (!muduo::Singleton<NodeInfo>::instance().getThirdIDSrcVideoInfo()->empty())
					videoInfo->srcVideoInfo =
							muduo::Singleton<NodeInfo>::instance().getThirdIDSrcVideoInfo()->operator [](
									videoInfo->u32TaskID);
				LOG_INFO << "src video infoHeight : "
						<< videoInfo->srcVideoInfo.u16VideoHeight << " width: "
						<< videoInfo->srcVideoInfo.u16VideoWidth;
				LOG_INFO << "src video start: x: y: end: x: y:  "
						<< videoInfo->srcVideoInfo.u16StartX << " "
						<< videoInfo->srcVideoInfo.u16StartY << " "
						<< videoInfo->srcVideoInfo.u16EndX << " "
						<< videoInfo->srcVideoInfo.u16EndY;
				buffSend.append(videoInfo.get(), sizeof(_sVideoTaskInfo));

			}
		LOG_INFO << "Get info send to remote " << buffSend.readableBytes()
				<< " bytes !";
		connPtr->send(&buffSend);
	}
		break;

	case Property_Get_OutputAudioChnInfo:
		//获取输出节点的音频输入输出的通道信息
	{

	}
		break;
//	default:
//		break;
	}
}

void LogicHandle::setInfo(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
}

void LogicHandle::createWindow(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "CreateWindow cmd...";
	_sRemote_CreateWindow *createWinData =
			(_sRemote_CreateWindow*) data.c_str();
//video DP_M2S_VO_GET_INFO_S
//get task id
	DP_S32 id = 0;
	if (createWinData->u8AudioIn == 0x00)
		id = muduo::Singleton<NodeInfo>::instance().getNewCodecTaskID(
				createWinData->u32TaskID, _eVideoTask);
	else if (createWinData->u8AudioIn == 0x01)
		id = muduo::Singleton<NodeInfo>::instance().getNewCodecTaskID(
				createWinData->u32TaskID, _eAudioAndVideoTask); //////_eAudioAndVideoTask tmp use _eVideoTask
	LOG_INFO << "Create win third task ID: " << createWinData->u32TaskID
			<< " codec ID: " << id;
//full
	if (id == -2)
		return;

	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();

	//down other window priority to minus 16
//	for_each(vAVDecInfo->begin(),
//			vAVDecInfo->end(),prioritySorted);

	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
		return;
	}

	//input node

	DP_M2S_SOURCE_RELATION_S srcRelaIn(DP_M2S_MOD_VDEC,
			createWinData->u8VoChnID, 0);
	DP_M2S_SOURCE_RELATION_S srcRelaOut(DP_M2S_MOD_VO, createWinData->u8VoChnID, //?? stout
			createWinData->u8VoChnID); //????????????????????????????
	DP_M2S_BIND_S stBind(srcRelaIn, srcRelaOut);
	DP_M2S_AVBIND_ATTR_S bindAttr(DP_M2S_AVBIND_VDEC2VO, DP_M2S_BIND_S(),
			stBind);
	DP_M2S_RTSP_CLIENT_ATTR_S clientAttr;

	///////////////////////////////
	it->stStream.enType = DP_M2S_STREAM_RTSP_CLIENT;
//	memcpy(it->stStream._rtsp.stRtspClient.au8Url, "rtsp://172.16.10.199:554/chn1",
//	DP_M2S_URL_LEN);
	memset(it->stStream._rtsp.stRtspClient.au8Url, 0, DP_M2S_URL_LEN);
	strcpy((DP_CHAR*) it->stStream._rtsp.stRtspClient.au8Url,
			(DP_CHAR*) createWinData->au8RtspURL);
	it->stStream._rtsp.stRtspClient.bMulticast = DP_FALSE;
	it->stStream._rtsp.stRtspClient.s32TransType = 1;
	it->stStream._rtsp.stRtspClient.s8Open = 2;

	///////////////////////////////

	clientAttr.bMulticast = DP_FALSE;
	clientAttr.s32TransType = 1;
//一般 0位禁用 1为启用
	if (createWinData->u8AudioMute == 0x00)
		clientAttr.s8Open = 0x02;
	else if (createWinData->u8AudioMute == 0x01) {
		bindAttr.enBindType = DP_M2S_AVBIND_ADEC2AO_VDEC2VO;
		bindAttr.stAudio.stOut.ModId = DP_M2S_MOD_ADEC;
		bindAttr.stAudio.stOut.u32ChnId = createWinData->u8AoChnID;
		clientAttr.s8Open = 0x03;
	}
	LOG_INFO << "createWinData->u8AudioMute : " << createWinData->u8AudioMute
			<< "  createWinData->au8RtspURL: " << createWinData->au8RtspURL;
	clientAttr.s8Open = 2;

//	DP_M2S_CROP_ATTR_S corpAttr;
//	corpAttr.s32X = createWinData->dstVideoInfo.u16StartX;
//	corpAttr.s32Y = createWinData->dstVideoInfo.u16StartY;
//	corpAttr.u32Height = createWinData->dstVideoInfo.u16VideoHeight;
//	corpAttr.u32Width = createWinData->dstVideoInfo.u16VideoWidth;
	DP_M2S_ALG_ATTR_S alg;
	alg.enAlg = DP_M2S_ALG_H264_DEC;
	DP_M2S_RECT_S rect;
	rect.s32X = createWinData->dstVideoInfo.u16StartX;
	rect.s32Y = createWinData->dstVideoInfo.u16StartY;
	rect.u32Height = createWinData->dstVideoInfo.u16VideoHeight;
	rect.u32Width = createWinData->dstVideoInfo.u16VideoWidth;

	LOG_INFO << "rect x: y: height: width: " << rect.s32X << " " << rect.s32Y
			<< " " << rect.u32Height << " " << rect.u32Width;
	//first param 0 --------------> default
//	DP_M2S_SWMS_ATTR_S swms(0, 0, 0, rect);
//	DP_M2S_SWMS_ATTR_S swms;
//	swms.s32SwmsChn+=16;
//	swms.stRect=rect;
//	DP_M2S_VDEC_ATTR_S viDecAttr(DP_TRUE, DP_FALSE, DP_FALSE, DP_TRUE, alg,
//			DP_M2S_CROP_ATTR_S(), DP_M2S_ZOOM_ATTR_S(), DP_M2S_OSD_ATTR_S(),
//			swms);
	DP_M2S_ADEC_ATTR_S auDecAttr(DP_M2S_ALG_H264_DEC);
	DP_M2S_STREAM_ATTR_S streamAttr(DP_M2S_STREAM_RTSP_CLIENT, clientAttr);
//	DP_M2S_AVDEC_SET_INFO_S info(id, bindAttr, streamAttr, auDecAttr,
//			viDecAttr);

	///////////////////////////////

	//priority
	NodeInfo::VctrWindowPriorityPtr winPriority =
			muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();
	DP_U32 newPriority = 0;
	if (!winPriority->empty())
		newPriority = winPriority->back() + 1;
	winPriority->push_back(newPriority);
	sort(winPriority->begin(), winPriority->end());
	for (NodeInfo::VctrWindowPriority::iterator itP = winPriority->begin();
			itP != winPriority->end(); itP++)
		LOG_INFO << "new priority: " << *itP;
	muduo::Singleton<NodeInfo>::instance().updateVctrWindowPriority(
			winPriority);

	it->stVdec.stAlg.enAlg = DP_M2S_ALG_H264_DEC;
	_sSrcVideoInfo srcVideo = createWinData->srcVideoInfo;
	DP_M2S_CROP_ATTR_S crop;
	crop.s32X = srcVideo.u16StartX;
	crop.s32Y = srcVideo.u16StartY;
	crop.u32Height = srcVideo.u16EndX - srcVideo.u16StartX;
	crop.u32Width = srcVideo.u16EndY - srcVideo.u16StartY;

	LOG_INFO << "corp start x: y: end x: y: hei: Wid: " << srcVideo.u16StartX
			<< " " << srcVideo.u16StartY << " " << srcVideo.u16EndX << " "
			<< srcVideo.u16EndY << " " << srcVideo.u16VideoHeight << " "
			<< srcVideo.u16VideoWidth;
	it->stVdec.bCrop = DP_TRUE;
	it->stVdec.stCrop = crop;

	it->stVdec.bSwms = DP_TRUE;
	it->stVdec.stSwms.stRect = rect;

	it->stVdec.stSwms.u32Priority = newPriority;
	LOG_INFO << "priority current create win task : " << newPriority;
//	LOG_INFO<<"it->stVdec.stSwms.s32SwmsChn :::::::::: "<<it->stVdec.stSwms.s32SwmsChn;
	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	///////////////////////////////

	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32)
			+ sizeof(DP_M2S_AVDEC_SET_INFO_S);
//	DP_U32 infoLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, sizeof(DP_M2S_AVDEC_SET_INFO_S));

//	LOG_INFO << "In create win PackageLen: " << packageLen
//	<< "  setInfo.stHeader.u16PacketLen"
//	<< setInfo.stHeader.u16PacketLen << " and Task codec ID:: "
//	<< " sizeof(DP_M2S_AVDEC_SET_INFO_S): "
//	<< sizeof(DP_M2S_AVDEC_SET_INFO_S)
//	<< " sizeof(DP_M2S_INFO_TYPE_E): " << sizeof(DP_M2S_INFO_TYPE_E);
	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CreateWindow), Command_CreateWindow,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CreateWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			createWinData->u32TaskID, 0);
	muduo::net::Buffer sendBuff;
	sendBuff.append(&setInfo,
			sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
					+ sizeof(DP_U32));
	sendBuff.append(&(*it), sizeof(DP_M2S_AVDEC_SET_INFO_S));
//	LOG_INFO << " --------	info.stVdec.stSwms.stRect.u32Height: "
//	<< info.stVdec.stSwms.stRect.u32Height
//	<< " createWinData->dstVideoInfo.u16VideoHeight: "
//	<< createWinData->dstVideoInfo.u16VideoHeight
//	<< " info.stVdec.stSwms.stRect.u32Width :"
//	<< info.stVdec.stSwms.stRect.u32Width
//	<< "  createWinData->dstVideoInfo.u16VideoWidth: "
//	<< createWinData->dstVideoInfo.u16VideoWidth << " swmschid: "
//	<< info.stVdec.stSwms.s32SwmsChn << " u32Priority:: "
//	<< info.stVdec.stSwms.u32Priority;
	if (sendAckToCodec(sendBuff.toStringPiece().data(), packageLen, 0x01)) {
		reply.u32Success = 0;
	} else {
		LOG_WARN << "Send to codec return false in creating win func!";
//		reply.u32Success = 1;
	}
	reply.u32Success = 0;
	//modify win priority

	//modify win priority end

	//add update 视频任务（窗口）信息 _sVideoTaskInfo <-- VctrVOGetInfo
	NodeInfo::VctrVOGetInfoPtr AOInfo =
			muduo::Singleton<NodeInfo>::instance().getVOGetInfo();

	//src video info
	NodeInfo::MapThirdIDSrcVideoInfoPtr thirdIDSrcVideo = muduo::Singleton<
			NodeInfo>::instance().getThirdIDSrcVideoInfo();
	thirdIDSrcVideo->insert(
			NodeInfo::MapThirdIDSrcVideoInfo::value_type(
					createWinData->u32TaskID, createWinData->srcVideoInfo));

	muduo::Singleton<NodeInfo>::instance().updateMapThirdIDSrcVideoInfo(
			thirdIDSrcVideo);
	NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
			NodeInfo>::instance().getOutSWMSChCodecDecInfo();
//	muduo::PrintBuff::printBufferByHex("ittttttturl:::::::: ",
//			it->stStream._rtsp.stRtspClient.au8Url, 128);
	swmsDecInfo->insert(
			NodeInfo::MapOutSWMSChCodecDecInfo::value_type(
					it->stVdec.stSwms.s32SwmsChn, *it));
	muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
			swmsDecInfo);
//	if (createWinData->header.u8PackageType == 0x01) {
	muduo::PrintBuff::printBufferByHex("Reply to pc by creating win ", &reply,
			reply.header.u16PackageLen);
	muduo::net::Buffer buff;
	buff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&buff);
//	} else {
//	}
}

void LogicHandle::moveWindow(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "Move win cmd.....";
	_sRemote_MoveWindow *moveWinData = (_sRemote_MoveWindow*) data.c_str();
//	LOG_WARN<<"src:: "<<moveWinData->srcVideoInfo.u16VideoHeight;
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			moveWinData->u32TaskID, _eVideoTask);
	if (id == 0xffff) {
		LOG_WARN << "Did not find a codec task ID by third task ID :"
				<< moveWinData->u32TaskID << " codec ID: " << id;
		return;
	} else
	LOG_INFO << "codec id in move win: " << id;

	//
	///////////get task id =---> avdec info and modify
	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
	}

	_sDstVideoInfo dstVideo = moveWinData->dstVideoInfo;
	_sSrcVideoInfo srcVideo = moveWinData->srcVideoInfo;

//	DP_M2S_SWMS_ATTR_S swms;
//	swms.stRect.s32X = dstVideo.u16StartX;
//	swms.stRect.s32Y = dstVideo.u16StartY;
//	swms.stRect.u32Height = dstVideo.u16VideoHeight;
//	swms.stRect.u32Width = dstVideo.u16VideoWidth;
	DP_M2S_RECT_S rect;
	rect.s32X = dstVideo.u16StartX;
	rect.s32Y = dstVideo.u16StartY;
	rect.u32Height = dstVideo.u16VideoHeight;
	rect.u32Width = dstVideo.u16VideoWidth;
	LOG_INFO << "swms x: y: hei: Wid: " << dstVideo.u16StartX << " "
			<< dstVideo.u16StartY << " " << dstVideo.u16VideoHeight << " "
			<< dstVideo.u16VideoWidth;

	DP_M2S_CROP_ATTR_S crop;
	crop.s32X = srcVideo.u16StartX;
	crop.s32Y = srcVideo.u16StartY;
	crop.u32Height = srcVideo.u16EndX - srcVideo.u16StartX;
	crop.u32Width = srcVideo.u16EndY - srcVideo.u16StartY;

	LOG_INFO << "corp start x: y: end x: y: hei: Wid: " << srcVideo.u16StartX
			<< " " << srcVideo.u16StartY << " " << srcVideo.u16EndX << " "
			<< srcVideo.u16EndY << " " << srcVideo.u16VideoHeight << " "
			<< srcVideo.u16VideoWidth;

	it->stVdec.bCrop = DP_TRUE;
	it->stVdec.stCrop = crop;
	it->stVdec.stSwms.stRect = rect;
	DP_U32 originPriority = it->stVdec.stSwms.u32Priority;
	LOG_INFO << "priority current move task : " << originPriority;

	//priority
	NodeInfo::VctrWindowPriorityPtr winPriority =
			muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();

	if (!winPriority->empty())
		it->stVdec.stSwms.u32Priority = winPriority->back();

	DP_U32 dataLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + dataLen;
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, dataLen);

	muduo::net::Buffer sendBuff;
	sendBuff.append(&setInfo, packageLen - dataLen);
	sendBuff.append(&(*it), dataLen);
	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_MoveWindow), Command_MoveWindow,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_MoveWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			moveWinData->u32TaskID, 0);
	if (sendAckToCodec(sendBuff.toStringPiece().data(), packageLen, 0x01)) {
	} else {
//		reply.u32Success = 1;
	}

	reply.u32Success = 0;

	//modify all avdec priority
	muduo::net::Buffer prioriytBuff;
	for (NodeInfo::VctrAVDECGetInfo::iterator itP = vAVDecInfo->begin();
			itP != vAVDecInfo->end(); itP++) {
		if (itP->stVdec.bSwms == DP_TRUE
				&& itP->stVdec.stSwms.u32Priority >= originPriority
				&& itP != it) {
			itP->stVdec.stSwms.u32Priority -= 1;
			prioriytBuff.retrieveAll();
			prioriytBuff.append(&setInfo, packageLen - dataLen);
			prioriytBuff.append(&(*itP), dataLen);
			sendAckToCodec(prioriytBuff.toStringPiece().data(), packageLen,
					0x01);
		}
	}

	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);

	NodeInfo::MapThirdIDSrcVideoInfoPtr thirdIDSrcInfo = muduo::Singleton<
			NodeInfo>::instance().getThirdIDSrcVideoInfo();
	thirdIDSrcInfo->operator [](moveWinData->u32TaskID) =
			moveWinData->srcVideoInfo;
	muduo::Singleton<NodeInfo>::instance().updateMapThirdIDSrcVideoInfo(
			thirdIDSrcInfo);

	muduo::net::Buffer buff;
	buff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&buff);

}
void LogicHandle::closeWindow(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "Close win cmd..........";
	_sRemote_CloseWindow *closeWinData = (_sRemote_CloseWindow*) data.c_str();
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			closeWinData->u32TaskID, _eVideoTask);  //l
	if (id == 0xffff)
		return;
	muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
			closeWinData->u32TaskID, _eVideoTask);

	//new
	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
	}
	LOG_INFO << "close codec task ID:: " << id;
	it->stStream._rtsp.stRtspClient.s8Open = 0;
	it->stVdec.bSwms = DP_FALSE;
//	if (it->stVdec.stSwms.u32Priority > 16)
//	it->stVdec.stSwms.u32Priority -= 16;

//	//down other window priority to minus 16
//	for_each(vAVDecInfo->begin(),
//			vAVDecInfo->end(),priorityMinus16);
	//new
	DP_U32 dataLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + dataLen;
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, dataLen);
	muduo::net::Buffer buffSend;
	buffSend.append(&setInfo, packageLen - dataLen);
	buffSend.append(&(*it), dataLen);
	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CloseWindow), Command_CloseWindow,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CloseWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			closeWinData->u32TaskID, 0);

	if (sendAckToCodec(buffSend.toStringPiece().data(),
			sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
					+ sizeof(DP_U32) + sizeof(DP_M2S_AVDEC_SET_INFO_S), 0x01)) {
		LOG_INFO << "send to codec return true";
	} else {
		LOG_WARN << "send to codec return false";
		reply.u32Success = 0;
	}
	reply.u32Success = 0;

//	vAVDecInfo->erase(it++);
	NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
			NodeInfo>::instance().getOutSWMSChCodecDecInfo();

	swmsDecInfo->erase(it->stVdec.stSwms.s32SwmsChn);

	//priority
	NodeInfo::VctrWindowPriorityPtr winPriority =
			muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();

	DP_U32 originPriority = it->stVdec.stSwms.u32Priority;
	LOG_INFO << "priority current close task : " << originPriority;

	if (!winPriority->empty()) {
		winPriority->pop_back();
//		winPriority->erase(
//				find(winPriority->begin(), winPriority->end(), originPriority));
		if (!winPriority->empty())
			sort(winPriority->begin(), winPriority->end());
	}
	muduo::Singleton<NodeInfo>::instance().updateVctrWindowPriority(
			winPriority);

	//modify all avdec priority

	muduo::net::Buffer prioriytBuff;
	for (NodeInfo::VctrAVDECGetInfo::iterator itP = vAVDecInfo->begin();
			itP != vAVDecInfo->end(); itP++) {
		if (itP->stVdec.bSwms == DP_TRUE
				&& itP->stVdec.stSwms.u32Priority >= originPriority
				&& itP != it) {
			itP->stVdec.stSwms.u32Priority -= 1;
			prioriytBuff.retrieveAll();
			prioriytBuff.append(&setInfo, packageLen - dataLen);
			prioriytBuff.append(&(*itP), dataLen);
			sendAckToCodec(prioriytBuff.toStringPiece().data(), packageLen,
					0x01);
		}
	}
	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);

	muduo::net::Buffer buff;
	buff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&buff);
}

void LogicHandle::openAudio(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	_sRemote_OpenAudio *openAudioData = (_sRemote_OpenAudio*) data.c_str();
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			openAudioData->u32TaskID, _eAudioTask);
	if (id == 0xffff)
		return;

	DP_M2S_SOURCE_RELATION_S srcRelaOut(DP_M2S_MOD_ADEC, 0,  //?? stout
			openAudioData->u8AoChnID);  //??
	DP_M2S_BIND_S stBind(DP_M2S_SOURCE_RELATION_S(), srcRelaOut);
	DP_M2S_AVBIND_ATTR_S bindAttr(DP_M2S_AVBIND_ADEC2AO, stBind,
			DP_M2S_BIND_S());

	DP_M2S_RTSP_CLIENT_ATTR_S clientAttr;
	clientAttr.s8Open = 0x01;
	DP_M2S_STREAM_ATTR_S streamAttr(DP_M2S_STREAM_RTSP_CLIENT, clientAttr);

	DP_M2S_AVDEC_SET_INFO_S info(id, bindAttr, streamAttr, DP_M2S_ADEC_ATTR_S(),
			DP_M2S_VDEC_ATTR_S());

	DP_M2S_CMD_SETINFO_S setInfo(
			sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
					+ sizeof(DP_U32) + sizeof(DP_M2S_AVDEC_SET_INFO_S),
			DP_M2S_CMD_SETINFO, 0x01, DP_M2S_INFO_TYPE_SET_AVDEC,
			sizeof(DP_M2S_AVDEC_SET_INFO_S), (DP_U8*) &info);

	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CloseWindow_tag), Command_OpenAudio,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CloseWindow_tag reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			openAudioData->u32TaskID, 0);
	if (sendAckToCodec(&setInfo,
			sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
					+ sizeof(DP_U32) + sizeof(DP_M2S_AVDEC_SET_INFO_S), 0x01)) {
	} else {
		reply.u32Success = 1;
	}

	if (openAudioData->header.u8PackageType == 0x01) {
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
	}
}

void LogicHandle::closeAudio(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	_sRemote_CloseAudio *closeAudio = (_sRemote_CloseAudio*) data.c_str();
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			closeAudio->u32TaskID, _eAudioTask);  //l
	if (id == 0xffff)
		return;

	DP_M2S_SOURCE_RELATION_S srcRela(DP_M2S_MOD_ADEC, 0,  //?? stout//l
			closeAudio->u8AoChnID);
	DP_M2S_BIND_S stBind(DP_M2S_SOURCE_RELATION_S(), srcRela);
	DP_M2S_AVBIND_ATTR_S bindAttr(DP_M2S_AVBIND_ADEC2AO, DP_M2S_BIND_S(),  //l
			stBind);

	DP_M2S_RTSP_CLIENT_ATTR_S clientAttr;
	clientAttr.s8Open = 0x00;  //l
	DP_M2S_STREAM_ATTR_S streamAttr(DP_M2S_STREAM_RTSP_CLIENT, clientAttr);

	DP_M2S_AVDEC_SET_INFO_S info(id, bindAttr, streamAttr, DP_M2S_ADEC_ATTR_S(),
			DP_M2S_VDEC_ATTR_S());

	DP_M2S_CMD_SETINFO_S setInfo(
			sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
					+ sizeof(DP_U32) + sizeof(DP_M2S_AVDEC_SET_INFO_S),
			DP_M2S_CMD_SETINFO, 0x01, DP_M2S_INFO_TYPE_SET_AVDEC,
			sizeof(DP_M2S_AVDEC_SET_INFO_S), (DP_U8*) &info);

	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CloseWindow), Command_CloseAudio,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CloseWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			closeAudio->u32TaskID, 0);
	if (sendAckToCodec(&setInfo, setInfo.stHeader.u16PacketLen, 0x01)) {
	} else {
		reply.u32Success = 1;
	}

	if (closeAudio->header.u8PackageType == 0x01) {
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
	}
}

bool LogicHandle::sendAckToCodec(const void *data, DP_S32 dataLen,
		DP_U8 needReply) {
	muduo::net::Buffer buff;
	buff.append(data, dataLen);
	cout << "data len :::::::::::::::::::::::::::::::::::" << dataLen << endl;
	if (needReply == 0x01) {
		UnixSockClientDataPtr client(new UnixSockClientData(NodeInfo::recvCB));
		if (client->doSendCommand(buff.toStringPiece().data(), dataLen) == 0)
			return true;
		else
			return false;
	}
	return true;
}

//void LogicHandle::GetInputNodeInfoCB(const muduo::net::TcpConnectionPtr& conn,
//		muduo::net::Buffer* buf, muduo::Timestamp receiveTime) {
//	DP_M2S_CMD_SETINFO_S setInfo;
//	_sAllVencChnInfo *streamInfo =
//			(_sAllVencChnInfo*) buf->toStringPiece().data();
//
//	_cond.notify();
//}
