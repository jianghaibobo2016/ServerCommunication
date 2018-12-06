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
	case Property_Get_InputAudioChnInfo: { //获取输入节点的音频输入的通道信息
		Get_InputAudioChnInfo(connPtr);
	}
		break;
	case Property_Get_OutputVideoChnInfo: { //获取输出节点的显示视频通道信息
		Get_OutputVideoChnInfo(connPtr);
	}
		break;
	case Property_Get_OutputAudioChnInfo: { //获取输出节点的音频输入输出的通道信息
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
	do {
		//get task id
		DP_S32 id = 0;
		if (createWinData->u8AudioIn == 0x00)
			id = getNewCodecTaskID(createWinData->u32TaskID, _eVideoTask,
					reply.u32Success);
		else if (createWinData->u8AudioIn == 0x01)
			/////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			id = getNewCodecTaskID(createWinData->u32TaskID,
					_eAudioAndVideoTask, ///_eAudioAndVideoTask
					reply.u32Success);
		else {
			reply.u32Success = 3;
			break;
		}

		LOG_INFO << "Create win third task ID: " << createWinData->u32TaskID
				<< " codec ID: " << id;
//full
		if (id == -1) {
			reply.u32Success = 2;
//			sendBuff.retrieveAll();
//			sendBuff.append(&reply, reply.header.u16PackageLen);
//			connPtr->send(&sendBuff);
			break;
		}

		//get avdec
		NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
				muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
		NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
				vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
		if (it == vAVDecInfo->end()) {
			LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
			reply.u32Success = 1;
//			sendBuff.retrieveAll();
//			sendBuff.append(&reply, reply.header.u16PackageLen);
//			connPtr->send(&sendBuff);
			break;
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

		LOG_INFO << " createWinData->au8RtspURL: " << createWinData->au8RtspURL
				<< " it->stStream._rtsp.stRtspClient.au8Url: "
				<< it->stStream._rtsp.stRtspClient.au8Url;
		///////////////////////////////

		DP_M2S_ALG_ATTR_S alg;
		alg.enAlg = DP_M2S_ALG_H264_DEC;
		DP_M2S_RECT_S rect;
		rect.s32X = createWinData->dstVideoInfo.u16StartX;
		rect.s32Y = createWinData->dstVideoInfo.u16StartY;
		rect.u32Height = createWinData->dstVideoInfo.u16VideoHeight;
		rect.u32Width = createWinData->dstVideoInfo.u16VideoWidth;

		LOG_INFO << "rect x: y: height: width: " << rect.s32X << " "
				<< rect.s32Y << " " << rect.u32Height << " " << rect.u32Width;
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

		LOG_INFO << "corp start x: y: end x: y: hei: Wid: "
				<< srcVideo.u16StartX << " " << srcVideo.u16StartY << " "
				<< srcVideo.u16EndX << " " << srcVideo.u16EndY << " "
				<< srcVideo.u16VideoHeight << " " << srcVideo.u16VideoWidth;
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
		sendBuff.retrieveAll();
		sendBuff.append(&setInfo, packageLen - stSize);
		sendBuff.append(&(*it), stSize);
//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
		NodeInfo::printAVDEC(&(*it));
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

		break;
//		muduo::net::Buffer buff;
//		buff.append(&reply, reply.header.u16PackageLen);
//		connPtr->send(&buff);
//
	} while (0);
	muduo::PrintBuff::printBufferByHex("Reply to pc by creating win ", &reply,
			reply.header.u16PackageLen);
	sendBuff.retrieveAll();
	sendBuff.append(&reply, reply.header.u16PackageLen);
	LOG_INFO << "Reply success : " << reply.u32Success;
	connPtr->send(&sendBuff);
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

//	muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
//	NodeInfo::printAVDEC(&(*it));
//	muduo::Logger::setLogLevel(muduo::Logger::INFO);

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

//	//close audio
//	DP_U8 AOChnID = 0;
//	switch (closeWinData->u8VoChnID) {
//	case ID_VO_CHN_VIDEOOUT1:
//		AOChnID = _videoAudioChn;
//		break;
//	default:
//		break;
//	}
//	closeAAudio(AOChnID, closeWinData->u8VoChnID, setInfo, vAVDecInfo);

	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);

	buff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&buff);
}

void LogicHandle::clearAllTask(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	LOG_WARN << "Clear all task cmd..........";
	_sRemote_ClearTask *clearData = (_sRemote_ClearTask*) data.c_str();
	_sRemote_Header head(_netInfo.ip2U32(), Type_DeviceOutput, 0x01,
			sizeof(_sRemote_Reply_ClearTask), Command_ClearTask,
			DP_DEV_ID_LEN + sizeof(DP_U32));
	_sRemote_Reply_ClearTask reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			0);
	//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it;
	DP_U32 dataLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	DP_U32 packageLen = sizeof(DP_M2S_INF_PROT_HEAD_S)
			+ sizeof(DP_M2S_INFO_TYPE_E) + sizeof(DP_U32) + dataLen;
	DP_M2S_CMD_SETINFO_S setInfo(packageLen, DP_M2S_CMD_SETINFO, 0x01,
			DP_M2S_INFO_TYPE_SET_AVDEC, dataLen);
	muduo::net::Buffer buffSend;
	muduo::net::Buffer buffSendToCodec;
	buffSend.retrieveAll();
	LOG_INFO << "Reply success : " << reply.u32Success;
	buffSend.append(&reply, reply.header.u16PackageLen);
	/////////////////////// send after sendto codec//////////!!!!
	connPtr->send(&buffSend);
	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;
	for (it = vAVDecInfo->begin(); it != vAVDecInfo->end(); it++) {
		if (clearData->u8TaskType == 0x00
				&& (it->AvBindAttr.enBindType == DP_M2S_AVBIND_VDEC2VO
						|| it->AvBindAttr.enBindType
								== DP_M2S_AVBIND_ADEC2AO_VDEC2VO)
				&& it->stStream._rtsp.stRtspClient.s8Open != 0x00) {
		} else if (clearData->u8TaskType == 0x01
				&& (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				&& it->stStream._rtsp.stRtspClient.s8Open != 0x00) {
		} else if (clearData->u8TaskType == 0x02
				&& (it->AvBindAttr.enBindType == DP_M2S_AVBIND_VDEC2VO
						|| it->AvBindAttr.enBindType
								== DP_M2S_AVBIND_ADEC2AO_VDEC2VO
						|| it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				&& it->stStream._rtsp.stRtspClient.s8Open != 0x00) {

		} else
			continue;
		it->stStream._rtsp.stRtspClient.s8Open = 0x00;
		buffSendToCodec.retrieveAll();
		buffSendToCodec.append(&setInfo, packageLen - dataLen);
		buffSendToCodec.append(&(*it), dataLen);
		if (sendAckToCodec(buffSendToCodec.toStringPiece().data(), packageLen,
				0x01)) {
//			LOG_INFO << "send to codec return true";
			it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
					bind2nd(findThirdIDByCodecID(), it->TskId));
			if (it_ID != thirdCodecID->end()) {
				muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
						it_ID->first);
			} else {
				LOG_ERROR << "Can not find third task id by codec id : "
						<< it->TskId;
				continue;
			}
			LOG_INFO << "]Clear third task id : " << it_ID->first
					<< " with codec id : " << it->TskId;
//			reply.u32Success = 0;
		} else {
			LOG_WARN << "send to codec return false";
//			reply.u32Success = 1;
		}

	}
	LOG_WARN << "Clear task over !";

	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
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
	DP_BOOL thirdIDExisted = DP_FALSE;
	if (thirdCodecID->find(openAudioData->u32TaskID) != thirdCodecID->end()) {
		thirdIDExisted = DP_TRUE;
		id = thirdCodecID->operator [](openAudioData->u32TaskID);
		LOG_INFO << "Find third task ID: " << openAudioData->u32TaskID
				<< " with codec id: " << id;
	}

	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
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
	DP_S32 closeAudioTaskID = 0;
	//shift or open audio
	NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
			muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
	LOG_INFO << "Open Audio on chn : " << AOChnID;
	NodeInfo::VctrAVDECGetInfo::iterator itAudio;
	if (AODevCodecID->find(AOChnID) != AODevCodecID->end()) {
		//shift audio!
		LOG_INFO << "Find aochn: " << AOChnID;
		closeAudioTaskID = AODevCodecID->operator [](AOChnID);
		if (thirdIDExisted == DP_TRUE) { //ignore
			reply.u32Success = 0;
			sendBuff.retrieveAll();
			sendBuff.append(&reply, reply.header.u16PackageLen);
			LOG_INFO << "Reply success : " << reply.u32Success;
			connPtr->send(&sendBuff);
			return;
		}
		// new third task id
		if (((itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
				bind2nd(findAVDecInfoByCodecID(), closeAudioTaskID)))
				!= vAVDecInfo->end())) {
			LOG_INFO << "Shift AO";
//						if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
			itAudio->stStream._rtsp.stRtspClient.s8Open = 0x00;
			sendBuff.retrieveAll();
			sendBuff.append(&setInfo, packageLen - stSize);
			sendBuff.append(&(*itAudio), stSize);
//						it = itAudio;
			LOG_INFO << "Close and shift AO devID: " << openAudioData->u8AoChnID
					<< "with old codecID: " << closeAudioTaskID;
			if (sendAckToCodec(sendBuff.toStringPiece().data(),
					setInfo.stHeader.u16PacketLen, 0x01)) {
				//			it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
				//					bind2nd(findThirdIDByCodecID(), closeAudioTaskID));
				//			if (it_ID != thirdCodecID->end())
				//				//remove audio codec
				//				if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				//					muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
				//							it_ID->first);
			} else {
				LOG_ERROR << "Send to codec return false in  shift audio !";
			}
		} else {
			LOG_ERROR << "Can not find closeAudioTaskID: " << closeAudioTaskID
					<< " in vAVDecInfo";
		}
//					AODevCodecID->operator [](AOChnID) = closeAudioTaskID;
		memset(itAudio->stStream._rtsp.stRtspClient.au8Url, 0, DP_M2S_URL_LEN);
		strcpy((DP_CHAR*) itAudio->stStream._rtsp.stRtspClient.au8Url,
				(DP_CHAR*) openAudioData->au8RtspURL);
		if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
			itAudio->stStream._rtsp.stRtspClient.s8Open = 1;
		LOG_INFO << "it->stStream._rtsp.stRtspClient.s8Open : "
				<< itAudio->stStream._rtsp.stRtspClient.s8Open << " AOChnID "
				<< AOChnID;
		itAudio->AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AO;
		itAudio->stAdec.enAlg = DP_M2S_ALG_AAC_DEC;
		itAudio->AvBindAttr.stAudio.stOut.u32DevId = AOChnID;
		sendBuff.retrieveAll();
		sendBuff.append(&setInfo, packageLen - stSize);
		sendBuff.append(&(*itAudio), stSize);

		if (sendAckToCodec(sendBuff.toStringPiece().data(),
				setInfo.stHeader.u16PacketLen, 0x01)) {
			reply.u32Success = 0;
			//update third task id --- codec task id
			muduo::Singleton<NodeInfo>::instance().updateThirdTaskIDCodecTaskid(
					openAudioData->u32TaskID, closeAudioTaskID);
		} else {
			LOG_WARN << "Send to codec return false in  Open audio func!";
			reply.u32Success = 1;
		}

	} else {
		//new open audio!
		id = getNewCodecTaskID(openAudioData->u32TaskID, _eAudioTask,
				reply.u32Success);
		LOG_INFO << "Open Audio third task ID: " << openAudioData->u32TaskID
				<< " codec ID: " << id;
		LOG_INFO << "Not find aochn: " << AOChnID;
		itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
				bind2nd(findAVDecInfoByCodecID(), id));
		if (itAudio != vAVDecInfo->end()) {
			memset(itAudio->stStream._rtsp.stRtspClient.au8Url, 0,
			DP_M2S_URL_LEN);
			strcpy((DP_CHAR*) itAudio->stStream._rtsp.stRtspClient.au8Url,
					(DP_CHAR*) openAudioData->au8RtspURL);
			if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				itAudio->stStream._rtsp.stRtspClient.s8Open = 1;
			LOG_INFO << "it->stStream._rtsp.stRtspClient.s8Open : "
					<< itAudio->stStream._rtsp.stRtspClient.s8Open
					<< " AOChnID " << AOChnID;
			itAudio->AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AO;
			itAudio->stAdec.enAlg = DP_M2S_ALG_AAC_DEC;
			itAudio->AvBindAttr.stAudio.stOut.u32DevId = AOChnID;
			sendBuff.retrieveAll();
			sendBuff.append(&setInfo, packageLen - stSize);
			sendBuff.append(&(*itAudio), stSize);

			if (sendAckToCodec(sendBuff.toStringPiece().data(),
					setInfo.stHeader.u16PacketLen, 0x01)) {
				reply.u32Success = 0;
			} else {
				LOG_WARN << "Send to codec return false in  Open audio func!";
				reply.u32Success = 1;
			}

			AODevCodecID->insert(
					NodeInfo::MapAODevIDCodecID::value_type(AOChnID, id));
		} else {
		}
	}

#if 0

	//////////////////////old
//get task id
	if (thirdCodecID->find(openAudioData->u32TaskID) != thirdCodecID->end())
	id = thirdCodecID->operator [](openAudioData->u32TaskID);
	else
	id = getNewCodecTaskID(openAudioData->u32TaskID, _eAudioTask,
			reply.u32Success);

	LOG_INFO << "Open Audio third task ID: " << openAudioData->u32TaskID
	<< " codec ID: " << id;
//full
	if (id == -2 || id == -3) {
		muduo::net::Buffer buff;
		buff.append(&reply, reply.header.u16PackageLen);
		connPtr->send(&buff);
		return;
	}

//	///shift audio
//	if (id == -1) {
//
//	}

//get avdec
	NodeInfo::VctrAVDECGetInfo::iterator it;
	if (id != -1) {
		it = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
				bind2nd(findAVDecInfoByCodecID(), id));
		if (it == vAVDecInfo->end()) {
			LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
			reply.u32Success = 1;
			muduo::net::Buffer buff;
			buff.append(&reply, reply.header.u16PackageLen);
			connPtr->send(&buff);
			return;
		}

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
		//id same     !=-1
		if (closeAudioTaskID == id) {
			LOG_INFO << "closeAudioTaskID: " << closeAudioTaskID << " == id : "
			<< id;
			reply.u32Success = 0;
			sendBuff.retrieveAll();
			sendBuff.append(&reply, reply.header.u16PackageLen);
			connPtr->send(&sendBuff);
			return;
		}

		//id == -1
		LOG_INFO << "codec id == -1?" << id;
		if (id == -1) {
			if (((itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
											bind2nd(findAVDecInfoByCodecID(), closeAudioTaskID)))
							!= vAVDecInfo->end())) {
				LOG_INFO << "Shift AO";
				if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				itAudio->stStream._rtsp.stRtspClient.s8Open = 0x00;
//			else if (itAudio->AvBindAttr.enBindType
//					== DP_M2S_AVBIND_ADEC2AO_VDEC2VO)
//				itAudio->stStream._rtsp.stRtspClient.s8Open = 0x02;
				sendBuff.retrieveAll();
				sendBuff.append(&setInfo, packageLen - stSize);
				sendBuff.append(&(*itAudio), stSize);
				it = itAudio;
				LOG_INFO << "Close and shift AO devID: "
				<< openAudioData->u8AoChnID << "with old codecID: "
				<< closeAudioTaskID;
				if (sendAckToCodec(sendBuff.toStringPiece().data(),
								setInfo.stHeader.u16PacketLen, 0x01)) {
//			it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
//					bind2nd(findThirdIDByCodecID(), closeAudioTaskID));
//			if (it_ID != thirdCodecID->end())
//				//remove audio codec
//				if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
//					muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
//							it_ID->first);
				} else {
					LOG_ERROR << "Send to codec return false in  shift audio !";
				}
			} else {
				LOG_ERROR << "Can not find closeAudioTaskID: "
				<< closeAudioTaskID << " in vAVDecInfo";
			}
			AODevCodecID->operator [](AOChnID) = closeAudioTaskID;
		} else {	//new  shift != -1

		}
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
		if (id == -1) {
			it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
					bind2nd(findThirdIDByCodecID(), closeAudioTaskID));
			if (it_ID != thirdCodecID->end()) {
				//shift audio codec
//				it_ID->first = openAudioData->u32TaskID;
				if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
						it_ID->first);
				thirdCodecID->insert(
						NodeInfo::MapOutThirdCodecTaskID::value_type(
								openAudioData->u32TaskID, closeAudioTaskID));
				LOG_INFO << "Insert third id : " << openAudioData->u32TaskID
				<< " with codec TaskID: " << closeAudioTaskID;
			}
		}

	} else {
		LOG_WARN << "Send to codec return false in  Open audio func!";
		reply.u32Success = 1;
	}
#endif
	muduo::Singleton<NodeInfo>::instance().updateMapOutThirdCodecTaskID(
			thirdCodecID);
	muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(AODevCodecID);
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	reply.u32Success = 0;
	sendBuff.retrieveAll();
	sendBuff.append(&reply, reply.header.u16PackageLen);
	LOG_INFO << "Reply success : " << reply.u32Success;
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
		LOG_INFO << "Reply success : " << reply.u32Success;
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
		LOG_INFO << "Reply success : " << reply.u32Success;
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
	if (id == -1 && type == _eAudioTask) {
		LOG_INFO << "Shift audio? codec id return -1.";
		return -1;
	}
	if (id == -2 || id == -3) {
		LOG_ERROR << "Error codec task ID: " << id << " third task ID: "
				<< thirdTaskID;
		reply = 1;
		return id;
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
	muduo::net::Buffer buffSend;
	if (VOInfo->empty()) {
		replyGetInfoToThird(Property_Get_OutputVideoChnInfo,
				DP_ERR_STATE_ABNORMAL, buffSend, connPtr);
		return;
	}
	DP_U32 voCount = VOInfo->size();
	NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsChInfo =
			muduo::Singleton<NodeInfo>::instance().getOutSWMSChCodecDecInfo();
	DP_U32 taskCount = swmsChInfo->size();
	DP_U32 propertyLen = sizeof(_sAllVoChnInfo)
			+ voCount * sizeof(_sSingleVoChnInfo)
			+ taskCount * sizeof(_sVideoTaskInfo);

	_sAllVoChnInfo allVOChInfo(voCount, taskCount);

	buffSend.append(&allVOChInfo, sizeof(_sAllVoChnInfo));
	NodeInfo::VctrAVENCGetInfoPtr AVEncInfo =
			muduo::Singleton<NodeInfo>::instance().getAVEncGetInfo();
	NodeInfo::VctrAVENCGetInfo::iterator iter_AVEnc;
	NodeInfo::MapMapAODevAudioInfoPtr aoAudioInfo =
			muduo::Singleton<NodeInfo>::instance().AODevAudioInfo();

	//append _sSingleVoChnInfo
	DP_U32 voRealCount = 0;
	for (NodeInfo::VctrVOGetInfo::iterator it = VOInfo->begin();
			it != VOInfo->end(); it++) {
		iter_AVEnc = find_if(AVEncInfo->begin(), AVEncInfo->end(),
				bind2nd(findEchoRtspURL<DP_M2S_AVENC_INFO_S>(), it->enDevId));

		if (it->enDevId == DP_M2S_VO_DEV_HDMI0_HI3536) {
			boost::shared_ptr<_sSingleVoChnInfo> singleVoInfo(
					new _sSingleVoChnInfo(it->bEnable, ID_VO_CHN_VIDEOOUT1,
					NULL, 0, ID_AO_CHN_VIDEOOUT1, 0, 0));
			if (iter_AVEnc != AVEncInfo->end()) {
				memset(singleVoInfo->au8PreviewRtspURL, 0, DP_URL_LEN);
				strcpy((DP_CHAR*) singleVoInfo->au8PreviewRtspURL,
						(DP_CHAR*) iter_AVEnc->stStream._rtsp.stRtspServer.au8Url);
				if (!aoAudioInfo->empty()) {
					singleVoInfo->u8AoChnMute = aoAudioInfo->operator [](
							DP_M2S_AO_DEV_HDMI0_HI3536).u8AoChnMute;
					singleVoInfo->u8AoChnVolume = aoAudioInfo->operator [](
							DP_M2S_AO_DEV_HDMI0_HI3536).u8AoChnVolume;
				} else {
					LOG_INFO << "aoAudioInfo is empty !";
				}
			} else {
				LOG_ERROR << "Can not find devid: " << it->enDevId
						<< " in avenc!";
			}
			buffSend.append(singleVoInfo.get(), sizeof(_sSingleVoChnInfo));
			voRealCount++;
			LOG_DEBUG << "singleVoInfo attr: u8VoChnOpenStatus: "
					<< singleVoInfo->u8VoChnOpenStatus << " VOchiD : "
					<< singleVoInfo->u8VoChnID << " URL : "
					<< singleVoInfo->au8PreviewRtspURL << " ao chid : "
					<< singleVoInfo->u8RelateAoChnID << " ao mute : "
					<< singleVoInfo->u8AoChnMute << " ao volume : "
					<< singleVoInfo->u8AoChnVolume;
		} else {
			LOG_ERROR << "Error dev id : " << it->enDevId;
			replyGetInfoToThird(Property_Get_OutputVideoChnInfo,
					DP_ERR_VO_ID_INVALID, buffSend, connPtr);
			return;
		}
	}
	/// judge vo count num voCount == voRealCount?
	if (voRealCount != voCount) {
		LOG_ERROR << "Send vo count conflicted voRealCount : " << voRealCount
				<< " voCount: " << voCount;
		replyGetInfoToThird(Property_Get_OutputVideoChnInfo, DP_ERR_VO_OPERATE,
				buffSend, connPtr);
		return;
	}

	//video task info //append pVideoTaskInfo
	boost::shared_ptr<_sVideoTaskInfo> videoInfo(new _sVideoTaskInfo);
	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();
	DP_U32 codecTaskID = 0;
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;
	LOG_DEBUG << "swmsChInfo size :: " << swmsChInfo->size();
	DP_U32 realVideoCount = 0;
	for (NodeInfo::MapOutSWMSChCodecDecInfo::iterator it = swmsChInfo->begin();
			it != swmsChInfo->end(); it++) {
		// without audio
		////////////////////////////////////////////////////////////////////////////////
		//			if (it->second.AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO){
		//				continue;
		//			}
		codecTaskID = it->second.s32TskId;
		it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
				bind2nd(findThirdIDByCodecID(), codecTaskID));
		if (it_ID != thirdCodecID->end()) {
			videoInfo->u32TaskID = it_ID->first;
			LOG_DEBUG << " third task ID: " << videoInfo->u32TaskID
					<< " codec task ID: " << codecTaskID;
		} else {
			LOG_ERROR << "Can not find third task id by codec task id: "
					<< codecTaskID;
			replyGetInfoToThird(Property_Get_OutputVideoChnInfo,
					DP_ERR_TASK_ID_NOTEXIST, buffSend, connPtr);
			return;
		}
		if (it->second.stVdec.bSwms) {
			if (it->second.stVdec.stSwms.enVoDevId
					== DP_M2S_VO_DEV_HDMI0_HI3536) {
				videoInfo->u8VoChnID = ID_VO_CHN_VIDEOOUT1;
			} else {
				LOG_ERROR << "Error dev id : "
						<< it->second.stVdec.stSwms.enVoDevId;
				replyGetInfoToThird(Property_Get_OutputVideoChnInfo,
						DP_ERR_VO_ID_INVALID, buffSend, connPtr);
				return;
			}
			videoInfo->u8WinZIndex = it->second.stVdec.stSwms.u32Priority;
			videoInfo->dstVideoInfo.u16StartX =
					it->second.stVdec.stSwms.stRect.s32X;
			videoInfo->dstVideoInfo.u16StartY =
					it->second.stVdec.stSwms.stRect.s32Y;
			videoInfo->dstVideoInfo.u16VideoHeight =
					it->second.stVdec.stSwms.stRect.u32Height;
			videoInfo->dstVideoInfo.u16VideoWidth =
					it->second.stVdec.stSwms.stRect.u32Width;
			LOG_INFO << "Dst video info:: x: y: W: H: "
					<< videoInfo->dstVideoInfo.u16StartX << " "
					<< videoInfo->dstVideoInfo.u16StartY << " "
					<< videoInfo->dstVideoInfo.u16VideoWidth << " "
					<< videoInfo->dstVideoInfo.u16VideoHeight;
		}

		memset(videoInfo->au8InputRtspURL, 0, DP_URL_LEN);
		strcpy((DP_CHAR*) videoInfo->au8InputRtspURL,
				(DP_CHAR*) it->second.stStream._rtsp.stRtspClient.au8Url);
		LOG_INFO << "videoInfo->au8InputRtspURL: " << videoInfo->au8InputRtspURL
				<< " from : " << it->second.stStream._rtsp.stRtspClient.au8Url;

		NodeInfo::MapThirdIDSrcVideoInfoPtr srcVideoInfo = muduo::Singleton<
				NodeInfo>::instance().getThirdIDSrcVideoInfo();

		if (!srcVideoInfo->empty()) {
			if (NodeInfo::MapThirdIDSrcVideoInfo::iterator iter =
					srcVideoInfo->find(videoInfo->u32TaskID)
							!= srcVideoInfo->end()) {
				videoInfo->srcVideoInfo = iter->second;
				//jhbnote error src info occasionally
				LOG_INFO << "srcVideoInfo size: " << srcVideoInfo->size()
						<< "src video width : "
						<< videoInfo->srcVideoInfo.u16VideoWidth
						<< " infoHeight: "
						<< videoInfo->srcVideoInfo.u16VideoHeight;
				LOG_INFO << "src video start: x: y: end: x: y:  "
						<< videoInfo->srcVideoInfo.u16StartX << " "
						<< videoInfo->srcVideoInfo.u16StartY << " "
						<< videoInfo->srcVideoInfo.u16EndX << " "
						<< videoInfo->srcVideoInfo.u16EndY;
			}
		}
		buffSend.append(videoInfo.get(), sizeof(_sVideoTaskInfo));
		realVideoCount++;
	}

	if (realVideoCount != taskCount) {
		LOG_ERROR << "Send video task count conflicted realVideoCount : "
				<< realVideoCount << " taskCount: " << taskCount;
		replyGetInfoToThird(Property_Get_OutputVideoChnInfo,
				DP_ERR_TASK_OPERATE, buffSend, connPtr);
		return;
	}

	replyGetInfoToThird(Property_Get_OutputVideoChnInfo, 0, buffSend, connPtr);
}

void LogicHandle::Get_OutputAudioChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
	LOG_INFO << "Get 'Property_Get_OutputAudioChnInfo' cmd from "
			<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrAOGetInfoPtr AOInfo =
			muduo::Singleton<NodeInfo>::instance().getAOGetInfo();
	muduo::net::Buffer buffSend;
	if (AOInfo->empty()) {
		replyGetInfoToThird(Property_Get_OutputAudioChnInfo,
				DP_ERR_STATE_ABNORMAL, buffSend, connPtr);
		return;
	}
	DP_U32 audioCount = AOInfo->size();
	DP_U32 realCount = 0;

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

	eDeviceAudioChannelID AOChnID = 0;
	DP_U8 audioIn = 0;
	boost::shared_ptr<_sAudioTaskInfo> audioTask(new _sAudioTaskInfo());

	buffSend.append(&allAOChnInfo, sizeof(_sAllAoChnInfo));
//	DP_BOOL success = DP_TRUE;
	for (NodeInfo::VctrAOGetInfo::iterator it = AOInfo->begin();
			it != AOInfo->end(); it++) {

		if (it->enDevId == DP_M2S_AO_DEV_LINEOUT0_HI3536) {
			AOChnID = ID_AO_CHN_LINEOUT1;
		} else if (it->enDevId == DP_M2S_AO_DEV_HDMI0_HI3536) {
			AOChnID = ID_AO_CHN_VIDEOOUT1;
		} else {
			LOG_ERROR << "Error dev id : " << it->enDevId;
			replyGetInfoToThird(Property_Get_OutputAudioChnInfo,
					DP_ERR_AO_ID_INVALID, buffSend, connPtr);
			return;
		}
		boost::shared_ptr<_sAllAoChnInfo::_sSingleAoChnInfo> singleAO(
				new _sAllAoChnInfo::_sSingleAoChnInfo(0x00, AOChnID, it->u8Vol,
						0x00, *audioTask.get()));

		if (AODEVCodecID->find(it->enDevId) == AODEVCodecID->end()) {
			//jhbnote when thinking about uninit condition.
			LOG_ERROR << "Can not find devID: " << it->enDevId << " in prog.";
			buffSend.append(singleAO.get(),
					sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
			realCount++;
//			success = DP_FALSE;
//			continue;
		} else {
			codecID = AODEVCodecID->operator [](it->enDevId);

			NodeInfo::VctrAVDECGetInfo::iterator itAVDec = find_if(
					vAVDecInfo->begin(), vAVDecInfo->end(),
					std::bind2nd(findAVDecInfoByCodecID(), codecID));

			if (itAVDec == vAVDecInfo->end()) {
				LOG_ERROR << "Can not find avDec by codec id : " << codecID
						<< " in prog.";
				buffSend.append(singleAO.get(),
						sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
				realCount++;
				//			success = DP_FALSE;
//				continue;
			} else {
//				if (itAVDec->AvBindAttr.enBindType != DP_M2S_AVBIND_VDEC2VO) {
//
//					audioIn = 0;
//				} else {
				audioIn = 0x01;
				//jhbnote volume : dev's or chn's?
				it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
						bind2nd(findThirdIDByCodecID(), codecID));
				if (it_ID == thirdCodecID->end()) {
					LOG_ERROR << "Can not find third task id  by codec id : "
							<< codecID << " in prog.";
					buffSend.append(singleAO.get(),
							sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
					realCount++;
					//				success = DP_FALSE;
//						continue;
				} else {
					_sSrcAudioInfo audioInfo(SampleRate_AUDIO_44K, 0x10, 1,
							AUDIO_ENCODE_AAC);
					audioTask.reset(
							new _sAudioTaskInfo(it_ID->first, 0x01,
									itAVDec->stStream._rtsp.stRtspClient.au8Url,
									audioInfo, AOChnID));
					boost::shared_ptr<_sAllAoChnInfo::_sSingleAoChnInfo> singleAO(
							new _sAllAoChnInfo::_sSingleAoChnInfo(0x01, AOChnID,
									it->u8Vol, audioIn, *audioTask.get()));
					buffSend.append(singleAO.get(),
							sizeof(_sAllAoChnInfo::_sSingleAoChnInfo));
					realCount++;
				}
			}
//			}
		}

//		if (success == DP_FALSE) {
//			DP_U32 successResult = 1;
//			memcpy(
//					const_cast<DP_CHAR*>(buffSend.toStringPiece().data())
//							+ sizeof(_sRemote_Header) + DP_DEV_ID_LEN
//							+ sizeof(eRemotePropertyName), &successResult,
//					sizeof(DP_U32));
//		}
//		LOG_INFO << "Get info send to remote " << buffSend.readableBytes()
//				<< " bytes !";
//		connPtr->send(&buffSend);
	}

	if (realCount != audioCount) {
		LOG_ERROR << "Send audio task count conflicted realCount : "
				<< realCount << " audioCount: " << audioCount;
		replyGetInfoToThird(Property_Get_OutputAudioChnInfo,
				DP_ERR_TASK_OPERATE, buffSend, connPtr);
		return;
	} else {
		replyGetInfoToThird(Property_Get_OutputAudioChnInfo, 0, buffSend,
				connPtr);
	}
}

void LogicHandle::commonReplyToThird(eRemoteCommand cmd, DP_U32 taskID,
		DP_U32 success, const muduo::net::TcpConnectionPtr connPtr) {
	_sRemote_Header head(
			muduo::Singleton<NodeInfo>::instance().getNetInfo().ip2U32(),
			g_DevType, 0x01, sizeof(Remote_Common_Reply_S), cmd,
			DP_DEV_ID_LEN + sizeof(DP_U32) * 2);
	Remote_Common_Reply_S reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			taskID, success);
	muduo::net::Buffer sendBuff;
	sendBuff.append(&reply, reply.header.u16PackageLen);
	connPtr->send(&sendBuff);
}

void LogicHandle::replyGetInfoToThird(eRemotePropertyName proterty,
		DP_U32 success, muduo::net::Buffer buffSend,
		const muduo::net::TcpConnectionPtr connPtr) {

	DP_U32 propertyLen = buffSend.readableBytes();
	DP_U32 cmdLen = DP_DEV_ID_LEN + sizeof(eRemotePropertyName) + sizeof(DP_U32)
			+ sizeof(DP_U16) + propertyLen;
	DP_U32 packageLen = sizeof(_sRemote_Header) + cmdLen;
//	LOG_INFO << "VOInfo->size()  : " << VOInfo->size() << " taskCount　: "
//			<< taskCount << " propertyLen : " << propertyLen << " cmdLen : "
//			<< cmdLen << " packageLen: " << packageLen;

	_sRemote_Reply_GetInfo replyGetInfo(
			muduo::Singleton<NodeInfo>::instance().getNetInfo().ip2U32(),
			g_DevType, 0x01, packageLen, Command_GetInfo, cmdLen,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			Property_Get_OutputVideoChnInfo, success, propertyLen);
	muduo::net::Buffer buff;
	buff.append(&replyGetInfo, packageLen - propertyLen);
	if (success == 0) {
		buff.append(buffSend.toStringPiece());
	}
	LOG_INFO << "Get info send to remote: " << buff.readableBytes()
			<< " bytes !";
	connPtr->send(&buff);
}

DP_U8 LogicHandle::DP_MediaClient_CheckCropDateIsIegitimate(
		DP_M2S_CROP_ATTR_S crop, DP_M2S_VIDEO_SYNC_E enSync) {
	DP_U32 width = 0, height = 0;

	if (enSync == DP_M2S_VIDEO_SYNC_720P60) {
		width = 1280;
		height = 720;
	} else if (enSync == DP_M2S_VIDEO_SYNC_1080P60) {
		width = 1920;
		height = 1080;
	} else if (enSync == DP_M2S_VIDEO_SYNC_4K30) {
		width = 3840;
		height = 2160;
	} else {
		LOG_ERROR << "crop param check is error,sync: " << enSync
				<< " is not support!";
		return DP_FALSE;
	}

	if (crop.s32X % 2 != 0 || crop.s32Y % 2 != 0) {
		LOG_ERROR << "crop param check is error,x(: " << crop.s32X
				<< ") %2 !=0 y(" << crop.s32Y << ") %2 !=0";
		return DP_FALSE;
	}
	if (crop.u32Width % 2 != 0 || crop.u32Height % 4 != 0) {
		LOG_ERROR << "crop param check is error,w( " << crop.u32Width
				<< ") %2 !=0 h(" << crop.u32Height << ") %4 !=0";
		return DP_FALSE;
	}
	if (crop.u32Width < DP_M2S_H264D_MIN_WIDTH
			|| crop.u32Width > DP_M2S_H264D_MAX_WIDTH) {
		LOG_ERROR
				<< "crop param check is error,u32Width(%d) < %d || u32Width(%d)>%d "
				<< crop.u32Width << " " << DP_M2S_H264D_MIN_WIDTH << " "
				<< crop.u32Width << " " << DP_M2S_H264D_MAX_WIDTH;
		return DP_FALSE;
	}
	if (crop.u32Height < DP_M2S_H264D_MIN_HEIGHT
			|| crop.u32Height > DP_M2S_H264D_MAX_HEIGHT) {
		LOG_ERROR
				<< "crop param check is error,u32Height(%d) < %d || u32Height(%d)>%d "
				<< crop.u32Height << " " << DP_M2S_H264D_MIN_HEIGHT << " "
				<< crop.u32Height << " " << DP_M2S_H264D_MAX_HEIGHT;
		return DP_FALSE;
	}
	if (crop.u32Width < (width / 16)) {
		LOG_ERROR << "crop param check is error,u32Width(%d) < %d "
				<< crop.u32Width << " " << (width / 16);
		return DP_FALSE;
	}
	if (crop.u32Height < (height / 16)) {
		LOG_ERROR << "crop param check is error,u32Height(%d) < %d "
				<< crop.u32Height << " " << (height / 16);
		return DP_FALSE;
	}
	return DP_TRUE;
}
