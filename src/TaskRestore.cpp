/*
 * TaskRestore.cpp
 *
 *  Created on: Jan 2, 2019
 *      Author: jhb
 */

//#include <muduo/base/Exception.h>
//#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <muduo/base/Logging.h>
#include <muduo/base/Singleton.h>
#include "TaskRestore.h"
#include "Data.h"
//#include "NodeInfo.h"

TaskRestore::TaskRestore() :
		_threadPool("TaskRestore"), _mDataInfo(new MapDataInfo), _mStrInfo(
				new MapStringInfo) {

}
TaskRestore::~TaskRestore() {

}

void TaskRestore::startRunning(DP_S32 maxDequeSize, DP_S32 numThreads) {
	_threadPool.setMaxQueueSize(maxDequeSize);
	_threadPool.start(numThreads);
}

void TaskRestore::setAVEncInfoToJson(DP_M2S_AVENC_INFO_S *info) {

}

void TaskRestore::setAVDecInfoToJson(DP_M2S_AVDEC_INFO_S info) {
	Json::Reader JsonReader;
	Json::StyledWriter JsonSWriter;
	Json::Value JsonValue;
	Json::Value newValue;
	std::ifstream IFStream;
	std::ofstream OFStream;
	DP_BOOL parseOk = DP_TRUE;
	muduo::MutexLockGuard lock(_AVDecMutex);
	IFStream.open(CODEC_AVDEC_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		parseOk = DP_FALSE;
//		IFStream.close();
		//could not parse json file to value
		LOG_INFO << "Could not parse json file to value";
		const DP_CHAR strJson_2[] = "{\"" CODEC_AVDEC_TITLE "\": []}";
		if (!JsonReader.parse(strJson_2, JsonValue)) {
			LOG_ERROR << "Parse failed!";
			return;
		}
	}
	IFStream.close();
	avDec2JsonValue(info, newValue);
	DP_U32 count = JsonValue[CODEC_AVDEC_TITLE].size();
	DP_U32 index = 0;
	DP_BOOL notFound = DP_TRUE;
	if (parseOk == DP_TRUE && count > 0) {
		while (notFound) {
			//jhbnote ismember! check
			std::cout << "pars ok count " << count << std::endl;
			if (JsonValue[CODEC_AVDEC_TITLE][--count]["s32TskId"].asInt()
					== info.s32TskId) {
				notFound = DP_FALSE;
			} else if (count == 0) {
				std::cout << "count 0 " << std::endl;
				break;
			}
		}
	}
	if (!notFound) {
		std::cout << "!notFound " << std::endl;
		JsonValue[CODEC_AVDEC_TITLE][count] = newValue;
	} else {
		std::cout << "notFound " << std::endl;
		JsonValue[CODEC_AVDEC_TITLE].append(newValue);
	}
	Json::StyledWriter oJsonSWriter;
	std::string strBetterWrite = oJsonSWriter.write(JsonValue);

	OFStream.open(CODEC_AVDEC_JSON_PATH);
	OFStream << strBetterWrite;
	OFStream.close();
}

void TaskRestore::setWindowInfoToJson(OpenAndMoveWindow_S *info) {
//	sleep(1);
	std::cout << "111111111task id : " << info->u32TaskID << std::endl;
	std::ofstream oOFStream;
	std::ifstream oIFStream;
	Json::Reader oJsonReader;
	Json::Value oJsonValue;
	oIFStream.open(THIRD_WINDOW_TASK_JSON_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();
#if 0
		DP_DEV_PUBLIC_USER_INFO_S use_1;
		memset(&use_1, 0, sizeof(DP_DEV_PUBLIC_USER_INFO_S));
		memcpy(use_1.au8PassWord, "us1", 3);
		memcpy(use_1.au8UserName, "name1", 5);
		use_1.u8Priority = 1;
		DP_DEV_PUBLIC_USER_INFO_S use_2;
		memset(&use_2, 0, sizeof(DP_DEV_PUBLIC_USER_INFO_S));
		memcpy(use_2.au8PassWord, "us2", 3);
		memcpy(use_2.au8UserName, "name2", 5);
		use_2.u8Priority = 2;

		map<string, DP_DEV_PUBLIC_USER_INFO_S> m_mPubUserInfo;

		m_mPubUserInfo["str1"] = use_1;
		m_mPubUserInfo["str2"] = use_2;

		std::stringstream strJson;
		map<string, DP_DEV_PUBLIC_USER_INFO_S>::iterator itor, last =
		m_mPubUserInfo.end();

		strJson << "{\"" << "PubUserInfo" << "\":[";
		for (itor = m_mPubUserInfo.begin(); itor != last; itor++) {
			strJson << "{";
			strJson << "\"au8UserName\":" << "\"" << itor->second.au8UserName
			<< "\",";
			strJson << "\"au8PassWord\":" << "\"" << itor->second.au8PassWord
			<< "\",";
			strJson << "\"u8Priority\":"
			/*<< "\""*/<< (DP_S32) itor->second.u8Priority
			/*		<< "\""*/;
			strJson << "}";
			if ((++itor) != last) {
				strJson << ",";
			}
			itor--;
		}
		strJson << "]}";
#endif
		Json::Reader oJsonReader;
		Json::StyledWriter oJsonSWriter;
		Json::Value oJsonRoot;
		std::ofstream oOFStream;
		const char* strJson_2 =
				"{\"DevList\": { \"TaskID\": [{ \"DevSn\":11111 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}}";
//		std::cout << "parsssssssssssssssss " << strJson.str() << std::endl;

//		const char* strJson_2 = "{\"Codec AV Dec\": []}";

//		if (oJsonReader.parse(strJson_2, oJsonRoot)) {
//			std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);
//			oOFStream.open(CODEC_AVDEC_JSON_PATH);
//			oOFStream << strBetterWrite;
//			oOFStream.close();
//			std::cout << "pars ok1 " << std::endl;
//		}

		if (oJsonReader.parse(strJson_2, oJsonRoot)) {
			std::cout << "pars ok1 " << std::endl;
//			oIFStream.open(CODEC_AVDEC_JSON_PATH, std::ios::binary);
//			if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
//			Json::Value newValue;
//			DP_M2S_AVDEC_INFO_S info;
//			avDec2JsonValue(info, newValue);
//			oJsonRoot["Codec AV Dec"].append(newValue);
			std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);
			oOFStream.open(THIRD_WINDOW_TASK_JSON_PATH);
			oOFStream << strBetterWrite;
			oOFStream.close();
//			} else {
//				std::cout << "pars ok3 " << std::endl;
//
//			}
//			oIFStream.close();
		}
//		Json::Value newValue;
//		DP_M2S_AVDEC_INFO_S info;
//		avDec2JsonValue(info, newValue);
//		if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
//			std::cout << "pars ok" << std::endl;
//			std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);
//			oOFStream.open(CODEC_AVDEC_JSON_PATH);
//			oOFStream << strBetterWrite;
//			oOFStream.close();
//		}

#if 0
		Json::Value oJsonRoot_test;
		oJsonRoot_test["Devsn"] = "000";
		oJsonRoot_test["DevType"] = 93;

		Json::Value oJsonRoot_test_2;
		oJsonRoot_test_2["Devsn"] = "1111";
		oJsonRoot_test_2["DevType"] = 9;

		Json::Value array;
		oJsonValue["dev 1"] = oJsonRoot_test;
		array.append(oJsonValue);
		oJsonValue.clear();
		oJsonValue["dev 2"] = oJsonRoot_test_2;
		array.append(oJsonValue);
//		oJsonReader.parse(strJsonApp, tmpAppend);

		Json::StyledWriter oJsonSWriter;
		std::string strBetterWrite = oJsonSWriter.write(array);

		oOFStream.open(CODEC_AVDEC_JSON_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();

#endif
#if 0
		const char* strJson =
		"{\"DevList\": [{ \"DevSn\":11111 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}";
		Json::Reader oJsonReader;
		Json::StyledWriter oJsonSWriter;
		Json::Value oJsonRoot;
		std::ofstream oOFStream;

		if (oJsonReader.parse(strJson, oJsonRoot)) {

//			oJsonRoot["DevListesttt"];
//			oJsonRoot["DevListesttt"]["DevSn"]="cccc";

			const char* strJsonApp =
			"{\"DevLisTTT\": [{ \"DevSn\":4444444444 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}";
			Json::Value tmpAppend;
			oJsonReader.parse(strJsonApp, tmpAppend);

			std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);
			std::string strBetterWrite_2 = oJsonSWriter.write(tmpAppend);

			oOFStream.open(CODEC_AVDEC_JSON_PATH);
			oOFStream << strBetterWrite;
			oOFStream << strBetterWrite_2;
			oOFStream.close();
		}
#endif

#if 0
		Json::Value jsonItem;
		Json::StyledWriter oJsonSWriterApp;
		const char* strJsonApp =
		"{\"DevList\": [{ \"DevSn\":22222222222 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}";
		Json::Reader oJsonReaderApp;
		oJsonReaderApp.parse(strJsonApp, jsonItem);
		oJsonValue.append(jsonItem);
		std::string strBetterWrite = oJsonSWriterApp.write(oJsonValue);

		oOFStream.open(CODEC_AVDEC_JSON_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();

#endif
		std::cout << "2222222222222" << std::endl;
	} else {
		oIFStream.close();
//		if (oJsonValue.isMember("dev 2")) {
		std::cout << "3333333333333" << std::endl;

//			if (oJsonValue.isMember("dev 3")) {
//		Json::Value getValue;
//		getValue = oJsonValue.get(1, oJsonValue);
//				oJsonValue.get(2, getValue);

		Json::Value::Members oMember;
		oMember = oJsonValue["DevList"]["TaskID"][0].getMemberNames();
//
//		for(Json::Value::Members::iterator iter = oMember.begin();
//						iter != oMember.end(); iter++){
//
//		std::cout<<"member: "<<*iter<<std::endl;
//		}
		std::cout << "member: " << *oMember.begin() << std::endl;
		Json::Value vatmp;
		if (oJsonValue["DevList"]["TaskID"].removeIndex(1, &vatmp)) {
			std::cout << "remove 1 " << std::endl;
		} else {
			std::cout << "remove 2" << std::endl;
		}
		Json::StyledWriter oJsonSWriter_2;
		std::string strBetterWrite_2 = oJsonSWriter_2.write(oJsonValue);
		oOFStream.open(THIRD_WINDOW_TASK_JSON_PATH);
		oOFStream << strBetterWrite_2;
		oOFStream.close();
//		std::cout << "finddev  3333333" << "size: " /*<< oJsonValue.size()*/
//		<< " Devsn: "
//				<< oJsonValue["Codec AV Dec"][0]["stStream"]["_rtsp"]["stRtspServer"]["s32ConnMax"].asInt()
//				<< std::endl;

		return;
		Json::Value newValue;
		Json::StyledWriter oJsonSWriter;
		DP_M2S_AVDEC_INFO_S info;
		info.s32TskId = 99999;
		avDec2JsonValue(info, newValue);
		DP_U32 count = oJsonValue.size();
		DP_U32 index = 0;
		DP_BOOL notFound = DP_TRUE;
		while (notFound) {
			std::cout << "pars ok count " << count << std::endl;
			if (oJsonValue["Codec AV Dec"][--count]["s32TskId"].asInt()
					== 100) {
				notFound = DP_FALSE;
			} else if (count == 0) {
				break;
			}
		}
		if (!notFound) {
			oJsonValue["Codec AV Dec"][count] = newValue;
		} else {
			std::cout << "pars ok count return " << count << std::endl;
			return;
		}
		std::cout << "pars ok 12 " << std::endl;
//		oJsonValue["Codec AV Dec"].append(newValue);
		std::string strBetterWrite = oJsonSWriter.write(oJsonValue);
		oOFStream.open(THIRD_WINDOW_TASK_JSON_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();

//				<< " Devsn: "
//				<< getValue.get(1, defaultValue)  .asInt()
//				<< std::endl;
//				oJsonValue.removeMember("dev:100");

//			} else {
//				std::cout << "not finddev  33333" << std::endl;
//			}
		{
//			Json::Value oJsonRoot_test_3;
//			oJsonRoot_test_3["Devsn"] = "111";
//			oJsonRoot_test_3["DevType"] = 3444;
//
//			int a = 100;
//			DP_CHAR taskID[16] = { 0 };
//			sprintf(taskID, "dev:%d", a);
//			oJsonValue[taskID] = oJsonRoot_test_3;
//			Json::StyledWriter oJsonSWriter;
//			std::string strBetterWrite = oJsonSWriter.write(oJsonValue);
//
//			oOFStream.open(CODEC_AVDEC_JSON_PATH);
//			oOFStream << strBetterWrite;
//			oOFStream.close();
		}
//		} else {
//			std::cout << "3333333333333*************" << std::endl;
//		}

		return;
//		Json::Value jsonItem;
//		Json::StyledWriter oJsonSWriterApp;
//		const char* strJsonApp =
//				"{\"DevLisTTT\": [{ \"DevSn\":4444444444 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}";
//		Json::Reader oJsonReaderApp;
//		oJsonReaderApp.parse(strJsonApp, jsonItem);
//		oJsonValue.append(jsonItem);
//		std::string strBetterWrite = oJsonSWriterApp.write(oJsonValue);
//
//		oOFStream.open(CODEC_AVDEC_JSON_PATH);
//		oOFStream << strBetterWrite;
//		oOFStream.close();
#if 0
		oIFStream.close();
		DP_CHAR taskID[6] = {0};
		sprintf(taskID, "%d", info->u32TaskID);
		if (oJsonValue.isMember(taskID)) {
			std::cout << "444444444444" << std::endl;
		} else {
			std::cout << "5555555555" << std::endl;

			const char* strJson =
			"{\"DevList\": [{ \"DevSn\":0 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}";
			Json::Reader oJsonReader;
			Json::StyledWriter oJsonSWriter;
			Json::Value oJsonRoot;
			std::ofstream oOFStream;

			if (oJsonReader.parse(strJson, oJsonRoot)) {
				std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

				oOFStream.open(CODEC_AVDEC_JSON_PATH);
				oOFStream << strBetterWrite;
				oOFStream.close();
			}
		}
#endif
	}
}

void TaskRestore::setThirdWindowTaskToJson(OpenAndMoveWindow_S info,
		DP_U32 priority, DP_S32 codecID) {
	Json::Reader JsonReader;
	Json::StyledWriter JsonSWriter;
	Json::Value JsonValue;
	Json::Value newValue;
	std::ifstream IFStream;
	std::ofstream OFStream;
	DP_BOOL parseOk = DP_TRUE;
	muduo::MutexLockGuard lock(_thirdWindowMutex);
	IFStream.open(THIRD_WINDOW_TASK_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		parseOk = DP_FALSE;
//		IFStream.close();
		//could not parse json file to value
		LOG_INFO << "Could not parse json file to value";
		const DP_CHAR strJson_2[] = "{\"" THIRD_WINDOW_TITLE "\": []}";
		if (!JsonReader.parse(strJson_2, JsonValue)) {
			LOG_ERROR << "Parse failed!";
			return;
		}
	}
	IFStream.close();
	thirdWindowData2JsonValue(info, priority, codecID, newValue);
	DP_U32 count = JsonValue[THIRD_WINDOW_TITLE].size();
	DP_U32 index = 0;
	DP_BOOL notFound = DP_TRUE;
	if (parseOk == DP_TRUE && count > 0) {
		while (notFound) {
			//jhbnote ismember! check
			if (JsonValue[THIRD_WINDOW_TITLE][--count]["u32TaskID"].asInt()
					== info.u32TaskID) {
				notFound = DP_FALSE;
			} else if (count == 0) {
				std::cout << "count 0 " << std::endl;
				break;
			}
		}
	}
	if (!notFound) {
		std::cout << "!notFound " << std::endl;
		JsonValue[THIRD_WINDOW_TITLE][count] = newValue;
	} else {
		std::cout << "notFound " << std::endl;
		JsonValue[THIRD_WINDOW_TITLE].append(newValue);
	}
	Json::StyledWriter oJsonSWriter;
	std::string strBetterWrite = oJsonSWriter.write(JsonValue);
	OFStream.open(THIRD_WINDOW_TASK_JSON_PATH);
	OFStream << strBetterWrite;
	OFStream.close();
}

void TaskRestore::setThirdAudioTaskToJson(DP_U32 thirdID, DP_S32 codecID,
		DP_M2S_AO_DEV_E AOChnID) {
	Json::Reader JsonReader;
	Json::StyledWriter JsonSWriter;
	Json::Value JsonValue;
	Json::Value newValue;
	std::ifstream IFStream;
	std::ofstream OFStream;
	DP_BOOL parseOk = DP_TRUE;
	muduo::MutexLockGuard lock(_thirdAudioMutex);
	IFStream.open(THIRD_AUDIO_TASK_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		parseOk = DP_FALSE;
		//		IFStream.close();
		//could not parse json file to value
		LOG_INFO << "Could not parse json file to value";
		const DP_CHAR strJson_2[] = "{\"" THIRD_AUDIO_TITLE "\": []}";
		if (!JsonReader.parse(strJson_2, JsonValue)) {
			LOG_ERROR << "Parse failed!";
			return;
		}
	}
	IFStream.close();
	thirdAudioData2JsonValue(thirdID, codecID, AOChnID, newValue);
	DP_U32 count = JsonValue[THIRD_AUDIO_TITLE].size();
	DP_U32 index = 0;
	DP_BOOL notFound = DP_TRUE;
	if (parseOk == DP_TRUE && count > 0) {
		while (notFound) {
			//jhbnote ismember! check
			if (JsonValue[THIRD_AUDIO_TITLE][--count]["codecID"].asInt()
					== codecID) {
				notFound = DP_FALSE;
			} else if (count == 0) {
				std::cout << "count 0 " << std::endl;
				break;
			}
		}
	}
	if (!notFound) {
		std::cout << "!notFound " << std::endl;
		JsonValue[THIRD_AUDIO_TITLE][count] = newValue;
	} else {
		std::cout << "notFound " << std::endl;
		JsonValue[THIRD_AUDIO_TITLE].append(newValue);
	}
	Json::StyledWriter oJsonSWriter;
	std::string strBetterWrite = oJsonSWriter.write(JsonValue);
	OFStream.open(THIRD_AUDIO_TASK_JSON_PATH);
	OFStream << strBetterWrite;
	OFStream.close();
}

void TaskRestore::removeThirdTask(CloseThirdTask_E enClose, DP_U32 thirdID) {
	Json::Reader JsonReader;
	Json::StyledWriter JsonSWriter;
	Json::Value JsonValue;
	Json::Value JsonValue_tmp;
	std::ifstream IFStream;
	std::ofstream OFStream;
	DP_BOOL notFound = DP_TRUE;
	if (enClose == _eCloseWindow) {
		muduo::MutexLockGuard lock(_thirdWindowMutex);
		IFStream.open(THIRD_WINDOW_TASK_JSON_PATH, std::ios::binary);
		if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
			LOG_ERROR << "Can not find third id : " << thirdID << " in json";
			return;
		} else {
			DP_U32 count = JsonValue[THIRD_WINDOW_TITLE].size();
			while (count--) {
				if (JsonValue[THIRD_WINDOW_TITLE][count]["u32TaskID"].asUInt()
						== thirdID) {
					if (!JsonValue[THIRD_WINDOW_TITLE].removeIndex(count,
							&JsonValue_tmp)) {
						LOG_ERROR << "Remove thirdID : " << thirdID
								<< " in json failed!";
						return;
					} else {
						notFound = DP_FALSE;
					}
				} else {
				}
			}
			if (notFound) {
				LOG_ERROR << "Find thirdID : " << thirdID << " in json failed!";
				return;
			}
		}
	} else if (enClose == _eCloseAudio) {
		muduo::MutexLockGuard lock(_thirdAudioMutex);
		IFStream.open(THIRD_AUDIO_TASK_JSON_PATH, std::ios::binary);
		if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
			LOG_ERROR << "Can not parser and find third id : " << thirdID << " in json";
			return;
		} else {
			DP_U32 count = JsonValue[THIRD_AUDIO_TITLE].size();
			while (count--) {
				if (JsonValue[THIRD_AUDIO_TITLE][count]["thirdID"].asUInt()
						== thirdID) {
					if (!JsonValue[THIRD_AUDIO_TITLE].removeIndex(count,
							&JsonValue_tmp)) {
						LOG_ERROR << "Remove thirdID : " << thirdID
								<< " in json failed!";
						return;
					} else {
						notFound = DP_FALSE;
					}
				} else {
				}
			}
			if (notFound) {
				LOG_ERROR << "Find thirdID : " << thirdID << " in json failed!";
				return;
			}
		}
	}
	Json::StyledWriter oJsonSWriter;
	std::string strBetterWrite = oJsonSWriter.write(JsonValue);
	if (enClose == _eCloseWindow) {
		OFStream.open(THIRD_WINDOW_TASK_JSON_PATH);

	} else if (enClose == _eCloseAudio) {
		OFStream.open(THIRD_AUDIO_TASK_JSON_PATH);
	}
	OFStream << strBetterWrite;
	OFStream.close();
}

DP_BOOL TaskRestore::getAVDecJson(NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo) {
	Json::Reader JsonReader;
	Json::Value JsonValue;
	std::ifstream IFStream;
//	muduo::MutexLockGuard lock(_AVDecmutex);
	IFStream.open(CODEC_AVDEC_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
//		IFStream.close();
		//could not parse json file to value
		LOG_INFO << "Could not parse json file to value";
		for (NodeInfo::VctrAVDECGetInfo::iterator it = AVDecInfo->begin();
				it != AVDecInfo->end(); it++) {
			setDataToJson(*it);
		}
	} else {
		jsonValue2AVDec(JsonValue, AVDecInfo);
	}
	IFStream.close();
	return DP_TRUE;
}

DP_BOOL TaskRestore::getThirdInfo(
		NodeInfo::MapThirdIDSrcVideoInfoPtr &thirdIDSrcVideo,
		NodeInfo::MapOutThirdCodecTaskIDPtr &thirdCodecID,
		NodeInfo::MapAODevIDCodecIDPtr &aoIDCodecID) {
	Json::Reader JsonReader;
	Json::Value JsonValue;
	std::ifstream IFStream;

//	NodeInfo::MapThirdIDSrcVideoInfoPtr thirdIDSrcVideo = muduo::Singleton<
//			NodeInfo>::instance().getThirdIDSrcVideoInfo();
//	NodeInfo::MapOutThirdCodecTaskIDPtr thirdCodecID =
//			muduo::Singleton<NodeInfo>::instance().getOutThirdCodecTaskID();

//	NodeInfo::VctrWindowPriorityPtr priority =
//			muduo::Singleton<NodeInfo>::instance().getVctrWindowPriority();
//
//	NodeInfo::MapAODevIDCodecIDPtr aoIDCodecID =
//			muduo::Singleton<NodeInfo>::instance().getAODevIDCodecID();
//	muduo::Singleton<NodeInfo>::instance().getAudioTaskIDMap();

	//window
	IFStream.open(THIRD_WINDOW_TASK_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		LOG_INFO
				<< "Could not parse THIRD_WINDOW_TASK_JSON_PATH json file to value";
	} else if (JsonValue.isMember(THIRD_WINDOW_TITLE)) {
		thirdCodecID->clear();
		thirdIDSrcVideo->clear();
		boost::shared_ptr<_sSrcVideoInfo> srcVideo(new _sSrcVideoInfo);
		for (DP_U32 count = 0; count < JsonValue[THIRD_WINDOW_TITLE].size();
				count++) {
			thirdCodecID->insert(
					NodeInfo::MapOutThirdCodecTaskID::value_type(
							JsonValue[THIRD_WINDOW_TITLE][count]["u32TaskID"].asUInt(),
							JsonValue[THIRD_WINDOW_TITLE][count]["codecID"].asUInt()));

			srcVideo->u16VideoWidth =
					JsonValue[THIRD_WINDOW_TITLE][count]["srcVideoInfo"]["u16VideoWidth"].asInt();
			srcVideo->u16VideoHeight =
					JsonValue[THIRD_WINDOW_TITLE][count]["srcVideoInfo"]["u16VideoHeight"].asInt();
			srcVideo->u16StartX =
					JsonValue[THIRD_WINDOW_TITLE][count]["srcVideoInfo"]["u16StartX"].asInt();
			srcVideo->u16StartY =
					JsonValue[THIRD_WINDOW_TITLE][count]["srcVideoInfo"]["u16StartY"].asInt();
			srcVideo->u16EndX =
					JsonValue[THIRD_WINDOW_TITLE][count]["srcVideoInfo"]["u16EndX"].asInt();
			srcVideo->u16EndY =
					JsonValue[THIRD_WINDOW_TITLE][count]["srcVideoInfo"]["u16EndY"].asInt();

			thirdIDSrcVideo->insert(
					NodeInfo::MapThirdIDSrcVideoInfo::value_type(
							JsonValue[THIRD_WINDOW_TITLE][count]["u32TaskID"].asUInt(),
							*srcVideo.get()));
		}
	} else {
		LOG_INFO << "Could not get THIRD_WINDOW_TITLE";
	}
	IFStream.close();

	//audio
	IFStream.open(THIRD_AUDIO_TASK_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		LOG_INFO
				<< "Could not parse THIRD_AUDIO_TASK_JSON_PATH json file to value";
	} else if (JsonValue.isMember(THIRD_AUDIO_TITLE)) {
		aoIDCodecID->clear();
		for (DP_U32 count = 0; count < JsonValue[THIRD_AUDIO_TITLE].size();
				count++) {
			aoIDCodecID->insert(
					NodeInfo::MapAODevIDCodecID::value_type(
							(DP_M2S_AO_DEV_E) JsonValue[THIRD_AUDIO_TITLE][count]["AOChnID"].asUInt(),
							JsonValue[THIRD_AUDIO_TITLE][count]["codecID"].asUInt()));
			thirdCodecID->insert(
					NodeInfo::MapOutThirdCodecTaskID::value_type(
							JsonValue[THIRD_AUDIO_TITLE][count]["thirdID"].asUInt(),
							JsonValue[THIRD_AUDIO_TITLE][count]["codecID"].asUInt()));
		}
	} else {
		LOG_INFO << "Could not get THIRD_AUDIO_TITLE";
	}
	IFStream.close();
	return DP_TRUE;
}

void TaskRestore::avDec2JsonValue(DP_M2S_AVDEC_INFO_S &info,
		Json::Value &value) {
	value["s32TskId"] = info.s32TskId;
	value["AvBindAttr"]["enBindType"] = info.AvBindAttr.enBindType;

	value["AvBindAttr"]["stAudio"]["stIn"]["ModId"] =
			info.AvBindAttr.stAudio.stIn.ModId;
	value["AvBindAttr"]["stAudio"]["stIn"]["u32DevId"] =
			info.AvBindAttr.stAudio.stIn.u32DevId;
	value["AvBindAttr"]["stAudio"]["stIn"]["u32ChnId"] =
			info.AvBindAttr.stAudio.stIn.u32ChnId;
//	std::cout << "pars ok6.1 " << std::endl;

	value["AvBindAttr"]["stAudio"]["stOut"]["ModId"] =
			info.AvBindAttr.stAudio.stIn.ModId;
	value["AvBindAttr"]["stAudio"]["stOut"]["u32DevId"] =
			info.AvBindAttr.stAudio.stIn.u32DevId;
	value["AvBindAttr"]["stAudio"]["stOut"]["u32ChnId"] =
			info.AvBindAttr.stAudio.stIn.u32ChnId;

	value["AvBindAttr"]["stVideo"]["stIn"]["ModId"] =
			info.AvBindAttr.stVideo.stIn.ModId;
	value["AvBindAttr"]["stVideo"]["stIn"]["u32DevId"] =
			info.AvBindAttr.stVideo.stIn.u32DevId;
	value["AvBindAttr"]["stVideo"]["stIn"]["u32ChnId"] =
			info.AvBindAttr.stVideo.stIn.u32ChnId;

	value["AvBindAttr"]["stVideo"]["stOut"]["ModId"] =
			info.AvBindAttr.stVideo.stIn.ModId;
	value["AvBindAttr"]["stVideo"]["stOut"]["u32DevId"] =
			info.AvBindAttr.stVideo.stIn.u32DevId;
	value["AvBindAttr"]["stVideo"]["stOut"]["u32ChnId"] =
			info.AvBindAttr.stVideo.stIn.u32ChnId;

	value["stStream"]["enType"] = info.stStream.enType;
//	std::cout << "pars ok6.11 " << std::endl;
	if (info.stStream.enType == DP_M2S_STREAM_RTSP_SERVER) {
		value["stStream"]["_rtsp"]["stRtspServer"]["bOpen"] =
				info.stStream._rtsp.stRtspServer.bOpen;
		value["stStream"]["_rtsp"]["stRtspServer"]["bUDP"] =
				info.stStream._rtsp.stRtspServer.bUDP;
		value["stStream"]["_rtsp"]["stRtspServer"]["bMulticast"] =
				info.stStream._rtsp.stRtspServer.bMulticast;
		value["stStream"]["_rtsp"]["stRtspServer"]["s32ConnTimeout"] =
				info.stStream._rtsp.stRtspServer.s32ConnTimeout;
		value["stStream"]["_rtsp"]["stRtspServer"]["s32ConnMax"] =
				info.stStream._rtsp.stRtspServer.s32ConnMax;
		value["stStream"]["_rtsp"]["stRtspServer"]["s32ConnNums"] =
				info.stStream._rtsp.stRtspServer.s32ConnNums;
		value["stStream"]["_rtsp"]["stRtspServer"]["au8Url"] =
				(DP_CHAR*) info.stStream._rtsp.stRtspServer.au8Url;
	} else if (info.stStream.enType == DP_M2S_STREAM_RTSP_CLIENT) {
		value["stStream"]["_rtsp"]["stRtspClient"]["s8Open"] =
				info.stStream._rtsp.stRtspClient.s8Open;
		value["stStream"]["_rtsp"]["stRtspClient"]["bUDP"] =
				info.stStream._rtsp.stRtspClient.bUDP;
		value["stStream"]["_rtsp"]["stRtspClient"]["bMulticast"] =
				info.stStream._rtsp.stRtspClient.bMulticast;
		value["stStream"]["_rtsp"]["stRtspClient"]["s32ConnTimeout"] =
				info.stStream._rtsp.stRtspClient.s32ConnTimeout;
		value["stStream"]["_rtsp"]["stRtspClient"]["au8Url"] =
				(DP_CHAR*) info.stStream._rtsp.stRtspClient.au8Url;
	}

	value["stAdec"]["enAlg"] = info.stAdec.enAlg;

	value["stVdec"]["bCrop"] = info.stVdec.bCrop;
	value["stVdec"]["bZoom"] = info.stVdec.bZoom;
	value["stVdec"]["bOsd"] = info.stVdec.bOsd;
	value["stVdec"]["bSwms"] = info.stVdec.bSwms;

	value["stVdec"]["stAlg"]["enAlg"] = info.stVdec.stAlg.enAlg;
//	std::cout << "pars ok6.2 " << std::endl;
	if (info.stVdec.stAlg.enAlg == DP_M2S_ALG_H264_ENC) {
		value["stVdec"]["stAlg"]["stH264Enc"]["u32FrmRate"] =
				info.stVdec.stAlg.stH264Enc.u32FrmRate;
		value["stVdec"]["stAlg"]["stH264Enc"]["stSize"]["u32Width"] =
				info.stVdec.stAlg.stH264Enc.stSize.u32Width;
		value["stVdec"]["stAlg"]["stH264Enc"]["stSize"]["u32Height"] =
				info.stVdec.stAlg.stH264Enc.stSize.u32Height;
		value["stVdec"]["stAlg"]["stH264Enc"]["enRcMode"] =
				info.stVdec.stAlg.stH264Enc.enRcMode;
		value["stVdec"]["stAlg"]["stH264Enc"]["u32Bitrate"] =
				info.stVdec.stAlg.stH264Enc.u32Bitrate;
		value["stVdec"]["stAlg"]["stH264Enc"]["enProfile"] =
				info.stVdec.stAlg.stH264Enc.enProfile;
		value["stVdec"]["stAlg"]["stH264Enc"]["u32Gop"] =
				info.stVdec.stAlg.stH264Enc.u32Gop;
		value["stVdec"]["stAlg"]["stH264Enc"]["u16SF"] =
				info.stVdec.stAlg.stH264Enc.u16SF;
		value["stVdec"]["stAlg"]["stH264Enc"]["u16TF"] =
				info.stVdec.stAlg.stH264Enc.u16TF;
	} else if (info.stVdec.stAlg.enAlg == DP_M2S_ALG_H264_DEC) {
		value["stVdec"]["stAlg"]["enAlg"] = info.stVdec.stAlg.enAlg;
		value["stVdec"]["stAlg"]["stH264Dec"]["enProfile"] =
				info.stVdec.stAlg.stH264Dec.enProfile;
		value["stVdec"]["stAlg"]["stH264Dec"]["stSize"]["u32Width"] =
				info.stVdec.stAlg.stH264Dec.stSize.u32Width;
		value["stVdec"]["stAlg"]["stH264Dec"]["stSize"]["u32Height"] =
				info.stVdec.stAlg.stH264Dec.stSize.u32Height;
	} else if (info.stVdec.stAlg.enAlg == DP_M2S_ALG_AAC_ENC) {
		value["stVdec"]["stAlg"]["stAACEnc"]["u32Bitrate"] =
				info.stVdec.stAlg.stAACEnc.u32Bitrate;
		value["stVdec"]["stAlg"]["stAACEnc"]["bAdts"] =
				info.stVdec.stAlg.stAACEnc.bAdts;
	} else if (info.stVdec.stAlg.enAlg == DP_M2S_ALG_AAC_DEC) {
		value["stVdec"]["stAlg"]["stAACDec"]["bAdts"] =
				info.stVdec.stAlg.stAACDec.bAdts;
	}
//	std::cout << "pars ok6.3 " << std::endl;
	value["stVdec"]["stCrop"]["s32X"] = info.stVdec.stCrop.s32X;
	value["stVdec"]["stCrop"]["s32Y"] = info.stVdec.stCrop.s32Y;
	value["stVdec"]["stCrop"]["u32Width"] = info.stVdec.stCrop.u32Width;
	value["stVdec"]["stCrop"]["u32Height"] = info.stVdec.stCrop.u32Height;

	value["stVdec"]["stZoom"]["enType"] = info.stVdec.stZoom.enType;
//	std::cout << "pars ok6.31 " << std::endl;
	if (info.stVdec.stZoom.enType == DP_M2S_ZOOM_RECT) {
//		std::cout << "pars ok6.3111 " << std::endl;
		value["stVdec"]["stZoom"]["stRect"]["s32X"] =
				info.stVdec.stZoom.stRect.s32X;
//		std::cout << "pars ok6.3112 " << std::endl;
		value["stVdec"]["stZoom"]["stRect"]["s32Y"] =
				info.stVdec.stZoom.stRect.s32Y;
		value["stVdec"]["stZoom"]["stRect"]["u32Width"] =
				info.stVdec.stZoom.stRect.u32Width;
		value["stVdec"]["stZoom"]["stRect"]["u32Height"] =
				info.stVdec.stZoom.stRect.u32Height;
	} else if (info.stVdec.stZoom.enType == DP_M2S_ZOOM_RATIO) {
//		std::cout << "pars ok6.32 " << std::endl;
		value["stVdec"]["stZoom"]["stRatio"]["u32XRatio"] =
				info.stVdec.stZoom.stRatio.u32XRatio;
		value["stVdec"]["stZoom"]["stRatio"]["u32YRatio"] =
				info.stVdec.stZoom.stRatio.u32YRatio;
		value["stVdec"]["stZoom"]["stRatio"]["u32WRatio"] =
				info.stVdec.stZoom.stRatio.u32WRatio;
		value["stVdec"]["stZoom"]["stRatio"]["u32HRatio"] =
				info.stVdec.stZoom.stRatio.u32HRatio;
	}
//	std::cout << "pars ok6.33 " << std::endl;
	value["stVdec"]["stOsd"]["enType"] = info.stVdec.stOsd.enType;

	if (info.stVdec.stOsd.enType == DP_M2S_OSD_PIC) {
		value["stVdec"]["stOsd"]["au8PicPath"] =
				(DP_CHAR*) info.stVdec.stOsd.au8PicPath;
	} else if (info.stVdec.stOsd.enType == DP_M2S_OSD_STRING) {
//		std::cout << "pars ok6.34 " << std::endl;
		value["stVdec"]["stOsd"]["stStr"]["au8Str"] =
				(DP_CHAR*) info.stVdec.stOsd.stStr.au8Str;
		value["stVdec"]["stOsd"]["stStr"]["u32Color"] =
				info.stVdec.stOsd.stStr.u32Color;
	}
//	std::cout << "pars ok6.35 " << std::endl;
	value["stVdec"]["stOsd"]["enDispMode"] = info.stVdec.stOsd.enDispMode;
	value["stVdec"]["stOsd"]["stPoint"]["s32X"] =
			info.stVdec.stOsd.stPoint.s32X;
	value["stVdec"]["stOsd"]["stPoint"]["s32Y"] =
			info.stVdec.stOsd.stPoint.s32Y;
//	std::cout << "pars ok6.4 " << std::endl;
	value["stVdec"]["stSwms"]["enVoDevId"] = info.stVdec.stSwms.enVoDevId;
	value["stVdec"]["stSwms"]["u32SwmsChn"] = info.stVdec.stSwms.u32SwmsChn;
	value["stVdec"]["stSwms"]["u32Priority"] = info.stVdec.stSwms.u32Priority;
	value["stVdec"]["stSwms"]["stRect"]["s32X"] =
			info.stVdec.stSwms.stRect.s32X;
	value["stVdec"]["stSwms"]["stRect"]["s32Y"] =
			info.stVdec.stSwms.stRect.s32Y;
	value["stVdec"]["stSwms"]["stRect"]["u32Width"] =
			info.stVdec.stSwms.stRect.u32Width;
	value["stVdec"]["stSwms"]["stRect"]["u32Height"] =
			info.stVdec.stSwms.stRect.u32Height;
//	std::cout << "pars ok6.5 " << std::endl;
}

void TaskRestore::jsonValue2AVDec(Json::Value JsonValue,
		NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo) {
	DP_U32 taskCount = JsonValue[CODEC_AVDEC_TITLE].size();
	if (taskCount == 0) {
		return;
	} else {
		AVDecInfo->clear();
	}
	boost::shared_ptr<DP_M2S_AVDEC_INFO_S> tmp(new DP_M2S_AVDEC_INFO_S);
	while (taskCount--) {
		memset(tmp.get(), 0, sizeof(DP_M2S_AVDEC_INFO_S));
		tmp->s32TskId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["s32TskId"].asInt();
		tmp->AvBindAttr.enBindType =
				(DP_M2S_AVBIND_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["enBindType"].asUInt();
		tmp->AvBindAttr.stAudio.stIn.ModId =
				(DP_M2S_MOD_ID_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stAudio"]["stIn"]["ModId"].asUInt();
		tmp->AvBindAttr.stAudio.stIn.u32DevId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stAudio"]["stIn"]["u32DevId"].asUInt();
		tmp->AvBindAttr.stAudio.stIn.u32ChnId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stAudio"]["stIn"]["u32ChnId"].asUInt();

		tmp->AvBindAttr.stAudio.stIn.ModId =
				(DP_M2S_MOD_ID_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stAudio"]["stOut"]["ModId"].asUInt();
		tmp->AvBindAttr.stAudio.stIn.u32DevId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stAudio"]["stOut"]["u32DevId"].asUInt();
		tmp->AvBindAttr.stAudio.stIn.u32ChnId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stAudio"]["stOut"]["u32ChnId"].asUInt();
		tmp->AvBindAttr.stVideo.stIn.ModId =
				(DP_M2S_MOD_ID_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stVideo"]["stIn"]["ModId"].asUInt();
		tmp->AvBindAttr.stVideo.stIn.u32DevId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stVideo"]["stIn"]["u32DevId"].asUInt();
		tmp->AvBindAttr.stVideo.stIn.u32ChnId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stVideo"]["stIn"]["u32ChnId"].asUInt();
		tmp->AvBindAttr.stVideo.stIn.ModId =
				(DP_M2S_MOD_ID_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stVideo"]["stOut"]["ModId"].asUInt();
		tmp->AvBindAttr.stVideo.stIn.u32DevId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stVideo"]["stOut"]["u32DevId"].asUInt();
		tmp->AvBindAttr.stVideo.stIn.u32ChnId =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["AvBindAttr"]["stVideo"]["stOut"]["u32ChnId"].asUInt();
		tmp->stStream.enType =
				(DP_M2S_STREAM_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["enType"].asUInt();
		if (tmp->stStream.enType == DP_M2S_STREAM_RTSP_SERVER) {
			tmp->stStream._rtsp.stRtspServer.bOpen =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["bOpen"].asInt();

			tmp->stStream._rtsp.stRtspServer.bUDP =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["bUDP"].asInt();
			tmp->stStream._rtsp.stRtspServer.bMulticast =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["bMulticast"].asUInt();
			tmp->stStream._rtsp.stRtspServer.s32ConnTimeout =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["s32ConnTimeout"].asInt();
			tmp->stStream._rtsp.stRtspServer.s32ConnMax =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["s32ConnMax"].asInt();
			tmp->stStream._rtsp.stRtspServer.s32ConnNums =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["s32ConnNums"].asInt();
			memset(tmp->stStream._rtsp.stRtspServer.au8Url, 0, DP_M2S_URL_LEN);
			memcpy(tmp->stStream._rtsp.stRtspServer.au8Url,
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["au8Url"].asCString(),
					strlen(
							JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspServer"]["au8Url"].asCString()));

		} else if (tmp->stStream.enType == DP_M2S_STREAM_RTSP_CLIENT) {
			tmp->stStream._rtsp.stRtspClient.s8Open =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspClient"]["s8Open"].asUInt();
			tmp->stStream._rtsp.stRtspClient.bUDP =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspClient"]["bUDP"].asInt();
			tmp->stStream._rtsp.stRtspClient.bMulticast =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspClient"]["bMulticast"].asUInt();
			tmp->stStream._rtsp.stRtspClient.s32ConnTimeout =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspClient"]["s32ConnTimeout"].asInt();
			memset(tmp->stStream._rtsp.stRtspClient.au8Url, 0, DP_M2S_URL_LEN);
			memcpy(tmp->stStream._rtsp.stRtspClient.au8Url,
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspClient"]["au8Url"].asCString(),
					strlen(
							JsonValue[CODEC_AVDEC_TITLE][taskCount]["stStream"]["_rtsp"]["stRtspClient"]["au8Url"].asCString()));

		}

		tmp->stAdec.enAlg =
				(DP_M2S_ALG_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stAdec"]["enAlg"].asUInt();
		tmp->stVdec.bCrop =
				(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["bCrop"].asInt();
		tmp->stVdec.bZoom =
				(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["bZoom"].asInt();
		tmp->stVdec.bOsd =
				(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["bOsd"].asInt();
		tmp->stVdec.bSwms =
				(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["bSwms"].asInt();

		tmp->stVdec.stAlg.enAlg =
				(DP_M2S_ALG_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["enAlg"].asUInt();
		if (tmp->stVdec.stAlg.enAlg == DP_M2S_ALG_H264_ENC) {
			tmp->stVdec.stAlg.stH264Enc.u32FrmRate =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["u32FrmRate"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.stSize.u32Width =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["stSize"]["u32Width"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.stSize.u32Height =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["stSize"]["u32Height"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.enRcMode =
					(DP_M2S_RC_MODE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["enRcMode"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.u32Bitrate =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["u32Bitrate"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.enProfile =
					(DP_M2S_ALG_PROFILE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["enProfile"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.u32Gop =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["u32Gop"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.u16SF =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["u16SF"].asUInt();
			tmp->stVdec.stAlg.stH264Enc.u16TF =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Enc"]["u16TF"].asUInt();

		} else if (tmp->stVdec.stAlg.enAlg == DP_M2S_ALG_H264_DEC) {
			tmp->stVdec.stAlg.enAlg =
					(DP_M2S_ALG_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["enAlg"].asUInt();
			tmp->stVdec.stAlg.stH264Dec.enProfile =
					(DP_M2S_ALG_PROFILE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Dec"]["enProfile"].asUInt();
			tmp->stVdec.stAlg.stH264Dec.stSize.u32Width =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Dec"]["stSize"]["u32Width"].asUInt();
			tmp->stVdec.stAlg.stH264Dec.stSize.u32Height =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stH264Dec"]["stSize"]["u32Height"].asUInt();

		} else if (tmp->stVdec.stAlg.enAlg == DP_M2S_ALG_AAC_ENC) {
			tmp->stVdec.stAlg.stAACEnc.u32Bitrate =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stAACEnc"]["u32Bitrate"].asUInt();
			tmp->stVdec.stAlg.stAACEnc.bAdts =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stAACEnc"]["bAdts"].asInt();
		} else if (tmp->stVdec.stAlg.enAlg == DP_M2S_ALG_AAC_DEC) {
			tmp->stVdec.stAlg.stAACDec.bAdts =
					(DP_BOOL) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stAlg"]["stAACDec"]["bAdts"].asInt();

		}
		tmp->stVdec.stCrop.s32X =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stCrop"]["s32X"].asInt();
		tmp->stVdec.stCrop.s32Y =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stCrop"]["s32Y"].asInt();
		tmp->stVdec.stCrop.u32Width =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stCrop"]["u32Width"].asUInt();
		tmp->stVdec.stCrop.u32Height =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stCrop"]["u32Height"].asUInt();
		tmp->stVdec.stZoom.enType =
				(DP_M2S_ZOOM_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["enType"].asUInt();
		if (tmp->stVdec.stZoom.enType == DP_M2S_ZOOM_RECT) {
			tmp->stVdec.stZoom.stRect.s32X =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRect"]["s32X"].asInt();
			tmp->stVdec.stZoom.stRect.s32Y =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRect"]["s32Y"].asInt();
			tmp->stVdec.stZoom.stRect.u32Width =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRect"]["u32Width"].asUInt();
			tmp->stVdec.stZoom.stRect.u32Height =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRect"]["u32Height"].asUInt();

		} else if (tmp->stVdec.stZoom.enType == DP_M2S_ZOOM_RATIO) {
			tmp->stVdec.stZoom.stRatio.u32XRatio =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRatio"]["u32XRatio"].asUInt();
			tmp->stVdec.stZoom.stRatio.u32YRatio =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRatio"]["u32YRatio"].asUInt();
			tmp->stVdec.stZoom.stRatio.u32WRatio =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRatio"]["u32WRatio"].asUInt();
			tmp->stVdec.stZoom.stRatio.u32HRatio =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stZoom"]["stRatio"]["u32HRatio"].asUInt();
		}
		tmp->stVdec.stOsd.enType =
				(DP_M2S_OSD_TYPE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["enType"].asUInt();
		if (tmp->stVdec.stOsd.enType == DP_M2S_OSD_PIC) {
			memset(tmp->stVdec.stOsd.au8PicPath, 0, DP_M2S_OSD_PIC_PATH_LEN);
			memcpy(tmp->stVdec.stOsd.au8PicPath,
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["au8PicPath"].asCString(),
					strlen(
							JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["au8PicPath"].asCString()));
		} else if (tmp->stVdec.stOsd.enType == DP_M2S_OSD_STRING) {
			//		std::cout << "pars ok6.34 " << std::endl;
			memset(tmp->stVdec.stOsd.stStr.au8Str, 0, DP_M2S_OSD_STRING_LEN);
			memcpy(tmp->stVdec.stOsd.stStr.au8Str,
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["stStr"]["au8Str"].asCString(),
					strlen(
							JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["stStr"]["au8Str"].asCString()));
			tmp->stVdec.stOsd.stStr.u32Color =
					JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["stStr"]["u32Color"].asUInt();

		}
		tmp->stVdec.stOsd.enDispMode =
				(DP_M2S_OSD_DISPLAY_MODE_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["enDispMode"].asUInt();
		tmp->stVdec.stOsd.stPoint.s32X =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["stPoint"]["s32X"].asInt();
		tmp->stVdec.stOsd.stPoint.s32Y =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stOsd"]["stPoint"]["s32Y"].asInt();
		tmp->stVdec.stSwms.enVoDevId =
				(DP_M2S_VO_DEV_E) JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["enVoDevId"].asUInt();
		tmp->stVdec.stSwms.u32SwmsChn =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["u32SwmsChn"].asUInt();
		tmp->stVdec.stSwms.u32Priority =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["u32Priority"].asUInt();
		tmp->stVdec.stSwms.stRect.s32X =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["stRect"]["s32X"].asInt();
		tmp->stVdec.stSwms.stRect.s32Y =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["stRect"]["s32Y"].asInt();
		tmp->stVdec.stSwms.stRect.u32Width =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["stRect"]["u32Width"].asUInt();
		tmp->stVdec.stSwms.stRect.u32Height =
				JsonValue[CODEC_AVDEC_TITLE][taskCount]["stVdec"]["stSwms"]["stRect"]["u32Height"].asUInt();
		AVDecInfo->push_back(*tmp.get());
	}
}

void TaskRestore::thirdWindowData2JsonValue(OpenAndMoveWindow_S info,
		DP_U32 priority, DP_S32 codecID, Json::Value &value) {
	value["codecID"] = codecID;
//	value["priority"] = priority;
//	value["VOCHID"] = info.u8VoChnID;
	value["u32TaskID"] = info.u32TaskID;

	value["srcVideoInfo"]["u16VideoWidth"] = info.srcVideoInfo.u16VideoWidth;
	value["srcVideoInfo"]["u16VideoHeight"] = info.srcVideoInfo.u16VideoHeight;
	value["srcVideoInfo"]["u16StartX"] = info.srcVideoInfo.u16StartX;
	value["srcVideoInfo"]["u16StartY"] = info.srcVideoInfo.u16StartY;
	value["srcVideoInfo"]["u16EndX"] = info.srcVideoInfo.u16EndX;
	value["srcVideoInfo"]["u16EndY"] = info.srcVideoInfo.u16EndY;

//	value["dstVideoInfo"]["u16VideoWidth"] = info.dstVideoInfo.u16VideoWidth;
//	value["dstVideoInfo"]["u16VideoHeight"] = info.dstVideoInfo.u16VideoHeight;
//	value["dstVideoInfo"]["u16StartX"] = info.dstVideoInfo.u16StartX;
//	value["dstVideoInfo"]["u16StartY"] = info.dstVideoInfo.u16StartY;
}

void TaskRestore::thirdAudioData2JsonValue(DP_U32 thirdID, DP_S32 codecID,
		DP_M2S_AO_DEV_E AOChnID, Json::Value &value) {
	value["thirdID"] = thirdID;
	value["codecID"] = codecID;
	value["AOChnID"] = AOChnID;
}

//
//
///
//
//
//
