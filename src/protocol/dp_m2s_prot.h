/**
 * 文件: dp_m2s_prot.h
 * 简要: 编解码模块控制协议
 * 详述：
 该协议主要是用于（设备节点）编解码模块与服务逻辑模块之间的内部控制操作。
 * 作者：lyw
 * 版本：v01.00
 * 日期：2018/10/19
 */

#ifndef __DP_M2S_PROT_H__
#define __DP_M2S_PROT_H__
#include <string.h>
#include "./dp_sd_type.h"
#include "DevStaticConfigure.h"

#pragma pack(push)
#pragma pack(1)

/* 项目芯片类型定义 */
#define DP_M2S_CHIP IDMS4K_CHIP_HI3536

/* 协议版本号 */
#define DP_M2S_INF_PROT_VERSION			"00.01"
/* 协议通信接口定义 */
#define DP_M2S_INF_PROT_UNIX_FIFO		"/tmp/UNIX_AV2APP"
/* 协议固定包头标识 */
#define DP_M2S_INF_PROT_PKT_HEAD		0xF3F4
/* 协议固定包头长度 */
#define DP_M2S_INF_PROT_PKT_HEAD_LEN 	5
/* URL地址长度 */
#define DP_M2S_URL_LEN 128
/* 文字叠加长度（Bytes） */
#define DP_M2S_OSD_STRING_LEN 32 
/* 叠加图片路径长度（Bytes） */
#define DP_M2S_OSD_PIC_PATH_LEN 64

#if (DP_M2S_CHIP == IDMS4K_CHIP_HI3536)

#define DP_AI_DEV_MAX 2
#define DP_VI_DEV_MAX 2
#define DP_AENC_CHN_MAX 4
#define DP_VENC_CHN_MAX 4
#define DP_AO_DEV_MAX 2
#define DP_VO_DEV_MAX 2
#define DP_ADEC_CHN_MAX 32
#define DP_VDEC_CHN_MAX 32

#define DP_H264E_MIN_WIDTH 352
#define DP_H264E_MAX_WIDTH 3840
#define DP_H264E_MIN_HEIGHT 288
#define DP_H264E_MAX_HEIGHT 2160
#define DP_H264E_MIN_ALIGN 2

#define DP_H264D_MIN_WIDTH 352
#define DP_H264D_MAX_WIDTH 3840
#define DP_H264D_MIN_HEIGHT 288
#define DP_H264D_MAX_HEIGHT 2160
#define DP_H264D_MIN_ALIGN 2

#else
//...default（预留）
#endif

/* 音频采集设备最大数量 */
#define DP_M2S_AI_DEV_MAX DP_AI_DEV_MAX
/* 视频采集设备最大数量 */
#define DP_M2S_VI_DEV_MAX DP_VI_DEV_MAX
/* 音频编码通道最大数量 */
#define DP_M2S_AENC_CHN_MAX DP_AENC_CHN_MAX
/* 视频编码通道最大数量 */
#define DP_M2S_VENC_CHN_MAX DP_VENC_CHN_MAX

/* 音频输出设备最大数量 */
#define DP_M2S_AO_DEV_MAX DP_AO_DEV_MAX
/* 视频输出设备最大数量 */
#define DP_M2S_VO_DEV_MAX DP_VO_DEV_MAX
/* 支持音频解码最大数量*/
#define DP_M2S_ADEC_CHN_MAX DP_ADEC_CHN_MAX
/* 支持视频解码最大数量*/
#define DP_M2S_VDEC_CHN_MAX DP_VDEC_CHN_MAX

/* 一个视频输出设备的窗口拼接最大数量*/
#define DP_M2S_SWMS_MAX ((DP_M2S_VDEC_CHN_MAX)/(DP_M2S_VO_DEV_MAX))

/* H264编码宽高范围 以及 对齐*/
#define DP_M2S_H264E_MIN_WIDTH DP_H264E_MIN_WIDTH
#define DP_M2S_H264E_MAX_WIDTH DP_H264E_MAX_WIDTH
#define DP_M2S_H264E_MIN_HEIGHT DP_H264E_MIN_HEIGHT
#define DP_M2S_H264E_MAX_HEIGHT DP_H264E_MAX_HEIGHT
#define DP_M2S_H264E_MIN_ALIGN DP_H264E_MIN_ALIGN
/* H264解码宽高范围 以及 对齐*/
#define DP_M2S_H264D_MIN_WIDTH DP_H264D_MIN_WIDTH
#define DP_M2S_H264D_MAX_WIDTH DP_H264D_MAX_WIDTH
#define DP_M2S_H264D_MIN_HEIGHT DP_H264D_MIN_HEIGHT
#define DP_M2S_H264D_MAX_HEIGHT DP_H264D_MAX_HEIGHT
#define DP_M2S_H264D_MIN_ALIGN DP_H264D_MIN_ALIGN

/* RTSP服务端单个URL最大支持的连接 */
#define DP_M2S_RTSP_SERVER_CONNECT_MAX 128

/********************************任务ID分配 介绍************************/

/* 任务ID介绍
 * 使用对象：
 *		DP_M2S_INFO_TYPE_GET_AVENC	:  获取音视频编码通道信息，引用结构： DP_M2S_AVENC_GET_INFO_S
 *		DP_M2S_INFO_TYPE_SET_AVENC	:  设置音视频编码通道信息，引用结构： DP_M2S_AVENC_SET_INFO_S
 *		DP_M2S_INFO_TYPE_GET_AVDEC	:  获取音视频解码通道信息，引用结构： DP_M2S_AVDEC_GET_INFO_S
 *		DP_M2S_INFO_TYPE_SET_AVDEC	:  设置音视频解码通道信息，引用结构： DP_M2S_AVDEC_SET_INFO_S
 * 范围定义：
 * 	[0~255]，为音频任务ID，选中此ID范围时，仅可操作音频的相关属性；
 *	[256~511]，为视频任务ID，选中此ID范围时，仅可操作音频的相关属性；
 *	[512~767]，为音视频任务ID，选中此ID范围时，可操作音频和视频的相关属性；
 *	其他预留，无效；
 */
typedef DP_U32 DP_M2S_TSK_ID;

/********************************（编码/解码）资源关系绑定 ************************/
/*
 *说明： 音视频资源绑定关系类型
 *定义： DP_M2S_AVBIND_TYPE_E
 *成员：
 *	DP_M2S_AVBIND_AI2AO ： 音频输入绑到音频输出
 *	DP_M2S_AVBIND_AI2AENC ： 音频输入绑到音频编码
 *	DP_M2S_AVBIND_ADEC2AO ： 音频解码绑到音频输出
 *	DP_M2S_AVBIND_VI2VO ： 视频输入绑到视频输出
 *	DP_M2S_AVBIND_VI2VENC ： 视频输入绑到视频编码
 *	DP_M2S_AVBIND_VDEC2VO ： 视频解码绑到视频输出
 *	DP_M2S_AVBIND_AI2AENC_VI2VENC ： 音频输入绑到音频编码，同时，视频输入绑到视频编码
 *	DP_M2S_AVBIND_ADEC2AO_VDEC2VO ： 音频解码绑到音频输出，同时，视频解码绑到视频输出
 *注意：无


 */
typedef enum _DP_M2S_AVBIND_TYPE_E {

	/* 音频 */
	DP_M2S_AVBIND_AI2AO = 0x0, DP_M2S_AVBIND_AI2AENC, DP_M2S_AVBIND_ADEC2AO,

	/* 视频 */
	DP_M2S_AVBIND_VI2VO = 0x1000, DP_M2S_AVBIND_VI2VENC, DP_M2S_AVBIND_VDEC2VO,

	/* 音视频 */
	DP_M2S_AVBIND_AI2AENC_VI2VENC = 0x2000, DP_M2S_AVBIND_ADEC2AO_VDEC2VO,

	DP_M2S_AVBIND_BUTT
} DP_M2S_AVBIND_TYPE_E;

/*
 *说明： 音视频模块ID
 *定义： DP_M2S_MOD_ID_E
 *成员：
 *	DP_M2S_MOD_AI：音频输入模块
 *	DP_M2S_MOD_AO：音频输出模块
 *	DP_M2S_MOD_VI：视频输入模块
 *	DP_M2S_MOD_VO：视频输出模块
 *	DP_M2S_MOD_AENC：音频编码模块
 *	DP_M2S_MOD_VENC：视频编码模块
 *	DP_M2S_MOD_ADEC ：音频解码模块
 *	DP_M2S_MOD_VDEC ：视频解码模块
 *注意：无
 */
typedef enum _DP_M2S_MOD_ID_E {

	DP_M2S_MOD_AI = 0x0,
	DP_M2S_MOD_AO,
	DP_M2S_MOD_VI,
	DP_M2S_MOD_VO,
	DP_M2S_MOD_AENC,
	DP_M2S_MOD_VENC,
	DP_M2S_MOD_ADEC,
	DP_M2S_MOD_VDEC,
	DP_M2S_MOD_BUTT
} DP_M2S_MOD_ID_E;

/*
 *说明：资源配置
 *定义：DP_M2S_BIND_S
 *成员：
 *	u32ModId：模块号，引用 DP_M2S_MOD_ID_E
 *	u32DevId：设备号
 *		若配置模块ID为 DP_M2S_MOD_AI 时，设备号有效，通道号无效；设备范围[0,DP_M2S_AI_DEV_MAX)
 *		若配置模块ID为 DP_M2S_MOD_AO 时，设备号有效，通道号无效；设备范围[0,DP_M2S_AO_DEV_MAX)	
 *		若配置模块ID为 DP_M2S_MOD_VI 时，设备号有效，通道号无效；设备范围[0,DP_M2S_VI_DEV_MAX)
 *		若配置模块ID为 DP_M2S_MOD_VO 时，设备号有效，通道号无效；设备范围[0,DP_M2S_VO_DEV_MAX)
 *		若配置模块ID为 DP_M2S_MOD_AENC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_AENC_CHN_MAX)
 *		若配置模块ID为 DP_M2S_MOD_VENC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_VENC_CHN_MAX)
 *		若配置模块ID为 DP_M2S_MOD_ADEC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_ADEC_CHN_MAX)
 *		若配置模块ID为 DP_M2S_MOD_VDEC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_VDEC_CHN_MAX)
 *	u32ChnId：通道号
 *注意：无
 */
typedef struct _DP_M2S_SOURCE_RELATION_S {
	_DP_M2S_SOURCE_RELATION_S(DP_M2S_MOD_ID_E modId, DP_U32 devId, DP_U32 chnId) :
			ModId(modId), u32DevId(devId), u32ChnId(chnId) {
	}
	_DP_M2S_SOURCE_RELATION_S() :
			ModId(), u32DevId(), u32ChnId() {
	}
	DP_M2S_MOD_ID_E ModId;
	DP_U32 u32DevId;
	DP_U32 u32ChnId;
} DP_M2S_SOURCE_RELATION_S;

/*
 *说明： 资源关系
 *定义： DP_M2S_BIND_S
 *成员：
 *	stIn：输入资源配置
 *	stOut：输出资源配置
 *注意：无	 
 */
typedef struct _DP_M2S_BIND_S {
	_DP_M2S_BIND_S(DP_M2S_SOURCE_RELATION_S in, DP_M2S_SOURCE_RELATION_S out) :
			stIn(in), stOut(out) {
	}
	_DP_M2S_BIND_S() :
			stIn(), stOut() {
	}
	DP_M2S_SOURCE_RELATION_S stIn;
	DP_M2S_SOURCE_RELATION_S stOut;
} DP_M2S_BIND_S;

/*
 *说明： 音视频资源绑定属性
 *定义： DP_M2S_AVBIND_ATTR_S
 *成员：
 *	enBindType：音视频任务类型，引用 DP_M2S_AVTSK_TYPE_E
 *			当赋值为DP_M2S_AVBIND_AI2AO、DP_M2S_AVBIND_AI2AENC、DP_M2S_AVBIND_ADEC2AO时，仅需设置stAudio；
 *			当赋值为DP_M2S_AVBIND_VI2VO、DP_M2S_AVBIND_VI2VENC、DP_M2S_AVBIND_VDEC2VO时，仅需设置stVideo；
 *			当赋值为DP_M2S_AVBIND_AI2AENC_VI2VENC、DP_M2S_AVBIND_ADEC2AO_VDEC2VO时，需设置stAudio和stVideo；
 *	stAudio：音频方面的资源绑定关系，引用 DP_M2S_BIND_S
 *	stVideo：视频方面的资源绑定关系，引用 DP_M2S_BIND_S
 *注意：无
 */

typedef struct _DP_M2S_AVBIND_ATTR_S {
	_DP_M2S_AVBIND_ATTR_S(DP_M2S_AVBIND_TYPE_E bindType, DP_M2S_BIND_S audio,
			DP_M2S_BIND_S video) :
			enBindType(bindType), stAudio(audio), stVideo(video) {
	}
	_DP_M2S_AVBIND_ATTR_S() :
			enBindType(), stAudio(), stVideo() {
	}
	DP_M2S_AVBIND_TYPE_E enBindType;  //??
	DP_M2S_BIND_S stAudio;
	DP_M2S_BIND_S stVideo;
} DP_M2S_AVBIND_ATTR_S;

/**************************************************************************************************************/
/***************************************************控制指令操作类型 声明***********************************************/
/**************************************************************************************************************/
/*
 *说明：控制指令操作类型
 *定义：DP_M2S_CMD_ID_E
 *成员：
 *	DP_M2S_CMD_INIT_BEGIN	： 资源初始化开始
 *	DP_M2S_CMD_INIT			： 资源初始化设置
 *	DP_M2S_CMD_INIT_END		： 资源初始化结束
 *	DP_M2S_CMD_DEINIT_BEGIN	： 资源去初始化开始
 *	DP_M2S_CMD_DEINIT		： 资源去初始化设置
 *	DP_M2S_CMD_DEINIT_END	： 资源去初始化结束
 *	DP_M2S_CMD_GETINFO_BEGIN	： 信息获取开始
 *	DP_M2S_CMD_GETINFO			： 信息获取
 *	DP_M2S_CMD_GETINFO_END		： 信息获取结束
 *	DP_M2S_CMD_SETINFO_BEGIN	： 信息设置开始
 *	DP_M2S_CMD_SETINFO			： 信息设置
 *	DP_M2S_CMD_SETINFO_END		： 信息设置结束
 *	DP_M2S_CMD_EXIT	：程序退出
 *注意：
 *	须使用获取或设置指令前先初始化系统资源；
 */
//typedef enum _DP_M2S_CMD_ID_E {
//
//	DP_M2S_CMD_INIT_BEGIN = 0x0, DP_M2S_CMD_INIT, DP_M2S_CMD_INIT_END,
//
//	DP_M2S_CMD_DEINIT_BEGIN, DP_M2S_CMD_DEINIT, DP_M2S_CMD_DEINIT_END,
//
//	DP_M2S_CMD_GETINFO_BEGIN, DP_M2S_CMD_GETINFO, DP_M2S_CMD_GETINFO_END,
//
//	DP_M2S_CMD_SETINFO_BEGIN, DP_M2S_CMD_SETINFO, DP_M2S_CMD_SETINFO_END,
//
//	DP_M2S_CMD_EXIT, DP_M2S_CMD_BUTT
//} DP_M2S_CMD_ID_E;
typedef enum _DP_M2S_CMD_ID_E {

	DP_M2S_CMD_INIT = 0x0,

	DP_M2S_CMD_DEINIT,

	DP_M2S_CMD_GETINFO,

	DP_M2S_CMD_SETINFO,

	DP_M2S_CMD_EXIT, DP_M2S_CMD_BUTT
} DP_M2S_CMD_ID_E;
/**************************************************************************************************************/
/***************************************************公共数据结构 声明***********************************************/
/**************************************************************************************************************/
/*
 *说明： 与服务逻辑模块通信的协议包头
 *定义： DP_M2S_INF_PROT_HEAD_S
 *成员：
 *	u16HeadTag	：包头标识，引用DP_M2S_INF_PROT_UNIX_FIFO
 *	u16PacketLen：
 *		Bit[15]表示确认包返回标识，0表示不需要返回，1表示需要返回；
 *		Bit[14:0]表示数据包长度（固定包头长度+业务数据长度），范围[5~32767]Bytes；
 *	u8CommandID	：数据指令类型，引用 DP_M2S_CMD_ID_E
 *注意：无
 */
typedef struct _DP_M2S_INF_PROT_HEAD_S {
	_DP_M2S_INF_PROT_HEAD_S(DP_U16 packageLen, DP_M2S_CMD_ID_E cmdID,
			DP_U8 needReply) :
			u16HeadTag(DP_M2S_INF_PROT_PKT_HEAD), u16PacketLen(packageLen), u8CommandID(
					cmdID) {
		if (needReply == 0x00)
			u16PacketLen &= 0x7FFF;
		else if (needReply == 0x01)
			u16PacketLen |= 0x8000;
	}
	_DP_M2S_INF_PROT_HEAD_S() :
			u16HeadTag(DP_M2S_INF_PROT_PKT_HEAD), u16PacketLen(), u8CommandID() {

	}
	DP_U16 u16HeadTag;
	DP_U16 u16PacketLen;
	DP_U8 u8CommandID;
} DP_M2S_INF_PROT_HEAD_S;

/*
 *说明： 视频时序
 *定义： DP_M2S_VIDEO_SYNC_E
 *成员：
 *  DP_M2S_VIDEO_SYNC_720P60	：分辨率1280x720，帧率60
 *  DP_M2S_VIDEO_SYNC_1080P60	：分辨率1920x1080，帧率60
 *  DP_M2S_VIDEO_SYNC_4K30		：分辨率3840x2160，帧率30
 *注意：无
 */
typedef enum _DP_M2S_VIDEO_SYNC_E {

	DP_M2S_VIDEO_SYNC_720P60 = 0x0,
	DP_M2S_VIDEO_SYNC_1080P60,
	DP_M2S_VIDEO_SYNC_4K30,
	DP_M2S_VIDEO_SYNC_BUTT
} DP_M2S_VIDEO_SYNC_E;

/*
 *说明： 音频采样率
 *定义： DP_M2S_AUDIO_SAMPLE_RATE_E
 *成员：
 *  DP_M2S_AUDIO_SAMPLE_RATE_8000	：8K采样率
 *  DP_M2S_AUDIO_SAMPLE_RATE_16000	：16K采样率
 *  DP_M2S_AUDIO_SAMPLE_RATE_32000	：32K采样率
 *  DP_M2S_AUDIO_SAMPLE_RATE_44100	：44.1K采样率
 *  DP_M2S_AUDIO_SAMPLE_RATE_48000	：48K采样率
 *注意：无
 */
typedef enum _DP_M2S_AUDIO_SAMPLE_RATE_E {

	DP_M2S_AUDIO_SAMPLE_RATE_8000 = 8000,
	DP_M2S_AUDIO_SAMPLE_RATE_16000 = 16000,
	DP_M2S_AUDIO_SAMPLE_RATE_32000 = 32000,
	DP_M2S_AUDIO_SAMPLE_RATE_44100 = 44100,
	DP_M2S_AUDIO_SAMPLE_RATE_48000 = 48000,
	DP_M2S_AUDIO_SAMPLE_RATE_BUTT
} DP_M2S_AUDIO_SAMPLE_RATE_E;

/*
 *说明： 音频采样精度
 *定义： DP_M2S_AUDIO_BIT_WIDTH_E
 *成员：
 *	DP_M2S_AUDIO_BIT_WIDTH_8	：8位采样率精度
 *	DP_M2S_AUDIO_BIT_WIDTH_16	：16位采样率精度
 *注意：无
 */
typedef enum _DP_M2S_AUDIO_BIT_WIDTH_E {

	DP_M2S_AUDIO_BIT_WIDTH_8 = 0x0,
	DP_M2S_AUDIO_BIT_WIDTH_16 = 0x1,
	DP_M2S_AUDIO_BIT_WIDTH_BUTT
} DP_M2S_AUDIO_BIT_WIDTH_E;

/*
 *说明： 音频声道模式
 *定义： DP_M2S_AUDIO_BIT_WIDTH_E
 *成员：
 *	DP_M2S_AUDIO_SOUND_MODE_MONO	：单声道
 *	DP_M2S_AUDIO_SOUND_MODE_STEREO	：立体声
 *注意：无
 */
typedef enum _DP_M2S_AUDIO_SOUND_MODE_E {

	DP_M2S_AUDIO_SOUND_MODE_MONO = 0x0,
	DP_M2S_AUDIO_SOUND_MODE_STEREO = 0x1,
	DP_M2S_AUDIO_SOUND_MODE_BUTT
} DP_M2S_AUDIO_SOUND_MODE_E;

/*
 *说明： 音频公共属性
 *定义： DP_M2S_AUDIO_COMM_ATTR_S
 *成员：
 *	DP_M2S_AUDIO_SAMPLE_RATE_E	：音频采样率
 *	DP_M2S_AUDIO_BIT_WIDTH_E	：音频采样精度
 *	DP_M2S_AUDIO_SOUND_MODE_E	：音频声道模式
 *注意：无
 */
typedef struct _DP_M2S_AUDIO_COMM_ATTR_S {

	DP_M2S_AUDIO_SAMPLE_RATE_E enSamplerate;
	DP_M2S_AUDIO_BIT_WIDTH_E enBitwidth;
	DP_M2S_AUDIO_SOUND_MODE_E enSoundmode;
} DP_M2S_AUDIO_COMM_ATTR_S;

/*
 *说明： 坐标
 *定义： DP_M2S_POINT_S
 *成员：
 *	s32X 	：起始坐标x
 *	s32Y	：起始坐标y
 *注意：无
 */
typedef struct _DP_M2S_POINT_S {

	DP_S32 s32X;
	DP_S32 s32Y;
} DP_M2S_POINT_S;

/*
 *说明： 宽高
 *定义： DP_M2S_SIZE_S
 *成员：
 *	u32Width	： 图像宽度
 *	u32Height	： 图像高度
 *注意：无
 */
typedef struct _DP_M2S_SIZE_S {
	DP_U32 u32Width;
	DP_U32 u32Height;
} DP_M2S_SIZE_S;

/*
 *说明： 矩阵参数
 *定义： DP_M2S_RECT_S
 *成员：
 *	s32X		： 图像起始坐标x
 *	s32Y		： 图像起始坐标y
 *	u32Width	： 图像宽度
 *	u32Height	： 图像高度
 *注意：无
 */
//POD
typedef struct _DP_M2S_RECT_S {
//	_DP_M2S_RECT_S(DP_S32 x, DP_S32 y, DP_U32 width, DP_U32 height) :
//			s32X(x), s32Y(y), u32Width(width), u32Height(height) {
//	}
//	_DP_M2S_RECT_S() :
//			s32X(), s32Y(), u32Width(), u32Height() {
//	}
	DP_S32 s32X;
	DP_S32 s32Y;
	DP_U32 u32Width;
	DP_U32 u32Height;
} DP_M2S_RECT_S;

/*
 *说明： 图像裁剪属性
 *定义： DP_M2S_CROP_ATTR_S
 *成员： 引用 DP_M2S_RECT_S
 *注意：无
 */
typedef DP_M2S_RECT_S DP_M2S_CROP_ATTR_S;

/*
 *说明： 图像缩放类型
 *定义： DP_M2S_ZOOM_TYPE_E
 *成员：
 *	DP_M2S_ZOOM_RECT		： 按矩形方式缩放，对应结构 DP_M2S_RECT_S
 *	DP_M2S_ZOOM_RATIO		： 按比例方式缩放，对应结构 DP_M2S_ZOOM_RATIO_S
 *注意：无
 */
typedef enum _DP_M2S_ZOOM_TYPE_E {

	DP_M2S_ZOOM_RECT = 0x0, DP_M2S_ZOOM_RATIO, DP_M2S_ZOOM_BUTT
} DP_M2S_ZOOM_TYPE_E;

/*
 *说明： 比例方式缩放
 *定义： DP_M2S_ZOOM_RATIO_S
 *成员：
 *	u32XRatio 屏幕坐标上，待缩放区域起始点横坐标与显示通道宽的比例。
 *	u32YRatio 屏幕坐标上，待缩放区域起始点纵坐标与显示通道高的比例。
 *	u32WRatio 屏幕坐标上，待缩放区域宽与显示通道宽的比例。
 *	u32HRatio 屏幕坐标上，待缩放区域高与显示通道高的比例。
 *注意：无，
 */
//POD
typedef struct _DP_M2S_ZOOM_RATIO_S {

	DP_U32 u32XRatio;
	DP_U32 u32YRatio;
	DP_U32 u32WRatio;
	DP_U32 u32HRatio;
} DP_M2S_ZOOM_RATIO_S;

/*
 *说明： 图像缩放属性
 *定义： DP_M2S_ZOOM_ATTR_S
 *成员：
 *  enType	：  图像缩放类型，引用 DP_M2S_ZOOM_TYPE_E
 *  stRect  ：  按矩形方式缩放，对应结构 DP_M2S_RECT_S
 *  stRatio ：  按比例方式缩放，对应结构 DP_M2S_ZOOM_RATIO_S
 *注意：无
 */
//POD
typedef struct _DP_M2S_ZOOM_ATTR_S {

	DP_M2S_ZOOM_TYPE_E enType;
	union {
		DP_M2S_RECT_S stRect;
		DP_M2S_ZOOM_RATIO_S stRatio;
	};
} DP_M2S_ZOOM_ATTR_S;

/*
 *说明： 图像内容叠加类型
 *定义： DP_M2S_OSD_TYPE_E
 *成员：
 *	DP_M2S_OSD_PIC		： 图片
 *	DP_M2S_OSD_STRING	： 字符串文字
 *注意：无
 */
typedef enum _DP_M2S_OSD_TYPE_E {

	DP_M2S_OSD_PIC = 0x0, DP_M2S_OSD_STRING, DP_M2S_OSD_BUTT
} DP_M2S_OSD_TYPE_E;

/*
 *说明： 显示位置模式
 *定义： DP_M2S_OSD_DISPLAY_MODE_E
 *成员：
 *	DP_M2S_OSD_DISPLAY_MODE_COORD		： 坐标
 *	DP_M2S_OSD_DISPLAY_MODE_LTOP		： 左上
 *	DP_M2S_OSD_DISPLAY_MODE_RTOP		： 右上
 *	DP_M2S_OSD_DISPLAY_MODE_LBOTTOM		： 左下
 *	DP_M2S_OSD_DISPLAY_MODE_RBOTTOM		： 右下
 *注意：无
 */
typedef enum _DP_M2S_OSD_DISPLAY_MODE_E {

	DP_M2S_OSD_DISPLAY_MODE_COORD = 0x0,
	DP_M2S_OSD_DISPLAY_MODE_LTOP,
	DP_M2S_OSD_DISPLAY_MODE_RTOP,
	DP_M2S_OSD_DISPLAY_MODE_LBOTTOM,
	DP_M2S_OSD_DISPLAY_MODE_RBOTTOM,
	DP_M2S_OSD_DISPLAY_MODE_BUTT
} DP_M2S_OSD_DISPLAY_MODE_E;

/*
 *说明： 图像叠加属性
 *定义： DP_M2S_OSD_ATTR_S
 *成员：
 *	enType			： 图像内容叠加类型，引用 DP_M2S_OSD_TYPE_E
 *	stStr.au8Str	:  字符串文字内容，长度： DP_M2S_OSD_STRING_LEN
 *	stStr.u32Color	:  字符串文字颜色，支持RGB888颜色输入
 *	au8PicPath[]	:  图片路径，长度最大： DP_M2S_OSD_PIC_PATH_LEN
 *	enDispMode		:  显示位置模式，引用 DP_M2S_OSD_DISPLAY_MODE_E
 *	stPoint			:  坐标，引用 DP_M2S_POINT_S
 *注意：无
 */
typedef struct _DP_M2S_OSD_ATTR_S {

	DP_M2S_OSD_TYPE_E enType;
	union {
		struct {
			DP_U8 au8Str[DP_M2S_OSD_STRING_LEN];
			DP_U32 u32Color;
		} stStr;
		DP_U8 au8PicPath[DP_M2S_OSD_PIC_PATH_LEN];
	};
	DP_M2S_OSD_DISPLAY_MODE_E enDispMode;
	DP_M2S_POINT_S stPoint;
} DP_M2S_OSD_ATTR_S;

/*
 *说明： 编解码算法类型
 *定义： DP_M2S_ALG_TYPE_E
 *成员：
 *	DP_M2S_ALG_H264_ENC	：h264编码
 *	DP_M2S_ALG_H264_DEC	：h264解码
 *	DP_M2S_ALG_H265_ENC	：h265编码
 *	DP_M2S_ALG_H265_DEC	：h265解码
 *	DP_M2S_ALG_AAC_ENC	：aac编码
 *	DP_M2S_ALG_AAC_DEC	：aac解码
 *	DP_M2S_ALG_PCM_ENC	：pcm编码
 *	DP_M2S_ALG_PCM_DEC	：pcm解码
 *注意：无
 */
typedef enum _DP_M2S_ALG_TYPE_E {

	DP_M2S_ALG_H264_ENC = 0x0,
	DP_M2S_ALG_H264_DEC,
	DP_M2S_ALG_H265_ENC,
	DP_M2S_ALG_H265_DEC,
	DP_M2S_ALG_AAC_ENC,
	DP_M2S_ALG_AAC_DEC,
	DP_M2S_ALG_PCM_ENC,
	DP_M2S_ALG_PCM_DEC,
	DP_M2S_ALG_AAC_BUTT
} DP_M2S_ALG_TYPE_E;

/*
 *说明： 编码Profile类型
 *定义： DP_M2S_ALG_PROFILE_E
 *成员：
 *	DP_M2S_ALG_PROFILE_H264_BP	：H264 Baseline
 *	DP_M2S_ALG_PROFILE_H264_MP	：H264 Main Profile
 *	DP_M2S_ALG_PROFILE_H264_HP	: H264 High Profile
 *注意：无
 */
typedef enum _DP_M2S_ALG_PROFILE_E {

	DP_M2S_ALG_PROFILE_H264_BP = 0x0,
	DP_M2S_ALG_PROFILE_H264_MP,
	DP_M2S_ALG_PROFILE_H264_HP,
	DP_M2S_ALG_PROFILE_BUTT
} DP_M2S_ALG_PROFILE_E;

/*
 *说明： 码率模式
 *定义： DP_M2S_RC_MODE_E
 *成员：
 *	DP_M2S_RC_MODE_H264CBR		： h264固定码流模式
 *	DP_M2S_RC_MODE_H264VBR		： h264可变码流模式
 *注意：无
 */
typedef enum _DP_M2S_RC_MODE_E {

	DP_M2S_RC_MODE_H264CBR = 0x0, DP_M2S_RC_MODE_H264VBR, DP_M2S_RC_MODE_BUTT
} DP_M2S_RC_MODE_E;

/*
 *说明： h264编码属性
 *定义： DP_M2S_ALG_H264ENC_ATTR_S
 *成员：
 *   u32FrmRate : 编码帧率，范围(0,60]fps
 *   stSize     : 编码的图像宽高大小，引用 DP_M2S_SIZE_S
 *					宽度范围[DP_M2S_H264E_MIN_WIDTH, DP_M2S_H264E_MAX_WIDTH]
 *					高度范围[DP_M2S_H264E_MIN_HEIGHT, DP_M2S_H264E_MAX_HEIGHT]
 *   enRcMode   : 码率模式，引用 DP_M2S_RC_MODE_E
 *   u32Bitrate : 码率，范围256Kbps~40000Kbps
 *	 enProfile	：编码Profile类型，引用 DP_M2S_ALG_PROFILE_E
 *   u32Gop 	: GOP，范围0~120
 *   u16SF		: 预留
 *   u16TF 		: 预留
 *注意：无
 */
typedef struct _DP_M2S_ALG_H264ENC_ATTR_S {

	DP_U32 u32FrmRate;
	DP_M2S_SIZE_S stSize;
	DP_M2S_RC_MODE_E enRcMode;
	DP_U32 u32Bitrate;
	DP_M2S_ALG_PROFILE_E enProfile;
	DP_U32 u32Gop;
	DP_U16 u16SF;
	DP_U16 u16TF;
} DP_M2S_ALG_H264ENC_ATTR_S;

/*
 *说明： h264解码属性
 *定义： DP_M2S_ALG_H264DEC_ATTR_S
 *成员：
 *	enProfile	: 解码Profile，引用 DP_M2S_ALG_PROFILE_E
 *	stSize		: 解码图像最大宽高，引用 DP_M2S_SIZE_S
 *					宽度范围[DP_M2S_H264D_MIN_WIDTH, DP_M2S_H264D_MAX_WIDTH]
 *					高度范围[DP_M2S_H264D_MIN_HEIGHT, DP_M2S_H264D_MAX_HEIGHT]
 *注意：无
 */
typedef struct _DP_M2S_ALG_H264DEC_ATTR_S {

	DP_M2S_ALG_PROFILE_E enProfile;
	DP_M2S_SIZE_S stSize;
} DP_M2S_ALG_H264DEC_ATTR_S;

/*
 *说明： AAC编码属性
 *定义： DP_M2S_ALG_AACENC_ATTR_S
 *成员：
 *	u32Bitrate 	: 码率，范围32Kbps~192Kbps
 *	bAdts		: 是否支持ADTS，DP_FALSE：不支持，DP_TRUE：支持；
 *注意：无
 */
typedef struct _DP_M2S_ALG_AACENC_ATTR_S {

	DP_U32 u32Bitrate;
	DP_BOOL bAdts;
} DP_M2S_ALG_AACENC_ATTR_S;

/*
 *说明： AAC解码属性
 *定义： DP_M2S_ALG_AACDEC_ATTR_S
 *成员：
 *	bAdts : 是否支持ADTS，DP_FALSE：不支持，DP_TRUE：支持；
 *注意：无
 */
typedef struct _DP_M2S_ALG_AACDEC_ATTR_S {

	DP_BOOL bAdts;
} DP_M2S_ALG_AACDEC_ATTR_S;

/*
 *说明： 编解码算法属性
 *定义： DP_M2S_ALG_ATTR_S
 *成员：
 *	enAlg		：编解码算法类型，引用 DP_M2S_ALG_TYPE_E
 *	stH264Enc	：h264编码属性参数，引用 DP_M2S_ALG_H264ENC_ATTR_S
 *	stH264Dec	：h264解码属性参数，引用 DP_M2S_ALG_H264DEC_ATTR_S
 *注意：无
 */
typedef struct _DP_M2S_ALG_ATTR_S {

	DP_M2S_ALG_TYPE_E enAlg;
	union {
		DP_M2S_ALG_H264ENC_ATTR_S stH264Enc;
		DP_M2S_ALG_H264DEC_ATTR_S stH264Dec;
		DP_M2S_ALG_AACENC_ATTR_S stAACEnc;
		DP_M2S_ALG_AACDEC_ATTR_S stAACDec;
	};
} DP_M2S_ALG_ATTR_S;

/*
 *说明：	拼接属性
 *定义： DP_M2S_SWMS_ATTR_S
 *成员： 
 *	s32VoDevId	： 视频输出设备ID，范围[0,DP_M2S_VO_DEV_MAX)
 *	s32SwmsChn	： 拼接窗口通道，范围[0,DP_M2S_SWMS_MAX)
 *	u32Priority	： 窗口优先级；
 *	stRect		： 窗口位置显示参数，引用 DP_M2S_RECT_S
 *注意：无
 */
// priority max 64
typedef struct _DP_M2S_SWMS_ATTR_S {
	_DP_M2S_SWMS_ATTR_S(DP_S32 voDevId, DP_S32 swmsChn, DP_U32 priority,
			DP_M2S_RECT_S rect) :
			s32VoDevId(voDevId), s32SwmsChn(swmsChn), u32Priority(priority), stRect(
					rect) {
	}
	_DP_M2S_SWMS_ATTR_S() :
			s32VoDevId(), s32SwmsChn(), u32Priority(), stRect() {
	}
	DP_S32 s32VoDevId;
	DP_S32 s32SwmsChn;
	DP_U32 u32Priority;
	DP_M2S_RECT_S stRect;
} DP_M2S_SWMS_ATTR_S;

/*
 *说明： 输出图像效果
 *定义： DP_M2S_CSC_ATTR_S
 *成员：
 *	u8Sharpen		： 锐化度，范围[0,255]
 *	u8Luma			： 亮度，范围[0,100]
 *	u8Contrast		:  对比度，范围[0,100]
 *	u8Hue			:  色度，范围[0,100]
 *	u8Staturation	:  饱和度，范围[0,100]
 *注意：
 *	对应于 DP_M2S_INFO_TYPE_GET_AVENC ；
 */
typedef struct _DP_M2S_CSC_ATTR_S {
	_DP_M2S_CSC_ATTR_S() :
			u8Sharpen(), u8Luma(), u8Contrast(), u8Hue(), u8Staturation() {
	}
	DP_U8 u8Sharpen;
	DP_U8 u8Luma;
	DP_U8 u8Contrast;
	DP_U8 u8Hue;
	DP_U8 u8Staturation;
} DP_M2S_CSC_ATTR_S;

/*
 *说明： 视频编码属性
 *定义： DP_M2S_VENC_ATTR_S
 *成员：
 *  bCrop	：  裁剪开关，DP_TRUE：开启，stCrop有效；DP_FALSE：禁止，stCrop无效；
 *  bZoom	：  缩放开关，DP_TRUR：开启，stZoom有效，DP_FALSE：禁止，stZoom无效；
 *  bOsd	：  叠加开关，DP_TRUE：开启，stOsd有效，DP_FALSE：禁止，stOsd无效；
 *  stCrop	：  图像裁剪属性参数，引用 DP_M2S_CROP_ATTR_S
 *  stZoom	：  图像缩放属性参数，引用 DP_M2S_ZOOM_ATTR_S
 *  stOsd	：  图像叠加属性参数，引用 DP_M2S_OSD_ATTR_S
 *  stAlg	：  编解码算法属性参数，引用 DP_M2S_ALG_ATTR_S
 *注意：无
 */
typedef struct _DP_M2S_VENC_ATTR_S {
	_DP_M2S_VENC_ATTR_S(DP_BOOL crop, DP_BOOL zoom, DP_BOOL osd,
			DP_M2S_CROP_ATTR_S sCrop, DP_M2S_ZOOM_ATTR_S sZoom,
			DP_M2S_OSD_ATTR_S sOsd, DP_M2S_ALG_ATTR_S sAlg) :
			bCrop(crop), bZoom(zoom), bOsd(osd) {
		stCrop = sCrop;
		stZoom = sZoom;
		stOsd = sOsd;
		stAlg = sAlg;
	}
	_DP_M2S_VENC_ATTR_S() :
			bCrop(), bZoom(), bOsd() {
	}
	DP_BOOL bCrop;
	DP_BOOL bZoom;
	DP_BOOL bOsd;
	DP_M2S_CROP_ATTR_S stCrop;
	DP_M2S_ZOOM_ATTR_S stZoom;
	DP_M2S_OSD_ATTR_S stOsd;
	DP_M2S_ALG_ATTR_S stAlg;
} DP_M2S_VENC_ATTR_S;

/*
 *说明： 视频解码属性
 *定义： DP_M2S_VENC_ATTR_S
 *成员：
 *  bCrop	：  裁剪开关，DP_TRUE：开启，stCrop有效；DP_FALSE：禁止，stCrop无效；
 *  bZoom	：  缩放开关，DP_TRUR：开启，stZoom有效，DP_FALSE：禁止，stZoom无效；
 *  bOsd	：  叠加开关，DP_TRUE：开启，stOsd有效，DP_FALSE：禁止，stOsd无效；
 *  bSwms	：  窗口开关，DP_TRUE：开启，stSwms有效，DP_FALSE：禁止，stSwms无效；
 *  stAlg	：  编解码算法属性参数，引用 DP_M2S_ALG_ATTR_S
 *  stCrop	：  图像裁剪属性参数，引用 DP_M2S_CROP_ATTR_S
 *  stZoom	：  图像缩放属性参数，引用 DP_M2S_ZOOM_ATTR_S
 *  stOsd	：  图像叠加属性参数，引用 DP_M2S_OSD_ATTR_S
 *  stSwms  ：  图像拼接属性参数，引用 DP_M2S_SWMS_ATTR_S
 *注意： 无
 */
typedef struct _DP_M2S_VDEC_ATTR_S {
	_DP_M2S_VDEC_ATTR_S(DP_BOOL crop, DP_BOOL zoom, DP_BOOL osd, DP_BOOL swms,
			DP_M2S_ALG_ATTR_S sAlg, DP_M2S_CROP_ATTR_S sCrop,
			DP_M2S_ZOOM_ATTR_S sZoom, DP_M2S_OSD_ATTR_S sOsd,
			DP_M2S_SWMS_ATTR_S sSwms) :
			bCrop(crop), bZoom(zoom), bOsd(osd), bSwms(swms) {
		stAlg = sAlg;
		stCrop = sCrop;
		stZoom = sZoom;
		stOsd = sOsd;
		stSwms = sSwms;
	}
	_DP_M2S_VDEC_ATTR_S() :
			bCrop(), bZoom(), bOsd(), bSwms(), stAlg(), stCrop(), stZoom(), stOsd(), stSwms() {
	}
	DP_BOOL bCrop;
	DP_BOOL bZoom;
	DP_BOOL bOsd;
	DP_BOOL bSwms;
	DP_M2S_ALG_ATTR_S stAlg;
	DP_M2S_CROP_ATTR_S stCrop;
	DP_M2S_ZOOM_ATTR_S stZoom;
	DP_M2S_OSD_ATTR_S stOsd;
	DP_M2S_SWMS_ATTR_S stSwms;
} DP_M2S_VDEC_ATTR_S;

/*
 *说明： 音频编码属性
 *定义： DP_M2S_VENC_ATTR_S
 *成员：
 *	enAlg		: 编解码算法类型，引用 DP_M2S_ALG_TYPE_E
 *	s32Bitrate	：编码码率，范围 32~192Kbps；
 *注意：无
 */
typedef struct _DP_M2S_AENC_ATTR_S {
	_DP_M2S_AENC_ATTR_S(DP_M2S_ALG_TYPE_E alg, DP_S32 bitrate) :
			enAlg(alg), s32Bitrate(bitrate) {
	}
	_DP_M2S_AENC_ATTR_S() :
			enAlg(), s32Bitrate() {
	}
	DP_M2S_ALG_TYPE_E enAlg;
	DP_S32 s32Bitrate;
} DP_M2S_AENC_ATTR_S;

/*
 *说明： 音频解码属性
 *定义： DP_M2S_ADEC_ATTR_S
 *成员：
 *	enAlg : 编解码算法类型，引用 DP_M2S_ALG_TYPE_E
 *注意：无
 */
typedef struct _DP_M2S_ADEC_ATTR_S {
	_DP_M2S_ADEC_ATTR_S(DP_M2S_ALG_TYPE_E alg) :
			enAlg(alg) {
	}
	_DP_M2S_ADEC_ATTR_S() :
			enAlg() {
	}
	DP_M2S_ALG_TYPE_E enAlg;
} DP_M2S_ADEC_ATTR_S;

/*
 *说明： 流媒体类型
 *定义： DP_M2S_STREAM_TYPE_E
 *成员：
 *	DP_M2S_STREAM_RTSP_SERVER	：rtsp服务端类型
 *	DP_M2S_STREAM_RTSP_CLIENT	：rtsp客户端类型
 *	DP_M2S_STREAM_RTMP_SERVER	：rtsp客户端类型 预留
 *	DP_M2S_STREAM_RTMP_CLIENT	：rtsp客户端类型 预留
 *注意：无
 */
typedef enum _DP_M2S_STREAM_TYPE_E {

	DP_M2S_STREAM_RTSP_SERVER = 0x0, DP_M2S_STREAM_RTSP_CLIENT,
	// DP_M2S_STREAM_RTMP_SERVER,
	// DP_M2S_STREAM_RTMP_CLIENT,
	DP_M2S_STREAM_BUTT
} DP_M2S_STREAM_TYPE_E;

/*
 *说明： RTSP服务端属性
 *定义： DP_M2S_RTSP_SERVER_ATTR_S
 *成员：
 *  bOpen			： 服务端开关，DP_FALSE：关，DP_TRUE：开；
 *  bMulticast		:  是否支持组播，DP_FALSE：单播，DP_TRUE：组播
 *  s32TransType	:  传输类型，0：tcp，1：udp
 *  s32ConnTimeout	:  连接超时，范围0~300秒
 *  s32ConnMax		： 服务端可支持的最大连接数目，范围[1,DP_M2S_RTSP_SERVER_CONNECT_MAX]
 *  s32ConnNums		： 服务端当前连接数目[0,DP_M2S_RTSP_SERVER_CONNECT_MAX];
 *  au8Url[]		:  连接地址url，长度范围[1,DP_M2S_URL_LEN];
 *注意：无
 */
//POD
typedef struct _DP_M2S_RTSP_SERVER_ATTR_S {
	DP_BOOL bOpen;
	DP_BOOL bMulticast;
	DP_S32 s32TransType;
	DP_S32 s32ConnTimeout;
	DP_S32 s32ConnMax;
	DP_S32 s32ConnNums;
	DP_U8 au8Url[DP_M2S_URL_LEN];
} DP_M2S_RTSP_SERVER_ATTR_S;

/*
 *说明： RTSP客户端属性
 *定义： DP_M2S_RTSP_CLIENT_ATTR_S
 *成员：
 *  s8Open			： 客户端开关，0：关，1：开（仅接收音频）；2：开（仅接收视频）；3：开（接收音视频）
 *  bMulticast		:  是否支持组播，0：单播，1：组播
 *  s32TransType	:  传输类型，0：tcp，1：udp
 *  s32ConnTimeout	:  连接超时，范围0~300秒
 *  au8Url[]		:  连接地址url，长度范围[1,DP_M2S_URL_LEN];
 *注意：无
 */
//POD
typedef struct _DP_M2S_RTSP_CLIENT_ATTR_S {
	DP_S8 s8Open;
	DP_BOOL bMulticast;
	DP_S32 s32TransType;
	DP_S32 s32ConnTimeout;
	DP_U8 au8Url[DP_M2S_URL_LEN];
} DP_M2S_RTSP_CLIENT_ATTR_S;

/*
 *说明： 流媒体属性
 *定义： DP_M2S_RTSP_CLIENT_ATTR_S
 *成员：
 *	stRtspServer ： RTSP服务端属性
 *	stRtspClient ： RTSP客户端属性
 *注意：无
 */
#if 1
typedef struct _DP_M2S_STREAM_ATTR_S {
	_DP_M2S_STREAM_ATTR_S(DP_M2S_STREAM_TYPE_E type,
			DP_M2S_RTSP_SERVER_ATTR_S rtspServer) :
			enType(type) {
		_rtsp.stRtspServer = rtspServer;
	}
	_DP_M2S_STREAM_ATTR_S(DP_M2S_STREAM_TYPE_E type,
			DP_M2S_RTSP_CLIENT_ATTR_S rtspClient) :
			enType(type) {
		_rtsp.stRtspClient = rtspClient;
	}
	_DP_M2S_STREAM_ATTR_S() :
			enType() {
	}
	DP_M2S_STREAM_TYPE_E enType;
	union rtsp {
		DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
		DP_M2S_RTSP_CLIENT_ATTR_S stRtspClient;
	} _rtsp;
} DP_M2S_STREAM_ATTR_S;
#endif

/**************************************************************************************************************/
/**********************************************信息类型以及结构声明********************************************/
/**************************************************************************************************************/
/*
 *说明：信息类型
 *定义：DP_M2S_INFO_TYPE_E
 *成员：
 *	DP_M2S_INFO_TYPE_GET_AI		:  获取音频输入设备信息，引用结构： DP_M2S_AI_GET_INFO_S
 *	DP_M2S_INFO_TYPE_SET_AI		:  设置音频输入设备信息，引用结构： DP_M2S_AI_SET_INFO_S
 *	DP_M2S_INFO_TYPE_GET_VI		:  获取视频输入设备信息，引用结构： DP_M2S_VI_GET_INFO_S
 *	DP_M2S_INFO_TYPE_SET_VI		:  设置视频输入设备信息，引用结构： DP_M2S_VI_SET_INFO_S
 *	DP_M2S_INFO_TYPE_GET_AVENC	:  获取音视频编码通道信息，引用结构： DP_M2S_AVENC_GET_INFO_S
 *	DP_M2S_INFO_TYPE_SET_AVENC	:  设置音视频编码通道信息，引用结构： DP_M2S_AVENC_SET_INFO_S
 *	DP_M2S_INFO_TYPE_GET_AVDEC	:  获取音视频解码通道信息，引用结构： DP_M2S_AVDEC_GET_INFO_S
 *	DP_M2S_INFO_TYPE_SET_AVDEC	:  设置音视频解码通道信息，引用结构： DP_M2S_AVDEC_SET_INFO_S
 *	DP_M2S_INFO_TYPE_GET_AO		： 获取音频输出信息，引用结构： DP_M2S_AO_GET_INFO_S
 *	DP_M2S_INFO_TYPE_SET_AO		： 设置音频输出信息，引用结构： DP_M2S_AO_SET_INFO_S
 *	DP_M2S_INFO_TYPE_GET_VO		： 获取视频输出信息，引用结构： DP_M2S_VO_GET_INFO_S
 *	DP_M2S_INFO_TYPE_SET_VO		： 设置视频输出信息，引用结构： DP_M2S_VO_SET_INFO_S	
 *注意：无
 */
typedef enum _DP_M2S_INFO_TYPE_E {

	DP_M2S_INFO_TYPE_GET_AI = 0x0,
	DP_M2S_INFO_TYPE_SET_AI,
	DP_M2S_INFO_TYPE_GET_VI,
	DP_M2S_INFO_TYPE_SET_VI,
	// DP_M2S_INFO_TYPE_GET_AENC,
	// DP_M2S_INFO_TYPE_SET_AENC,
	// DP_M2S_INFO_TYPE_GET_ADEC,
	// DP_M2S_INFO_TYPE_SET_ADEC,
	DP_M2S_INFO_TYPE_GET_AVENC,
	DP_M2S_INFO_TYPE_SET_AVENC,

	DP_M2S_INFO_TYPE_GET_AVDEC,
	DP_M2S_INFO_TYPE_SET_AVDEC,

	DP_M2S_INFO_TYPE_GET_AO,
	DP_M2S_INFO_TYPE_SET_AO,
	DP_M2S_INFO_TYPE_GET_VO,
	DP_M2S_INFO_TYPE_SET_VO,

	DP_M2S_INFO_TYPE_BUTT
} DP_M2S_INFO_TYPE_E;

/*
 *说明： 获取音频输入设备信息
 *定义： DP_M2S_AI_GET_INFO_S
 *成员：
 *	s32DevId		：音频输入设备ID，范围[0,DP_M2S_AI_DEV_MAX)
 *	bConn			：音频输入设备连接状态，DP_FALSE，断开；DP_TRUE，已连接；
 *	bSignal			：音频输入设备信号状态，DP_FALSE，无信号；DP_TRUE，有信号；
 *	u8Vol			：音频输入设备音量0-100	
 *	bSelfAdaptive	：音频输入自适应，DP_FALSE，非自适应；DP_TRUE，自适应；
 *	stCommAttr		：音频输入设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S
 *注意：
 对应于 DP_M2S_INFO_TYPE_GET_AI ；
 */
typedef struct _DP_M2S_AI_GET_INFO_S {

	DP_U32 u32DevId;
	DP_BOOL bsConn;
	DP_BOOL bSignal;
	DP_U8 u8Vol;
	DP_BOOL bSelfAdaptive;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
} DP_M2S_AI_GET_INFO_S;

/*
 *说明： 设置音频输入设备信息
 *定义： DP_M2S_AI_SET_INFO_S
 *成员：
 *	s32DevId		：音频输入设备ID，范围[0,DP_M2S_AI_DEV_MAX)
 *	u8Vol			：音频输入设备音量0-100	
 *	bSelfAdaptive	：音频输入自适应
 *	stCommAttr		：音频输入设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S
 *注意：
 *	对应于 DP_M2S_INFO_TYPE_SET_AI ；
 */
typedef struct _DP_M2S_AI_SET_INFO_S {

	DP_U32 s32DevId;
	DP_U8 u8Vol;
	DP_BOOL bSelfAdaptive;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
} DP_M2S_AI_SET_INFO_S;

/*
 *说明： 获取视频输入设备信息
 *定义： DP_M2S_VI_GET_INFO_S
 *成员：
 *	s32DevId	： 视频输入设备ID，范围[0,DP_M2S_VI_DEV_MAX)，其他无效
 *	bConn		： 视频输入设备是否连接，DP_TRUE：连接；DP_FALSE：断开
 *	bSignal		： 视频输入设备信号是否正常，DP_TRUE：有信号，DP_FALSE：无信号
 *	s32FrmRate	： 目标帧率，范围0~60
 *	stCap		:  原始图像坐标和宽高，引用 DP_M2S_RECT_S
 *	stDst		:  目标图像大小，引用 DP_M2S_SIZE_S
 *注意：
 *	对应于 DP_M2S_INFO_TYPE_GET_VI ；
 */
typedef struct _DP_M2S_VI_GET_INFO_S {

	DP_S32 s32DevId;
	DP_BOOL bConn;
	DP_BOOL bSignal;
	DP_S32 s32FrmRate;
	DP_M2S_RECT_S stCap;
	DP_M2S_SIZE_S stDst;
} DP_M2S_VI_GET_INFO_S;

/*
 *说明： 设置视频输入设备信息
 *定义： DP_M2S_VI_SET_INFO_S
 *成员：
 *	s32DevId	： 视频输入设备ID，范围[0,DP_M2S_VI_DEV_MAX)，其他无效
 *	s32FrmRate	： 目标帧率，范围0~60
 *	stCap		:  原始图像坐标和宽高，引用 DP_M2S_RECT_S
 *	stDst		:  目标图像大小，引用 DP_M2S_SIZE_S
 *注意：
 *	对应于 DP_M2S_INFO_TYPE_SET_VI ；
 */
typedef struct _DP_M2S_VI_SET_INFO_S {

	DP_S32 s32DevId;
	DP_S32 s32FrmRate;
	DP_M2S_RECT_S stCap;
	DP_M2S_SIZE_S stDst;
} DP_M2S_VI_SET_INFO_S;

/*
 *说明： 获取音视频编码通道信息
 *定义： DP_M2S_AVENC_GET_INFO_S
 *成员：
 *  TskId    	:  任务ID，引用 DP_M2S_TSK_ID
 *  AvBindAttr	：音视频资源绑定属性，引用 DP_M2S_AVBIND_ATTR_S
 *  stAenc		：音频编码属性参数，引用 DP_M2S_AENC_ATTR_S
 *  stVenc		：视频编码属性参数，引用 DP_M2S_VENC_ATTR_S
 *  stStream	：流媒体属性参数，引用 DP_M2S_STREAM_ATTR_S
 *注意：  
 对应于 DP_M2S_INFO_TYPE_GET_AENC ；
 */
typedef struct _DP_M2S_AVENC_GET_INFO_S {
	_DP_M2S_AVENC_GET_INFO_S(DP_M2S_TSK_ID tskId,
			DP_M2S_AVBIND_ATTR_S avBindAttr, DP_M2S_AENC_ATTR_S aenc,
			DP_M2S_VENC_ATTR_S venc, DP_M2S_STREAM_TYPE_E type,
			DP_M2S_RTSP_CLIENT_ATTR_S rtspClient) :
			TskId(tskId), AvBindAttr(avBindAttr), stAenc(aenc), stVenc(venc), stStream(
					type, rtspClient) {
	}
	_DP_M2S_AVENC_GET_INFO_S(DP_M2S_TSK_ID tskId,
			DP_M2S_AVBIND_ATTR_S avBindAttr, DP_M2S_AENC_ATTR_S aenc,
			DP_M2S_VENC_ATTR_S venc, DP_M2S_STREAM_TYPE_E type,
			DP_M2S_RTSP_SERVER_ATTR_S rtspServer) :
			TskId(tskId), AvBindAttr(avBindAttr), stAenc(aenc), stVenc(venc), stStream(
					type, rtspServer) {
	}
	_DP_M2S_AVENC_GET_INFO_S() :
			TskId(), AvBindAttr(), stAenc(), stVenc(), stStream() {
	}
	DP_M2S_TSK_ID TskId;
	DP_M2S_AVBIND_ATTR_S AvBindAttr;
	DP_M2S_AENC_ATTR_S stAenc;
	DP_M2S_VENC_ATTR_S stVenc;
	DP_M2S_STREAM_ATTR_S stStream;
} DP_M2S_AVENC_GET_INFO_S;

// 说明： 设置音视频编码通道信息
typedef DP_M2S_AVENC_GET_INFO_S DP_M2S_AVENC_SET_INFO_S;

/*
 *说明： 获取音视频解码通道信息
 *定义： DP_M2S_AVDEC_GET_INFO_S
 *成员：
 *  TskId    	:  任务ID，引用 DP_M2S_TSK_ID
 *  AvBindAttr	： 音视频资源绑定属性，引用 DP_M2S_AVBIND_ATTR_S
 *	stStream	： 流媒体属性参数，引用 DP_M2S_STREAM_ATTR_S
 *	stAdec		:  音频解码属性参数，引用 DP_M2S_ADEC_ATTR_S
 *	stVdec		:  视频解码属性参数，引用 DP_M2S_VDEC_ATTR_S
 *注意：
 *	对应于 DP_M2S_INFO_TYPE_GET_AVENC ；
 */
typedef struct _DP_M2S_AVDEC_GET_INFO_S {
	_DP_M2S_AVDEC_GET_INFO_S(DP_M2S_TSK_ID tskId,
			DP_M2S_AVBIND_ATTR_S avBindAttr, DP_M2S_STREAM_ATTR_S stream,
			DP_M2S_ADEC_ATTR_S adec, DP_M2S_VDEC_ATTR_S vdec) :
			TskId(tskId), AvBindAttr(avBindAttr), stStream(stream), stAdec(
					adec), stVdec(vdec) {
	}
	_DP_M2S_AVDEC_GET_INFO_S() :
			TskId(), AvBindAttr(), stStream(), stAdec(), stVdec() {
	}
	DP_M2S_TSK_ID TskId;
	DP_M2S_AVBIND_ATTR_S AvBindAttr;
	DP_M2S_STREAM_ATTR_S stStream;
	DP_M2S_ADEC_ATTR_S stAdec;
	DP_M2S_VDEC_ATTR_S stVdec;
} DP_M2S_AVDEC_GET_INFO_S;

// 说明： 设置音视频解码通道信息
typedef DP_M2S_AVDEC_GET_INFO_S DP_M2S_AVDEC_SET_INFO_S;

/*
 *说明： 获取音频输出信息
 *定义： DP_M2S_AO_GET_INFO_S
 *成员：
 *	s32DevId	： 音频输出设备ID，范围[0, DP_M2S_AO_DEV_MAX )，其他无效
 *	u8Vol		：音频输出设备音量0-100	
 *	stCommAttr	：音频输出设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S
 *注意：
 *	对应于 DP_M2S_INFO_TYPE_GET_AO ；
 */
typedef struct _DP_M2S_AO_GET_INFO_S {
	_DP_M2S_AO_GET_INFO_S(DP_S32 devId, DP_M2S_AUDIO_COMM_ATTR_S commAttr) :
			s32DevId(devId), u8Vol(), stCommAttr(commAttr) {
	}
	DP_S32 s32DevId;
	DP_U8 u8Vol;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
} DP_M2S_AO_GET_INFO_S;

// 说明： 获取音频输出信息
typedef DP_M2S_AO_GET_INFO_S DP_M2S_AO_SET_INFO_S;

/*
 *说明： 获取视频输出信息
 *定义： DP_M2S_VO_GET_INFO_S
 *成员：
 *	s32DevId： 视频输出设备ID，范围[0,DP_M2S_VO_DEV_MAX)，其他无效
 *	bEnable	： 视频输出使能， DP_FALSE : 禁止； DP_TRUE ：启用；
 *	enSync	:  视频输出时序，引用 DP_M2S_VIDEO_SYNC_E
 *	stSwms	:  视频输出的拼接窗口参数，引用 DP_M2S_SWMS_ATTR_S
 *	stCsc	： 视频输出图像效果参数，引用 DP_M2S_CSC_ATTR_S
 *注意：
 *	1.对应于 DP_M2S_INFO_TYPE_GET_VO ；
 *	2.参数s32DevId和参数stSwms[].s32VoDevId一样；
 */
typedef struct _DP_M2S_VO_GET_INFO_S {
	_DP_M2S_VO_GET_INFO_S(DP_S32 devId, DP_BOOL enable,
			DP_M2S_VIDEO_SYNC_E sync, DP_M2S_SWMS_ATTR_S *swms, DP_U32 swmNum,
			DP_M2S_CSC_ATTR_S csc) :
			s32DevId(devId), bEnable(enable), enSync(sync), stCsc(csc) {
		memcpy(stSwms, swms, swmNum * sizeof(DP_M2S_SWMS_ATTR_S));
	}
	_DP_M2S_VO_GET_INFO_S() :
			s32DevId(), bEnable(), enSync() {
		memset(stSwms, 0, DP_M2S_SWMS_MAX * sizeof(DP_M2S_SWMS_ATTR_S));
	}
	DP_S32 s32DevId;
	DP_BOOL bEnable;
	DP_M2S_VIDEO_SYNC_E enSync;
	DP_M2S_SWMS_ATTR_S stSwms[DP_M2S_SWMS_MAX];
	DP_M2S_CSC_ATTR_S stCsc;
} DP_M2S_VO_GET_INFO_S;

// 说明： 设置视频输出信息
typedef DP_M2S_VO_GET_INFO_S DP_M2S_VO_SET_INFO_S;

/**************************************************************************************************************/
/***************************************************控制指令参数结构 声明***********************************************/
/**************************************************************************************************************/
/*
 *说明：通用的返回结果的结构参数
 *定义：DP_M2S_CMD_ACK_S
 *成员：
 *	u32Success：0:成功，其他失败，返回错误码
 *注意：无
 */
typedef struct _DP_M2S_CMD_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
} DP_M2S_CMD_ACK_S;

/*************************************1.2 DP_M2S_CMD_INIT*************************/
/*
 *说明： 资源初始化设置
 *定义： DP_M2S_CMD_INIT_S
 *成员：
 *	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_INIT ；
 *	enInfoTYpe：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_INIT_S {
#if (OutputDevice)
	_DP_M2S_CMD_INIT_S() :
			stHeader(sizeof(struct _DP_M2S_CMD_INIT_S), DP_M2S_CMD_INIT, 0x01), enInfoTYpe(
					DP_M2S_INFO_TYPE_SET_AVDEC) {
	}
#endif
#if (InputDevice)
	_DP_M2S_CMD_INIT_S() :
	stHeader(sizeof(struct _DP_M2S_CMD_INIT_S), DP_M2S_CMD_INIT, 0x01), enInfoTYpe(
			DP_M2S_INFO_TYPE_SET_AVENC) {
	}
#endif
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;

} DP_M2S_CMD_INIT_S;

//“资源初始化设置”应答
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_INIT_RESPOND_S;

/*************************************2.2 DP_M2S_CMD_DEINIT*************************/
/*
 *说明： 资源去初始化设置
 *定义： DP_M2S_CMD_DEINIT_S
 *成员：
 *	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_DEINIT ；
 *	enInfoTYpe：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_DEINIT_S {
#if (OutputDevice)
	_DP_M2S_CMD_DEINIT_S() :
			stHeader(sizeof(struct _DP_M2S_CMD_DEINIT_S), DP_M2S_CMD_DEINIT,
					0x01), enInfoTYpe(DP_M2S_INFO_TYPE_SET_AVDEC) {
	}
#endif
#if (InputDevice)
	_DP_M2S_CMD_DEINIT_S() :
	stHeader(sizeof(struct _DP_M2S_CMD_DEINIT_S), DP_M2S_CMD_DEINIT,
			0x01), enInfoTYpe(DP_M2S_INFO_TYPE_SET_AVENC) {
	}
#endif
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;
} DP_M2S_CMD_DEINIT_S;

//“资源去初始化设置” 应答
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_DEINIT_RESPOND_S;

/*************************************3.2 DP_M2S_CMD_GETINFO*************************/
/*
 *说明： 获取信息指令 
 *定义： DP_M2S_CMD_GETINFO_S
 *成员：
 *	stHeader：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_GETINFO ；
 *	enInfoTYpe：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_GETINFO_S {
	_DP_M2S_CMD_GETINFO_S(DP_U16 packageLen, DP_M2S_CMD_ID_E cmdID,
			DP_U8 needReply, DP_M2S_INFO_TYPE_E infoTYpe) :
			stHeader(packageLen, cmdID, needReply), enInfoTYpe(infoTYpe) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;

} DP_M2S_CMD_GETINFO_S;

/*
 *说明：获取信息指令 应答 
 *定义：DP_M2S_CMD_GETINFO_S
 *成员：
 *	stHeader	：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_GETINFO ；
 *	enInfoTYpe	：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
 *	u32Success	：0:成功，其他失败，返回错误码
 *	u32InfoLen	: 对应enInfoTYpe的信息数据长度；
 *	pu8InfoData	: 对应enInfoTYpe的信息数据结构指针；
 *注意：无
 */
typedef struct _DP_M2S_CMD_GETINFO_RESPOND_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;
	DP_U32 u32Success;
	DP_U32 u32InfoLen;
	DP_U8 *pu8InfoData;
} DP_M2S_CMD_GETINFO_RESPOND_S;

/*************************************4.2 DP_M2S_CMD_SETINFO*************************/
/*
 *说明： 设置信息指令结构体
 *定义： DP_M2S_CMD_SETINFO_S
 *成员：
 *	stHeader	：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_SETINFO ；
 *	enInfoTYpe	：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
 *	u32InfoLen	: 对应enInfoTYpe的信息数据长度；
 *	pu8InfoData	: 对应enInfoTYpe的信息数据结构指针；
 *注意：无
 */
typedef struct _DP_M2S_CMD_SETINFO_S {
	_DP_M2S_CMD_SETINFO_S(DP_U16 packageLen, DP_M2S_CMD_ID_E cmdID,
			DP_U8 needReply, DP_M2S_INFO_TYPE_E infoType, DP_U32 infoLen) :
			stHeader(packageLen, cmdID, needReply), enInfoTYpe(infoType), u32InfoLen(
					infoLen), pu8InfoData() {
	}
	_DP_M2S_CMD_SETINFO_S(DP_U16 packageLen, DP_M2S_CMD_ID_E cmdID,
			DP_U8 needReply, DP_M2S_INFO_TYPE_E infoType, DP_U32 infoLen,
			DP_U8 *infoData) :
			stHeader(packageLen, cmdID, needReply), enInfoTYpe(infoType), u32InfoLen(
					infoLen), pu8InfoData(infoData) {
	}
	_DP_M2S_CMD_SETINFO_S() :
			stHeader(), enInfoTYpe(), u32InfoLen(), pu8InfoData() {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;
	DP_U32 u32InfoLen;
	DP_U8 *pu8InfoData;
} DP_M2S_CMD_SETINFO_S;

//“设置信息指令” 应答
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_SETINFO_RESPOND_S;

/*************************************5. DP_M2S_CMD_EXIT*************************/
/*
 *说明：程序退出
 *定义：DP_M2S_CMD_EXIT_INFO_S
 *成员：
 *	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_EXIT ；
 *注意：无
 */
typedef DP_M2S_INF_PROT_HEAD_S DP_M2S_CMD_EXIT_S;

//“程序退出”应答
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_EXIT_RESPOND_S;

#pragma pack(pop)

#endif 
