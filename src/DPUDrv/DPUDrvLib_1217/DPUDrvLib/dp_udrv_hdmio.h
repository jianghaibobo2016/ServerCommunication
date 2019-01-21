/*************************************************************************
    > File Name: HDMIO.h
    > Author: liyang
    > Mail: no
    > Created Time: 2018年10月17日 星期三 17时18分53秒
 ************************************************************************/
#ifndef DP_UDRV_HDMIO_H_
#define DP_UDRV_HDMIO_H_

#include <dp_udrv_hdmi_common.h>
#include <dp_udrv_hdmi_infoframe.h>
#include <dp_udrv_type.h>


#define DP_UDRV_HDMIO_CHIP_TYPE_9022 1


typedef struct HDMIO_VIDEO_IN_ATTR_S
{	
		HDMI_V_FMT_E inFmt; 			//1080P60?
		HDMI_V_TIMING_MODE_E timing;		//EMB SYNC SDR
		HDMI_V_MODE_E   vidoeMode;				//�������Ƶģ	
		HDMI_V_COLORSPACE inColorSpace;
		HDMI_V_COLOR_DEPTH_E inColorDepth;						
		HDMI_V_COLORSPACE outColorSpace;
		HDMI_V_COLOR_DEPTH_E outColorDepth;						//�����colorDepth
}HDMIO_VIDEO_IN_ATTR_S;




//--------------GET STATUS--------------------------------------


typedef struct HDMIO_STATUS_S
{	
	DP_U8 IsStable;				// 1 is stable,0 is unstable���ź��Ƿ��ȶ����
	DP_U8 FrameRate;			//֡�ʣ�
	DP_U8 VideoIsMute;			// 1 is mute 0 is unMute ��ǰ��Ƶ����״̬��1���������0�����
	DP_U8 InputColorMode;		//������ɫ�ռ䣬��YUV422 RGB444
	DP_U8 InputColorDepth;		//����λ��16bit 24bit ��
	DP_U8 OutputColorMode;		//�����ɫ�ռ�
	DP_U8 OutputColorDepth;		//���λ��
	DP_U8 ScanMode;				//����   ���е�ɨ�跽ʽ
	DP_U8 BusMode;				//���������ģʽ:HDMI  DVI  MHL
	DP_U8 AudioSrcEnable;		// 1 is Enable 0 is not exist,  Enable means the hdmi transmit with audio HDMI�����ź��Ƿ������Ƶ
	DP_U8 AudioIsMute;			// 1 is mute 0 is unMute
	DP_U32 AudioSampleRate;		//��Ƶ������
	DP_U8 AudioSampleSize;		//��Ƶ����λ��

}HDMIO_STATUS;


//----------------SEND CEC_CTRL-------------------------------------------------

typedef struct HDMIO_CEC_TRL_S
{	
	;
}HDMIO_CEC_TRL;



//-----------------------------------------------------------------------------------------------------


DP_S32 DP_UDRV_HDMIO_Init();
DP_S32 DP_UDRV_HDMIO_DeInit();	
DP_S32 DP_UDRV_HDMIO_GetChipType(DP_U32*  pChipType);
DP_S32 DP_UDRV_HDMIO_GetFWVersion(DP_U32* pVersion);

DP_S32 DP_UDRV_HDMIO_GetHPDStatus(DP_U32* pHPDStatus);
DP_S32 DP_UDRV_HDMIO_GetEDIDRAM(DP_U32* pEDIDRam);

DP_S32 DP_UDRV_HDMIO_SetVideoInputAttr(HDMIO_VIDEO_IN_ATTR_S *pAttr);
DP_S32 DP_UDRV_HDMIO_GetVideoInputAttr(HDMIO_VIDEO_IN_ATTR_S *pAttr);
DP_S32 DP_UDRV_HDMIO_SetAudioAttr(HDMI_AUDIO_ATTR_S *pAudioAttr);
DP_S32 DP_UDRV_HDMIO_GetAudioAttr(HDMI_AUDIO_ATTR_S *pAudioAttr);

DP_S32 DP_UDRV_HDMIO_SetInfoFrame(HDMI_INFOFRAME_TYPE_E type,void* pAttr);
DP_S32 DP_UDRV_HDMIO_GetInfoFrame(HDMI_INFOFRAME_TYPE_E type,void* pAttr);
DP_S32 DP_UDRV_HDMIO_GetStatus(HDMIO_STATUS *statusInfo);
DP_S32 DP_UDRV_HDMIO_SendCECCtrl(HDMIO_CEC_TRL *pCecTrl);
DP_S32 DP_UDRV_HDMIO_SetExternAttr(void* pAttr);




#endif

