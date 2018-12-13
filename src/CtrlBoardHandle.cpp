/*
 * CtrlBoardHandle.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: jhb
 */

#include <boost/bind.hpp>
#include "CtrlBoardHandle.h"

CircleBoundedQueue<IRRecv_S> CtrlBoardHandle::_queue(3);

CtrlBoardHandle::CtrlBoardHandle(EventLoop *loop) :
		_loop(loop) {
}
CtrlBoardHandle::~CtrlBoardHandle() {
}

int CtrlBoardHandle::IrLearnRcvCallback(DP_U8 *rcvData, DP_U32 rcvDataLen) {

	IRRecv_S recv;
	recv.irRcvLen = rcvDataLen;

//	irRcvLen = rcvDataLen;
	memcpy(recv.irRcvCode, rcvData, rcvDataLen);
	printf("rcv IR Code len %d\r\n", rcvDataLen);
	for (unsigned int i = 0; i < rcvDataLen; i++) {
		printf("0x%0.2x ", rcvData[i]);
		if ((i + 1) % 16 == 0)
			printf("\r\n");
	}
	printf("\r\n");
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	std::cout << "time start put : " << tv.tv_sec << " . " << tv.tv_usec
//			<< std::endl;
	CtrlBoardHandle::_queue.put(recv);

//	gettimeofday(&tv, NULL);
//	std::cout << "time end  put : " << tv.tv_sec << " . " << tv.tv_usec
//			<< std::endl;
	return 0;
}

int CtrlBoardHandle::RS232Port0RcvCallBack(DP_U8 *rcvData, DP_U32 rcvDataLen) {
	printf("Port0 RcvDataLen %d rcvData %s \r\n", rcvDataLen, rcvData);
	return 0;
}

int CtrlBoardHandle::RS232Port1RcvCallBack(DP_U8 *rcvData, DP_U32 rcvDataLen) {
	printf("Port1 RcvDataLen %d rcvData %s \r\n", rcvDataLen, rcvData);
//	for (unsigned int i = 0; i < 50; i++) {
//		printf("0x%0.2x ", rcvData[i]);
//		if ((i + 1) % 16 == 0)
//			printf("\r\n");
//	}
//	printf("\r\n");
	return 0;

}

int CtrlBoardHandle::RS485RcvCallBack(DP_U8 *rcvData, DP_U32 rcvDataLen) {

	printf("RS485 RcvDataLen %d 0x%x rcvData %s \r\n", rcvDataLen, rcvData[0],
			rcvData);
	return 0;
}
void CtrlBoardHandle::startRunning() {
	DP_UDRV_REG_Init();
	printf("%s %s %d\r\n", __FILE__, __FUNCTION__, __LINE__);
	DP_UDRV_GPIO_Init();
	DP_UDRV_UART_Init();
	DP_UDRV_BOARD_SetupPinMuxer();
	_loop->runInLoop(boost::bind(&CtrlBoardHandle::run, this));
#if 1
//	typedef void (*TestFunc)();
	typedef boost::function<void()> TestFunc;

	typedef struct _FuncTest_S {
		char* name;
		TestFunc func;
	} FuncTest_S;
//	void PrintfUsage();

	FuncTest_S TestFuncArry[] = {

	{ (char*) "StartIRLearn", boost::bind(  //0
			&CtrlBoardHandle::startIRLearn, this) }, { (char*) "StopIRLearn",
			boost::bind( 	 //1
					&CtrlBoardHandle::StopIRLearn, this) }, {
			(char*) "StartIRSendSTD", boost::bind(	 //2
					&CtrlBoardHandle::StartIRSendSTD, this) }, {
			(char*) "StartIRSendPlusWidth", boost::bind(	 //3
					&CtrlBoardHandle::StartIRSendPlusWidth, this) }, {
			(char*) "StartIRRcv", boost::bind(	 //4
					&CtrlBoardHandle::StartIRRcv, this) }, {
			(char*) "StopIRRcv", boost::bind(	 //5
					&CtrlBoardHandle::StopIRRcv, this) }, {
			(char*) "OpenRS232Port0AndSend", boost::bind(	 //6
					&CtrlBoardHandle::OpenRS232Port0AndSend, this) }, {
			(char*) "OpenRS232Port1AndSend", boost::bind(	 //7
					&CtrlBoardHandle::OpenRS232Port1AndSend, this) }, {
			(char*) "OpenRS485Rcv", boost::bind(	 //8
					&CtrlBoardHandle::OpenRS485Rcv, this) }, {
			(char*) "RS485Send", boost::bind(	 //9
					&CtrlBoardHandle::RS485Send, this) }, { (char*) "RS232Test",
			boost::bind(	 //10
					&CtrlBoardHandle::RS232Test, this) }, { (char*) "IOTest",
			boost::bind(&CtrlBoardHandle::IOTest,	 //11
					this) }, };

	printf("#############################\r\n");
	for (unsigned int i = 0; i < (DP_ARRAY_SIZE(TestFuncArry)); i++) {
		printf("%d.%s\r\n", i, TestFuncArry[i].name);
	}
	printf("#############################\r\n");

	while (1) {
		char cmdBuf[1024];
		if ((fgets(cmdBuf, sizeof(cmdBuf), stdin)) != NULL) {
			//printf("Read line with len: %d\n", strlen(cmdBuf));
			unsigned int idx = 0;
			printf("%s", cmdBuf);
			if (strcmp(cmdBuf, "\n") == 0) {
				continue;
			}

			sscanf(cmdBuf, "%u", &idx);
			printf("idx %d\r\n", idx);

			if (idx < (DP_ARRAY_SIZE(TestFuncArry))) {
				TestFuncArry[idx].func();
			}
		}

	}
#endif
}

void CtrlBoardHandle::run() {
	int retval;
	int inputEventFd[5];
	struct input_event event;
	fd_set readfds;
	int ret;
	struct timeval tv;
	DP_UDRV_CtrlBoard_Init();
	DP_UDRV_CtrlBoard_RegisterIRLearnCallback(IrLearnRcvCallback);

	for (DP_U32 idx = 0; idx < DP_ARRAY_SIZE(inputEventFd); idx++)
		inputEventFd[idx] = -1;
	while (_loop->isLoop()) {
//		std::cout << "====================run looppppppppppppppppp======================"
//					<< std::endl;
		// 设置最长等待时间
		char inputEventPath[256];
		bool inputEventExist = false;
		for (DP_U32 idx = 0; idx < DP_ARRAY_SIZE(inputEventFd); idx++) {
			sprintf(inputEventPath, "/dev/input/event%d", idx);
			//printf("inputEventPath %s\r\n",inputEventPath);
			if ((access(inputEventPath, R_OK)) == 0) {
				//printf("access OK\r\n");
				if (inputEventFd[idx] < 0) {
					//printf("access %OK\r\n");
					inputEventFd[idx] = open(inputEventPath, O_RDONLY);
					// 判断是否打开成功
					if (inputEventFd[idx] < 0) {
						printf("open %s error\r\n", inputEventPath);
					}
					printf("Open %s\r\n", inputEventPath);
				}
				inputEventExist = true;
			} else {
				if (inputEventFd[idx] > 0) {
					printf("%s Not Exist Close it!\r\n", inputEventPath);
					close(inputEventFd[idx]);
					inputEventFd[idx] = -1;
				}
			}
		}
		if (inputEventExist) {
			int maxfd = 0;
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			FD_ZERO(&readfds);
			for (DP_U32 idx = 0; idx < DP_ARRAY_SIZE(inputEventFd); idx++) {
				FD_SET(inputEventFd[idx], &readfds);
				if (maxfd < inputEventFd[idx])
					maxfd = inputEventFd[idx];
			}
			//printf("maxfd is %d\r\n",maxfd);
			retval = select(maxfd + 1, &readfds, NULL, NULL, &tv);
			if (retval == 0) {
				printf("Time out!\n");
			} else if (retval < 0) {
				printf("some error\r\n");
			}
			for (DP_U32 idx = 0; idx < DP_ARRAY_SIZE(inputEventFd); idx++) {
				if ((inputEventFd[idx] > 0)
						&& (FD_ISSET(inputEventFd[idx], &readfds))) {
					static unsigned int lastSendType = 0;
					// 读取鼠标设备中的数据
					ret = read(inputEventFd[idx], &event,
							sizeof(struct input_event));
					DP_UDRV_CtrlBoard_SendInputEvent(event);
				}
			}
		} else {
			sleep(1);
			continue;
		}

	}
	for (DP_U32 idx = 0; idx < DP_ARRAY_SIZE(inputEventFd); idx++)
		close(inputEventFd[idx]);
	DP_UDRV_CtrlBoard_DeInit();
}

void CtrlBoardHandle::startIRLearn() {
	std::cout << "====================start ir learn======================"
			<< std::endl;
	DP_UDRV_CtrlBoard_StartIRLearn();
}

void CtrlBoardHandle::StopIRLearn() {
	std::cout << "====================stop ir learn======================"
			<< std::endl;
	DP_UDRV_CtrlBoard_StopIRLearn();
}

void CtrlBoardHandle::StartIRSendSTD() {
	std::cout << "====================start ir send std======================"
			<< std::endl;
	IRRecv_S irRecv = CtrlBoardHandle::_queue.take();
	DP_UDRV_CtrlBoard_SendIRLearnBuf(0, irRecv.irRcvCode, irRecv.irRcvLen);
}

void CtrlBoardHandle::StartIRSendPlusWidth() {
	std::cout
			<< "====================start ir send plus width ======================"
			<< std::endl;
	IRRecv_S irRecv = CtrlBoardHandle::_queue.take();
	DP_UDRV_CtrlBoard_SendIRLearnBuf(1, irRecv.irRcvCode, irRecv.irRcvLen);
}

void CtrlBoardHandle::StartIRRcv() {
	std::cout << "====================start ir recv======================"
			<< std::endl;
	DP_S32 ret = DP_UDRV_CtrlBoard_StartIRRcv();
	std::cout << "retttttttttttttir recv: " << ret << std::endl;
}

void CtrlBoardHandle::StopIRRcv() {
	std::cout << "====================stop ir recv======================"
			<< std::endl;
	DP_UDRV_CtrlBoard_StopIRRcv();
}

void CtrlBoardHandle::OpenRS232Port0AndSend() {
	std::cout
			<< "====================open rs 232 prot 0 and send ======================"
			<< std::endl;
	DP_UDRV_CTRL_BOARD_UartAttr_S attr;

	attr.baudRate = 115200;
	attr.dataBits = 8;
	attr.parity = 'n';
	attr.stopBits = 1;
	if (DP_UDRV_CtrlBoard_RS232Open(0, attr) != DP_UDRV_SUCCESS)
		printf("DP_UDRV_CtrlBoard_RS232Open error\r\n");

	DP_UDRV_CtrlBoard_RegisterRS232CallBack(0, RS232Port0RcvCallBack);

	char cmdBuf[1024];
	printf("Enter Send String :");
	if ((fgets(cmdBuf, sizeof(cmdBuf), stdin)) != NULL) {
		DP_UDRV_CtrlBoard_RS232SendData(0, (unsigned char*) cmdBuf,
				strlen(cmdBuf));
		printf("  String :: %s\n", cmdBuf);
	}
}

void CtrlBoardHandle::OpenRS232Port1AndSend() {
	std::cout
			<< "====================open rs 232 prot 1 and send ======================"
			<< std::endl;
	DP_UDRV_CTRL_BOARD_UartAttr_S attr;

	attr.baudRate = 115200;
	attr.dataBits = 8;
	attr.parity = 'n';
	attr.stopBits = 1;
	if (DP_UDRV_CtrlBoard_RS232Open(1, attr) != DP_UDRV_SUCCESS)
		printf("DP_UDRV_CtrlBoard_RS232Open error\r\n");

	DP_UDRV_CtrlBoard_RegisterRS232CallBack(1, RS232Port1RcvCallBack);
	char cmdBuf[1024];
	printf("Enter Send String :");
	if ((fgets(cmdBuf, sizeof(cmdBuf), stdin)) != NULL) {
		DP_UDRV_CtrlBoard_RS232SendData(1, (unsigned char*) cmdBuf,
				strlen(cmdBuf));
		printf("  String :: %s len: %d\n", cmdBuf, strlen(cmdBuf));
	}
}

void CtrlBoardHandle::OpenRS485Rcv() {
	std::cout << "====================open rs 485 recv ======================"
			<< std::endl;
	DP_UDRV_CTRL_BOARD_UartAttr_S attr;

	static unsigned int RS485Mode = 0;
	switch (RS485Mode) {
	default:
	case 0:
		attr.baudRate = 115200;
		attr.dataBits = 8;
		attr.parity = 'e';
		attr.stopBits = 1;
		break;
	case 1:
		attr.baudRate = 38400;
		attr.dataBits = 8;
		attr.parity = 'o';
		attr.stopBits = 1;
		break;
	case 2:
		attr.baudRate = 9600;
		attr.dataBits = 8;
		attr.parity = 'e';
		attr.stopBits = 1;
		break;
	case 3:
		attr.baudRate = 57600;
		attr.dataBits = 8;
		attr.parity = 'e';
		attr.stopBits = 2;
		break;

	}
	RS485Mode = (++RS485Mode) % 4;
	char *cmdBuf = "HelloWorld";
	printf("RS485Mode %d %d%c%d Send String :%s \r\n", attr.baudRate,
			attr.dataBits, attr.parity, attr.stopBits, cmdBuf);
	getchar();
	if (DP_UDRV_CtrlBoard_RS485Open(1, attr) != DP_UDRV_SUCCESS)
		printf("DP_UDRV_CtrlBoard_RS232Open error\r\n");

	DP_UDRV_CtrlBoard_RegisterRS485CallBack(0, RS485RcvCallBack);
	DP_UDRV_CtrlBoard_RS485SendData(0, (unsigned char*) cmdBuf,
			strlen(cmdBuf) + 1);
}

void CtrlBoardHandle::RS485Send() {
	std::cout << "==================== rs 485 send ======================"
			<< std::endl;
	char cmdBuf[1024];
	printf("Enter Send String :");
	if ((fgets(cmdBuf, sizeof(cmdBuf), stdin)) != NULL) {
		DP_UDRV_CtrlBoard_RS485SendData(0, (unsigned char*) cmdBuf,
				strlen(cmdBuf));
	}

}

void CtrlBoardHandle::RS232Test() {
	std::cout << "==================== rs 232 test ======================"
			<< std::endl;
	DP_UDRV_CTRL_BOARD_UartAttr_S attr;
	int testBaudRate[] = { 115200, 57600, 38400, 19200 };
	int testParity[] = { 'n', 'o', 'e' };
	int testStopBit[] = { 1, 2 };
	int testDataBit[] = { 8 };
	//int testDataBit[]={8,7,6,5};
	for (unsigned int testDataBitIdx = 0;
			testDataBitIdx < DP_ARRAY_SIZE(testDataBit); testDataBitIdx++) {
		attr.dataBits = testDataBit[testDataBitIdx];
		for (unsigned int i = 0; i < DP_ARRAY_SIZE(testBaudRate); i++) {
			attr.baudRate = testBaudRate[i];
			for (unsigned int parityIdx = 0;
					parityIdx < DP_ARRAY_SIZE(testParity); parityIdx++) {
				attr.parity = testParity[parityIdx];
				for (unsigned int stopBitIdx = 0;
						stopBitIdx < DP_ARRAY_SIZE(testStopBit); stopBitIdx++) {
					attr.stopBits = testStopBit[stopBitIdx];
					DP_UDRV_CtrlBoard_RS232Close(0);
					DP_UDRV_CtrlBoard_RS232Close(1);

					if (DP_UDRV_CtrlBoard_RS232Open(0, attr) != DP_UDRV_SUCCESS)
						printf("DP_UDRV_CtrlBoard_RS232Open error\r\n");

					if (DP_UDRV_CtrlBoard_RS232Open(1, attr) != DP_UDRV_SUCCESS)
						printf("DP_UDRV_CtrlBoard_RS232Open error\r\n");

					DP_UDRV_CtrlBoard_RegisterRS232CallBack(0,
							RS232Port0RcvCallBack);
					DP_UDRV_CtrlBoard_RegisterRS232CallBack(1,
							RS232Port1RcvCallBack);

					char *cmdBuf = "HelloWorld";
					printf("Port%d %d %d%c%d Send String :%s \r\n", 0,
							attr.baudRate, attr.dataBits, attr.parity,
							attr.stopBits, cmdBuf);
					DP_UDRV_CtrlBoard_RS232SendData(0, (unsigned char*) cmdBuf,
							strlen(cmdBuf));
					//usleep(500000);
					printf("Port%d %d %d%c%d Send String :%s \r\n", 1,
							attr.baudRate, attr.dataBits, attr.parity,
							attr.stopBits, cmdBuf);
					DP_UDRV_CtrlBoard_RS232SendData(1, (unsigned char*) cmdBuf,
							strlen(cmdBuf));
				}
			}

		}
	}

}

void CtrlBoardHandle::IOTest() {
	std::cout << "====================io test ======================"
			<< std::endl;
	for (unsigned int stopBitIdx = 0; stopBitIdx < 10; stopBitIdx++)

	{

		DP_S32 ret = 0;
		ret = DP_UDRV_CtrlBoard_SetIOValue(0, 1);
		ret = DP_UDRV_CtrlBoard_SetIOValue(1, 1);
		usleep(100000);
		DP_UDRV_CtrlBoard_SetIOValue(0, 0);
		DP_UDRV_CtrlBoard_SetIOValue(1, 0);
		usleep(100000);
	}
}
