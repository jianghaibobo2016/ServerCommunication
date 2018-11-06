/*
 * Console.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */
#if 0
#ifndef UTIL_CONSOLE_H_
#define UTIL_CONSOLE_H_
#include <LoggerUtil.h>
#include <stdio.h>
#include <stdarg.h>
#include "Handle.h"
#include "DebugFuncCall.h"
#include "DateTime.h"
#include "Handle.h"
#include <iostream>
using namespace std;
//using namespace DSPPAUtil;
#define CLR_RED   "\033[31m"
#define CLR_GREEN "\033[32m"
#define CLR_BLUE "\033[34m"
#define CLR_RESET "\033[0m"
class Console: public LoggerUtil {
public:
	Console() {
		DebugFuncCall::DebugConstruct(DEBUGFUNCCALL);
	}
	~Console() {
		DebugFuncCall::DebugDestruct(DEBUGFUNCCALL);
	}
	static Handle<Console> g_Console;
	static Handle<Console> instance() {
		if (!g_Console) {
			g_Console = new Console();
		}
		return g_Console;
	}
private:

	virtual void onLog(int level, const std::string& log) {
		if (level == LoggerUtil::WARNING)
			cout << CLR_BLUE << "WARNING ";
		else if (level == LoggerUtil::ERROR)
			cout << CLR_RED << "ERROR ";
		else
			;
//		cout<<DateTime::now().toDateTimeString()<<" >>"<<log<<CLR_RESET<<endl;
	}
};
typedef Handle<Console> ConsolePtr;

#endif /* UTIL_CONSOLE_H_ */
#endif
