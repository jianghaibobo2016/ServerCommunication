/*
 * ConvertString.h
 *
 *  Created on: 2017年8月5日
 *      Author: zxb
 */

#ifndef UTIL_CONVERTSTRING_H_
#define UTIL_CONVERTSTRING_H_
#if 0
#include <iconv.h>
#include "SystemException.h"
class ConvertString {
public:
	enum eConvertStringFmt{
		CONVERT_STRING_UTF8 = 0,
		CONVERT_STRING_GB2312 = 1,
	};
	static void Convert(eConvertStringFmt srcFmt, char* srcStr, size_t srclen,
			eConvertStringFmt dstFmt, char* dstStr, size_t dstlen) throw(SystemException){
		iconv_t cd;
		//int rc;
		string strSrcFmt,strDstFmt;
		strSrcFmt = FormatName(srcFmt);
		strDstFmt = FormatName(dstFmt);
		if (strSrcFmt==strDstFmt){
			throw SystemException("Conert string, can not same format");
		}
		cd = iconv_open(strDstFmt.c_str(),strSrcFmt.c_str());
		if (cd==0)
			throw SystemException("call iconv_open failure");
		memset(dstStr,0,dstlen);
		if (iconv(cd,&srcStr,&srclen,&dstStr,&dstlen)==(size_t)-1)
			throw SystemException("convert#######");
		iconv_close(cd);
		return ;
	}
private:
	static string FormatName(eConvertStringFmt fmt)throw(SystemException){
		switch (fmt) {
		case CONVERT_STRING_UTF8:
			return "utf8";
		case CONVERT_STRING_GB2312:
			return "gbk";
		default:
			throw SystemException("Not support Conertion string format");
		}
	}
};
#endif
#endif /* UTIL_CONVERTSTRING_H_ */
