/*
 * Buffer.h
 *
 *  Created on: 2017年7月23日
 *      Author: zxb
 */

#ifndef UTIL_BUFFER_H_
#define UTIL_BUFFER_H_
#include "Mutex.h"
#include "Handle.h"
#include "Shared.h"
#include "Console.h"
#include <string.h>
#include <vector>
using namespace std;
class RecvBufferTool {
public:
	RecvBufferTool() {
		_readpos = 0;
	}
	~RecvBufferTool() {

	}
	void setByte(void* pData, int len){
		assert(pData);
		assert(len);
		_buffer.resize(len);
		memcpy(&_buffer[0],pData,len);
	}
	uint8_t getByte() throw (SystemException) {
		if (_readpos < (int)_buffer.size()) {
			return _buffer[_readpos++];
		} else
			throw SystemException("RecvBufferTool getByte none buffer");
	}
	void getByte(uint8_t *pData, int len) throw (SystemException) {
		if (_readpos + len < (int)_buffer.size()) {
			memcpy(pData, &_buffer[_readpos], len);
			_readpos += len;
		} else {
			throw SystemException("RecvBufferTool getByte array none buffer");
		}
	}
	uint16_t getShort() throw (SystemException) {
		uint16_t value;
		getByte((uint8_t*) &value, sizeof(uint16_t));
		return value;
	}
	uint32_t getInt() throw (SystemException) {
		uint32_t value;
		getByte((uint8_t*) &value, sizeof(uint32_t));
		return value;
	}
	void getString(string& value, int len) throw (SystemException){
		assert(len>0);
		vector<uint8_t> temp;
		temp.resize(len+1);
		getByte(&temp[0],len);
		temp[len] = 0;
		value = (const char*)&temp.front();
	}
private:
	int _readpos;
	vector<uint8_t> _buffer;
};
class SendBufferTool {
public:
	SendBufferTool() {

	}
	~SendBufferTool() {

	}
	void updateByte(int pos ,uint8_t value)throw(SystemException){
		if (pos<(int)_buffer.size()){
			_buffer[pos] = value;
		}else{
			throw SystemException("sendbuffertool update byte pos invalid");
		}
	}
	void updateByte(int pos, const void* pData, int len){
		for (int i=0; i<len; i++){
			updateByte(pos+i,*((uint8_t*)pData + i));
		}
	}
	void updateShort(int pos, uint16_t value){
		updateByte(pos, &value, sizeof(value) );
	}
	void updateInt(int pos, uint32_t value){
		updateByte(pos, &value, sizeof(value) );
	}

	void putByte(uint8_t value) {
		_buffer.push_back(value);
	}
	void putByte(const uint8_t *pData, int len) {
		for (int i = 0; i < len; i++)
			putByte(pData[i]);
	}

	void putShort(uint16_t value) {
		putByte((const uint8_t*) &value, sizeof(uint16_t));
	}
	void putInt(uint32_t value) {
		putByte((const uint8_t*) &value, sizeof(uint32_t));
	}
	void putString(const string& strvalue, bool containEndChar = false) {
		putByte((const uint8_t*) strvalue.c_str(), strvalue.length());
	}
	void putString(const char* strvalue, bool containEndChar = false) {
		assert(strvalue);
		putByte((const uint8_t*) strvalue, strlen(strvalue));
	}
	int getLength() {
		return (int) _buffer.size();
	}
	void* getData(){
		assert(_buffer.size()>0);
		return &_buffer.front();
	}
	int getData(void* pData, int len) {
		if (len < (int) _buffer.size()) {
			memcpy((void*) pData, (void*) &_buffer.front(), len);
			return len;
		} else {
			memcpy((void*) pData, (void*) &_buffer.front(), _buffer.size());
			return _buffer.size();
		}
	}
private:
	vector<uint8_t> _buffer;
};
class Buffer: public Shared {
public:
	Buffer(int initSize = 1024 * 10) {
		assert(initSize > 0);
		_buffer.resize(initSize, 0);
		_wsize = 0;
	}
	virtual ~Buffer() {
		_buffer.clear();
		_wsize = 0;
	}
	void push_back(const void* pData, int len) {
		assert(len > 0);
		AutoLock lock(&_mutex);
		if (_wsize + len > _buffer.size()) {
			//缓冲区不够用
			Console::instance()->warning("缓冲区不够用,当前%d, 新的％d", _buffer.size(),
					_wsize + len);
			vector<uint8_t> vTemp = _buffer;
			_buffer.resize(_wsize + len, 0);
			memcpy(&_buffer.front(), &vTemp.front(), vTemp.size());
		}
		memcpy(&_buffer[_wsize], pData, len);
		_wsize += len;
	}
	int pop_begin(void* pData, int len) {
		assert(len > 0);
		AutoLock lock(&_mutex);
		if (_wsize == 0)
			return 0;
		int ret = 0;
		if ((size_t) len > _wsize) {
			memcpy(pData, &_buffer.front(), _wsize);
			ret = _wsize;
			_wsize = 0;
			return ret;
		} else { //缓冲去数据较多
			memcpy(pData, &_buffer.front(), len);
			_wsize -= len;
			memcpy(&_buffer[0], &_buffer[len], _wsize);
			ret = len;
			return ret;
		}
	}
	void clear() {
		AutoLock lock(&_mutex);
		_wsize = 0;
	}
	int length() {
		return _wsize;
	}
private:
	Mutex _mutex;
	vector<uint8_t> _buffer;
	size_t _wsize;
};
typedef Handle<Buffer> BufferPtr;
#endif /* UTIL_BUFFER_H_ */
