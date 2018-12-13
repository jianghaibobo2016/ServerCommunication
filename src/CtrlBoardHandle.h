/*
 * CtrlBoardHandle.h
 *
 *  Created on: Dec 11, 2018
 *      Author: jhb
 */

#ifndef SRC_CTRLBOARDHANDLE_H_
#define SRC_CTRLBOARDHANDLE_H_

#include <muduo/net/EventLoop.h>
//#include <muduo/base/BoundedBlockingQueue.h>

#include "CircleBoundedQueue.h"
#include <fcntl.h>

#include "dp_udrv_uart.h"
#include "dp_udrv_type.h"
#include "dp_udrv_regctrl.h"
#include "dp_udrv_gpio.h"
#include "dp_udrv_uart.h"
#include "dp_udrv_common.h"
#include "dp_udrv_ctrlboard.h"
#include "dp_udrv_board.h"

#define MOUSE_PATH "/dev/input/event0"
//unsigned char irRcvCode[1024];
//unsigned int irRcvLen = 0;
typedef struct _IRRecv_S {
	unsigned char irRcvCode[1024] = { 0 };
	unsigned int irRcvLen = 0;
} IRRecv_S;

using namespace muduo;
using namespace muduo::net;

class CtrlBoardHandle {
public:
	CtrlBoardHandle(EventLoop *loop);
	~CtrlBoardHandle();

	void startRunning();

	void startIRLearn();
	void StopIRLearn();
	void StartIRSendSTD();
	void StartIRSendPlusWidth();
	void StartIRRcv();
	void StopIRRcv();
	void OpenRS232Port0AndSend();
	void OpenRS232Port1AndSend();
	void OpenRS485Rcv();
	void RS485Send();
	void RS232Test();
	void IOTest();

private:
	EventLoop *_loop;
	static CircleBoundedQueue<IRRecv_S> _queue;
	void run();

	static int IrLearnRcvCallback(DP_U8 *rcvData, DP_U32 rcvDataLen);
	static int RS232Port0RcvCallBack(DP_U8 *rcvData, DP_U32 rcvDataLen);
	static int RS232Port1RcvCallBack(DP_U8 *rcvData, DP_U32 rcvDataLen);
	static int RS485RcvCallBack(DP_U8 *rcvData, DP_U32 rcvDataLen);

};

#endif /* SRC_CTRLBOARDHANDLE_H_ */
