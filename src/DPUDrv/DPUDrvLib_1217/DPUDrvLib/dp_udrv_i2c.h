#ifndef DP_UDRV_I2C_H_
#define DP_UDRV_I2C_H_

#include "dp_udrv_type.h"


DP_S32 DP_UDRV_I2C_Init(void);	//初始化
DP_S32 DP_UDRV_I2C_DeInit(void);	//去初始化

DP_S32 DP_UDRV_I2C_ReadMulti(DP_U8 port,DP_U16 slaveAddr,DP_U8* pRegisterAddr,DP_U8 RegisterAddrSize,DP_U8* pRegAddr,DP_U8 readLen,DP_U8 flags);		//读多个字节

DP_S32 DP_UDRV_I2C_WriteMulti(DP_U8 port,DP_U16 slaveAddr,DP_U8* pWritingBytes,DP_U32 writeLen,DP_U8 flags);		//写多个字节
DP_S32 DP_UDRV_I2C_ReadOneByte(DP_U8 port,DP_U16 slaveAddr,DP_U8 regAddr,DP_U8* regValue,DP_U8 flag);	//读单个字节
DP_S32 DP_UDRV_I2C_WriteOneByte(DP_U8 port,DP_U16 slaveAddr,DP_U8 regAddr,DP_U8 regValue,DP_U8 flag);	//写单个字节

#endif

