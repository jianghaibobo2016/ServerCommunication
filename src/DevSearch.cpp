/*
 * DevSearch.cpp
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#include <muduo/base/Singleton.h>
#include <stdio.h>

#include "LocalInfo.h"
#include "DevSearch.h"

void DevSearch::devSearch(const muduo::net::TcpConnectionPtr connPtr) {
	LocalInfo::RRSPtr localInfo =
			muduo::Singleton<LocalInfo>::instance().getLocalInfo();
	connPtr->send(localInfo.get(), sizeof(_sRemote_Reply_Search));
}

