/*
 * DevStaticConfigure.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_DEVSTATICCONFIGURE_H_
#define SRC_DEVSTATICCONFIGURE_H_

#include "interactivepro.h"
#include "dp_sd_type.h"
//
#define OutputDevice 1
#define InputDevice  0
//
//#define OutputDevice 0
//#define InputDevice  1

#if (InputDevice)
static const DP_CHAR *SoftVersion = "V0.0.0.1";
static const eDeviceType g_DevType = Type_DeviceInput;
static const DP_CHAR *LogFileName = "/root/APPDIR/Server/In/InputNodeServer";
static const DP_CHAR *Welcome = "DSPPA IDMS4K IN";
static const DP_BOOL RecoverTask = DP_TRUE;
#endif

#if (OutputDevice)
static const DP_CHAR *SoftVersion = "V0.0.0.11";
static const eDeviceType g_DevType = Type_DeviceOutput;
static const DP_CHAR *LogFileName = "/root/APPDIR/Server/Out/OutNodeServer";
static const DP_CHAR *Welcome = "DSPPA IDMS4K OUT";
static const DP_BOOL RecoverTask = DP_TRUE;
#endif

static const DP_CHAR *Working = "WORKING";
static const DP_CHAR *Idle = "IDLE";

static const DP_CHAR *MultiCastAddrRECV = "234.0.0.2";
static const DP_CHAR *MultiCastAddrSEND = "234.0.0.2";
static const DP_U32 MultiCastRecvPort = 15011;
static const DP_U32 MultiCastSendPort = 15012;
static const DP_U32 UDPBufferSizeMax = 1400;

static const DP_CHAR* g_ServerProgVersion = "V00.01";

static const DP_CHAR* g_DevIDPR = "0066";

static const DP_CHAR* DevModelType = " ";

static const DP_CHAR *INIFILE = "./NET_CONF.ini";
//static const DP_CHAR *g_IFNAMEDEV = "eth0";
static const DP_CHAR *g_IFNAMEDEV = "bond0";
//static const DP_CHAR *IFNAMEDEV = "ens33";

static const DP_U32 g_LogFileMaxSize = 4 * 1024 * 1024;

const int BUFFER_SIZE_PIPESOCKET = 1024 * 1024;

static DP_U8 g_NeedReply = 0x01;
static DP_U8 g_NoNeedReply = 0x00;

static const DP_CHAR *UpShellRespondTag_1 = "0F1F2F";
static const DP_CHAR *UpShellRespondTag_3_Success = "00";
static const DP_CHAR *UpShellRespondTag_4_Fail = "01";
static const DP_CHAR *UpgradeShell = "update.sh";

#endif /* SRC_DEVSTATICCONFIGURE_H_ */
