#ifndef DP_UDRV_CTRL_BOARD_H_
#define DP_UDRV_CTRL_BOARD_H_
#include "dp_udrv_type.h"
#include <linux/input.h>

//dataBits is only support 8  
//parity : 0 is NO, 1 is Odd 2 is Even
//stopBits:0 is 0.5, 1 is 1,2 is 1.5,3 is 2
typedef struct DP_UDRV_CTRL_BOARD_UartAttr_S_
{
	DP_S32 baudRate;//38400
	DP_U8 dataBits;// only support 8 
	DP_S8 parity;//'n' or 'o' or 'e'
	DP_U8 stopBits;//1 or 2
	DP_U8 hwFlow;//now not support
}DP_UDRV_CTRL_BOARD_UartAttr_S;


typedef int (*CtrlBoardRcvCallback)(DP_U8 *rcvData,DP_U32 rcvDataLen);

DP_S32 DP_UDRV_CtrlBoard_Init();
DP_S32 DP_UDRV_CtrlBoard_DeInit();

DP_S32 DP_UDRV_CtrlBoard_GetFWVersion(DP_U32 *pVersion);

//only rcv ir code
DP_S32 DP_UDRV_CtrlBoard_StartIRRcv();
DP_S32 DP_UDRV_CtrlBoard_StopIRRcv();


DP_S32 DP_UDRV_CtrlBoard_StartIRLearn();
DP_S32 DP_UDRV_CtrlBoard_StopIRLearn();
//do not block in callback,do not call the API here which will cause dead lock
DP_S32 DP_UDRV_CtrlBoard_RegisterIRLearnCallback(CtrlBoardRcvCallback callback);
//Ret: > 0 is datalen send.<=0 is dp common return value
DP_S32 DP_UDRV_CtrlBoard_SendIRLearnBuf(DP_U8 port,DP_U8 *irData,DP_U32 irDataLen);


DP_S32 DP_UDRV_CtrlBoard_SetIOValue(DP_U8 ioIdx,DP_U8 ioValue);


DP_S32 DP_UDRV_CtrlBoard_RS232Open(DP_U8 port,DP_UDRV_CTRL_BOARD_UartAttr_S attr);
DP_S32 DP_UDRV_CtrlBoard_RS232Close(DP_U8 port);
DP_S32 DP_UDRV_CtrlBoard_RS232SendData(DP_U8 port,DP_U8 *sendBuf,DP_U32 sendBufLen);
//do not block in callback,do not call the API here which will cause dead lock
DP_S32 DP_UDRV_CtrlBoard_RegisterRS232CallBack(DP_U8 port,CtrlBoardRcvCallback callback);

DP_S32 DP_UDRV_CtrlBoard_RS485Open(DP_U8 port,DP_UDRV_CTRL_BOARD_UartAttr_S attr);
DP_S32 DP_UDRV_CtrlBoard_RS485Close(DP_U8 port);
DP_S32 DP_UDRV_CtrlBoard_RS485SendData(DP_U8 port,DP_U8 *sendBuf,DP_U32 sendBufLen);
//do not block in callback,do not call the API here which will cause dead lock
DP_S32 DP_UDRV_CtrlBoard_RegisterRS485CallBack(DP_U8 port,CtrlBoardRcvCallback callback);

//send mouse and keyboard,it did not wait for res because the event int is too short
DP_S32 DP_UDRV_CtrlBoard_SendInputEvent(struct input_event event);

DP_S32 DP_UDRV_CtrlBoard_GetCtrlBoardVersion(DP_U32 *pFWVersion);

typedef void (*ProcessCallback)(float precent);

DP_S32 DP_UDRV_CtrlBoard_Upgrade(char* Upgrade_HEXFilePath,ProcessCallback callback);
#endif

