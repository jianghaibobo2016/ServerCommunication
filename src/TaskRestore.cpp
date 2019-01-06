/*
 * TaskRestore.cpp
 *
 *  Created on: Jan 2, 2019
 *      Author: jhb
 */

//#include <muduo/base/Exception.h>
//#include <boost/bind.hpp>
#include <muduo/base/Logging.h>
#include <map>
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
	muduo::MutexLockGuard lock(_AVDecmutex);
	IFStream.open(CODEC_AVDEC_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		std::cout << "no file  " << std::endl;
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
	if (parseOk) {
		while (notFound) {
			std::cout << "pars ok count " << count << std::endl;
			if (JsonValue["Codec AV Dec"][--count]["s32TskId"].asInt()
					== info.s32TskId) {
				std::cout << "info->s32TskId: " << info.s32TskId << std::endl;
				std::cout << "task id:  "
						<< JsonValue["Codec AV Dec"][count]["s32TskId"].asInt()
						<< std::endl;
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
	oIFStream.open(CODEC_AVDEC_JSON_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();
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

		Json::Reader oJsonReader;
		Json::StyledWriter oJsonSWriter;
		Json::Value oJsonRoot;
		std::ofstream oOFStream;
//		const char* strJson_2 =
//				"{\"DevList\": [{ \"DevSn\":11111 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端1\" , \"DevID\":\"3111000909\" , \"DevIP\":\"172.16.0.1\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 } , { \"DevSn\":1 , \"DevType\":4 , \"DevNum\":1 , \"DevName\":\"测试终端2\" , \"DevID\":\"3111000910\" , \"DevIP\":\"172.16.0.2\" , \"DevSoftVersion\":1 , \"DevHardVersion\":1 }]}";
//		std::cout << "parsssssssssssssssss " << strJson.str() << std::endl;

		const char* strJson_2 = "{\"Codec AV Dec\": []}";

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
			Json::Value newValue;
			DP_M2S_AVDEC_INFO_S info;
			avDec2JsonValue(info, newValue);
			oJsonRoot["Codec AV Dec"].append(newValue);
			std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);
			oOFStream.open(CODEC_AVDEC_JSON_PATH);
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
		std::cout << "finddev  3333333" << "size: " /*<< oJsonValue.size()*/
		<< " Devsn: "
				<< oJsonValue["Codec AV Dec"][0]["stStream"]["_rtsp"]["stRtspServer"]["s32ConnMax"].asInt()
				<< std::endl;

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
		oOFStream.open(CODEC_AVDEC_JSON_PATH);
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
void TaskRestore::setOpenAudioInfoToJson(_sRemote_OpenAudio *info) {

}

DP_BOOL TaskRestore::getAVDecJson(NodeInfo::VctrAVDECGetInfoPtr &AVDecInfo) {
	Json::Reader JsonReader;
	Json::Value JsonValue;
	std::ifstream IFStream;
//	muduo::MutexLockGuard lock(_AVDecmutex);
	IFStream.open(CODEC_AVDEC_JSON_PATH, std::ios::binary);
	if (!JsonReader.parse(IFStream, JsonValue, FALSE)) {
		IFStream.close();
		//could not parse json file to value
		LOG_ERROR << "Could not parse json file to value";
	} else {
		IFStream.close();

	}
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
		value["stVdec"]["stOsd"]["au8PicPath"] = info.stVdec.stOsd.au8PicPath;
	} else if (info.stVdec.stOsd.enType == DP_M2S_OSD_STRING) {
//		std::cout << "pars ok6.34 " << std::endl;
		value["stVdec"]["stOsd"]["stStr"]["au8Str"] =
				info.stVdec.stOsd.stStr.au8Str;
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

//
///
//
//
//
