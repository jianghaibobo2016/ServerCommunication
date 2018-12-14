/*
 * UnixSockClient.h
 *
 *  Created on: 2017年9月20日
 *      Author: root
 */
#ifndef VIDEOLAYER_UNIXSOCKCLIENTDATA_H_
#define VIDEOLAYER_UNIXSOCKCLIENTDATA_H_
#include <TimerUtil.h>
#include <vector>
#include "Handle.h"
#include "SocketLayer.h"
#include "dp_m2s_prot.h"
#include "DevStaticConfigure.h"

//#include "SystemException.h"
typedef int (*recvCallBack)(void* pData, int len);

//template<typename T, typename S>
//typedef int (*NodeInfoRecvCallBack)(void* pData, int len, T vctrGetInfo);
using namespace std;
//using namespace DSPPAUtil;
class UnixSockClientData/*: public TimerUtil*/{
public:
	UnixSockClientData(recvCallBack cb = NULL);
//	UnixSockClientData(NodeInfoRecvCallBack cb = NULL);
	~UnixSockClientData();
//	void addCommand(const void* pData, int len);
	int doSendCommand(const void* pData, int len) /*throw (SystemException)*/;
	int onlySendMsg(const void* pData, int len)/* throw (SystemException)*/;
	uint8_t * getRecvBuff() {
//		DP_U64 sec = 0;
//		DP_U64 usec = 0;
//		struct timeval tnow;
//		struct timespec outtime;
//		gettimeofday(&tnow, NULL);
//		usec = tnow.tv_usec + 1000 * 1000;
//		sec = tnow.tv_sec + usec / 1000000;
//		outtime.tv_nsec = (usec % 1000000) * 1000;
//		outtime.tv_sec = sec;

//		outtime.tv_sec = now.tv_sec + 5;
//		outtime.tv_nsec = now.tv_usec * 1000;
//		if (pthread_cond_timedwait(&_cond, &_condMutex, &outtime) != 0) {
//		cout<< "_buffer:: "<<_buffer<<endl;
//			return NULL;
//		}
		return _buffer;
	}
public:
//	void SendGetVideoState(DP_M2S_INFO_TYPE_E type);
//	void SendVideoInit();
//	void SendStartAVdec(DP_M2S_TSK_ID id, DP_M2S_AVDEC_SET_INFO_S state);
//	void SendChangeAVdec(DP_M2S_TSK_ID id, DP_M2S_AVDEC_SET_INFO_S state);
//	void SendSetAVenc(DP_M2S_TSK_ID id, DP_M2S_AVENC_SET_INFO_S state);
//	void SendStopAVdec(DP_M2S_TSK_ID id,DP_M2S_AVDEC_SET_INFO_S state);

//	void GetAvdecStatus(DP_M2S_TSK_ID id, DP_M2S_AVDEC_GET_INFO_S &state){
//		memcpy(&state,&_avdecStatus[id],sizeof(DP_M2S_AVDEC_GET_INFO_S));
//	}
//	void GetAvencStatus(DP_M2S_TSK_ID id,DP_M2S_AVENC_GET_INFO_S &state){
//		memcpy(&state,&_avencStatus[id],sizeof(DP_M2S_AVENC_GET_INFO_S));
//	}
private:
//	virtual void onTimer(int nID);
	void doRecvCommand(void* pData, int len) /*throw (SystemException)*/;
private:
	Mutex _mutex;
//	pthread_mutex_t _condMutex;
//	pthread_cond_t _cond;
	vector<vector<int8_t> > _packages;
	bool _bInitCommandOK;
	recvCallBack _cb;
	uint8_t *_buffer;
//	NodeInfoRecvCallBack _Nodecb;
//	DP_M2S_AVDEC_GET_INFO_S _avdecStatus[16];
//	DP_M2S_AVENC_GET_INFO_S _avencStatus[16];
};
//typedef Handle<UnixSockClientData> UnixSockClientDataPtr;
#endif /* VIDEOLAYER_UNIXSOCKCLIENTDATA_H_ */
