/*
 * LightDisplay.cpp
 *
 *  Created on: Jan 9, 2019
 *      Author: jhb
 */
#include <muduo/base/Singleton.h>
#include "dp_sd_type.h"
#include "dp_udrv_common.h"
#include "dp_udrv_led.h"
#include "dp_udrv_lcd.h"
#include "StateDisplayHandle.h"
#include "LightDisplay.h"
#include "NodeInfo.h"

#define DP_REG_HI3536_MAC_BASE (0x11020000)
#define DP_REG_HI3536_MAC_TXFRAMECOUNT(n) (DP_REG_HI3536_MAC_BASE+0x0118+(n)*0x4000)
#define DP_REG_HI3536_MAC_RXFRAMECOUNT(n) (DP_REG_HI3536_MAC_BASE+0x0180+(n)*0x4000)

#define DP_UDRV_NET_BOND_PATH "/proc/net/bonding/bond0"

static FILE *fd = NULL;
static void DP_UDRV_NET_Init() {
	fd = fopen(DP_UDRV_NET_BOND_PATH, "r");
}

static DP_S32 DP_UDRV_NET_GetActiveMacId(DP_S32 *ps32MacIndex) {
	assert(ps32MacIndex);

	/* ���ڳ�������ʱ��һ�Σ��Ҳ������ͷţ�ֱ���������˳� */
//	FILE *fd = NULL;
//	fd = fopen(DP_UDRV_NET_BOND_PATH, "r");
	if (NULL == fd) {
		printf("[Err] fopen failed with %s\n", strerror(errno));
		return DP_UDRV_COMMON_ERROR;
	} else {
		fseek(fd, 0, SEEK_SET);
		//printf("[Dbg] fopen %s successfully\n", DP_UDRV_NET_BOND_PATH);
		DP_CHAR s8TmpBuf[1024] = { 0 };
		fread(s8TmpBuf, sizeof(s8TmpBuf), 1, fd);
//		fclose(fd);
//		fd = NULL;
		const DP_CHAR *ps8MacStr = "Currently Active Slave: eth";
		size_t MacStrLen = strlen(ps8MacStr);
		DP_CHAR *ps8Ptr = strstr(s8TmpBuf, ps8MacStr);
		if (ps8Ptr != NULL) {
			if (ps8Ptr[MacStrLen] == '0') {
				*ps32MacIndex = 0;
			} else if (ps8Ptr[MacStrLen] == '1') {
				*ps32MacIndex = 1;
			} else {
				printf("[Err] Not Find Mac/Net Info\n");
				return DP_UDRV_COMMON_ERROR;
			}
		} else {
			return DP_UDRV_COMMON_ERROR;
		}
	}
	return DP_UDRV_SUCCESS;
}

static DP_S32 DP_UDRV_NET_GetMacTxRxVal(const DP_S32 s32MacIndex,
		DP_S32 *ps32TxVal, DP_S32 *ps32RxVal) {
	assert(ps32TxVal);
	assert(ps32RxVal);

	DP_S32 s32Ret = DP_UDRV_SUCCESS;
	DP_U32 u32RegVal = 0;
	DP_U32 u32RegAddr = 0;

	u32RegAddr = DP_REG_HI3536_MAC_TXFRAMECOUNT(s32MacIndex);
	s32Ret = DP_UDRV_REG_ReadIO32(u32RegAddr, &u32RegVal);
	if (s32Ret != DP_UDRV_SUCCESS) {
		printf("[Err] DP_UDRV_REG_ReadIO32 Mac%d TX 0x%x err with %d\n",
				s32MacIndex, u32RegAddr, s32Ret);
		return DP_UDRV_COMMON_ERROR;
	} else {
		//printf("[Dbg] DP_UDRV_REG_ReadIO32 Mac%d TX 0x%x: %u\n", s32MacIndex, u32RegAddr, u32RegVal);
		*ps32TxVal = u32RegVal;
	}

	u32RegAddr = DP_REG_HI3536_MAC_RXFRAMECOUNT(s32MacIndex);
	s32Ret = DP_UDRV_REG_ReadIO32(u32RegAddr, &u32RegVal);
	if (s32Ret != DP_UDRV_SUCCESS) {
		printf("[Err] DP_UDRV_REG_ReadIO32 Mac%d RX 0x%x err with %d\n",
				s32MacIndex, u32RegAddr, s32Ret);
		return DP_UDRV_COMMON_ERROR;
	} else {
		//printf("[Dbg] DP_UDRV_REG_ReadIO32 Mac%d RX 0x%x: %u\n", s32MacIndex, u32RegAddr, u32RegVal);
		*ps32RxVal = u32RegVal;
	}

	return DP_UDRV_SUCCESS;
}

DP_S32 lightDisplay() {

	DP_S32 s32Ret = DP_UDRV_SUCCESS;
	DP_S32 s32MacIndex = -1;
	DP_S32 s32CurTxVal = 0, s32CurRxVal = 0;
	DP_S32 s32LastTxVal = 0, s32LastRxVal = 0;
	DP_BOOL bTxLed = DP_FALSE, bRxLed = DP_FALSE;

	DP_UDRV_LED_Init();
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0); //tx led
	DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0); //rx led
	DP_UDRV_NET_Init();
	while (1) {
		s32Ret = DP_UDRV_NET_GetActiveMacId(&s32MacIndex);
		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_NET_GetActiveMacId Err: %d\n", s32Ret);
		} else {

			DP_UDRV_NET_GetMacTxRxVal(s32MacIndex, &s32CurTxVal, &s32CurRxVal);
		}

		if (s32LastTxVal != s32CurTxVal) {
			s32LastTxVal = s32CurTxVal;
			bTxLed = DP_TRUE;
		} else {
			bTxLed = DP_FALSE;
		}

		if (s32LastRxVal != s32CurRxVal) {
			s32LastRxVal = s32CurRxVal;
			bRxLed = DP_TRUE;
		} else {
			bRxLed = DP_FALSE;
		}

		if (bTxLed == DP_TRUE && bRxLed == DP_TRUE) {

			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 1); //tx led
			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 1); //rx led
			usleep(100 * 1000);
			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0); //tx led
			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0); //rx led
		} else if (bTxLed == DP_TRUE && bRxLed == DP_FALSE) {

			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 1); //tx led
			usleep(100 * 1000);
			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0); //tx led
		} else if (bTxLed == DP_FALSE && bRxLed == DP_TRUE) {

			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 1); //rx led
			usleep(100 * 1000);
			DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0); //rx led
		}

		usleep(50 * 1000);
	}

	fclose(fd);
	fd = NULL;

	DP_UDRV_LED_DeInit();
	return 0;

#if 0
	DP_S32 s32Ret = DP_UDRV_SUCCESS;
	DP_RUNNING_INFO_ATTR_S stRunningInfo;
	DP_UDRV_NET_INFO_ATTR_S stNetInfo;
	DP_RUNNING_INFO_ATTR_S stRunningInfoLast;
	DP_UDRV_NET_INFO_ATTR_S stNetInfoLast;

#if (InputDevice)
	stRunningInfo.eDevtype = DP_DEV_TYPE_IDMS4K_INPUT;
	stRunningInfoLast.eDevtype = DP_DEV_TYPE_IDMS4K_INPUT;
#elif (OutputDevice)
	stRunningInfo.eDevtype = DP_DEV_TYPE_IDMS4K_OUTPUT;
	stRunningInfoLast.eDevtype = DP_DEV_TYPE_IDMS4K_INPUT;
#endif

	memcpy(stRunningInfo.u8IP,
			muduo::Singleton<NodeInfo>::instance().getNetInfo().getNetConfStruct().ipAddr.c_str(),
			strlen(
					muduo::Singleton<NodeInfo>::instance().getNetInfo().getNetConfStruct().ipAddr.c_str()));
	memcpy(stRunningInfoLast.u8IP,
			muduo::Singleton<NodeInfo>::instance().getNetInfo().getNetConfStruct().ipAddr.c_str(),
			strlen(
					muduo::Singleton<NodeInfo>::instance().getNetInfo().getNetConfStruct().ipAddr.c_str()));

//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

//Init
//LED
	s32Ret = DP_UDRV_LED_Init();
	if (s32Ret != DP_UDRV_SUCCESS) {
		printf("[Err] DP_UDRV_LED_Init failed with: %d\n", s32Ret);
	} else {

#if (OutputDevice)
		s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX0, 0);		//close HDMI LED
#endif

		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
		}
		s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0);		//Tx LED
		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
		}
		s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0);		//Rx LED
		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
		}
		s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 0);		//Run LED
		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
		}
	}

	//LCD
	s32Ret = DP_UDRV_LCD_Init();
	if (s32Ret != DP_UDRV_SUCCESS) {
		printf("[Err] DP_UDRV_LCD_Init failed with: %d\n", s32Ret);
	} else {
		s32Ret = StateDisplayHandle::DP_Running_GetStatusInfo(&stRunningInfo);
		if (s32Ret != DP_SUCCESS) {
			printf("[Err] DP_Running_GetStatusInfo Err: %d\n", s32Ret);
		}

		s32Ret = StateDisplayHandle::DP_UDRV_NET_GetStatusInfo(&stNetInfo);
		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_NET_GetStatusInfo Err: %d\n", s32Ret);
		}

		s32Ret = StateDisplayHandle::DP_LCD_Fresh(&stRunningInfo, &stNetInfo);
		if (s32Ret != DP_SUCCESS) {
			printf("[Err] DP_LCD_Fresh Err: %d\n", s32Ret);
		}
	}

	//Runing
	while (1) {
		s32Ret = StateDisplayHandle::DP_Running_GetStatusInfo(&stRunningInfo);
		if (s32Ret != DP_SUCCESS) {
			printf("[Err] DP_Running_GetStatusInfo Err: %d\n", s32Ret);
			continue;
		}

		s32Ret = StateDisplayHandle::DP_UDRV_NET_GetStatusInfo(&stNetInfo);
		if (s32Ret != DP_UDRV_SUCCESS) {
			printf("[Err] DP_UDRV_NET_GetStatusInfo Err: %d\n", s32Ret);
			continue;
		}

		//LED
		if (stRunningInfo.eDevWorkState != DP_DEV_WORK_STATE_FREE) {
			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 1);		//Run LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}
		} else {
			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX3, 0);		//Run LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}
		}
//		printf("[Dbg] s32ActiveMacId %d s32TxStatus %d s32RxStatus %d\n",
//				stNetInfo.s32ActiveMacId, stNetInfo.s32TxStatus,
//				stNetInfo.s32RxStatus);
		if (stNetInfo.s32TxStatus == 1) {
			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 1); //Tx LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}
			printf("send light  11111\n");
			usleep(100000);

			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 0); //Tx LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}
		}

		if (stNetInfo.s32RxStatus == 1) {
			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 1); //Rx LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}
			printf("recv light  11111\n");
			usleep(100000);

			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX2, 0); //Rx LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}
		}

		//LCD
		if ((stRunningInfo.eDevtype != stRunningInfoLast.eDevtype)
				|| (stNetInfo.s32ActiveMacId != stNetInfoLast.s32ActiveMacId)
				|| (stRunningInfo.eDevWorkMode != stRunningInfoLast.eDevWorkMode)
				|| (stRunningInfo.eDevRunningState
						!= stRunningInfoLast.eDevRunningState)
				|| (stRunningInfo.eDevWorkState
						!= stRunningInfoLast.eDevWorkState)) {

			if (stRunningInfo.eDevtype != stRunningInfoLast.eDevtype) {
				printf(
						"stRunningInfo.eDevtype != stRunningInfoLast.eDevtype\n");
				printf("stRunningInfo.eDevtype %d\n", stRunningInfo.eDevtype);
				printf("stRunningInfoLast.eDevtype %d\n",
						stRunningInfoLast.eDevtype);
			}
			if (stNetInfo.s32ActiveMacId != stNetInfoLast.s32ActiveMacId) {
				printf(
						"stNetInfo.s32ActiveMacId != stNetInfoLast.s32ActiveMacId\n");
			}
			if (stRunningInfo.eDevWorkMode != stRunningInfoLast.eDevWorkMode) {
				printf(
						"stRunningInfo.eDevWorkMode != stRunningInfoLast.eDevWorkMode\n");
			}
			if (stRunningInfo.eDevRunningState
					!= stRunningInfoLast.eDevRunningState) {
				printf(
						"stRunningInfo.eDevRunningState!= stRunningInfoLast.eDevRunningState\n");
			}
			if (stRunningInfo.eDevWorkState
					!= stRunningInfoLast.eDevWorkState) {
				printf(
						"stRunningInfo.eDevWorkState!= stRunningInfoLast.eDevWorkState\n");
			}

			s32Ret = StateDisplayHandle::DP_LCD_Fresh(&stRunningInfo,
					&stNetInfo);
			if (s32Ret != DP_SUCCESS) {
				printf("[Err] DP_LCD_Fresh Err: %d\n", s32Ret);
			}

//			stRunningInfoLast.eDevtype = stRunningInfo.eDevtype;
//			strcpy(stRunningInfoLast.u8IP, stRunningInfo.u8IP);
			stRunningInfoLast.eDevWorkMode = stRunningInfo.eDevWorkMode;
			stRunningInfoLast.eDevRunningState = stRunningInfo.eDevRunningState;
			stRunningInfoLast.eDevWorkState = stRunningInfo.eDevWorkState;
			memcpy(&stNetInfoLast, &stNetInfo, sizeof(DP_UDRV_NET_INFO_ATTR_S));
		}

		usleep(100000);
	}

	return 0;
#endif
}

