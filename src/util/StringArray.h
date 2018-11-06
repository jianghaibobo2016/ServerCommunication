/*
 * StringArray.h
 *
 *  Created on: 2018年7月31日
 *      Author: zxb
 */

#ifndef DSPPAUTIL_UTIL_STRINGARRAY_H_
#define DSPPAUTIL_UTIL_STRINGARRAY_H_
#include <vector>
using namespace std;

namespace DSPPAUtil {
/**字符串数组
 * @note:该类不是线程安全的*/
class StringStorage;
class StringArray {
public:
	/**构造函数*/
	StringArray();

	/**析构函数*/
	virtual ~StringArray();

	/**增加字符串*/
	void addString(const char* strtemp);

	/**增加字符串*/
	void addString(const StringStorage& strtemp);

	/**修改指定位置字符串内容*/
	void updateItem(size_t nIndex, const char* strtemp);

	/**修改指定位置字符串内容*/
	void updateItem(size_t nIndex, const StringStorage& strtemp);

	/**删除指定位置的字符串*/
	void deleteItem(size_t nIndex);

	/**清空字符串数组*/
	void clear();

	/**获取字符串个数*/
	size_t getCount();

	/**获取指定位置的字符串
	 * @return NULL：nINdex越界 other：正确的字符串
	 */
	const char* getAt(size_t nIndex);

	/**获取指定位置的字符串
	* @return false：nINdex越界 true：strItem为正确的字符串
	*/
	bool getAt(size_t nIndex, StringStorage& strItem);

private:
	vector<StringStorage> _array;
};

} /* namespace DSPPAUtil */

#endif /* DSPPAUTIL_UTIL_STRINGARRAY_H_ */
