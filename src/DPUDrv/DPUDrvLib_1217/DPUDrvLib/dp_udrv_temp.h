#ifndef DP_UDRV_TEMP_H_
#define DP_UDRV_TEMP_H_

#include "dp_udrv_type.h"

#define DP_UDRV_TEMP_CHIP_TYPE_INTERNEL	0
#define DP_UDRV_TEMP_CHIP_TYPE_LM73		1

#define DP_UDRV_TEMP_CHIP_INTERNEL	0
#define DP_UDRV_TEMP_CHIP_EXTERNEL	1

DP_S32 DP_UDRV_TEMP_Init(DP_U8 chipIdx);
DP_S32 DP_UDRV_TEMP_DeInit(DP_U8 chipIdx);
DP_S32 DP_UDRV_TEMP_GetChipType(DP_U8 chipIdx,DP_U32 *pType);
DP_S32 DP_UDRV_TEMP_GetFWVersion(DP_U8 chipIdx,DP_U32 *pVersion);

//pTempValue 返回温度数据为浮点数温度值如31.xxx 摄氏度
DP_S32 DP_UDRV_TEMP_GetTemp(DP_U8 chipIdx,DP_FLOAT* pTempValue);


#endif
