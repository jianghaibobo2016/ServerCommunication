#ifndef DP_UDRV_HDMI_COMMON_H_
#define DP_UDRV_HDMI_COMMON_H_

#include <dp_udrv_type.h>
//-----------------SET AND GET VIDEO_ATTR-----------------------------------

typedef enum _HDMI_V_FMT_E
{	
 	HDMI_V_FMT_PAL = 0,
    HDMI_V_FMT_NTSC,
    HDMI_V_FMT_960H_PAL,              /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced)*/
    HDMI_V_FMT_960H_NTSC,             /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced)*/
    
    HDMI_V_FMT_1080P24,
    HDMI_V_FMT_1080P25,
    HDMI_V_FMT_1080P30,
    
    HDMI_V_FMT_720P50, 
    HDMI_V_FMT_720P60,   
    HDMI_V_FMT_1080I50,
    HDMI_V_FMT_1080I60,    
    HDMI_V_FMT_1080P50,
    HDMI_V_FMT_1080P60,            

    HDMI_V_FMT_576P50,
    HDMI_V_FMT_480P60,

    HDMI_V_FMT_640x480_60,            /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
    HDMI_V_FMT_800x600_60,            /* VESA 800 x 600 at 60 Hz (non-interlaced) */
    HDMI_V_FMT_1024x768_60,           /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    HDMI_V_FMT_1280x1024_60,          /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    HDMI_V_FMT_1366x768_60,           /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    HDMI_V_FMT_1440x900_60,           /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */
    HDMI_V_FMT_1280x800_60,           /* 1280*800@60Hz VGA@60Hz*/    
    HDMI_V_FMT_1680x1050_60,          /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */
    HDMI_V_FMT_1920x2160_30,          /* 1920x2160_30 */
    HDMI_V_FMT_1600x1200_60,          /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
    HDMI_V_FMT_1920x1200_60,          /* VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT (Reduced Blanking)*/ 
    HDMI_V_FMT_2560x1440_30,          /* 2560x1440_30 */
    HDMI_V_FMT_2560x1440_60,          /* 2560x1440_60 */
    HDMI_V_FMT_2560x1600_60,          /* 2560x1600_60 */
    HDMI_V_FMT_3840x2160_25,          /* 3840x2160_25 */
    HDMI_V_FMT_3840x2160_30,          /* 3840x2160_30 */
    HDMI_V_FMT_3840x2160_50,          /* 3840x2160_50 */
    HDMI_V_FMT_3840x2160_60,          /* 3840x2160_60 */
    HDMI_V_FMT_USER,
    HDMI_V_FMT_BUTT		

}HDMI_V_FMT_E;



typedef enum _HDMI_V_TIMING_MODE_E
{
	HDMI_V_TIM_EMB_SYNC_SDR = 0,
	HDMI_V_TIM_EMB_SYNC_DDR,
	HDMI_V_TIM_SEP_SYNC_SDR,
	HDMI_V_TIM_SEP_SYNC_DDR,
	HDMI_V_TIM_HALF_BUS,
	HDMI_V_TIM_RESERVE
}HDMI_V_TIMING_MODE_E;
	

typedef enum _HDMI_V_COLORSPACE
{	
	HDMI_V_COLORSPACE_AUTO = 0,
	HDMI_V_COLORSPACE_RGB444,				
 	HDMI_V_COLORSPACE_YCBCR422,		
 	HDMI_V_COLORSPACE_YCBCR444,	
 	HDMI_V_COLORSPACE_CCIR656,

}HDMI_V_COLORSPACE;


typedef enum _HDMI_V_DEPTH_E
{	
	HDMI_V_DEPTH_AUTO = 0,
	HDMI_V_DEPTH_8BIT ,	
	HDMI_V_DEPTH_10BIT,			
	HDMI_V_DEPTH_12BIT,			
}HDMI_V_COLOR_DEPTH_E;


//VID use in INFOFrame
typedef enum _HDMI_V_MODE_E
{	
	HDMI_VIC_CEA ,
	HDMI_VIC_HDMI_DVI,
	HDMI_VIC_3D,

}HDMI_V_MODE_E;



#define HDMI_V_CAPS_COLOR_MODE_RGB444_24BIT	DP_BIT0
#define HDMI_V_CAPS_COLOR_MODE_YUV444_24BIT	DP_BIT1
#define HDMI_V_CAPS_COLOR_MODE_RGB444_30BIT	DP_BIT2
#define HDMI_V_CAPS_COLOR_MODE_YUV444_30BIT	DP_BIT3
#define HDMI_V_CAPS_COLOR_MODE_YUV422_8BIT	DP_BIT4
#define HDMI_V_CAPS_COLOR_MODE_YUV422_16BIT	DP_BIT5
#define HDMI_V_CAPS_COLOR_MODE_YUV422_24BIT	DP_BIT6
#define HDMI_V_CAPS_COLOR_MODE_ALL		(0xffff)	

#define HDMI_V_CAPS_DATA_RATE_NORMAL	DP_BIT0
#define HDMI_V_CAPS_DATA_RATE_DOUBLE	DP_BIT1
#define HDMI_V_CAPS_DATA_RATE_BOTH	(HDMI_V_CAPS_DATA_RATE_NORMAL|HDMI_V_CAPS_DATA_RATE_DOUBLE)


#define HDMI_V_CAPS_SYNC_MODE_EMB	DP_BIT0
#define HDMI_V_CAPS_SYNC_MODE_SEP	DP_BIT1
#define HDMI_V_CAPS_SYNC_MODE_BOTH	(HDMI_V_CAPS_SYNC_MODE_EMB|HDMI_V_CAPS_SYNC_MODE_SEP)

typedef enum _HDMI_V_CAPS_MAX_LINK_RATE
{
	HDMI_V_CAPS_MAX_LINK_RATE_3G = 0,//MAX Support 4Kx2KP30 or 1080P120
	HDMI_V_CAPS_MAX_LINK_RATE_1P5G,//MAX Support 1080P60
	HDMI_V_CAPS_MAX_LINK_RATE_RESERVED
}HDMI_V_CAPS_MAX_LINK_RATE_E;
	
typedef struct _HDMI_V_CAPS
{
	DP_U16 colorSpaceCaps;//YUV422 444 RGB444
	DP_U8 syncModeCaps;//EMB SEP
	HDMI_V_CAPS_MAX_LINK_RATE_E videoLinkRateMax;//1.5G 3G
	DP_U8 videoDataRateCaps;//DDR SDR
}HDMI_V_CAPS;

#define HDMI_A_CAPS_SAMPLE_RATE_32K		DP_BIT0
#define HDMI_A_CAPS_SAMPLE_RATE_44P1K	DP_BIT1
#define HDMI_A_CAPS_SAMPLE_RATE_48K		DP_BIT2
#define HDMI_A_CAPS_SAMPLE_RATE_96K		DP_BIT3
#define HDMI_A_CAPS_SAMPLE_RATE_88P2K	DP_BIT4
#define HDMI_A_CAPS_SAMPLE_RATE_176P4K	DP_BIT5
#define HDMI_A_CAPS_SAMPLE_RATE_192K		DP_BIT6
#define HDMI_A_CAPS_SAMPLE_RATE_BOTE		(0xffff)

#define HDMI_A_CAPS_AMP_RATIO_0X		DP_BIT0
#define HDMI_A_CAPS_AMP_RATIO_1X		DP_BIT1
#define HDMI_A_CAPS_AMP_RATIO_2X		DP_BIT2
#define HDMI_A_CAPS_AMP_RATIO_3X		DP_BIT3
#define HDMI_A_CAPS_AMP_RATIO_BOTF	(0xffff)

#define HDMI_A_CAPS_INT_I2S	DP_BIT0
#define HDMI_A_CAPS_INT_SPDIF	DP_BIT1
#define HDMI_A_CAPS_INT_ALL	(0xff)


typedef struct _HDMI_A_CAPS
{
	DP_U8  interfaceCaps;//I2S SPDIF
	DP_U16 sampleRateCaps;//
	DP_BOOL isSupportReSample;
	DP_U16 channelNumMax;
	DP_U8 widthMax;
	DP_U16 ampRatioCaps;
}HDMI_A_CAPS;


typedef struct _HDMI_CHIP_CAPS
{
	HDMI_V_CAPS videoCaps;
	HDMI_A_CAPS audioCaps;
}HDMI_CHIP_CAPS;


typedef enum _HDMI_A_BIT_WIDTH_E
{
    HDMI_A_BIT_WIDTH_8   = 0,   /* 8bit width */
    HDMI_A_BIT_WIDTH_16  = 1,   /* 16bit width*/
    HDMI_A_BIT_WIDTH_17  = 2,   /* 17bit width*/
	HDMI_A_BIT_WIDTH_18  = 3,	/*18bit width*/
	HDMI_A_BIT_WIDTH_19  = 4,	/*19bit width*/
	HDMI_A_BIT_WIDTH_20  = 5,	/*120bit width*/
	HDMI_A_BIT_WIDTH_21  = 6,	/*21bit width*/
	HDMI_A_BIT_WIDTH_22  = 7,	/*22bit width*/
	HDMI_A_BIT_WIDTH_23  = 8,	/*23bit width*/
	HDMI_A_BIT_WIDTH_24  = 9,	/*24bit width*/

    HDMI_A_BIT_WIDTH_BUTT,
} HDMI_A_BIT_WIDTH_E;
	
typedef enum _HDMI_A_SAMPLE_RATE_E 
{ 
    HDMI_A_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    HDMI_A_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/    
    HDMI_A_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
    HDMI_A_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    HDMI_A_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
    HDMI_A_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
    HDMI_A_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    HDMI_A_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
    HDMI_A_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
    HDMI_A_SAMPLE_RATE_BUTT,
}HDMI_A_SAMPLE_RATE_E ; 


typedef enum _HDMI_A_INT_E 
{ 
	HDMI_A_INT_I2S = 0,
	HDMI_A_INT_SPDIF,
	HDMI_A_INT_HBR,
	HDMI_A_INT_DSD,
} HDMI_A_INT_E; 

typedef enum _HDMI_A_MCLK_FS_RATIO_E
{ 
	HDMI_A_MCLK_TO_FS_128 = 0,
	HDMI_A_MCLK_TO_FS_192,
	HDMI_A_MCLK_TO_FS_256,
	HDMI_A_MCLK_TO_FS_384,
	HDMI_A_MCLK_TO_FS_512,
	HDMI_A_MCLK_TO_FS_768,
	HDMI_A_MCLK_TO_FS_1024,
} HDMI_A_MCLK_FS_RATIO_E; 



typedef struct _HDMI_AUDIO_ATTR_S
{	
	HDMI_A_INT_E   interfaceMode;   //SPDIF I2S
	DP_U8   channelNum;		//ͨ������0��auto
	HDMI_A_SAMPLE_RATE_E  sampleRate;	//������,0�ǲ���
	HDMI_A_BIT_WIDTH_E   width;	// 16bit 20bit 24bit
	HDMI_A_MCLK_FS_RATIO_E	mclkToFsRatio;
}HDMI_AUDIO_ATTR_S;


#endif
