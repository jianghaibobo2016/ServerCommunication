#ifndef DP_UDRV_REGCTRL_H_
#define	DP_UDRV_REGCTRL_H_

#include "dp_udrv_type.h"

//Thread Safety
DP_S32 DP_UDRV_REG_Init(void);
DP_S32 DP_UDRV_REG_DeInit(void);

DP_S32 DP_UDRV_REG_ReadIO32(DP_U32 regAddr,DP_U32* pRegVal);
DP_S32 DP_UDRV_REG_WriteIO32(DP_U32 regAddr,DP_U32 regVal);


#endif

