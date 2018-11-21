/*
 * GlobalProfile.cpp
 *
 *  Created on: 2017年7月23日
 *      Author: root
 */

#include "GlobalProfile.h"

#include <muduo/base/Logging.h>

//namespace DSPPAUtil{

GlobalProfile::Profile::Profile(const string &path) {
	_strcurpath = path;
	_bChanged = false;
	XMLResults result;
	_top = XMLNode::parseFile(path.c_str(), "NodeAppProfile", &result);
	if (_top.isEmpty()) {
		_top = XMLNode::createXMLTopNode("NodeAppProfile");
		_bChanged = true;
	}
}
GlobalProfile::Profile::~Profile() {
	if (_bChanged) {
		XMLError ret = _top.writeToFile(_strcurpath.c_str());
//		if (ret != eXMLErrorNone)
//			throw SyscallException(_EXCEPTION_POSTION_);
	}
}

const string GlobalProfile::Profile::getString(const char* section,
		const char* strKey, const string& strDefualtValue) {
	XMLNode childnode = _top.getChildNode(section);
	if (childnode.isEmpty()) {
		childnode = _top.addChild(section);
		_bChanged = true;
	}

	string strValue = strDefualtValue;
	XMLNode curnode = childnode.getChildNode(strKey);
	if (curnode.isEmpty()) {
		XMLNode child = childnode.addChild(strKey);
		child.addText(strValue.c_str());
		_bChanged = true;
	} else {
		const char * pTemp = curnode.getText();
		if (pTemp)
			strValue = pTemp;
		else
			strValue = "";
	}
	return strValue;
}
const int GlobalProfile::Profile::getInt(const char* section,
		const char* strKey, const int defualtvalue) {
	string strValue;
	char strNum[10];
	sprintf(strNum, "%d", defualtvalue);
	strValue = strNum;
	strValue = getString(section, strKey, strValue);
	return atoi(strValue.c_str());
}
const unsigned short GlobalProfile::Profile::getUShort(const char* section,
		const char* strKey, const unsigned short defualtvalue) {
	return getInt(section, strKey, (int) defualtvalue);
}
const bool GlobalProfile::Profile::getBool(const char* section,
		const char* strKey, const bool defualtvalue) {
	string strValue;
	char strNum[10];
	sprintf(strNum, "%d", defualtvalue);
	strValue = strNum;
	strValue = getString(section, strKey, strValue);
	return atoi(strValue.c_str());
}

void GlobalProfile::Profile::updateValue(const char* section,
		const char* strKey, const char* fmt, ...) {
	_bChanged = true;
	char strValue[2048];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(strValue, 2048, fmt, vl);
	va_end(vl);

	XMLNode childnode = _top.getChildNode(section);
	if (childnode.isEmpty()) { //section : empty child node
		childnode = _top.addChild(section); //add section
		XMLNode node = childnode.addChild(strKey); //add key
		node.addText(strValue);
	} else { //section: find ok
		XMLNode node = childnode.getChildNode(strKey);
		if (node.isEmpty()) { //strkey is empty
			node = childnode.addChild(strKey);
			node.addText(strValue);
		} else {
			node.updateText(strValue);
		}
	}
}
void GlobalProfile::Profile::updateProperty(const char* section,
		const char* strKey, const char* mainpropertyname,
		const char* mainpropertyvalue, const char* udpatepropertyname,
		const char* updateproertyvalue) {
	XMLNode childnode = _top.getChildNode(section);
	if (childnode.isEmpty()) {
		childnode = _top.addChild(section);
		XMLNode node = childnode.addChild(strKey);
		_bChanged = true;
		node.addAttribute(mainpropertyname, mainpropertyvalue);
		node.addAttribute(udpatepropertyname, updateproertyvalue);
		return;
	}
	//find old
	XMLNode item = childnode.getChildNodeWithAttribute(strKey, mainpropertyname,
			mainpropertyvalue);
	if (item.isEmpty()) { //not find
		XMLNode node = childnode.addChild(strKey);
		_bChanged = true;
		node.addAttribute(mainpropertyname, mainpropertyvalue);
		node.addAttribute(udpatepropertyname, updateproertyvalue);
		return;

	} else { //find ok
		const char* value = item.getAttribute(udpatepropertyname);
		_bChanged = true;
		if (value == NULL) {
			item.addAttribute(udpatepropertyname, updateproertyvalue);
		} else {
			item.updateAttribute(updateproertyvalue, udpatepropertyname,
					udpatepropertyname);
		}
	}
}
void GlobalProfile::Profile::enumeratorProperty(const char* section,
		const char* strKey, const vector<string>& propertyname,
		vector<vector<string> >& rows, vector<string>& values) {
	//1.get child node
	XMLNode childnode = _top.getChildNode(section);
	if (childnode.isEmpty()) {
		childnode = _top.addChild(section);
		_bChanged = true;
	}
	int nCount = childnode.nChildNode(strKey);
	for (int i = 0; i < nCount; i++) {
		XMLNode node = childnode.getChildNode(strKey, i);
		if (node.isEmpty()) {
			cout << "none xml node" << endl;
		} else { //read attribute
			vector<string> propertyvalues;
			for (int j = 0; j < (int) propertyname.size(); j++) {
				//read propertys
				const char* attributevalue = node.getAttribute(
						propertyname[j].c_str());
				if (attributevalue == NULL) {
					propertyvalues.push_back("error");
				} else {
					propertyvalues.push_back(attributevalue);
				}
			}
			rows.push_back(propertyvalues);
			const char* text = node.getText();
			if (text == NULL) {
				values.push_back("error");
			} else {
				values.push_back(text);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalProfilePtr GlobalProfile::g_Profile = 0;

GlobalProfile::GlobalProfile() :
		Shared() {
	loadFromJSON();
	loadFromXML();
}
GlobalProfile::~GlobalProfile() {

}

void GlobalProfile::printJson(Json::Value value) {
	Json::Value::Members mem = value.getMemberNames();
	for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end();
			iter++) {
		cout << *iter << "\t:  ";
		if (value[*iter].type() == Json::objectValue) {
			cout << endl;
			printJson(value[*iter]);
		} else if (value[*iter].type() == Json::arrayValue) {
			cout << endl;
			unsigned int count = value[*iter].size();
			for (unsigned int i = 0; i < count; i++) {
				printJson(value[*iter][i]);
			}
		} else if (value[*iter].type() == Json::stringValue) {
			printf("%s", value[*iter].asString().c_str());
			cout << endl;
		} else if (value[*iter].type() == Json::realValue) {
			printf("%lf", value[*iter].asDouble());
			cout << endl;
		} else if (value[*iter].type() == Json::uintValue) {
			printf("%u", value[*iter].asUInt());
			cout << endl;
		} else if (value[*iter].type() == Json::intValue) {
			printf("%d", value[*iter].asInt());
			cout << endl;
		} else if (value[*iter].type() == Json::booleanValue) {
			printf("%d", value[*iter].asBool());
			cout << endl;
		}
	}
	return;
}

void GlobalProfile::FreshPubDevList(void) {
	std::stringstream strJson;
	map<unsigned short, DP_DEV_PUBLIC_DEVLIST_INFO_S>::iterator itor, last =
			m_mPubDevInfo.end();

	strJson << "{\"" << "PubDevInfo" << "\":[";
	for (itor = m_mPubDevInfo.begin(); itor != last; itor++) {
		strJson << "{";
		strJson << "\"u16DevSn\":" << "\"" << itor->second.u16DevSn << "\",";
		strJson << "\"u16DevType\":" << "\"" << itor->second.u16DevType
				<< "\",";
		strJson << "\"u16DevNum\":" << "\"" << itor->second.u16DevNum << "\",";
		strJson << "\"au8DevName\":" << "\"" << itor->second.au8DevName
				<< "\",";
		strJson << "\"au8DevID\":" << "\"" << itor->second.au8DevID << "\",";
		strJson << "\"u32DevIP\":" << "\"" << itor->second.u32DevIP << "\",";
		strJson << "\"u16DevSoftVersion\":" << "\""
				<< itor->second.u16DevSoftVersion << "\",";
		strJson << "\"u16DevHardVersion\":" << "\""
				<< itor->second.u16DevHardVersion << "\"";
		strJson << "}";
		if ((itor++) != last) {
			strJson << ",";
		}
		itor--;
	}
	strJson << "]}";

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonRoot;
	std::ofstream oOFStream;

	if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
		std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

		oOFStream.open(PROFILE_JSON_PUBDEVINFO_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();
	}
}

void GlobalProfile::FreshPubUserInfo(void) {
	std::stringstream strJson;
	map<string, DP_DEV_PUBLIC_USER_INFO_S>::iterator itor, last =
			m_mPubUserInfo.end();

	strJson << "{\"" << "PubUserInfo" << "\":[";
	for (itor = m_mPubUserInfo.begin(); itor != last; itor++) {
		strJson << "{";
		strJson << "\"au8UserName\":" << "\"" << itor->second.au8UserName
				<< "\",";
		strJson << "\"au8PassWord\":" << "\"" << itor->second.au8PassWord
				<< "\",";
		strJson << "\"u8Priority\":" << "\"" << itor->second.u8Priority << "\"";
		strJson << "}";
		if ((itor++) != last) {
			strJson << ",";
		}
		itor--;
	}
	strJson << "]}";

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonRoot;
	std::ofstream oOFStream;

	if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
		std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

		oOFStream.open(PROFILE_JSON_USERINFO_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();
	}
}

void GlobalProfile::FreshPubPermission(void) {
	std::stringstream strJson;
	map<string, vector<DP_DEV_PUBLIC_USERPERMISSION_INFO_S> >::iterator itor,
			last = m_mPubUserPermissionInfo.end();
	vector<DP_DEV_PUBLIC_USERPERMISSION_INFO_S>::const_iterator vitor, vlast;

	strJson << "{\"" << "PubPermissionInfo" << "\":{";
	for (itor = m_mPubUserPermissionInfo.begin(); itor != last; itor++) {
		strJson << "\"" << itor->first << "\":[";

		vlast = itor->second.end();
		for (vitor = itor->second.begin(); vitor != vlast; vitor++) {
			strJson << "{";
			strJson << "\"u16DevSn\":" << "\"" << vitor->u16DevSn << "\",";
			strJson << "\"u8Permission\":" << "\"" << vitor->u8Permission
					<< "\"";
			strJson << "}";
			if ((vitor++) != vlast) {
				strJson << ",";
			}
			vitor--;
		}

		strJson << "]";
		if ((itor++) != last) {
			strJson << ",";
		}
		itor--;
	}
	strJson << "}}";

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonRoot;
	std::ofstream oOFStream;

	if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
		std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

		oOFStream.open(PROFILE_JSON_PERMISSION_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();
	}
}

void GlobalProfile::FreshMatrixSetInfo(void) {
	std::stringstream strJson;
	map<unsigned int, DP_DEV_PUBLIC_MOUSEMATRIXSETTING_INFO_S>::iterator itor,
			last = m_mPubMatrixSetInfo.end();

	strJson << "{\"" << "MatrixSetInfo" << "\":[";
	for (itor = m_mPubMatrixSetInfo.begin(); itor != last; itor++) {
		strJson << "{";
		strJson << "\"u32MatrixID\":" << "\"" << itor->second.u32MatrixID
				<< "\",";
		strJson << "\"au8MatrixName\":" << "\"" << itor->second.au8MatrixName
				<< "\",";
		strJson << "\"u8Row\":" << "\"" << itor->second.u8Row << "\",";
		strJson << "\"u8Colum\":" << "\"" << itor->second.u8Colum << "\"";
		strJson << "}";
		if ((itor++) != last) {
			strJson << ",";
		}
		itor--;
	}
	strJson << "]}";

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonRoot;
	std::ofstream oOFStream;

	if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
		std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

		oOFStream.open(PROFILE_JSON_MATRIXSET_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();
	}
}

void GlobalProfile::FreshMatrix(void) {
	std::stringstream strJson;
	map<unsigned int, vector<unsigned short> >::iterator itor, last =
			m_mPubMatrixInfo.end();
	vector<unsigned short>::const_iterator vitor, vlast;

	strJson << "{\"" << "MatrixInfo" << "\":{";
	for (itor = m_mPubMatrixInfo.begin(); itor != last; itor++) {
		strJson << "\"" << itor->first << "\":[";

		vlast = itor->second.end();
		for (vitor = itor->second.begin(); vitor != vlast; vitor++) {
			strJson << "{";
			strJson << "\"u16DevSn\":" << "\"" << *vitor << "\"";
			strJson << "}";
			if ((vitor++) != vlast) {
				strJson << ",";
			}
			vitor--;
		}

		strJson << "]";
		if ((itor++) != last) {
			strJson << ",";
		}
		itor--;
	}
	strJson << "}}";

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonRoot;
	std::ofstream oOFStream;

	if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
		std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

		oOFStream.open(PROFILE_JSON_MATRIX_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();
	}
}

void GlobalProfile::FreshIRList(void) {
	std::stringstream strJson;
	map<unsigned short, DP_DEV_IRLIST_INFO_S>::iterator itor, last =
			m_mIRListInfo.end();

	strJson << "{\"" << "IRList" << "\":[";
	for (itor = m_mIRListInfo.begin(); itor != last; itor++) {
		strJson << "{";
		strJson << "\"u8IRType\":" << "\"" << itor->second.u8IRType << "\",";
		strJson << "\"u8IRNum\":" << "\"" << itor->second.u8IRNum << "\",";
		strJson << "\"u16IRValue\":" << "\"" << itor->second.u16IRValue
				<< "\",";
		strJson << "}";
		if ((itor++) != last) {
			strJson << ",";
		}
		itor--;
	}
	strJson << "]}";

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonRoot;
	std::ofstream oOFStream;

	if (oJsonReader.parse(strJson.str(), oJsonRoot)) {
		std::string strBetterWrite = oJsonSWriter.write(oJsonRoot);

		oOFStream.open(PROFILE_JSON_IRLIST_PATH);
		oOFStream << strBetterWrite;
		oOFStream.close();
	}
}

void GlobalProfile::GetVersionInfo(
		DP_DEV_PUBLIC_LISTVERSION_INFO_S& VersionInfo) {
	memset(&VersionInfo, 0, sizeof(DP_DEV_PUBLIC_LISTVERSION_INFO_S));
	memcpy(&VersionInfo, &m_stPubVersionInfo,
			sizeof(DP_DEV_PUBLIC_LISTVERSION_INFO_S));
}
void GlobalProfile::SetVersionInfo(
		const DP_DEV_PUBLIC_LISTVERSION_INFO_S& VersionInfo) {
	memset(&m_stPubVersionInfo, 0, sizeof(DP_DEV_PUBLIC_LISTVERSION_INFO_S));
	memcpy(&m_stPubVersionInfo, &VersionInfo,
			sizeof(DP_DEV_PUBLIC_LISTVERSION_INFO_S));

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("VersionInfo", "u32DeviceListVersion", "%d",
			m_stPubVersionInfo.u32DeviceListVersion);
	profile->updateValue("VersionInfo", "u32OnlineDeviceListVersion", "%d",
			m_stPubVersionInfo.u32OnlineDeviceListVersion);
	profile->updateValue("VersionInfo", "u32UserInfoListVersion", "%d",
			m_stPubVersionInfo.u32UserInfoListVersion);
	profile->updateValue("VersionInfo", "u32UserAuthInfoListVersion", "%d",
			m_stPubVersionInfo.u32UserAuthInfoListVersion);
	profile->updateValue("VersionInfo", "u32MouseMatrixSetVersion", "%d",
			m_stPubVersionInfo.u32MouseMatrixSetVersion);
	profile->updateValue("VersionInfo", "u32MouseMatrixVersion", "%d",
			m_stPubVersionInfo.u32MouseMatrixVersion);
}
unsigned int GlobalProfile::GetDevOnlineListVersion(void) {
	return m_stPubVersionInfo.u32OnlineDeviceListVersion;
}
void GlobalProfile::SetDevOnlineListVersion(unsigned int version) {
	m_stPubVersionInfo.u32OnlineDeviceListVersion = version;

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("VersionInfo", "u32OnlineDeviceListVersion", "%d",
			m_stPubVersionInfo.u32OnlineDeviceListVersion);
}
unsigned int GlobalProfile::GetUserListVersion(void) {
	return m_stPubVersionInfo.u32UserInfoListVersion;
}
void GlobalProfile::SetUserListVersion(unsigned int version) {
	m_stPubVersionInfo.u32UserInfoListVersion = version;

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("VersionInfo", "u32UserInfoListVersion", "%d",
			m_stPubVersionInfo.u32UserInfoListVersion);
}
unsigned int GlobalProfile::GetPermissionListVersion(void) {
	return m_stPubVersionInfo.u32UserAuthInfoListVersion;
}
void GlobalProfile::SetPermissionListVersion(unsigned int version) {
	m_stPubVersionInfo.u32UserAuthInfoListVersion = version;

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("VersionInfo", "u32UserInfoListVersion", "%d",
			m_stPubVersionInfo.u32UserAuthInfoListVersion);
}
void GlobalProfile::GetBaseInfo(DP_DEV_BASE_INFO_S& baseInfo) {
	memset(&baseInfo, 0, sizeof(DP_DEV_BASE_INFO_S));
	memcpy(&baseInfo, &m_stDevBaseInfo, sizeof(DP_DEV_BASE_INFO_S));
}
void GlobalProfile::SetBaseInfo(const DP_DEV_BASE_INFO_S& baseInfo) {
	memset(&m_stDevBaseInfo, 0, sizeof(DP_DEV_BASE_INFO_S));
	memcpy(&m_stDevBaseInfo, &baseInfo, sizeof(DP_DEV_BASE_INFO_S));

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("BaseInfo", "u16DevSn", "%d",
			m_stDevBaseInfo.u16DevSn);
	profile->updateValue("BaseInfo", "u16Devtype", "%d",
			m_stDevBaseInfo.u16Devtype);
	profile->updateValue("BaseInfo", "u16DevNum", "%d",
			m_stDevBaseInfo.u16DevNum);
	profile->updateValue("BaseInfo", "au8DevName", "%s",
			m_stDevBaseInfo.au8DevName);
	profile->updateValue("BaseInfo", "au8DevID", "%s",
			m_stDevBaseInfo.au8DevID);
	profile->updateValue("BaseInfo", "u16DevWorkMode", "%d",
			m_stDevBaseInfo.u16DevWorkMode);
	profile->updateValue("BaseInfo", "u16DevFuncOnOff", "%d",
			m_stDevBaseInfo.u16DevFuncOnOff);
	profile->updateValue("BaseInfo", "u16DevSoftVersion", "%d",
			m_stDevBaseInfo.u16DevSoftVersion);
	profile->updateValue("BaseInfo", "u16DevHardVersion", "%d",
			m_stDevBaseInfo.u16DevHardVersion);
}
unsigned short GlobalProfile::GetFunctionOnOff(void) {
	return m_stDevBaseInfo.u16DevFuncOnOff;
}
void GlobalProfile::SetFunctionOnOff(unsigned short functionOnOff) {
	m_stDevBaseInfo.u16DevFuncOnOff = functionOnOff;

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("BaseInfo", "u16DevFuncOnOff", "%d",
			m_stDevBaseInfo.u16DevFuncOnOff);
}
void GlobalProfile::GetNetInfo(DP_DEV_NET_INFO_S& netInfo) {
	memset(&netInfo, 0, sizeof(DP_DEV_NET_INFO_S));
	memcpy(&netInfo, &m_stDevNetInfo, sizeof(DP_DEV_NET_INFO_S));
}
void GlobalProfile::SetNetInfo(const DP_DEV_NET_INFO_S& netInfo) {
	memset(&m_stDevNetInfo, 0, sizeof(DP_DEV_NET_INFO_S));
	memcpy(&m_stDevNetInfo, &netInfo, sizeof(DP_DEV_NET_INFO_S));

	ProfilePtr profile = new Profile(PROFILE_PATH);
	profile->updateValue("NetInfo", "u8PhyNums", "%d",
			m_stDevNetInfo.u8PhyNums);
	profile->updateValue("NetInfo", "u8PhyMode", "%d",
			m_stDevNetInfo.u8PhyMode);
	profile->updateValue("NetInfo", "u8PhyUsedOnBongding", "%d",
			m_stDevNetInfo.u8PhyUsedOnBongding);
}
void GlobalProfile::GetUARTInfo(DP_DEV_UARTSET_INFO_S& uartInfo,
		unsigned char num) {
	memset(&uartInfo, 0, sizeof(DP_DEV_UARTSET_INFO_S));
	memcpy(&uartInfo, &m_astDevUartInfo[num], sizeof(DP_DEV_UARTSET_INFO_S));
}
void GlobalProfile::SetUARTInfo(const DP_DEV_UARTSET_INFO_S& uartInfo,
		unsigned char num) {
	memset(&m_astDevUartInfo[num], 0, sizeof(DP_DEV_UARTSET_INFO_S));
	memcpy(&m_astDevUartInfo[num], &uartInfo, sizeof(DP_DEV_UARTSET_INFO_S));

	ProfilePtr profile = new Profile(PROFILE_PATH);
	if (num == 0) {
		profile->updateValue("UartInfo0", "u8UartID", "%d",
				m_astDevUartInfo[num].u8UartID);
		profile->updateValue("UartInfo0", "u16BaudRate", "%d",
				m_astDevUartInfo[num].u16BaudRate);
		profile->updateValue("UartInfo0", "u8WordLength", "%d",
				m_astDevUartInfo[num].u8WordLength);
		profile->updateValue("UartInfo0", "u8StopBit", "%d",
				m_astDevUartInfo[num].u8StopBit);
		profile->updateValue("UartInfo0", "u8Parity", "%d",
				m_astDevUartInfo[num].u8Parity);
	} else if (num == 1) {
		profile->updateValue("UartInfo1", "u8UartID", "%d",
				m_astDevUartInfo[num].u8UartID);
		profile->updateValue("UartInfo1", "u16BaudRate", "%d",
				m_astDevUartInfo[num].u16BaudRate);
		profile->updateValue("UartInfo1", "u8WordLength", "%d",
				m_astDevUartInfo[num].u8WordLength);
		profile->updateValue("UartInfo1", "u8StopBit", "%d",
				m_astDevUartInfo[num].u8StopBit);
		profile->updateValue("UartInfo1", "u8Parity", "%d",
				m_astDevUartInfo[num].u8Parity);
	} else if (num == 2) {
		profile->updateValue("UartInfo2", "u8UartID", "%d",
				m_astDevUartInfo[num].u8UartID);
		profile->updateValue("UartInfo2", "u16BaudRate", "%d",
				m_astDevUartInfo[num].u16BaudRate);
		profile->updateValue("UartInfo2", "u8WordLength", "%d",
				m_astDevUartInfo[num].u8WordLength);
		profile->updateValue("UartInfo2", "u8StopBit", "%d",
				m_astDevUartInfo[num].u8StopBit);
		profile->updateValue("UartInfo2", "u8Parity", "%d",
				m_astDevUartInfo[num].u8Parity);
	}
}

void GlobalProfile::loadFromJSON() {
	if (access(PROFILE_JSON_DIR, F_OK) != 0) {
		if (mkdir(PROFILE_JSON_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
				== -1) {
			LOG_FATAL << "Can not access and mkdir : " << PROFILE_JSON_DIR;
		}
	}

	unsigned short u16Count;

	Json::Reader oJsonReader;
	Json::StyledWriter oJsonSWriter;
	Json::Value oJsonValue;
	Json::Value oJsonValueTmp;
	Json::Value::Members oMember;

	std::ofstream oOFStream;
	std::ifstream oIFStream;

	m_mPubDevInfo.clear();
	m_mPubUserInfo.clear();
	m_mPubUserPermissionInfo.clear();
	m_mPubMatrixSetInfo.clear();
	m_mPubMatrixInfo.clear();
	m_mIRListInfo.clear();

//PubDeviceInfo
	oIFStream.open(PROFILE_JSON_PUBDEVINFO_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();

		const char* strJson = "{\"PubDevInfo\": []}";
		oJsonReader.parse(strJson, oJsonValueTmp);
		std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
		oOFStream.open(PROFILE_JSON_PUBDEVINFO_PATH);
		oOFStream << strWrite;
		oOFStream.close();
	} else {
		oIFStream.close();
		if (oJsonValue.isMember("PubDevInfo")) {
			if (!oJsonValue["PubDevInfo"].isNull()) {
				DP_DEV_PUBLIC_DEVLIST_INFO_S stDevListTmp;
				for (u16Count = 0; u16Count < oJsonValue["PubDevInfo"].size();
						u16Count++) {

					stDevListTmp.u16DevSn =
							oJsonValue["PubDevInfo"][u16Count]["u16DevSn"].asUInt();
					stDevListTmp.u16DevType =
							oJsonValue["PubDevInfo"][u16Count]["u16DevType"].asUInt();
					stDevListTmp.u16DevNum =
							oJsonValue["PubDevInfo"][u16Count]["u16DevNum"].asUInt();
					strcpy((char*) stDevListTmp.au8DevName,
							oJsonValue["PubDevInfo"][u16Count]["au8DevName"].asString().c_str());
					strcpy((char*) stDevListTmp.au8DevID,
							oJsonValue["PubDevInfo"][u16Count]["au8DevID"].asString().c_str());
					stDevListTmp.u32DevIP =
							oJsonValue["PubDevInfo"][u16Count]["u32DevIP"].asUInt();
					stDevListTmp.u16DevSoftVersion =
							oJsonValue["PubDevInfo"][u16Count]["u16DevSoftVersion"].asUInt();
					stDevListTmp.u16DevHardVersion =
							oJsonValue["PubDevInfo"][u16Count]["u16DevHardVersion"].asUInt();

					m_mPubDevInfo[(unsigned short) oJsonValue["PubDevInfo"][u16Count]["u16DevSn"].asUInt()] =
							stDevListTmp;
				}
			}
		} else {
			const char* strJson = "{\"PubDevInfo\": []}";
			oJsonReader.parse(strJson, oJsonValueTmp);
			std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
			oOFStream.open(PROFILE_JSON_PUBDEVINFO_PATH);
			oOFStream << strWrite;
			oOFStream.close();
		}
		//size:0
	}

//PubUserInfo
	oIFStream.open(PROFILE_JSON_USERINFO_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();

		const char* strJson = "{\"PubUserInfo\": []}";
		oJsonReader.parse(strJson, oJsonValueTmp);
		std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
		oOFStream.open(PROFILE_JSON_USERINFO_PATH);
		oOFStream << strWrite;
		oOFStream.close();
	} else {
		oIFStream.close();
		if (oJsonValue.isMember("PubUserInfo")) {
			if (!oJsonValue["PubUserInfo"].isNull()) {
				DP_DEV_PUBLIC_USER_INFO_S stUserInfoTmp;
				for (u16Count = 0; u16Count < oJsonValue["PubUserInfo"].size();
						u16Count++) {

					strcpy((char*) stUserInfoTmp.au8UserName,
							oJsonValue["PubUserInfo"][u16Count]["au8UserName"].asString().c_str());
					strcpy((char*) stUserInfoTmp.au8PassWord,
							oJsonValue["PubUserInfo"][u16Count]["au8PassWord"].asString().c_str());
					stUserInfoTmp.u8Priority =
							oJsonValue["PubUserInfo"][u16Count]["u8Priority"].asUInt();

					m_mPubUserInfo[oJsonValue["PubUserInfo"][u16Count]["au8UserName"].asCString()] =
							stUserInfoTmp;
				}
			}
		} else {
			const char* strJson = "{\"PubUserInfo\": []}";
			oJsonReader.parse(strJson, oJsonValueTmp);
			std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
			oOFStream.open(PROFILE_JSON_USERINFO_PATH);
			oOFStream << strWrite;
			oOFStream.close();
		}
		//size:0
	}

//PubUserPermissionInfo
	oIFStream.open(PROFILE_JSON_PERMISSION_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();

		const char* strJson = "{\"PubPermissionInfo\": []}";
		oJsonReader.parse(strJson, oJsonValueTmp);
		std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
		oOFStream.open(PROFILE_JSON_PERMISSION_PATH);
		oOFStream << strWrite;
		oOFStream.close();
	} else {
		oIFStream.close();
		if (oJsonValue.isMember("PubPermissionInfo")) {
			if (!oJsonValue["PubPermissionInfo"].isNull()) {
				DP_DEV_PUBLIC_USERPERMISSION_INFO_S stUserPermissionTmp;
				vector<DP_DEV_PUBLIC_USERPERMISSION_INFO_S> vPermissionTmp;

				oMember = oJsonValue.getMemberNames();
				for (Json::Value::Members::iterator iter = oMember.begin();
						iter != oMember.end(); iter++) {
					if ((*iter == "PubPermissionInfo") || (*iter == "u16DevSn")
							|| (*iter == "u8Permission")) {
						continue;
					}

					for (u16Count = 0;
							u16Count
									< oJsonValue["PubPermissionInfo"][*iter].size();
							u16Count++) {
						stUserPermissionTmp.u16DevSn =
								oJsonValue["PubPermissionInfo"][*iter][u16Count]["u16DevSn"].asUInt();
						stUserPermissionTmp.u8Permission =
								oJsonValue["PubPermissionInfo"][*iter][u16Count]["u8Permission"].asUInt();
						vPermissionTmp.push_back(stUserPermissionTmp);
					}
//					m_lPubUserPermissionTmp.push_back(vPermissionTmp);

					m_mPubUserPermissionInfo[*iter] = vPermissionTmp;
				}
			}
		} else {
			const char* strJson = "{\"PubPermissionInfo\": []}";
			oJsonReader.parse(strJson, oJsonValueTmp);
			std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
			oOFStream.open(PROFILE_JSON_PERMISSION_PATH);
			oOFStream << strWrite;
			oOFStream.close();
		}
		//size:0
	}

//MatrixSetInfo
	oIFStream.open(PROFILE_JSON_MATRIXSET_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();

		const char* strJson = "{\"MatrixSetInfo\": []}";
		oJsonReader.parse(strJson, oJsonValueTmp);
		std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
		oOFStream.open(PROFILE_JSON_MATRIXSET_PATH);
		oOFStream << strWrite;
		oOFStream.close();
	} else {
		oIFStream.close();
		if (oJsonValue.isMember("MatrixSetInfo")) {
			if (!oJsonValue["MatrixSetInfo"].isNull()) {
				DP_DEV_PUBLIC_MOUSEMATRIXSETTING_INFO_S stMatrixSetInfoTmp;
				for (u16Count = 0;
						u16Count < oJsonValue["MatrixSetInfo"].size();
						u16Count++) {

					stMatrixSetInfoTmp.u32MatrixID =
							oJsonValue["MatrixSetInfo"][u16Count]["u32MatrixID"].asUInt();
					strcpy((char*) stMatrixSetInfoTmp.au8MatrixName,
							oJsonValue["MatrixSetInfo"][u16Count]["au8MatrixName"].asString().c_str());
					stMatrixSetInfoTmp.u8Row =
							oJsonValue["MatrixSetInfo"][u16Count]["u8Row"].asUInt();
					stMatrixSetInfoTmp.u8Colum =
							oJsonValue["MatrixSetInfo"][u16Count]["u8Colum"].asUInt();

					m_mPubMatrixSetInfo[oJsonValue["MatrixSetInfo"][u16Count]["u32MatrixID"].asUInt()] =
							stMatrixSetInfoTmp;
				}
			}
		} else {
			const char* strJson = "{\"MatrixSetInfo\": []}";
			oJsonReader.parse(strJson, oJsonValueTmp);
			std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
			oOFStream.open(PROFILE_JSON_MATRIXSET_PATH);
			oOFStream << strWrite;
			oOFStream.close();
		}
		//size:0
	}

//MatrixInfo
	unsigned int u32MatrixIDTmp;
	oIFStream.open(PROFILE_JSON_MATRIX_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();

		const char* strJson = "{\"MatrixInfo\": []}";
		oJsonReader.parse(strJson, oJsonValueTmp);
		std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
		oOFStream.open(PROFILE_JSON_MATRIX_PATH);
		oOFStream << strWrite;
		oOFStream.close();
	} else {
		oIFStream.close();
		if (oJsonValue.isMember("MatrixInfo")) {
			if (!oJsonValue["MatrixInfo"].isNull()) {
				unsigned short u16DevSnTmp;
				vector<unsigned short> vDevSnTmp;

				oMember = oJsonValue.getMemberNames();
				for (Json::Value::Members::iterator iter = oMember.begin();
						iter != oMember.end(); iter++) {
					if ((*iter == "MatrixInfo") || (*iter == "u16DevSn")) {
						continue;
					}

					for (u16Count = 0;
							u16Count < oJsonValue["MatrixInfo"][*iter].size();
							u16Count++) {
						u16DevSnTmp =
								oJsonValue["MatrixInfo"][*iter][u16Count]["u16DevSn"].asUInt();
						vDevSnTmp.push_back(u16DevSnTmp);
					}

					u32MatrixIDTmp = str2int(*iter);
					m_mPubMatrixInfo[u32MatrixIDTmp] = vDevSnTmp;
				}
			}
		} else {
			const char* strJson = "{\"MatrixInfo\": []}";
			oJsonReader.parse(strJson, oJsonValueTmp);
			std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
			oOFStream.open(PROFILE_JSON_MATRIX_PATH);
			oOFStream << strWrite;
			oOFStream.close();
		}
		//size:0
	}

//IRListInfo
	oIFStream.open(PROFILE_JSON_IRLIST_PATH, std::ios::binary);
	if (!oJsonReader.parse(oIFStream, oJsonValue, FALSE)) {
		oIFStream.close();

		const char* strJson = "{\"IRList\": []}";
		oJsonReader.parse(strJson, oJsonValueTmp);
		std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
		oOFStream.open(PROFILE_JSON_IRLIST_PATH);
		oOFStream << strWrite;
		oOFStream.close();
	} else {
		oIFStream.close();
		if (oJsonValue.isMember("IRList")) {
			if (!oJsonValue["IRList"].isNull()) {
				DP_DEV_IRLIST_INFO_S stIRList;

				for (u16Count = 0; u16Count < oJsonValue["IRList"].size();
						u16Count++) {

					stIRList.u8IRType =
							oJsonValue["IRList"][u16Count]["u8IRType"].asUInt();
					stIRList.u8IRNum =
							oJsonValue["IRList"][u16Count]["u8IRNum"].asUInt();
					stIRList.u16IRValue =
							oJsonValue["IRList"][u16Count]["u16IRValue"].asUInt();

					m_mIRListInfo[((stIRList.u8IRType << 8) + stIRList.u8IRNum)] =
							stIRList;
				}
			}
		} else {
			const char* strJson = "{\"IRList\": []}";
			oJsonReader.parse(strJson, oJsonValueTmp);
			std::string strWrite = oJsonSWriter.write(oJsonValueTmp);
			oOFStream.open(PROFILE_JSON_IRLIST_PATH);
			oOFStream << strWrite;
			oOFStream.close();
		}
		//size:0
	}

	cout << "loadjson success" << endl;
}

void GlobalProfile::loadFromXML() {
	if (access(PROFILE_DIR, F_OK) != 0) {
		if (mkdir(PROFILE_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
//			throw SyscallException(_EXCEPTION_POSTION_);
			;
		}
	}
	ProfilePtr profile = new Profile(PROFILE_PATH);

	m_stPubVersionInfo.u32DeviceListVersion = profile->getInt("VersionInfo",
			"u32DeviceListVersion", 0);
	m_stPubVersionInfo.u32OnlineDeviceListVersion = profile->getInt(
			"VersionInfo", "u32OnlineDeviceListVersion", 0);
	m_stPubVersionInfo.u32UserInfoListVersion = profile->getInt("VersionInfo",
			"u32UserInfoListVersion", 0);
	m_stPubVersionInfo.u32UserAuthInfoListVersion = profile->getInt(
			"VersionInfo", "u32UserAuthInfoListVersion", 0);
	m_stPubVersionInfo.u32MouseMatrixSetVersion = profile->getInt("VersionInfo",
			"u32MouseMatrixSetVersion", 0);
	m_stPubVersionInfo.u32MouseMatrixVersion = profile->getInt("VersionInfo",
			"u32MouseMatrixVersion", 0);

	m_stDevBaseInfo.u16DevSn = profile->getUShort("BaseInfo", "u16DevSn",
			0xFFFF);
	m_stDevBaseInfo.u16Devtype = profile->getUShort("BaseInfo", "u16Devtype",
			0xFFFF);
	m_stDevBaseInfo.u16DevNum = profile->getUShort("BaseInfo", "u16DevNum",
			0xFFFF);
	//注意'/0'
	strcpy((char*) m_stDevBaseInfo.au8DevName,
			profile->getString("BaseInfo", "au8DevName", "未命名").c_str());
	strcpy((char*) m_stDevBaseInfo.au8DevID,
			profile->getString("BaseInfo", "au8DevID", "FFFFFFFFFFFF").c_str());
	m_stDevBaseInfo.u16DevWorkMode = profile->getUShort("BaseInfo",
			"u16DevWorkMode", 0xFFFF);
	m_stDevBaseInfo.u16DevFuncOnOff = profile->getUShort("BaseInfo",
			"u16DevFuncOnOff", 0xFFFF);
	m_stDevBaseInfo.u16DevSoftVersion = profile->getUShort("BaseInfo",
			"u16DevSoftVersion", 0x0001);
	m_stDevBaseInfo.u16DevHardVersion = profile->getUShort("BaseInfo",
			"u16DevHardVersion", 0x0001);

	m_stDevNetInfo.u8PhyNums = profile->getUShort("NetInfo", "u8PhyNums",
			0x0002);
	m_stDevNetInfo.u8PhyMode = profile->getUShort("NetInfo", "u8PhyMode",
			0x0001);
	m_stDevNetInfo.u8PhyUsedOnBongding = profile->getUShort("NetInfo",
			"u8PhyUsedOnBongding", 0x0000);

	m_astDevUartInfo[0].u8UartID = profile->getUShort("UartInfo0", "u8UartID",
			0x0000);
	m_astDevUartInfo[0].u16BaudRate = profile->getUShort("UartInfo0",
			"u16BaudRate", 9600);
	m_astDevUartInfo[0].u8WordLength = profile->getUShort("UartInfo0",
			"u8WordLength", 8);
	m_astDevUartInfo[0].u8StopBit = profile->getUShort("UartInfo0", "u8StopBit",
			1);
	m_astDevUartInfo[0].u8Parity = profile->getUShort("UartInfo0", "u8Parity",
			0x0000);

	m_astDevUartInfo[1].u8UartID = profile->getUShort("UartInfo1", "u8UartID",
			0x0001);
	m_astDevUartInfo[1].u16BaudRate = profile->getUShort("UartInfo1",
			"u16BaudRate", 9600);
	m_astDevUartInfo[1].u8WordLength = profile->getUShort("UartInfo1",
			"u8WordLength", 8);
	m_astDevUartInfo[1].u8StopBit = profile->getUShort("UartInfo1", "u8StopBit",
			1);
	m_astDevUartInfo[1].u8Parity = profile->getUShort("UartInfo1", "u8Parity",
			0x0000);

	m_astDevUartInfo[2].u8UartID = profile->getUShort("UartInfo2", "u8UartID",
			0x0002);
	m_astDevUartInfo[2].u16BaudRate = profile->getUShort("UartInfo2",
			"u16BaudRate", 9600);
	m_astDevUartInfo[2].u8WordLength = profile->getUShort("UartInfo2",
			"u8WordLength", 8);
	m_astDevUartInfo[2].u8StopBit = profile->getUShort("UartInfo2", "u8StopBit",
			1);
	m_astDevUartInfo[2].u8Parity = profile->getUShort("UartInfo2", "u8Parity",
			0x0000);

	profile = 0;
	cout << "loadxml success" << endl;
}

//} /* namespace DSPPAUtil */
