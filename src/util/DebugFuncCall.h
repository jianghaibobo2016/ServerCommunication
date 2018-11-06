/*
 * DebugFuncCall.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_DEBUGFUNCCALL_H_
#define UTIL_DEBUGFUNCCALL_H_
#include <iostream>
using namespace std;

#define DEBUGFUNCCALL __FILE__,__FUNCTION__
class DebugFuncCall {
public:
	static void DebugConstruct(const char* filename, const char* func){
		cout<<"构造－"<<func<<" file:"<<filename<<endl;
	}
	static void DebugDestruct(const char* filename, const char* func){
		cout<<"析构－"<<func<<" file:"<<filename<<endl;
	}
	static void DebugFunc(const char* filename, const char* func){
		cout<<"调用－"<<func<<" file:"<<filename<<endl;
	}

};

#endif /* UTIL_DEBUGFUNCCALL_H_ */
