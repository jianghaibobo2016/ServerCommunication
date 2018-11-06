/*
 * StringArray.cpp
 *
 *  Created on: 2018年7月31日
 *      Author: zxb
 */

#include "StringArray.h"
#include "StringStorage.h"
namespace DSPPAUtil {

StringArray::StringArray() {

}

StringArray::~StringArray() {

	clear();
}

void StringArray::addString(const char* strtemp){
	if (strtemp)
		_array.push_back(strtemp);
}

void StringArray::addString(const StringStorage& strtemp){
	_array.push_back(strtemp);
}

void StringArray::updateItem(size_t nIndex, const char* strtemp){
	if (nIndex>=0 && nIndex<_array.size()){
		_array[nIndex] = strtemp;
	}
}

void StringArray::updateItem(size_t nIndex, const StringStorage& strtemp){
	if (nIndex>=0 && nIndex<_array.size()){
			_array[nIndex] = strtemp;
		}
}

void StringArray::deleteItem(size_t nIndex){
	if (nIndex>=0 && nIndex<_array.size()){
			_array.erase(_array.begin()+nIndex);
		}
}

void StringArray::clear(){
	_array.clear();
}

size_t StringArray::getCount(){
	return _array.size();
}

const char* StringArray::getAt(size_t nIndex){
	if (nIndex>=0 && nIndex<_array.size()){
		return _array[nIndex].c_str();
	}else
		return 0;
}

bool StringArray::getAt(size_t nIndex, StringStorage& strItem){
	if (nIndex>=0 && nIndex<_array.size()){
		    strItem = _array[nIndex];
			return true;;
		}else
			return false;
}

} /* namespace DSPPAUtil */
