/*
 * UnixSockClient.cpp
 *
 *  Created on: 2017年9月20日
 *      Author: root
 */

#include <string.h>
#include <muduo/base/Logging.h>
#include "PrintBuff.h"
#include "UnixSockClientData.h"
#include "ErrorCode.h"

UnixSockClientData::UnixSockClientData(recvCallBack cb) {
	// TODO Auto-generated constructor stub
//	pthread_mutex_init(&_condMutex, NULL);
//	pthread_cond_init(&_cond, NULL);
	_bInitCommandOK = false;
//	setTimer(2, 100, true);
	_cb = cb;
	_buffer = new uint8_t[BUFFER_SIZE_PIPESOCKET];
//	_Nodecb = NULL;
}

//UnixSockClientData::UnixSockClientData(NodeInfoRecvCallBack cb) {
//	// TODO Auto-generated constructor stub
//	_bInitCommandOK = false;
//	setTimer(2, 100, true);
//	_cb = NULL;
//	_Nodecb = cb;
//}

UnixSockClientData::~UnixSockClientData() {
	delete[] _buffer;
	_buffer = NULL;
	// TODO Auto-generated destructor stub
}

//void UnixSockClientData::onTimer(int nID) {
//	if (nID == 1) { //每3秒发送在线包
//
//	} else if (nID == 2) { //每100毫秒检查是否有数据包要发送
////		try {
//		vector<vector<int8_t> > vTempPackages;
//		{
//			AutoLock lock(&_mutex);
//			if (_packages.size() == 0)
//				return;
//			vTempPackages = _packages;
//			_packages.clear();
//		}
//		for (size_t i = 0; i < vTempPackages.size(); i++) {
////				try {
//			doSendCommand(&vTempPackages[i].front(), vTempPackages[i].size());
//			ThreadUtil::Sleep(20);
////				} catch (SystemException &ex) {
////					Console::instance()->warning(ex.what());
////				}
//		}
////		} catch (SystemException& ex) {
////			Console::instance()->error(ex.what());
////		}
//
//	}
//}

//void UnixSockClientData::addCommand(const void* pData, int len) {
//	return;
//	if (len <= 0) {
//		return;
////		throw SystemException(pData, 20);
//	}
//	vector<int8_t> vData;
//	vData.assign((uint8_t*) pData, (((uint8_t*) pData) + len));
//	AutoLock lock(&_mutex);
//	_packages.push_back(vData);
//}
#if 1
int UnixSockClientData::onlySendMsg(const void* pData, int len)
/*throw (SystemException)*/{
	assert(pData);
	//1.create sock & connect
	int sock = SocketLayer::CreatePipeSock();
	int retrycount = 10;
	sockaddr_un serveraddr = SocketLayer::FilePath2UnixAddress(
	DP_M2S_INF_PROT_UNIX_FIFO);
	bool bConnected = false;
	while ((--retrycount) > 0) {
		/////////////////////////////////////////////////if failed
		int ret = connect(sock, (const sockaddr*) &serveraddr,
				sizeof(serveraddr));
		if (ret == -1) {
			if (errno == EISCONN) {
				bConnected = true;
				break;
			} else if (errno == EINTR) {
				continue;
			} else
				ThreadUtil::Sleep(5);
		}
	}
	if (!bConnected) {
		SocketLayer::CloseSock(sock);
		return DP_ERR_COMMUNICATE_ABNORMAL;
//		throw SystemException("can not connect unix socket");
	}
	//2.send data
	int wbytes = send(sock, pData, len, 0);
	if (wbytes <= 0) {
		SocketLayer::CloseSock(sock);
		return DP_ERR_COMMUNICATE_ABNORMAL;
//		throw SystemException("Send failed !");
	}
	//	muduo::PrintBuff::printBufferByHex("send to fifo : ", pData, wbytes);
	if (wbytes != len) {
		SocketLayer::CloseSock(sock);
		return DP_ERR_COMMUNICATE_ABNORMAL;
//		throw SystemException("send bytes!=actual send bytes");
	}
	SocketLayer::CloseSock(sock);
	return wbytes;
}

int UnixSockClientData::doSendCommand(const void* pData, int len)
/*throw (SystemException) */{
	assert(pData);
	//1.create sock & connect
	int sock = SocketLayer::CreatePipeSock();
	int retrycount = 10;
	sockaddr_un serveraddr = SocketLayer::FilePath2UnixAddress(
	DP_M2S_INF_PROT_UNIX_FIFO);
	bool bConnected = false;
	while ((--retrycount) > 0) {
		/////////////////////////////////////////////////if failed
		int ret = connect(sock, (const sockaddr*) &serveraddr,
				sizeof(serveraddr));
		if (ret == -1) {
			if (errno == EISCONN) {
				bConnected = true;
				break;
			} else if (errno == EINTR) {
				continue;
			} else
				ThreadUtil::Sleep(5);
		}
	}
	if (!bConnected) {
		SocketLayer::CloseSock(sock);
		return DP_ERR_COMMUNICATE_ABNORMAL;
//		throw SystemException("can not connect unix socket");
	}
	//2.send data
	int wbytes = send(sock, pData, len, 0);
	if (wbytes <= 0) {
		SocketLayer::CloseSock(sock);
		return DP_ERR_COMMUNICATE_ABNORMAL;
//		throw SystemException(__FILE__, __FUNCTION__, __LINE__);
	}
	muduo::PrintBuff::printBufferByHex("send to fifo : ", pData, wbytes);
	if (wbytes != len) {
		SocketLayer::CloseSock(sock);
		return DP_ERR_COMMUNICATE_ABNORMAL;
//		throw SystemException("send bytes!=actual send bytes");
	}
	//3.recv &&close socket
	fd_set readfd;
	timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0; //100ms
	int result = -1;
	int rbytes;

	memset(_buffer, 0, BUFFER_SIZE_PIPESOCKET);
//	uint8_t buffer[BUFFER_SIZE];
	while (1) {
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);
		result = select(sock + 1, &readfd, 0, 0, &timeout);
		if (result == 0) { //timeout
			SocketLayer::CloseSock(sock);
			return DP_ERR_COMMUNICATE_ABNORMAL_TIMEOUT;
		} else if (result > 0) {
			rbytes = recv(sock, _buffer, BUFFER_SIZE_PIPESOCKET, 0);
			if (rbytes > 0) {

//				cout << "rbytes:: " << rbytes << "buff: " << _buffer << endl;
				if (_cb) {
					SocketLayer::CloseSock(sock);
					return _cb(_buffer, rbytes);
				}
				SocketLayer::CloseSock(sock);
//				doRecvCommand((void*) buffer, rbytes);
			} else if (rbytes == 0) {
				SocketLayer::CloseSock(sock);
				return -1;
			}
		} else {
			SocketLayer::CloseSock(sock);
			return DP_ERR_COMMUNICATE_ABNORMAL;
//			throw SystemException("Recv 0 msg from codec !");
		}
	}
	SocketLayer::CloseSock(sock);
	//4.end
}
#if 0
void UnixSockClientData::doRecvCommand(void* pData, int len) throw (SystemException) {
	DP_M2S_INF_PROT_HEAD_S *pHeader = (DP_M2S_INF_PROT_HEAD_S *) pData;
	switch (pHeader->u8CommandID) {
		case DP_M2S_CMD_GETINFO: {
			DP_M2S_CMD_GETINFO_RESPOND_S *pGetInfo = (DP_M2S_CMD_GETINFO_RESPOND_S *) pData;
			DP_U8 *recvBuffer = (DP_U8*) pData;
			if (pGetInfo->enInfoTYpe == DP_M2S_INFO_TYPE_GET_AI) {
				Console::instance()->debug("#############recv get ai");
				DP_M2S_AI_SET_INFO_S aiStatus;
				DP_U8 aiNum = (pGetInfo->u32InfoLen / sizeof(DP_M2S_AI_SET_INFO_S));
				for (int i = 0; i < aiNum; i++) {
					memcpy(&aiStatus, &recvBuffer[sizeof(DP_M2S_CMD_GETINFO_RESPOND_S) + sizeof(DP_M2S_AI_SET_INFO_S) * i], sizeof(DP_M2S_AI_SET_INFO_S));
					Console::instance()->debug("##################AI:%d####################", i);
					Console::instance()->debug("dev id :%d", aiStatus.s32DevId);
					Console::instance()->debug("vol:%d", aiStatus.u8Vol);
					Console::instance()->debug("selfadaptive:%d", aiStatus.bSelfAdaptive);
					Console::instance()->debug("sample:%d", aiStatus.stCommAttr.enSamplerate);
					Console::instance()->debug("bit width:%d", aiStatus.stCommAttr.enBitwidth);
					Console::instance()->debug("sound mode :%d", aiStatus.stCommAttr.enSoundmode);
					Console::instance()->debug("#################END####################");
				}
			} else if (pGetInfo->enInfoTYpe == DP_M2S_INFO_TYPE_GET_VI) {
				DP_M2S_VI_GET_INFO_S viStatus;
				DP_U8 viNum = (pGetInfo->u32InfoLen / sizeof(DP_M2S_VI_GET_INFO_S));
				for (int i = 0; i < viNum; i++) {
					memcpy(&viStatus, &recvBuffer[sizeof(DP_M2S_CMD_GETINFO_RESPOND_S) + sizeof(DP_M2S_VI_GET_INFO_S) * i], sizeof(DP_M2S_VI_GET_INFO_S));
					Console::instance()->debug("##################VI:%d####################", i);
					Console::instance()->debug("dec id :%d", viStatus.s32DevId);
					Console::instance()->debug("dec conn:%d", viStatus.bConn);
					Console::instance()->debug("dec signal:%d", viStatus.bSignal);
					Console::instance()->debug("dec framRate:%d", viStatus.s32FrmRate);
					Console::instance()->debug("dec stCap x:%d", viStatus.stCap.s32X);
					Console::instance()->debug("dec stCap y:%d", viStatus.stCap.s32Y);
					Console::instance()->debug("dec stCap widget:%d", viStatus.stCap.u32Width);
					Console::instance()->debug("dec stCap height:%d", viStatus.stCap.u32Height);
					Console::instance()->debug("dec stDst widget:%d", viStatus.stDst.u32Width);
					Console::instance()->debug("dec stDst height:%d", viStatus.stDst.u32Height);
					Console::instance()->debug("#################END####################");
				}
			} else if (pGetInfo->enInfoTYpe == DP_M2S_INFO_TYPE_GET_AVENC) {
				DP_M2S_AVENC_GET_INFO_S info;
				DP_U8 AvencNum = (pGetInfo->u32InfoLen / sizeof(DP_M2S_AVENC_GET_INFO_S));
				for (int i = 0; i < AvencNum; i++) {
					memcpy(&info, &recvBuffer[sizeof(DP_M2S_CMD_GETINFO_RESPOND_S) + sizeof(DP_M2S_AVENC_GET_INFO_S) * i], sizeof(DP_M2S_AVENC_GET_INFO_S));
					memcpy(&_avencStatus[i], &info, sizeof(DP_M2S_AVENC_GET_INFO_S));
					Console::instance()->debug("###################AVENC:%d####################", i);
					Console::instance()->debug("task id:%d", info.TskId);
					Console::instance()->debug("-------------------------avbind");
					Console::instance()->debug("av bind type:%d", info.AvBindAttr.enBindType);
					Console::instance()->debug("av bind audio src mode id:%d", info.AvBindAttr.stAudio.stIn.ModId);
					Console::instance()->debug("av bind audio src dev id:%d", info.AvBindAttr.stAudio.stIn.u32DevId);
					Console::instance()->debug("av bind audio src chn id:%d", info.AvBindAttr.stAudio.stIn.u32ChnId);
					Console::instance()->debug("av bind audio dst mode id:%d", info.AvBindAttr.stAudio.stOut.ModId);
					Console::instance()->debug("av bind audio dst mode id:%d", info.AvBindAttr.stAudio.stOut.u32DevId);
					Console::instance()->debug("av bind audio dst mode id:%d", info.AvBindAttr.stAudio.stOut.u32ChnId);
					Console::instance()->debug("av bind video src mode id:%d", info.AvBindAttr.stVideo.stIn.ModId);
					Console::instance()->debug("av bind video src dev id:%d", info.AvBindAttr.stVideo.stIn.u32DevId);
					Console::instance()->debug("av bind video src chn id:%d", info.AvBindAttr.stVideo.stIn.u32ChnId);
					Console::instance()->debug("av bind video dst mode id:%d", info.AvBindAttr.stVideo.stOut.ModId);
					Console::instance()->debug("av bind video dst mode id:%d", info.AvBindAttr.stVideo.stOut.u32DevId);
					Console::instance()->debug("av bind video dst mode id:%d", info.AvBindAttr.stVideo.stOut.u32ChnId);
					Console::instance()->debug("-------------------------stream");
					Console::instance()->debug("stream type:%d", info.stStream.enType);
//				Console::instance()->debug("stream rtsp server open:%d", info.stStream.stRtspServer.bOpen);
//				Console::instance()->debug("stream rtsp server multicast:%d", info.stStream.stRtspServer.bMulticast);
//				Console::instance()->debug("stream rtsp server transtype:%d", info.stStream.stRtspServer.s32TransType);
//				Console::instance()->debug("stream rtsp server conntimeout :%d", info.stStream.stRtspServer.s32ConnTimeout);
//				Console::instance()->debug("stream rtsp server connmax :%d", info.stStream.stRtspServer.s32ConnMax);
//				Console::instance()->debug("stream rtsp server connNums:%d", info.stStream.stRtspServer.s32ConnNums);
//				Console::instance()->debug("stream rtsp server au8url :%s", info.stStream.stRtspServer.au8Url);
					Console::instance()->debug("-------------------------stAenc");
					Console::instance()->debug("aenc alg :%d ", info.stAenc.enAlg);
					Console::instance()->debug("aenc s32bitrate :%d", info.stAenc.s32Bitrate);
					Console::instance()->debug("-------------------------stVenc");
					Console::instance()->debug("venc is bCrop :%d", info.stVenc.bCrop);
					Console::instance()->debug("venc crop status x:%d", info.stVenc.stCrop.s32X);
					Console::instance()->debug("venc crop status y:%d", info.stVenc.stCrop.s32Y);
					Console::instance()->debug("venc crop status widget:%d", info.stVenc.stCrop.u32Width);
					Console::instance()->debug("venc crop status height:%d", info.stVenc.stCrop.u32Height);
					Console::instance()->debug("venc is zoom :%d", info.stVenc.bZoom);
					Console::instance()->debug("venc zoom type:%d", info.stVenc.stZoom.enType);
					if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RECT) {
						Console::instance()->debug("venc zoom rect x:%d", info.stVenc.stZoom.stRect.s32X);
						Console::instance()->debug("venc zoom rect y:%d", info.stVenc.stZoom.stRect.s32Y);
						Console::instance()->debug("venc zoom rect widget:%d", info.stVenc.stZoom.stRect.u32Width);
						Console::instance()->debug("venc zoom rect height:%d", info.stVenc.stZoom.stRect.u32Height);
					} else if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RATIO) {
						Console::instance()->debug("venc zoom ratio x:%d", info.stVenc.stZoom.stRatio.u32XRatio);
						Console::instance()->debug("venc zoom ratio y:%d", info.stVenc.stZoom.stRatio.u32YRatio);
						Console::instance()->debug("venc zoom ratio widget:%d", info.stVenc.stZoom.stRatio.u32WRatio);
						Console::instance()->debug("venc zoom ratio height:%d", info.stVenc.stZoom.stRatio.u32HRatio);
					}
					Console::instance()->debug("venc is osd:%d", info.stVenc.bOsd);
					Console::instance()->debug("venc osd type :%d", info.stVenc.stOsd.enType);
					if (info.stVenc.stOsd.enType == DP_M2S_OSD_PIC) {
						Console::instance()->debug("venc osd pic path :%s", info.stVenc.stOsd.au8PicPath);
					} else if (info.stVenc.stOsd.enType == DP_M2S_OSD_STRING) {
						Console::instance()->debug("venc osd str path :%s", info.stVenc.stOsd.stStr.au8Str);
						Console::instance()->debug("venc osd str color :%d", info.stVenc.stOsd.stStr.u32Color);
					}
					Console::instance()->debug("venc osd display mode :%d", info.stVenc.stOsd.enDispMode);
					Console::instance()->debug("venc osd point x:%d", info.stVenc.stOsd.stPoint.s32X);
					Console::instance()->debug("venc osd point y:%d", info.stVenc.stOsd.stPoint.s32Y);
					Console::instance()->debug("venc alg type :%d", info.stVenc.stAlg.enAlg);
					Console::instance()->debug("venc alg frmrate:%d", info.stVenc.stAlg.stH264Enc.u32FrmRate);
					Console::instance()->debug("venc alg stSize Widget:%d", info.stVenc.stAlg.stH264Enc.stSize.u32Width);
					Console::instance()->debug("venc alg stSize Height:%d", info.stVenc.stAlg.stH264Enc.stSize.u32Height);
					Console::instance()->debug("venc alg RcMode:%d", info.stVenc.stAlg.stH264Enc.enRcMode);
					Console::instance()->debug("venc alg BitRate:%d", info.stVenc.stAlg.stH264Enc.u32Bitrate);
					Console::instance()->debug("venc alg Profile:%d", info.stVenc.stAlg.stH264Enc.enProfile);
					Console::instance()->debug("venc alg gop:%d", info.stVenc.stAlg.stH264Enc.u32Gop);
					Console::instance()->debug("venc alg sf:%d", info.stVenc.stAlg.stH264Enc.u16SF);
					Console::instance()->debug("venc alg tf:%d", info.stVenc.stAlg.stH264Enc.u16TF);
					Console::instance()->debug("####################END####################");
				}
			} else if (pGetInfo->enInfoTYpe == DP_M2S_INFO_TYPE_GET_AVDEC) {
				DP_M2S_AVDEC_GET_INFO_S info;
				DP_U8 AvdecNum = (pGetInfo->u32InfoLen / sizeof(DP_M2S_AVDEC_GET_INFO_S));
				for (int i = 0; i < AvdecNum; i++) {
					memcpy(&info, &recvBuffer[sizeof(DP_M2S_CMD_GETINFO_RESPOND_S) + sizeof(DP_M2S_AVDEC_GET_INFO_S) * i], sizeof(DP_M2S_AVDEC_GET_INFO_S));
					memcpy(&_avdecStatus[i], &info, sizeof(DP_M2S_AVDEC_GET_INFO_S));
					Console::instance()->debug("###################ADEC:%d####################", i);
					Console::instance()->debug("task id:%d", info.TskId);
					Console::instance()->debug("-------------------------avbind");
					Console::instance()->debug("av bind type:%d", info.AvBindAttr.enBindType);
					Console::instance()->debug("av bind audio src mode id:%d", info.AvBindAttr.stAudio.stIn.ModId);
					Console::instance()->debug("av bind audio src dev id:%d", info.AvBindAttr.stAudio.stIn.u32DevId);
					Console::instance()->debug("av bind audio src chn id:%d", info.AvBindAttr.stAudio.stIn.u32ChnId);
					Console::instance()->debug("av bind audio dst mode id:%d", info.AvBindAttr.stAudio.stOut.ModId);
					Console::instance()->debug("av bind audio dst mode id:%d", info.AvBindAttr.stAudio.stOut.u32DevId);
					Console::instance()->debug("av bind audio dst mode id:%d", info.AvBindAttr.stAudio.stOut.u32ChnId);
					Console::instance()->debug("av bind video src mode id:%d", info.AvBindAttr.stAudio.stIn.ModId);
					Console::instance()->debug("av bind video src dev id:%d", info.AvBindAttr.stVideo.stIn.u32DevId);
					Console::instance()->debug("av bind video src chn id:%d", info.AvBindAttr.stVideo.stIn.u32ChnId);
					Console::instance()->debug("av bind video dst mode id:%d", info.AvBindAttr.stVideo.stOut.ModId);
					Console::instance()->debug("av bind video dst mode id:%d", info.AvBindAttr.stVideo.stOut.u32DevId);
					Console::instance()->debug("av bind video dst mode id:%d", info.AvBindAttr.stVideo.stOut.u32ChnId);
					Console::instance()->debug("-------------------------stream");
					Console::instance()->debug("stream type:%d", info.stStream.enType);
//				Console::instance()->debug("stream rtsp client open:%d", info.stStream.stRtspClient.s8Open);
//				Console::instance()->debug("stream rtsp client multicast:%d", info.stStream.stRtspClient.bMulticast);
//				Console::instance()->debug("stream rtsp client transtype:%d", info.stStream.stRtspClient.s32TransType);
//				Console::instance()->debug("stream rtsp client conntimeout :%d", info.stStream.stRtspClient.s32ConnTimeout);
//				Console::instance()->debug("stream rtsp clinet au8url :%s", info.stStream.stRtspClient.au8Url);
					Console::instance()->debug("-------------------------stAdec");
					Console::instance()->debug("adec alg :%d ", info.stAdec.enAlg);
					Console::instance()->debug("-------------------------stVdec");
					Console::instance()->debug("vdec is bCrop :%d", info.stVdec.bCrop);
					Console::instance()->debug("vdec crop status x:%d", info.stVdec.stCrop.s32X);
					Console::instance()->debug("vdec crop status y:%d", info.stVdec.stCrop.s32Y);
					Console::instance()->debug("vdec crop status widget:%d", info.stVdec.stCrop.u32Width);
					Console::instance()->debug("vdec crop status height:%d", info.stVdec.stCrop.u32Height);
					Console::instance()->debug("vdec is zoom :%d", info.stVdec.bZoom);
					Console::instance()->debug("vdec zoom type:%d", info.stVdec.stZoom.enType);
					if (info.stVdec.stZoom.enType == DP_M2S_ZOOM_RECT) {
						Console::instance()->debug("vdec zoom rect x:%d", info.stVdec.stZoom.stRect.s32X);
						Console::instance()->debug("vdec zoom rect y:%d", info.stVdec.stZoom.stRect.s32Y);
						Console::instance()->debug("vdec zoom rect widget:%d", info.stVdec.stZoom.stRect.u32Width);
						Console::instance()->debug("vdec zoom rect height:%d", info.stVdec.stZoom.stRect.u32Height);
					} else if (info.stVdec.stZoom.enType == DP_M2S_ZOOM_RATIO) {
						Console::instance()->debug("vdec zoom ratio x:%d", info.stVdec.stZoom.stRatio.u32XRatio);
						Console::instance()->debug("vdec zoom ratio y:%d", info.stVdec.stZoom.stRatio.u32YRatio);
						Console::instance()->debug("vdec zoom ratio widget:%d", info.stVdec.stZoom.stRatio.u32WRatio);
						Console::instance()->debug("vdec zoom ratio height:%d", info.stVdec.stZoom.stRatio.u32HRatio);
					}
					Console::instance()->debug("vdec is osd:%d", info.stVdec.bOsd);
					Console::instance()->debug("vdec osd type :%d", info.stVdec.stOsd.enType);
					if (info.stVdec.stOsd.enType == DP_M2S_OSD_PIC) {
						Console::instance()->debug("vdec osd pic path :%s", info.stVdec.stOsd.au8PicPath);
					} else if (info.stVdec.stOsd.enType == DP_M2S_OSD_STRING) {
						Console::instance()->debug("vdec osd str path :%s", info.stVdec.stOsd.stStr.au8Str);
						Console::instance()->debug("vdec osd str color :%d", info.stVdec.stOsd.stStr.u32Color);
					}
					Console::instance()->debug("vdec osd display mode :%d", info.stVdec.stOsd.enDispMode);
					Console::instance()->debug("vdec osd point x:%d", info.stVdec.stOsd.stPoint.s32X);
					Console::instance()->debug("vdec osd point y:%d", info.stVdec.stOsd.stPoint.s32Y);
					Console::instance()->debug("vdec swms is open:%d", info.stVdec.bSwms);
					Console::instance()->debug("vdec swms vo dev id:%d", info.stVdec.stSwms.s32VoDevId);
					Console::instance()->debug("vdec swms chn:%d", info.stVdec.stSwms.s32SwmsChn);
					Console::instance()->debug("vdec swms priority:%d", info.stVdec.stSwms.u32Priority);
					Console::instance()->debug("vdec swms strec x:%d", info.stVdec.stSwms.stRect.s32X);
					Console::instance()->debug("vdec swms strec y:%d", info.stVdec.stSwms.stRect.s32Y);
					Console::instance()->debug("vdec swms strec widget:%d", info.stVdec.stSwms.stRect.u32Width);
					Console::instance()->debug("vdec swms strec height:%d", info.stVdec.stSwms.stRect.u32Height);
					Console::instance()->debug("vdec alg type:%d", info.stVdec.stAlg.enAlg);
					Console::instance()->debug("vdec alg enprofile:%d", info.stVdec.stAlg.stH264Dec.enProfile);
					Console::instance()->debug("vdec alg stSize widget:%d", info.stVdec.stAlg.stH264Dec.stSize.u32Width);
					Console::instance()->debug("vdec alg stSize height:%d", info.stVdec.stAlg.stH264Dec.stSize.u32Height);
					Console::instance()->debug("####################END####################");
				}
			} else if (pGetInfo->enInfoTYpe == DP_M2S_INFO_TYPE_GET_AO) {
				DP_M2S_AO_SET_INFO_S aoStatus;
				DP_U8 AoNum = (pGetInfo->u32InfoLen / sizeof(DP_M2S_AO_SET_INFO_S));
				for (int i = 0; i < AoNum; i++) {
					memcpy(&aoStatus, &recvBuffer[sizeof(DP_M2S_CMD_GETINFO_RESPOND_S) + sizeof(DP_M2S_AO_SET_INFO_S) * i], sizeof(DP_M2S_AO_SET_INFO_S));
					Console::instance()->debug("##################AO:%d####################", i);
					Console::instance()->debug("dev id :%d", aoStatus.s32DevId);
					Console::instance()->debug("vol:%d", aoStatus.u8Vol);
					Console::instance()->debug("sample:%d", aoStatus.stCommAttr.enSamplerate);
					Console::instance()->debug("bit width:%d", aoStatus.stCommAttr.enBitwidth);
					Console::instance()->debug("sound mode :%d", aoStatus.stCommAttr.enSoundmode);
					Console::instance()->debug("#################END####################");
				}
			} else if (pGetInfo->enInfoTYpe == DP_M2S_INFO_TYPE_GET_VO) {
				DP_M2S_VO_GET_INFO_S voStatus;
				DP_U8 VoNum = (pGetInfo->u32InfoLen / sizeof(DP_M2S_VO_GET_INFO_S));
				for (int i = 0; i < VoNum; i++) {
					memcpy(&voStatus, &recvBuffer[sizeof(DP_M2S_CMD_GETINFO_RESPOND_S) + sizeof(DP_M2S_VO_GET_INFO_S) * i], sizeof(DP_M2S_VO_GET_INFO_S));
					Console::instance()->debug("###################VO:%d####################", i);
					Console::instance()->debug("dev id:%d", voStatus.s32DevId);
					Console::instance()->debug("enable:%d", voStatus.bEnable);
					Console::instance()->debug("enSync :%d", voStatus.enSync);
					for (int j = 0; j < DP_M2S_SWMS_MAX; j++) {
						Console::instance()->debug("-------------------stSwms:%d", j);
						Console::instance()->debug("vo dec id:%d", voStatus.stSwms[j].s32VoDevId);
						Console::instance()->debug("swms chn:%d", voStatus.stSwms[j].s32SwmsChn);
						Console::instance()->debug("priority:%d", voStatus.stSwms[j].u32Priority);
						Console::instance()->debug("stRect x:%d", voStatus.stSwms[j].stRect.s32X);
						Console::instance()->debug("stRect y:%d", voStatus.stSwms[j].stRect.s32Y);
						Console::instance()->debug("stRect widget:%d", voStatus.stSwms[j].stRect.u32Width);
						Console::instance()->debug("stRect height:%d", voStatus.stSwms[j].stRect.u32Height);
					}
					Console::instance()->debug("stCsc sharpen:%d", voStatus.stCsc.u8Sharpen);
					Console::instance()->debug("stCsc luma:%d", voStatus.stCsc.u8Luma);
					Console::instance()->debug("stCsc contrast:%d", voStatus.stCsc.u8Contrast);
					Console::instance()->debug("stCsc hue:%d", voStatus.stCsc.u8Hue);
					Console::instance()->debug("stCsc staturation:%d", voStatus.stCsc.u8Staturation);
					Console::instance()->debug("####################END####################");
				}
			}
		}
		break;
	}
}
//video init commandID:0x02
void UnixSockClientData::SendVideoInit() {
	vector<uint8_t> sendBuffer;
	DP_M2S_CMD_INIT_S init;
	init.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	init.stHeader.u8CommandID = DP_M2S_CMD_INIT;
	init.stHeader.u16PacketLen = sizeof(DP_M2S_CMD_INIT_S) | 0x80;

	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &init, (uint8_t*) &init + sizeof(DP_M2S_CMD_INIT_S));
	addCommand(&sendBuffer.front(), sizeof(DP_M2S_CMD_INIT_S));
}

void UnixSockClientData::SendGetVideoState(DP_M2S_INFO_TYPE_E type) {
	vector<uint8_t> sendBuffer;
	DP_M2S_CMD_GETINFO_S info;
	info.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	info.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
	info.stHeader.u16PacketLen = sizeof(DP_M2S_CMD_GETINFO_S) | 0x80;
	info.enInfoTYpe = type;
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &info, (uint8_t*) &info + sizeof(DP_M2S_CMD_GETINFO_S));
	addCommand(&sendBuffer.front(), sizeof(DP_M2S_CMD_GETINFO_S));
}

void UnixSockClientData::SendStartAVdec(DP_M2S_TSK_ID id, DP_M2S_AVDEC_SET_INFO_S state) {
	vector<uint8_t> sendBuffer;
	DP_M2S_CMD_SETINFO_S info;
	info.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	info.stHeader.u8CommandID = DP_M2S_CMD_SETINFO;
	info.stHeader.u16PacketLen = (sizeof(DP_M2S_CMD_SETINFO_S) + sizeof(DP_M2S_AVDEC_GET_INFO_S)) | 0x80;
	info.enInfoTYpe = DP_M2S_INFO_TYPE_SET_AVDEC;
	info.u32InfoLen = sizeof(DP_M2S_AVDEC_GET_INFO_S);
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &info, (uint8_t*) &info + sizeof(DP_M2S_CMD_SETINFO_S));
	memcpy(&_avdecStatus[id], &state, sizeof(DP_M2S_AVDEC_SET_INFO_S));
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &state, (uint8_t*) &state + sizeof(DP_M2S_AVDEC_SET_INFO_S));
	addCommand(&sendBuffer.front(), info.stHeader.u16PacketLen);
}

void UnixSockClientData::SendChangeAVdec(DP_M2S_TSK_ID id, DP_M2S_AVDEC_SET_INFO_S state) {
	vector<uint8_t> sendBuffer;
	DP_M2S_CMD_SETINFO_S info;
	info.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	info.stHeader.u8CommandID = DP_M2S_CMD_SETINFO;
	info.stHeader.u16PacketLen = (sizeof(DP_M2S_CMD_SETINFO_S) + sizeof(DP_M2S_AVDEC_GET_INFO_S)) | 0x80;
	info.enInfoTYpe = DP_M2S_INFO_TYPE_SET_AVDEC;
	info.u32InfoLen = sizeof(DP_M2S_AVDEC_GET_INFO_S);
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &info, (uint8_t*) &info + sizeof(DP_M2S_CMD_SETINFO_S));
	memcpy(&_avdecStatus[id], &state, sizeof(DP_M2S_AVDEC_SET_INFO_S));
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &state, (uint8_t*) &state + sizeof(DP_M2S_AVDEC_SET_INFO_S));
	addCommand(&sendBuffer.front(), info.stHeader.u16PacketLen);
}

void UnixSockClientData::SendSetAVenc(DP_M2S_TSK_ID id, DP_M2S_AVENC_SET_INFO_S state) {
	vector<uint8_t> sendBuffer;
	DP_M2S_CMD_SETINFO_S info;
	info.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	info.stHeader.u8CommandID = DP_M2S_CMD_SETINFO;
	info.stHeader.u16PacketLen = (sizeof(DP_M2S_CMD_SETINFO_S) + sizeof(DP_M2S_AVENC_SET_INFO_S)) | 0x80;
	info.enInfoTYpe = DP_M2S_INFO_TYPE_SET_AVENC;
	info.u32InfoLen = sizeof(DP_M2S_AVENC_SET_INFO_S);
	Console::instance()->debug("#####################task id:%d", state.TskId);
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &info, (uint8_t*) &info + sizeof(DP_M2S_CMD_SETINFO_S));
	memcpy(&_avencStatus[0], &state, sizeof(DP_M2S_AVENC_SET_INFO_S));
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &state, (uint8_t*) &state + sizeof(DP_M2S_AVENC_SET_INFO_S));
	addCommand(&sendBuffer.front(), info.stHeader.u16PacketLen);
}
void UnixSockClientData::SendStopAVdec(DP_M2S_TSK_ID id, DP_M2S_AVDEC_SET_INFO_S state) {
	vector<uint8_t> sendBuffer;
	DP_M2S_CMD_SETINFO_S info;
	info.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	info.stHeader.u8CommandID = DP_M2S_CMD_SETINFO;
	info.stHeader.u16PacketLen = (sizeof(DP_M2S_CMD_SETINFO_S) + sizeof(DP_M2S_AVDEC_SET_INFO_S)) | 0x80;
	info.enInfoTYpe = DP_M2S_INFO_TYPE_SET_AVDEC;
	info.u32InfoLen = sizeof(DP_M2S_AVDEC_SET_INFO_S);
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &info, (uint8_t*) &info + sizeof(DP_M2S_CMD_SETINFO_S));
	memcpy(&_avencStatus[0], &state, sizeof(DP_M2S_AVDEC_SET_INFO_S));
	sendBuffer.insert(sendBuffer.end(), (uint8_t*) &state, (uint8_t*) &state + sizeof(DP_M2S_AVDEC_SET_INFO_S));
	addCommand(&sendBuffer.front(), info.stHeader.u16PacketLen);
}

#endif
#endif
