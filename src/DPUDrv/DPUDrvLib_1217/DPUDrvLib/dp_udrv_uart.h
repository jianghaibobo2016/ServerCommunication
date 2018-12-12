#ifndef DP_UDRV_UART_H_
#define DP_UDRV_UART_H_

#include "dp_udrv_type.h"

typedef enum _DP_UDRV_UART_PROTOCOL
{
	DP_UDRV_UART_PROTOCOL_NONE = 0,
	DP_UDRV_UART_PROTOCOL_DB,//use to communicate to ctrl board
	DP_UDRV_UART_PROTOCOL_RAW,
}DP_UDRV_UART_PROTOCOL_E;

#define DP_UDRV_UART_MAX_CMD_BUF	1024
typedef struct _dp_udrv_uart_cmd_info
{
	DP_U8 machineID;
	DP_U16 cmd;
	DP_U8 buf[DP_UDRV_UART_MAX_CMD_BUF];
	DP_U16 len;
}DP_UDRV_UART_CMDInfo_S;

typedef int (*ProtocolCMDRcvCB)(DP_UDRV_UART_CMDInfo_S *cmd,DP_U32 cmdCount,void* cbParms);

typedef struct _dp_udrv_uart_attr_s
{
	DP_S32	baudRate;
	DP_S32	dataBits;
	DP_S8	parity;
	DP_S32	stopBits;
	DP_UDRV_UART_PROTOCOL_E protocol;
	ProtocolCMDRcvCB	protocolCB;
	void*	protocolCBParms;
}DP_UDRV_UART_ATTR_S;	


typedef struct _CommonBuf
{
	unsigned char machineID;
	unsigned char *buf;
	unsigned int bufLen;
}CommonBuf_S;

typedef struct _dp_udrv_uart_cmd
{
	DP_U8 machineID;
	DP_U16 cmd;
	DP_U8 *pbuf;
	DP_U16 bufLen;
}DP_UDRV_UART_CMD_S;


DP_S32 DP_UDRV_UART_Init();
DP_S32 DP_UDRV_UART_DeInit();

DP_S32 DP_UDRV_UART_GetMaxNum();
//Note That DO NOT Block in CB,it will cause the rcv thread can not rcv.
//DO NOT Use SendData IN CALLBACK,it will cause
DP_S32 DP_UDRV_UART_Open(DP_U16 port,DP_UDRV_UART_ATTR_S attrs);
DP_S32 DP_UDRV_UART_Close(DP_U16 port);

//Ret: > 0 is datalen send.<=0 is dp common return value
DP_S32 DP_UDRV_UART_SendData(DP_S32 port,DP_U8 *sendData,DP_U32 sendDataLen);
//Ret: > 0 is datalen rcv.<=0 is dp common return value
DP_S32 DP_UDRV_UART_RecvData(DP_S32 port,DP_U8 *rcvData,DP_U32 rcvDataLen);

//
DP_S32 DP_UDRV_UART_DataSendRcvAndWaitForRes(DP_S32 port,DP_U8 mode,DP_UDRV_UART_CMD_S sendCmd,DP_U32 timeOutMs);




#endif


