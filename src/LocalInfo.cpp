/*
 * LocalInfo.cpp
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#include <string.h>
#include <stdio.h>
#include "LocalInfo.h"
#include "DevStaticConfigure.h"
#include "SetNetwork.h"
#include "GlobalProfile.h"

LocalInfo::LocalInfo() :
		_sLocalSearchInfo(new _sRemote_Reply_Search()) {
	initLocalInfo();
}

LocalInfo::~LocalInfo() {
}

LocalInfo::RRSPtr LocalInfo::getLocalInfo() const {
	muduo::MutexLockGuard lock(_mutex);
	return _sLocalSearchInfo;
}

void LocalInfo::updateLocalInfo(RRSPtr newData) {
	if (newData) {
		muduo::MutexLockGuard lock(_mutex);
		_sLocalSearchInfo.swap(newData);
	}
}

void LocalInfo::initLocalInfo() {
	DP_DEV_BASE_INFO_S stBaseInfoTemp;
	SetNetwork netInfo;
	netInfo.setIfname(g_IFNAMEDEV);
	netInfo.getNetworkConfig();
	DP_U8 mac[13] = { 0 };
	netInfo.mac2U8(mac);

	GlobalProfile::getInstance()->GetBaseInfo(stBaseInfoTemp);

	muduo::MutexLockGuard lock(_mutex);
	_sRemote_Header head(netInfo.ip2U32(), (eDeviceType)stBaseInfoTemp.u16Devtype, 0x01,
			sizeof(_sRemote_Reply_Search)
					+ sizeof(_sRemote_Reply_Search_ExtendDeviceOutput),
					//jhbnote
			Command_Search,
			sizeof(_sRemote_Reply_Search) - sizeof(_sRemote_Header)
					+ sizeof(_sRemote_Reply_Search_ExtendDeviceInput));
	_sLocalSearchInfo->header = head;
	memcpy(_sLocalSearchInfo->au8DevID, mac, 13);
	_sLocalSearchInfo->u8DevType = g_DevType;
	_sLocalSearchInfo->u8DevNum = stBaseInfoTemp.u16DevNum;
	memcpy(_sLocalSearchInfo->au8DevName, stBaseInfoTemp.au8DevName, DP_DEV_NAME_LEN);
	_sLocalSearchInfo->u32DevIP = netInfo.ip2U32();
	_sLocalSearchInfo->u16DevSoftVersion = stBaseInfoTemp.u16DevSoftVersion;
	_sLocalSearchInfo->u16DevHardVersion = stBaseInfoTemp.u16DevHardVersion;
	_sLocalSearchInfo->u16ExtendDataLen =
			sizeof(_sRemote_Reply_Search_ExtendDeviceOutput);

}
