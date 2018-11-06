/*
 * StringStorage.h
 *
 *  Created on: 2018年7月31日
 *      Author: zxb
 */

#ifndef DSPPAUTIL_UTIL_STRINGSTORAGE_H_
#define DSPPAUTIL_UTIL_STRINGSTORAGE_H_

#include <vector>
#include <string>
using namespace std;
namespace DSPPAUtil {


/**
 String class.
 */
class StringArray;
class StringStorage {
public:

	/**
	 默认构造函数，创建空字符串
	 */
	StringStorage();
	/**
	 构造新的的字符串
	 @param pStr 新字符串.
	 */
	StringStorage(const char* pStrcopy);

	/**
	 构造函数复制
	 @param strcopy 复制字符串.
	 */
	StringStorage(const StringStorage& strtemp);

	StringStorage(const string& strtemp);

	/**
	 Class destructor.
	 */
	virtual ~StringStorage();

	/**
	 * 设置字符串
	 * @param 新字符串，字符串指针不能为空.
	 */
	void setString(const char* pStrCopy);

	/**
	 * 设置字符串
	 * @param 新字符串
	*/
	void setString(const StringStorage& strcopy);

	/**格式化赋值字符串*/
	void format(const char* fmt, ...);

	/**格式化拼接字符串*/
	void appendformat(const char* fmt, ...);


	/**
	 * 获取字符串长度
	 * @return 返回字符串长度 （0～n）
	 */
	const size_t length()const ;

	/**
	 * 获取字符串指定位置的子串
	 * @param [OUT] substr 获取字符串
	 * @param [IN] startIndex:起始位置 count:个数
	 * @return true:获取成功 false:获取失败
	 */
	bool getSubstring(StringStorage &substr,  size_t startIndex,  size_t count) const;

	/** 分割字符串
	 * @param [IN]delimiters分隔符
	 * @param [OUT]strArray分割后的返回数组
	 */
	void split(const char *delimiters, StringArray& strArray) const;


	/**
	 * 获取字符串
	 * @return 字符串已‘\0’结束
	 */
	const char* c_str()const;


	/**获取字符串首部指定个数的字符*/
	StringStorage left(size_t count);

	/**获取字符串尾部指定个数的字符*/
	StringStorage right(size_t count);

	/**获取字符串任意位置指定个数的字符*/
	StringStorage mid(size_t startIndex,size_t count);

	/**清除字符串首尾多余的空格*/
	StringStorage& trim();

	/**清除字符串首部空格*/
	StringStorage& trimleft();

	/**清除字符串尾部空格*/
	StringStorage& trimright();

	/*判断字符串是否为空
	 * @return true:空字符串 false：非空*/
	bool isEmpty();

	/**将字符串轻质转换为大写*/
	StringStorage& makeUpper();

	/**将字符串轻质转换为小写*/
	StringStorage& makeLower();

	/**
	 * 查找字符串出现的位置
	 * @return -1:未找到,0~n:位置*/
	int find(const char* str);

	/**
	 * 反向查找字符串出现的位置
	 * @return @see find
	 */
	int reversefind(const char* str);

	/**
	* 查找字符出现的位置
	* @return @see find
	*/
	int findchar(const char ch);

	/**替换字符串
	 * @param  oldsubstring 不能等于 newsubstring
	 */
	void replace(const char* oldsubstring, const char* newsubstring);

	/**删除指定位置的字符串*/
	void remove(int startIndex, size_t count);

	/**清空字符串*/
	void clear();

	/**赋值操作
	 * @return 返回新的字符串
	 * */
	 StringStorage& operator = (const StringStorage &other);

	 StringStorage& operator = (const string &other);


	 /**赋值操作
	 	 * @return 返回新的字符串
	 	 * */
	 StringStorage& operator = (const char* str);


	 /**
	  * 比较字符串是否相等
	  * @return true:相等 false：不想等
	  * */
	 bool operator == (const StringStorage &str) const;

	 /**
	 * 比较字符串是否相等
	 * @return true:相等 false：不想等
	 */
	 bool operator == (const char* str) const;

	 /**比较字符串*/
	 bool operator < (const StringStorage &str) const;

	 /**比较字符串*/
	 bool operator < (const char* str) const;

	 /**比较字符串*/
	 bool operator > (const StringStorage &str) const;

	 /**比较字符串*/
	 bool operator > (const char* str) const;

	 /**拼接字符串*/
	 void operator += (const char* str);

	 /**拼接字符串*/
	 void operator += (const StringStorage &str);

	 void operator += (const string &str);

	 /**拼接字符串*/
	 StringStorage& operator + (const char* str);

	 /**拼接字符串*/
	 StringStorage& operator + (const StringStorage &str);

	 StringStorage& operator + (const string &str);


private:
	
	 vector<char> _strSource;
};

} /* namespace DSPPAUtil */

#endif /* DSPPAUTIL_UTIL_STRINGSTORAGE_H_ */
