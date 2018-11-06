/*
 * Data.h
 *
 *  Created on: 2018年10月19日
 *      Author: queen
 */

#ifndef DEFINE_DATA_H_
#define DEFINE_DATA_H_

#include "dp_sd_type.h"

#define DP_DEV_NAME_LEN			32			//设备型号长度
#define DP_DEV_ID_LEN			13			//设备ID长度
#define DP_MATRIX_NAME_LEN		32			//矩阵名字长度
#define DP_USER_NAME_LEN		20			//用户账号长度
#define DP_USER_PASSWORD_LEN	20			//用户密码长度
#define DP_URL_LEN				256			//URL地址长度

//公共数据：每个节点都需要 保存且同步 的 相同信息
//1.设备组网列表
//profile
typedef struct _DP_DEV_PUBLIC_DEVLIST_INFO_S {
	DP_U16 u16DevSn;						//序列号
	DP_U16 u16DevType;						//设备类型
	DP_U16 u16DevNum;						//设备型号
	DP_U8 au8DevName[DP_DEV_NAME_LEN];		//设备名
	DP_U8 au8DevID[DP_DEV_ID_LEN];			//设备ID
	DP_U32 u32DevIP;						// IP 172.16.0.1表示为0xAC100001
	DP_U16 u16DevSoftVersion;				//设备软件版本
	DP_U16 u16DevHardVersion;				//设备硬件版本
} DP_DEV_PUBLIC_DEVLIST_INFO_S;
//DP_DEV_PUBLIC_DEVLIST_INFO_S astDeviceList[];

//2.设备在线列表
//通信、定時（策略）
typedef struct _DP_DEV_PUBLIC_DEVONLINELIST_INFO_S {
	DP_U16 u16DevSn;				//序列号
} DP_DEV_PUBLIC_DEVONLINELIST_INFO_S;
//DP_DEV_PUBLIC_DEVONLINELIST_INFO_S stDeviceOnlineList[];

//3.用户信息列表
//profile
typedef struct _DP_DEV_PUBLIC_USER_INFO_S {
	DP_U8 au8UserName[DP_USER_NAME_LEN];			//用户名
	DP_U8 au8PassWord[DP_USER_PASSWORD_LEN];		//密码
	DP_U8 u8Priority;				//用户优先级 0：优先级最高 1：优先级低于0
} DP_DEV_PUBLIC_USER_INFO_S;
//DP_DEV_PUBLIC_USER_INFO_S astUser[];

//4.用户权限信息列表
//profile
//Note：每个用户都有一个权限信息表。
typedef struct _DP_DEV_PUBLIC_USERPERMISSION_INFO_S {
	DP_U16 u16DevSn;			//序列号
	DP_U8 u8Permission;			//权限	0：可独占
								//1：可完全控制
								//2：仅视频
								//3：禁止操作
} DP_DEV_PUBLIC_USERPERMISSION_INFO_S;
//DP_DEV_PUBLIC_USERPERMISSION_INFO_S astUserPermission[];

//5.鼠标矩阵配置信息列表
//profile
//Note：该列表信息仅用于输出节点。
typedef struct _DP_DEV_PUBLIC_MOUSEMATRIXSETTING_INFO_S {
	DP_U32 u32MatrixID;				//矩阵ID
	DP_U8 au8MatrixName[DP_MATRIX_NAME_LEN];	//矩阵名
	DP_U8 u8Row;					//行数 0：表示0行
	DP_U8 u8Colum;					//列数 0：0表示0行
} DP_DEV_PUBLIC_MOUSEMATRIXSETTING_INFO_S;
//DP_DEV_PUBLIC_MOUSEMATRIXSETTING_INFO_S astMouseMatrixSetting[];

//6.鼠标矩阵列表
//profile
//Note：该列表信息仅用于输出节点，每个鼠标矩阵ID都有一个鼠标矩阵列表。
typedef struct _DP_DEV_PUBLIC_MOUSEMATRIXSETTING_MOUSEMATRIX_INFO_S {
	DP_U16 u16DevSn;				//序列号
} DP_DEV_PUBLIC_MOUSEMATRIXSETTING_MOUSEMATRIX_INFO_S;
//DP_DEV_PUBLIC_MOUSEMATRIXSETTING_MOUSEMATRIX_INFO_S astMouseMatrix[];

//7.版本列表
//profile
typedef struct _DP_DEV_PUBLIC_LISTVERSION_INFO_S {
	DP_U32 u32DeviceListVersion;				//设备列表版本（时间值）
	DP_U32 u32OnlineDeviceListVersion;			//在线设备列表版本（时间值）
	DP_U32 u32UserInfoListVersion;				//用户信息列表版本（时间值）
	DP_U32 u32UserAuthInfoListVersion;			//用户权限信息列表版本（时间值）
	DP_U32 u32MouseMatrixSetVersion;			//鼠标矩阵配置列表版本（时间值）
	DP_U32 u32MouseMatrixVersion;				//鼠标矩阵列表版本（时间值）
} DP_DEV_PUBLIC_LISTVERSION_INFO_S;

//私有数据：每个节点自身的 不同信息
//1.基本信息
//profile
typedef struct _DP_DEV_BASE_INFO_S {
	DP_U16 u16DevSn;				//序列号
	DP_U16 u16Devtype;				//设备类型
	DP_U16 u16DevNum;					//设备型号
	DP_U8 au8DevName[DP_DEV_NAME_LEN];			//设备名
	DP_U8 au8DevID[DP_DEV_ID_LEN];				//设备ID
	DP_U16 u16DevWorkMode;			//工作模式
	DP_U16 u16DevFuncOnOff;			//功能模块开启禁用状态
	DP_U16 u16DevSoftVersion;		//设备软件版本
	DP_U16 u16DevHardVersion;		//设备硬件版本
} DP_DEV_BASE_INFO_S;

//2.系统时间信息
//接口、用時獲取
typedef struct _DP_DEV_SYSTIME_INFO_S {
	DP_U64 u64SysTime;				//系统时间
} DP_DEV_SYSTIME_INFO_S;

//3.网络信息
//profile接口、初始
typedef struct _DP_DEV_NET_INFO_S {
	DP_U8 u8PhyNums;				//网口数量 0：没有 1：1个
	DP_U8 u8PhyMode;				//网口工作模式 0：正常模式 1：主备模式
	DP_U8 u8PhyUsedOnBongding;		//主备模式运行网口：0：用网口0；1：用网口1
} DP_DEV_NET_INFO_S;

//4.网口信息
//接口、初始
//Note：每个网口都会有此信息。
typedef struct _DP_DEV_NETINTERFACE_INFO_S {
	DP_U8 u8PhyID;				//网口ID
	DP_U8 u8PhyState;			//网口工作状态：0：非工作；1：工作中
	DP_U16 u16Speed;			//网速(Mb/s)
	DP_U32 u32IPAddr;			//IP 172.16.0.1表示为0xAC100001
	DP_U32 u32SunNetMask;		//子网掩码
	DP_U32 u32GateWay;			//网关
} DP_DEV_NETINTERFACE_INFO_S;

//5.整体状态
//接口、定時（策略）
typedef struct _DP_DEV_STATE_INFO_S {
	DP_U16 u16RunningState;	//运行阶段
	DP_U16 u16WorkState;		//当前工作状态
} DP_DEV_STATE_INFO_S;

//6.性能状态
//接口、定時（策略）
typedef struct _DP_DEV_CAPABILITY_INFO_S {
	DP_U8 u8CPUUsageRate;			//CPU使用率0：0%   50：50%
	DP_U8 u8MemoryUsageRate;		//内存使用率0：0%   50：50%
	DP_U8 u32FlashUsageRate;		//Flash使用率0：0%   50：50%
} DP_DEV_CAPABILITY_INFO_S;

//7.温度状态
//接口、定時（策略）
typedef struct _DP_DEV_TEMPERATURE_INFO_S {
	DP_U16 u16DevEnvTemp; //温度℃高8位表示小数点前数值，第八位表示小数点后数值
} DP_DEV_TEMPERATURE_INFO_S;

//8.中控开关状态
//通信、定時（策略）
typedef struct _DP_DEV_CONTROLONOFF_INFO_S {
	DP_U8 u8ControlBoardState;	//当前是否有中控板0：没有中控板	1：有中控板
	DP_U16 u16ControlState;		//当前中控设备开关状态（按位表示）0：关 1：开
	//最低位为第一个IO
	//4个IO、4个IR输出、1个IR输入、3个串口
} DP_DEV_CONTROLONOFF_INFO_S;

//9.红外码
//profile
//Note：每个红外码都会有此信息。
typedef struct _DP_DEV_IRLIST_INFO_S {
	DP_U8 u8IRType;		//红外码类型
	DP_U8 u8IRNum;		//红外码(與红外码类型組成鍵值)
	DP_U16 u16IRValue;		//红外码值
} DP_DEV_IRLIST_INFO_S;

//10.串口配置
//profile
//Note：每个串口都会有此信息。
typedef struct _DP_DEV_UARTSET_INFO_S {
	DP_U8 u8UartID;				//串口ID
	DP_U16 u16BaudRate;			//波特率
	DP_U8 u8WordLength;			//数据位
	DP_U8 u8StopBit;			//停止位
	DP_U8 u8Parity;				//校验方法
} DP_DEV_UARTSET_INFO_S;

//11.键鼠接口状态
//接口、定時（策略）
typedef struct _DP_DEV_MOUSEKEY_INFO_S {
	DP_U8 u8MouseState;		//当前鼠标状态（按位表示）0：有 1：无
	DP_U8 u8KeyBoardState;		//当前键盘状态（按位表示）0：有 1：无
} DP_DEV_MOUSEKEY_INFO_S;

//12.U盘接口状态
//接口、定時（策略）
typedef struct _DP_DEV_USBINTERFACE_INFO_S {
	DP_U8 u8USBInterfaceState;		//当前USB状态（按位表示）0：有 1：无
} DP_DEV_USBINTERFACE_INFO_S;

//13.U盘信息
//接口、用後定時（策略）
//Note：每个U盘接口都会有此信息。
typedef struct _DP_DEV_USB_INFO_S {
	//暂未定义
} DP_DEV_USB_INFO_S;

//14.音频接口数目信息
//接口、初始
typedef struct _DP_DEV_AUDIOINTERFACE_INFO_S {
	DP_U8 u8AiDevIntfNums;	//音频输入设备数量 0：没 1：1个
	DP_U8 u8AoDevIntfNums;	//音频输出设备数量 0：没 1：1个
	DP_U8 u8AencChnNums;	//音频编码通道数量 0：没 1：1个
	DP_U8 u8AdecChnNums;	//音频解码通道数量 0：没 1：1个
} DP_DEV_AUDIOINTERFACE_INFO_S;

//15.音频输入接口状态
//接口通信、用後定時（策略）
//Note：每个音频输入接口都会有此信息。
typedef struct _DP_DEV_AUDIOIN_INFO_S {
	DP_U8 u8AiDevIntfId;				//音频输入设备ID
	DP_U8 u8AiDevIntfConnStatus;		//音频输入设备连接状态0：没 1：有
	DP_U8 u8AiDevIntfSignalStatus;		//音频输入设备信号状态0：没 1：有
	DP_U8 u8AiDevIntfSampleRate;		//音频输入设备采样率
	//8/16/22/32/44/48/96/…/192
	DP_U8 u8AiDevIntfBitwidth;		//音频输入设备采样率精度
	DP_U8 u8AiDevIntfSoundMode;		//音频输入设备声道模式
									//0：单声道 1：立体声道
	DP_U8 u8AiDevIntfVolume;		//音频输入设备音量0-100
} DP_DEV_AUDIOIN_INFO_S;

//16.音频编码状态
//通信、用後定時（策略）
//Note：每个音频编码通道都会有此信息。
typedef struct _DP_DEV_AUDIOENCODED_INFO_S {
	DP_U8 u8AencChnId;			//编码通道ID
	DP_U8 u8AencBindAiId;		//音频输入设备ID
	DP_U8 u8AencType;			//编码类型
	DP_U8 u8AencBitRate;		//编码码率
} DP_DEV_AUDIOENCODED_INFO_S;

//17.音频解码状态
//通信、用後定時（策略）
//Note：每个音频解码通道都会有此信息。
typedef struct _DP_DEV_AUDIODECODE_INFO_S {
	DP_U8 u8AdecChnId;			//解码通道ID
	DP_U8 u8AdecBindAoId;		//解码绑定的音频输出设备ID
	DP_U8 u8AdecType;			//解码类型
} DP_DEV_AUDIODECODE_INFO_S;

//18.音频播放状态
//接口通信、用後定時（策略）
//Note：每个音频输出接口都会有此信息。
typedef struct _DP_DEV_AUDIOOUT_INFO_S {
	DP_U8 u8AoDevIntfId;			//音频输出设备ID
	DP_U8 u8AoDevIntfConnStatus;	//音频输出设备连接状态0：没 1：有
	DP_U8 u8AoDevIntfSignalStatus;	//音频输出设备信号状态0：没 1：有
	DP_U8 u8AoDevIntfSampleRate;	//音频输出设备采样率
									//8/16/22/32/44/48/96/…/192
	DP_U8 u8AoDevIntfBitwidth;		//音频输出设备采样率精度
	DP_U8 u8AoDevIntfSoundMode;		//音频输出设备声道模式
									//0：单声道 1：立体声道
	DP_U8 u8AoDevIntfVolume;		//音频输出设备音量0-100
} DP_DEV_AUDIOOUT_INFO_S;

//19.视频接口数目信息
//接口、初始
typedef struct _DP_DEV_VIDEOINTERFACE_INFO_S {
	DP_U8 u8ViDevIntfNums;	//视频输入设备数量 0：没 1：1个
	DP_U8 u8VoDevIntfNums;	//视频输出设备数量 0：没 1：1个
	DP_U8 u8VencChnNums;	//视频编码通道数量 0：没 1：1个
	DP_U8 u8VdecChnNums;	//视频解码通道数量 0：没 1：1个
} DP_DEV_VIDEOINTERFACE_INFO_S;

//20.视频输入设备接口状态
//接口通信、用後定時（策略）
//Note：每个视频输入接口都会有此信息。
typedef struct _DP_DEV_VIDEOIN_INFO_S {
	DP_U8 u8ViDevIntfId;				//视频输入设备ID
	DP_U8 u8ViDevIntfConnStatus;		//视频输入设备连接状态0：没 1：有
	DP_U8 u8ViDevIntfSignalStatus;		//视频输入设备信号状态0：没 1：有
	DP_S32 s32ViDevIntfSrcX;			//视频输入设备信号图像源起始坐标X
	DP_S32 s32ViDevIntfSrcY;			//视频输入设备信号图像源起始坐标Y
	DP_U32 u32ViDevIntfSrcWidth;		//视频输入设备信号源宽度
	DP_U32 u32ViDevIntfSrcHeight;		//视频输入设备信号源高度
	DP_U32 u32ViDevIntfdstWidth;		//视频输入设备信号目标宽度
	DP_U32 u32ViDevIntfdstHeight;		//视频输入设备信号目标高度
	DP_U8 u8ViDevIntfsrcFrm;			//视频输入设备信号源帧率
	DP_U8 u8ViDevIntfdstFrm;			//视频输入设备信号目标帧率
} DP_DEV_VIDEOIN_INFO_S;

//21.视频编码信息
//通信、用後定時（策略）
//Note：每个视频编码通道都会有此信息。
typedef struct _DP_DEV_VIDEOENCODED_BASE_INFO_S {
	DP_U8 u8VencBindViId;		//编码绑定的视频输入设备ID
	DP_U8 u8VencType;			//编码类型
	DP_U8 u8VencProfile;		//编码Profile
	DP_U8 u8VencRes;			//编码分辨率
	DP_U8 u8VencRCMode;			//编码码率控制模式
	DP_U32 u32VencBitrate;		//编码码率
	DP_U32 u32VencGop;			//编码GOP
	DP_U16 u16VencSF;			//编码空域
	DP_U16 u16VencTF;			//编码时域
} DP_DEV_VIDEOENCODED_BASE_INFO_S;

//22.图像缩放
//通信、用後定時（策略）
//Note：每个视频编码通道都会有此信息。
typedef struct _DP_DEV_VIDEOENCODED_ZOOM_INFO_S {
	DP_U8 u8ImgZoomOpen;		//图像缩放开关0：关 1：开
	DP_U8 u8ImgZoomMode;		//缩放模式0：等比缩放 1：固定缩放
	DP_U32 u32ZoomWidth;		//图像固定宽度/图像等比缩放宽度比例
	DP_U32 u32ZoomHeight;		//图像固定高度/图像等比缩放高度比例
} DP_DEV_VIDEOENCODED_ZOOM_INFO_S;

//23.图像信息叠加
//通信、用後定時（策略）
//Note：每个视频编码通道都会有此信息。
typedef struct _DP_DEV_VIDEOENCODED_OSD_INFO_S {
	DP_U8 u8OSDOpen;				// 图像信息叠加开关0：关 1：开
	DP_U8 u8OSDDispColor;			// 图像信息叠加显示颜色
	DP_U8 u8OSDDispMode;			// 图像信息叠加显示方式0：位置 1：坐标
	DP_S32 s32OSDRectX;				//图像信息叠加显示区域起始坐标X
	DP_S32 s32OSDRectY;				//图像信息叠加显示区域起始坐标Y
	DP_U32 u32OSDRectW;				//图像信息叠加显示区域起始坐标宽度
	DP_U32 u32OSDRectH;				//图像信息叠加显示区域起始坐标高度
} DP_DEV_VIDEOENCODED_OSD_INFO_S;

//24.视频编码状态
//通信、用後定時（策略）
//Note：每个视频编码通道都会有此信息。
typedef struct _DP_DEV_VIDEOENCODED_INFO_S {
	DP_U8 u8VencChnId;			//编码通道ID
	DP_DEV_VIDEOENCODED_BASE_INFO_S stBase;	//视频编码信息
	DP_DEV_VIDEOENCODED_ZOOM_INFO_S stZoom;	//图像缩放信息
	DP_DEV_VIDEOENCODED_OSD_INFO_S stOSD;	//图像叠加信息
}DP_DEV_VIDEOENCODED_INFO_S;

//25.视频基础信息
//通信、用後定時（策略）
//Note：每个视频输出接口都会有此信息。
typedef struct _DP_DEV_VIDEOOUT_BASE_INFO_S {
	DP_U8 u8VoDevIntfConnStatus;		//视频输出设备连接状态
	DP_U8 u8VoDevIntfSignalStatus;		//视频输出设备信号状态
	DP_U8 u8VoDevIntfOpen;				//视频输出设备显示开关
	DP_U8 u8VoDevIntfTiming;			//视频输出设备显示时序
	DP_U8 u8VoDevIntfSwmsChnNums;		//视频输出设备显示时的视频拼接通道数目
} DP_DEV_VIDEOOUT_BASE_INFO_S;

//26.图像拼接信息
//通信、用後定時（策略）
//Note：每个视频输出接口都会有此信息。
typedef struct _DP_DEV_VIDEOOUT_SPLICING_INFO_S {
	DP_U8 u8ImgSwmsChnId;			//图像拼接通道ID
	DP_U8 u8ImgSwmsOpen;			//图像拼接通道开关0：关 1：开
	DP_U8 u8WindowPriority;			//图像拼接通道优先级0：最高优先级
	DP_S32 s32ImgSwmsRectX;			//图像拼接矩阵区域起始坐标X
	DP_S32 s32ImgSwmsRectY;			//图像拼接矩阵区域起始坐标Y
	DP_U32 u32ImgSwmsRectW;		//图像拼接矩阵区域起始坐标宽度
	DP_U32 u32ImgSwmsRectH;		//图像拼接矩阵区域起始坐标高度
} DP_DEV_VIDEOOUT_SPLICING_INFO_S;

//27.视频播放OSD信息
//通信、用後定時（策略）
//Note：每个视频输出接口都会有此信息。
typedef struct _DP_DEV_VIDEOOUT_OSD_INFO_S {
	DP_U8 u8OSDOpen;			// 图像信息叠加开关0：关 1：开
	DP_U8 u8OSDDispColor;		// 图像信息叠加显示颜色
	DP_U8 u8OSDDispMode;		// 图像信息叠加显示方式：0：位置 1：坐标
	DP_S32 s32OSDRectX;			//图像信息叠加显示区域起始坐标X
	DP_S32 s32OSDRectY;			//图像信息叠加显示区域起始坐标Y
	DP_U32 u32OSDRectW;			//图像信息叠加显示区域起始坐标宽度
	DP_U32 u32OSDRectH;			//图像信息叠加显示区域起始坐标高度
} DP_DEV_VIDEOOUT_OSD_INFO_S;

//28.图像输出效果
//通信、用後定時（策略）
//Note：每个视频输出接口都会有此信息。
typedef struct _DP_DEV_VIDEOOUT_EFFECT_INFO_S {
	DP_U8 u8ImgSharpen;			//锐化度
	DP_U8 u8ImgCSCLuma;			//亮度
	DP_U8 u8ImgCSCContrast;			//对比度
	DP_U8 u8ImgCSCHue;				//色度
	DP_U8 u8ImgCSCSaturation;		//饱和度
} DP_DEV_VIDEOOUT_EFFECT_INFO_S;

//29.视频输出设备接口状态
//通信、用後定時（策略）
//Note：每个视频输出接口都会有此信息。
typedef struct _DP_DEV_VIDOEOUT_INFO_S {
	DP_U8 u8VoDevIntfId;			//视频输出设备ID
	DP_DEV_VIDEOOUT_BASE_INFO_S stBase;		//视频基础信息
	DP_DEV_VIDEOOUT_SPLICING_INFO_S astSplicing[];	//图像拼接信息
	DP_DEV_VIDEOOUT_OSD_INFO_S stOSD;		//视频播放OSD信息
	DP_DEV_VIDEOOUT_EFFECT_INFO_S stEffect;		//图像输出效果
} DP_DEV_VIDOEOUT_INFO_S;

//30.视频基础信息
//通信、用後定時（策略）
//Note：每个视频解码通道都会有此信息。
typedef struct _DP_DEV_VIDEODECODE_BASE_INFO_S {
	DP_U8 u8VdecBindVoId;			//解码绑定的视频输出设备ID
	DP_U8 u8VdecBindVoChnId;		//解码绑定的视频输出显示通道ID
	DP_U8 u8VdecType;				//解码类型
} DP_DEV_VIDEODECODE_BASE_INFO_S;

//31.视频播放裁剪信息
//通信、用後定時（策略）
//Note：每个视频解码通道都会有此信息。
typedef struct _DP_DEV_VIDEODECODE_CUT_INFO_S {
	DP_U8 u8ImgCropOpen;		//图像裁剪开关0：关 1：开
	DP_S32 s32ImgCropRectX;		//图像裁剪矩阵区域起始坐标X
	DP_S32 s32ImgCropRectY;		//图像裁剪矩阵区域起始坐标Y
	DP_U32 u32ImgCropRectW;		//图像裁剪矩阵区域起始坐标宽度
	DP_U32 u32ImgCropRectH;		//图像裁剪矩阵区域起始坐标高度
} DP_DEV_VIDEODECODE_CUT_INFO_S;

//32.视频播放缩放信息
//通信、用後定時（策略）
//Note：每个视频解码通道都会有此信息。
typedef struct _DP_DEV_VIDEODECODE_ZOOM_INFO_S {
	DP_U8 u8ImgScaleOpen;			//图像缩放开关0：关 1：开
	DP_U32 u32ImgScaleW;			//图像缩放高度
	DP_U32 u32ImgScaleH;			//图像缩放宽度
} DP_DEV_VIDEODECODE_ZOOM_INFO_S;

//33.视频解码状态
//通信、用後定時（策略）
//Note：每个视频解码通道都会有此信息。
typedef struct _DP_DEV_VIDEODECODE_INFO_S {
	DP_U8 u8VdecChnId;			//解码通道ID

	DP_DEV_VIDEODECODE_BASE_INFO_S stBase;	//视频基础信息
	DP_DEV_VIDEODECODE_CUT_INFO_S stCut;	//视频播放裁剪信息
	DP_DEV_VIDEODECODE_ZOOM_INFO_S stZoom;	//视频播放缩放信息
} DP_DEV_VIDEODECODE_INFO_S;

//34.流媒体服务数量信息
//接口、初始
typedef struct _DP_DEV_SERVERNUM_INFO_S {
	DP_U16 u16RTSPServerNums;		//RTSP启动服务个数
	DP_U8 u8RTSPClientNums;			//RTSP客户服务个数
	DP_U16 u16RTMPServerNums;		//RTMP启动服务个数
	DP_U8 u8RTMPClientNums;		//RTMP客户服务个数
} DP_DEV_SERVERNUM_INFO_S;

//35.RTSP服务基本信息
//通信、用後定時（策略）
//Note：每个服务都会有此信息。
typedef struct _DP_DEV_RTSPSERVER_BASE_INFO_S {
	DP_U8 u8RTSPServerAVType;			//RTSP服务的音视频类型
										//0：音频、1：视频、2：音视频
	DP_U8 u8RTSPServerBindVencChnId;	//RTSP服务的音频编码通道ID
	DP_U8 u8RTSPServerBindAencChnId;	//RTSP服务的视频编码通道ID

	DP_U8 u8RTSPServerOpen;			//RTSP服务开关0：关 1：开
	DP_U8 au8RTSPServerURL[DP_URL_LEN];	//RTSP服务URL地址
										//(rtsp://172.16.10.228:1935/...)
	DP_U32 u32RTSPServerTimeout;			//RTSP服务超时
	DP_U8 u8RTSPServerMulticastOpen;		//RTSP服务组播开关0：单播 1：组播
	DP_U8 u8RTSPServerTransProt;			//RTSP服务传输协议类型0：TCP 1：UDP

	DP_U16 u16RTSPServerConnMax;		//RTSP服务最大支持的客户端连接数目
										//0：表示0个 1：表示1个
	DP_U16 u16RTSPServerConnNums;		//RTSP服务当前连接的客户端数目
										//0： 0个 1： 1个
} DP_DEV_RTSPSERVER_BASE_INFO_S;

//36.RTSP服务流信息
//通信、用後定時（策略）
//Note：每个服务都会有此信息。
typedef struct _DP_DEV_RTSPSERVER_VFLOW_INFO_S {
	DP_U16 u16ConnId;			//RTSP服务端连接ID
	DP_U32 u32LocalIPAddr;		//RTSP服务端本地IP地址
								//172.16.0.1表示为0xAC100001
	DP_U16 u16LocalPort;			//RTSP服务端本地端口
	DP_U32 u32PeerIPAddr;		//RTSP服务端连接的客户端IP地址
								//172.16.0.1表示为0xAC100001
	DP_U16 u16PeerPort;			//RTSP服务端连接的客户端端口
} DP_DEV_RTSPSERVER_VFLOW_INFO_S;

//37.RTSP服务信息
//通信、用後定時（策略）
//Note：每个服务都会有此信息。
typedef struct _DP_DEV_RTSPSERVER_INFO_S {
	DP_U16 u16RTSPServerTaskID;		//RTSP服务任务ID

	DP_DEV_RTSPSERVER_BASE_INFO_S stBase;		// RTSP服务基本信息
	DP_DEV_RTSPSERVER_VFLOW_INFO_S stVFlow[];	// RTSP服务流信息
} DP_DEV_RTSPSERVER_INFO_S;

//38.RTSP客户端基本信息
//通信、用後定時（策略）
//Note：每个客户端服务都会有此信息。
typedef struct _DP_DEV_RTSPCLIENT_BASE_INFO_S {
	DP_U8 u8RTSPClientAVType;			//音视频类型：0：音频、1：视频、2：音视频
	DP_U8 u8RTSPClientBindVdecChnId;		//RTSP客户的音频解码通道ID
	DP_U8 u8RTSPClientBindAdecChnId;		//RTSP客户的视频解码通道ID
	DP_U8 u8RTSPClientOpen;				//RTSP客户开关0：关 1：开
	DP_U32 u32RTSPClientTimeout;			//超时时间
	DP_U8 u8RTSPClientMulticastOpen;		//是否组播0：单播 1：组播
	DP_U8 u8RTSPClientTransPort;			//RTSP客户传输协议类型0：TCP 1：UDP
} DP_DEV_RTSPCLIENT_BASE_INFO_S;

//39.RTSP客户端流信息
//通信、用後定時（策略）
//Note：每个客户端服务都会有此信息。
typedef struct _DP_DEV_RTSPCLIENT_VFLOW_INFO_S {
	DP_U16 u16ConnId;			//RTSP服务端连接ID
	DP_U32 u32LocalIPAddr;		//RTSP服务端本地IP地址
								//172.16.0.1表示为0xAC100001
	DP_U16 u16LocalPort;			//RTSP服务端本地端口
	DP_U32 u32PeerIPAddr;		//RTSP服务端连接的客户端IP地址
								//172.16.0.1表示为0xAC100001
	DP_U16 u16PeerPort;			//RTSP服务端连接的客户端端口
} DP_DEV_RTSPCLIENT_VFLOW_INFO_S;

//40.RTSP客户端信息
//通信、用後定時（策略）
//Note：每个客户端服务都会有此信息。
typedef struct _DP_DEV_RTSPCLIENT_INFO_S {
	DP_U16 u16RTSPClientTaskID;		//RTSP客户任务ID

	DP_DEV_RTSPCLIENT_BASE_INFO_S stBase;	// RTSP客户端基本信息
	DP_DEV_RTSPCLIENT_VFLOW_INFO_S stVFlow;	// RTSP客户端流信息
} DP_DEV_RTSPCLIENT_INFO_S;

typedef struct _DP_DEV_MOUSEMATRIX_DEVICE_INFO_S {
	DP_U16 u16DeviceID;				//设备Sn
	DP_U32 u32IP;					//IP 172.16.0.1表示为0xAC100001
} DP_DEV_MOUSEMATRIX_DEVICE_INFO_S;

//接口、用後初始
//当前鼠标矩阵信息（仅输出节点）
typedef struct _DP_DEV_MOUSEMATRIX_INFO_S {
	DP_U8 u8IsEnable;				//是否启用0：不启用  1：启用
	DP_U32 u32MatrixID;				//矩阵ID
	DP_U8 au8ServerTaskID[DP_MATRIX_NAME_LEN];		//矩阵名
	DP_U8 u8AudioVideoType;			//行数0：表示0行
	DP_U8 u8AChannelID;			//列数0：表示0列

	DP_DEV_MOUSEMATRIX_DEVICE_INFO_S astDevice[];

	DP_U8 u8CoordinateSystem;		//坐标系0：相对坐标  1：绝对坐标
	DP_U16 u16XPosition;				//当前鼠标x坐标
	DP_U16 u16YPosition;				//当前鼠标y坐标
} DP_DEV_MOUSEMATRIX_INFO_S;

typedef struct _DP_DEV_LOGINUSER_USERPERMISSION_INFO_S {
	DP_U16 u16DeviceID;			//设备Sn
	DP_U8 u8Permission;			//权限	0：可独占
								//1：可完全控制
								//2：仅视频
								//3：禁止操作
} DP_DEV_LOGINUSER_USERPERMISSION_INFO_S;

//接口、用後初始
//当前登录用户信息（仅输出节点）
typedef struct _DP_DEV_LOGINUSER_INFO_S {
	DP_U8 au8UserName[DP_USER_NAME_LEN];		//用户名
	DP_U8 au8password[DP_USER_PASSWORD_LEN];	//密码
	DP_U8 u8Priority;			//用户优先级0：优先级最高 1：优先级低于0

	DP_DEV_LOGINUSER_USERPERMISSION_INFO_S astUserPermission[];
} DP_DEV_LOGINUSER_INFO_S;

#endif /* DEFINE_DATA_H_ */
