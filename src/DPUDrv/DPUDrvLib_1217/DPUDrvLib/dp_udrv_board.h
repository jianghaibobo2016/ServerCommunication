#ifndef DP_UDRV_BOARD_H_
#define DP_UDRV_BOARD_H_

#include "dp_udrv_type.h"

#define DP_UDRV_BOARD_TYPE_4K_INOUT		1

#define DP_UDRV_BOARD_ERROR_MODULE_COMMON		0
#define DP_UDRV_BOARD_ERROR_MODULE_TEMP_EXT		(1 <<8)
#define DP_UDRV_BOARD_ERROR_MODULE_HDMII		(2<<8)
#define DP_UDRV_BOARD_ERROR_MODULE_HDMIO		(3<<8)
#define DP_UDRV_BOARD_ERROR_MODULE_LCD			(4<<8)
#define DP_UDRV_BOARD_ERROR_MODULE_LED			(5<<8)

#define DP_UDRV_BOARD_ERROR_MODULE_MASK			(0xff00)

DP_S32 DP_UDRV_BOARD_Init();

DP_S32 DP_UDRV_BOARD_DeInit(); 
DP_S32 DP_UDRV_BOARD_SetupPinMuxer();

DP_S32 DP_UDRV_BOARD_GetFWVersion(DP_U32 *pVersion); 

DP_S32 DP_UDRV_BOARD_GetBoardType(DP_U32 *pBoardType);

#endif

