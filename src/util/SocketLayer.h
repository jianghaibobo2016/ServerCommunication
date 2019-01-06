/*
 * SocketLayer.h
 *
 *  Created on: 2017年7月23日
 *      Author: zxb
 */

#ifndef UTIL_SOCKETLAYER_H_
#define UTIL_SOCKETLAYER_H_
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/un.h>
#include <vector>
//using namespace std;
class SocketLayer {
public:
	static int CreateTCPSock(bool bNoblock = true)/* throw (SystemException)*/{
		int ret;
		if (bNoblock)
			ret = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
		else
			ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
//		else
		return ret;
	}
	static int CreateUDPSock(bool bNoblock = true) /*throw (SystemException)*/{
		int ret;
		if (bNoblock)
			ret = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
		else
			ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
//		else
		return ret;
	}
	static int CreatePipeSock(bool bNoblock = true)/* throw (SystemException)*/{
		int ret;
		if (bNoblock)
			ret = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
		else
			ret = socket(AF_UNIX, SOCK_STREAM, 0);
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
//		else
		return ret;
	}
	/**cIP:＂＂时，表示绑定本机任意ＩＰ*/
	static void BindAddress(const int sock, const uint16_t port,
			const std::string addr = "") /*throw (SystemException)*/{
		sockaddr_in saddr;
		bzero(&saddr, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		if (addr.length() == 0)
			saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		else
			saddr.sin_addr.s_addr = inet_addr(addr.c_str());
		if (bind(sock, (struct sockaddr*) &saddr, sizeof(saddr)) < 0) {

		}
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		else
			return;
	}
	static void Listen(const int sock, int listencount = 2) {
		int ret = listen(sock, listencount);
//		if (ret == -1)
//			throw SystemException();
	}
//	static void GetIdlePort(vector<uint16_t>& idlePort, const int needCount) {
//		uint16_t lastidleport = 3000;
//		for (int i = 0; i < needCount; i++) {
//			lastidleport = GetIdlePort(lastidleport + 1);
//			idlePort.push_back(lastidleport);
//		}
//	}
//	static uint16_t GetIdlePort(const uint16_t startPort = 3000, const int checkCount = 1000) throw (SystemException) {
//		for (int i = 0; i < checkCount; i++) {
//			int sock;
//			try {
//				sock = CreateUDPSock();
//			} catch (SystemException &ex) {
//				throw SystemException("Error create UDP sock");
//			}
//			try {
//				BindAddress(sock, startPort + i);
//			} catch (SystemException &ex) {
//				CloseSock(sock);
//				continue;
//			}
//			CloseSock(sock);
//			return startPort + i;
//
//		}
//		throw SystemException("none idle port");
//		return 0;
//	}
	static uint16_t GetIdlePort(const uint16_t startPort = 3000,
			const int checkCount = 1000) {
		uint16_t result;
		static uint16_t g_tempidleport = startPort;
		if ((g_tempidleport + 100) > (startPort + checkCount)) {
			g_tempidleport = startPort;
		}
		for (int i = 0; i < 100;) {
			int sock;
			result = g_tempidleport + i;
//			try {
			sock = CreateUDPSock();
//			} catch (SystemException &ex) {
//				throw SystemException("Error create UDP sock");
//			}
//			try {
			BindAddress(sock, result);
//			} catch (SystemException &ex) {
//				CloseSock(sock);
//				i += 2;
//				continue;
//			}
			CloseSock(sock);
			g_tempidleport = result + 2;
			return result;

		}
//		throw SystemException("none idle port");
		return 0;

	}
	static sockaddr_un FilePath2UnixAddress(const std::string &strfilepath) {
		sockaddr_un addr;
		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, strfilepath.c_str());
		return addr;
	}
	static void SetNoneBlock(int sock,
			bool bBlock = false)/* throw (SystemException) */{
		int flags = fcntl(sock, F_GETFL, 0);
//		if (flags < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {

		}
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		else
			return;
	}
	static void SetSendBufferSize(int sock,
			int size) /*throw (SystemException)*/{
		int ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
//		else
		return;
	}
	static void SetRecvBufferSize(int sock,
			int size) /*throw (SystemException)*/{
		int ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
//		else
		return;
	}
	static void SetReuseAddr(int sock)/* throw (SystemException)*/{
		int reuse = 1;
		int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse,
				sizeof(reuse));
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
//		else
		return;
	}
	static void Sock2IPPort(int sock, std::string& ip,
			uint16_t &port) /*throw (SystemException) */{
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int ret = getsockname(sock, (sockaddr*) &addr, &len);
//		if (ret < 0)
//			throw SystemException(__FILE__, __FUNCTION__, __LINE__);

		ip = inet_ntoa(addr.sin_addr);
		port = ntohs(addr.sin_port);
	}
	static void IP2Address(const std::string& ip, const uint16_t& port,
			sockaddr_in& saddr) {
		bzero(&saddr, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		if (ip.length() == 0)
			saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		else
			saddr.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	static void CloseSock(int sock) /*throw (SystemException) */{
		if (sock >= 0) {
			shutdown(sock, SHUT_RDWR);
			close(sock);
//			LOG_WARN
//					<< "****************************************** close connect countConnect: "
//					<< countConnect;
		}

	}
	static uint32_t IPString2Int(const std::string& ip) {

		return inet_addr(ip.c_str());
	}
	static std::string Int2IPString(uint32_t ip) {
		struct in_addr addr;
		addr.s_addr = ip;
		return inet_ntoa(addr);
	}
	static std::string ByteToHexString(const void *pData, int len,
			const std::string & split = "") {
		char strNum[100];
		std::string strResult;
		for (int i = 0; i < len; i++) {
			sprintf(strNum, "%02x%s", *((unsigned char*) pData + i),
					split.c_str());
			strResult += strNum;
		}
		return strResult;
	}
	static int stringtoint(char* str, uint8_t* data, int nstrlen) {
		//去掉空格及其他字符
		std::string strbuf;
		int nCount = 0;
		for (int nj = 0; nj < nstrlen; nj++) {
			if (str[nj] != ' ') {
				strbuf[nCount++] = str[nj];
			}
		}

		for (int ni = 0; ni < nCount / 2; ni++) {
			data[ni] = hexchar2int(strbuf[ni * 2]) * 16
					+ hexchar2int(strbuf[ni * 2 + 1]);
		}
		return nCount;
	}

	static uint8_t hexchar2int(char ch) {
		switch (ch) {
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'a':
		case 'A':
			return 10;
		case 'b':
		case 'B':
			return 11;
		case 'c':
		case 'C':
			return 12;
		case 'd':
		case 'D':
			return 13;
		case 'e':
		case 'E':
			return 14;
		case 'f':
		case 'F':
			return 15;
		default:
			return 0;
		}
	}
//	static int countConnect;
};
//int SocketLayer::countConnect = 0;

#endif /* UTIL_SOCKETLAYER_H_ */
