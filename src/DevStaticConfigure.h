/*
 * DevStaticConfigure.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_DEVSTATICCONFIGURE_H_
#define SRC_DEVSTATICCONFIGURE_H_

#include "dp_sd_type.h"
#include "interactivepro.h"
//
#define OutputDevice 1
#define InputDevice  0
//
//#define OutputDevice 0
//#define InputDevice  1
//
#if (InputDevice)
static const eDeviceType g_DevType = Type_DeviceInput;
static const DP_CHAR *LogFileName = "/root/APPDIR/InputNodeServer";
#endif

#if (OutputDevice)
static const eDeviceType g_DevType = Type_DeviceOutput;
static const DP_CHAR *LogFileName = "/root/APPDIR/OutNodeServer";
#endif

static const DP_CHAR *MultiCastAddrRECV = "234.0.0.2";
static const DP_CHAR *MultiCastAddrSEND = "234.0.0.2";
static const DP_U32 MultiCastRecvPort = 15011;
static const DP_U32 MultiCastSendPort = 15012;
static const DP_U32 UDPBufferSizeMax = 1400;

static const DP_CHAR* g_ServerProgVersion = "V00.01";

static const DP_CHAR* g_DevIDPR = "0066";

static const DP_CHAR* DevModelType = " ";

static const DP_CHAR *INIFILE = "./NET_CONF.ini";
static const DP_CHAR *g_IFNAMEDEV = "eth0";
//static const DP_CHAR *IFNAMEDEV = "ens33";

static const DP_U32 g_LogFileMaxSize = 2 * 1024 * 1024;

static DP_U8 g_NeedReply = 0x01;
static DP_U8 g_NoNeedReply = 0x00;

#endif /* SRC_DEVSTATICCONFIGURE_H_ */
