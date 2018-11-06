/*
 * LocalInfo.h
 *
 *  Created on: Oct 17, 2018
 *      Author: jhb
 */

#ifndef SRC_LOCALINFO_H_
#define SRC_LOCALINFO_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <muduo/base/Mutex.h>

#include "interactivepro.h"

//singleton class -->template
class LocalInfo: boost::noncopyable {
public:
	LocalInfo();
	~LocalInfo();

	typedef boost::shared_ptr<_sRemote_Reply_Search> RRSPtr;
public:
	RRSPtr getLocalInfo() const;
	void updateLocalInfo(RRSPtr newData);

private:
	RRSPtr _sLocalSearchInfo;
	mutable muduo::MutexLock _mutex;
	void initLocalInfo();

};

#endif /* SRC_LOCALINFO_H_ */
