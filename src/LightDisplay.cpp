/*
 * LightDisplay.cpp
 *
 *  Created on: Jan 9, 2019
 *      Author: jhb
 */
#include "dp_sd_type.h"
#include "dp_udrv_common.h"
#include "dp_udrv_led.h"
#include "dp_udrv_lcd.h"
#include "StateDisplayHandle.h"
#include "LightDisplay.h"

DP_S32 lightDisplay() {
	DP_S32 s32Ret = DP_UDRV_SUCCESS;
	DP_RUNNING_INFO_ATTR_S stRunningInfo;
	DP_UDRV_NET_INFO_ATTR_S stNetInfo;
	DP_RUNNING_INFO_ATTR_S stRunningInfoLast;
	DP_UDRV_NET_INFO_ATTR_S stNetInfoLast;

//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

//Init
//LED
	s32Ret = DP_UDRV_LED_Init();
	if (s32Ret != DP_UDRV_SUCCESS) {
		printf("[Err] DP_UDRV_LED_Init failed with: %d\n", s32Ret);
	} else {
		s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX0, 0);		//close HDMI LED
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
		//	        printf("[Dbg] s32ActiveMacId %d s32TxStatus %d s32RxStatus %d\n", stNetInfo.s32ActiveMacId, stNetInfo.s32TxStatus, stNetInfo.s32RxStatus);
		if (stNetInfo.s32TxStatus == 1) {
			s32Ret = DP_UDRV_LED_SetValue(DP_UDRV_LED_IDX1, 1); //Tx LED
			if (s32Ret != DP_UDRV_SUCCESS) {
				printf("[Err] DP_UDRV_LED_SetValue failed with: %d\n", s32Ret);
			}

			usleep(50000);

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

			usleep(50000);

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

			s32Ret = StateDisplayHandle::DP_LCD_Fresh(&stRunningInfo,
					&stNetInfo);
			if (s32Ret != DP_SUCCESS) {
				printf("[Err] DP_LCD_Fresh Err: %d\n", s32Ret);
			}

			stRunningInfoLast.eDevtype = stRunningInfo.eDevtype;
			strcpy(stRunningInfoLast.u8IP, stRunningInfo.u8IP);
			stRunningInfoLast.eDevWorkMode = stRunningInfo.eDevWorkMode;
			stRunningInfoLast.eDevRunningState = stRunningInfo.eDevRunningState;
			stRunningInfoLast.eDevWorkState = stRunningInfo.eDevWorkState;
			memcpy(&stNetInfoLast, &stNetInfo, sizeof(DP_UDRV_NET_INFO_ATTR_S));
		}

		usleep(10000);
	}

	return 0;
}

