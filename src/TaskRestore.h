/*
 * TaskRestore.h
 *
 *  Created on: Jan 2, 2019
 *      Author: jhb
 */

#ifndef SRC_TASKRESTORE_H_
#define SRC_TASKRESTORE_H_

//#include <muduo/base/Condition.h>
//#include <muduo/base/Mutex.h>
//#include <muduo/base/Thread.h>
//
//#include <boost/function.hpp>
//#include <boost/noncopyable.hpp>
//#include <boost/ptr_container/ptr_vector.hpp>
//
//#include <deque>
#include <iostream>
#include <fstream>
#include <muduo/base/ThreadPool.h>
#include <muduo/base/Mutex.h>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <string.h>
#include <map>
#include "dp_sd_type.h"
#include "interactivepro.h"
#include "dp_m2s_prot.h"
#include "xmlParser.h"
#include "NodeInfo.h"

#include <json/json.h>

typedef enum _CloseThirdTask_E {
	_eCloseWindow, _eCloseAudio, _eButt
} CloseThirdTask_E;

#if (InputDevice)
static const DP_CHAR *CODEC_AVDEC_JSON_PATH = "./json/InCodec_AVDec.json";
static const DP_CHAR *THIRD_WINDOW_TASK_JSON_PATH="./json/InThirdWindowTaskInfo.json";
static const DP_CHAR *THIRD_AUDIO_TASK_JSON_PATH="./json/InThirdAudioTaskInfo.json";
#elif (OutputDevice)
static const DP_CHAR *CODEC_AVDEC_JSON_PATH = "./json/OutCodec_AVDec.json";
static const DP_CHAR *THIRD_WINDOW_TASK_JSON_PATH =
		"./json/OutThirdWindowTaskInfo.json";
static const DP_CHAR *THIRD_AUDIO_TASK_JSON_PATH =
		"./json/OutThirdAudioTaskInfo.json";
#endif

#define CODEC_AVDEC_TITLE "Codec AV Dec"
#define THIRD_WINDOW_TITLE "Third Window"
#define THIRD_AUDIO_TITLE "Third Audio"

class TaskRestore: boost::noncopyable {
public:

	TaskRestore();
	~TaskRestore();

	typedef std::map<std::string, DP_U32> MapDataInfo;
	typedef boost::shared_ptr<MapDataInfo> MapDataInfoPtr;

	typedef std::map<std::string, std::string> MapStringInfo;
	typedef boost::shared_ptr<MapStringInfo> MapStringInfoPtr;

	void startRunning(DP_S32 maxDequeSize, DP_S32 numThreads);

	void setDataToJson(DP_M2S_AVENC_INFO_S data) {
		_threadPool.run(
				boost::bind(&TaskRestore::setAVEncInfoToJson, this, &data));
	}

	void setDataToJson(DP_M2S_AVDEC_INFO_S data) {
		_threadPool.run(
				boost::bind(&TaskRestore::setAVDecInfoToJson, this, data));
	}

	void setDataToJson(OpenAndMoveWindow_S data) {
		_threadPool.run(
				boost::bind(&TaskRestore::setWindowInfoToJson, this, &data));
	}
	void setDataToJson(_sRemote_OpenAudio data) {
		_threadPool.run(
				boost::bind(&TaskRestore::setOpenAudioInfoToJson, this, &data));
	}

	void setDataToJson(OpenAndMoveWindow_S data, DP_S32 codecID) {
		_threadPool.run(
				boost::bind(&TaskRestore::setThirdWindowTaskToJson, this, data,
						codecID));
	}

	void setDataToJson(DP_U32 thirdID, DP_S32 codecID,
			DP_M2S_AO_DEV_E AOChnID) {
		_threadPool.run(
				boost::bind(&TaskRestore::setThirdAudioTaskToJson, this,
						thirdID, codecID, AOChnID));
	}

	void setDataToJson(CloseThirdTask_E enClose, DP_U32 thirdID) {
		_threadPool.run(
				boost::bind(&TaskRestore::removeThirdTask, this, enClose,
						thirdID));
	}

	DP_BOOL getAVDecJson(NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo);
	DP_BOOL getThirdInfo(NodeInfo::MapThirdIDSrcVideoInfoPtr &thirdIDSrcVideo,
			NodeInfo::MapOutThirdCodecTaskIDPtr &thirdCodecID,
			NodeInfo::MapAODevIDCodecIDPtr &aoIDCodecID);

private:

	muduo::ThreadPool _threadPool;
	muduo::MutexLock _AVDecMutex;
	muduo::MutexLock _thirdWindowMutex;
	muduo::MutexLock _thirdAudioMutex;

	//useless
	MapDataInfoPtr _mDataInfo;
	MapStringInfoPtr _mStrInfo;
//
	void setAVEncInfoToJson(DP_M2S_AVENC_INFO_S *info);
	void setAVDecInfoToJson(DP_M2S_AVDEC_INFO_S info);

	void setWindowInfoToJson(OpenAndMoveWindow_S *info);

	void setThirdWindowTaskToJson(OpenAndMoveWindow_S info, DP_S32 codecID);

	void setThirdAudioTaskToJson(DP_U32 thirdID, DP_S32 codecID,
			DP_M2S_AO_DEV_E AOChnID);

	void removeThirdTask(CloseThirdTask_E enClose, DP_U32 thirdID);

	void setOpenAudioInfoToJson(_sRemote_OpenAudio *info);

	void avDec2JsonValue(DP_M2S_AVDEC_INFO_S &info, Json::Value &value);
	void jsonValue2AVDec(Json::Value JsonValue,
			NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo);
	void thirdWindowData2JsonValue(OpenAndMoveWindow_S info, DP_S32 codecID,
			Json::Value &value);
	void thirdAudioData2JsonValue(DP_U32 thirdID, DP_S32 codecID,
			DP_M2S_AO_DEV_E AOChnID, Json::Value &value);
//	void jsonValue2ThirdData(Json::Value JsonValue,
//			NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo);

//	void codecDataCastToMap();
//	void thirdDataCastToMap();

};

#endif /* SRC_TASKRESTORE_H_ */
