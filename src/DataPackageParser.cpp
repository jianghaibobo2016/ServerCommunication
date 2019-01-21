/*
 * DataPackageParser.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: jhb
 */

#include <boost/shared_ptr.hpp>
#include <iostream>
#include "DataPackageParser.h"
#include "interactivepro.h"
#include "PrintBuff.h"

DataPackageParser::DataPackageParser() {

}

DataPackageParser::~DataPackageParser() {
}

_E_RemoteCMD DataPackageParser::parserDataPackage(DP_U32 &count,
		std::string data) {
	boost::shared_ptr<_S_DataHeadInfo> dataHeadInfo(new _S_DataHeadInfo);
	if (!parserDataHead(dataHeadInfo.get(), data)) {
		LOG_TRACE << "Unvalid package .";
		return _eButt;
	}
	count = dataHeadInfo->u32PackLen;
	switch (dataHeadInfo->u8CommandID) {
	case Command_MulticastSearch:
		return _eSearch;
	case Command_CreateWindow:
		return _eCreateWindow;
	case Command_MoveWindow:
		return _eMoveWindow;
	case Command_GetInfo:
		return _eGetInfo;
	case Command_SetInfo:
		return _eSetInfo;
	case Command_CloseWindow:
		return _eCloseWindow;
	case Command_OpenAudio:
		return _eOpenAudio;
	case Command_CloseAudio:
		return _eCloseAudio;
	case Command_SetAudio:
		return _eSetAudio;
	case Command_ClearTask:
		return _eClearTask;
	case Command_GetJsonInfo:
		return _eGetJsonInfo;
	case Command_SetJsonInfo:
		return _eSetJsonInfo;
	case Command_UnicastSearch:
		return _eUnicastSearch;
	case Command_OpenAndMoveWindow:
		return _eOpenAndMoveWindow;
	case Command_WindowBatchControl:
		return _eWindowBatchControl;
	case Command_UpdateBatch:
		return _eUpdateBatch;
	}
	return _eButt;
}

bool DataPackageParser::parserDataHead(_S_DataHeadInfo *dataHeadInfo,
		std::string data) {
	_s_AVServer_Header *headr = (_s_AVServer_Header*) data.c_str();
	//jhbnote check len
//	muduo::PrintBuff::printBufferByHex("fff", data.c_str(), 10);
	if (headr->u16Headtag != 0xF0F1)
		return false;
	dataHeadInfo->u8CommandID = headr->stFunctionMsg.u8CommandID;
	dataHeadInfo->u8PackageType = headr->u8PackageType;
	dataHeadInfo->u32PackLen = headr->u16PackageLen;
	return true;
}
