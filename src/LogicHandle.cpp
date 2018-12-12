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
	_sRemote_CreateWindow *createWinData =
			(_sRemote_CreateWindow*) data.c_str();
	LOG_WARN << "CreateWindow cmd...u32RequestID: "
			<< createWinData->header.u32RequestID;
	DP_U32 success = 0;
	do {
		//get task id
		DP_S32 id = 0;
		if (createWinData->u8AudioIn == 0x00) {
			id = getNewCodecTaskID(createWinData->u32TaskID, _eVideoTask);
		} else if (createWinData->u8AudioIn == 0x01)
			/////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			id = getNewCodecTaskID(createWinData->u32TaskID,
					_eAudioAndVideoTask);
		else {
			success = DP_ERR_TASK_PARAM_ILLEGAL;
			break;
		}

		LOG_INFO << "Create win third task ID: " << createWinData->u32TaskID
				<< " codec ID: " << id;
		if (id > 1792) {
			success = id;
			break;
		}

		//get avdec
		NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
				muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
		NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
				vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
		if (it == vAVDecInfo->end()) {
			success = DP_ERR_TASK_ID_NOTEXIST;
			LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
			//jhbnote !! if break in process ,free source such as codec task id  !
			break;
		}

		it->stStream.enType = DP_M2S_STREAM_RTSP_CLIENT;
		it->stStream._rtsp.stRtspClient.bMulticast = DP_FALSE;
		it->stStream._rtsp.stRtspClient.bUDP = DP_FALSE;
		if (createWinData->u8VoChnID == ID_VO_CHN_VIDEOOUT1) {
			it->AvBindAttr.stVideo.stOut.u32DevId = DP_M2S_VO_DEV_HDMI0_HI3536;
		} else {
			LOG_ERROR << "Error VO devID : " << createWinData->u8VoChnID;
			success = DP_ERR_VO_ID_INVALID;
			break;
		}

		it->stStream._rtsp.stRtspClient.s8Open = 0x02;

		memset(it->stStream._rtsp.stRtspClient.au8Url, 0, DP_M2S_URL_LEN);
		strcpy((DP_CHAR*) it->stStream._rtsp.stRtspClient.au8Url,
				(DP_CHAR*) createWinData->au8RtspURL);

		LOG_INFO << " createWinData->au8RtspURL: " << createWinData->au8RtspURL
				<< " it->stStream._rtsp.stRtspClient.au8Url: "
				<< it->stStream._rtsp.stRtspClient.au8Url;

		//swms
		DP_M2S_ALG_ATTR_S alg;
		alg.enAlg = DP_M2S_ALG_H264_DEC;
		DP_M2S_RECT_S rect;
		rect.s32X = createWinData->dstVideoInfo.u16StartX;
		rect.s32Y = createWinData->dstVideoInfo.u16StartY;
		rect.u32Height = createWinData->dstVideoInfo.u16VideoHeight;
		rect.u32Width = createWinData->dstVideoInfo.u16VideoWidth;
		it->stVdec.bSwms = DP_TRUE;
		it->stVdec.stSwms.stRect = rect;
		LOG_INFO << "rect x: y: u32Height: u32Width: " << rect.s32X << " "
				<< rect.s32Y << " " << rect.u32Height << " " << rect.u32Width;

		//priority handle
		NodeInfo::VctrWindowPriorityPtr winPriority =
				muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();
		DP_U32 newPriority = 0;
		if (!winPriority->empty()) {
			newPriority = winPriority->back() + 1;
		} else {
			winPriority->push_back(newPriority);
		}
		sort(winPriority->begin(), winPriority->end());
		for (NodeInfo::VctrWindowPriority::iterator itP = winPriority->begin();
				itP != winPriority->end(); itP++)
			LOG_INFO << "New win priority: " << *itP;

		muduo::Singleton<NodeInfo>::instance().updateVctrWindowPriority(
				winPriority);
		it->stVdec.stSwms.u32Priority = newPriority;
		LOG_INFO << "Priority current create win task : " << newPriority;

		//crop -- src video
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
		muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
		//useless
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
		//useless
#if 0 // useless
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

		NodeInfo::printAVDEC(&(*it));
		DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
				DP_M2S_CMD_AVDEC_SETINFO_S>((DP_M2S_AVDEC_INFO_S) (*it),
				g_NeedReply, DP_M2S_CMD_AVDEC_SET);

		if (ret != 0) {
			LOG_ERROR << "Send to codec return false in creating win func!";
			success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
			break;
		}

		//src video info
		NodeInfo::MapThirdIDSrcVideoInfoPtr thirdIDSrcVideo = muduo::Singleton<
				NodeInfo>::instance().getThirdIDSrcVideoInfo();
		thirdIDSrcVideo->insert(
				NodeInfo::MapThirdIDSrcVideoInfo::value_type(
						createWinData->u32TaskID, createWinData->srcVideoInfo));
		muduo::Singleton<NodeInfo>::instance().updateMapThirdIDSrcVideoInfo(
				thirdIDSrcVideo);

		// SWMS chn  num -->> avdec
		NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
				NodeInfo>::instance().getOutSWMSChCodecDecInfo();
		swmsDecInfo->insert(
				NodeInfo::MapOutSWMSChCodecDecInfo::value_type(
						it->stVdec.stSwms.u32SwmsChn, *it));
		muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
				swmsDecInfo);
	} while (0);
	commonReplyToThird(Command_CreateWindow, createWinData->u32TaskID, success,
			connPtr);
	return;
}

void LogicHandle::moveWindow(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	_sRemote_MoveWindow *moveWinData = (_sRemote_MoveWindow*) data.c_str();
	LOG_WARN << "Move win cmd.....u32RequestID: "
			<< moveWinData->header.u32RequestID;

	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			moveWinData->u32TaskID);

	DP_U32 success = 0;
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	do {
		if (id == DP_ERR_TASK_ID_NOTEXIST) {
			LOG_WARN << "Did not find a codec task ID by third task ID :"
					<< moveWinData->u32TaskID << " codec ID: " << id;
			success = DP_ERR_TASK_ID_NOTEXIST;
			break;
		} else {
			LOG_INFO << "codec id in move win: " << id;
		}

		NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
				vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
		if (it == vAVDecInfo->end()) {
			LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
			success = DP_ERR_TASK_ID_NOTEXIST;
			break;
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

		LOG_INFO << "corp start x: y: end x: y: hei: Wid: "
				<< srcVideo.u16StartX << " " << srcVideo.u16StartY << " "
				<< srcVideo.u16EndX << " " << srcVideo.u16EndY << " "
				<< srcVideo.u16VideoHeight << " " << srcVideo.u16VideoWidth;
		LOG_INFO << "corp width: height: " << crop.u32Width << " "
				<< crop.u32Height;

		it->stVdec.bCrop = DP_TRUE;
		it->stVdec.stCrop = crop;
		it->stVdec.stSwms.stRect = rect;

//		NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
//				NodeInfo>::instance().getOutSWMSChCodecDecInfo();
//		swmsDecInfo->operator [](it->stVdec.stSwms.u32SwmsChn) = *it;
//		muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
//				swmsDecInfo);

		//priority
		DP_U32 originPriority = it->stVdec.stSwms.u32Priority;
		LOG_INFO << "priority current move task : " << originPriority;
		NodeInfo::VctrWindowPriorityPtr winPriority =
				muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();

		if (!winPriority->empty()) {
			it->stVdec.stSwms.u32Priority = winPriority->back();
		} else {
			success = DP_ERR_STATE_ABNORMAL;
			break;
		}

//		NodeInfo::printAVDEC(&(*it));

		DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
				DP_M2S_CMD_AVDEC_SETINFO_S>((DP_M2S_AVDEC_INFO_S) (*it),
				g_NeedReply, DP_M2S_CMD_AVDEC_SET);

		if (ret != 0) {
			LOG_ERROR << "Send to codec return false in moving win func!";
			success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
			break;
		}

		//modify all avdec priority
		// jhbnote test real data or not?
		for (NodeInfo::VctrAVDECGetInfo::iterator itP = vAVDecInfo->begin();
				itP != vAVDecInfo->end(); itP++) {
			if (itP->stVdec.bSwms == DP_TRUE
					&& itP->stVdec.stSwms.u32Priority >= originPriority
					&& itP != it) {
				itP->stVdec.stSwms.u32Priority -= 1;
				//jhbnote test send only instead of waiting recv.
				NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
						DP_M2S_CMD_AVDEC_SETINFO_S>(
						(DP_M2S_AVDEC_INFO_S) (*itP), g_NoNeedReply,
						DP_M2S_CMD_AVDEC_SET);
			}
		}
		//src
		NodeInfo::MapThirdIDSrcVideoInfoPtr thirdIDSrcInfo = muduo::Singleton<
				NodeInfo>::instance().getThirdIDSrcVideoInfo();
		thirdIDSrcInfo->operator [](moveWinData->u32TaskID) =
				moveWinData->srcVideoInfo;
		muduo::Singleton<NodeInfo>::instance().updateMapThirdIDSrcVideoInfo(
				thirdIDSrcInfo);

		// SWMS chn  num -->> avdec
		NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
				NodeInfo>::instance().getOutSWMSChCodecDecInfo();
		swmsDecInfo->operator [](it->stVdec.stSwms.u32SwmsChn) = *it;
		muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
				swmsDecInfo);
	} while (0);
	commonReplyToThird(Command_MoveWindow, moveWinData->u32TaskID, success,
			connPtr);
	//update avdec
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
}

void LogicHandle::closeWindow(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	//jhbnote erase src video info with third task id && swms chn with dec(clear task)
	_sRemote_CloseWindow *closeWinData = (_sRemote_CloseWindow*) data.c_str();
	LOG_WARN << "Close win cmd..........u32RequestID: "
			<< closeWinData->hedader.u32RequestID;
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			closeWinData->u32TaskID);
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	DP_U32 success = 0;
	do {
		if (id == DP_ERR_TASK_ID_NOTEXIST) {
			LOG_WARN << "Did not find a codec task ID by third task ID :"
					<< closeWinData->u32TaskID << " codec ID: " << id;
			success = DP_ERR_TASK_ID_NOTEXIST;
			break;
		} else {
			LOG_INFO << "codec id in close win: " << id;
		}

		//jhbnote check this func
		muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
				closeWinData->u32TaskID);

		NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
				vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
		if (it == vAVDecInfo->end()) {
			LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
			success = DP_ERR_TASK_ID_NOTEXIST;
			break;
		}

		it->stStream._rtsp.stRtspClient.s8Open = 0;
		it->stVdec.bSwms = DP_FALSE;

		DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
				DP_M2S_CMD_AVDEC_SETINFO_S>((DP_M2S_AVDEC_INFO_S) (*it),
				g_NeedReply, DP_M2S_CMD_AVDEC_SET);

		if (ret != 0) {
			LOG_ERROR << "Send to codec return false in close win func!";
			success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
			break;
		}

		//erase swms in dec
		NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
				NodeInfo>::instance().getOutSWMSChCodecDecInfo();
		swmsDecInfo->erase(it->stVdec.stSwms.u32SwmsChn);
		muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
				swmsDecInfo);

		//priority
		NodeInfo::VctrWindowPriorityPtr winPriority =
				muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();

		DP_U32 originPriority = it->stVdec.stSwms.u32Priority;
		LOG_INFO << "Priority current close task : " << originPriority;

		if (!winPriority->empty()) {
			winPriority->pop_back();
			if (!winPriority->empty())
				sort(winPriority->begin(), winPriority->end());
		}
		muduo::Singleton<NodeInfo>::instance().updateVctrWindowPriority(
				winPriority);

		//modify all avdec priority
		for (NodeInfo::VctrAVDECGetInfo::iterator itP = vAVDecInfo->begin();
				itP != vAVDecInfo->end(); itP++) {
			if (itP->stVdec.bSwms == DP_TRUE
					&& itP->stVdec.stSwms.u32Priority >= originPriority
					&& itP != it) {
				itP->stVdec.stSwms.u32Priority -= 1;
				NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
						DP_M2S_CMD_AVDEC_SETINFO_S>(
						(DP_M2S_AVDEC_INFO_S) (*itP), g_NoNeedReply,
						DP_M2S_CMD_AVDEC_SET);
			}
		}
	} while (0);
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	commonReplyToThird(Command_CloseWindow, closeWinData->u32TaskID, success,
			connPtr);
}

void LogicHandle::clearAllTask(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
	_sRemote_ClearTask *clearData = (_sRemote_ClearTask*) data.c_str();
	LOG_WARN << "Clear all task cmd..........u32RequestID: "
			<< clearData->header.u32RequestID;

	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	NodeInfo::VctrAVDECGetInfo::iterator it;

	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();
	NodeInfo::MapOutThirdCodecTaskID::iterator it_ID;

	NodeInfo::VecCodecTaskID vTaskID;
	NodeInfo::VecErrInfo errThirdiD;
	NodeInfo::VecErrInfo errInfo;
	NodeInfo::VecErrInfo swmsChn;

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

		} else {
			continue;
		}
		it->stStream._rtsp.stRtspClient.s8Open = 0x00;
		// batch close avdec
		vTaskID.push_back(it->s32TskId);
		it_ID = std::find_if(thirdCodecID->begin(), thirdCodecID->end(),
				bind2nd(findThirdIDByCodecID(), it->s32TskId));
		if (it_ID != thirdCodecID->end()) {
			errThirdiD.push_back(it_ID->first);
			muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
					it_ID->first);
		} else {
			LOG_ERROR << "Can not find codec task id: " << it_ID->first
					<< " in thirdCodecID.";
			replyClearToThird(DP_ERR_STATE_ABNORMAL, errThirdiD, errInfo,
					connPtr);
			return;
		}
		swmsChn.push_back(it->stVdec.stSwms.u32SwmsChn);

//		//priority
//		NodeInfo::VctrWindowPriorityPtr winPriority =
//				muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();
//		if (!winPriority->empty()) {
//			winPriority->pop_back();
//			if (!winPriority->empty())
//				sort(winPriority->begin(), winPriority->end());
//		}
//		muduo::Singleton<NodeInfo>::instance().updateVctrWindowPriority(
//				winPriority);
		LOG_DEBUG << "Clear third task id : " << it_ID->first
				<< " with codec id : " << it->s32TskId;
	}

	LOG_WARN << "Clear task over !";

	DP_U32 retSetting = NodeInfo::batchSetting(DP_M2S_CMD_AVDEC_CLOSE_BATCH,
			vTaskID, errInfo);
	replyClearToThird(retSetting, errThirdiD, errInfo, connPtr);
	if (!swmsChn.empty()) {
		NodeInfo::MapOutSWMSChCodecDecInfoPtr swmsDecInfo = muduo::Singleton<
				NodeInfo>::instance().getOutSWMSChCodecDecInfo();
		for (NodeInfo::VecErrInfo::iterator iter_swms = swmsChn.begin();
				iter_swms != swmsChn.end(); iter_swms++) {
			if (swmsDecInfo->find(*iter_swms) != swmsDecInfo->end()) {
				swmsDecInfo->erase(*iter_swms);
			} else {
				LOG_ERROR << "Can not find swmschn: "
						<< it->stVdec.stSwms.u32SwmsChn << " in map.";
				return;
			}
		}
		muduo::Singleton<NodeInfo>::instance().updateMapOutSWMSChCodecDecInfo(
				swmsDecInfo);
	}
	NodeInfo::VctrWindowPriorityPtr winPriority =
			muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();
	if (!winPriority->empty()) {
		winPriority->clear();
		muduo::Singleton<NodeInfo>::instance().updateVctrWindowPriority(
				winPriority);
	}
	muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
}

void LogicHandle::openAudio(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
#if (OutputDevice)
	_sRemote_OpenAudio *openAudioData = (_sRemote_OpenAudio*) data.c_str();
	LOG_WARN << "OpenAudio cmd...u32RequestID: "
			<< openAudioData->header.u32RequestID;

	DP_S32 success = 0;
	DP_S32 id = 0;
//check is shift audio or not
	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();
	DP_BOOL thirdIDExisted = DP_FALSE;
	do {
		if (thirdCodecID->find(openAudioData->u32TaskID)
				!= thirdCodecID->end()) {
			thirdIDExisted = DP_TRUE;
			id = thirdCodecID->operator [](openAudioData->u32TaskID);
			LOG_INFO << "Find third task ID: " << openAudioData->u32TaskID
					<< " with codec id: " << id;
			break;
		}

		NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
				muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
		DP_M2S_AO_DEV_E AOChnID;

		if (openAudioData->u8AoChnID == ID_AO_CHN_VIDEOOUT1) {
			AOChnID = DP_M2S_AO_DEV_HDMI0_HI3536;
		} else if (openAudioData->u8AoChnID == ID_AO_CHN_LINEOUT1) {
			AOChnID = DP_M2S_AO_DEV_LINEOUT0_HI3536;
		}
		LOG_INFO << "Open Audio on chn : " << AOChnID
				<< " with thirdopen audio ch : " << openAudioData->u8AoChnID;

		DP_S32 closeAudioTaskID = 0;
//shift or open audio
		NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
				muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
		NodeInfo::VctrAVDECGetInfo::iterator itAudio;
		if (AODevCodecID->find(AOChnID) != AODevCodecID->end()) {
			//shift audio!
			closeAudioTaskID = AODevCodecID->operator [](AOChnID);
			// new third task id
			if (((itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
					bind2nd(findAVDecInfoByCodecID(), closeAudioTaskID)))
					!= vAVDecInfo->end())) {
				itAudio->stStream._rtsp.stRtspClient.s8Open = 0x00;
				LOG_INFO << "Close and shift AO devID: " << AOChnID
						<< "with old codecID: " << closeAudioTaskID;

				DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<
						DP_M2S_AVDEC_INFO_S, DP_M2S_CMD_AVDEC_SETINFO_S>(
						(DP_M2S_AVDEC_INFO_S) (*itAudio), g_NeedReply,
						DP_M2S_CMD_AVDEC_SET);
				if (ret != 0) {
					LOG_ERROR
							<< "Send to codec return false in shift audio func!";
					success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
					break;
				}
			} else {
				LOG_ERROR << "Can not find closeAudioTaskID: "
						<< closeAudioTaskID << " in vAVDecInfo";
				success = DP_ERR_TASK_ID_NOTEXIST;
				break;
			}
			memset(itAudio->stStream._rtsp.stRtspClient.au8Url, 0,
			DP_M2S_URL_LEN);
			strcpy((DP_CHAR*) itAudio->stStream._rtsp.stRtspClient.au8Url,
					(DP_CHAR*) openAudioData->au8RtspURL);
			if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
				itAudio->stStream._rtsp.stRtspClient.s8Open = 1;
			itAudio->AvBindAttr.stAudio.stOut.u32DevId = AOChnID;

			DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
					DP_M2S_CMD_AVDEC_SETINFO_S>(
					(DP_M2S_AVDEC_INFO_S) (*itAudio), g_NeedReply,
					DP_M2S_CMD_AVDEC_SET);
			if (ret != 0) {
				LOG_ERROR << "Send to codec return false in  Open audio func!";
				success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
				break;
			} else {
				//update third task id --- codec task id
				muduo::Singleton<NodeInfo>::instance().updateThirdTaskIDCodecTaskid(
						openAudioData->u32TaskID, closeAudioTaskID);
			}
		} else {
			//new open audio!
			id = getNewCodecTaskID(openAudioData->u32TaskID, _eAudioTask);
			LOG_INFO << "Open Audio third task ID: " << openAudioData->u32TaskID
					<< " codec ID: " << id;
			if (id > 1792) {
				success = id;
				break;
			}
			itAudio = find_if(vAVDecInfo->begin(), vAVDecInfo->end(),
					bind2nd(findAVDecInfoByCodecID(), id));
			if (itAudio != vAVDecInfo->end()) {
				memset(itAudio->stStream._rtsp.stRtspClient.au8Url, 0,
				DP_M2S_URL_LEN);
				strcpy((DP_CHAR*) itAudio->stStream._rtsp.stRtspClient.au8Url,
						(DP_CHAR*) openAudioData->au8RtspURL);
				if (itAudio->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
					itAudio->stStream._rtsp.stRtspClient.s8Open = 1;
				itAudio->AvBindAttr.stAudio.stOut.u32DevId = AOChnID;
				DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<
						DP_M2S_AVDEC_INFO_S, DP_M2S_CMD_AVDEC_SETINFO_S>(
						(DP_M2S_AVDEC_INFO_S) (*itAudio), g_NeedReply,
						DP_M2S_CMD_AVDEC_SET);
				if (ret != 0) {
					LOG_ERROR
							<< "Send to codec return false in  Open new audio func!";
					success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
					break;
				} else {
				}
				AODevCodecID->insert(
						NodeInfo::MapAODevIDCodecID::value_type(AOChnID, id));
				muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(
						AODevCodecID);
			} else {
				LOG_ERROR << "Not found id : " << id << " in avdec!";
				success = DP_ERR_TASK_ID_NOTEXIST;
				break;
			}
		}

//jhbnote not be taken effect without update
		muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
	} while (0);
	commonReplyToThird(Command_OpenAudio, openAudioData->u32TaskID, success,
			connPtr);
#endif
}

void LogicHandle::closeAudio(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
#if (OutputDevice)
	_sRemote_CloseAudio *closeAudio = (_sRemote_CloseAudio*) data.c_str();
	LOG_WARN << "CloseAudio cmd...u32RequestID: "
			<< closeAudio->header.u32RequestID;
	DP_U32 id = muduo::Singleton<NodeInfo>::instance().setServerTaskID(
			closeAudio->u32TaskID);
//get avdec
	NodeInfo::VctrAVDECGetInfoPtr vAVDecInfo =
			muduo::Singleton<NodeInfo>::instance().getAVDecGetInfo();
	DP_S32 success = 0;
	do {
		if (id == DP_ERR_TASK_ID_NOTEXIST) {
			LOG_ERROR << "Did not find a codec task ID by third task ID :"
					<< closeAudio->u32TaskID << " codec ID: " << id;
			success = DP_ERR_TASK_ID_NOTEXIST;
			break;
		} else {
			LOG_INFO << "Codec id in close audio: " << id;
		}

		NodeInfo::VctrAVDECGetInfo::iterator it = find_if(vAVDecInfo->begin(),
				vAVDecInfo->end(), bind2nd(findAVDecInfoByCodecID(), id));
		if (it == vAVDecInfo->end()) {
			LOG_ERROR << "Can not find a AVDec info by codec task ID: " << id;
			success = DP_ERR_TASK_ID_NOTEXIST;
			break;
		}

//remove audio codec id
		if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO)
			muduo::Singleton<NodeInfo>::instance().removeCodecTaskID(
					closeAudio->u32TaskID);

		if (it->AvBindAttr.enBindType == DP_M2S_AVBIND_ADEC2AO_VDEC2VO)
			it->stStream._rtsp.stRtspClient.s8Open = 0x02;

		DP_S32 ret = NodeInfo::sendCodecAVEncDecInfo<DP_M2S_AVDEC_INFO_S,
				DP_M2S_CMD_AVDEC_SETINFO_S>((DP_M2S_AVDEC_INFO_S) (*it),
				g_NeedReply, DP_M2S_CMD_AVDEC_SET);
		if (ret != 0) {
			LOG_ERROR << "Send to codec return false in close audio func!";
			success = DP_ERR_COMMUNICATE_ABNORMAL_INNER;
			break;
		} else {
		}
		muduo::Singleton<NodeInfo>::instance().updateAVDecGetInfo(vAVDecInfo);
		DP_M2S_AO_DEV_E AOChnID;
		if (closeAudio->u8AoChnID == ID_AO_CHN_VIDEOOUT1) {
			AOChnID = DP_M2S_AO_DEV_HDMI0_HI3536;
		} else if (closeAudio->u8AoChnID == ID_AO_CHN_LINEOUT1) {
			AOChnID = DP_M2S_AO_DEV_LINEOUT0_HI3536;
		}
		NodeInfo::MapAODevIDCodecIDPtr AODevCodecID =
				muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
		NodeInfo::MapAODevIDCodecID::iterator iter;
		if ((iter = AODevCodecID->find(AOChnID)) != AODevCodecID->end()) {
			AODevCodecID->erase(iter);
			muduo::Singleton<NodeInfo>::instance().updateAODevIDCodecID(
					AODevCodecID);
		}
	} while (0);
	commonReplyToThird(Command_CloseAudio, closeAudio->u32TaskID, success,
			connPtr);
#endif
}

void OpenWinsBatch(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
#if (OutputDevice)
	//jhbnote todo
#endif
}
void LogicHandle::setAudioInfo(const muduo::net::TcpConnectionPtr connPtr,
		std::string data) {
#if (OutputDevice)
	//jhbnote todo restore mute vol status
	//jhbnote todo check devID?
	//jhbnote todo get more audio info.

	_sRemote_SetAudio *setAudio = (_sRemote_SetAudio*) data.c_str();
	LOG_WARN << "SetAudio cmd...u32RequestID: "
			<< setAudio->header.u32RequestID;

	NodeInfo::MapMapAODevAudioInfoPtr aoAudioInfo =
			muduo::Singleton<NodeInfo>::instance().AODevAudioInfo();

	muduo::Singleton<NodeInfo>::instance().updateAODevAudioInfo(aoAudioInfo);

#endif
}

DP_S32 LogicHandle::getNewCodecTaskID(DP_U32 thirdTaskID,
		TaskObjectType_E type) {
	return muduo::Singleton<NodeInfo>::instance().getNewCodecTaskID(thirdTaskID,
			type);
}

void LogicHandle::Get_InputVideoChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
#if (InputDevice)
	LOG_INFO << "Get 'Property_Get_InputVideoChnInfo' cmd from "
	<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrVIGetInfoPtr viGetInfo =
	muduo::Singleton<NodeInfo>::instance().getVIGetInfo();

	muduo::net::Buffer buffSend;
	if (viGetInfo->empty()) {
		replyGetInfoToThird(Property_Get_InputVideoChnInfo,
				DP_ERR_STATE_ABNORMAL, buffSend, connPtr);
		return;
	}

	DP_U32 viChCount = viGetInfo->size();
	DP_U32 viRealCount = 0;
	_sAllViChnInfo allViChInfo((DP_U8) viChCount);
	buffSend.append(&allViChInfo, sizeof(_sAllAiChnInfo));

	for (NodeInfo::VctrVIGetInfo::iterator it = viGetInfo->begin();
			it != viGetInfo->end(); it++) {
		boost::shared_ptr<_sAllViChnInfo::_sSingleViChnInfo> singleViCh(
				new _sAllViChnInfo::_sSingleViChnInfo((DP_S8) it->bSignal,
						it->stCap.u32Width, it->stCap.u32Height,
						it->u32FrmRate));
		buffSend.append(singleViCh.get(),
				sizeof(_sAllViChnInfo::_sSingleViChnInfo));
		viRealCount++;
	}

	if (viRealCount != viChCount) {
		LOG_ERROR << "Send viChCount conflicted viRealCount : " << viRealCount
		<< " viChCount: " << viChCount;
		replyGetInfoToThird(Property_Get_InputVideoChnInfo, DP_ERR_TASK_OPERATE,
				buffSend, connPtr);
		return;
	} else {
		replyGetInfoToThird(Property_Get_InputVideoChnInfo, 0, buffSend,
				connPtr);
	}
#endif
}

void LogicHandle::Get_InputAudioChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
#if (InputDevice)
	LOG_INFO << "Get 'Property_Get_InputAudioChnInfo' cmd from "
	<< connPtr->peerAddress().toIpPort();
	NodeInfo::VctrAIGetInfoPtr aiGetInfo =
	muduo::Singleton<NodeInfo>::instance().getAIGetInfo();
	muduo::net::Buffer buffSend;

	if (aiGetInfo->empty()) {
		replyGetInfoToThird(Property_Get_InputAudioChnInfo,
				DP_ERR_STATE_ABNORMAL, buffSend, connPtr);
		return;
	}

	DP_U32 aiChCount = aiGetInfo->size();
	DP_U32 aiRealCount = 0;
	_sAllAiChnInfo allAiChInfo(aiChCount);

	buffSend.append(&allAiChInfo, sizeof(_sAllAiChnInfo));
	NodeInfo::VctrAVENCGetInfoPtr vAVEncInNodeInfo =
	muduo::Singleton<NodeInfo>::instance().getAVEncGetInfo();

	for (NodeInfo::VctrAIGetInfo::iterator it = aiGetInfo->begin();
			it != aiGetInfo->end(); it++) {
		if (it->enDevId != DP_M2S_AI_DEV_LINEIN0_HI3536) {
			LOG_ERROR << "Error dev id : " << it->enDevId;
			replyGetInfoToThird(Property_Get_InputAudioChnInfo,
					DP_ERR_AI_ID_INVALID, buffSend, connPtr);
			return;
		} else {
			DP_U32 devID = it->enDevId;
			NodeInfo::VctrAVENCGetInfo::iterator it_cam = std::find_if(
					vAVEncInNodeInfo.get()->begin(),
					vAVEncInNodeInfo.get()->end(),
					bind2nd(compareDevID<DP_M2S_AVENC_INFO_S>(), devID));

			if (it_cam != vAVEncInNodeInfo.get()->end()) {
				boost::shared_ptr<_sAllAiChnInfo::_sSingleAiChnInfo> singleAICh(
						new _sAllAiChnInfo::_sSingleAiChnInfo(it->bSignal,
								it_cam->stStream._rtsp.stRtspServer.au8Url,
								DP_M2S_URL_LEN, it->stCommAttr.enSamplerate,
								it->stCommAttr.enBitwidth,
								it->stCommAttr.enSoundmode, AUDIO_ENCODE_AAC));
				buffSend.append(singleAICh.get(),
						sizeof(_sAllAiChnInfo::_sSingleAiChnInfo));
				aiRealCount++;
				LOG_DEBUG << "_sAllAiChnInfo.u8AiSignalStatus: "
				<< singleAICh->u8AiSignalStatus << " url : "
				<< singleAICh->au8PreviewRtspURL
				<< " u8AiDevIntfSampleRate: "
				<< singleAICh->srcAudioInfo.u8AiDevIntfSampleRate
				<< " u8AiDevIntfSoundMode: "
				<< singleAICh->srcAudioInfo.u8AiDevIntfSoundMode
				<< " u8AiDevIntfBitwidth: "
				<< singleAICh->srcAudioInfo.u8AiDevIntfBitwidth
				<< " u8AencType: "
				<< singleAICh->srcAudioInfo.u8AencType;
			} else {
				//can not find dev id from avenc map
				//then reply null ai info
				LOG_ERROR << "Can not find dev id from avenc map devID: "
				<< devID << " in prog.";
				boost::shared_ptr<_sAllAiChnInfo::_sSingleAiChnInfo> singleAICh(
						new _sAllAiChnInfo::_sSingleAiChnInfo());
				buffSend.append(singleAICh.get(),
						sizeof(_sAllAiChnInfo::_sSingleAiChnInfo));
				aiRealCount++;
			}
		}
	}
	if (aiRealCount != aiChCount) {
		LOG_ERROR << "Send video aiChCount conflicted aiRealCount : "
		<< aiRealCount << " aiChCount: " << aiChCount;
		replyGetInfoToThird(Property_Get_InputAudioChnInfo, DP_ERR_TASK_OPERATE,
				buffSend, connPtr);
		return;
	} else {
		replyGetInfoToThird(Property_Get_InputAudioChnInfo, 0, buffSend,
				connPtr);
	}
#endif
}

void LogicHandle::Get_VideoChnVencInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
#if (InputDevice)
	LOG_INFO << "Get 'Property_Get_VideoChnVencInfo' cmd from "
	<< connPtr->peerAddress().toIpPort();

	muduo::net::Buffer buffSend;
	NodeInfo::VctrAVENCGetInfoPtr vAVEncInNodeInfo =
	muduo::Singleton<NodeInfo>::instance().getAVEncGetInfo();
	NodeInfo::VctrVIGetInfoPtr viInfo =
	muduo::Singleton<NodeInfo>::instance().getVIGetInfo();
	if ((!muduo::Singleton<NodeInfo>::instance().getCodecInited())
			|| (viInfo->empty()) || (vAVEncInNodeInfo->empty())) {
		replyGetInfoToThird(Property_Get_VideoChnVencInfo,
				DP_ERR_STATE_ABNORMAL, buffSend, connPtr);
		return;
	}

	DP_U32 vencCount = 0;
	DP_U32 vencRealCount = 0;
	for (NodeInfo::VctrAVENCGetInfo::iterator it = vAVEncInNodeInfo->begin();
			it != vAVEncInNodeInfo->end(); it++) {
		if (it->stAvBind.enBindType == DP_M2S_AVBIND_VI2VENC
				|| it->stAvBind.enBindType == DP_M2S_AVBIND_AI2AENC_VI2VENC)
		vencCount++;
	}

//	DP_U8 avEncChCount = vAVEncInNodeInfo->size();
	_sAllVencChnInfo allAiChInfo(viInfo->size(), vencCount);

	DP_U8 audioIn = 0;

	buffSend.append(&allAiChInfo, sizeof(_sAllVencChnInfo));
	eDeviceVideoChannelID viCh = ID_VI_CHN_VIDEOIN1;
	NodeInfo::VctrAIGetInfo::iterator itAI;
	for (NodeInfo::VctrAVENCGetInfo::iterator it = vAVEncInNodeInfo->begin();
			it != vAVEncInNodeInfo->end(); it++) {

//get vi ch
		if (it->stAvBind.enBindType == DP_M2S_AVBIND_VI2VENC
				|| it->stAvBind.enBindType == DP_M2S_AVBIND_AI2AENC_VI2VENC) {
			if (it->stAvBind.stVideo.stIn.u32DevId
					== DP_M2S_VI_DEV_HDMI0_ITE6801) {
				viCh = ID_VI_CHN_VIDEOIN1;
			} else {
				//error
				replyGetInfoToThird(Property_Get_VideoChnVencInfo,
						DP_ERR_VI_ID_INVALID, buffSend, connPtr);
				return;
			}
		}

		boost::shared_ptr<_sAllVencChnInfo::_sSingleVencChnInfo> singleVencCh(
				new _sAllVencChnInfo::_sSingleVencChnInfo(viCh,
						VENCIDID_ORIGINAL_SINAGL, 0x0000,
						it->stVenc.stCrop.u32Width, it->stVenc.stCrop.u32Height,
						it->stVenc.stCrop.s32X, it->stVenc.stCrop.s32Y,
						it->stStream._rtsp.stRtspServer.au8Url,
						DP_M2S_URL_LEN, audioIn, _sSrcAudioInfo()));

		if (it->stAvBind.enBindType == DP_M2S_AVBIND_VI2VENC) {
			singleVencCh->u8AudioIn = 0;
			buffSend.append(singleVencCh.get(),
					sizeof(_sAllVencChnInfo::_sSingleVencChnInfo));
			vencRealCount++;
		} else if (it->stAvBind.enBindType == DP_M2S_AVBIND_AI2AENC_VI2VENC) {
			singleVencCh->u8AudioIn = 1;
			NodeInfo::VctrAIGetInfoPtr aiInfo =
			muduo::Singleton<NodeInfo>::instance().getAIGetInfo();
			DP_M2S_AI_GET_INFO_S *aiGetInfo = NULL;
			itAI =
			find_if(aiInfo->begin(), aiInfo->end(),
					bind2nd(
							findAVDevID<DP_M2S_AI_GET_INFO_S,
							DP_M2S_AI_DEV_E>(),
							DP_M2S_AI_DEV_HDMI0_ITE6801));
			if (itAI != aiInfo->end()) {
				aiGetInfo = (DP_M2S_AI_GET_INFO_S *) (&*itAI);
				_sSrcAudioInfo srcAiInfo(aiGetInfo->stCommAttr.enSamplerate,
						aiGetInfo->stCommAttr.enBitwidth,
						aiGetInfo->stCommAttr.enSoundmode, AUDIO_ENCODE_AAC); //AAC default
				singleVencCh->srcAudioInfo = srcAiInfo;
				buffSend.append(singleVencCh.get(),
						sizeof(_sAllVencChnInfo::_sSingleVencChnInfo));
				vencRealCount++;
			} else {
				LOG_ERROR << "Can not find dev id : "
				<< DP_M2S_AI_DEV_HDMI0_ITE6801
				<< " in ai info map. aiInfo size: " << aiInfo->size();
			}
		} else {
			continue;
		}

	}
	if (vencRealCount != vencCount) {
		LOG_ERROR << "Send video vencCount conflicted vencRealCount : "
		<< vencRealCount << " vencCount: " << vencCount;
		replyGetInfoToThird(Property_Get_VideoChnVencInfo, DP_ERR_TASK_OPERATE,
				buffSend, connPtr);
		return;
	} else {
		replyGetInfoToThird(Property_Get_VideoChnVencInfo, 0, buffSend,
				connPtr);
	}

#endif
}

void LogicHandle::Get_OutputVideoChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
#if (OutputDevice)
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
			NodeInfo::MapThirdIDSrcVideoInfo::iterator iter =
					srcVideoInfo->find(videoInfo->u32TaskID);
			if (iter != srcVideoInfo->end()) {
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
	} else {
		replyGetInfoToThird(Property_Get_OutputVideoChnInfo, 0, buffSend,
				connPtr);
	}
#endif
}

void LogicHandle::Get_OutputAudioChnInfo(
		const muduo::net::TcpConnectionPtr connPtr) {
#if (OutputDevice)
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

	eDeviceAudioChannelID AOChnID;
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
#endif
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
	LOG_INFO << "Send to third : " << reply.header.u16PackageLen
			<< " bytes ! and success: " << success;
	connPtr->send(&sendBuff);
}

void LogicHandle::replyClearToThird(DP_U32 success,
		NodeInfo::VecErrInfo errTaskID, NodeInfo::VecErrInfo errCode,
		const muduo::net::TcpConnectionPtr connPtr) {
	_sRemote_Header head(
			muduo::Singleton<NodeInfo>::instance().getNetInfo().ip2U32(),
			g_DevType, 0x01, sizeof(_sRemote_Reply_ClearTask),
			Command_ClearTask,
			DP_DEV_ID_LEN + (sizeof(DP_U32) << 1));
	_sRemote_Reply_ClearTask reply(head,
			muduo::Singleton<LocalInfo>::instance().getLocalInfo()->au8DevID,
			success, 0);
	muduo::net::Buffer sendBuff;
	DP_U32 count = errTaskID.size();
	if (errCode.size() != errTaskID.size()) {
		LOG_ERROR << "errCode.size()!=errTaskID.size()! ";
		sendBuff.append(&reply, reply.header.u16PackageLen);
	} else if (count == 0) {
		sendBuff.append(&reply, reply.header.u16PackageLen);
	} else {
		if (success == 0) {
			sendBuff.append(&reply, reply.header.u16PackageLen);
		} else {
			head.stFunctionMsg.u16CommandLen += (count * sizeof(DP_U32)) << 1;
			reply.header.u16PackageLen += (count * sizeof(DP_U32)) << 1;
			reply.u32ErrTaskCount = count;
			DP_U32 *taskID = new DP_U32[count];
			DP_U32 *code = new DP_U32[count];
			memset(taskID, 0, sizeof(DP_U32) * count);
			memset(code, 0, sizeof(DP_U32) * count);
			for (DP_U32 i = 0; i < count; i++) {
				taskID[i] = errTaskID[i];
				code[i] = errCode[i];
			}
			sendBuff.append(&reply, reply.header.u16PackageLen);
			sendBuff.append(taskID, sizeof(taskID));
			sendBuff.append(code, sizeof(code));
		}

		LOG_INFO << "Send to third : " << reply.header.u16PackageLen
				<< " bytes !";
	}
	connPtr->send(&sendBuff);
}

void LogicHandle::replyGetInfoToThird(eRemotePropertyName proterty,
		DP_U32 success, muduo::net::Buffer buffSend,
		const muduo::net::TcpConnectionPtr connPtr) {
	DP_U32 propertyLen = 0;
	if (success != 0) {
		propertyLen = sizeof(DP_U8 *);
	} else {
		propertyLen = buffSend.readableBytes();
	}
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
	if (success == 0) {
		buff.append(&replyGetInfo, packageLen - propertyLen);
		buff.append(buffSend.toStringPiece());
	} else {
		buff.append(&replyGetInfo, sizeof(_sRemote_Reply_GetInfo));
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
#if (OutputDevice)
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
#elif (InputDevice)
	if (crop.u32Width < DP_M2S_H264E_MIN_WIDTH
			|| crop.u32Width > DP_M2S_H264E_MAX_WIDTH) {
		LOG_ERROR
		<< "crop param check is error,u32Width(%d) < %d || u32Width(%d)>%d "
		<< crop.u32Width << " " << DP_M2S_H264E_MIN_WIDTH << " "
		<< crop.u32Width << " " << DP_M2S_H264E_MAX_WIDTH;
		return DP_FALSE;
	}
	if (crop.u32Height < DP_M2S_H264E_MIN_HEIGHT
			|| crop.u32Height > DP_M2S_H264E_MAX_HEIGHT) {
		LOG_ERROR
		<< "crop param check is error,u32Height(%d) < %d || u32Height(%d)>%d "
		<< crop.u32Height << " " << DP_M2S_H264E_MIN_HEIGHT << " "
		<< crop.u32Height << " " << DP_M2S_H264E_MAX_HEIGHT;
		return DP_FALSE;
	}
#endif

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
