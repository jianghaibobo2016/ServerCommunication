/*
 * StateDisplayHandle.cpp
 *
 *  Created on: 2019年1月4日
 *      Author: queen
 */

#include <StateDisplayHandle.h>
#include "Public.h"

using namespace std;

StateDisplayHandle::StateDisplayHandle() {
	// TODO Auto-generated constructor stub

}

StateDisplayHandle::~StateDisplayHandle() {
	// TODO Auto-generated destructor stub
}

//string int2str(const int &int_temp) {
//	string string_temp;
//	stringstream stream;
//	stream << int_temp;
//	stream >> string_temp;
//	return string_temp;
//}

DP_S32 StateDisplayHandle::DP_Running_GetStatusInfo(
		DP_RUNNING_INFO_ATTR_S *pstRunningInfo) {
	assert(pstRunningInfo);

	pstRunningInfo->eDevtype = DP_DEV_TYPE_IDMS4K_OUTPUT;
	strcpy(pstRunningInfo->u8IP, "172.16.100.77");
	pstRunningInfo->eDevWorkMode = DP_DEV_WORK_MODE_MONTAGE;
	pstRunningInfo->eDevRunningState = DP_DEV_RUNNING_STATE_NORMAL;
	pstRunningInfo->eDevWorkState = DP_DEV_WORK_STATE_FREE;

	return DP_SUCCESS;
}

DP_S32 StateDisplayHandle::DP_UDRV_NET_GetStatusInfo(
		DP_UDRV_NET_INFO_ATTR_S *pstNetInfo) {
	assert(pstNetInfo);

	DP_S32 s32Ret = DP_UDRV_SUCCESS;
	DP_UDRV_NET_INFO_ATTR_S *pstInfo = pstNetInfo;
	DP_UDRV_NET_INFO_ATTR_S stInfo;
	DP_U32 u32RegVal = 0;
	DP_U32 u32RegAddr = 0;
	DP_S32 s32MacIndex = 0;
	DP_S32 s32MaxCount, i;
	DP_U32 u32TxFrameCountBegin = 0;
	DP_U32 u32RxFrameCountBegin = 0;
	DP_U32 u32TxFrameCountEnd = 0;
	DP_U32 u32RxFrameCountEnd = 0;

	i = 0;
	s32MaxCount = 2;
	memset(&stInfo, -1, sizeof(DP_UDRV_NET_INFO_ATTR_S));

	FILE *fd = fopen(DP_UDRV_NET_BOND_PATH, "r");
	if (NULL == fd) {
		printf("[Err] fopen failed with %s\n", strerror(errno));
		return DP_UDRV_COMMON_ERROR;
	} else {
		DP_CHAR s8TmpBuf[1024] = { 0 };
		fread(s8TmpBuf, sizeof(s8TmpBuf), 1, fd);
		fclose(fd);
		fd = NULL;
		const DP_CHAR *ps8MacStr = "Currently Active Slave: eth";
		size_t MacStrLen = strlen(ps8MacStr);
		DP_CHAR *ps8Ptr = strstr(s8TmpBuf, ps8MacStr);
		if (ps8Ptr != NULL) {
			if (ps8Ptr[MacStrLen] == '0') {
				stInfo.s32ActiveMacId = 0;
			} else if (ps8Ptr[MacStrLen] == '1') {
				stInfo.s32ActiveMacId = 1;
			} else {
				printf("[Err] Not Find Mac/Net Info\n");
				return DP_UDRV_COMMON_ERROR;
			}
		} else {
			return DP_UDRV_COMMON_ERROR;
		}
	}

	for (s32MacIndex = 0; s32MacIndex < 2; s32MacIndex++) {
		for (i = 0; i < s32MaxCount; i++) {
			u32RegAddr = DP_REG_HI3536_MAC_TXFRAMECOUNT(s32MacIndex);
			s32Ret = DP_UDRV_REG_ReadIO32(u32RegAddr, &u32RegVal);
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf(
						"[Err] %d DP_UDRV_REG_ReadIO32 Mac%d TX 0x%x err with %d\n",
						i, s32MacIndex, u32RegAddr, s32Ret);
			} else {
				//printf("[Dbg] %d DP_UDRV_REG_ReadIO32 Mac%d TX 0x%x: %u\n", i, s32MacIndex, u32RegAddr, u32RegVal);
				if (0 == i) {
					u32TxFrameCountBegin = u32RegVal;
				}
				if ((s32MaxCount - 1) == i) {
					u32TxFrameCountEnd = u32RegVal;
				}
			}

			usleep(10);

			u32RegAddr = DP_REG_HI3536_MAC_RXFRAMECOUNT(s32MacIndex);
			s32Ret = DP_UDRV_REG_ReadIO32(u32RegAddr, &u32RegVal);
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf(
						"[Err] %d DP_UDRV_REG_ReadIO32 Mac%d RX 0x%x err with %d\n",
						i, s32MacIndex, u32RegAddr, s32Ret);
			} else {
				//printf("[Dbg] %d DP_UDRV_REG_ReadIO32 Mac%d RX 0x%x: %u\n", i, s32MacIndex, u32RegAddr, u32RegVal);
				if (0 == i) {
					u32RxFrameCountBegin = u32RegVal;
				}
				if ((s32MaxCount - 1) == i) {
					u32RxFrameCountEnd = u32RegVal;
				}
			}
		}

		if ((u32TxFrameCountBegin > 0) && (u32TxFrameCountEnd > 0)
				&& (u32TxFrameCountEnd - u32TxFrameCountBegin) > 0) {
			stInfo.s32TxStatus = 1;
		} else {
			stInfo.s32TxStatus = -1;
		}

		if ((u32RxFrameCountBegin > 0) && (u32RxFrameCountEnd > 0)
				&& (u32RxFrameCountEnd - u32RxFrameCountBegin) > 0) {
			stInfo.s32RxStatus = 1;
		} else {
			stInfo.s32RxStatus = -1;
		}

		if ((1 == stInfo.s32TxStatus) || (1 == stInfo.s32RxStatus)) {
			break;
		}
	}

	//printf("stInfo.s32ActiveMacId %d Tx %d Rx %d\n", stInfo.s32ActiveMacId, stInfo.s32TxStatus, stInfo.s32RxStatus);
	memcpy(pstInfo, &stInfo, sizeof(DP_UDRV_NET_INFO_ATTR_S));
	return DP_UDRV_SUCCESS;
}

DP_S32 StateDisplayHandle::DP_LCD_Fresh(DP_RUNNING_INFO_ATTR_S *pstRunningInfo,
		DP_UDRV_NET_INFO_ATTR_S *pstNetInfo) {
	assert(pstRunningInfo);
	assert(pstNetInfo);

	DP_S32 s32Ret = DP_UDRV_SUCCESS;

	s32Ret = DP_UDRV_LCD_CleanScreen();
	if (s32Ret != DP_UDRV_SUCCESS) {
		printf("[Err] DP_UDRV_LCD_CleanScreen failed with: %d\n", s32Ret);
	}

	if (pstRunningInfo->eDevtype == DP_DEV_TYPE_IDMS4K_INPUT) {
		DP_UDRV_LCD_ShowString(0, 0, "DSPPA IDMS4K IN",
				strlen("DSPPA IDMS4K IN"));
	} else if (pstRunningInfo->eDevtype == DP_DEV_TYPE_IDMS4K_OUTPUT) {
		DP_UDRV_LCD_ShowString(0, 0, "DSPPA IDMS4K OUT",
				strlen("DSPPA IDMS4K OUT"));
	} else {
		printf("[Err] No this eDevtype: %d\n", pstRunningInfo->eDevtype);
	}

	if (pstNetInfo->s32ActiveMacId == 0) {
		DP_UDRV_LCD_ShowString(0, 1, "A", strlen("A"));
	} else if (pstNetInfo->s32ActiveMacId == 1) {
		DP_UDRV_LCD_ShowString(0, 1, "B", strlen("B"));
	} else {
		printf("[Err] No this s32ActiveMacId: %d\n",
				pstNetInfo->s32ActiveMacId);
	}
	DP_UDRV_LCD_ShowString(10, 1, pstRunningInfo->u8IP,
			strlen(pstRunningInfo->u8IP)); // jhbnote add ETH0/1

	DP_UDRV_LCD_ShowString(0, 2, "M:", strlen("M:"));
	DP_UDRV_LCD_ShowString(16, 2,
			int2str(pstRunningInfo->eDevWorkMode - DP_DEV_WORK_MODE_MATRIX).c_str(),
			1);
	DP_UDRV_LCD_ShowString(32, 2, "R:", strlen("R:"));
	DP_UDRV_LCD_ShowString(48, 2,
			int2str(
					pstRunningInfo->eDevRunningState
							- DP_DEV_RUNNING_STATE_INITIALIZED).c_str(), 1);
	DP_UDRV_LCD_ShowString(64, 2, "W:", strlen("W:"));
	DP_UDRV_LCD_ShowString(80, 2,
			int2str(pstRunningInfo->eDevWorkState - DP_DEV_WORK_STATE_FREE).c_str(),
			1);

	DP_UDRV_LCD_ShowString(48, 3, VERSION, strlen(VERSION));

	return DP_SUCCESS;
}
