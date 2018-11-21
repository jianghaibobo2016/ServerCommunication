/*
 * Public.cpp
 *
 *  Created on: 2017年5月6日
 *      Author: dsppa
 */
#include "Public.h"

//namespace DSPPAUtil{

void PrintCurTime() {
	const char* wday[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep); //gmtime
	char cText[1024] = { 0 };
	sprintf(cText, "%d%d%d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	cout << cText << endl;
	sprintf(cText, "%s %d:%d:%d", wday[p->tm_wday], p->tm_hour, p->tm_min,
			p->tm_sec);
	cout << cText << endl;
}

tm GetCurTime() {
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep); //gmtime
	return *p;
}

// 返回自系统开机以来的毫秒数（tick）
unsigned long GetTickCount() {
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void int2str(const int &int_temp, string &string_temp) {
	stringstream stream;
	stream << int_temp;
	stream >> string_temp;   //此处也可以用 stream>>string_temp
}

string int2str(const int &int_temp) {
	string string_temp;
	stringstream stream;
	stream << int_temp;
	stream >> string_temp;
	return string_temp;
}

void str2int(int &int_temp, const string &string_temp) {
	stringstream stream(string_temp);
	stream >> int_temp;
}

int str2int(const string &string_temp) {
	int int_temp;
	stringstream stream(string_temp);
	stream >> int_temp;
	return int_temp;
}

bool is_str_utf8(const char* str) {
	unsigned int nBytes = 0;   //UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr = *str;
	bool bAllAscii = true;

	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}
		if (nBytes == 0) {
			//如果不是ASCII码,应该是多字节符,计算字节数
			if (chr >= 0x80) {
				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				} else if (chr >= 0xF8) {
					nBytes = 5;
				} else if (chr >= 0xF0) {
					nBytes = 4;
				} else if (chr >= 0xE0) {
					nBytes = 3;
				} else if (chr >= 0xC0) {
					nBytes = 2;
				} else {
					return false;
				}
				nBytes--;
			}
		} else {
			//多字节符的非首字节,应为 10xxxxxx
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//减到为零为止
			nBytes--;
		}
	}
	//违返UTF8编码规则
	if (nBytes != 0) {
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
		return true;
	}
	return true;
}

//} /* namespace DSPPAUtil */
