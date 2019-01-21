
#ifndef DP_UDRV_GSPI_H_
#define DP_UDRV_GSPI_H_

//GPIO SPI Driver HAL
#include "dp_udrv_type.h"

typedef enum DP_UDRV_SPI_MODE_
{
	DP_UDRV_SPI_MODE_0 = 1,//CPOL=0 CPHA=0
	DP_UDRV_SPI_MODE_1, //CPOL=0 CPHA=1
	DP_UDRV_SPI_MODE_2,//CPOL=1 CPHA=0
	DP_UDRV_SPI_MODE_3//CPOL=1 CPHA=1
}DP_UDRV_SPI_MODE_E;

#define DP_UDRV_GSPI_PORT_NUM		1

//return spi port num or common error code
DP_S32 DP_UDRV_GSPI_Init();
DP_S32 DP_UDRV_GSPI_DeInit();

//open one port and set mode and gpio pin,it should set mux before using DP_UDRV_REG_WriteIO32
//pin defined: (port << 8  )| (pin). eg: GPIO2_5 is 0x25
DP_S32 DP_UDRV_GSPI_Open(DP_U16 spiPort,DP_U32 csPin,DP_U32 clkPin,DP_U32 misoPin,DP_U32 mosiPin,DP_UDRV_SPI_MODE_E mode);

//>=0 return readLen  or return common error code
DP_S32 DP_UDRV_GSPI_ReadWriteMulti(DP_U16 spiPort,DP_U8 *writeBuf,DP_U8 readWriteLen,DP_U8* readBuf);
DP_S32 DP_UDRV_GSPI_ReadWriteOneByte(DP_U16 spiPort,DP_U8 writeByte,DP_U8* pReadByte);

#endif


