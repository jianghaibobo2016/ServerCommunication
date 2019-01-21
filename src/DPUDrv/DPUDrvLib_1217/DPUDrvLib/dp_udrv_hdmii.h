#ifndef DP_UDRV_HDMII_H_
#define DP_UDRV_HDMII_H_

#include <dp_udrv_hdmi_common.h>
#include <dp_udrv_type.h>

#define HDMII_CHIP_TYPE_ITE6801	1

typedef struct _HDMII_STATUS_INFO_S
{
	DP_U8 IsStable;// 1 is stable,0 is unstable，信号是否稳定输出
	DP_U16 HActive ;//水平有效数据长
	DP_U16 VActive ;//垂直有效数据长
	DP_U16 HTotal ;//水平数据长
	DP_U16 VTotal ;//垂直数据长
	DP_U8 FrameRate;//帧率，ITE6801 4Kx2K帧率获取不准确
	DP_U8 VideoIsMute;// 1 is mute 0 is unMute 当前视频静音状态，1就无输出，0有输出

	DP_U8 InputColorMode;//输入颜色空间，如YUV422 RGB444
	DP_U8 InputColorDepth;//输入位深16bit 24bit 等 
	DP_U8 OutputColorMode;//输出颜色空间
	DP_U8 OutputColorDepth;//输出位深
	DP_U8 ScanMode;//逐行   隔行等扫描方式
	DP_U8 BusMode;//输入的总线模式:HDMI  DVI  MHL
	DP_U8 AudioSrcEnable;// 1 is Enable 0 is not exist,  Enable means the hdmi transmit with audio HDMI输入信号是否带有音频
	DP_U8 AudioIsMute;// 1 is mute 0 is unMute
	DP_U32 AudioSampleRate;//音频采样率
}HDMII_STATUS_INFO_S;

typedef struct _HDMII_VIDEO_OUT_ATTR_S
{
	HDMI_V_TIMING_MODE_E timingMode;
	HDMI_V_COLORSPACE inColorSpace;
	HDMI_V_COLOR_DEPTH_E inColorDepth;						
	HDMI_V_COLORSPACE outColorSpace;
	HDMI_V_COLOR_DEPTH_E outColorDepth;						//�����colorDepth
}HDMII_VIDEO_OUT_ATTR_S;


DP_S32 DP_UDRV_HDMII_Init();
DP_S32 DP_UDRV_HDMII_DeInit(); 
DP_S32 DP_UDRV_HDMII_GetChipType(DP_U32*  pChipType);
DP_S32 DP_UDRV_HDMII_GetFWVersion(DP_U32* pVersion);
DP_S32 DP_UDRV_HDMII_GetCaps(HDMI_CHIP_CAPS* pCaps);

DP_S32 DP_UDRV_HDMII_GetStatusInfo(HDMII_STATUS_INFO_S*statusInfo);
DP_S32 DP_UDRV_HDMII_SetVideoOutputAttr(HDMII_VIDEO_OUT_ATTR_S *pAttr);
DP_S32 DP_UDRV_HDMII_GetVideoOutputAttr(HDMII_VIDEO_OUT_ATTR_S *pAttr);

DP_S32 DP_UDRV_HDMII_SetAudioAttr(HDMI_AUDIO_ATTR_S *pAudioAttr);
DP_S32 DP_UDRV_HDMII_GetAudioAttr(HDMI_AUDIO_ATTR_S *pAudioAttr);

DP_S32 DP_UDRV_HDMII_UpdateEDID(const DP_U8 *pEDID,DP_U16 EDIDLen);
//return <=0 Common Error Code >0 is EDID bufLen,default is 256
DP_S32 DP_UDRV_HDMII_GetEDID(DP_U8 *pEDID,DP_U16 EDIDLen);

DP_S32 DP_UDRV_HDMII_HPDCtrl(DP_U8 Value);
DP_S32 DP_UDRV_HDMII_SetAudioMute(DP_U8 isMute);
DP_S32 DP_UDRV_HDMII_SetVideoMute(DP_U8 isMute);
DP_S32 DP_UDRV_HDMII_GetInfoFrame(DP_U8* pInfoFrameBuf,DP_U16 * pBufLen);
DP_S32 DP_UDRV_HDMII_SetExternAttr(void* pAttr);

#endif
