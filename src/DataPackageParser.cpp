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

_E_RemoteCMD DataPackageParser::parserDataPackage(std::string data) {
	boost::shared_ptr<_S_DataHeadInfo> dataHeadInfo(new _S_DataHeadInfo);
	if (!parserDataHead(dataHeadInfo.get(), data)) {
		LOG_TRACE << "Unvalid package .";
		return _eButt;
	}
	switch (dataHeadInfo->u8CommandID) {
	case Command_Search:
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
	}

	return _eButt;
}

bool DataPackageParser::parserDataHead(_S_DataHeadInfo *dataHeadInfo,
		std::string data) {
	_s_AVServer_Header *headr = (_s_AVServer_Header*) data.c_str();
//	muduo::PrintBuff::printBufferByHex("fff", data.c_str(), 10);
	if (headr->u16Headtag != 0xF0F1)
		return false;
	dataHeadInfo->u8CommandID = headr->stFunctionMsg.u8CommandID;
	dataHeadInfo->u8PackageType = headr->u8PackageType;
	return true;
}
