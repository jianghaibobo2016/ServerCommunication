/*
 * SpliterString.h
 *
 *  Created on: 2017年8月31日
 *      Author: zxb
 */

#ifndef UTIL_SPLITERSTRING_H_
#define UTIL_SPLITERSTRING_H_
#if 0
#include <string>
#include <vector>
using namespace std;
class SpliterString {
public:
	///, space
	static void SpliteAuto(const string& strSrc,
			vector<string>& vSpliterStrArray) {
		Splite(strSrc, ",", vSpliterStrArray);
		if (vSpliterStrArray.size() <= 1) {
			Splite(strSrc, " ", vSpliterStrArray);
			return;
		} else
			return;
	}
	static void Splite(const string& strSrc, const char* delimiters,
			vector<string>& vSpliterStrArray) {
		vSpliterStrArray.clear();
		string strtemp = strSrc;
		while (1) {
			if (strtemp.length() == 0) //remian string is none
				return;
			size_t pos = strtemp.find(delimiters);
			if (pos == string::npos) { //can not find delimiters
				vSpliterStrArray.push_back(strtemp);
				return;
			} else {
				if (pos == 0) { //find is first pos, eg: delmiters + other
					strtemp = strtemp.substr(strlen(delimiters));
				} else {
					vSpliterStrArray.push_back(strtemp.substr(0, pos));
					strtemp = strtemp.substr(pos + strlen(delimiters));
				} //end else pos>0
			} //end else pos!=npos
		} //end while
	} //end func SpliterString
	static void SpliterHex(const string& strSrc, const char* delimiters,
			vector<uint8_t>& vHexArray) {
		vector<string> vHexNumStr;
		Splite(strSrc, delimiters, vHexNumStr);
		vHexArray.clear();
		for (size_t i = 0; i < vHexNumStr.size(); i++) {
			uint8_t temp = String2Hex(vHexNumStr[i]);
			vHexArray.push_back(temp);
		}
	}

	static void SpliterAutoHex(const string& strSrc,
			vector<uint8_t>& vHexArray) {
		vector<string> vHexNumStr;
//		Splite(strSrc, delimiters, vHexNumStr);
		SpliteAuto(strSrc, vHexNumStr);
		vHexArray.clear();
		for (size_t i = 0; i < vHexNumStr.size(); i++) {
			uint8_t temp = String2Hex(vHexNumStr[i]);
			vHexArray.push_back(temp);
		}
	}

	static uint8_t String2Hex(const string& strNum) throw (SystemException) {
		if (strNum.length() != 2) {
			throw SystemException("hex num error");
		}
		uint8_t h, l;
		h = Char2Hex(strNum[0]);
		l = Char2Hex(strNum[1]);
		return h * 16 + l;
	}
	static void Hex2String(const vector<uint8_t> vHexArray,
			const char* delimiters, string& strHex) {
		assert(delimiters && strlen(delimiters) < 7);
		char strNum[10];
		for (size_t i = 0; i < vHexArray.size(); i++) {
			sprintf(strNum, "%02x%s", vHexArray[i], delimiters);
			strHex += strNum;
		}
	}
	static uint8_t Char2Hex(const char cNum) throw (SystemException) {
		if (cNum >= '0' && cNum <= '9') {
			return cNum - '0';
		} else if (cNum >= 'a' && cNum <= 'f') {
			return cNum - 'a' + 10;
		} else if (cNum >= 'A' && cNum <= 'F') {
			return cNum - 'A' + 10;
		} else
			throw SystemException("char2hex num range error");
	}
	static bool CompareHex(const uint8_t*pArray1, int nArray1Len,
			const uint8_t*pArray2, int nArray2Len) {
		assert(pArray1 && pArray2);
		if (nArray1Len != nArray2Len)
			return false;
		for (int i = 0; i < nArray1Len; i++) {
			if (pArray1[i] != pArray2[i])
				return false;
		}
		return true;
	}
};
#endif
#endif /* UTIL_SPLITERSTRING_H_ */
