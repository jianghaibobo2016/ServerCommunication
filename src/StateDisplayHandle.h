/*
 * StateDisplayHandle.h
 *
 *  Created on: 2019年1月4日
 *      Author: queen
 */

#ifndef STATEDISPLAYHANDLE_H_
#define STATEDISPLAYHANDLE_H_

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sstream>
#include "DeviceDefine.h"
#include "dp_sd_type.h"
#include "dp_udrv_common.h"
#include "dp_udrv_regctrl.h"
#include "dp_udrv_lcd.h"

#define VERSION "V0.0.0.1"

#define DP_REG_HI3536_MAC_BASE (0x11020000)
#define DP_REG_HI3536_MAC_TXFRAMECOUNT(n) (DP_REG_HI3536_MAC_BASE+0x0118+(n)*0x4000)
#define DP_REG_HI3536_MAC_RXFRAMECOUNT(n) (DP_REG_HI3536_MAC_BASE+0x0180+(n)*0x4000)

#define DP_UDRV_NET_BOND_PATH "/proc/net/bonding/bond0"

typedef struct {

	DP_S32 s32ActiveMacId;
	DP_S32 s32RxStatus;
	DP_S32 s32TxStatus;
} DP_UDRV_NET_INFO_ATTR_S;

typedef struct _DP_RUNNING_INFO_ATTR_S {
	DP_DEV_TYPE_E eDevtype;

	DP_CHAR u8IP[16];

	DP_DEV_WORK_MODE_E eDevWorkMode;
	DP_DEV_RUNNING_STATE_E eDevRunningState;
	DP_DEV_WORK_STATE_E eDevWorkState;
} DP_RUNNING_INFO_ATTR_S;

class StateDisplayHandle {
public:
	StateDisplayHandle();
	virtual ~StateDisplayHandle();

	static DP_S32 DP_Running_GetStatusInfo(
			DP_RUNNING_INFO_ATTR_S *pstRunningInfo);
	static DP_S32 DP_UDRV_NET_GetStatusInfo(
			DP_UDRV_NET_INFO_ATTR_S *pstNetInfo);

	static DP_S32 DP_LCD_Fresh(DP_RUNNING_INFO_ATTR_S *pstRunningInfo,
			DP_UDRV_NET_INFO_ATTR_S *pstNetInfo);
};

#endif /* STATEDISPLAYHANDLE_H_ */
