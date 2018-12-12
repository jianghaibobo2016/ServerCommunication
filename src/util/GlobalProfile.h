/*
 * GlobalProfile.h
 *
 *  Created on: 2017年7月23日
 *      Author: zxb
 */

#ifndef UTIL_GLOBALPROFILE_H_
#define UTIL_GLOBALPROFILE_H_
#if 1
#include "stdio.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <list>
#include <vector>
#include <map>

#include "Public.h"
//#include "Shared.h"
//#include "Handle.h"
//#include "InternalException.h"
#include <json/json.h>
#include "xmlParser.h"
#include "Data.h"
#include "RCSP.h"

using namespace std;

//namespace DSPPAUtil{

#define PROFILE_DIR 		"./"
#define PROFILE_PATH   		"./NodeApp_conf.xml"
#define PROFILE_JSON_DIR    "./json"
#define PROFILE_JSON_PUBDEVINFO_PATH   "./json/Public_DeviceInfo.json"
#define PROFILE_JSON_USERINFO_PATH   "./json/Public_UserInfo.json"
#define PROFILE_JSON_PERMISSION_PATH   "./json/Public_Permission.json"
#define PROFILE_JSON_MATRIXSET_PATH   "./json/Public_MatrixSet.json"
#define PROFILE_JSON_MATRIX_PATH   "./json/Public_Matrix.json"
#define PROFILE_JSON_IRLIST_PATH   "./json/Public_IRList.json"

class GlobalProfile/*: public Shared */{
private:
	class Profile/*: public Shared */{
	public:
		Profile(const string &path);
		~Profile();

		const string getString(const char* section, const char* strKey,
				const string& strDefualtValue);
		const int getInt(const char* section, const char* strKey,
				const int defualtvalue);
		const unsigned short getUShort(const char* section, const char* strKey,
				const unsigned short defualtvalue);
		const bool getBool(const char* section, const char* strKey,
				const bool defualtvalue);

		void updateValue(const char* section, const char* strKey,
				const char* fmt, ...);
		void updateProperty(const char* section, const char* strKey,
				const char* mainpropertyname, const char* mainpropertyvalue,
				const char* udpatepropertyname, const char* updateproertyvalue);
		void enumeratorProperty(const char* section, const char* strKey,
				const vector<string>& propertyname,
				vector<vector<string> >& rows, vector<string>& values);

	private:
		string _strcurpath;
		XMLNode _top;
		bool _bChanged;
	};
	typedef SmartPtr<Profile> ProfilePtr;

public:
	GlobalProfile();
	virtual ~GlobalProfile();

	static SmartPtr<GlobalProfile> getInstance() {
		if (!g_Profile.get()) {
			static SmartPtr<GlobalProfile> g_Profile_2(new GlobalProfile());
			g_Profile = g_Profile_2;
		}
		return g_Profile;
	}

	static SmartPtr<GlobalProfile> g_Profile;
	/////////////////////////////////////////////////////////////////////

	void printJson(Json::Value value);

	void FreshPubDevList(void);
	void FreshPubUserInfo(void);
	void FreshPubPermission(void);
	void FreshMatrixSetInfo(void);
	void FreshMatrix(void);
	void FreshIRList(void);

	/////////////////////////////////////////////////////////////////////
	void GetVersionInfo(DP_DEV_PUBLIC_LISTVERSION_INFO_S& VersionInfo);
	void SetVersionInfo(const DP_DEV_PUBLIC_LISTVERSION_INFO_S& VersionInfo);

	unsigned int GetDevOnlineListVersion(void);
	void SetDevOnlineListVersion(unsigned int version);

	unsigned int GetUserListVersion(void);
	void SetUserListVersion(unsigned int version);

	unsigned int GetPermissionListVersion(void);
	void SetPermissionListVersion(unsigned int version);

	void GetBaseInfo(DP_DEV_BASE_INFO_S& baseInfo);
	void SetBaseInfo(const DP_DEV_BASE_INFO_S& baseInfo);

	unsigned short GetFunctionOnOff(void);
	void SetFunctionOnOff(unsigned short functionOnOff);

	void GetNetInfo(DP_DEV_NET_INFO_S& netInfo);
	void SetNetInfo(const DP_DEV_NET_INFO_S& netInfo);

	void GetUARTInfo(DP_DEV_UARTSET_INFO_S& uartInfo, unsigned char num);
	void SetUARTInfo(const DP_DEV_UARTSET_INFO_S& uartInfo, unsigned char num);

private:
	void loadFromJSON();
	void loadFromXML();

public:
	//1.设备组网列表
	map<unsigned short, DP_DEV_PUBLIC_DEVLIST_INFO_S> m_mPubDevInfo;
	//3.用户信息列表
	map<string, DP_DEV_PUBLIC_USER_INFO_S> m_mPubUserInfo;
	//4.用户权限信息列表
	map<string, vector<DP_DEV_PUBLIC_USERPERMISSION_INFO_S> > m_mPubUserPermissionInfo;
	//5.鼠标矩阵配置信息列表
	map<unsigned int, DP_DEV_PUBLIC_MOUSEMATRIXSETTING_INFO_S> m_mPubMatrixSetInfo;
	//6.鼠标矩阵列表
	map<unsigned int, vector<unsigned short> > m_mPubMatrixInfo;
	//7777

	//9.红外码
	map<unsigned short, DP_DEV_IRLIST_INFO_S> m_mIRListInfo;

	//////////////////////////////////////////////////
private:
	//7.版本列表
	DP_DEV_PUBLIC_LISTVERSION_INFO_S m_stPubVersionInfo;

	//1.基本信息
	DP_DEV_BASE_INFO_S m_stDevBaseInfo;
	//3.网络信息
	DP_DEV_NET_INFO_S m_stDevNetInfo;
	//10.串口配置
	DP_DEV_UARTSET_INFO_S m_astDevUartInfo[3];
};

typedef SmartPtr<GlobalProfile> GlobalProfilePtr;

//} /* namespace DSPPAUtil */

#endif
#endif /* UTIL_GLOBALPROFILE_H_ */
