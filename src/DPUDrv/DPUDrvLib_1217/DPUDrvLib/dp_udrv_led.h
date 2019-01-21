#ifndef DP_UDRV_LED_H_
#define DP_UDRV_LED_H_

#include "dp_udrv_type.h"

#define DP_UDRV_LED_IDX0	(1<<0)
#define DP_UDRV_LED_IDX1	(1<<1)
#define DP_UDRV_LED_IDX2	(1<<2)
#define DP_UDRV_LED_IDX3	(1<<3)
#define DP_UDRV_LED_IDX4	(1<<4)
#define DP_UDRV_LED_IDX5	(1<<5)
#define DP_UDRV_LED_ALL		(0xff)


DP_S32 DP_UDRV_LED_Init();
DP_S32 DP_UDRV_LED_DeInit();
//return the max num of led,or return error code
DP_S32 DP_UDRV_LED_GetMaxNum();
//led is one bit for one led,1 is on,0 is off
DP_S32 DP_UDRV_LED_SetValue(DP_U16 led,DP_U16 value);
DP_S32 DP_UDRV_LED_GetValue(DP_U16 led,DP_U16* pValue);


#endif

