/*
 * Public.h
 *
 *  Created on: 2017年5月6日
 *      Author: dsppa
 */

#ifndef OTHER_PUBLIC_H_
#define OTHER_PUBLIC_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <cmath>
#include "time.h"

using namespace std;

//namespace DSPPAUtil{

void PrintCurTime();
tm GetCurTime();
unsigned long GetTickCount(); // 返回自系统开机以来的毫秒数（tick）

void int2str(const int &int_temp, string &string_temp);
string int2str(const int &int_temp);
template<class T>
void toString(string & result, const T& t) {
	ostringstream oss; //创建一个流
	oss << t; //把值传递如流中
	result = oss.str(); //获取转换后的字符转并将其写入result
}
template<class T>
string toString(const T& t) {
	string string_temp;
	ostringstream oss; //创建一个流
	oss << t; //把值传递如流中
	string_temp = oss.str(); //获取转换后的字符转并将其写入result
	return string_temp;
}
void str2int(int &int_temp, const string &string_temp);
int str2int(const string &string_temp);
template<class out_type, class in_value>
out_type convert(const in_value & t) {
	stringstream stream;
	stream << t; //向流中传值
	out_type result; //这里存储转换结果
	stream >> result; //向result中写入值
	return result;
}

bool is_str_utf8(const char* str);

//} /* namespace DSPPAUtil */

#endif /* OTHER_PUBLIC_H_ */
