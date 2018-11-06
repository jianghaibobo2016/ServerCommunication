/*
 * StringStorage.cpp
 *
 *  Created on: 2018年7月31日
 *      Author: zxb
 */

#include "StringStorage.h"
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "StringArray.h"
namespace DSPPAUtil {

StringStorage::StringStorage() {
	// TODO Auto-generated constructor stub
	setString("");
}

StringStorage::StringStorage(const char* pStrcopy) {
	setString(pStrcopy);
}

StringStorage::StringStorage(const StringStorage& strtemp) {
	setString(strtemp);
}
StringStorage::StringStorage(const string& strtemp) {
	setString(strtemp.c_str());
}
StringStorage::~StringStorage() {
	// TODO Auto-generated destructor stub
	clear();
}

void StringStorage::setString(const char* pStrCopy) {
	if (pStrCopy == 0)
		return;
	//_strSource = pStrCopy;
	size_t len = strlen(pStrCopy);
	_strSource.resize(len+1);
	memcpy(&_strSource.front(),pStrCopy,len);
	_strSource[len] = 0;
}
void StringStorage::setString(const StringStorage& strcopy) {
	setString(strcopy.c_str());
}
void StringStorage::format(const char* fmt, ...) {
	va_list vl;

	va_start(vl, fmt);
	int count = vsnprintf(0, 0, fmt, vl);
	va_end(vl);

	_strSource.resize(count + 1);

	va_start(vl, fmt);
	vsnprintf(&_strSource[0], count + 1, fmt, vl);
	va_end(vl);

	//_strSource[count] = '\0';
}
void StringStorage::appendformat(const char* fmt, ...) {
	vector<char> vTemp;
	va_list vl;

	va_start(vl, fmt);
	int count = vsnprintf(0, 0, fmt, vl);
	va_end(vl);

	vTemp.resize(count + 1);

	va_start(vl, fmt);
	vsnprintf(&vTemp[0], count + 1, fmt, vl);
	va_end(vl);
	//vTemp[count] = '\0';
	_strSource.insert(_strSource.begin()+length(),vTemp.begin(),vTemp.begin()+count);
}
const size_t StringStorage::length()const {
	return _strSource.size()-1;
}
bool StringStorage::getSubstring(StringStorage &substr,  size_t startIndex,  size_t count) const{
	if (startIndex >= length())
		return false;
	if (startIndex + count > length())
		return false;

	std::vector<char> autoBuffer(count + 1);
	char *buffer = &autoBuffer.front();
	memcpy(buffer, &_strSource[startIndex], count * sizeof(char));
	buffer[count] = '\0';

	substr = buffer;
	return true;
}
void StringStorage::split(const char *delimiters, StringArray& strArray) const {
	if (delimiters == 0)
		return;
	strArray.clear();
	int pos = 0;
	StringStorage strtemp = *this;
	int delimiterlen = strlen(delimiters);
	while (1) {
		pos = strtemp.find(delimiters);
		if (pos >= 0) {
			strArray.addString(strtemp.left(pos));
			strtemp = strtemp.right(strtemp.length() - pos - delimiterlen);
		} else { //find end
			if (strtemp.length() > 0)
				strArray.addString(strtemp);
			break;
		}
	} //end while
}
const char* StringStorage::c_str()const{
	return &_strSource.front();
}
StringStorage StringStorage::left(size_t count){
	size_t nLength = length();
	if (count >= nLength) {
		return (*this);
	}
	StringStorage strtemp;
	getSubstring(strtemp,0,count);
	return strtemp;
}
StringStorage StringStorage::right(size_t count) {
	size_t nLength = length();
	if (count >= nLength) {
		return (*this);
	}
	StringStorage strtemp;
	getSubstring(strtemp, nLength - count, count);
	return strtemp;
}
StringStorage StringStorage::mid(size_t startIndex, size_t count) {
	if (startIndex>=length())
		return "";
	if ( (startIndex+count) > length())
		return "";
	StringStorage strtemp;
	getSubstring(strtemp, startIndex, count);
	return strtemp;
}
StringStorage& StringStorage::trim(){
	return trimleft().trimright();
}
StringStorage& StringStorage::trimleft(){
	int nSpaceCount = 0;
	size_t nLen = length();
	size_t npos = 0;
	while(npos<nLen && _strSource[npos] == ' '){
		nSpaceCount++;
		npos++;
	}
	if (nSpaceCount>0){
		remove(0,nSpaceCount);
		return *this;
	}else
		return *this;
}
StringStorage& StringStorage::trimright() {
	int nSpaceCount = 0;
	size_t nLen = length();
	int npos = nLen - 1;
	while (npos > 0 && _strSource[npos] == ' ') {
		nSpaceCount++;
		npos--;
	}
	if (nSpaceCount > 0) {
		remove(nLen-nSpaceCount, nSpaceCount);
		return *this;
	} else
		return *this;
}
bool StringStorage::isEmpty(){
	return length()==0;
}
StringStorage& StringStorage::makeUpper() {
	for (size_t i = 0; i < (_strSource.size() - 1); i++) {
		if (isalpha(_strSource[i]))
			_strSource[i] = toupper(_strSource[i]);
	}
	return *this;
}
StringStorage& StringStorage::makeLower() {
	for (size_t i = 0; i < (_strSource.size() - 1); i++) {
		if (isalpha(_strSource[i]))
			_strSource[i] = tolower(_strSource[i]);
	}
	return *this;
}
int StringStorage::find(const char* str) {
	char* pFindAddress = strstr(&_strSource.front(), str);
	if (pFindAddress == 0)
		return -1;
	else
		return pFindAddress - &_strSource.front();
}

//Linux 没有反向查找函数
char* strrstr(char *haystack, const char *needle)
{
	if (!haystack || !needle)
		return NULL;
	int srclen = strlen(haystack);
	int needlen = strlen(needle);
	if (needlen <= 0)
		return NULL;
	int nStartPos = srclen - needlen;
	if (nStartPos < 0)
		return NULL;
	char* cResult = haystack + nStartPos;
	for (; cResult >= haystack; cResult--) {
		if ((cResult[0] == needle[0]) && (memcmp(cResult, needle, needlen) == 0))
			return cResult;
	}
	return NULL;
}

int StringStorage::reversefind(const char* str) {
	char* pFindAddress = strrstr(&_strSource.front(), str);
	if (pFindAddress == 0)
		return -1;
	else
		return pFindAddress - &_strSource.front();
}
int StringStorage::findchar(const char ch) {
	char* pFindAddress = strchr(&_strSource.front(), ch);
	if (pFindAddress == 0)
		return -1;
	else
		return pFindAddress - &_strSource.front();
}
void StringStorage::replace(const char* oldsubstring, const char* newsubstring){

	if (!oldsubstring || !newsubstring)
		return;
	if (strcmp(oldsubstring,newsubstring)==0)
		return;
	if (strlen(oldsubstring)==0 || strlen(newsubstring)==0)
		return;
	int npos = 0;
	int checkpos =0;
	while(true){
		char* pFindAddress = strstr(&_strSource[checkpos], oldsubstring);
		if (pFindAddress == NULL)
			return;
		npos = pFindAddress - &_strSource.front();
		//erase old
		_strSource.erase(_strSource.begin() + npos, _strSource.begin() + npos + strlen(oldsubstring));
		//insert new
		_strSource.insert(_strSource.begin() + npos, newsubstring, newsubstring + strlen(newsubstring));
		//update check pos
		checkpos = npos + strlen(newsubstring);
	}
}
void StringStorage::remove(int startIndex, size_t count){
	 if ((startIndex + count) > length())
		 return;
	  vector<char> newBuffer = _strSource;
	  size_t copyCount = _strSource.size() - (startIndex + 1) * sizeof(char);//
	  memcpy(&newBuffer[startIndex], &newBuffer[startIndex + count], copyCount);
	  setString(&newBuffer.front());
}
void StringStorage::clear(){
	setString("");
}
StringStorage& StringStorage::operator =(const StringStorage &other){
	if(this == &other){
		return *this;
	}
	setString(other);
	return *this;
}

StringStorage& StringStorage::operator =(const string &other){
	setString(other);
	return *this;
}
StringStorage& StringStorage::operator =(const char* str) {
	setString(str);
	return *this;
}
bool StringStorage::operator ==(const StringStorage &str) const{
	if (strcmp(&_strSource.front(),str.c_str()) == 0)
		return true;
	else
		return false;
}
bool StringStorage::operator ==(const char* str) const {
	if (strcmp(&_strSource.front(), str) == 0)
		return true;
	else
		return false;
}
/**比较字符串*/
bool StringStorage::operator <(const StringStorage &str) const {
	if (strcmp(&_strSource.front(), str.c_str()) < 0)
		return true;
	else
		return false;
}

/**比较字符串*/
bool StringStorage::operator <(const char* str) const {
	if (strcmp(&_strSource.front(), str) < 0)
		return true;
	else
		return false;
}

/**比较字符串*/
bool StringStorage::operator >(const StringStorage &str) const{
	if (strcmp(&_strSource.front(), str.c_str()) > 0)
		return true;
	else
		return false;
}

/**比较字符串*/
bool StringStorage::operator >(const char* str) const {
	if (strcmp(&_strSource.front(), str) > 0)
		return true;
	else
		return false;
}

/**拼接字符串*/
void StringStorage::operator +=(const char* str) {
	if (str == 0) {
		return;
	}
	vector<char>::iterator to = _strSource.begin() + length();
	_strSource.insert(to, str, str + strlen(str));
}

/**拼接字符串*/
void StringStorage::operator +=(const StringStorage &strtemp) {
	const char *str = strtemp.c_str();
	vector<char>::iterator to = _strSource.begin() + length();
	_strSource.insert(to, str, str + strlen(str));
}

void StringStorage::operator +=(const string &strtemp) {
	const char *str = strtemp.c_str();
	vector<char>::iterator to = _strSource.begin() + length();
	_strSource.insert(to, str, str + strlen(str));
}

/**拼接字符串*/
StringStorage& StringStorage::operator +(const char* str) {
	if (str == 0) {
		return *this;
	}
	vector<char>::iterator to = _strSource.begin() + length();
	_strSource.insert(to, str, str + strlen(str));
	return *this;
}

/**拼接字符串*/
StringStorage& StringStorage::operator +(const StringStorage &str){
	return *this + str.c_str();
}

StringStorage& StringStorage::operator +(const string &str){
	return *this + str.c_str();
}

} /* namespace DSPPAUtil */
