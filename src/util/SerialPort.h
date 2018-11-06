/*
 * SerialPort.h
 *
 *  Created on: 2017年7月23日
 *      Author: zxb
 */

#ifndef UTIL_SERIALPORT_H_
#define UTIL_SERIALPORT_H_
#include "SocketLayer.h"
#include "SystemException.h"
#include <termio.h>
#include <ThreadUtil.h>
#include <unistd.h>

class SerialPort{
public:
	enum OpenFlags {
		OPENFLAGS_ODD = 'O',
		OPENFLAGS_EVEN = 'E',
		OPENFLAGS_SPACE = 'S',
		OPENFLAGS_NONE = 'N',
	};
	enum Baudrate {
		BAUD4800 = 4800, BAUD9600 = 9600, BAUD38400 = 38400, BAUD115200 = 115200,
	};
	enum DataBits {
		DATABIT7 = 7, DATABIT8 = 8,
	};
	enum StopBIts {
		STOPBIT1 = 1, STOPBIT2 = 2,
	};
	enum FlowControl{
		FlowControl_None = 0,
		FlowControl_Hard = 1,
		FlowControl_Soft = 2,
	};
	SerialPort() {
		_fd = -1;
	}
	virtual ~SerialPort(){

	}

	void openPort(const string& strfile, Baudrate baud = SerialPort::BAUD38400,
			OpenFlags flags = SerialPort::OPENFLAGS_NONE, DataBits databits =
					SerialPort::DATABIT8, StopBIts stopbits =
					SerialPort::STOPBIT1, FlowControl flowcontrol = FlowControl_None)
					throw (SystemException) {
		_devname = strfile;
		_fd = open(strfile.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
		if (-1 == _fd) {
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		}
		//测试是否为终端设备
//		if (isatty(STDIN_FILENO) == 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);

		//设置串口参数

		struct termios newtio, oldtio;
		if (tcgetattr(_fd, &oldtio) != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);

		bzero(&newtio, sizeof(newtio));

		///1.设置字符大小
		newtio.c_cflag |= CLOCAL | CREAD;
		newtio.c_cflag &= ~CSIZE;
		///２．设置数据位
		switch (databits) {
		case SerialPort::DATABIT7:
			newtio.c_cflag |= CS7;
			break;
		case SerialPort::DATABIT8:
			newtio.c_cflag |= CS8;
			break;
		default:
			newtio.c_cflag |= CS8;
			break;
		}
		///3.设置奇偶校验位
		switch (flags) {
		case SerialPort::OPENFLAGS_ODD:                     //奇校验
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_iflag |= (INPCK | ISTRIP);
			break;
		case SerialPort::OPENFLAGS_EVEN:                     //偶校验
			newtio.c_iflag |= (INPCK | ISTRIP);
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD;
			break;
		case SerialPort::OPENFLAGS_NONE:                    //无校验
			newtio.c_cflag &= ~PARENB;
			break;
		case SerialPort::OPENFLAGS_SPACE: //设置为空格
			newtio.c_cflag &= ~PARENB;
			newtio.c_cflag &= ~CSTOPB;
			break;
		}

		switch (baud) {
		case SerialPort::BAUD4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case SerialPort::BAUD9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case SerialPort::BAUD38400:
			cfsetispeed(&newtio, B38400);
			cfsetospeed(&newtio, B38400);
			break;
		case SerialPort::BAUD115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		}

		switch (stopbits) {
		case SerialPort::STOPBIT1:
			newtio.c_cflag &= ~CSTOPB;
			break;
		case SerialPort::STOPBIT2:
			newtio.c_cflag |= CSTOPB;
			break;
		default:
			newtio.c_cflag &= ~CSTOPB;
			break;
		}

		//设置数据流控制
		switch (flowcontrol) {
		case SerialPort::FlowControl_None: //不使用流控制
			newtio.c_cflag &= ~CRTSCTS;
			break;

		case SerialPort::FlowControl_Hard: //使用硬件流控制
			newtio.c_cflag |= CRTSCTS;
			break;
		case SerialPort::FlowControl_Soft: //使用软件流控制
			newtio.c_cflag |= IXON | IXOFF | IXANY;
			break;
		}

		newtio.c_cc[VTIME] = 0;
		newtio.c_cc[VMIN] = 0;
		tcflush(_fd, TCIFLUSH);
		if ((tcsetattr(_fd, TCSANOW, &newtio)) != 0)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);

	}
	void closePort() throw (SystemException) {
		if (isOpen()) {
			if (close(_fd) == -1)
				throw SystemException(__FILE__, __FUNCTION__, __LINE__);
			_fd = -1;
		}
	}
	bool isOpen() {
		return _fd!=(-1);
	}
	int getFD(){
		return _fd;
	}
	const string& DevName()const {
		return _devname;
	}
private:
	int _fd;
	string _devname;
};

#endif /* UTIL_SERIALPORT_H_ */
