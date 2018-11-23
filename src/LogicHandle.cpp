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

LogicHandle::LogicHandle() {
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
		Get_InputVideoChnInfo(connPtr);
	}
		break;
	case Property_Get_VideoChnVencInfo: { //获取输入节点的视频采集通道的流信息
		Get_VideoChnVencInfo(connPtr);
	}
		break;
	case Property_Get_InputAudioChnInfo: {
		//获取输入节点的音频输入的通道信息
		Get_InputAudioChnInfo(connPtr);
	}
		break;
	case Property_Get_OutputVideoChnInfo: {
		/// real time
		//获取输出节点的显示视频通道信息
		Get_OutputVideoChnInfo(connPtr);
	}
		break;
		//获取输出节点的音频输入输出的通道信息
	case Property_Get_OutputAudioChnInfo: {
		Get_OutputAudioChnInfo(connPtr);
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
//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
	LOG_WARN << "CreateWindow cmd...";
	_sRemote_CreateWindow *createWinData =
			(_sRemote_CreateWindow*) data.c_str();

	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CreateWindow), Command_CreateWindow,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CreateWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			createWinData->u32TaskID, 0);
	muduo::net::Buffer sendBuff;
	//get task id
	DP_S32 id = 0;
	if (createWinData->u8AudioIn == 0x00)
		id = getNewCodecTaskID(createWinData->u32TaskID, _eVideoTask,
				reply.u32Success);
	else if (createWinData->u8AudioIn == 0x01)
		/////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		id = getNewCodecTaskID(createWinData->u32TaskID, _eAudioAndVideoTask, ///_eAudioAndVideoTask
				reply.u32Success);
	LOG_INFO << "Create win third task ID: " << createWinData->u32TaskID
			<< " codec ID: " << id;
//full
	if (id == -1) {
		sendBuff.retrieveAll();
		sendBuff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&sendBuff);
		return;
	}

	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
		reply.u32Success = 1;
		sendBuff.retrieveAll();
		sendBuff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&sendBuff);
		return;
	}

	///////////////////////////////
	DP_U8 AOChnID = 0;
	switch (createWinData->u8AoChnID) {
	case ID_AO_CHN_VIDEOOUT1:
		AOChnID = _videoAudioChn;
		break;
	case ID_AO_CHN_LINEOUT1:
		AOChnID = _pureAudioChn;
		break;
	default:
		break;
	}

	it->stStream.enType = DP_M2S_STREAM_RTSP_CLIENT;
	it->stStream._rtsp.stRtspClient.bMulticast = DP_FALSE;
	it->stStream._rtsp.stRtspClient.s32TransType = 1;
	it->AvBindAttr.stVideo.stOut.u32DevId =
			createWinData->u8VoChnID == 0x60 ? 0 : 1;
//	it->AvBindAttr.enBindType = DP_M2S_AVBIND_VDEC2VO;
	if (createWinData->u8AudioEnable == 0x00) {
		it->stStream._rtsp.stRtspClient.s8Open = 0x02;
	} else if (createWinData->u8AudioEnable == 0x01) {
		if (createWinData->u8AudioIn == 0x01) {
//			it->AvBindAttr.enBindType = DP_M2S_AVBIND_ADEC2AO_VDEC2VO;
			it->AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_ADEC;
//			it->AvBindAttr.stAudio.stOut.u32DevId = 0x60 ? 0 : 1;
			it->stAdec.enAlg = DP_M2S_ALG_AAC_DEC;
			LOG_INFO << "AOChnID in enable 1 audioin 1 :" << AOChnID;
			it->AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AO;
			it->AvBindAttr.stAudio.stOut.u32DevId = AOChnID;
			it->stStream._rtsp.stRtspClient.s8Open = 0x02;
		} else {
			it->stStream._rtsp.stRtspClient.s8Open = 0x02;
		}
	} else if (createWinData->u8AudioEnable == 0x02) {
		it->stStream._rtsp.stRtspClient.s8Open = 0x02;
	}

	memset(it->stStream._rtsp.stRtspClient.au8Url, 0, DP_M2S_URL_LEN);
	strcpy((DP_CHAR*) it->stStream._rtsp.stRtspClient.au8Url,
			(DP_CHAR*) createWinData->au8RtspURL);

	///////////////////////////////

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
	crop.u32Width = srcVideo.u16EndX - srcVideo.u16StartX;
	crop.u32Height = srcVideo.u16EndY - srcVideo.u16StartY;

	LOG_INFO << "corp start x: y: end x: y: hei: Wid: " << srcVideo.u16StartX
			<< " " << srcVideo.u16StartY << " " << srcVideo.u16EndX << " "
			<< srcVideo.u16EndY << " " << srcVideo.u16VideoHeight << " "
			<< srcVideo.u16VideoWidth;
	LOG_INFO << "corp width: height: " << crop.u32Width << " "
			<< crop.u32Height;
	it->stVdec.bCrop = DP_TRUE;
	it->stVdec.stCrop = crop;

	it->stVdec.bSwms = DP_TRUE;
	it->stVdec.stSwms.stRect = rect;

	it->stVdec.stSwms.u32Priority = newPriority;
	LOG_INFO << "priority current create win task : " << newPriority;
//	LOG_INFO<<"it->stVdec.stSwms.s32SwmsChn :::::::::: "<<it->stVdec.stSwms.s32SwmsChn;

	/*
	 *---------------------------------------------
	 * 	enable-		0		1		2
	 * -audioin
	 * --------------------------------------------
	 * 0			close	close	remaining
	 * --------------------------------------------
	 * 1			close	close	remaining
	 * 						&open
	 * --------------------------------------------
	 * notify VO AO dev ID
	 * if AV play chn --> VO DEVID
	 * if pure Audio play chn --> AO DEVID
	 * */
	DP_U32 stSize = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + stSize;
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, stSize);
#if 0
	//to shift audio or close audio
	switch (createWinData->u8VoChnID) {
		case ID_VO_CHN_VIDEOOUT1:
		AOChnID = _videoAudioChn;
		break;
		default:
		break;
	}
	NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
	muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
	NodeInfo::VctrAVDECGetInfo::iterator itAudio;

	if (createWinData->u8AudioEnable == 0x00
			|| createWinData->u8AudioEnable == 0x01) {
		LOG_INFO
		<< "createWinData->u8AudioEnable == 0x00 or  createWinData->u8AudioEnable == 0x01";
		closeAAudio(AOChnID, createWinData->u8VoChnID, setInfo, vAVDecInfo);
	}

	if (createWinData->u8AudioEnable == 0x01
			&& createWinData->u8AudioEnable == 0x01) {
		LOG_INFO
		<< "createWinData->u8AudioEnable == 0x01 and  createWinData->u8AudioEnable == 0x01";
		AODevCodecID->insert(
				NodeInfo::MapAODevIDCodecID::value_type(AOChnID, id));
	}
	muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(AODevCodecID);
#endif
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	///////////////////////////////

//	LOG_INFO << "In create win PackageLen: " << packageLen
//	<< "  setInfo.stHeader.u16PacketLen"
//	<< setInfo.stHeader.u16PacketLen << " and Task codec ID:: "
//	<< " sizeof(DP_M2S_AVDEC_SET_INFO_S): "
//	<< sizeof(DP_M2S_AVDEC_SET_INFO_S)
//	<< " sizeof(DP_M2S_INFO_TYPE_E): " << sizeof(DP_M2S_INFO_TYPE_E);
//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
//	NodeInfo::printAVDEC(&(*it));
	sendBuff.retrieveAll();
	sendBuff.append(&setInfo, packageLen - stSize);
	sendBuff.append(&(*it), stSize);
//	muduo::Logger::setLogLevel(muduo::Logger::INFO);
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
		reply.u32Success = 1;
	}
//	reply.u32Success = 0;

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

	muduo::PrintBuff::printBufferByHex("Reply to pc by creating win ", &reply,
			reply.header.u16PackageLen);
	muduo::net::Buffer buff;
	buff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&buff);
//

}

void LogicHandle::moveWindow(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "Move win cmd.....";
	_sRemote_MoveWindow *moveWinData = (_sRemote_MoveWindow*) data.c_str();
//	LOG_WARN<<"src:: "<<moveWinData->srcVideoInfo.u16VideoHeight;
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			moveWinData->u32TaskID);

	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_MoveWindow), Command_MoveWindow,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_MoveWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			moveWinData->u32TaskID, 0);
	if (id == 0xffff) {
		LOG_WARN << "Did not find a codec task ID by third task ID :"
				<< moveWinData->u32TaskID << " codec ID: " << id;
		reply.u32Success = 1;
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
		return;
	} else {
		LOG_INFO << "codec id in move win: " << id;
	}

	//
	///////////get task id =---> avdec info and modify
	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
		reply.u32Success = 1;
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
	}

	_sDstVideoInfo dstVideo = moveWinData->dstVideoInfo;
	_sSrcVideoInfo srcVideo = moveWinData->srcVideoInfo;

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
	crop.u32Width = srcVideo.u16EndX - srcVideo.u16StartX;
	crop.u32Height = srcVideo.u16EndY - srcVideo.u16StartY;

	LOG_INFO << "corp start x: y: end x: y: hei: Wid: " << srcVideo.u16StartX
			<< " " << srcVideo.u16StartY << " " << srcVideo.u16EndX << " "
			<< srcVideo.u16EndY << " " << srcVideo.u16VideoHeight << " "
			<< srcVideo.u16VideoWidth;
	LOG_INFO << "corp width: height: " << crop.u32Width << " "
			<< crop.u32Height;

	it->stVdec.bCrop = DP_TRUE;
	it->stVdec.stCrop = crop;

	it->stVdec.stSwms.stRect = rect;
	DP_U32 originPriority = it->stVdec.stSwms.u32Priority;
	LOG_INFO << "priority current move task : " << originPriority;

	NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
			NodeInfo>::instance().getOutSWMSChCodecDecInfo();
//	muduo::PrintBuff::printBufferByHex("ittttttturl:::::::: ",
//			it->stStream._rtsp.stRtspClient.au8Url, 128);
	swmsDecInfo->operator [](it->stVdec.stSwms.s32SwmsChn) = *it;
	muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
			swmsDecInfo);
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
	if (sendAckToCodec(sendBuff.toStringPiece().data(), packageLen, 0x01)) {
		reply.u32Success = 0;
	} else {
		LOG_WARN << "Send to codec return false in moving win func!";
		reply.u32Success = 1;
	}
//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
//	NodeInfo::printAVDEC(&(*it));
//	muduo::Logger::setLogLevel(muduo::Logger::INFO);

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
//			muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
//			NodeInfo::printAVDEC(&(*itP));
//			muduo::Logger::setLogLevel(muduo::Logger::INFO);
		}
	}

	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);

	//
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
			closeWinData->u32TaskID);
	if (id == 0xffff)
		return;
	muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
			closeWinData->u32TaskID);
	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CloseWindow), Command_CloseWindow,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CloseWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			closeWinData->u32TaskID, 0);
	//new
	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
		reply.u32Success = 1;
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
	}

	LOG_INFO << "close codec task ID:: " << id;
	it->stStream._rtsp.stRtspClient.s8Open = 0;
	it->stVdec.bSwms = DP_FALSE;

	//new
	DP_U32 dataLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + dataLen;
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, dataLen);
	muduo::net::Buffer buffSend;
	buffSend.append(&setInfo, packageLen - dataLen);
	buffSend.append(&(*it), dataLen);

	if (sendAckToCodec(buffSend.toStringPiece().data(),
			sizeof(DP_M2S_INF_PROT_HEAD_S) + sizeof(DP_M2S_INFO_TYPE_E)
					+ sizeof(DP_U32) + sizeof(DP_M2S_AVDEC_SET_INFO_S), 0x01)) {
		LOG_INFO << "send to codec return true";
		reply.u32Success = 0;
	} else {
		LOG_WARN << "send to codec return false";
		reply.u32Success = 1;
	}

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
	muduo::net::Buffer buff;

	//close audio
	DP_U8 AOChnID = 0;
	switch (closeWinData->u8VoChnID) {
	case ID_VO_CHN_VIDEOOUT1:
		AOChnID = _videoAudioChn;
		break;
	default:
		break;
	}
//	closeAAudio(AOChnID, closeWinData->u8VoChnID, setInfo, vAVDecInfo);

	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);

	buff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&buff);
}

void LogicHandle::openAudio(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "OpenAudio cmd...";
	_sRemote_OpenAudio *openAudioData = (_sRemote_OpenAudio*) data.c_str();

	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CreateWindow), Command_OpenAudio,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CreateWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			openAudioData->u32TaskID, 0);
	DP_S32 id = 0;
//check is shift audio or not
	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();
//get task id
	if (thirdCodecID->find(openAudioData->u32TaskID) != thirdCodecID->end())
		id = thirdCodecID->operator [](openAudioData->u32TaskID);
	else
		id = getNewCodecTaskID(openAudioData->u32TaskID, _eAudioTask,
				reply.u32Success);

	LOG_INFO << "Open Audio third task ID: " << openAudioData->u32TaskID
			<< " codec ID: " << id;
//full
	if (id == -1) {
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
		return;
	}

//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
		reply.u32Success = 1;
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
		return;
	}
	DP_U8 AOChnID = 0;
	switch (openAudioData->u8AoChnID) {
	case ID_AO_CHN_VIDEOOUT1:
		AOChnID = _videoAudioChn;
		break;
	case ID_AO_CHN_LINEOUT1:
		AOChnID = _pureAudioChn;
		break;
	}

	muduo::net::Buffer sendBuff;
	DP_U32 stSize = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + stSize;
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, stSize);

	//shift or open audio
	NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
			muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
	NodeInfo::VctrAVDECGetInfo::iterator itAudio;
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;
	DP_S32 closeAudioTaskID = 0;
	LOG_INFO << "Open Audio on chn : " << AOChnID;
	if (AODevCodecID->find(AOChnID) != AODevCodecID->end()) {
		LOG_INFO << "Find aochn: " << AOChnID;
		closeAudioTaskID = AODevCodecID->operator [](AOChnID);
		if (closeAudioTaskID == id) {
			LOG_INFO << "closeAudioTaskID: " << closeAudioTaskID << " == id : "
					<< id;
			reply.u32Success = 0;
			sendBuff.retrieveAll();
			sendBuff.append(&reply, reply.header.u16PackageLen);
			connPtr->send(&sendBuff);
			return;
		}

		if (((itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
				bind2nd(findAVDecInfoByCodecID(), closeAudioTaskID)))
				!= vAVDecInfo->end())) {
			LOG_INFO << "Shift AO";
			if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				itAudio->stStream._rtsp.stRtspClient.s8Open = 0x00;
			else if (itAudio->AvBindAttr.enBindType
					== DP_M2S_AVBIND_ADEC2AO_VDEC2VO)
				itAudio->stStream._rtsp.stRtspClient.s8Open = 0x02;
			sendBuff.retrieveAll();
			sendBuff.append(&setInfo, packageLen - stSize);
			sendBuff.append(&(*itAudio), stSize);
			LOG_INFO << "Close and shift AO devID: " << openAudioData->u8AoChnID
					<< "with old codecID: " << closeAudioTaskID;
			if (sendAckToCodec(sendBuff.toStringPiece().data(),
					setInfo.stHeader.u16PacketLen, 0x01)) {
			}
			it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
					bind2nd(findThirdIDByCodecID(), closeAudioTaskID));
			if (it_ID != thirdCodecID->end())
				//remove audio codec
				if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
					muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
							it_ID->first);
		} else {
			LOG_ERROR << "Can not find closeAudioTaskID: " << closeAudioTaskID
					<< " in vAVDecInfo";
		}
		AODevCodecID->operator [](AOChnID) = id;
	} else {
		LOG_INFO << "Not find aochn: " << AOChnID;
		AODevCodecID->insert(
				NodeInfo::MapAODevIDCodecID::value_type(AOChnID, id));
	}

	memset(it->stStream._rtsp.stRtspClient.au8Url, 0, DP_M2S_URL_LEN);
	strcpy((DP_CHAR*) it->stStream._rtsp.stRtspClient.au8Url,
			(DP_CHAR*) openAudioData->au8RtspURL);
	if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
		it->stStream._rtsp.stRtspClient.s8Open = 1;
	else if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO_VDEC2VO)
		it->stStream._rtsp.stRtspClient.s8Open = 3;
	LOG_INFO << "it->stStream._rtsp.stRtspClient.s8Open : "
			<< it->stStream._rtsp.stRtspClient.s8Open << " AOChnID " << AOChnID;
	it->AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AO;
	it->stAdec.enAlg = DP_M2S_ALG_AAC_DEC;
	it->AvBindAttr.stAudio.stOut.u32DevId = AOChnID;
//	it->AvBindAttr.enBindType = DP_M2S_AVBIND_ADEC2AO;
	LOG_INFO << "Sendto codec url:" << it->stStream._rtsp.stRtspClient.au8Url
			<< " aochn: " << it->AvBindAttr.stAudio.stOut.u32DevId;

	sendBuff.retrieveAll();
	sendBuff.append(&setInfo, packageLen - stSize);
	sendBuff.append(&(*it), stSize);

	if (sendAckToCodec(sendBuff.toStringPiece().data(),
			setInfo.stHeader.u16PacketLen, 0x01)) {
		reply.u32Success = 0;
	} else {
		LOG_WARN << "Send to codec return false in  Open audio func!";
		reply.u32Success = 1;
	}

	muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(AODevCodecID);
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	reply.u32Success = 0;
	sendBuff.retrieveAll();
	sendBuff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&sendBuff);
}

void LogicHandle::closeAudio(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "CloseAudio cmd...";
	_sRemote_CloseAudio *closeAudio = (_sRemote_CloseAudio*) data.c_str();
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			closeAudio->u32TaskID);

	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_CreateWindow), Command_CloseAudio,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	_sRemote_Reply_CreateWindow reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			closeAudio->u32TaskID, 0);
	if (id == 0xffff) {
		LOG_WARN << "Did not find a codec task ID by third task ID :"
				<< closeAudio->u32TaskID << " codec ID: " << id;
		reply.u32Success = 1;
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
		return;
	} else {
		LOG_INFO << "codec id in close audio: " << id;
	}

//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
			vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
	if (it == vAVDecInfo->end()) {
		LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
		reply.u32Success = 1;
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
		return;
	}

//	TaskObjectType_E taskType = _eVideoTask;
//	switch (it->AvBindAttr.enBindType) {
//	case DP_M2S_AVBIND_VDEC2VO:
//		taskType = _eVideoTask;
//		break;
//	case DP_M2S_AVBIND_ADEC2AO_VDEC2VO:
//		taskType = _eAudioAndVideoTask;
//		break;
//	case DP_M2S_AVBIND_ADEC2AO:
//		taskType = _eAudioTask;
//		break;
//	default:
//		break;
//	}

	//remove audio codec id
	if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
		muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
				closeAudio->u32TaskID);

	if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
		it->stStream._rtsp.stRtspClient.s8Open = 0x00;
	else if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO_VDEC2VO)
		it->stStream._rtsp.stRtspClient.s8Open = 0x02;

	DP_U8 AOChnID = 0;
	switch (closeAudio->u8AoChnID) {
	case ID_AO_CHN_VIDEOOUT1:
		AOChnID = _videoAudioChn;
		break;
	case ID_AO_CHN_LINEOUT1:
		AOChnID = _pureAudioChn;
		break;
	}
	NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
			muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
	NodeInfo::MapAODevIDCodecID::iterator iter;
	if ((iter = AODevCodecID->find(AOChnID)) != AODevCodecID->end())
		AODevCodecID->erase(iter);

	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(AODevCodecID);

	sendCMD<DP_M2S_AVDEC_GET_INFO_S, _sRemote_Reply_CreateWindow>(connPtr,
			&(*it), reply);
}

void LogicHandle::setAudioInfo(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "SetAudio cmd...";
	_sRemote_SetAudio *setAudio = (_sRemote_SetAudio*) data.c_str();

}
bool LogicHandle::sendAckToCodec(const void *data, DP_S32 dataLen,
		DP_U8 needReply) {
	muduo::net::Buffer buff;
	buff.append(data, dataLen);
	try {
		//	cout << "data len :::::::::::::::::::::::::::::::::::" << dataLen << endl;
		if (needReply == 0x01) {
			UnixSockClientDataPtr client(
					new UnixSockClientData(NodeInfo::recvCB));
			if (client->doSendCommand(buff.toStringPiece().data(), dataLen)
					== 0)
				return true;
			else
				return false;
		}
	} catch (SystemException &ex) {
		LOG_ERROR << ex.what();
		return DP_FALSE;
	}
	return false;
}

void LogicHandle::closeAAudio(DP_U8 AOChnID, DP_U8 voChnID,
		DP_M2S_CMD_SETINFO_S &setInfo,
		NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo) {

	DP_U32 dataLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + dataLen;
	NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
			muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
	NodeInfo::VctrAVDECGetInfo::iterator itAudio;
	if (AODevCodecID->find(AOChnID) != AODevCodecID->end()) {
		//the u8VoChnID is existed
		DP_U32 closeAudioTaskID = AODevCodecID->operator [](AOChnID);
		//find avdec by closing codec id
		if (((itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
				bind2nd(findAVDecInfoByCodecID(), closeAudioTaskID)))
				!= vAVDecInfo->end())) {
//			itAudio->AvBindAttr.enBindType = DP_M2S_AVBIND_VDEC2VO;
			itAudio->stStream._rtsp.stRtspClient.s8Open = 0x02;
			muduo::net::Buffer buff;
			buff.append(&setInfo, packageLen - dataLen);
			buff.append(&(*itAudio), dataLen);
			if (sendAckToCodec(buff.toStringPiece().data(),
					setInfo.stHeader.u16PacketLen, 0x01)) {
			}
			LOG_INFO << "Close AO devID: " << voChnID << "with  codecID: "
					<< closeAudioTaskID;
		} else {
			LOG_ERROR << "Will close audio with chn num: " << AOChnID
					<< " but is not found with codec task id: "
					<< closeAudioTaskID;
		}
		NodeInfo::MapAODevIDCodecID::iterator iter;
		if ((iter = AODevCodecID->find(AOChnID)) != AODevCodecID->end())
			AODevCodecID->erase(iter);
	}

	muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(AODevCodecID);
}
DP_S32 LogicHandle::getNewCodecTaskID(DP_U32 thirdTaskID, TaskObjectType_E type,
		DP_U32 &reply) {
	DP_S32 id = 0;
	id = muduo::Singleton<NodeInfo>::instance().getNewCodecTaskID(thirdTaskID,
			type);
//full
	if (id == -2 || id == -1) {
		LOG_ERROR << "Error codec task ID: " << id << " third task ID: "
				<< thirdTaskID;
		reply = 1;
		return -1;
	}
	return id;
}

void LogicHandle::Get_InputVideoChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
	LOG_INFO << "Get 'Property_Get_InputVideoChnInfo' cmd from "
			<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrVIGetInfoPtr viGetInfo =
			muduo::Singleton<NodeInfo>::instance().getVIGetInfo();
	DP_U32 propertyLen = sizeof(_sAllViChnInfo)
			+ viGetInfo->size() * sizeof(_sAllViChnInfo::_sSingleViChnInfo);
	DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName) + sizeof(DP_U32)
			+ sizeof(DP_U16) + propertyLen;
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

void LogicHandle::Get_InputAudioChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
	LOG_INFO << "Get 'Property_Get_InputAudioChnInfo' cmd from "
			<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrAIGetInfoPtr aiGetInfo =
			muduo::Singleton<NodeInfo>::instance().getAIGetInfo();
	DP_U32 propertyLen = sizeof(_sAllAiChnInfo)
			+ aiGetInfo->size() * sizeof(_sAllAiChnInfo::_sSingleAiChnInfo);
	DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName) + sizeof(DP_U32)
			+ sizeof(DP_U16) + propertyLen;
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
	NodeInfo::VctrAVENCGetInfoPtr vAVEncInNodeInfo =
			muduo::Singleton<NodeInfo>::instance().getAVEncGetInfo();
	for (NodeInfo::VctrAIGetInfo::iterator it = aiGetInfo->begin();
			it != aiGetInfo->end(); it++) {
		DP_U32 devID = it->u32DevId;
		NodeInfo::VctrAVENCGetInfo::iterator it_cam = std::find_if(
				vAVEncInNodeInfo.get()->begin(), vAVEncInNodeInfo.get()->end(),
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
			LOG_INFO << "_sAllAiChnInfo.u8AiSignalStatus: "
					<< singleAICh->u8AiSignalStatus << " url : "
					<< singleAICh->au8PreviewRtspURL
					<< " u8AiDevIntfSampleRate: "
					<< singleAICh->srcAudioInfo.u8AiDevIntfSampleRate
					<< " u8AiDevIntfSoundMode: "
					<< singleAICh->srcAudioInfo.u8AiDevIntfSoundMode
					<< " u8AiDevIntfBitwidth: "
					<< singleAICh->srcAudioInfo.u8AiDevIntfBitwidth
					<< " u8AencType: " << singleAICh->srcAudioInfo.u8AencType;
		}
	}

	LOG_INFO << "aiChCount: " << aiChCount
			<< " sizeof(_sAllAiChnInfo::_sSingleAiChnInfo): "
			<< sizeof(_sAllAiChnInfo::_sSingleAiChnInfo);
	connPtr->send(&buffSend);
}
void LogicHandle::Get_VideoChnVencInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
	LOG_INFO << "Get 'Property_Get_VideoChnVencInfo' cmd from "
			<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrAVENCGetInfoPtr vAVEncInNodeInfo =
			muduo::Singleton<NodeInfo>::instance().getAVEncGetInfo();

	DP_U32 videoCount = 0;
	for (NodeInfo::VctrAVENCGetInfo::iterator it = vAVEncInNodeInfo->begin();
			it != vAVEncInNodeInfo->end(); it++) {
		if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_VI2VENC
				|| it->AvBindAttr.enBindType == DP_M2S_AVBIND_AI2AENC_VI2VENC)
			videoCount++;
	}
	DP_U32 propertyLen = sizeof(_sAllVencChnInfo)
			+ videoCount * sizeof(_sAllVencChnInfo::_sSingleVencChnInfo);
	DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName) + sizeof(DP_U32)
			+ sizeof(DP_U16) + propertyLen;
	DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;

	_sRemote_Reply_GetInfo replyGetInfo(_netInfo.ip2U32(), g_DevType, 0x01,
			packageLen, Command_GetInfo, cmdLen,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			Property_Get_VideoChnVencInfo, 0x00, propertyLen);

	DP_U8 avEncChCount = vAVEncInNodeInfo->size();
	_sAllVencChnInfo allAiChInfo(DP_VI_DEV_MAX, videoCount);

	DP_U8 audioIn = 0;

	muduo::net::Buffer buffSend;
	buffSend.append(&replyGetInfo, packageLen - propertyLen);
	buffSend.append(&allAiChInfo, sizeof(_sAllVencChnInfo));
	for (NodeInfo::VctrAVENCGetInfo::iterator it = vAVEncInNodeInfo->begin();
			it != vAVEncInNodeInfo->end(); it++) {
		audioIn = (it->TskId >= 256 && it->TskId <= 511) ? 0x00 : 0x01;
		boost::shared_ptr<_sAllVencChnInfo::_sSingleVencChnInfo> singleVencCh(
				new _sAllVencChnInfo::_sSingleVencChnInfo(
						it->AvBindAttr.stVideo.stIn.u32DevId == 0 ?
								ID_VI_CHN_VIDEOIN1 : ID_VI_CHN_VIDEOIN2,
						it->AvBindAttr.stVideo.stOut.u32ChnId, 0x0000,
						it->stVenc.stCrop.u32Width, it->stVenc.stCrop.u32Height,
						it->stVenc.stCrop.s32X, it->stVenc.stCrop.s32Y,
						it->stStream._rtsp.stRtspServer.au8Url,
						DP_M2S_URL_LEN, audioIn, _sSrcAudioInfo()));
		//			LOG_WARN << "check url 1: and len:  "
		//					<< it->stStream._rtsp.stRtspServer.au8Url << " "
		//					<< strlen((DP_CHAR*) it->stStream._rtsp.stRtspServer.au8Url)
		//					<< " Check url 2: and len : "
		//					<< singleVencCh->au8PreviewRtspURL << " "
		//					<< strlen((DP_CHAR*) singleVencCh->au8PreviewRtspURL);

		if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_VI2VENC) //视频输入绑到视频编码
				{
			buffSend.append(singleVencCh.get(),
					sizeof(_sAllVencChnInfo::_sSingleVencChnInfo));
		} else if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_AI2AENC_VI2VENC) { //音频输入绑到音频编码，同时，视频输入绑到视频编码
			singleVencCh->u8AudioIn = 0x01;
			NodeInfo::VctrAIGetInfoPtr aiInfo =
					muduo::Singleton<NodeInfo>::instance().getAIGetInfo();
			DP_M2S_AI_GET_INFO_S *aiGetInfo = NULL;
			for (NodeInfo::VctrAIGetInfo::iterator iter = aiInfo->begin();
					iter != aiInfo->end(); iter++) {
				if (iter->u32DevId == it->AvBindAttr.stAudio.stIn.u32DevId) {
					aiGetInfo = (DP_M2S_AI_GET_INFO_S *) (&*iter);
					break;
				}
			}
			if (aiGetInfo) {
				_sSrcAudioInfo srcAiInfo(aiGetInfo->stCommAttr.enSamplerate,
						aiGetInfo->stCommAttr.enBitwidth,
						aiGetInfo->stCommAttr.enSoundmode, AUDIO_ENCODE_AAC); //AAC default
				singleVencCh->srcAudioInfo = srcAiInfo;
			}

			buffSend.append(singleVencCh.get(),
					sizeof(_sAllVencChnInfo::_sSingleVencChnInfo));
		}
	}

	LOG_INFO << "packageLen: " << packageLen << " propertyLen: " << propertyLen
			<< "  sizeof(_sAllVencChnInfo): " << sizeof(_sAllVencChnInfo);
	LOG_INFO << "avEncChCount: " << avEncChCount
			<< " sizeof(_sAllVencChnInfo::_sSingleVencChnInfo): "
			<< sizeof(_sAllVencChnInfo::_sSingleVencChnInfo);
	LOG_INFO << "Send to third " << buffSend.readableBytes() << " bytes. ";
	connPtr->send(&buffSend);
}

void LogicHandle::Get_OutputVideoChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
	LOG_INFO << "Get 'Property_Get_OutputVideoChnInfo' cmd from "
			<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrVOGetInfoPtr VOInfo =
			muduo::Singleton<NodeInfo>::instance().getVOGetInfo();
	NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsChInfo =
			muduo::Singleton<NodeInfo>::instance().getOutSWMSChCodecDecInfo();
	DP_U32 taskCount = swmsChInfo->size();
	DP_U32 propertyLen = sizeof(_sAllVoChnInfo)
			+ VOInfo->size() * sizeof(_sSingleVoChnInfo)
			+ taskCount * sizeof(_sVideoTaskInfo);

	DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName) + sizeof(DP_U32)
			+ sizeof(DP_U16) + propertyLen;

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

	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();

	DP_U32 codecTaskID = 0;
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;
	LOG_INFO << "swmsChInfo size :: " << swmsChInfo->size();
	if (!swmsChInfo->empty())
		//equal task info
		LOG_INFO << "Video task count: " << swmsChInfo->size();
	for (NodeInfo::MapOutSWMSChCodecDecInfo::iterator it = swmsChInfo->begin();
			it != swmsChInfo->end(); it++) {
		// without audio
		////////////////////////////////////////////////////////////////////////////////
		//			if (it->second.AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO){
		//				continue;
		//			}
		codecTaskID = it->second.TskId;
		it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
				bind2nd(findThirdIDByCodecID(), codecTaskID));
		if (it_ID != thirdCodecID->end()) {
			videoInfo->u32TaskID = it_ID->first;
			LOG_INFO << " third task ID: " << videoInfo->u32TaskID
					<< " codec task ID: " << codecTaskID;
		} else {
			LOG_ERROR << "Can not find third task id by codec task id: "
					<< codecTaskID;
			return;
		}
		if (it->second.stVdec.bSwms) {
			videoInfo->u8VoChnID =
					it->second.stVdec.stSwms.s32VoDevId == 0x00 ?
							ID_VO_CHN_VIDEOOUT1 : ID_VO_CHN_VIDEOOUT2;
			videoInfo->u8WinZIndex = it->second.stVdec.stSwms.u32Priority;
			//				videoInfo->u8VStreamStatus  // none
			videoInfo->dstVideoInfo.u16StartX =
					it->second.stVdec.stSwms.stRect.s32X;
			videoInfo->dstVideoInfo.u16StartY =
					it->second.stVdec.stSwms.stRect.s32Y;
			videoInfo->dstVideoInfo.u16VideoHeight =
					it->second.stVdec.stSwms.stRect.u32Height;
			videoInfo->dstVideoInfo.u16VideoWidth =
					it->second.stVdec.stSwms.stRect.u32Width;
			LOG_INFO << "Dst video info:: x: y: H: W: "
					<< videoInfo->dstVideoInfo.u16StartX << " "
					<< videoInfo->dstVideoInfo.u16StartY << " "
					<< videoInfo->dstVideoInfo.u16VideoHeight << " "
					<< videoInfo->dstVideoInfo.u16VideoWidth;
		}

		memset(videoInfo->au8InputRtspURL, 0, DP_URL_LEN);
		strcpy((DP_CHAR*) videoInfo->au8InputRtspURL,
				(DP_CHAR*) it->second.stStream._rtsp.stRtspClient.au8Url);
		//				LOG_WARN << "check url 1: and len:  "
		//						<< it->second.stStream._rtsp.stRtspClient.au8Url << " "
		//						<< strlen(
		//								(DP_CHAR*) it->second.stStream._rtsp.stRtspClient.au8Url)
		//						<< " Check url 2: and len : "
		//						<< videoInfo->au8InputRtspURL
		//						<< strlen((DP_CHAR*) videoInfo->au8InputRtspURL);
		LOG_INFO << "videoInfo->au8InputRtspURL: " << videoInfo->au8InputRtspURL
				<< " from : " << it->second.stStream._rtsp.stRtspClient.au8Url;
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

void LogicHandle::Get_OutputAudioChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
	LOG_INFO << "Get 'Property_Get_OutputAudioChnInfo' cmd from "
			<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrAOGetInfoPtr AOInfo =
			muduo::Singleton<NodeInfo>::instance().getAOGetInfo();
	DP_U32 audioCount = AOInfo->size();

	_sAllAoChnInfo allAOChnInfo(audioCount);
	LOG_INFO << "audioCount size: " << audioCount;
	DP_U32 codecID = 0;
	NodeInfo::MapAODevIDCodecIDPtr AODEVCodecID =
			muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;

	DP_U8 AOChnID = 0;
	DP_U8 audioIn = 0;
	boost::shared_ptr<_sAudioTaskInfo> audioTask(new _sAudioTaskInfo());
	DP_U32 propertyLen = sizeof(_sAllAoChnInfo)
			+ audioCount * sizeof(_sAllAoChnInfo::_sSingleAoChnInfo);
	DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName) + sizeof(DP_U32)
			+ sizeof(DP_U16) + propertyLen;
	DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;

	_sRemote_Reply_GetInfo replyGetInfo(_netInfo.ip2U32(), g_DevType, 0x01,
			packageLen, Command_GetInfo, cmdLen,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			Property_Get_OutputAudioChnInfo, 0x00, propertyLen);
	muduo::net::Buffer buffSend;
	buffSend.append(&replyGetInfo, packageLen - propertyLen);
	buffSend.append(&allAOChnInfo, sizeof(_sAllAoChnInfo));
	DP_BOOL success = DP_TRUE;
	for (NodeInfo::VctrAOGetInfo::iterator it = AOInfo->begin();
			it != AOInfo->end(); it++) {
		switch (it->s32DevId) {
		case _videoAudioChn:
			AOChnID = ID_AO_CHN_VIDEOOUT1;
			break;
		case _pureAudioChn:
			AOChnID = ID_AO_CHN_LINEOUT1;
			break;
		default:
			break;
		}
		if (AODEVCodecID->find(it->s32DevId) == AODEVCodecID->end()) {
			LOG_ERROR << "Can not find devID: " << it->s32DevId << " in prog.";
			boost::shared_ptr<_sAllAoChnInfo::_sSingleAoChnInfo> singleAO(
					new _sAllAoChnInfo::_sSingleAoChnInfo(0x00, AOChnID,
							it->u8Vol, 0x00, *audioTask.get()));
			buffSend.append(singleAO.get(),
					sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
//			success = DP_FALSE;
			continue;
		}
		codecID = AODEVCodecID->operator [](it->s32DevId);

		NodeInfo::VctrAVDECGetInfo::iterator itAVDec = find_if(
				vAVDecInfo->begin(), vAVDecInfo->end(),
				std::bind2nd(findAVDecInfoByCodecID(), codecID));
		if (itAVDec == vAVDecInfo->end()) {
			LOG_ERROR << "Can not find avDec by codec id : " << codecID
					<< " in prog.";
			boost::shared_ptr<_sAllAoChnInfo::_sSingleAoChnInfo> singleAO(
					new _sAllAoChnInfo::_sSingleAoChnInfo(0x00, AOChnID,
							it->u8Vol, 0x00, *audioTask.get()));
			buffSend.append(singleAO.get(),
					sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
//			success = DP_FALSE;
			continue;
		}
		if (itAVDec->AvBindAttr.enBindType == DP_M2S_AVBIND_VDEC2VO) {
			audioIn = 0;
		} else {
			audioIn = 0x01;
			it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
					bind2nd(findThirdIDByCodecID(), codecID));
			if (it_ID == thirdCodecID->end()) {
				LOG_ERROR << "Can not find third task id  by codec id : "
						<< codecID << " in prog.";
				boost::shared_ptr<_sAllAoChnInfo::_sSingleAoChnInfo> singleAO(
						new _sAllAoChnInfo::_sSingleAoChnInfo(0x00, AOChnID,
								it->u8Vol, 0x00, *audioTask.get()));
				buffSend.append(singleAO.get(),
						sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
//				success = DP_FALSE;
				continue;
			}
			//				DP_U8 u8VStreamStatus;		 ///<视频输入网络流媒体信号有无???????
			_sSrcAudioInfo audioInfo(it->stCommAttr.enSamplerate,
					it->stCommAttr.enBitwidth, it->stCommAttr.enSoundmode,
					AUDIO_ENCODE_AAC);
			audioTask.reset(
					new _sAudioTaskInfo(it_ID->first, 0x01,
							itAVDec->stStream._rtsp.stRtspClient.au8Url,
							audioInfo, AOChnID));
		}
		boost::shared_ptr<_sAllAoChnInfo::_sSingleAoChnInfo> singleAO(
				new _sAllAoChnInfo::_sSingleAoChnInfo(0x01, AOChnID, it->u8Vol,
						audioIn, *audioTask.get()));
		buffSend.append(singleAO.get(),
				sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
	}
	if (success == DP_FALSE) {
		DP_U32 successResult = 1;
		memcpy(
				const_cast<DP_CHAR*>(buffSend.toStringPiece().data())
						+ sizeof(_sRemote_Header) + DP_DEV_ID_LEN
						+ sizeof(eRemotePropertyName), &successResult,
				sizeof(DP_U32));
	}
	LOG_INFO << "Get info send to remote " << buffSend.readableBytes()
			<< " bytes !";
	connPtr->send(&buffSend);
}
