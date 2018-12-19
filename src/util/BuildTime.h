/*
 * BuildTime.h
 *
 *  Created on: 2017年8月22日
 *      Author: root
 */

#ifndef UTIL_BUILDTIME_H_
#define UTIL_BUILDTIME_H_
#include <string>
//#include <iostream>
//using namespace std;
//class BuildTime {
//public:
//	static const string BUILDTIME;
//	static const string BUILDVERSION;
//private:
//	BuildTime(){
//
//	}
//
//};
const char BuildTime[] = __DATE__ " at " __TIME__;

const std::string BUILDVERSION = "Hardware:VED-V00.01 SoftWare:V00.03";

#endif /* UTIL_BUILDTIME_H_ */
