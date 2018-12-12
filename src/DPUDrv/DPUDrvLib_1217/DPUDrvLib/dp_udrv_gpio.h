#ifndef DP_UDRV_GPIO_H_
#define DP_UDRV_GPIO_H_

#include "dp_udrv_type.h"
//Thread Safety

DP_S32 DP_UDRV_GPIO_Init(void);
DP_S32 DP_UDRV_GPIO_DeInit(void);

//value : 1 is set to output Mode ,0 is set to Input Mode
DP_S32 DP_UDRV_GPIO_SetMode(DP_U16 port,DP_U16 pin,DP_U32 mode);
DP_S32 DP_UDRV_GPIO_GetMode(DP_U16 port,DP_U16 pin,DP_U32* pMode);

//value : 1 is set to Hight ,0 is set to Low
DP_S32 DP_UDRV_GPIO_SetValue(DP_U16 port,DP_U16 pin,DP_U8 value);
DP_S32 DP_UDRV_GPIO_GetValue(DP_U16 port,DP_U16 pin,DP_U8* pValue);

#endif

