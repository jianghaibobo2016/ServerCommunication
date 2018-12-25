/**
 * 文件: dp_m2s_prot.h
 * 简要: 编解码模块控制协议
 * 详述：
 该协议主要是用于（设备节点）编解码模块与服务逻辑模块之间的内部控制操作。
 * 作者：lyw
 * 版本：v01.00
 * 日期：2018/10/19
 */

#if 1
/**
 * 文件: dp_m2s_prot.h
 * 简要: 编解码模块控制协议
 * 详述：
 该协议主要是用于（设备节点）编解码模块与服务逻辑模块之间的内部控制操作。
 * 作者：lyw
 * 版本：v02.10
 * 日期：2018/12/07
 */

#ifndef __DP_M2S_PROT_H__
#define __DP_M2S_PROT_H__

#include "./dp_sd_type.h"
#include "DevStaticConfigure.h"
#include <string.h>
#pragma pack(push)
#pragma pack(1)

/* IDMS4K 输入设备 */
#define IDMS4K_DEV_HI3536_IN 0x1f
/* IDMS4K 输出设备 */
#define IDMS4K_DEV_HI3536_OUT 0x2f
#if (InputDevice)
/* IDMS4K 设备类型定义 */
#define IMDS4K_DEV IDMS4K_DEV_HI3536_IN
#endif
#if (OutputDevice)
/* IDMS4K 设备类型定义 */
#define IMDS4K_DEV IDMS4K_DEV_HI3536_OUT
#endif

/* 协议版本号 */
#define DP_M2S_INF_PROT_VERSION			"02.10"
#define DP_M2S_INF_PROT_VERSION_LEN		5
/* 协议通信接口定义 */
#define DP_M2S_INF_PROT_UNIX_FIFO		"/root/APPDIR/M2SFifo/UNIX_AV2APP"
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

//#if (IMDS4K_DEV == IDMS4K_DEV_HI3536_IN)
#if (InputDevice)

/*********************************** 输入节点 ***********************************/
/*说明： 音频输入设备枚举
 *定义： DP_M2S_AI_DEV_E
 *成员：
 *	DP_M2S_AI_DEV_LINEIN0_HI3536 : HI3536 音频模块的线性输入
 *	DP_M2S_AI_DEV_HDMI0_ITE6801 : ITE6801 音频输入
 *	DP_M2S_AI_DEV_MAX	: 音频输入设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_AI_DEV_E {

	DP_M2S_AI_DEV_LINEIN0_HI3536 = 0x0,
	DP_M2S_AI_DEV_HDMI0_ITE6801,
	DP_M2S_AI_DEV_MAX
} DP_M2S_AI_DEV_E;

/*说明： 视频输入设备枚举
 *定义： DP_M2S_VI_DEV_E
 *成员：
 *	DP_M2S_VI_DEV_HDMI0_ITE6801	： ITE6801 视频输入
 *	DP_M2S_VI_DEV_MAX	： 视频输入设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_VI_DEV_E {

	DP_M2S_VI_DEV_HDMI0_ITE6801 = 0x0, DP_M2S_VI_DEV_MAX
} DP_M2S_VI_DEV_E;

/*说明： 音频输出设备枚举
 *定义： DP_M2S_AO_DEV_E
 *成员：
 *	DP_M2S_AO_DEV_LINEOUT0_HI3536 ： HI3536 音频模块的线性输出
 *	DP_M2S_AO_DEV_MAX	： 音频输出设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_AO_DEV_E {

	DP_M2S_AO_DEV_LINEOUT0_HI3536 = 0x0, DP_M2S_AO_DEV_MAX
} DP_M2S_AO_DEV_E;

/*说明： HI3535 输出节点的视频输出设备枚举
 *定义： DP_M2S_VO_DEV_E
 *成员：
 *	DP_M2S_VO_DEV_MAX	: 视频输出设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_VO_DEV_E {
	DP_M2S_VO_DEV_HDMI0_HI3536 = 0x0, DP_M2S_VO_DEV_MAX
} DP_M2S_VO_DEV_E;

/* 音频编码通道最大数量 */
#define DP_M2S_AENC_CHN_MAX 2
/* 视频编码通道最大数量 */
#define DP_M2S_VENC_CHN_MAX 2

/* 音频解码通道最大数量*/
#define DP_M2S_ADEC_CHN_MAX 1
/* 视频解码通道最大数量*/
#define DP_M2S_VDEC_CHN_MAX 0

/* 一个视频输出设备的窗口拼接最大数量*/
#define DP_M2S_SWMS_MAX 1

/* H264编码宽高范围 以及 对齐*/
#define DP_M2S_H264E_MIN_WIDTH 160
#define DP_M2S_H264E_MAX_WIDTH 3840
#define DP_M2S_H264E_MIN_HEIGHT 64
#define DP_M2S_H264E_MAX_HEIGHT 2160
#define DP_M2S_H264E_MIN_ALIGN 2

//#elif (IMDS4K_DEV == IDMS4K_DEV_HI3536_OUT)
#elif (OutputDevice)
/*********************************** 输出节点 ***********************************/
/*说明： 音频输入设备枚举
 *定义： DP_M2S_AI_DEV_E
 *成员：
 *	DP_M2S_AI_DEV_LINEIN0_HI3536 	： HI3536 音频模块的线性输入
 *	DP_M2S_AI_DEV_MAX	: 音频输入设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_AI_DEV_E {

	DP_M2S_AI_DEV_LINEIN0_HI3536 = 0x0, DP_M2S_AI_DEV_MAX
}DP_M2S_AI_DEV_E;

/*说明： 视频输入设备枚举
 *定义： DP_M2S_VI_DEV_E
 *成员：
 *	DP_M2S_VI_DEV_WBC0_HI3536		： HI3536 HDMI视频输出信号回显输入
 *	DP_M2S_VI_DEV_MAX	： 视频输入设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_VI_DEV_E {

	DP_M2S_VI_DEV_WBC0_HI3536 = 0x0, DP_M2S_VI_DEV_MAX
}DP_M2S_VI_DEV_E;

/*说明： 音频输出设备枚举
 *定义： DP_M2S_AO_DEV_E
 *成员：
 *	DP_M2S_AO_DEV_LINEOUT0_HI3536 	： HI3536 音频模块的线性输出
 *	DP_M2S_AO_DEV_HDMI0_HI3536		： HI3536 HDMI音频输出
 *	DP_M2S_AO_DEV_MAX	： 音频输出设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_AO_DEV_E {

	DP_M2S_AO_DEV_LINEOUT0_HI3536 = 0x0,
	DP_M2S_AO_DEV_HDMI0_HI3536,
	DP_M2S_AO_DEV_MAX
}DP_M2S_AO_DEV_E;

/*说明： 视频输出设备枚举
 *定义： DP_M2S_VO_DEV_E
 *成员：
 *	DP_M2S_VO_DEV_HDMI0_HI3536	： HI3536 HDMI视频输出
 *	DP_M2S_VO_DEV_MAX	: 视频输出设备枚举数量
 *注意：无
 */
typedef enum _DP_M2S_VO_DEV_E {

	DP_M2S_VO_DEV_HDMI0_HI3536 = 0x0, DP_M2S_VO_DEV_MAX
}DP_M2S_VO_DEV_E;

/* 音频输入通道最大数量 */
#define DP_M2S_AI_CHN_MAX 1
/* 音频频输出通道最大数量 */
#define DP_M2S_AO_CHN_MAX 2
/* 视频输出通道最大数量 */
#define DP_M2S_VOU_CHN_MAX 1
/* 视频层是否使用虚拟层进行拼接 */
#define DP_M2S_IS_USE_VIR 1

/* 音频编码通道最大数量 */
#define DP_M2S_AENC_CHN_MAX 1
/* 视频编码通道最大数量 */
#define DP_M2S_VENC_CHN_MAX 1

/* 音频解码通道最大数量*/
#define DP_M2S_ADEC_CHN_MAX (16+2)
/* 视频解码通道最大数量*/
#define DP_M2S_VDEC_CHN_MAX 16

/* 一个视频输出设备的窗口拼接最大数量*/
#define DP_M2S_SWMS_MAX 16

/* H264编码宽高范围 以及 对齐*/
#define DP_M2S_H264E_MIN_WIDTH 160
#define DP_M2S_H264E_MAX_WIDTH 1920
#define DP_M2S_H264E_MIN_HEIGHT 64
#define DP_M2S_H264E_MAX_HEIGHT 1080
#define DP_M2S_H264E_MIN_ALIGN 2
/* H264解码宽高范围 以及 对齐*/
#define DP_M2S_H264D_MIN_WIDTH 64
#define DP_M2S_H264D_MAX_WIDTH 3840
#define DP_M2S_H264D_MIN_HEIGHT 64
#define DP_M2S_H264D_MAX_HEIGHT 2160
#define DP_M2S_H264D_MIN_ALIGN 2

#else
/* None */
#endif

/* RTSP服务端单个URL最大支持的连接 */
#define DP_M2S_RTSP_SERVER_CONNECT_MAX 128

/********************************任务ID分配 介绍************************/
/* 任务ID介绍::s32TskId
 * 适用对象：
 *		DP_M2S_CMD_AVENC_GET 和 DP_M2S_CMD_AVENC_SET	: 获取音视频编码通道信息，引用结构： DP_M2S_AVENC_INFO_S
 *		DP_M2S_CMD_AVDEC_GET 和 DP_M2S_CMD_AVDEC_SET	: 获取音视频解码通道信息，引用结构： DP_M2S_AVDEC_INFO_S
 * 范围定义：
 *	[0,256)，为音频编码任务ID，选中此ID范围时，仅可操作音频编码的相关属性；
 *	[256,512)，为音频解码任务ID，选中此ID范围时，仅可操作音频解码的相关属性；
 *	[512,1024)，为视频编码任务ID，选中此ID范围时，仅可操作视频编码的相关属性；
 *	[1024,1280)，为视频解码任务ID，选中此ID范围时，仅可操作视频解码的相关属性；
 *	[1280,1536)，为音视频编码任务ID，选中此ID范围时，仅可操作音视频编码的相关属性；
 *	[1536,1792)，为音视频解码任务ID，选中此ID范围时，仅可操作音视频解码的相关属性；
 *	其他预留，无效；
 */

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
 *	DP_M2S_AVBIND_AI2AENC_VI2VENC ： 音频输入绑到音频输出，同时，音频输入绑到音频编码
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
 *	DP_M2S_MOD_ADEC：音频解码模块
 *	DP_M2S_MOD_VDEC：视频解码模块
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
 *	u32ModId ：模块号，引用 DP_M2S_MOD_ID_E
 *	u32DevId ：设备号
 *	u32ChnId ：通道号
 *注意：
 * 		若配置模块ID为 DP_M2S_MOD_AI 时，设备号有效，引用 DP_M2S_AI_DEV_E ，通道号无效；
 *		若配置模块ID为 DP_M2S_MOD_AO 时，设备号有效，引用 DP_M2S_AO_DEV_E ，通道号无效；
 *		若配置模块ID为 DP_M2S_MOD_VI 时，设备号有效，引用 DP_M2S_VI_DEV_E ，通道号无效；
 *		若配置模块ID为 DP_M2S_MOD_VO 时，设备号有效，引用 DP_M2S_VO_DEV_E ，通道号无效；
 *		若配置模块ID为 DP_M2S_MOD_AENC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_AENC_CHN_MAX)
 *		若配置模块ID为 DP_M2S_MOD_VENC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_VENC_CHN_MAX)
 *		若配置模块ID为 DP_M2S_MOD_ADEC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_ADEC_CHN_MAX)
 *		若配置模块ID为 DP_M2S_MOD_VDEC 时，设备号无效，通道号有效；通道号范围[0,DP_M2S_VDEC_CHN_MAX)
 */
typedef struct _DP_M2S_SOURCE_RELATION_S {

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
	DP_M2S_AVBIND_TYPE_E enBindType;
	DP_M2S_BIND_S stAudio;
	DP_M2S_BIND_S stVideo;
} DP_M2S_AVBIND_ATTR_S;

/**************************************************************************************************************/
/***************************************************公共数据结构 声明***********************************************/
/**************************************************************************************************************/
/*
 *说明： 控制指令操作类型
 *定义： DP_M2S_CMD_ID_E
 *成员：
 *	DP_M2S_CMD_SYS_INIT			： 初始化系统资源 ，引用结构 DP_M2S_CMD_SYS_INIT_S 和 DP_M2S_CMD_SYS_INIT_ACK_S
 *	DP_M2S_CMD_SYS_DEINIT		： 去初始化系统资源 ，引用结构 DP_M2S_CMD_SYS_DEINIT_S 和 DP_M2S_CMD_SYS_DEINIT_ACK_S
 *	DP_M2S_CMD_EXIT				： 退出程序 ，引用结构 DP_M2S_CMD_EXIT_S 和 DP_M2S_CMD_EXIT_ACK_S
 *	DP_M2S_CMD_AI_GET			： 获取音频输入设备信息 ，引用结构 DP_M2S_CMD_AI_GETINFO_S 和 DP_M2S_CMD_AI_GETINFO_ACK_S
 *	DP_M2S_CMD_AI_SET			： 设置音频输入设备信息 ，引用结构 DP_M2S_CMD_AI_SETINFO_S 和 DP_M2S_CMD_AI_SETINFO_ACK_S
 *	DP_M2S_CMD_VI_GET			： 获取视频输入设备信息 ， 引用结构 DP_M2S_CMD_VI_GETINFO_S 和 DP_M2S_CMD_VI_GETINFO_ACK_S
 *	DP_M2S_CMD_VI_SET			： 设置视频输入设备信息 ，引用结构  DP_M2S_CMD_VI_SETINFO_S 和 DP_M2S_CMD_VI_SETINFO_ACK_S
 *	DP_M2S_CMD_AVENC_GET		： 获取音视频编码信息 ，引用结构 DP_M2S_CMD_AVENC_GETINFO_S 和 DP_M2S_CMD_AVENC_GETINFO_ACK_S
 *	DP_M2S_CMD_AVENC_SET		： 设置音视频编码信息 ，引用结构 DP_M2S_CMD_AVENC_SETINFO_S 和 DP_M2S_CMD_AVENC_SETINFO_ACK_S
 *	DP_M2S_CMD_AVENC_GET_BATCH		: 批量获取音视频编码信息，引用结构 DP_M2S_CMD_AVENC_GETBATCHINFO_S 和 DP_M2S_CMD_AVENC_GETBATCHINFO_ACK_S
 *	DP_M2S_CMD_AVENC_SET_BATCH		: 批量设置音视频编码信息，引用结构 DP_M2S_CMD_AVENC_SETBATCHINFO_S 和 DP_M2S_CMD_AVENC_SETBATCHINFO_ACK_S
 *	DP_M2S_CMD_AVENC_OPEN_BATCH		: 批量打开音视频编码，引用结构 DP_M2S_CMD_AVENC_OPENINFO_S 和 DP_M2S_CMD_AVENC_OPENINFO_ACK_S
 *	DP_M2S_CMD_AVENC_CLOSE_BATCH	: 批量关闭音视频编码，引用结构 DP_M2S_CMD_AVENC_CLOSEINFO_S 和 DP_M2S_CMD_AVENC_CLOSEINFO_ACK_S
 *	DP_M2S_CMD_AVDEC_GET		： 获取音视频解码信息 ，引用结构 DP_M2S_CMD_AVDEC_GETINFO_S 和 DP_M2S_CMD_AVDEC_GETINFO_ACK_S
 *	DP_M2S_CMD_AVDEC_SET		： 设置音视频解码信息 ，引用结构 DP_M2S_CMD_AVDEC_SETINFO_S 和 DP_M2S_CMD_AVDEC_SETINFO_ACK_S
 * 	DP_M2S_CMD_AVDEC_GET_BATCH		： 批量获取音视频解码信息，引用结构 DP_M2S_CMD_AVDEC_GETBATCHINFO_S 和 DP_M2S_CMD_AVDEC_GETBATCHINFO_ACK_S
 * 	DP_M2S_CMD_AVDEC_SET_BATCH		： 批量设置音视频解码信息，引用结构 DP_M2S_CMD_AVDEC_SETBATCHINFO_S 和 DP_M2S_CMD_AVDEC_SETBATCHINFO_ACK_S
 *	DP_M2S_CMD_AVDEC_OPEN_BATCH		： 批量打开音视频解码，引用结构 DP_M2S_CMD_AVDEC_OPENINFO_S 和 DP_M2S_CMD_AVDEC_OPENINFO_ACK_S
 *	DP_M2S_CMD_AVDEC_CLOSE_BATCH	： 批量关闭音视频解码，引用结构 DP_M2S_CMD_AVDEC_CLOSEINFO_S 和 DP_M2S_CMD_AVDEC_CLOSEINFO_ACK_S
 *	DP_M2S_CMD_WINS_GET			： 获取音视频解码的窗口信息 ，引用结构 DP_M2S_CMD_WINS_GETINFO_S 和 DP_M2S_CMD_WINS_GETINFO_ACK_S
 *	DP_M2S_CMD_WINS_SET			： 设置音视频解码的窗口信息 ，引用结构 DP_M2S_CMD_WINS_SETINFO_S 和 DP_M2S_CMD_WINS_SETINFO_ACK_S
 * 	DP_M2S_CMD_WINS_GET_BATCH		： 批量获取（视频）窗口信息，引用结构 DP_M2S_CMD_WINS_GETBATCHINFO_S 和 DP_M2S_CMD_WINS_GETBATCHINFO_ACK_S
 * 	DP_M2S_CMD_WINS_SET_BATCH		： 批量设置（视频）窗口信息，引用结构 DP_M2S_CMD_WINS_SETBATCHINFO_S 和 DP_M2S_CMD_WINS_SETBATCHINFO_ACK_S
 *	DP_M2S_CMD_WINS_OPEN_BATCH		： 批量打开（视频）窗口，引用结构 DP_M2S_CMD_WINS_OPENINFO_S 和 DP_M2S_CMD_WINS_OPENINFO_ACK_S
 *	DP_M2S_CMD_WINS_CLOSE_BATCH		： 批量关闭（视频）窗口，引用结构 DP_M2S_CMD_WINS_CLOSEINFO_S 和 DP_M2S_CMD_WINS_CLOSEINFO_ACK_S
 *	DP_M2S_CMD_AO_GET			： 获取音频输出设备信息 ，引用结构 DP_M2S_CMD_AO_GETINFO_S 和 DP_M2S_CMD_AO_GETINFO_ACK_S
 *	DP_M2S_CMD_AO_SET			： 设置音频输出设备信息 ，引用结构 DP_M2S_CMD_AO_SETINFO_S 和 DP_M2S_CMD_AO_SETINFO_ACK_S
 *	DP_M2S_CMD_VO_GET			： 获取视频输出设备信息 ，引用结构 DP_M2S_CMD_VO_GETINFO_S 和 DP_M2S_CMD_VO_GETINFO_ACK_S
 *	DP_M2S_CMD_VO_SET			： 设置视频输出设备信息 ，引用结构 DP_M2S_CMD_VO_SETINFO_S 和 DP_M2S_CMD_VO_SETINFO_ACK_S
 *注意：
 *	1、 须先 初始化系统资源；
 *	2、 DP_M2S_CMD_WINS_GET 或 DP_M2S_CMD_WINS_SET 使用前，须保证相关的任务有效；
 */

typedef enum {

	/* system */
	DP_M2S_CMD_SYS_INIT = 0x0,
	DP_M2S_CMD_SYS_DEINIT,
	DP_M2S_CMD_EXIT,

	/* audio capture */
	DP_M2S_CMD_AI_GET,
	DP_M2S_CMD_AI_SET,

	/* video capture */
	DP_M2S_CMD_VI_GET,
	DP_M2S_CMD_VI_SET,

	/* audio & video encode */
	DP_M2S_CMD_AVENC_GET,
	DP_M2S_CMD_AVENC_SET,
	DP_M2S_CMD_AVENC_GET_BATCH,
	DP_M2S_CMD_AVENC_SET_BATCH,

	DP_M2S_CMD_AVENC_OPEN_BATCH,
	DP_M2S_CMD_AVENC_CLOSE_BATCH,

	/* audio & video decode */
	DP_M2S_CMD_AVDEC_GET,
	DP_M2S_CMD_AVDEC_SET,
	DP_M2S_CMD_AVDEC_GET_BATCH,
	DP_M2S_CMD_AVDEC_SET_BATCH,

	DP_M2S_CMD_AVDEC_OPEN_BATCH,
	DP_M2S_CMD_AVDEC_CLOSE_BATCH,

	/* video wins */
	DP_M2S_CMD_WINS_GET,
	DP_M2S_CMD_WINS_SET,
	DP_M2S_CMD_WINS_GET_BATCH,
	DP_M2S_CMD_WINS_SET_BATCH,

	DP_M2S_CMD_WINS_OPEN_BATCH,
	DP_M2S_CMD_WINS_CLOSE_BATCH,

	/* audio output */
	DP_M2S_CMD_AO_GET,
	DP_M2S_CMD_AO_SET,

	/* video output */
	DP_M2S_CMD_VO_GET,
	DP_M2S_CMD_VO_SET,

	DP_M2S_CMD_BUTT

} DP_M2S_CMD_ID_E;
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
 *定义： DP_M2S_AUDIO_SOUND_MODE_E
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
	_DP_M2S_AUDIO_COMM_ATTR_S(DP_M2S_AUDIO_SAMPLE_RATE_E samplerate,
			DP_M2S_AUDIO_BIT_WIDTH_E bitwidth,
			DP_M2S_AUDIO_SOUND_MODE_E soundmode) :
			enSamplerate(samplerate), enBitwidth(bitwidth), enSoundmode(
					soundmode) {
	}
	_DP_M2S_AUDIO_COMM_ATTR_S() :
			enSamplerate(), enBitwidth(), enSoundmode() {
	}
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
typedef struct _DP_M2S_RECT_S {
	DP_S32 s32X;
	DP_S32 s32Y;
	DP_U32 u32Width;
	DP_U32 u32Height;
} DP_M2S_RECT_S;

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
 *说明： h264编码属性
 *定义： DP_M2S_ALG_H264ENC_ATTR_S
 *成员：
 *   u32FrmRate : 编码帧率，范围(0,60]fps
 *   stSize     : 编码的图像宽高大小，引用 DP_M2S_SIZE_S
 *					宽度范围[DP_M2S_H264E_MIN_WIDTH, DP_M2S_H264E_MAX_WIDTH]
 *					高度范围[DP_M2S_H264E_MIN_HEIGHT, DP_M2S_H264E_MAX_HEIGHT]
 *   enRcMode   : 码率模式，引用 DP_M2S_RC_MODE_E
 *   u32Bitrate : 码率，范围[256,40000]Kbps
 *	 enProfile	：编码Profile类型，引用 DP_M2S_ALG_PROFILE_E
 *   u32Gop 	: GOP，表示I帧之间的间隔，范围(0, 120]
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
 *	stAACEnc	：aac编码属性参数，引用 DP_M2S_ALG_AACENC_ATTR_S
 *	stAACDec	：aac解码属性参数，引用 DP_M2S_ALG_AACDEC_ATTR_S
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
 *说明：	拼接属性
 *定义： DP_M2S_SWMS_ATTR_S
 *成员：
 *	enVoDevId	： 视频输出设备ID，设备数： DP_M2S_VO_DEV_MAX ，引用 DP_M2S_VO_DEV_E
 *	u32SwmsChn	： 拼接窗口通道，范围[0,DP_M2S_SWMS_MAX)
 *	u32Priority	： 窗口优先级；
 *	stRect		： 窗口位置显示参数，引用 DP_M2S_RECT_S
 *注意：无
 */
typedef struct _DP_M2S_SWMS_ATTR_S {
	_DP_M2S_SWMS_ATTR_S(DP_M2S_VO_DEV_E voDevId, DP_S32 swmsChn,
			DP_U32 priority, DP_M2S_RECT_S rect) :
			enVoDevId(voDevId), u32SwmsChn(swmsChn), u32Priority(priority), stRect(
					rect) {
	}
	_DP_M2S_SWMS_ATTR_S() :
			enVoDevId(), u32SwmsChn(), u32Priority(), stRect() {
	}
	DP_M2S_VO_DEV_E enVoDevId;
	DP_U32 u32SwmsChn;
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
 *说明： 音频编码属性
 *定义： DP_M2S_AENC_ATTR_S
 *成员：
 *  stAlg	：  编解码算法属性参数，引用 DP_M2S_ALG_ATTR_S
 *注意：无
 */
typedef struct _DP_M2S_AENC_ATTR_S {
	_DP_M2S_AENC_ATTR_S(DP_M2S_ALG_ATTR_S alg) :
			stAlg(alg) {
	}
	_DP_M2S_AENC_ATTR_S() :
			stAlg() {
	}
	DP_M2S_ALG_ATTR_S stAlg;
} DP_M2S_AENC_ATTR_S;

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
 *说明： 视频解码属性
 *定义： DP_M2S_VDEC_ATTR_S
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
 *说明： 流媒体类型
 *定义： DP_M2S_STREAM_TYPE_E
 *成员：
 *	DP_M2S_STREAM_RTSP_SERVER	：rtsp服务端类型
 *	DP_M2S_STREAM_RTSP_CLIENT	：rtsp客户端类型
 *	DP_M2S_STREAM_RTMP_SERVER	：rtmp客户端类型 预留
 *	DP_M2S_STREAM_RTMP_CLIENT	：rtmp客户端类型 预留
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
 *  bUDP			:  传输类型，DP_FALSE：tcp，DP_TRUE：udp
 *  bMulticast		:  是否支持组播，当bUDP等于DP_TRUE，该参数生效，DP_FALSE：单播，DP_TRUE：组播
 *  s32ConnTimeout	:  连接超时，范围0~300秒
 *  s32ConnMax		： 服务端可支持的最大连接数目，范围[1,DP_M2S_RTSP_SERVER_CONNECT_MAX]
 *  s32ConnNums		： 服务端当前连接数目[0,DP_M2S_RTSP_SERVER_CONNECT_MAX];
 *  au8Url[]		:  连接地址url，长度范围[1,DP_M2S_URL_LEN];
 *注意：无
 */
typedef struct _DP_M2S_RTSP_SERVER_ATTR_S {
	DP_BOOL bOpen;
	DP_BOOL bUDP;
	DP_BOOL bMulticast;
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
 *  bUDP			:  传输类型，DP_FALSE：tcp，DP_TRUE：udp
 *  bMulticast		:  是否支持组播，当bUDP等于DP_TRUE，该参数生效，DP_FALSE：单播，DP_TRUE：组播
 *  s32ConnTimeout	:  连接超时，范围0~300秒
 *  au8Url[]		:  连接地址url，长度范围[1,DP_M2S_URL_LEN];
 *注意：无
 */
typedef struct _DP_M2S_RTSP_CLIENT_ATTR_S {
	DP_S8 s8Open;
	DP_BOOL bUDP;
	DP_BOOL bMulticast;
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

/************************************* 音频输入设备信息 *********************************/

/*
 *说明： 获取 音频输入设备信息
 *定义： DP_M2S_AI_GET_INFO_S
 *成员：
 *	enDevId			：音频输入设备ID，设备数： DP_M2S_AI_DEV_MAX ，引用 DP_M2S_AI_DEV_E
 *	bConn			：音频输入设备连接状态，DP_FALSE，断开；DP_TRUE，已连接；
 *	bSignal			：音频输入设备信号状态，DP_FALSE，无信号；DP_TRUE，有信号；
 *	u8Vol			：音频输入设备音量[0,100]
 *	stCommAttr		：音频输入设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S;
 *注意：
 *	对应指令： DP_M2S_CMD_AI_GET ；
 */
typedef struct _DP_M2S_AI_GET_INFO_S {
	DP_M2S_AI_DEV_E enDevId;
	DP_BOOL bsConn;
	DP_BOOL bSignal;
	DP_U8 u8Vol;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
} DP_M2S_AI_GET_INFO_S;

/*
 *说明： 设置 音频输入设备信息
 *定义： DP_M2S_AI_SET_INFO_S
 *成员：
 *	enDevId			：音频输入设备ID，设备数： DP_M2S_AI_DEV_MAX ，引用 DP_M2S_AI_DEV_E
 *	u8Vol			：音频输入设备音量[0,100]
 *	stCommAttr		：音频输入设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S;
 *注意：
 *	对应指令： DP_M2S_CMD_SYS_INIT 或 DP_M2S_CMD_AI_SET
 *	须注意的是，当使用指令 DP_M2S_CMD_AI_SET 时，成员参数 stCommAttr 无效；
 */
typedef struct _DP_M2S_AI_SET_INFO_S {
	_DP_M2S_AI_SET_INFO_S(DP_M2S_AI_DEV_E devId, DP_U8 vol,
			DP_M2S_AUDIO_SAMPLE_RATE_E samplerate,
			DP_M2S_AUDIO_BIT_WIDTH_E bitwidth,
			DP_M2S_AUDIO_SOUND_MODE_E soundmode) :
			enDevId(devId), u8Vol(vol), stCommAttr(samplerate, bitwidth,
					soundmode) {
	}
	_DP_M2S_AI_SET_INFO_S() :
			enDevId(), u8Vol(), stCommAttr() {
	}
	DP_M2S_AI_DEV_E enDevId;
	DP_U8 u8Vol;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
} DP_M2S_AI_SET_INFO_S;

/************************************* 视频输入设备信息 *********************************/
/*
 *说明： 获取 视频输入设备信息
 *定义： DP_M2S_VI_GET_INFO_S
 *成员：
 *	enDevId		： 视频输入设备ID，设备数： DP_M2S_VI_DEV_MAX ，引用 DP_M2S_VI_DEV_E
 *	bConn		： 视频输入设备是否连接，DP_TRUE：连接；DP_FALSE：断开
 *	bSignal		： 视频输入设备信号是否正常，DP_TRUE：有信号，DP_FALSE：无信号
 *  u32FrmRate  :  目标帧率，范围[0,60]fps
 *	stCap		:  原始图像坐标和宽高，引用 DP_M2S_RECT_S
 *	stDst		:  目标图像大小，引用 DP_M2S_SIZE_S
 *注意：
 *	对应指令： DP_M2S_CMD_VI_GET ；
 */
typedef struct _DP_M2S_VI_GET_INFO_S {
	DP_M2S_VI_DEV_E enDevId;
	DP_BOOL bConn;
	DP_BOOL bSignal;
	DP_U32 u32FrmRate;
	DP_M2S_RECT_S stCap;
	DP_M2S_SIZE_S stDst;
} DP_M2S_VI_GET_INFO_S;

/*
 *说明： 设置 视频输入设备信息
 *定义： DP_M2S_VI_SET_INFO_S
 *成员：
 *	enDevId		： 视频输入设备ID，设备数： DP_M2S_VI_DEV_MAX ，引用 DP_M2S_VI_DEV_E
 *  u32FrmRate  :  目标帧率，范围[0,60]fps
 *	stCap		:  原始图像坐标和宽高，引用 DP_M2S_RECT_S
 *	stDst		:  目标图像大小，引用 DP_M2S_SIZE_S
 *注意：
 *	对应指令： DP_M2S_CMD_VI_SET ；
 */
typedef struct _DP_M2S_VI_SET_INFO_S {
	DP_M2S_VI_DEV_E enDevId;
	DP_U32 u32FrmRate;
	DP_M2S_RECT_S stCap;
	DP_M2S_SIZE_S stDst;
} DP_M2S_VI_SET_INFO_S;

/************************************* 音视频编码信息 *********************************/
/*
 *说明： 获取或设置 音视频编码信息
 *定义： DP_M2S_AVENC_INFO_S
 *成员：
 *  s32TskId    : 任务ID，详细参看“任务ID分配”介绍
 *  AvBindAttr	：音视频资源绑定属性，引用 DP_M2S_AVBIND_ATTR_S
 *  stAenc		：音频编码属性参数，引用 DP_M2S_AENC_ATTR_S
 *  stVenc		：视频编码属性参数，引用 DP_M2S_VENC_ATTR_S
 *  stStream	：流媒体属性参数，引用 DP_M2S_STREAM_ATTR_S
 *注意：
 对应指令： DP_M2S_CMD_AVENC_GET 和 DP_M2S_CMD_AVENC_SET ；
 */
typedef struct _DP_M2S_AVENC_INFO_S {
	_DP_M2S_AVENC_INFO_S(DP_S32 tskId, DP_M2S_AVBIND_ATTR_S avBindAttr,
			DP_M2S_AENC_ATTR_S aenc, DP_M2S_VENC_ATTR_S venc,
			DP_M2S_STREAM_TYPE_E type, DP_M2S_RTSP_CLIENT_ATTR_S rtspClient) :
			s32TskId(tskId), stAvBind(avBindAttr), stAenc(aenc), stVenc(venc), stStream(
					type, rtspClient) {
	}
	_DP_M2S_AVENC_INFO_S(DP_S32 tskId, DP_M2S_AVBIND_ATTR_S avBindAttr,
			DP_M2S_AENC_ATTR_S aenc, DP_M2S_VENC_ATTR_S venc,
			DP_M2S_STREAM_TYPE_E type, DP_M2S_RTSP_SERVER_ATTR_S rtspServer) :
			s32TskId(tskId), stAvBind(avBindAttr), stAenc(aenc), stVenc(venc), stStream(
					type, rtspServer) {
	}
	_DP_M2S_AVENC_INFO_S() :
			s32TskId(), stAvBind(), stAenc(), stVenc(), stStream() {
	}
	DP_S32 s32TskId;
	DP_M2S_AVBIND_ATTR_S stAvBind;
	DP_M2S_AENC_ATTR_S stAenc;
	DP_M2S_VENC_ATTR_S stVenc;
	DP_M2S_STREAM_ATTR_S stStream;
} DP_M2S_AVENC_INFO_S;

/************************************* 音视频解码信息 *********************************/
/*
 *说明： 获取或设置 音视频解码信息
 *定义： DP_M2S_AVDEC_INFO_S
 *成员：
 *  s32TskId    :  任务ID，详细参看“任务ID分配”介绍
 *  AvBindAttr	： 音视频资源绑定属性，引用 DP_M2S_AVBIND_ATTR_S
 *	stStream	： 流媒体属性参数，引用 DP_M2S_STREAM_ATTR_S
 *	stAdec		:  音频解码属性参数，引用 DP_M2S_ADEC_ATTR_S
 *	stVdec		:  视频解码属性参数，引用 DP_M2S_VDEC_ATTR_S
 *注意：
 对应指令： DP_M2S_CMD_AVDEC_GET 和 DP_M2S_CMD_AVDEC_SET ；
 */
typedef struct _DP_M2S_AVDEC_INFO_S {
	_DP_M2S_AVDEC_INFO_S(DP_S32 tskId, DP_M2S_AVBIND_ATTR_S avBindAttr,
			DP_M2S_STREAM_ATTR_S stream, DP_M2S_ADEC_ATTR_S adec,
			DP_M2S_VDEC_ATTR_S vdec) :
			s32TskId(tskId), AvBindAttr(avBindAttr), stStream(stream), stAdec(
					adec), stVdec(vdec) {
	}
	_DP_M2S_AVDEC_INFO_S() :
			s32TskId(), AvBindAttr(), stStream(), stAdec(), stVdec() {
	}
	DP_S32 s32TskId;
	DP_M2S_AVBIND_ATTR_S AvBindAttr;
	DP_M2S_STREAM_ATTR_S stStream;
	DP_M2S_ADEC_ATTR_S stAdec;
	DP_M2S_VDEC_ATTR_S stVdec;
} DP_M2S_AVDEC_INFO_S;

/************************************* 音频输出信息 *********************************/
/*
 *说明： 获取或设置 音频输出信息
 *定义： DP_M2S_AO_INFO_S
 *成员：
 *	enDevId		：音频输出设备ID，设备数： DP_M2S_AO_DEV_MAX ，引用 DP_M2S_AO_DEV_E
 *	u8Vol		：音频输出设备音量0-100
 *注意：
 *	对应指令： DP_M2S_CMD_AO_GET 或  DP_M2S_CMD_AO_SET ；
 */
typedef struct _DP_M2S_AO_INFO_S {
	_DP_M2S_AO_INFO_S(DP_M2S_AO_DEV_E devId, DP_U8 vol) :
			enDevId(devId), u8Vol(vol) {
	}
	DP_M2S_AO_DEV_E enDevId;
	DP_U8 u8Vol;
} DP_M2S_AO_INFO_S;

/************************************* 视频输出信息 *********************************/
/*
 *说明： 获取或设置 视频输出信息
 *定义： DP_M2S_VO_INFO_S
 *成员：
 *	enDevId	：音频输出设备ID，设备数： DP_M2S_VO_DEV_MAX ，引用 DP_M2S_VO_DEV_E
 *	bEnable	： 视频输出使能，DP_FALSE: 禁止；DP_TRUE：启用；
 *	enSync	:  视频输出时序，引用 DP_M2S_VIDEO_SYNC_E
 *	stSwms	:  视频输出的拼接窗口参数，引用 DP_M2S_SWMS_ATTR_S
 *	stCsc	： 视频输出图像效果参数，引用 DP_M2S_CSC_ATTR_S
 *注意：
 *	1.对应指令： DP_M2S_CMD_SYS_INIT 或 DP_M2S_CMD_VO_GET 或  DP_M2S_CMD_VO_SET，
 *	须注意的是，当使用指令 DP_M2S_CMD_VO_GET 和 DP_M2S_CMD_VO_SET 时，成员参数 enSync 无效；
 *	2.参数s32DevId和参数stSwms[].s32VoDevId一样；

 */
typedef struct _DP_M2S_VO_INFO_S {
	_DP_M2S_VO_INFO_S(DP_M2S_VO_DEV_E devId, DP_BOOL enable,
			DP_M2S_VIDEO_SYNC_E sync, DP_M2S_SWMS_ATTR_S *swms, DP_U32 swmNum,
			DP_M2S_CSC_ATTR_S csc) :
			enDevId(devId), bEnable(enable), enSync(sync), stCsc(csc) {
		if (swms != NULL && swmNum > 0) {
			memcpy(stSwms, swms, swmNum * sizeof(DP_M2S_SWMS_ATTR_S));
		}
	}
	_DP_M2S_VO_INFO_S() :
			enDevId(), bEnable(), enSync() {
		memset(stSwms, 0, DP_M2S_SWMS_MAX * sizeof(DP_M2S_SWMS_ATTR_S));
	}
	DP_M2S_VO_DEV_E enDevId;
	DP_BOOL bEnable;
	DP_M2S_VIDEO_SYNC_E enSync;
	DP_M2S_SWMS_ATTR_S stSwms[DP_M2S_SWMS_MAX];
	DP_M2S_CSC_ATTR_S stCsc;
} DP_M2S_VO_INFO_S;

/************************************* 窗口信息 *********************************/
/*
 *说明： 获取或设置 窗口信息
 *定义： DP_M2S_WINS_INFO_S
 *成员：
 *  s32CropEnable	：  裁剪设置使能，0：关闭裁剪功能；1：开启裁剪功能；-1：裁剪设置无效；
 *  s32ZoomEnable	：  缩放设置使能，0：关闭缩放功能；1：开启缩放功能；-1：缩放设置无效；
 *  s32OsdEnable	：  OSD设置使能， 0：关闭OSD功能； 1：开启OSD功能； -1：OSD设置无效；
 *  s32SwmsEnable	：  拼接设置使能，0：关闭拼接功能；1：开启拼接功能；-1：拼接设置无效；
 *  stCrop	：  图像裁剪属性参数，引用 DP_M2S_CROP_ATTR_S
 *  stZoom	：  图像缩放属性参数，引用 DP_M2S_ZOOM_ATTR_S
 *  stOsd	：  图像叠加属性参数，引用 DP_M2S_OSD_ATTR_S
 *  stSwms  ：  图像拼接属性参数，引用 DP_M2S_SWMS_ATTR_S
 *注意：
 *	1、当设备不支持某功能时，相应地设置为-1，即可表示无效；
 *	2、仅当任务有效时，该信息参数才可有效。
 */
typedef struct _DP_M2S_WINS_INFO_S {

	DP_S32 s32CropEnable;
	DP_S32 s32ZoomEnable;
	DP_S32 s32OsdEnable;
	DP_S32 s32SwmsEnable;
	DP_M2S_CROP_ATTR_S stCrop;
	DP_M2S_ZOOM_ATTR_S stZoom;
	DP_M2S_OSD_ATTR_S stOsd;
	DP_M2S_SWMS_ATTR_S stSwms;
} DP_M2S_WINS_INFO_S;

/**************************************************************************************************************/
/***************************************************控制指令操作类型 声明***********************************************/
/**************************************************************************************************************/

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
	_DP_M2S_CMD_ACK_S(DP_M2S_CMD_ID_E cmd, DP_U32 success) :
			stHeader(sizeof(_DP_M2S_CMD_ACK_S), cmd, 0x01), u32Success(success) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
} DP_M2S_CMD_ACK_S;

/*
 *说明： 通用批量式的返回结果的结构参数
 *定义： DP_M2S_CMD_BATCH_ACK_S
 *成员：
 *	u32Success	： 0:成功，其他失败，返回错误码
 *	u32Nums		： 数组个数，须大于0
 *	au32ErrInfo	： 错误信息数据内容（可变），成员个数等于 u32Nums ， 数据成员结构类型为 DP_U32（0:成功，非0；失败）
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_BATCH_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_U32 u32Nums;
	// DP_U32 au32ErrInfo[u32Nums];
} DP_M2S_CMD_BATCH_ACK_S;

/*************************************1. DP_M2S_CMD_SYS_INIT *************************/
/*
 *说明： 初始化系统资源
 *定义： DP_M2S_CMD_SYS_INIT_S
 *成员：
 *	stHeader ：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_SYS_INIT ；
 *	au8VerInfo : 协议版本字符串，内容等于 DP_M2S_INF_PROT_VERSION ，用于通信交互时双方协议版本的匹配；
 *	stAiInfo : 音频输入设备开机配置属性，引用 DP_M2S_AI_SET_INFO_S
 *	stVoInfo : 视频输出设备开机配置属性，引用 DP_M2S_VO_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_SYS_INIT_S {
	_DP_M2S_CMD_SYS_INIT_S(const DP_M2S_AI_SET_INFO_S *aiInfo, DP_U32 aiCount,
			const DP_M2S_VO_INFO_S *voInfo, DP_U32 voCount) :
			stHeader(sizeof(_DP_M2S_CMD_SYS_INIT_S), DP_M2S_CMD_SYS_INIT, 0x01) {
		memset(au8VerInfo, 0, DP_M2S_INF_PROT_VERSION_LEN);
		memcpy(au8VerInfo, DP_M2S_INF_PROT_VERSION,
				sizeof(DP_M2S_INF_PROT_VERSION));
		if (aiCount <= DP_M2S_AI_DEV_MAX && aiCount > 0)
			memcpy(stAiInfo, aiInfo, aiCount * sizeof(DP_M2S_AI_SET_INFO_S));
		if (voCount <= DP_M2S_VO_DEV_MAX && voCount > 0)
			memcpy(stVoInfo, voInfo, voCount * sizeof(DP_M2S_VO_INFO_S));
	}
	_DP_M2S_CMD_SYS_INIT_S() :
			stHeader(sizeof(_DP_M2S_CMD_SYS_INIT_S), DP_M2S_CMD_SYS_INIT, 0x01) {
		memset(au8VerInfo, 0, DP_M2S_INF_PROT_VERSION_LEN);
		memcpy(au8VerInfo, DP_M2S_INF_PROT_VERSION,
				sizeof(DP_M2S_INF_PROT_VERSION));
		memset(stAiInfo, 0, sizeof(DP_M2S_AI_SET_INFO_S) * DP_M2S_AI_DEV_MAX);
		memset(stVoInfo, 0, sizeof(DP_M2S_VO_INFO_S) * DP_M2S_VO_DEV_MAX);
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U8 au8VerInfo[DP_M2S_INF_PROT_VERSION_LEN];
	DP_M2S_AI_SET_INFO_S stAiInfo[DP_M2S_AI_DEV_MAX];
	DP_M2S_VO_INFO_S stVoInfo[DP_M2S_VO_DEV_MAX];
} DP_M2S_CMD_SYS_INIT_S;

/* “初始化系统资源” 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_SYS_INIT_ACK_S;

/*************************************2. DP_M2S_CMD_SYS_DEINIT *************************/
/*
 *说明： 去初始化系统资源
 *定义： DP_M2S_CMD_SYS_DEINIT_S
 *成员：
 *	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_SYS_DEINIT ；
 *注意：无
 */
//typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_SYS_DEINIT_S;
typedef DP_M2S_INF_PROT_HEAD_S DP_M2S_CMD_SYS_DEINIT_S;

/* 去初始化系统资源 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_SYS_DEINIT_ACK_S;

/*************************************3. DP_M2S_CMD_EXIT *************************/
/*
 *说明： 退出程序
 *定义： DP_M2S_CMD_EXIT_S
 *成员：
 *	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_EXIT ；
 *注意：无
 */
//typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_EXIT_S;
typedef DP_M2S_INF_PROT_HEAD_S DP_M2S_CMD_EXIT_S;

/* 去初始化系统资源 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_EXIT_ACK_S;

/*************************************4. DP_M2S_CMD_AI_GET *************************/
/*
 *说明： 获取音频输入设备信息
 *定义： DP_M2S_CMD_AI_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AI_GET ；
 *	enDevId		： 音频输入设备ID，引用 DP_M2S_AI_DEV_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_AI_GETINFO_S {
	_DP_M2S_CMD_AI_GETINFO_S(DP_M2S_CMD_ID_E cmd, DP_M2S_AI_DEV_E dev) :
			stHeader(sizeof(_DP_M2S_CMD_AI_GETINFO_S), cmd, 0x01), enDevId(dev) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_AI_DEV_E enDevId;
} DP_M2S_CMD_AI_GETINFO_S;

/*
 *说明： 获取音频输入设备信息 应答
 *定义： DP_M2S_CMD_AI_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AI_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *  stInfo		:  音频输入设备信息，引用 DP_M2S_AI_GET_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AI_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_M2S_AI_GET_INFO_S stInfo;
} DP_M2S_CMD_AI_GETINFO_ACK_S;

/*************************************5. DP_M2S_CMD_AI_SET *************************/
/*
 *说明： 设置音频输入设备信息
 *定义： DP_M2S_CMD_AI_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AI_SET ；
 *	enDevId		：音频输入设备ID，设备数： DP_M2S_AI_DEV_MAX ，引用 DP_M2S_AI_DEV_E
 *	u8Vol		：音频输入设备音量[0,100]
 *注意：无
 */
typedef struct _DP_M2S_CMD_AI_SETINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_AI_SET_INFO_S stInfo;
} DP_M2S_CMD_AI_SETINFO_S;

/* 设置音频输入设备信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_AI_SETINFO_ACK_S;

/*************************************6. DP_M2S_CMD_VI_GET *************************/
/*
 *说明： 获取视频输入设备信息
 *定义： DP_M2S_CMD_VI_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_VI_GET ；
 *	enDevId	： 视频输入设备ID，引用 DP_M2S_VI_DEV_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_VI_GETINFO_S {
	_DP_M2S_CMD_VI_GETINFO_S(DP_M2S_CMD_ID_E cmd, DP_M2S_VI_DEV_E dev) :
			stHeader(sizeof(_DP_M2S_CMD_VI_GETINFO_S), cmd, 0x01), enDevId(dev) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_VI_DEV_E enDevId;
} DP_M2S_CMD_VI_GETINFO_S;

/*
 *说明： 获取视频输入设备信息 应答
 *定义： DP_M2S_CMD_VI_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_VI_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *  stInfo		:  视频输入设备信息，引用 DP_M2S_VI_GET_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_VI_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_M2S_VI_GET_INFO_S stInfo;
} DP_M2S_CMD_VI_GETINFO_ACK_S;

/*************************************7. DP_M2S_CMD_VI_SET *************************/
/*
 *说明： 设置视频输入设备信息
 *定义： DP_M2S_CMD_VI_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_VI_SET ；
 *  stInfo		:  视频输入设备信息，引用 DP_M2S_VI_SET_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_VI_SETINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_VI_SET_INFO_S stInfo;
} DP_M2S_CMD_VI_SETINFO_S;

/* 设置视频输入设备信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_VI_SETINFO_ACK_S;

/*************************************8. DP_M2S_CMD_AVENC_GET *************************/
/*
 *说明： 获取音视频编码信息
 *定义： DP_M2S_CMD_AVENC_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_GET ；
 *	s32TskId	： 任务ID，详细参看“任务ID分配”介绍；
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVENC_GETINFO_S {
	_DP_M2S_CMD_AVENC_GETINFO_S(DP_M2S_CMD_ID_E cmd) :
			stHeader(sizeof(_DP_M2S_CMD_AVENC_GETINFO_S), cmd, 0x01), s32TskId(
					0) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_S32 s32TskId;
} DP_M2S_CMD_AVENC_GETINFO_S;

//common struct
typedef DP_M2S_CMD_AVENC_GETINFO_S DP_M2S_CMD_COMMON_GETINFO_S;

/*
 *说明： 获取音视频编码信息 应答
 *定义： DP_M2S_CMD_AVENC_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *  stInfo		:  音视频编码信息，引用 DP_M2S_AVENC_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVENC_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_M2S_AVENC_INFO_S stInfo;
} DP_M2S_CMD_AVENC_GETINFO_ACK_S;

/*************************************9. DP_M2S_CMD_AVENC_SET *************************/
/*
 *说明： 设置音视频编码信息
 *定义： DP_M2S_CMD_AVENC_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_SET ；
 *  stInfo		:  音视频编码信息，引用 DP_M2S_AVENC_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVENC_SETINFO_S {
	_DP_M2S_CMD_AVENC_SETINFO_S(DP_M2S_CMD_ID_E cmd) :
			stHeader(sizeof(_DP_M2S_CMD_AVENC_SETINFO_S), cmd, 0x01), stInfo() {
	}
	_DP_M2S_CMD_AVENC_SETINFO_S(DP_M2S_CMD_ID_E cmd, DP_U8 isReply) :
			stHeader(sizeof(_DP_M2S_CMD_AVENC_SETINFO_S), cmd, isReply), stInfo() {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_AVENC_INFO_S stInfo;
} DP_M2S_CMD_AVENC_SETINFO_S;

/* 设置音视频编码信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_AVENC_SETINFO_ACK_S;

/*************************************10. DP_M2S_CMD_AVENC_GET_BATCH *************************/
/*
 *说明： 批量获取音视频编码信息
 *定义： DP_M2S_CMD_AVENC_GETBATCHINFO_S
 *成员：
 *	stHeader	：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_GET_BATCH ；
 *	u32Nums		：音视频编码信息个数，须大于0
 *	as32TskId	：任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVENC_GETBATCHINFO_S {
	_DP_M2S_CMD_AVENC_GETBATCHINFO_S(DP_U32 packageLen, DP_M2S_CMD_ID_E cmd,
			DP_U32 nums) :
			stHeader(packageLen, cmd, 0x01), u32Nums(nums) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_AVENC_GETBATCHINFO_S;

typedef DP_M2S_CMD_AVENC_GETBATCHINFO_S DP_M2S_CMD_BATCH_COMMON_S;

/*
 *说明： 批量获取音视频编码信息 应答
 *定义： DP_M2S_CMD_AVENC_GETBATCHINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_GET_BATCH ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *	u32Nums		： 音视频编码信息个数，须大于0
 *	au32ErrInfo	： 错误信息数据（可变），成员个数等于 u32Nums ， 数据成员结构类型为 DP_U32（0:成功，非0；失败）
 *  astInfo		:  音视频编码信息数组（可变），成员个数等于 u32Nums ，数组成员结构类型为 DP_M2S_AVENC_INFO_S ;
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVENC_GETBATCHINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_U32 u32Nums; // equal set nums
	// DP_U32 au32ErrInfo[u32Nums];
	// DP_M2S_AVENC_INFO_S astInfo[u32Nums];
} DP_M2S_CMD_AVENC_GETBATCHINFO_ACK_S;

typedef DP_M2S_CMD_AVENC_GETBATCHINFO_ACK_S DP_M2S_CMD_COMMON_GETBATCHINFO_ACK_S;

/*************************************11. DP_M2S_CMD_AVENC_SET_BATCH *************************/
/*
 *说明： 批量设置音视频编码信息
 *定义： DP_M2S_CMD_AVENC_SETBATCHINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_SET_BATCH ；
 *	u32Nums		： 音视频编码信息个数，须大于0
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *  astInfo		:  音视频编码信息数组（可变），成员个数等于 u32Nums ，数组成员结构类型为 DP_M2S_AVENC_INFO_S ;
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVENC_SETBATCHINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
	// DP_M2S_AVENC_INFO_S astInfo[u32Nums];
} DP_M2S_CMD_AVENC_SETBATCHINFO_S;

/* 批量设置音视频编码信息 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_AVENC_SETBATCHINFO_ACK_S;

/*************************************12. DP_M2S_CMD_AVENC_OPEN_BATCH *************************/
/*
 *说明： 批量打开音视频编码
 *定义： DP_M2S_CMD_AVENC_OPENINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_OPEN_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_AVENC_OPENINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_AVENC_OPENINFO_S;

/* 批量打开音视频编码 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_AVENC_OPENINFO_ACK_S;

/*************************************13. DP_M2S_CMD_AVENC_CLOSE_BATCH *************************/
/*
 *说明： 批量关闭音视频编码
 *定义： DP_M2S_CMD_AVENC_CLOSEINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVENC_CLOSE_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_AVENC_CLOSEINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_AVENC_CLOSEINFO_S;

/* 批量关闭音视频编码 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_AVENC_CLOSEINFO_ACK_S;

/*************************************14. DP_M2S_CMD_AVDEC_GET *************************/
/*
 *说明： 获取音视频解码信息
 *定义： DP_M2S_CMD_AVDEC_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_GET ；
 *	s32TskId	： 任务ID，详细参看“任务ID分配”介绍；
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVDEC_GETINFO_S {
	_DP_M2S_CMD_AVDEC_GETINFO_S(DP_M2S_CMD_ID_E cmd) :
			stHeader(sizeof(_DP_M2S_CMD_AVDEC_GETINFO_S), cmd, 0x01), s32TskId(
					0) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_S32 s32TskId;
} DP_M2S_CMD_AVDEC_GETINFO_S;

/*
 *说明： 获取音视频解码信息 应答
 *定义： DP_M2S_CMD_AVDEC_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *  stInfo		:  音视频解码信息，引用 DP_M2S_AVDEC_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVDEC_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_M2S_AVDEC_INFO_S stInfo;
} DP_M2S_CMD_AVDEC_GETINFO_ACK_S;

/*************************************15. DP_M2S_CMD_AVDEC_SET *************************/
/*
 *说明： 设置音视频解码信息
 *定义： DP_M2S_CMD_AVDEC_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_SET ；
 *  stInfo		:  音视频解码信息，引用 DP_M2S_AVDEC_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVDEC_SETINFO_S {
	_DP_M2S_CMD_AVDEC_SETINFO_S(DP_M2S_CMD_ID_E cmd) :
			stHeader(sizeof(_DP_M2S_CMD_AVDEC_SETINFO_S), cmd, 0x01), stInfo() {
	}
	_DP_M2S_CMD_AVDEC_SETINFO_S(DP_M2S_CMD_ID_E cmd, DP_U8 isReply) :
			stHeader(sizeof(_DP_M2S_CMD_AVDEC_SETINFO_S), cmd, isReply), stInfo() {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_AVDEC_INFO_S stInfo;
} DP_M2S_CMD_AVDEC_SETINFO_S;

/* 设置音视频解码信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_AVDEC_SETINFO_ACK_S;

/*************************************16. DP_M2S_CMD_AVDEC_GET_BATCH *************************/
/*
 *说明： 批量获取音视频解码信息
 *定义： DP_M2S_CMD_AVDEC_GETBATCHINFO_S
 *成员：
 *	stHeader	：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_GET_BATCH ；
 *	u32Nums		：音视频解码信息个数，须大于0
 *	as32TskId	：任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVDEC_GETBATCHINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_AVDEC_GETBATCHINFO_S;

/*
 *说明： 批量获取音视频解码信息 应答
 *定义： DP_M2S_CMD_AVDEC_GETBATCHINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_GET_BATCH ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *	u32Nums		： 音视频解码信息个数，须大于0
 *	au32ErrInfo	： 错误信息数据（可变），成员个数等于 u32Nums ， 数据成员结构类型为 DP_U32（0:成功，非0；失败）
 *  astInfo		:  视频解码信息数组（可变），成员个数等于 u32Nums ，数组成员结构类型为 DP_M2S_AVDEC_INFO_S ;
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVDEC_GETBATCHINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_U32 u32Nums;
	// DP_U32 au32ErrInfo[u32Nums];
	// DP_M2S_AVDEC_INFO_S astInfo[u32Nums];
} DP_M2S_CMD_AVDEC_GETBATCHINFO_ACK_S;

/*************************************17. DP_M2S_CMD_AVDEC_SET_BATCH *************************/
/*
 *说明： 批量设置音视频解码信息
 *定义： DP_M2S_CMD_AVDEC_SETBATCHINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_SET_BATCH ；
 *	u32Nums		： 音视频解码信息个数，须大于0
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *  astInfo		:  视频解码信息数组（可变），成员个数等于 u32Nums ，数组成员结构类型为 DP_M2S_AVDEC_INFO_S ;
 *注意：无
 */
typedef struct _DP_M2S_CMD_AVDEC_SETBATCHINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
	// DP_M2S_AVDEC_INFO_S astInfo[u32Nums];
} DP_M2S_CMD_AVDEC_SETBATCHINFO_S;

/* 批量设置音视频解码信息 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_AVDEC_SETBATCHINFO_ACK_S;

/*************************************18. DP_M2S_CMD_AVDEC_OPEN_BATCH *************************/
/*
 *说明： 批量打开音视频解码
 *定义： DP_M2S_CMD_AVDEC_OPENINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_OPEN_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_AVDEC_OPENINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_AVDEC_OPENINFO_S;

/* 批量打开音视频解码 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_AVDEC_OPENINFO_ACK_S;

/*************************************19. DP_M2S_CMD_AVDEC_CLOSE_BATCH *************************/
/*
 *说明： 批量关闭音视频解码
 *定义： DP_M2S_CMD_AVDEC_CLOSEINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AVDEC_CLOSE_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_AVDEC_CLOSEINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_AVDEC_CLOSEINFO_S;

/* 批量关闭音视频解码 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_AVDEC_CLOSEINFO_ACK_S;

/*************************************20. DP_M2S_CMD_WINS_GET *************************/
/*
 *说明： 获取（视频）窗口信息
 *定义： DP_M2S_CMD_WINS_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_GET ；
 *	s32TskId	： 任务ID，详细参看“任务ID分配”介绍；
 *注意：无
 */
typedef struct _DP_M2S_CMD_WINS_GETINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_S32 s32TskId;
} DP_M2S_CMD_WINS_GETINFO_S;

/*
 *说明： 获取（视频）窗口信息 应答
 *定义： DP_M2S_CMD_WINS_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *	s32TskId	： 任务ID，详细参看“任务ID分配”介绍；
 *  stInfo		:  窗口信息，引用 DP_M2S_WINS_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_WINS_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_S32 s32TskId;
	DP_M2S_WINS_INFO_S stInfo;
} DP_M2S_CMD_WINS_GETINFO_ACK_S;

/*************************************21. DP_M2S_CMD_WINS_SET *************************/
/*
 *说明： 设置（视频）窗口信息
 *定义： DP_M2S_CMD_WINS_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_SET ；
 *	s32TskId	： 任务ID，详细参看“任务ID分配”介绍；
 *  stInfo		:  窗口信息，引用 DP_M2S_WINS_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_WINS_SETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_S32 s32TskId;
	DP_M2S_WINS_INFO_S stInfo;
} DP_M2S_CMD_WINS_SETINFO_S;

/* 设置（视频）窗口信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_WINS_SETINFO_ACK_S;

/*************************************22. DP_M2S_CMD_WINS_GET_BATCH *************************/
/*
 *说明： 批量获取（视频）窗口信息
 *定义： DP_M2S_CMD_WINS_GETBATCHINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_GET_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员s32WinNums影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_WINS_GETBATCHINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_WINS_GETBATCHINFO_S;

/*
 *说明： 批量获取（视频）窗口信息 应答
 *定义： DP_M2S_CMD_WINS_GETBATCHINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_GET_BATCH ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *	u32Nums		： 窗口数量，须大于0；
 *	au32ErrInfo	： 错误信息数据（可变），成员个数等于 u32Nums ， 数据成员结构类型为 DP_U32（0:成功，非0；失败）
 *  astInfo		:  窗口信息数组（可变），成员个数等于 u32Nums ，数组成员结构类型为 DP_M2S_WINS_INFO_S；
 *注意：无
 */
typedef struct _DP_M2S_CMD_WINS_GETBATCHINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_U32 u32Nums;
	// DP_U32 au32ErrInfo[u32Nums];
	// DP_M2S_WINS_INFO_S astInfo[u32Nums];
} DP_M2S_CMD_WINS_GETBATCHINFO_ACK_S;

/*************************************23. DP_M2S_CMD_WINS_SET_BATCH *************************/
/*
 *说明： 批量设置（视频）窗口信息 应答
 *定义： DP_M2S_CMD_WINS_SETBATCHINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_SET_BATCH ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *  astInfo		:  窗口信息数组（可变），成员个数等于 u32Nums ，数组成员结构类型为 DP_M2S_WINS_INFO_S；
 *注意：无
 */
typedef struct _DP_M2S_CMD_WINS_SETBATCHINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
	// DP_M2S_WINS_INFO_S astInfo[u32Nums];
} DP_M2S_CMD_WINS_SETBATCHINFO_S;

/* 批量设置（视频）窗口信息 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_WINS_SETBATCHINFO_ACK_S;

/*************************************24. DP_M2S_CMD_WINS_OPEN_BATCH *************************/
/*
 *说明： 批量打开（视频）窗口
 *定义： DP_M2S_CMD_WINS_OPENINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_OPEN_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_WINS_OPENINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_WINS_OPENINFO_S;

/* 批量打开（视频）窗口 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_WINS_OPENINFO_ACK_S;

/*************************************25. DP_M2S_CMD_WINS_CLOSE_BATCH *************************/
/*
 *说明： 批量关闭（视频）窗口
 *定义： DP_M2S_CMD_WINS_CLOSEINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_WINS_CLOSE_BATCH ；
 *	u32Nums		： 窗口数量，须大于0；
 *	as32TskId	： 任务ID数组(可变)，成员个数等于 u32Nums ，数组成员结构类型为DP_S32；任务ID，详细参看“任务ID分配”介绍；
 *注意：
 *	本结构为可变结构，成员 u32Nums 影响整个结构的大小；
 */
typedef struct _DP_M2S_CMD_WINS_CLOSEINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Nums;
	// DP_S32 as32TskId[u32Nums];
} DP_M2S_CMD_WINS_CLOSEINFO_S;

/* 批量打开（视频）窗口 应答 */
typedef DP_M2S_CMD_BATCH_ACK_S DP_M2S_CMD_WINS_CLOSEINFO_ACK_S;

/*************************************26. DP_M2S_CMD_AO_GET *************************/
/*
 *说明： 获取音频输出设备信息
 *定义： DP_M2S_CMD_AO_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AO_GET ；
 *	enDevId	： 音频输出设备ID，引用 DP_M2S_AO_DEV_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_AO_GETINFO_S {
	_DP_M2S_CMD_AO_GETINFO_S(DP_M2S_CMD_ID_E cmd, DP_M2S_AO_DEV_E devId) :
			stHeader(sizeof(_DP_M2S_CMD_AO_GETINFO_S), cmd, 0x01), enDevId(
					devId) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_AO_DEV_E enDevId;
} DP_M2S_CMD_AO_GETINFO_S;

/*
 *说明： 获取音频输出设备信息 应答
 *定义： DP_M2S_CMD_AO_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AO_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *  stInfo		:  音频输出设备信息，引用 DP_M2S_AO_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AO_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_M2S_AO_INFO_S stInfo;
} DP_M2S_CMD_AO_GETINFO_ACK_S;

/*************************************27. DP_M2S_CMD_AO_SET *************************/
/*
 *说明： 设置音频输出设备信息
 *定义： DP_M2S_CMD_AO_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_AO_SET ；
 *  stInfo		:  音频输出设备信息，引用 DP_M2S_AO_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_AO_SETINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_AO_INFO_S stInfo;
} DP_M2S_CMD_AO_SETINFO_S;

/* 设置音频输出设备信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_AO_SETINFO_ACK_S;

/*************************************28. DP_M2S_CMD_VO_GET *************************/
/*
 *说明： 获取视频输出设备信息
 *定义： DP_M2S_CMD_VO_GETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_VO_GET ；
 *	enDevId	： 视频输出设备ID，引用 DP_M2S_VO_DEV_E
 *注意：无
 */
typedef struct _DP_M2S_CMD_VO_GETINFO_S {
	_DP_M2S_CMD_VO_GETINFO_S(DP_M2S_CMD_ID_E cmd, DP_M2S_VO_DEV_E devId) :
			stHeader(sizeof(_DP_M2S_CMD_VO_GETINFO_S), cmd, 0x01), enDevId(
					devId) {
	}
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_VO_DEV_E enDevId;
} DP_M2S_CMD_VO_GETINFO_S;

/*
 *说明： 获取视频输出设备信息 应答
 *定义： DP_M2S_CMD_VO_GETINFO_ACK_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_VO_GET ；
 *	u32Success	： 0:成功，其他失败，返回错误码
 *  stInfo		:  视频输出设备信息，引用 DP_M2S_VO_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_VO_GETINFO_ACK_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
	DP_M2S_VO_INFO_S stInfo;
} DP_M2S_CMD_VO_GETINFO_ACK_S;

/*************************************29. DP_M2S_CMD_VO_SET *************************/
/*
 *说明： 设置视频输出设备信息
 *定义： DP_M2S_CMD_VO_SETINFO_S
 *成员：
 *	stHeader	： 协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_VO_SET ；
 *  stInfo		:  视频输出设备信息，引用 DP_M2S_VO_INFO_S
 *注意：无
 */
typedef struct _DP_M2S_CMD_VO_SETINFO_S {

	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_VO_INFO_S stInfo;
} DP_M2S_CMD_VO_SETINFO_S;

/* 设置视频输出设备信息 应答 */
typedef DP_M2S_CMD_ACK_S DP_M2S_CMD_VO_SETINFO_ACK_S;

#pragma pack(pop)

#endif

#endif

