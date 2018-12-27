/**
 * @file interactivepro.h
 * @brief 分布式控制软件协议
 * @details
 * <pre>
 *          该协议文档包含如下内容：
 *          - 控制软件与输入输出节点的通信协议
 *            -# 搜索设备协议 \a _sRemote_Search
 *            -# 查询设备信息协议 \a _sRemote_GetInfo
 *            -# 设置设备信息协议 \a _sRemote_SetInfo
 *            -# 开窗协议 \a _sRemote_CreateWindow
 *          - 控制软件与PAD软件之间的同步协议
 *          - 控制软件与流媒体服务器的通信协议
 *          - 控制软件与中控主机的通信协议
 * </pre>
 * @author 庄志光
 * @version v0.1
 * @date 2018
 */

/*! \def DP_U8
 \brief 定义数据类型
 */
#ifndef _INTERACTIVEPRO_H
#define _INTERACTIVEPRO_H
#include <string.h>
#include <iostream>
#include "dp_sd_type.h"
typedef unsigned char DP_U8;   ///<u8数据类型
typedef unsigned short DP_U16; ///<u16数据类型
typedef unsigned int DP_U32;   ///<u32数据类型

/*! \def DP_DEV_NAME_LEN
 \brief 设备名称长度
 \details 设备名称采用utf8编码，字符串传输过程中包含字符串结束符‘\0’
 */
#define DP_DEV_NAME_LEN 32 ///<设备名称长度

/*! \def DP_DEV_ID_LEN
 \brief 设备ID长度
 \details 设备ID暂定义为字符串，
 采用utf8编码，字符串传输过程中包含字符串结束符‘\0’
 */
#define DP_DEV_ID_LEN 13 ///<设备ID长度

/*! \def DP_URL_LEN
 \brief URL地址长度
 \details URL地址为字符串，
 采用utf8编码，字符串传输过程中包含字符串结束符‘\0’
 */
#define DP_URL_LEN 256 ///<URL地址长度

/*! \brief 视频流ID定义
 \details <pre>
 * ///////////////////////////////////硬件信息定义///////////////////////////
 * //定义输入节点信号通道ID名称
 * |---------------------------------------------------------------|
 * |-----------------------输入节点通道信息------------------------|
 * |----设备信号划分----|--------------功能描述--------------------|
 * |   音视频输入1      |第一路音视频信号采集通道（HDMI1）         |
 * |   音视频输入2      |第二路音视频信号采集通道（HDMI2）预留     |
 * |   模拟音频输入     |独立的模拟音频采集通道（AI）              |
 * |   模拟音频输出     |独立的模拟音频播放通道（AO）              |
 * |    音视频环出      |本地监视的一路HDMI，设备配死              |
 * |---------------------------------------------------------------|
 * |-----------------音视频输入通道RTSP通道号定义------------------|
 * |----通道号--|----------------------意义------------------------|
 * |-----0------|  原始分辨率码流，最大4K（含音频）                |
 * |-----1------|  子码流（1/4，1/8，1/16缩小倍率），开机配置		|
 * |-----2------|  1080P码流，当0通道为4K时，1号通道有效（含音频） |
 * |----3~8-----|  分割图像（1920x1080时为2*3分割，每块640x540）   |
 * |----3~13----|  分割图像（3840x2160时为3*3分割，每块1280x720）  |
 * |----14~15---|  预留其他分辨率通道                              |
 * |---------------------------------------------------------------| </pre>
 */
typedef enum eInputDeviceChannelVencID {
	VENCIDID_ORIGINAL_SINAGL = 0X0, ///<原始码流
	VENCIDID_SUB_PREVIEW = 0X01,    ///<预览码流
	VENCIDID_SUB_1 = 0X02,          ///<1080P码流
	VENCIDID_SUB_2 = 0X03,          ///<其他自定义码流
	//....
	VENCIDID_MAX_END = 0X0F         ///<定义一个通道最多16个码流
} eInputDeviceChannelVencID;

/*! \brief 视频输入输出物理通道ID
 \details <pre>
 |---------------------------------------------------------------|
 |-----------------------输出节点通道信息------------------------|
 |----设备信号划分----|---------------功能描述-------------------|
 |   音视频输出1      |第一路音视频信号显示通道（HDMI1）         |
 |   音视频输出2      |第二路音视频信号显示通道（HDMI2）预留     |
 |   音视频回显信号   |显示画面的小码流，及定死的AO音频          |
 |   模拟音频输入     |独立的模拟音频采集通道（AI）              |
 |   模拟音频输出     |独立的模拟音频播放通道（AO）              |
 |---------------------------------------------------------------| </pre>
 \warning ID定义范围限定为0x00~0xff，因为协议传输ID使用1个BYTE
 */
typedef enum _eDeviceVideoChannelID {
	ID_VI_CHN_VIDEOIN1 = 0x50, ///<视频采集1的视频通道ID （输入节点使用）
	ID_VI_CHN_VIDEOIN2 = 0x51, ///<视频采集2的视频通道ID （输入节点使用）

	ID_VO_CHN_VIDEOOUT1 = 0x60,	///<视频显示1的视频通道ID （输出节点使用）
	ID_VO_CHN_VIDEOOUT2 = 0x61,	///<视频显示2的视频通道ID （输出节点使用）
	ID_VO_CHN_LOOPVIDEOOUT = 0x70, ///<输入节点的回显信号的视频通道ID （预留，暂不使用）
} eDeviceVideoChannelID;

/*! \brief 音频输入输出物理通道ID
 \warning ID定义范围限定为0x00~0xff，因为协议传输ID使用1个BYTE
 */
typedef enum _eDeviceAudioChannelID {

	ID_AI_CHN_VIDEOIN1 = 0X01, ///<视频采集1的音频采集通道ID （输入节点使用）
	ID_AI_CHN_VIDEOIN2 = 0X02, ///<视频采集2的音频采集通道ID （输入节点使用）
	ID_AI_CHN_LINEIN1 = 0X10,  ///<线路输入1的音频采集通道ID （输入、输出节点使用）
	ID_AI_CHN_LINEIN2 = 0X11,  ///<线路输入2的音频采集通道ID （预留，暂不使用）

	ID_AO_CHN_VIDEOOUT1 = 0X20,   ///<视频显示1的音频播放通道ID （输出节点使用）
	ID_AO_CHN_VIDEOOUT2 = 0X21,   ///<视频显示2的音频播放通道ID （输出节点使用）
	ID_AO_CHN_LINEOUT1 = 0X30,	///<线路输出1的音频播放通道ID （输入、输出节点使用）
	ID_AO_CHN_LINEOUT2 = 0X31,	///<线路输出2的音频播放通道ID （预留，暂不使用）
	ID_AO_CHN_LOOPVIDEOOUT = 0X40 ///<输入节点的回显音频播放通道ID （预留，暂不使用）

} eDeviceAudioChannelID;

/*! \brief 音频采样率定义*/
typedef enum _eAudioSampleRate {
	SampleRate_AUDIO_8K = 8,	 ///<8000
	SampleRate_AUDIO_16K = 16,   ///<16000
	SampleRate_AUDIO_22K = 22,   ///<22050
	SampleRate_AUDIO_44K = 44,   ///<44100
	// jhbnote 48K?
	SampleRate_AUDIO_96K = 96,   ///<96000
	SampleRate_AUDIO_192K = 192, ///<192000
} eAudioSampleRate;

/*! \brief 音频编码类型*/
typedef enum eAudioEncoderType {
	AUDIO_ENCODE_PCM = 0X01, ///<PCM
	AUDIO_ENCODE_MP3 = 0X02, ///<MP3
	AUDIO_ENCODE_AAC = 0X03, ///<AAC
} eAudioEncoderType;

/*! \brief 设备类型*/
typedef enum _eDeviceType {
	Type_Controller = 0X01, ///<控制软件
	Type_StreamMediaServer, ///<流媒体服务器
	Type_ProjectManager,	///<工程软件
	Type_DeviceInput,		///<输入节点
	Type_DeviceOutput,		///<输出节点
} eDeviceType;

/*!
 \brief 任务ID
 \detials 控制端对设备进行开窗操作或音频开启操作所产生的唯一ID，
 此ID由控制端生成，控制端负责保证该ID在设备中的唯一性，具体取值范围见如下定义：
 */

/*! \def TASK_PC_MAX_ID
 \brief PC端分配的最大任务ID
 */
#define TASK_PC_MAX_ID 0x30000

/*! \def TASK_PC_MIN_ID
 \brief PC端分配的最小任务ID
 */
#define TASK_PC_MIN_ID 0x20001

/*! \def TASK_PAD_MAX_ID
 \brief PAD端分配的最大任务ID
 */
#define TASK_PAD_MAX_ID 0x20000

/*! \def TASK_PAD_MIN_ID
 \brief PAD端分配的最小任务ID
 */
#define TASK_PAD_MIN_ID 0x10001

/*! \def TASK_DEVICE_MAX_ID
 \brief 设备端分配的最大任务ID
 */
#define TASK_DEVICE_MAX_ID 0x10000

/*! \def TASK_DEVICE_MIN_ID
 \brief 设备端分配的最小任务ID
 */
#define TASK_DEVICE_MIN_ID 0x00001

/*! \brief 错误码
 \details 协议中设备返回的操作结果（u32Success字段）
 */
typedef enum _eRemoteErrorCode {
	ErrorCode_Success = 0,			 ///<无错误
	ErrorCode_ProtocalParse,		 ///<协议解析错误
	ErrorCode_Parameter,			 ///<参数错误
	ErrorCode_WindowSourceNotEnough, ///<开窗资源不足
	ErrorCode_TaskIDNotExist,		 ///<任务ID找不到
	ErrorCode_DeviceIDNotMatch,		 ///<设备ID不匹配
										 //...
} eRemoteErrorCode;

#pragma pack(1)
//////////////////////////////基本结构体信息////////////////////////
//视频源信息（x,y,w,h）
typedef struct _sSrcVideoInfo_tag {
//	_sSrcVideoInfo_tag(DP_U16 videoWidth, DP_U16 videoHeight, DP_U16 startX,
//			DP_U16 startY, DP_U16 endX, DP_U16 endY) :
//			u16VideoWidth(videoWidth), u16VideoHeight(videoHeight), u16StartX(
//					startX), u16StartY(startY), u16EndX(endX), u16EndY(endY) {
//	}
//	_sSrcVideoInfo_tag() :
//			u16VideoWidth(), u16VideoHeight(), u16StartX(), u16StartY(), u16EndX(), u16EndY() {
//	}
	DP_U16 u16VideoWidth;  ///<源视频分辨率宽度
	DP_U16 u16VideoHeight; ///<源视频分辨率高度
	DP_U16 u16StartX;	  ///<待显示图像开始X坐标
	DP_U16 u16StartY;	  ///<待显示图像开始Y坐标
	DP_U16 u16EndX;		   ///<待显示图像结束X坐标
	DP_U16 u16EndY;		   ///<待显示图像结束Y坐标
} _sSrcVideoInfo;

//视频显示信息（x,y,w,h）
typedef struct _sDstVideoInfo_tag {
	DP_U16 u16VideoWidth;  ///<显示区域的分辨率宽度
	DP_U16 u16VideoHeight; ///<显示区域的分辨率高度
	DP_U16 u16StartX;	  ///<显示区域开始X坐标
	DP_U16 u16StartY;	  ///<显示区域开始Y坐标
} _sDstVideoInfo;

//音频源信息
typedef struct _sSrcAudioInfo_tag {
	_sSrcAudioInfo_tag(DP_U8 aiDevIntfSampleRate, DP_U8 aiDevIntfBitwidth,
			DP_U8 aiDevIntfSoundMode, DP_U8 aencType) :
			u8AiDevIntfSampleRate(aiDevIntfSampleRate), u8AiDevIntfBitwidth(
					aiDevIntfBitwidth), u8AiDevIntfSoundMode(
					aiDevIntfSoundMode), u8AencType(aencType) {
	}
	_sSrcAudioInfo_tag() :
			u8AiDevIntfSampleRate(), u8AiDevIntfBitwidth(), u8AiDevIntfSoundMode(), u8AencType() {
	}
	DP_U8 u8AiDevIntfSampleRate; ///<音频输入设备采样率 8/16/22/32/44/48/96	@see eAudioSampleRate
	DP_U8 u8AiDevIntfBitwidth;   ///<音频输入设备采样率精度 16:16bit 8:8bit
	DP_U8 u8AiDevIntfSoundMode;  ///<音频输入设备声道模式 0：单声道 1：立体声道
	DP_U8 u8AencType;			 ///<编码类型 aac mp3 pcm @see eAudioEncoderType
} _sSrcAudioInfo;

//视频任务（窗口）信息
typedef struct _sVideoTaskInfo_tag {
	DP_U8 u8VoChnID;	///<显示通道ID @see eDeviceVideoChannelID //add 2018.10.22
	DP_U32 u32TaskID;			 ///<见任务ID注释
	DP_U8 u8WinZIndex;			 ///<窗口层叠优先级
	DP_U8 u8VStreamStatus;		 ///<音频输入网络流媒体信号有无
	_sSrcVideoInfo srcVideoInfo; ///<视频源信息 @see _sSrcVideoInfo
	_sDstVideoInfo dstVideoInfo; ///<视频显示信息	@see _sDstVideoInfo
//	DP_U8 au8SrcDevID[DP_DEV_ID_LEN];	 ///<设备ID
//	DP_U8 u8ViChnID;						/// video input ch id
	DP_U8 au8InputRtspURL[DP_URL_LEN];    ///<预览流RTSP地址 //add 2018.10.30

} _sVideoTaskInfo;

//音频任务信息
typedef struct _sAudioTaskInfo_tag {
	_sAudioTaskInfo_tag(DP_U32 taskID, DP_U8 vStreamStatus, DP_U8 *inputRtspURL,
			_sSrcAudioInfo audioInfo, DP_U8 aoChnID) :
			u32TaskID(taskID), u8VStreamStatus(vStreamStatus), srcAudioInfo(
					audioInfo), u8AoChnID(aoChnID) {
		memset(au8InputRtspURL, 0, DP_URL_LEN);
		strcpy((DP_CHAR*) au8InputRtspURL, (DP_CHAR*) inputRtspURL);
	}
	_sAudioTaskInfo_tag() :
			u32TaskID(), u8VStreamStatus(), srcAudioInfo(), u8AoChnID() {
		memset(au8InputRtspURL, 0, DP_URL_LEN);
	}
	DP_U32 u32TaskID;			 ///<见任务ID注释
	DP_U8 u8VStreamStatus;		 ///<视频输入网络流媒体信号有无
//	DP_U8 au8SrcDevID[DP_DEV_ID_LEN];	 ///<设备ID
	DP_U8 au8InputRtspURL[DP_URL_LEN];    ///<预览流RTSP地址 //add 2018.10.30
//	DP_U8 u8AiChnID;			 ///<音频采集物理通道ID  @see eDeviceAudioChannelID
	_sSrcAudioInfo srcAudioInfo; ///<音频源信息  @see  _sSrcAudioInfo
	DP_U8 u8AoChnID;			 ///<音频播放通道ID @see eDeviceAudioChannelID
} _sAudioTaskInfo;

//开窗物理屏幕信息
typedef struct _sPhyScreenInfo_tag {
	DP_U16 u16PhyScreenWidth;
	DP_U16 u16PhyScreenHeight;

	DP_U16 u16SrcVideoWidth;
	DP_U16 u16SrcVideoHeight;
	DP_U16 u16SrcStartX;
	DP_U16 u16SrcStartY;

} _sPhyScreenInfo;

//视频输出通道信号的信息
typedef struct _sSingleVoChnInfo_tag {
	_sSingleVoChnInfo_tag(DP_U8 voChnOpenStatus, DP_U8 voChnID,
			DP_U8 *previewRtspURL, DP_U32 urlLen,
			eDeviceAudioChannelID relateAoChnID, DP_U8 aoChnMute,
			DP_U8 aoChnVolume) :
			u8VoChnOpenStatus(voChnOpenStatus), u8VoChnID(voChnID), u8RelateAoChnID(
					relateAoChnID), u8AoChnMute(aoChnMute), u8AoChnVolume(
					aoChnVolume) {
		memset(au8PreviewRtspURL, 0, DP_URL_LEN);
		if (urlLen)
			strcpy((DP_CHAR*) au8PreviewRtspURL, (DP_CHAR*) previewRtspURL);
//			memcpy(au8PreviewRtspURL, previewRtspURL, urlLen);
	}
	DP_U8 u8VoChnOpenStatus;			 ///<显示输出的开关状态
	DP_U8 u8VoChnID;					 ///<显示通道ID @see eDeviceVideoChannelID
	DP_U8 au8PreviewRtspURL[DP_URL_LEN]; ///<回显信号的RTSP URL
//	DP_U8 u8VideoTaskCount;				 ///<视频任务数量（窗口数量）
	DP_U8 u8RelateAoChnID; ///<与当前present通道一起的音频通道ID //add 2018.10.25
	//jhbnote restore mute status
	DP_U8 u8AoChnMute;	 ///<是否静音 0否 1是 //add 2018.10.25
	DP_U8 u8AoChnVolume;   ///<音量 0~100 //add 2018.10.25
} _sSingleVoChnInfo;

//设备属性定义
typedef enum _eRemotePropertyName
	: DP_U32
	{
		Property_Get_InputVideoChnInfo = 0x1000,   ///<获取输入节点的视频采集通道信息
	Property_Get_InputAudioChnInfo,            ///<获取输入节点的音频输入的通道信息
	Property_Get_OutputVideoChnInfo,           ///<获取输出节点的显示视频通道信息
	Property_Get_OutputAudioChnInfo,           ///<获取输出节点的音频输出的通道信息
	//获取设备其他基本信息...
	Property_Get_VideoChnVencInfo,    	///<获取输入节点的视频采集通道的流信息 //edit 2018.10.23
//设置设备基本信息...
} eRemotePropertyName;

//协议命令号定义
typedef enum _eRemoteCommand {
	//jhbnote notify
	Command_MulticastSearch = 0x01, //20181203 modify
	Command_GetInfo,
	Command_SetInfo,
	Command_CreateWindow,
	Command_MoveWindow,
	Command_CloseWindow,
	Command_OpenAudio,
	Command_CloseAudio,
	Command_SetAudio,
	Command_ClearTask = 0x0b,
	Command_UnicastSearch = 0XF1, //20181203 add
	Command_OpenAndMoveWindow = 0XF2, //20181220 add
	Command_UpdateBatch = 0XFA //20181224 add
} eRemoteCommand;

//设备属性值 @see Property_Get_InputVideoChnInfo
typedef struct _sAllViChnInfo_tag {
	_sAllViChnInfo_tag(DP_U8 viChnCount) :
			u8ViChnCount(viChnCount) {
	}
	DP_U8 u8ViChnCount;				///<视频物理通道数量,后面附带动态通道信息 \a _sSingleViChnInfo
	typedef struct _sSingleViChnInfo_tag ///<视频输入通道信号的信息
	{
		_sSingleViChnInfo_tag(DP_U8 vSignalStatus, DP_U16 videoWidth,
				DP_U16 videoHeight, DP_U8 videoFrm) :
				u8VSignalStatus(vSignalStatus), u16VideoWidth(videoWidth), u16VideoHeight(
						videoHeight), u8VideoFrm(videoFrm) {
		}
		DP_U8 u8VSignalStatus; ///<接入信号有无  // 0 -> false  1->true
		DP_U16 u16VideoWidth;  ///<视频信号宽度
		DP_U16 u16VideoHeight; ///<视频信号高度
		DP_U8 u8VideoFrm;	  ///<视频帧率
	} _sSingleViChnInfo;

	//_sSingleViChnInfo* pSingleViChnInfo;				///<视频输入通道信息数组

} _sAllViChnInfo;

//设备属性值 @see Property_Get_InputAudioChnInfo
typedef struct _sAllAiChnInfo_tag {
	_sAllAiChnInfo_tag(DP_U8 aiChnCount) :
			u8AiChnCount(aiChnCount) {
	}
	DP_U8 u8AiChnCount;				///<音频物理通道数量,后面附带动态通道信息 \a _sSingleAiChnInfo
	typedef struct _sSingleAiChnInfo_tag ///<音频输入通道信号的信息
	{
		_sSingleAiChnInfo_tag(DP_U8 aiSignalStatus, DP_U8 *RtspURL,
				DP_U32 urlLen, DP_U8 aiDevIntfSampleRate,
				DP_U8 aiDevIntfBitwidth, DP_U8 aiDevIntfSoundMode,
				DP_U8 aencType) :
				u8AiSignalStatus(), srcAudioInfo(aiDevIntfSampleRate,
						aiDevIntfBitwidth, aiDevIntfSoundMode, aencType) {
			memset(au8PreviewRtspURL, 0, DP_URL_LEN);
			strcpy((DP_CHAR*) au8PreviewRtspURL, (DP_CHAR*) RtspURL);
		}
		_sSingleAiChnInfo_tag() :
				u8AiSignalStatus(), srcAudioInfo() {
			memset(au8PreviewRtspURL, 0, DP_URL_LEN);
		}
		DP_U8 u8AiSignalStatus;		 ///<接入信号有无
		DP_U8 au8PreviewRtspURL[DP_URL_LEN]; ///<预览流RTSP地址
		//rtsp
		_sSrcAudioInfo srcAudioInfo; ///<音频源信息  @see  _sSrcAudioInfo
	} _sSingleAiChnInfo;

	//_sSingleAiChnInfo* pSingleAiChnInfo;				///<音频输入通道信息数组

} _sAllAiChnInfo;

//设备属性值 @see Property_Get_OutputVideoChnInfo
typedef struct _sAllVoChnInfo_tag {
	_sAllVoChnInfo_tag(DP_U8 voChnCount, DP_U8 voTaskCount) :
			u8VoChnCount(voChnCount), u8VoTaskCount(voTaskCount) {
	}
	_sAllVoChnInfo_tag() :
			u8VoChnCount(), u8VoTaskCount() {
	}
	DP_U8 u8VoChnCount;  ///<视频显示通道数量
	DP_U8 u8VoTaskCount; ///<视频任务数量（窗口数量）//add 2018.10.22
						 //DP_U8* pSingleVoChnInfo;							///<视频任务数量（窗口数量）//add 2018.10.22
						 //DP_U8* pVideoTaskInfo;							///<视频任务数量（窗口数量）//add 2018.10.22
} _sAllVoChnInfo;

//设备属性值 @see Property_Get_OutputAudioChnInfo
typedef struct _sAllAoChnInfo_tag {
	_sAllAoChnInfo_tag(DP_U8 aoChnCount) :
			u8AoChnCount(aoChnCount) {
	}
	DP_U8 u8AoChnCount;					 ///<音频播放通道数量
	typedef struct _sSingleAoChnInfo_tag ///<音频输出通道信号
	{
		_sSingleAoChnInfo_tag(DP_U8 aoChnMute, DP_U8 aoChnID, DP_U8 aoChnVolume,
				DP_U8 audioIn, _sAudioTaskInfo audioTask) :
				u8AoChnMute(aoChnMute), u8AoChnID(aoChnID), u8AoChnVolume(
						aoChnVolume), u8AudioIn(audioIn), audioTaskInfo(
						audioTask) {
		}
		DP_U8 u8AoChnMute;			   ///<音频播放通道开关状态
		DP_U8 u8AoChnID;			   ///<音频播放通道ID @see eDeviceAudioChannelID
		DP_U8 u8AoChnVolume;		   ///<音频播放通道音量
		DP_U8 u8AudioIn;			   ///<该通道是否有音频流 0没有 1有
		_sAudioTaskInfo audioTaskInfo; ///<单个的音频 仅当u8AudioIn为1时有效
	} _sSingleAoChnInfo;

	//_sSingleAoChnInfo* pSingleAoChnInfo;				///<音频输出通道信息数组

} _sAllAoChnInfo;

//设备属性值 @see Property_Get_VideoChnVencInfo  //Property_Get_VideoChnVencInfo
typedef struct _sAllVencChnInfo_tag //edit 2018.10.23
{
	_sAllVencChnInfo_tag(DP_U8 viChnCount, DP_U8 vencCount) :
			u8ViChnCount(viChnCount), u8VencCount(vencCount) {
	}
	_sAllVencChnInfo_tag() :
			u8ViChnCount(), u8VencCount() {
	}
	DP_U8 u8ViChnCount;									///<视频输入通道数量
	DP_U8 u8VencCount;									///<视频编码通道总数量
	typedef struct _sSingleVencChnInfo_tag				///<音视频流信息
	{
		_sSingleVencChnInfo_tag(DP_U8 viChnID, DP_U8 vencChnID,
				DP_U8 u8VencType, DP_U16 videoWidth, DP_U16 videoHeight,
				DP_U16 startX, DP_U16 startY, DP_U8*previewRtspURL,
				DP_U32 urlLen, DP_U8 audioIn, _sSrcAudioInfo audioInfo) :
				u8ViChnID(viChnID), u8VencChnID(vencChnID), u8VencType(
						u8VencType), u16VideoWidth(videoWidth), u16VideoHeight(
						videoHeight), u16StartX(startX), u16StartY(startY), u8AudioIn(
						audioIn), srcAudioInfo(audioInfo) {
			memset(au8PreviewRtspURL, 0, DP_URL_LEN);
			strcpy((DP_CHAR*) au8PreviewRtspURL, (DP_CHAR*) previewRtspURL);
		}
		DP_U8 u8ViChnID;								///<关联的视频输入通道ID
		DP_U8 u8VencChnID;///<流ID，对应音视频输入通道RTSP通道号， @see eInputDeviceChannelVencID
		DP_U8 u8VencType;								///<视频编码格式 未定义
		DP_U16 u16VideoWidth;							///<视频分辨率宽
		DP_U16 u16VideoHeight;							///<视频分辨率高
		DP_U16 u16StartX;								///<视频起始坐标
		DP_U16 u16StartY;								///<视频起始坐标
		DP_U8 au8PreviewRtspURL[DP_URL_LEN];            ///<预览流RTSP地址
		DP_U8 u8AudioIn;								///<该流是否含有音频 0没有 1有
		_sSrcAudioInfo srcAudioInfo;///<音频源信息 仅当u8AudioIn为1时有效 @see  _sSrcAudioInfo
	} _sSingleVencChnInfo;

	//_sSingleVencChnInfo* pSingleVencChnInfo;	///<多个视频流编码信息

} _sAllVencChnInfo;

///////////////////////////协议/////////////////////////////////////

/*! \brief 协议BODY的命令与长度*/
typedef struct _s_AVServer_MSG_tag {
	_s_AVServer_MSG_tag(DP_U8 cmdID, DP_U16 cmdLen) :
			u8CommandID(cmdID), u16CommandLen(cmdLen) {
	}
	_s_AVServer_MSG_tag() :
			u8CommandID(), u16CommandLen() {

	}
	DP_U8 u8CommandID; ///<\a eRemoteCommand
	DP_U16 u16CommandLen;
} _s_AVServer_MSG;

/*!
 *   \brief 通信协议头
 *   \extends _s_AVServer_MSG
 */
// eDeviceType name
typedef struct _s_AVServer_Header_tag {
	_s_AVServer_Header_tag(DP_U32 u32IP, eDeviceType devType, DP_U8 packageType,
			DP_U16 packageLen, DP_U8 cmdID, DP_U16 cmdLen) :
			u16Headtag(0xF0F1), u32SrcIP(u32IP), u8DeviceType(devType), u16Version(), u32RequestID(), u8PackageType(
					packageType), u16PackageLen(packageLen), stFunctionMsg(
					cmdID, cmdLen) {
	}
	_s_AVServer_Header_tag() :
			u16Headtag(0xF0F1), u32SrcIP(), u8DeviceType(), u16Version(), u32RequestID(), u8PackageType(), u16PackageLen(), stFunctionMsg() {
	}
	DP_U16 u16Headtag;			   ///<0xF0F1
	DP_U32 u32SrcIP;			   ///<源IP地址，小端模式数据
	DP_U16 u8DeviceType;			   ///<源设备类型
	DP_U16 u16Version;			   ///<协议版本号
	DP_U32 u32RequestID;		   ///<命令请求ID，自增字段
	DP_U8 u8PackageType;		   ///<0:request data 1:reply data
	DP_U16 u16PackageLen;		   ///<数据包长，是指该协议包长度
	_s_AVServer_MSG stFunctionMsg; ///<协议命令与协议body长度 \a _s_AVServer_MSG
								   //DP_U8* u8FunctionData;
} _s_AVServer_Header;

typedef _s_AVServer_Header _sRemote_Header; ///<定义_sRemote_Header类型，协议头 20B


/// @defgroup 控制软件与输入输出节点的协议
/// @{

/// @defgroup 设备搜索命令
///   \details
///    <pre>
///      服务端：输入、输出节点
///      客户端：控制软件
///      组播地址：234.0.0.2：45000（设备）45001（应用软件）
///		 支持单播，端口同上
///    </pre>
/// @{
////////////////////////////////////////////////////1.搜索命令///////////////////////////////////////////////////
/*!
 *   \brief 搜索命令
 *   \warning 输入、输出节点都需要实现
 *   \extends _sRemote_Header
 *   \return \a _sRemote_Reply_Search
 */
typedef struct _sRemote_Search_tag {
	_sRemote_Header header;
	//
} _sRemote_Search;
/*! 搜索命令应答 \extends _sRemote_Header */
typedef struct _sRemote_Reply_Search_tag {
	_sRemote_Reply_Search_tag(DP_U32 u32IP, eDeviceType devType,
			DP_U8 packageType, DP_U16 packageLen, DP_U8 cmdID, DP_U16 cmdLen,
			DP_U8 *devID, DP_U32 devIDLen, DP_U16 u8DevType, DP_U16 devNum,
			DP_U8 *devName, DP_U32 devNameLen, DP_U32 DevIP,
			DP_U16 devSoftVersion, DP_U16 DevHardVersion, DP_U16 extendDataLen) :
			header(u32IP, devType, packageType, packageLen, cmdID, cmdLen), u8DevType(
					u8DevType), u8DevNum(devNum), u32DevIP(DevIP), u16DevSoftVersion(
					devSoftVersion), u16DevHardVersion(DevHardVersion), u16ExtendDataLen(
					extendDataLen) {
		memset(au8DevID, 0, devIDLen);
		memset(au8DevName, 0, devNameLen);
		strcpy((DP_CHAR*) au8DevID, (DP_CHAR*) devID);
//		memcpy(au8DevID, devID, devIDLen);
		strcpy((DP_CHAR*) au8DevName, (DP_CHAR*) devName);
//		memcpy(au8DevName, devName, devNameLen);
	}
	_sRemote_Reply_Search_tag() :
			header(), au8DevID(), u8DevType(), u8DevNum(), au8DevName(), u32DevIP(), u16DevSoftVersion(), u16DevHardVersion(), u16ExtendDataLen() {
	}
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];	 ///<设备ID
	DP_U16 u8DevType;				   ///<设备类型 @see eDeviceType
	DP_U16 u8DevNum;					   ///<设备型号
	DP_U8 au8DevName[DP_DEV_NAME_LEN]; ///<设备名
	DP_U32 u32DevIP;				   ///<IP 172.16.0.1表示为0xAC100001
	DP_U16 u16DevSoftVersion;		   ///<设备软件版本
	DP_U16 u16DevHardVersion;		   ///<设备硬件版本

	DP_U16 u16ExtendDataLen; ///<附加信息长度 //add 2018.10.22
							 //DP_U8*		pEtendData;						///<附加信息 //add 2018.10.22
} _sRemote_Reply_Search;

//附加信息 //add 2018.10.22
//设备类型为 Type_DeviceOutput 时
typedef struct _sRemote_Reply_Search_ExtendDeviceOutput_tag {
	_sRemote_Reply_Search_ExtendDeviceOutput_tag(DP_U8 voChnCount,
			DP_U8 aiChnCount, DP_U8 aoChnCount) :
			u8VoChnCount(voChnCount), u8AiChnCount(aiChnCount), u8AoChnCount(
					aoChnCount) {
	}
	DP_U8 u8VoChnCount; ///<视频输出通道数量

	DP_U8 u8AiChnCount; ///<音频输入通道数量
	DP_U8 u8AoChnCount; ///<音频输出通道数量
} _sRemote_Reply_Search_ExtendDeviceOutput;
//设备类型为 Type_DeviceInput 时
typedef struct _sRemote_Reply_Search_ExtendDeviceInput_tag {
	_sRemote_Reply_Search_ExtendDeviceInput_tag(DP_U8 viChnCount,
			DP_U8 aiChnCount, DP_U8 aoChnCount) :
			u8ViChnCount(viChnCount), u8AiChnCount(aiChnCount), u8AoChnCount(
					aoChnCount) {
	}
	DP_U8 u8ViChnCount; ///<视频输入通道数量

	DP_U8 u8AiChnCount; ///<音频输入通道数量
	DP_U8 u8AoChnCount; ///<音频输出通道数量
} _sRemote_Reply_Search_ExtendDeviceInput;

/// @}
////////////////////////////////////////////////////2.获取信息命令///////////////////////////////////////////////////
/// @defgroup 获取信息命令
/// @{
/*!
 *   \brief 获取输入输出节点信息
 *   \details
 *           -# 获取输入节点的视频采集通道信息 \a eRemotePropertyName::Property_Get_InputVideoChnInfo
 *           -# 获取输入节点的音频输入的通道信息 \a eRemotePropertyName Property_Get_InputAudioChnInfo
 *           -# 获取输出节点的显示视频通道信息 \a eRemotePropertyName Property_Get_OutputVideoChnInfo
 *           -# 获取输出节点的音频输入输出的通道信息 \a eRemotePropertyName Property_Get_OutputAudioChnInfo
 *           -# 获取输入节点的视频采集通道的流信息 \a eRemotePropertyName Property_Get_VideoChnVencInfo
 *   \warning 输入、输出节点都需要实现
 *   \extends _sRemote_Header \extends eRemotePropertyName
 *   \return \a _sRemote_Reply_GetInfo
 */
typedef struct _sRemote_GetInfo_tag {
	_sRemote_GetInfo_tag(DP_U32 u32IP, eDeviceType devType, DP_U8 packageType,
			DP_U16 packageLen, eRemotePropertyName propertyName, DP_U8 cmdID,
			DP_U16 cmdLen) :
			header(u32IP, devType, packageType, packageLen, cmdID, cmdLen), u32Proterty(
					propertyName) {
	}
	_sRemote_Header header;
	eRemotePropertyName u32Proterty; //获取指定信息
} _sRemote_GetInfo;

/// @defgroup 获取输入节点的视频采集通道信息
/// @{
///  - 控制软件->输入节点 发送： \a Property_Get_InputVideoChnInfo
///  - 输入节点->控制软件 返回： \a _sAllViChnInfo
///  @}

/*! 获取输入输出节点信息应答
 \extends _sRemote_Header \extends eRemotePropertyName */
typedef struct _sRemote_Reply_GetInfo_tag {
	_sRemote_Reply_GetInfo_tag(DP_U32 u32IP, eDeviceType devType,
			DP_U8 packageType, DP_U16 packageLen, DP_U8 cmdID, DP_U16 cmdLen,
			DP_U8 *devID, eRemotePropertyName proterty, DP_U32 success,
			DP_U16 propertyLen) :
			header(u32IP, devType, packageType, packageLen, cmdID, cmdLen), u32Proterty(
					proterty), u32Success(success), u16PropertyLen(propertyLen), pProperty() {
		memset(au8DevID, 0, DP_DEV_ID_LEN);
		strcpy((DP_CHAR*) au8DevID, (DP_CHAR*) devID);
//		memcpy(au8DevID, devID, DP_DEV_ID_LEN);
	}
	_sRemote_Reply_GetInfo_tag() :
			header(), u32Proterty(), u32Success(), u16PropertyLen(), pProperty() {
	}
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];
	eRemotePropertyName u32Proterty; ///<返回指定信息
	DP_U32 u32Success;				 ///<0成功 其他失败，含错误码
	DP_U16 u16PropertyLen;			 ///<Property数据长度
	DP_U8 *pProperty;				 ///<sProperty返回对应指令的数据结构指针数据
} _sRemote_Reply_GetInfo;

///@}
////////////////////////////////////////////////////3.设置参数命令///////////////////////////////////////////////////
//输入、输出节点都需要实现
/// @defgroup 设置参数命令
/// @{
/*!
 *   \brief 设置输入输出节点信息
 *   \details
 *
 *   \warning 输入、输出节点都需要实现
 *   \extends _sRemote_Header \extends eRemotePropertyName
 *   \return \a _sRemote_Reply_SetInfo
 */
typedef struct _sRemote_SetInfo_tag {
	_sRemote_Header header;
	eRemotePropertyName u32Proterty; ///<设置指定参数
	DP_U16 u16PropertyLen;			 ///<Property数据长度
	DP_U8 *pProperty;				 ///<sProperty对应指令的数据结构指针数据
} _sRemote_SetInfo;

/*! 设置输入输出节点信息应答
 \extends _sRemote_Header \extends eRemotePropertyName */
typedef struct _sRemote_Reply_SetInfo_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];   ///<节点ID
	eRemotePropertyName u32Proterty; ///<设置指定信息
	DP_U32 u32Success;				 ///<@retval 0 表示执行成功 @retval !=0 执行失败的错误码
} _sRemote_Reply_SetInfo;
///@}
////////////////////////////////////////////////////4.开窗命令///////////////////////////////////////////////////
/// @defgroup 开窗命令
/// @{
//输出节点需要实现
typedef struct _sRemote_CreateWindow_tag {
	_sRemote_Header header;  // sizeof 20
	DP_U32 u32TaskID; ///<见任务ID注释

	DP_U8 au8RtspURL[DP_URL_LEN]; ///< 第三方标准RTSP
	DP_U8 u8AudioIn;  				/// is contain audio or not  // 281byte

	//输出节点信息
	//视频信息
	DP_U8 au8DstDevID[DP_DEV_ID_LEN]; ///<输出节点ID
	DP_U8 u8VoChnID;				  ///<显示通道ID \a eDeviceVideoChannelID
	_sSrcVideoInfo srcVideoInfo;///<视频源信息，如果输入信息为第三方标准RTSP，则此数据结构无效\a _sSrcVideoInfo
	_sDstVideoInfo dstVideoInfo;	  ///<显示目标信息 \a _sDstVideoInfo
	_sPhyScreenInfo phyScreenInfo;	///<物理屏幕，用于视频像素对齐，暂时保留 //add 2018.10.22
	//音频信息
	//0 关闭 1切换 2维持 // update : create win without audio
	DP_U8 u8AudioEnable;   ///<是否同时启用音频，当启用音频时，u8AiChnID、u8AoChnID才有效
	DP_U8 u8AoChnID;	 ///<音频播放通道ID \a eDeviceAudioChannelID
	_sSrcAudioInfo srcAudioInfo;//invalided if audio is enabled. // 2018/11/13

} _sRemote_CreateWindow;

typedef struct _sRemote_Reply_CreateWindow_tag {
	_sRemote_Reply_CreateWindow_tag(_sRemote_Header head, DP_U8 *devID,
			DP_U32 taskID, DP_U32 success) :
			header(head), u32TaskID(taskID), u32Success(success) {
		memset(au8DevID, 0, DP_DEV_ID_LEN);
		strcpy((DP_CHAR*) au8DevID, (DP_CHAR*) devID);
	}
	_sRemote_Reply_CreateWindow_tag() :
			header(), au8DevID(), u32TaskID(), u32Success() {
	}
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///<节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U32 u32Success;			   ///<0成功 其他失败，含错误码
} _sRemote_Reply_CreateWindow;

typedef _sRemote_Reply_CreateWindow Remote_Common_Reply_S;
///@}
////////////////////////////////////////////////////5.窗口调整（调整时置顶窗口）命令///////////////////////////////////////////////////
//输出节点需要实现
/// @defgroup 窗口调整命令
/// @{
typedef struct _sRemote_MoveWindow_tag {
	_sRemote_Header header;
	DP_U8 au8DstDevID[DP_DEV_ID_LEN]; ///<输出节点ID
	DP_U32 u32TaskID;				  ///<见任务ID注释
	DP_U8 u8VoChnID;				  ///<显示通道ID @see eDeviceVideoChannelID
	_sDstVideoInfo dstVideoInfo;	  ///<显示目标信息 @see _sDstVideoInfo
	_sSrcVideoInfo srcVideoInfo;					///<视频源信息
} _sRemote_MoveWindow;

typedef struct _sRemote_Reply_MoveWindow_tag {
	_sRemote_Reply_MoveWindow_tag(_sRemote_Header head, DP_U8 *devID,
			DP_U32 taskID, DP_U32 success) :
			header(head), u32TaskID(taskID), u32Success(success) {
		strcpy((DP_CHAR*) au8DevID, (DP_CHAR*) au8DevID);
//		memcpy(au8DevID, devID, DP_DEV_ID_LEN);
	}
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///<节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U32 u32Success;			   ///<0成功 其他失败，含错误码
} _sRemote_Reply_MoveWindow;

///@}
////////////////////////////////////////////////////6.关闭窗口命令///////////////////////////////////////////////////
/// @defgroup 关闭窗口命令
/// @{
//输出节点需要实现
typedef struct _sRemote_CloseWindow_tag {
	_sRemote_Header hedader;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///<输出节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U8 u8VoChnID;			   ///<显示通道ID  @see eDeviceVideoChannelID
} _sRemote_CloseWindow;

typedef struct _sRemote_Reply_CloseWindow_tag {
	_sRemote_Reply_CloseWindow_tag(_sRemote_Header head, DP_U8 *devID,
			DP_U32 taskID, DP_U32 success) :
			header(head), u32TaskID(taskID), u32Success(success) {
		memset(au8DevID, 0, DP_DEV_ID_LEN);
		strcpy((DP_CHAR*) au8DevID, (DP_CHAR*) devID);
//		memcpy(au8DevID, devID, DP_DEV_ID_LEN);
	}
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///<节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U32 u32Success;			   ///<0成功 其他失败，含错误码
} _sRemote_Reply_CloseWindow;

///@}
////////////////////////////////////////////////////7.开启音频命令，包含音频切换///////////////////////////////////////////////////
/// @defgroup  开启音频命令
/// @{
//输入、输出节点都需要实现
typedef struct _sRemote_OpenAudio_tag {
	_sRemote_Header header;
	DP_U32 u32TaskID; ///<见任务ID注释
	DP_U8 u8DevType;  ///<采集端设备类型 @see eDeviceType

	DP_U8 au8RtspURL[DP_URL_LEN]; ///< 第三方标准RTSP //server
	_sSrcAudioInfo srcAudioInfo; //2018/11/20

	//输出信号
	DP_U8 au8DstDevID[DP_DEV_ID_LEN]; ///<节点ID
	DP_U8 u8AoChnID;				  ///<音频播放通道ID @see eDeviceAudioChannelID
} _sRemote_OpenAudio;

typedef struct _sRemote_Reply_OpenAudio_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///<节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U32 u32Success;			   ///<0成功 其他失败，含错误码
} _sRemote_Reply_OpenAudio;

///@}
////////////////////////////////////////////////////8.关闭音频命令///////////////////////////////////////////////////
/// @defgroup  关闭音频命令
/// @{
//输入、输出节点都需要实现
typedef struct _sRemote_CloseAudio_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///<输出节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U8 u8AoChnID;			   ///<音频播放通道ID @see eDeviceAudioChannelID
} _sRemote_CloseAudio;

typedef struct _sRemote_Reply_CloseAudio_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN]; ///< 节点ID
	DP_U32 u32TaskID;			   ///<见任务ID注释
	DP_U32 u32Success;			   ///<0成功 其他失败，含错误码
} _sRemote_Reply_CloseAudio;
///@} //end 关闭音频命令
///@} //end 控制软件与输入输出节点的协议

///@}
////////////////////////////////////////////////////9.设置音频信息命令///////////////////////////////////////////////////
/// @defgroup  关闭音频命令
/// @{
//输入、输出节点都需要实现
typedef struct _sRemote_SetAudio_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];		///<输出节点ID
	DP_U8 u8AoChnID;					///<音频播放通道ID @see eDeviceAudioChannelID
	DP_U8 u8AoChnMute;					///<音频通道静音
	DP_U8 u8AoChnVolume;					///<音频通道音量 0~100
} _sRemote_SetAudio;

typedef struct _sRemote_Reply_SetAudio_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];		///< 节点ID
	DP_U32 u32TaskID;						///<见任务ID注释
	DP_U32 u32Success; 					///<0成功 其他失败，含错误码
} _sRemote_Reply_SetAudio;
///@} //end 关闭音频命令
///@} //end 控制软件与输入输出节点的协议
///////////////////////////////////////////////////11 clear all task///////////////////////////////////////////////////
typedef struct _sRemote_ClearTask_tag {
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];		///<输出节点ID
	DP_U8 u8TaskType;						///<0视频任务 1音频任务 2 video & audio
} _sRemote_ClearTask;

typedef struct _sRemote_Reply_ClearTask_tag {
	_sRemote_Reply_ClearTask_tag(_sRemote_Header head, DP_U8 *devID,
			DP_U32 u32Success, DP_U32 errTaskCount) :
			header(head), u32Success(u32Success), u32ErrTaskCount(errTaskCount) {
		memset(au8DevID, 0, DP_DEV_ID_LEN);
		strcpy((DP_CHAR*) au8DevID, (DP_CHAR*) devID);
	}
	_sRemote_Header header;
	DP_U8 au8DevID[DP_DEV_ID_LEN];		///< 节点ID
	DP_U32 u32Success; 					///<0成功 其他失败，含错误码

	DP_U32 u32ErrTaskCount; // add 20181207
	//DP_U32 u32ErrTaskID[u32ErrTaskCount];
	//DP_U32 u32ErrCode[u32ErrTaskCount];

} _sRemote_Reply_ClearTask;
//add new protocol 2018/11/26

////////////////////////////////////////////////////0x20.获取信息（Json格式）///////////////////////////////////////////////////
typedef struct _sRemote_JsonGetInfo_tag {
	_sRemote_Header header;
	DP_U8 au8KeyCount;
//	DP_U8* pau8Key[64]; //每一个键大小
//#pragma region key
	//##本机信息
	//设备id
	//设备类型
	//运行模式
	//ip
	//子网掩码
	//网关
	//网卡工作模式
	//当前网卡状态
	//设备版本号
	//中控启用状态
	//当前中控状态
	//键鼠控制启用状态
	//当前键鼠状态
	//文件传输启用状态
	//音频启用状态
	//视频启用状态
	//当前工作状态
	//音视频接口连接状态
	//当前是否有u盘
	//本机环境温度
	//本机运行性能
	//本机运行模式（矩阵、拼接屏、KVM）

	//##输入节点
	//采集设备id
	//输入连接状态
	//输入信号源状态
	//输入分辨率
	//输入帧率
	//已拉取音频流->数组 源Rtsp|源Rtsp
	//输出总带宽
	//rtsp接入数量（正在拉取流的rtsp客户端数量）
	//rtsp客户端详情->数组 Rtsp地址，客户端ip，客户端ip|Rtsp地址，客户端ip，客户端ip

	//##输出节点
	//视频通道1窗口详情->数组  源Rtsp、分辨率、帧率、码流|源Rtsp、分辨率、帧率、码流
	//视频通道2窗口详情
	//视音频通道1音频详情->源Rtsp
	//视音频通道2音频详情
	//纯音频通道1音频详情
	//纯音频通道2音频详情
//#pragma endregion key
} sRemote_JsonGetInfo_tag;

typedef struct _sRemote_Reply_JsonGetInfo_tag {
	_sRemote_Header header;
	DP_U32 u32Success;
//	DP_U8* pu8JsonString;	//所有的json字符串

} sRemote_Reply_JsonGetInfo_tag;

////////////////////////////////////////////////////0x21.设置参数（Json格式）///////////////////////////////////////////////////
typedef struct _sRemote_JsonSetInfo_tag {
	_sRemote_Header header;
	DP_U8 au8KeyCount;
//	DP_U8* pau8Key[64]; //每一个键大小
//#pragma region key
	//###基本信息
	//运行模式
	//ip信息

	//###输入节点
	//中控启用
	//键鼠控制启用
	//文件传输启用
	//音频启用
	//视频启用

	//###输出节点
	//视频显示时序
//#pragma endregion key
//	DP_U8* pu8JsonString;	//所有的json字符串
} sRemote_JsonSetInfo_tag;

typedef struct _sRemote_Reply_JsonSetInfo_tag {
	_sRemote_Header header;
	DP_U8 au8KeyCount;
//	DP_U8* pau8Key[64];
//	DP_U8* pau8Result;
} sRemote_Reply_JsonSetInfo_tag;

//Command_OpenAndMoveWindow
typedef struct _OpenAndMoveWindow_S {
	_sRemote_Header header;  // sizeof 20
	DP_U32 u32TaskID; ///<见任务ID注释

	DP_U8 au8RtspURL[DP_URL_LEN]; ///< 第三方标准RTSP
	DP_U8 u8AudioIn;  				/// is contain audio or not  // 281byte

	//输出节点信息
	//视频信息
	DP_U8 au8DstDevID[DP_DEV_ID_LEN]; ///<输出节点ID
	DP_U8 u8VoChnID;				  ///<显示通道ID \a eDeviceVideoChannelID
	_sSrcVideoInfo srcVideoInfo;///<视频源信息，如果输入信息为第三方标准RTSP，则此数据结构无效\a _sSrcVideoInfo
	_sDstVideoInfo dstVideoInfo;	  ///<显示目标信息 \a _sDstVideoInfo
	_sPhyScreenInfo phyScreenInfo;	///<物理屏幕，用于视频像素对齐，暂时保留 //add 2018.10.22
} OpenAndMoveWindow_S;

//use to update batch
typedef struct _UpdateInfo_S {
	_sRemote_Header header;
	DP_U32 u32SrcIP;  //for tftp
} UpdateInfo_S;

typedef struct _UpdateStatus_S {
	_UpdateStatus_S() :
			_success(0) {
		header.stFunctionMsg.u8CommandID = Command_UpdateBatch;
		header.u16PackageLen = sizeof(_UpdateStatus_S);
	}
	_sRemote_Header header;
	DP_U32 _success;  //for tftp
} UpdateStatus_S;

#pragma pack()

#endif
//修改说明
//2018.10.22 by zzg
//1.搜索协议增加了附加数据项
//2.获取信息，Property_Get_OutputVideoChnInfo修改了返回信息的数据结构
//3.开启音频命令，修改了数据结构并添加了对第三方音频的支持
//4.开窗命令，添加了像素对齐的结构体，但该结构体作保留，暂不使用

//2018.10.23 by zzg
//1.删除_sSingleVoChnInfo下的u8VideoTaskCount变量
//2.重新修改_sAllVencChnStreamInfo的数据结构 => _sAllVencChnInfo
//3.将有关VencID或StreamID的信息统一为VencID

//2018.10.24 by zzg
//1._sSingleAiChnInfo增加au8RtspURL变量
//2._sRemote_OpenAudio增加au8RtspURL变量，并删除联合体数据

//2018.10.25 by zzg
//1. _sSingleAoChnInfo 修改变量名 u8AoChnOpenStatus 为 u8AoChnMute
//2. _sSingleVoChnInfo 添加变量 u8RelateAoChnID 、u8AoChnMute 、u8AoChnVolumn
//3._sRemote_CreateWindow修改变量u8AudioEnable为u8AudioMute
//4.添加设置音量命令

// add 20181207
//clear task count err
