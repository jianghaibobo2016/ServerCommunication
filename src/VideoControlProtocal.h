/**
* 文件: dp_m2s_prot.h
* 简要: 编解码模块控制协议
* 详述：
	该协议主要是用于（设备节点）编解码模块与服务逻辑模块之间的内部控制操作。
* 作者：lyw
* 版本：v00.01
* 日期：2018/10/15
*/

#ifndef __DP_M2S_PROT_H__
#define __DP_M2S_PROT_H__
#if 0
#include "dp_sd_type.h"

#pragma pack(push)
#pragma pack(1)
/* 协议版本号 */
#define DP_M2S_INF_PROT_VERSION			"00.01"
/* 协议通信接口定义 */
#define DP_M2S_INF_PROT_UNIX_FIFO		"/tmp/UNIX_AV2APP"
/* 协议固定包头标识 */
#define DP_M2S_INF_PROT_PKT_HEAD		0xF3F4
/* 协议固定包头长度 */
#define DP_M2S_INF_PROT_PKT_HEAD_LEN 	5
/* URL地址长度 */
#define DP_M2S_URL_LEN 256
/* 文字叠加长度（Bytes） */
#define DP_M2S_OSD_STRING_LEN 32 

// #define DP_M2S_AI_DEV_MAX 2
// #define DP_M2S_AO_DEV_MAX 2
// #define DP_M2S_VI_DEV_MAX 2
// #define DP_M2S_VO_DEV_MAX 2
// #define DP_M2S_AENC_DEV_MAX 2
// #define DP_M2S_VENC_DEV_MAX 2
// #define DP_M2S_ADEC_DEV_MAX 2
// #define DP_M2S_VDEC_DEV_MAX 2

enum DP_M2S_AI_DEV_ID_E{
	DP_M2S_AI_DEV_ID_0=0x0,
	DP_M2S_AI_DEV_ID_1,
	DP_M2S_AI_DEV_ID_BUTT
};

enum DP_M2S_AO_DEV_ID_E{
	
	DP_M2S_AO_DEV_ID_0=0x1000,
	DP_M2S_AO_DEV_ID_1,
	DP_M2S_AO_DEV_ID_BUTT
};

enum DP_M2S_VI_DEV_ID_E{
	
	DP_M2S_VI_DEV_ID_0=0x2000,
	DP_M2S_VI_DEV_ID_1,
	DP_M2S_VI_DEV_ID_BUTT
};

enum DP_M2S_VO_DEV_ID_E{
	
	DP_M2S_VO_DEV_ID_0=0x3000,
	DP_M2S_VO_DEV_ID_1,
	DP_M2S_VO_DEV_ID_BUTT
};

enum DP_M2S_AENC_CHN_ID_E{
	
	DP_M2S_AENC_CHN_ID_0=0x4000,
	DP_M2S_AENC_CHN_ID_1,
	DP_M2S_AENC_CHN_ID_BUTT
};

enum DP_M2S_VENC_CHN_ID_E{
	
	DP_M2S_VENC_CHN_ID_0=0x5000,
	DP_M2S_VENC_CHN_ID_1,
	DP_M2S_VENC_CHN_ID_BUTT
};

enum DP_M2S_ADEC_CHN_ID_E{
	
	DP_M2S_ADEC_CHN_ID_0=0x6000,
	DP_M2S_ADEC_CHN_ID_1,
	DP_M2S_ADEC_CHN_ID_BUTT
};

enum DP_M2S_VDEC_CHN_ID_E{
	
	DP_M2S_VDEC_CHN_ID_0=0x7000,
	DP_M2S_VDEC_CHN_ID_1,
	DP_M2S_VDEC_CHN_ID_BUTT
};

/**************************************************************************************************************/
/***************************************************公共数据结构声明***********************************************/
/**************************************************************************************************************/
/*
*说明： 与服务逻辑模块通信的协议包头
*定义： DP_M2S_INF_PROT_HEAD_S
*成员：
*	u16HeadTag	：包头标识，引用DP_M2S_INF_PROT_UNIX_FIFO
*	u16PacketLen：Bit15表示确认包返回标识，0表示不需要返回，1表示需要返回；Bit[14:0]表示数据包长度（固定包头长度+业务数据长度），范围[5~32767]Bytes；
*	u8CommandID	：数据指令类型，引用DP_M2S_CMD_ID_E
*注意：无
*/
typedef struct _DP_M2S_INF_PROT_HEAD_S {
	
	DP_U16 u16HeadTag;
	DP_U16 u16PacketLen;	
	DP_U8 u8CommandID;
} DP_M2S_INF_PROT_HEAD_S;

/*
*说明： 视频时序
*定义： DP_M2S_VIDEO_SYNC_E
*成员：
	DP_M2S_VIDEO_SYNC_720P60	：分辨率1280x720，帧率60
	DP_M2S_VIDEO_SYNC_1080P60	：分辨率1920x1080，帧率60
	DP_M2S_VIDEO_SYNC_4K30		：分辨率3840x2160，帧率30
*注意：无
*/
typedef enum _DP_M2S_VIDEO_SYNC_E{
	
	DP_M2S_VIDEO_SYNC_720P60=0x0,
	DP_M2S_VIDEO_SYNC_1080P60,
	DP_M2S_VIDEO_SYNC_4K30,
	DP_M2S_VIDEO_SYNC_BUTT
} DP_M2S_VIDEO_SYNC_E;

/*
*说明： 音频采样率
*定义： DP_M2S_AUDIO_SAMPLE_RATE_E
*成员：
	DP_M2S_AUDIO_SAMPLE_RATE_8000	：8K采样率
	DP_M2S_AUDIO_SAMPLE_RATE_16000	：16K采样率
	DP_M2S_AUDIO_SAMPLE_RATE_32000	：32K采样率
	DP_M2S_AUDIO_SAMPLE_RATE_44100	：44.1K采样率
	DP_M2S_AUDIO_SAMPLE_RATE_48000	：48K采样率
*注意：无
*/
typedef enum _DP_M2S_AUDIO_SAMPLE_RATE_E{
	
	DP_M2S_AUDIO_SAMPLE_RATE_8000=8000,
	DP_M2S_AUDIO_SAMPLE_RATE_16000=16000,
	DP_M2S_AUDIO_SAMPLE_RATE_32000=32000,
	DP_M2S_AUDIO_SAMPLE_RATE_44100=44100,
	DP_M2S_AUDIO_SAMPLE_RATE_48000=48000,
	DP_M2S_AUDIO_SAMPLE_RATE_BUTT
}DP_M2S_AUDIO_SAMPLE_RATE_E;

/*
*说明： 音频采样精度
*定义： DP_M2S_AUDIO_BIT_WIDTH_E
*成员：
	DP_M2S_AUDIO_BIT_WIDTH_8	：8位采样率精度
	DP_M2S_AUDIO_BIT_WIDTH_16	：16位采样率精度
*注意：无
*/
typedef enum _DP_M2S_AUDIO_BIT_WIDTH_E{
	
	DP_M2S_AUDIO_BIT_WIDTH_8=0x0,
	DP_M2S_AUDIO_BIT_WIDTH_16=0x1,
	DP_M2S_AUDIO_BIT_WIDTH_BUTT
}DP_M2S_AUDIO_BIT_WIDTH_E;

/*
*说明： 音频声道模式
*定义： DP_M2S_AUDIO_BIT_WIDTH_E
*成员：
	DP_M2S_AUDIO_SOUND_MODE_MONO	：单声道
	DP_M2S_AUDIO_SOUND_MODE_STEREO	：立体声
*注意：无
*/
typedef enum _DP_M2S_AUDIO_SOUND_MODE_E{
	
	DP_M2S_AUDIO_SOUND_MODE_MONO=0x0,
	DP_M2S_AUDIO_SOUND_MODE_STEREO=0x1,
	DP_M2S_AUDIO_SOUND_MODE_BUTT
} DP_M2S_AUDIO_SOUND_MODE_E;

/*
*说明： 音频编码类型
*定义： DP_M2S_AUDIO_CODEC_TYPE_E
*成员：
	DP_M2S_AUDIO_CODEC_TYPE_AAC	：AAC-LC编码
	DP_M2S_AUDIO_CODEC_TYPE_PCM	：PCM编码
*注意：无
*/
typedef enum _DP_M2S_AUDIO_CODEC_TYPE_E{
	
	DP_M2S_AUDIO_CODEC_TYPE_AAC=0x0,
	DP_M2S_AUDIO_CODEC_TYPE_PCM,
	DP_M2S_AUDIO_CODEC_TYPE_BUTT
}DP_M2S_AUDIO_CODEC_TYPE_E;

/*
*说明： 音频公共属性
*定义： DP_M2S_AUDIO_COMM_ATTR_S
*成员：
	DP_M2S_AUDIO_SAMPLE_RATE_E	：音频采样率
	DP_M2S_AUDIO_BIT_WIDTH_E	：音频采样精度
	DP_M2S_AUDIO_SOUND_MODE_E	：音频声道模式
*注意：无
*/
typedef struct _DP_M2S_AUDIO_COMM_ATTR_S{
	
	DP_M2S_AUDIO_SAMPLE_RATE_E enSamplerate;
	DP_M2S_AUDIO_BIT_WIDTH_E enBitwidth;
	DP_M2S_AUDIO_SOUND_MODE_E enSoundmode;
}DP_M2S_AUDIO_COMM_ATTR_S;

/*
*说明： 流媒体类型
*定义： DP_M2S_STREAM_TYPE_E
*成员：
	DP_M2S_STREAM_RTSP	：rtsp
	DP_M2S_STREAM_RTMP	：rtmp
*注意：无
*/
typedef enum _DP_M2S_STREAM_TYPE_E{
	
	DP_M2S_STREAM_RTSP=0x0,
	DP_M2S_STREAM_RTMP,
	DP_M2S_STREAM_BUTT
}DP_M2S_STREAM_TYPE_E;

/*
*说明： 流媒体客户端属性
*定义： DP_M2S_STREAM_CLIENT_ATTR_S
*成员：
	enType			： 流媒体类型，引用 DP_M2S_STREAM_TYPE_E
	bOpen			： 客户端开关，0：关，1：开；
	bMulticast		:  是否支持组播，0：单播，1：组播
	s32TransType	:  传输类型，0：tcp，1：udp
	s32ConnTimeout	:  连接超时，范围0~300秒
	au8Url[]		:  连接地址url
*注意：无
*/
typedef struct _DP_M2S_STREAM_CLIENT_ATTR_S {
	DP_M2S_STREAM_TYPE_E enType;
	DP_BOOL bOpen;
	DP_BOOL bMulticast;
	DP_S32	s32TransType;
	DP_S32	s32ConnTimeout;
	DP_U8 au8Url[DP_M2S_URL_LEN];
}DP_M2S_STREAM_CLIENT_ATTR_S;

/*
*说明： 音频解码属性
*定义： DP_M2S_ADEC_ATTR_S
*成员：
	enChnId		： 解码通道ID，引用 DP_M2S_ADEC_CHN_ID_E
	enCodecType	： 解码类型，引用 DP_M2S_AUDIO_CODEC_TYPE_E
*注意：无
*/
typedef struct _DP_M2S_ADEC_ATTR_S {

	DP_M2S_ADEC_CHN_ID_E enChnId;
	DP_M2S_AUDIO_CODEC_TYPE_E enCodecType;
}DP_M2S_ADEC_ATTR_S;

/*
*说明： 音频输出设备属性
*定义： DP_M2S_AO_ATTR_S
*成员：
	enDevId		： 输出设备ID，引用 DP_M2S_AO_DEV_ID_E
	u8Vol		： 输出音量，范围0~100
	stCommAttr	：音频公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S
*注意：无
*/
typedef struct _DP_M2S_AO_ATTR_S {
	
	DP_M2S_AO_DEV_ID_E enDevId;
	DP_U8 u8Vol;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
}DP_M2S_AO_ATTR_S;

/*
*说明： 坐标
*定义： DP_M2S_COORD_S
*成员：
	s32X 	：起始坐标x
	s32Y	：起始坐标y
*注意：无
*/
typedef struct _DP_M2S_COORD_S {
		
	DP_S32 s32X;
	DP_S32 s32Y;
}DP_M2S_COORD_S;

/*
*说明： 矩阵参数
*定义： DP_M2S_RECT_S
*成员：
	s32X		： 起始坐标x
	s32Y		： 起始坐标y
	u32Width	： 宽度
	u32Height	： 高度
*注意：无
*/
typedef struct _DP_M2S_RECT_S {
	
	DP_S32 s32X;
	DP_S32 s32Y;
	DP_U32 u32Width;
	DP_U32 u32Height;
}DP_M2S_RECT_S;

/*
*说明： 视频输入设备属性
*定义： DP_M2S_VI_DEV_ATTR_S
*成员：
	bConn		： 视频输入设备是否连接，0：连接；1：断开
	bSignal		： 视频输入设备信号是否正常，0：正常，1：异常
	s32FrmRate	： 帧率，范围0~60
	u32Width	： 宽度，范围0~3840
	u32Height	： 高度，范围0~2160
*注意：无
*/
typedef struct _DP_M2S_VI_DEV_ATTR_S {
	
	DP_BOOL bConn;
	DP_BOOL bSignal;
	DP_S32	s32FrmRate
	DP_U32	u32Width;
	DP_U32	u32Height;
}DP_M2S_VI_DEV_ATTR_S;

/*
*说明： 视频输入裁剪属性
*定义： DP_M2S_VI_CROP_ATTR_S
*成员：
	bEnable		： 裁剪开关，0：开启；1：禁止
	stRect		： 裁剪目标矩阵参数，引用 DP_M2S_RECT_S
*注意：无
*/
typedef struct _DP_M2S_VI_CROP_ATTR_S {
	
	DP_BOOL bEnable;
	DP_M2S_RECT_S stRect;
}DP_M2S_VI_CROP_ATTR_S;


/*
*说明： 图像缩放类型
*定义： DP_M2S_ZOOM_TYPE_E
*成员：
	DP_M2S_ZOOM_RECT		： 按矩形方式缩放，对应结构 DP_M2S_RECT_S
	DP_M2S_ZOOM_RATIO		： 按比例方式缩放，对应结构 DP_M2S_ZOOM_RATIO_S
*注意：无
*/
typedef enum _DP_M2S_ZOOM_TYPE_E {
	
	DP_M2S_ZOOM_RECT=0x0,
	DP_M2S_ZOOM_RATIO,
	DP_M2S_ZOOM_BUTT
}DP_M2S_ZOOM_TYPE_E;

/*
*说明： 图像比例缩放
*定义： DP_M2S_ZOOM_RATIO_S
*成员：
	u32XRatio		： 屏幕坐标上，待缩放区域起始点横坐标与显示通道宽的比例
	u32YRatio		： 屏幕坐标上，待缩放区域起始点纵坐标与显示通道高的比例
	u32WRatio		： 屏幕坐标上，待缩放区域宽与显示通道宽的比例
	u32HRatio		： 屏幕坐标上，待缩放区域高与显示通道高的比例
*注意：无
*/
typedef struct _DP_M2S_ZOOM_RATIO_S {
	
	DP_U32 u32XRatio;
	DP_U32 u32YRatio;
	DP_U32 u32WRatio;
	DP_U32 u32HRatio;
}DP_M2S_ZOOM_RATIO_S;

/*
*说明： 编码码率模式
*定义： DP_M2S_VENC_RC_MODE_E
*成员：
	DP_M2S_VENC_RC_MODE_H264CBR		： h264固定码流模式
	DP_M2S_VENC_RC_MODE_H264VBR		： h264可变码流模式
*注意：无
*/
typedef enum _DP_M2S_VENC_RC_MODE_E {
	
	DP_M2S_VENC_RC_MODE_H264CBR=0x0,
	DP_M2S_VENC_RC_MODE_H264VBR,
	DP_M2S_VENC_RC_MODE_BUTT
}DP_M2S_VENC_RC_MODE_E;

/*
*说明： 视频编码类型
*定义： DP_M2S_VIDEO_CODEC_TYPE_E
*成员：
	DP_M2S_VIDEO_CODEC_H264		： h264编码类型
	DP_M2S_VIDEO_CODEC_H265		： h265编码类型
	
*注意：无
*/
typedef enum _DP_M2S_VIDEO_CODEC_TYPE_E {
	
	DP_M2S_VIDEO_CODEC_H264=0x0,
	DP_M2S_VIDEO_CODEC_H265,
	DP_M2S_VIDEO_CODEC_BUTT
}DP_M2S_VIDEO_CODEC_TYPE_E;

/*
*说明： 编码Profile类型
*定义： DP_M2S_VIDEO_PROFILE_E
*成员：
	DP_M2S_VIDEO_PROFILE_BP	：Baseline
	DP_M2S_VIDEO_PROFILE_MP	：Main Profile
	DP_M2S_VIDEO_PROFILE_HP	: High Profile
	
*注意：无
*/
typedef enum _DP_M2S_VIDEO_PROFILE_E {
	
	DP_M2S_VIDEO_PROFILE_BP=0x0,
	DP_M2S_VIDEO_PROFILE_MP,
	DP_M2S_VIDEO_PROFILE_HP,
	DP_M2S_VIDEO_CODEC_BUTT
}DP_M2S_VIDEO_PROFILE_E;

/*
*说明： 编码属性参数
*定义： DP_M2S_VENC_ATTR_S
*成员：
	u32FrmRate	： 帧率，范围：1~60
	enRcMode	： 码率模式，引用 DP_M2S_VENC_RC_MODE_E
	u32Bitrate	： 码率范围：128Kbps~40Mbps
	enType		:  编码类型，引用 DP_M2S_VIDEO_CODEC_TYPE_E
	enProfile	： 编码profile，引用 DP_M2S_VIDEO_PROFILE_E
	u32Gop		： Gop范围：1~120
	u16SF		： 空域，预留
	u16TF		： 时域，预留
*注意：无
*/
typedef struct _DP_M2S_VENC_ATTR_S {

	DP_U32 u32FrmRate;
	DP_M2S_VENC_RC_MODE_E enRcMode; 
	DP_U32 u32Bitrate;
	DP_M2S_VIDEO_CODEC_TYPE_E enType;
	DP_M2S_VIDEO_PROFILE_E enProfile;
	DP_U32 u32Gop;
	DP_U16 u16SF;
	DP_U16 u16TF;
}DP_M2S_VENC_ATTR_S;

/*
*说明： 编码缩放属性
*定义： DP_M2S_ZOOM_ATTR_S
*成员：
	enType		： 图像缩放类型，引用 DP_M2S_ZOOM_TYPE_E
	stZoomRect	： 按矩形方式缩放，对应结构 DP_M2S_RECT_S
	stZoomRatio	:  按比例方式缩放，对应结构 DP_M2S_ZOOM_RATIO_S
*注意：无
*/
typedef struct _DP_M2S_ZOOM_ATTR_S {
	DP_M2S_ZOOM_TYPE_E enType;
	union {
		DP_M2S_RECT_S stZoomRect;
		DP_M2S_ZOOM_RATIO_S stZoomRatio;
	};
}DP_M2S_ZOOM_ATTR_S;

/*
*说明： 图像内容叠加类型
*定义： DP_M2S_OSD_TYPE_E
*成员：
	DP_M2S_OSD_PIC		： 图片
	DP_M2S_OSD_STRING	： 字符串文字
*注意：无
*/
typedef enum _DP_M2S_OSD_TYPE_E {
	
	DP_M2S_OSD_PIC=0x0,
	DP_M2S_OSD_STRING,
	DP_M2S_OSD_BUTT
}DP_M2S_OSD_TYPE_E;

/*
*说明： 显示位置模式
*定义： DP_M2S_OSD_DISPLAY_MODE_E
*成员：
	DP_M2S_OSD_DISPLAY_MODE_COORD		： 坐标方式
	DP_M2S_OSD_DISPLAY_MODE_LTOP		： 左上
	DP_M2S_OSD_DISPLAY_MODE_RTOP		： 右上
	DP_M2S_OSD_DISPLAY_MODE_LBOTTOM		： 左下
	DP_M2S_OSD_DISPLAY_MODE_RBOTTOM		： 右下
*注意：无
*/
typedef enum _DP_M2S_OSD_DISPLAY_MODE_E {
	
	DP_M2S_OSD_DISPLAY_MODE_COORD=0x0,
	DP_M2S_OSD_DISPLAY_MODE_LTOP,
	DP_M2S_OSD_DISPLAY_MODE_RTOP,
	DP_M2S_OSD_DISPLAY_MODE_LBOTTOM,
	DP_M2S_OSD_DISPLAY_MODE_RBOTTOM,
	DP_M2S_OSD_DISPLAY_MODE_BUTT
}DP_M2S_OSD_DISPLAY_MODE_E;


/*
*说明： 图像叠加属性
*定义： DP_M2S_OSD_ATTR_S
*成员：
	bEnable			： 叠加使能，0：开启，1：禁止；
	enType			： 图像内容叠加类型，引用 DP_M2S_OSD_TYPE_E
	stStr.au8Str	:  字符串文字内容，长度： DP_M2S_OSD_STRING_LEN
	stStr.u32Color	:  字符串文字颜色，支持RGB888颜色输入
	au8PicPath[]	:  图片路径，长度最大： DP_M2S_OSD_PIC_PATH_LEN
	enDispMode		:  显示位置模式，引用 DP_M2S_OSD_DISPLAY_MODE_E
	stCoord			:  坐标，引用 DP_M2S_COORD_S
*注意：无
*/
typedef struct _DP_M2S_OSD_ATTR_S {
	DP_BOOL bEnable;
	DP_M2S_OSD_TYPE_E enType;
	union {
		struct stStr {
			DP_U8 au8Str[DP_M2S_OSD_STRING_LEN];
			DP_U32 u32Color;
		};
		DP_U8 au8PicPath[DP_M2S_OSD_PIC_PATH_LEN];
	};
	DP_M2S_OSD_DISPLAY_MODE_E enDispMode;
	DP_M2S_COORD_S stCoord;
}DP_M2S_OSD_ATTR_S;


/*
*说明： 流媒体服务端属性
*定义： DP_M2S_STREAM_SERVER_ATTR_S
*成员：
	enType			： 流媒体类型，引用 DP_M2S_STREAM_TYPE_E
	bOpen			： 客户端开关，0：关，1：开；
	bMulticast		:  是否支持组播，0：单播，1：组播
	s32TransType	:  传输类型，0：tcp，1：udp
	s32ConnTimeout	:  连接超时，范围0~300秒
	s32ConnMax		： 服务端可支持的最大连接数目
	s32ConnNums		： 服务端当前连接数目
	au8Url[]		:  连接地址url
*注意：无
*/
typedef struct _DP_M2S_STREAM_SERVER_ATTR_S {
	
	DP_M2S_STREAM_TYPE_E enType;
	DP_BOOL bOpen;
	DP_S32	s32TransType;
	DP_S32	s32ConnTimeout;
	DP_S32	s32ConnMax;
	DP_S32	s32ConnNums;
	DP_U8 au8Url[DP_M2S_URL_LEN];
}DP_M2S_STREAM_SERVER_ATTR_S;

/**************************************************************************************************************/
/**********************************************信息类型以及结构声明********************************************/
/**************************************************************************************************************/
/*
*说明：信息类型
*定义：DP_M2S_INFO_TYPE_E
*成员：
	DP_M2S_INFO_TYPE_GET_AI		: 获取音频输入设备信息，引用结构： DP_M2S_AI_GET_INFO_S
	DP_M2S_INFO_TYPE_SET_AI		: 设置音频输入设备信息，引用结构： DP_M2S_AI_SET_INFO_S
	DP_M2S_INFO_TYPE_GET_AENC	: 获取音频编码通道信息，引用结构： DP_M2S_AENC_GET_INFO_S
	DP_M2S_INFO_TYPE_SET_AENC	: 设置音频编码通道信息，引用结构： DP_M2S_AENC_SET_INFO_S
	DP_M2S_INFO_TYPE_GET_ADEC	: 获取音频解码通道信息，引用结构： DP_M2S_ADEC_GET_INFO_S
	DP_M2S_INFO_TYPE_SET_ADEC	: 设置音频解码通道信息，引用结构： DP_M2S_ADEC_SET_INFO_S
	// DP_M2S_INFO_TYPE_GET_AO		:
	// DP_M2S_INFO_TYPE_SET_AO		:
	
	DP_M2S_INFO_TYPE_GET_VI		: 获取视频输入设备信息，引用结构： DP_M2S_VI_GET_INFO_S
	DP_M2S_INFO_TYPE_SET_VI		: 设置视频输入设备信息，引用结构： DP_M2S_VI_SET_INFO_S
	DP_M2S_INFO_TYPE_GET_VENC	: 获取视频编码通道信息，引用结构： DP_M2S_VENC_GET_INFO_S
	DP_M2S_INFO_TYPE_SET_VENC	: 设置视频编码通道信息，引用结构： DP_M2S_VENC_SET_INFO_S
	DP_M2S_INFO_TYPE_GET_VDEC	:
	DP_M2S_INFO_TYPE_SET_VDEC	:
	DP_M2S_INFO_TYPE_GET_VO		:
	DP_M2S_INFO_TYPE_SET_VO		:
*注意：无
*/

typedef enum _DP_M2S_INFO_TYPE_E{
	DP_M2S_INFO_TYPE_GET_AI=0x0,
	DP_M2S_INFO_TYPE_SET_AI,
	DP_M2S_INFO_TYPE_GET_AENC,
	DP_M2S_INFO_TYPE_SET_AENC,
	DP_M2S_INFO_TYPE_GET_ADEC,
	DP_M2S_INFO_TYPE_SET_ADEC,
	// DP_M2S_INFO_TYPE_GET_AO,
	// DP_M2S_INFO_TYPE_SET_AO,
	
	DP_M2S_INFO_TYPE_GET_VI=0xF000,
	DP_M2S_INFO_TYPE_SET_VI,
	DP_M2S_INFO_TYPE_GET_VENC,
	DP_M2S_INFO_TYPE_SET_VENC,
	DP_M2S_INFO_TYPE_GET_VDEC,
	DP_M2S_INFO_TYPE_SET_VDEC,
	DP_M2S_INFO_TYPE_GET_VO,
	DP_M2S_INFO_TYPE_SET_VO,
	
	DP_M2S_INFO_TYPE_BUTT
}DP_M2S_INFO_TYPE_E;

/*
*说明： 获取音频输入设备信息
*定义： DP_M2S_AI_GET_INFO_S
*成员：
	s32DevId		：音频输入设备ID
	isConn			：音频输入设备连接状态
	isSignal		：音频输入设备信号状态
	u8Vol			：音频输入设备音量0-100	
	isSelfAdaptive	：音频输入自适应
	stCommAttr		：音频输入设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S
*注意：
	对应于 DP_M2S_INFO_TYPE_GET_AI ；
*/
typedef struct _DP_M2S_AI_GET_INFO_S {
	
	DP_M2S_AI_DEV_ID_E s32DevId;
	DP_BOOL isConn;
	DP_BOOL isSignal;
	DP_U8 u8Vol;
	DP_BOOL isSelfAdaptive;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
}DP_M2S_AI_GET_INFO_S;

/*
*说明： 设置音频输入设备信息
*定义： DP_M2S_AI_SET_INFO_S
*成员：
	s32DevId		：音频输入设备ID
	u8Vol			：音频输入设备音量0-100	
	isSelfAdaptive	：音频输入自适应
	stCommAttr		：音频输入设备公共属性，引用 DP_M2S_AUDIO_COMM_ATTR_S
*注意：
	对应于 DP_M2S_INFO_TYPE_SET_AI ；
*/
typedef struct _DP_M2S_AI_SET_INFO_S {
	
	DP_M2S_AI_DEV_ID_E s32DevId;
	DP_U8 u8Vol;
	DP_BOOL isSelfAdaptive;
	DP_M2S_AUDIO_COMM_ATTR_S stCommAttr;
}DP_M2S_AI_SET_INFO_S;

/*
*说明： 获取音频编码通道信息
*定义： DP_M2S_AENC_GET_INFO_S
*成员：
	enAencChnId		：音频编码通道ID，引用 DP_M2S_AENC_CHN_ID_E
	enAiDevId		：音频编码绑定的音频输入设备ID，引用 DP_M2S_AI_DEV_ID_E
	enCodecType		：音频编码类型，引用 DP_M2S_AUDIO_CODEC_TYPE_E
	s32Bitrate		: 音频编码码率，范围 32~192Kbps；
*注意：
	对应于 DP_M2S_INFO_TYPE_GET_AENC ；
*/
typedef struct _DP_M2S_AENC_GET_INFO_S{
	
	DP_M2S_AENC_CHN_ID_E enAencChnId;
	DP_M2S_AI_DEV_ID_E enAiDevId;
	DP_M2S_AUDIO_CODEC_TYPE_E enCodecType;
	DP_S32 s32Bitrate;
}DP_M2S_AENC_GET_INFO_S;

// 说明： 设置音频编码通道信息
typedef struct DP_M2S_AENC_GET_INFO_S DP_M2S_AENC_SET_INFO_S;

/*
*说明： 获取音频解码通道信息
*定义： DP_M2S_ADEC_GET_INFO_S
*成员：
	stStreamAttr	: 
	stAdecAttr		:
	stAoAttr		:
*注意：
	对应于 DP_M2S_INFO_TYPE_GET_ADEC ；
*/
typedef struct _DP_M2S_ADEC_GET_INFO_S{
	
	DP_M2S_STREAM_CLIENT_ATTR_S stStreamAttr;
	DP_M2S_ADEC_ATTR_S stAdecAttr;
	DP_M2S_AO_ATTR_S stAoAttr;
}DP_M2S_ADEC_GET_INFO_S;

// 说明： 设置音频解码通道信息
typedef struct DP_M2S_ADEC_GET_INFO_S DP_M2S_ADEC_SET_INFO_S;

/*
*说明： 获取视频输入设备信息
*定义： DP_M2S_VI_GET_INFO_S
*成员：
	s32DevId	：视频输入设备ID
	stDevAttr	：视频输入设备属性，引用 DP_M2S_VI_DEV_ATTR_S
	stCropAttr	：视频输入裁剪属性，引用 DP_M2S_VI_CROP_ATTR_S
*注意：
	对应于 DP_M2S_INFO_TYPE_GET_VI ；
*/
typedef struct _DP_M2S_VI_GET_INFO_S {
	
	DP_M2S_VI_DEV_ID_E s32DevId;
	DP_M2S_VI_DEV_ATTR_S stDevAttr;
	DP_M2S_VI_CROP_ATTR_S stCropAttr;
}DP_M2S_VI_GET_INFO_S;

// 说明： 设置视频输入设备信息
typedef struct DP_M2S_VI_GET_INFO_S DP_M2S_VI_SET_INFO_S;


/*
*说明： 获取视频编码通道信息
*定义： DP_M2S_VENC_GET_INFO_S
*成员：
	enVencChnId	： 视频编码通道ID
	enViDevId	： 视频输入编码通道ID
	stVencAttr	： 编码属性参数
	stZoomAttr	： 编码缩放属性
	stOsdAttr	： 图像叠加属性
	stServerAttr： 流媒体服务端属性
*注意：
	对应于 DP_M2S_INFO_TYPE_GET_VENC ；
*/
typedef struct _DP_M2S_VENC_GET_INFO_S{
	
	DP_M2S_VENC_CHN_ID_E enVencChnId;
	DP_M2S_VI_DEV_ID_E enViDevId;
	
	DP_M2S_VENC_ATTR_S stVencAttr;
	DP_M2S_ZOOM_ATTR_S stZoomAttr;
	DP_M2S_OSD_ATTR_S stOsdAttr;
	DP_M2S_STREAM_SERVER_ATTR_S stServerAttr;
}DP_M2S_VENC_GET_INFO_S;

// 说明： 设置视频编码通道信息
typedef DP_M2S_VENC_GET_INFO_S DP_M2S_VENC_SET_INFO_S;


/**************************************************************************************************************/
/***************************************************控制指令操作类型 声明***********************************************/
/**************************************************************************************************************/
/*
*说明：控制指令操作类型
*定义：DP_M2S_CMD_ID_E
*成员：
	DP_M2S_CMD_INIT		：初始化系统资源；
	DP_M2S_CMD_DEINIT	：去初始化系统资源；
	DP_M2S_CMD_EXIT		：编解码模块程序退出
	DP_M2S_CMD_GETINFO	：获取信息
	DP_M2S_CMD_SETINFO	：设置信息
*注意：
	须使用获取或设置指令前先初始化系统资源；
*/
typedef enum _DP_M2S_CMD_ID_E {
	
	DP_M2S_CMD_INIT=0x0,
	DP_M2S_CMD_DEINIT,
	DP_M2S_CMD_EXIT,
	DP_M2S_CMD_GETINFO,
	DP_M2S_CMD_SETINFO,
	DP_M2S_CMD_BUTT
}DP_M2S_CMD_ID_E;

/**************************************************************************************************************/
/***************************************************控制指令参数结构 声明***********************************************/
/**************************************************************************************************************/
/*
*说明：通用的返回结果的结构参数
*定义：DP_M2S_CMD_ACK_S
*成员：
	u32Success：0:成功，其他失败，返回错误码
*注意：无
*/
typedef struct _DP_M2S_CMD_ACK_S{
	
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_U32 u32Success;
}DP_M2S_CMD_ACK_S;

/*************************************1. DP_M2S_CMD_INIT*************************/
/*
*说明：系统资源初始化
*定义：DP_M2S_CMD_INIT_S
*成员：
	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_INIT ；
*注意：无
*/
typedef struct DP_M2S_INF_PROT_HEAD_S DP_M2S_CMD_INIT_S;

//“程序退出”应答
typedef struct DP_M2S_CMD_ACK_S DP_M2S_CMD_INIT_RESPOND_S;


/*************************************2. DP_M2S_CMD_DEINIT*************************/
/*
*说明：系统资源去初始化
*定义：DP_M2S_CMD_DEINIT_S
*成员：
	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_DEINIT ；
*注意：无
*/
typedef struct DP_M2S_INF_PROT_HEAD_S DP_M2S_CMD_DEINIT_S;

//“系统资源去初始化” 应答
typedef struct DP_M2S_CMD_ACK_S DP_M2S_CMD_DEINIT_RESPOND_S;

/*************************************3. DP_M2S_CMD_EXIT*************************/
/*
*说明：程序退出
*定义：DP_M2S_CMD_EXIT_INFO_S
*成员：
	stHeader：协议头，其内容：u16HeadTag + u16PacketLen + DP_M2S_CMD_EXIT ；
*注意：无
*/
typedef struct DP_M2S_INF_PROT_HEAD_S DP_M2S_CMD_EXIT_S;

//“程序退出”应答
typedef struct DP_M2S_CMD_ACK_S DP_M2S_CMD_EXIT_RESPOND_S;

/*************************************4. DP_M2S_CMD_GETINFO*************************/
/*
*说明：获取信息指令结构体
*定义：DP_M2S_CMD_GETINFO_S
*成员：
	stHeader：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_GETINFO ；
	enInfoTYpe：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
*注意：无
*/
typedef struct _DP_M2S_CMD_GETINFO_S{
	
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;
}DP_M2S_CMD_GETINFO_S;
/*
*说明：获取信息指令应答结构体
*定义：DP_M2S_CMD_GETINFO_S
*成员：
	stHeader	：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_GETINFO ；
	enInfoTYpe	：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
	u32Success	：0:成功，其他失败，返回错误码
	u32InfoLen	: 对应enInfoTYpe的信息数据长度；
	pu8InfoData	: 对应enInfoTYpe的信息数据结构指针；
*注意：无
*/
typedef struct _DP_M2S_CMD_GETINFO_RESPOND_S{
	
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;
	DP_U32 u32Success;
	DP_U32 u32InfoLen;
	DP_U8 *pu8InfoData;
}DP_M2S_CMD_GETINFO_RESPOND_S;

/*************************************5. DP_M2S_CMD_SETINFO*************************/
/*
*说明：设置信息指令结构体
*定义：DP_M2S_CMD_GETINFO_S
*成员：
	stHeader	：协议头，其内容：u16HeadTag + u16PacketLen+ DP_M2S_CMD_SETINFO ；
	enInfoTYpe	：指定获取类型信息，引用 DP_M2S_INFO_TYPE_E
	u32InfoLen	: 对应enInfoTYpe的信息数据长度；
	pu8InfoData	: 对应enInfoTYpe的信息数据结构指针；
*注意：无
*/
typedef struct _DP_M2S_CMD_SETINFO_S{
	
	DP_M2S_INF_PROT_HEAD_S stHeader;
	DP_M2S_INFO_TYPE_E enInfoTYpe;
	DP_U32 u32InfoLen;
	DP_U8 *pu8InfoData;
}DP_M2S_CMD_SETINFO_S;

//“设置信息指令” 应答
typedef struct DP_M2S_CMD_ACK_S DP_M2S_CMD_SETINFO_RESPOND_S;





typedef struct _S_DP_VIDEO_VIDEOPROCESSING_TAG {
	//视频裁剪
	DP_BOOL VideoClippingCntrol; //裁剪开关
	DP_U32 VideoClippingStartX; //裁剪开始坐标x
	DP_U32 VideoClippingStartY; //裁剪开始坐标y
	DP_U32 VideoClippingWidget; //裁剪宽
	DP_U32 VideoClippingHeight; //裁剪高
	//视频缩放
	DP_BOOL VideoZoomControl;	//缩放开关
	DP_U32 VideoZoomWidget;	//缩放宽
	DP_U32 VideoZoomHeight;	//缩放高
	//OSD
	DP_BOOL VideoOsdControl;	//osd开关
	DP_U8 VideoOsdDisplaySeat;	//osd显示位置
	DP_U32 VideoOsdStartX;	//osd开始坐标x
	DP_U32 VideoOsdStartY;	//osd开始坐标y
	DP_U32 VideoOsdColor;	//osd颜色

} DP_VIDEO_VIDEOPROCESSING;

enum eVideoControlPrototcal_CommandID {
	CMD_VIDEO_SYSTEM_INIT = 0x01, //初始化系统
	CMD_VIDEO_SYSTEM_FREE = 0X02, //释放系统资源
	CMD_VIDEO_SYSTEM_EXIT = 0X03, //退出程序
	CMD_VIDEO_VIN_CONTROL = 0X04, //视频输入控制
	CMD_VIDEO_AIN_CONTROL = 0X05, //音频输入控制
	CMD_VIDEO_VENC_CONTROL = 0X06, //视频编码控制
	CMD_VIDEO_AENC_CONTROL = 0X07, //音频编码控制
	CMD_VIDEO_VDEC_CONTROL = 0X08, //视频解码控制
	CMD_VIDEO_ADEC_CONTROL = 0X09, //音频解码控制
	CMD_VIDEO_STREAMING_MEDIA_SERVER_CONTROL = 0X0A,//流媒体服务器控制
};

//--------------------------------------------------------------------------------------------
//初始化系统
//command ID : 0x01
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_SYSTEM_INIT_TAG {
	DP_VIDEO_Header header;
} DP_VIDEO_SYSTEM_INIT;
//返回
typedef struct _S_DP_VIDEO_SYSTEM_INIT_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
} DP_VIDEO_SYSTEM_INIT_REPLAY;
//--------------------------------------------------------------------------------------------
//释放系统资源
//command ID : 0x02
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_SYSTEM_FREE_TAG {
	DP_VIDEO_Header header;
} DP_VIDEO_SYSTEM_FREE;
//返回
typedef struct _S_DP_VIDEO_SYSTEM_FREE_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
} DP_VIDEO_SYSTEM_FREE_REPLAY;
//--------------------------------------------------------------------------------------------
//退出程序
//command ID : 0x03
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_SYSTEM_EXIT_TAG {
	DP_VIDEO_Header header;
} DP_VIDEO_SYSTEM_EXIT;
//返回
typedef struct _S_DP_VIDEO_SYSTEM_EXIT_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
} DP_VIDEO_SYSTEM_EXIT_REPLAY;
//--------------------------------------------------------------------------------------------
//视频输入控制
//command ID : 0x04
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_VIN_STATUS_TAG {
	DP_BOOL isConnect;
	DP_BOOL isSignal;
	eVideoSync signalType;
	DP_BOOL isLogo;
	//OSD
	DP_BOOL VideoOsdControl;	//osd开关
	DP_U8 VideoOsdDisplaySeat;	//osd显示位置
	DP_U32 VideoOsdStartX;	//osd开始坐标x
	DP_U32 VideoOsdStartY;	//osd开始坐标y
	DP_U32 VideoOsdColor;	//osd颜色
} DP_VIDEO_VIN_STATUS;

typedef struct _S_DP_VIDEO_VIN_CONTROL_TAG {
	DP_VIDEO_Header header;
	DP_U8 commandType; // 1：读取 2：设置
	DP_U8 vinID; //输入设备采集ID
	DP_BOOL vinAutoModeControl;//视频输入自适应模式
	DP_VIDEO_VIN_STATUS status;
} DP_VIDEO_VIN_CONTROL;
//返回
typedef struct _S_DP_VIDEO_VIN_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_U8 vinID; //输入设备采集ID
	DP_BOOL vinAutoModeControl;//视频输入自适应模式
	DP_VIDEO_VIN_STATUS status;
} DP_VIDEO_VIN_CONTROL_REPLAY;

//--------------------------------------------------------------------------------------------
//音频输入控制
//command ID : 0x05
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_AIN_STATUS_TAG {
	DP_BOOL isConnect;
	eAudioRate audioRate;
	eAudioBitWidth audioBitWidth;
	eAudioSoundMode audioSoundMode;
	DP_U8 vol;
} DP_VIDEO_AIN_STATUS;

typedef struct _S_DP_VIDEO_AIN_CONTROL_TAG {
	DP_VIDEO_Header header;
	DP_U8 commandType; // 1：读取 2：设置
	DP_U8 ainID;
	DP_BOOL ainAutoModeControl;//音频自适应开关
	DP_VIDEO_AIN_STATUS status;
} DP_VIDEO_AIN_CONTROL;
//返回
typedef struct _S_DP_VIDEO_AIN_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_U8 ainID;
	DP_BOOL ainAutoModeControl;//音频自适应开关
	DP_VIDEO_AIN_STATUS status;
} DP_VIDEO_AIN_CONTROL_REPLAY;

//--------------------------------------------------------------------------------------------
//视频编码控制
//command ID : 0x06
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_STREAMINGMEDIA_SERVER_STATUS_TAG {
	DP_U8 MediaType; //流媒体服务器类型 0：RTSP 1：RTMP
	DP_U8 MediaControl; //流媒体服务器开关 0：关 1：开
	DP_U8 MediaUrlAddr[DP_VIDEO_MEDIA_URLADDR_MAX_LEN]; //流媒体服务器URL地址
	DP_U8 MediaConnentNum; //客户端最大连接数
	DP_U8 isMulticast; //是否支持组播 0：单播 1：组播
	DP_U8 ConnectMode; //通讯方式 0：TCP 1：UDP
} DP_VIDEO_STREAMINGMEDIA_SERVER_STATUS;

typedef struct _S_DP_VIDEO_VENC_STATUS_TAG {
	DP_U8 vencDev;	//视频编码设备ID
	DP_U32 u32Widget; //编码宽度
	DP_U32 u32Height; //编码高度
	DP_U8 vencType; //编码类型：0：H264 1：H265
	DP_U8 vencBitRateMode; //编码码率模式0：CBR 1：VBR
	DP_U32 vencBitRate; //码率  单位kbps 范围：256Kbps-40Mbps
	DP_U8 vencGop; //编码GOP 范围：1-120
	DP_U8 vencProfile; //编码质量参数 0：BP 1：MP 2：HP 注：编码模式为H265时只有MP
	DP_U8 vencTemporal; //时域 保留
	DP_U8 vencSpatital; //空域 保留
} DP_VIDEO_VENC_STATUS;

typedef struct _S_DP_VIDEO_VENC_CONTROL_TAG {
	DP_VIDEO_Header header;
	DP_U8 commandType; // 0：控制开关 1：读取 2：动态设置编码状态 3：动态设置视频处理状态
	DP_U8 VencID;
	DP_VIDEO_STREAMINGMEDIA_SERVER_STATUS MediaStatus;
	DP_VIDEO_VENC_STATUS VencStatus;
	DP_VIDEO_VIDEOPROCESSING VideoProcessingStatus;
} DP_VIDEO_VENC_CONTROL;

//返回
typedef struct _S_DP_VIDEO_VENC_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_U8 VencID;
	DP_VIDEO_STREAMINGMEDIA_SERVER_STATUS MediaStatus;
	DP_VIDEO_VENC_STATUS VencStatus;
	DP_VIDEO_VIDEOPROCESSING VideoProcessingStatus;
} DP_VIDEO_VENC_CONTROL_REPLAY;

//--------------------------------------------------------------------------------------------
//音频编码控制
//command ID : 0x07
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_AENC_STATUS_TAG {
	DP_U8 aencDev;	//音频编码设备ID
	DP_U8 aencType; //编码类型：0：AAC 1：PCM
	DP_U8 aencBitRate; //如果编码类型为AAC 该范围为32-128Kbps 如果为PCM该值为192Kbps
} DP_VIDEO_AENC_STATUS;

typedef struct _S_DP_VIDEO_AENC_CONTROL_TAG {
	DP_VIDEO_Header header;
	DP_U8 commandType; // 0：控制开关 1：读取 2：动态设置音频编码状态
	DP_U8 AencID;
	DP_VIDEO_STREAMINGMEDIA_SERVER_STATUS MediaStatus;
	DP_VIDEO_AENC_STATUS AencStatus;
} DP_VIDEO_AENC_CONTROL;

//返回
typedef struct _S_DP_VIDEO_AENC_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_U8 AencID;
	DP_VIDEO_STREAMINGMEDIA_SERVER_STATUS MediaStatus;
	DP_VIDEO_AENC_STATUS AencStatus;
} DP_VIDEO_AENC_CONTROL_REPLAY;

//--------------------------------------------------------------------------------------------
//视频解码控制
//command ID : 0x08
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_STREAMINGMEDIA_CLIENT_STATUS_TAG {
	DP_U8 MediaType; //流媒体客户端类型 0：RTSP 1：RTMP
	DP_U8 MediaControl; //流媒体客户端开关 0：关 1：开
	DP_U16 MediaConnectOverTime; //流媒体客户端连接超时时间
	DP_U8 MediaUrlAddr[DP_VIDEO_MEDIA_URLADDR_MAX_LEN]; //流媒体客户端URL地址
	DP_U16 MediaRecvDateOverTime; //流媒体客户端接收数据超时时间,该参数用于音频解码时无效
	DP_BOOL isLogo; //0：没接收到流媒体数据时不贴图，1：没接收到流媒体数据时贴图 ，该参数用于音频解码时无效
} DP_VIDEO_STREAMINGMEDIA_CLIENT_STATUS;

typedef struct _S_DP_VIDEO_VOUT_STATUS_TAG {
	DP_U8 VoutDev; //视频输出设备号
	DP_U8 VoutControl; //视频输出开关
	eVideoSync VoutSync; //视频输出分辨率
	//视频拼接参数
	DP_U8 VoutWindowControl;	//视频窗口开关
	DP_U8 VoutWindowPri;	//视频窗口优先级
	DP_U32 VoutWindowStartX;	//视频窗口起始坐标x
	DP_U32 VoutWindowStartY;	//视频窗口起始坐标y
	//视频效果参数
	DP_U8 VoutSharpness;	//锐比度
	DP_U8 VoutBrightness;	//亮度
	DP_U8 VoutChroma;	//色度
	DP_U8 VoutContrastratio;	//对比度
	DP_U8 VoutSaturation;	//饱和度
} DP_VIDEO_VOUT_STATUS;

typedef struct _S_DP_VIDEO_VDEC_STATUS_TAG {
	DP_U8 vdecType; //解码类型：0：H264 1：H265
} DP_VIDEO_VDEC_STATUS;

typedef struct _S_DP_VIDEO_VDEC_CONTROL_TAG {
	DP_VIDEO_Header header;
	DP_U8 commandType; // 0：控制开关 1：读取 2：动态设置视频输出参数 3：动态设置视频处理参数
	DP_U8 VdecID;
	DP_VIDEO_STREAMINGMEDIA_CLIENT_STATUS MediaStatus;
	DP_VIDEO_VDEC_STATUS VdecStatus;
	DP_VIDEO_VOUT_STATUS VoutStatus;
	DP_VIDEO_VIDEOPROCESSING VideoProcessingStatus;
} DP_VIDEO_VDEC_CONTROL;

//返回
typedef struct _S_DP_VIDEO_VDEC_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_U8 VdecID;
	DP_VIDEO_STREAMINGMEDIA_CLIENT_STATUS MediaStatus;
	DP_VIDEO_VDEC_STATUS VdecStatus;
	DP_VIDEO_VOUT_STATUS VoutStatus;
	DP_VIDEO_VIDEOPROCESSING VideoProcessingStatus;
} DP_VIDEO_VDEC_CONTROL_REPLAY;

//--------------------------------------------------------------------------------------------
//音频解码控制
//command ID : 0x09
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_ADEC_STATUS_TAG {
	DP_U8 adecType; //解码类型：0：aaclc 1：pcm
} DP_VIDEO_ADEC_STATUS;

typedef struct _S_DP_VIDEO_AOUT_STATUS_TAG {
	DP_U8 AO_DEV;
	eAudioRate audioRate;
	eAudioBitWidth audioBitWidth;
	eAudioSoundMode audioSoundMode;
	DP_U8 vol;
} DP_VIDEO_AOUT_STATUS;

typedef struct _S_DP_VIDEO_ADEC_CONTROL_TAG {
	DP_VIDEO_Header header;
	DP_U8 commandType; // 0：控制开关 1：读取 2：动态设置音频解码参数 3:动态设置音频输出参数
	DP_U8 AdecID;
	DP_VIDEO_STREAMINGMEDIA_CLIENT_STATUS MediaStatus;
	DP_VIDEO_ADEC_STATUS AdecStatus;
	DP_VIDEO_AOUT_STATUS AoutStatus;
} DP_VIDEO_ADEC_CONTROL;

//返回
typedef struct _S_DP_VIDEO_ADEC_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_U8 AdecID;
	DP_VIDEO_STREAMINGMEDIA_CLIENT_STATUS MediaStatus;
	DP_VIDEO_ADEC_STATUS AdecStatus;
	DP_VIDEO_AOUT_STATUS AoutStatus;
} DP_VIDEO_ADEC_CONTROL_REPLAY;

//--------------------------------------------------------------------------------------------
//流媒体服务器控制
//command ID : 0x0A
//--------------------------------------------------------------------------------------------
typedef struct _S_DP_VIDEO_STREAMMEIDA_SERVER_INIT_TAG{
	DP_S16 s16UrlMax;
	DP_U16 u16ServerPort;
	DP_S16 s16PollThreadMax;
	DP_S16 s16UrlClientRespMax;
	DP_S8 s8InputQueueDataMaxConut;
}DP_VIDEO_STREAMMEIDA_SERVER_INIT;

typedef struct _S_DP_VIDEO_MEDIA_SERVER_CONTROL_TAG{
	DP_VIDEO_Header header;
	DP_U8 commandType; // 0：控制开关 1：读取 2：修改流媒体服务器参数
	DP_VIDEO_STREAMMEIDA_SERVER_INIT serverDate;
}DP_VIDEO_MEDIA_SERVER_CONTROL;

//返回
typedef struct _S_DP_VIDEO_MEDIA_SERVER_CONTROL_REPLAY_TAG {
	DP_U32 isSuccess; //0:成功，失败返回错误码
	DP_VIDEO_STREAMMEIDA_SERVER_INIT serverDate;
} DP_VIDEO_MEDIA_SERVER_CONTROL_REPLAY;

#pragma pack(pop)
#endif
#endif /* SRC_PROTOCAL_VIDEOCONTROLPROTOCAL_H_ */
