/*
 * SystemException.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_SYSTEMEXCEPTION_H_
#define UTIL_SYSTEMEXCEPTION_H_
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>
//using namespace std;
class SystemException : public std::exception{
public:
	SystemException(){
		m_errormsg = strerror(errno);
		char temp[200];
		sprintf(temp," errorcode:%d",errno);
		m_errormsg += temp;

	}
	SystemException(const char* file, const char* func, int line){
		char head[2048];
		snprintf(head,2048,"%s %s-%d ",file,func,line);
		m_errormsg += head;
		m_errormsg += strerror(errno);

	}
	SystemException(const std::string& selfreason){
			m_errormsg = selfreason;

		}
	virtual ~SystemException()throw (){

	}
	SystemException(const void* pData, int len){
		m_errormsg = "Error data:";
		if (len <= 0)
			return;
		char strHex[10];
		char* pHex = (char*)pData;
		for(int i=0; i<len; i++){
			sprintf(strHex,"%02x,",pHex[i]);
			m_errormsg += strHex;
		}
	}

	virtual const char* what() {
		return m_errormsg.c_str();
	}
private:
	std::string m_errormsg;
};

#endif /* UTIL_SYSTEMEXCEPTION_H_ */
