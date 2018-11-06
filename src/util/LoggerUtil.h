/*
 * Logger.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_LOGGER_H_
#define UTIL_LOGGER_H_
#include <string>
//using namespace std;
#include <stdio.h>
#include <stdarg.h>
#include "Shared.h"
class LoggerUtil :  virtual public Shared {
public:
	LoggerUtil(){

	}
	virtual ~LoggerUtil(){

	}
	void debug(const char* fmt, ...) {
		char strMsg[4096];
		va_list vl;
		va_start(vl, fmt);
		vsnprintf(strMsg, 4096, fmt, vl);
		va_end(vl);
		onLog(DEBUG, strMsg);
	}
	void printbytes(void* pData, int len){
		std::string strdata;
		char strnum[10];
		for(int i=0; i<len; i++ ){
			sprintf(strnum,"%02x ",*((unsigned char*)pData + i));
			strdata += strnum;
		}
		debug(strdata.c_str());
	}
	void warning(const char* fmt, ...) {
		char strMsg[2048];
		va_list vl;
		va_start(vl, fmt);
		vsnprintf(strMsg, 2048, fmt, vl);
		va_end(vl);
		onLog(WARNING, strMsg);
	}
	void error(const char* fmt, ...) {
		char strMsg[2048];
		va_list vl;
		va_start(vl, fmt);
		vsnprintf(strMsg, 2048, fmt, vl);
		va_end(vl);
		onLog(ERROR, strMsg);
	}
protected:
	enum LOGLEVEL{
			DEBUG = 0,
			WARNING = 1,
			ERROR = 2
		};
private:

	virtual void onLog(int level, const std::string& log ) = 0;
};

#endif /* UTIL_LOGGER_H_ */
