/*
 * SetNetwork.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_SETNETWORK_H_
#define SRC_SETNETWORK_H_

#include <strings.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <arpa/inet.h>
#include "dp_sd_type.h"
#include <muduo/base/Mutex.h>
//using namespace std;

/************************static const variables*******************************/
/*For modifying INI file.*/
static const DP_S32 SUBNETFLAG = 1; /// check subnet
static const DP_S32 SETMAC = 1;
static const DP_CHAR LEFT_BRACE = '[';
static const DP_CHAR RIGHT_BRACE = ']';
/************************static const variables*******************************/

/****************************local struct*************************************/
/*For recording net configuration.*/
#pragma pack(push, 1)
typedef struct _NETWORKCONFIG_S_ {
	_NETWORKCONFIG_S_() :
			ipAddr(), netmaskAddr(), gatewayAddr(), macAddr() {
	}
	std::string ipAddr;
	std::string netmaskAddr;
	std::string gatewayAddr;
	std::string macAddr;
} NETWORKCONFIG;
#pragma pack(pop)
/****************************local struct*************************************/

/*****************************local enum**************************************/
/*For net status.*/
enum NetworkStatus {
	upStatus = 0, downStatus, errorStatus
};
/*****************************local enum**************************************/

/******************************clsaaes****************************************/
/*Class mem vars storage net data, with allocaing memory*/
class NetConfigTrans {
public:
	NetConfigTrans();
	~NetConfigTrans();

	const DP_CHAR *getIPT() const {
		return ipT;
	}
	const DP_CHAR *getSubmaskT() const {
		return submaskT;
	}
	const DP_CHAR *getGatewayT() const {
		return gatewayT;
	}

	const DP_S32 getFlag() const {
		return flag;
	}

	bool setIPT(const DP_CHAR *ip) {
		if (strlen(ip) > 18) {
			return false;
		}
		memset(ipT, 0, 18);
		memcpy(ipT, ip, strlen(ip));
		flag += 1;
		return true;
	}
	bool setSubmaskT(const DP_CHAR *submask) {
		if (strlen(submask) > 18) {
			return false;
		}
		memset(submaskT, 0, 18);
		memcpy(submaskT, submask, strlen(submask));
		flag += 2;
		return true;
	}
	bool setgatewayT(const DP_CHAR *gateway) {
		if (strlen(gateway) > 18) {
			return false;
		}
		memset(gatewayT, 0, 18);
		memcpy(gatewayT, gateway, strlen(gateway));
		flag += 3;
		return true;
	}

	DP_S32 flag;
private:
	DP_CHAR *ipT;
	DP_CHAR *submaskT;
	DP_CHAR *gatewayT;

};

/*Get and set net configuration main class*/
class SetNetwork {
	static muduo::MutexLock mutex;
public:

	SetNetwork();
	SetNetwork(const SetNetwork&);
	SetNetwork &operator=(const SetNetwork &setNet);
	~SetNetwork();

	const NETWORKCONFIG& getNetConfStruct() const {
		return m_netWorkConfig;
	}

	inline const DP_U32 ip2U32() const {
		return inet_addr(m_netWorkConfig.ipAddr.c_str());
	}

	inline const void mac2U8(DP_U8 *macAddr) const {
		strcpy((DP_CHAR*) macAddr,
				SetNetwork::castMacToChar13((DP_CHAR*) macAddr,
						m_netWorkConfig.macAddr));
	}

	const DP_CHAR* getIfname() const {
		return IFNAME;
	}
	const NetworkStatus &getNetStatus() const {
		return networkStatus;
	}

	const bool &getInitSet() const {
		return initSet;
	}
	void setNetConfStruct(const NETWORKCONFIG& netConfig) {
		memset(&m_netWorkConfig, 0, sizeof(NETWORKCONFIG));
		memcpy(&m_netWorkConfig, &netConfig, sizeof(NETWORKCONFIG));
//		m_netWorkConfig = netConfig;
	}

	void setIfname(const DP_CHAR* ifname) {
		memset(IFNAME, 0, 8);
		memcpy(IFNAME, ifname, strlen(ifname));
	}
	void setNetStatus(const NetworkStatus &netConfig) {
		networkStatus = netConfig;
	}
	void setInitSet(bool set) {
		initSet = set;
	}

	bool getNetworkConfig();
	bool setNetworkConfig(const DP_CHAR *ipaddr, const DP_CHAR *subnet,
			const DP_CHAR *gateway, const DP_CHAR *macaddr,
			const DP_CHAR *iniFile);
	bool upDownNetwork(NetworkStatus networkStatus);

	static DP_CHAR* castMacToChar13(DP_CHAR* macDest, std::string macaddr);
private:
	NETWORKCONFIG m_netWorkConfig;
	DP_CHAR* IFNAME;
	NetworkStatus networkStatus;
	bool initSet;
//	pthread_mutex_t mutex;

	static std::string ByteToHexString(const void *pData,
			int len/* , const string &split = "" */);
	bool setNet(DP_S32 mac, const DP_CHAR *macaddr);
	bool setNet(const DP_CHAR *ipaddr, const DP_CHAR *subnet,
			const DP_CHAR *gateway);
	DP_S32 stringToHex(const std::string &strNum);
	DP_S32 charToHex(const char cNum);
	bool cSplitString(const std::string strKey, const std::string strChar,
			std::vector<std::string> &vMac);
};

class CheckNetConfig {
public:
	CheckNetConfig() {
	}
	~CheckNetConfig() {
	}
	static CheckNetConfig &GetInstance();
	bool checkSubnet(const DP_CHAR *ipaddr, const DP_CHAR *subnet);
	bool checkGateway(const DP_CHAR *ipaddr, const DP_CHAR *subnet,
			const DP_CHAR *gateway);
	bool checkMAC(DP_CHAR* mac);
	bool checkIP(const DP_CHAR* ipaddr, const DP_S32 subnetFlag);

private:
};

/******************************clsaaes****************************************/

#endif /* SRC_SETNETWORK_H_ */
