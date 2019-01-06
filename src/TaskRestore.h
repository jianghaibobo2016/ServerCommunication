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

static const DP_CHAR *CODEC_AVDEC_JSON_PATH = "./json/Codec_AVDec.json";

//static const DP_CHAR CODEC_AVDEC_TITLE[] = "Codec AV Dec";
#define CODEC_AVDEC_TITLE "Codec AV Dec"

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
		std::cout<<"data taks id : "<<data.s32TskId<<std::endl;
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

	DP_BOOL getAVDecJson(NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo);

private:

	muduo::ThreadPool _threadPool;
	mutable muduo::MutexLock _AVDecmutex;

	//useless
	MapDataInfoPtr _mDataInfo;
	MapStringInfoPtr _mStrInfo;
//
	void setAVEncInfoToJson(DP_M2S_AVENC_INFO_S *info);
	void setAVDecInfoToJson(DP_M2S_AVDEC_INFO_S info);

	void setWindowInfoToJson(OpenAndMoveWindow_S *info);
	void setOpenAudioInfoToJson(_sRemote_OpenAudio *info);

	void avDec2JsonValue(DP_M2S_AVDEC_INFO_S &info, Json::Value &value);

//	void codecDataCastToMap();
//	void thirdDataCastToMap();

};

#endif /* SRC_TASKRESTORE_H_ */
