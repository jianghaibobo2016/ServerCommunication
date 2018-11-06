/*
 * DevSearch.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_DEVSEARCH_H_
#define SRC_DEVSEARCH_H_

#include <muduo/net/TcpConnection.h>
#include "LocalInfo.h"

class DevSearch {
public:
	DevSearch() {
	}
	~DevSearch() {
	}

	 void devSearch(const muduo::net::TcpConnectionPtr connPtr);
private:

};

#endif /* SRC_DEVSEARCH_H_ */
