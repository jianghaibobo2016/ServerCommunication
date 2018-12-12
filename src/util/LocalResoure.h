/*
 * LocalResoure.h
 *
 *  Created on: 2017年7月24日
 *      Author: mo
 */

#ifndef UTIL_LOCALRESOURE_H_
#define UTIL_LOCALRESOURE_H_
#if 0
#include "SystemException.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/route.h>
#include <string>
#include "SocketLayer.h"
#include "GlobalProfile.h"
#include <sys/reboot.h>
#include <TimerUtil.h>

#include "SystemTime.h"
using namespace std;
class LocalResoure {
public:

	/*static string GetLocalMacAddress() const throw (SystemException) {
	 int sock_name;
	 struct ifreq ifr;

	 sock_name = socket(AF_INET, SOCK_STREAM, 0);
	 if (sock_name == -1) {
	 Console::instance()->debug("sock_name open error");
	 return " ";
	 }
	 memset(&ifr, 0, sizeof(ifr));
	 if ((ioctl(sock_name, SIOCGIFHWADDR, &ifr)) < 0) {
	 Console::instance()->debug("ioctl error");
	 return "";
	 }
	 return string(ifr.ifr_name);
	 }*/

	static int RebootSystem(void) {
		sync();
		return reboot(RB_AUTOBOOT);
	}
	static void GetLocalIP(string& macaddr, string& ip, string& netmask, string& gateway) throw (SystemException) {
		int sock;
		struct sockaddr_in sin;
		struct ifreq ifr;

		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock == -1) {
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		}
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, GlobalProfile::instance()->GetNetCardName().c_str());
		//mac addr
		if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		} else {
			macaddr = SocketLayer::ByteToHexString(ifr.ifr_hwaddr.sa_data, 6);
		}
//			macaddr = ifr.ifr_hwaddr.sa_data;
		//ip addr
		if (ioctl(sock, SIOCGIFADDR, &ifr) != 0) {
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		} else {
			memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
			ip = inet_ntoa(sin.sin_addr);
		}
		//net mask
		if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		} else {
			memcpy(&sin, &ifr.ifr_netmask, sizeof(sin));
			netmask = inet_ntoa(sin.sin_addr);
		}
		close(sock);
		gateway = get_gateway();
	}
	static void SetLocalIP(const string& Ipaddr, const string& mask, const string& gateway) throw (SystemException) {
		int fd;

		struct ifreq ifr;
		struct sockaddr_in *sin;
		struct rtentry rt;

		fd = socket(AF_INET, SOCK_DGRAM, 0);
		if (fd < 0) {
			throw SystemException("socket   error");
			return;
		}
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, "eth0");
		sin = (struct sockaddr_in*) &ifr.ifr_addr;
		sin->sin_family = AF_INET;
		//IP地址
		if (inet_aton((const char*) &Ipaddr, &(sin->sin_addr)) < 0) {
			throw SystemException("inet_aton   error");
			return;
		}

		if (ioctl(fd, SIOCSIFADDR, &ifr) < 0) {
			throw SystemException("ioctl   SIOCSIFADDR   error");
			return;
		}
		//子网掩码
		if (inet_aton((const char*) &mask, &(sin->sin_addr)) < 0) {
			throw SystemException("inet_pton   error");
			return;
		}
		if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0) {
			throw SystemException("ioctl");
			return;
		}
		//网关
		memset(&rt, 0, sizeof(struct rtentry));
		memset(sin, 0, sizeof(struct sockaddr_in));
		sin->sin_family = AF_INET;
		sin->sin_port = 0;
		if (inet_aton((const char*) &gateway, &sin->sin_addr) < 0) {
			throw SystemException("inet_aton error\n");
		}
		memcpy(&rt.rt_gateway, sin, sizeof(struct sockaddr_in));
		((struct sockaddr_in *) &rt.rt_dst)->sin_family = AF_INET;
		((struct sockaddr_in *) &rt.rt_genmask)->sin_family = AF_INET;
		rt.rt_flags = RTF_GATEWAY;
		if (ioctl(fd, SIOCADDRT, &rt) < 0) {
			throw SystemException("ioctl(SIOCADDRT) error in set_default_route\n");
			close(fd);
			return;
		}
		close(fd);

	}
	static string get_gateway() throw (SystemException) {
		FILE *fp;
		char buf[256]; // 128 is enough for linux
		char iface[16];
		unsigned long dest_addr, gate_addr;
		string strResult;
		fp = fopen("/proc/net/route", "r");
		if (fp == NULL)
			throw SystemException(__FILE__, __FUNCTION__, __LINE__);
		/* Skip title line */
		fgets(buf, sizeof(buf), fp);
		while (fgets(buf, sizeof(buf), fp)) {
			if (sscanf(buf, "%s\t%lX\t%lX", iface, &dest_addr, &gate_addr) != 3 || dest_addr != 0)
				continue;
			struct in_addr addr;
			memcpy(&addr, &gate_addr, sizeof(gate_addr));
			strResult = inet_ntoa(addr);
			fclose(fp);
			return strResult;
		}
		fclose(fp);
		throw SystemException("cannot get gateway");
	}
private:
	typedef struct CPU_OCCUPY_Tag         //定义一个cpu occupy的结构体
	{
		char name[20];      //定义一个char类型的数组名name有20个元素
		unsigned int user; //定义一个无符号的int类型的user
		unsigned int nice; //定义一个无符号的int类型的nice
		unsigned int system; //定义一个无符号的int类型的system
		unsigned int idle; //定义一个无符号的int类型的idle
	} CPU_OCCUPY;
	typedef struct MEM_OCCUPY_Tag         //定义一个mem occupy的结构体
	{
		char name[20];      //定义一个char类型的数组名name有20个元素
		unsigned long total;
		char name2[20];
		unsigned long free;
		char name3[20];
		unsigned long available;
		char name4[20];
		unsigned long buffer;
		char name5[20];
		unsigned long cached;
		float occupy;
	} MEM_OCCUPY;

	static void get_memoccupy(MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O
			{
		FILE *fd;
		char buff[256];
		MEM_OCCUPY *m;
		m = mem;

		fd = fopen("/proc/meminfo", "r");

		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu", m->name, &m->total);
		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu", m->name2, &m->free);
		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu", m->name3, &m->available);
		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu", m->name4, &m->buffer);
		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu", m->name5, &m->cached);

		fclose(fd);     //关闭文件fd
		m->occupy = (float) (100 * (m->total - m->free - m->buffer - m->cached)) / (float) m->total;
	}

	static float cal_cpuoccupy(CPU_OCCUPY *o, CPU_OCCUPY *n) {
		unsigned long od, nd;
		unsigned long id, sd;
		float cpu_use = 0;

		od = (unsigned long) (o->user + o->nice + o->system + o->idle); //第一次(用户+优先级+系统+空闲)的时间再赋给od
		nd = (unsigned long) (n->user + n->nice + n->system + n->idle); //第二次(用户+优先级+系统+空闲)的时间再赋给od

		id = (unsigned long) (n->user - o->user);    //用户第一次和第二次的时间之差再赋给id
		sd = (unsigned long) (n->system - o->system);    //系统第一次和第二次的时间之差再赋给sd
		if ((nd - od) != 0)
			cpu_use = (int) ((sd + id) * 10000) / (nd - od); //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
		else
			cpu_use = 0;
		//printf("cpu: %u/n",cpu_use);
		return cpu_use / 100;
	}

	static void get_cpuoccupy(CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
			{
		FILE *fd;

		char buff[256];
		CPU_OCCUPY *cpu_occupy;
		cpu_occupy = cpust;

		fd = fopen("/proc/stat", "r");
		fgets(buff, sizeof(buff), fd);

		sscanf(buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle);

		fclose(fd);
	}

	void Set_IPAddress(string ip, string mask, string gw) {
//		char name[200];
//		sprintf(name,"./networkTest/resetNetwork.sh");
		string cmd = "resetNetwork.sh ";
		cmd += " ";
		cmd += ip;
		cmd += " ";
		cmd += mask;
		cmd += " ";
		cmd += gw;
		system(cmd.c_str());
	}

	void Set_HardTime(TIME *_time){
		char settime[200];
		sprintf(settime,"%d/%d/%d/%d/%d/%d",_time->year,_time->month,_time->day,_time->hour,_time->minute,_time->second);
		string cmd = "./dsppa/SetTime/Set_Time -s ";
		cmd += settime;
		system(cmd.c_str());
	}

public:
	static void GetLocalCPUandMEM(float& cpu, float& mem) throw (SystemException) {
		CPU_OCCUPY CurCpuOccupy;
		MEM_OCCUPY MemOccupy;
		get_cpuoccupy(&CurCpuOccupy);
		static CPU_OCCUPY g_PreCpuOccupy = CurCpuOccupy;
		cpu = cal_cpuoccupy(&g_PreCpuOccupy, &CurCpuOccupy);
		get_memoccupy(&MemOccupy);
		mem = MemOccupy.occupy;
		g_PreCpuOccupy = CurCpuOccupy;
	}

};
#endif
#endif /* UTIL_LOCALRESOURE_H_ */
