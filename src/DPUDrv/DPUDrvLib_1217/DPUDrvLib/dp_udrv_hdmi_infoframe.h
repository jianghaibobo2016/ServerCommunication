#ifndef HDMI_INFO_FRAME_H_
#define HDMI_INFO_FRAME_H_
#include <dp_udrv_type.h>

typedef enum hdmi_infoframe_type {
	HDMI_INFOFRAME_TYPE_VENDOR = 0x81,
	HDMI_INFOFRAME_TYPE_AVI = 0x82,
	HDMI_INFOFRAME_TYPE_SPD = 0x83,
	HDMI_INFOFRAME_TYPE_AUDIO = 0x84,
}HDMI_INFOFRAME_TYPE_E;

#define HDMI_INFOFRAME_HEADER_SIZE  4
#define HDMI_AVI_INFOFRAME_SIZE    13
#define HDMI_SPD_INFOFRAME_SIZE    25
#define HDMI_AUDIO_INFOFRAME_SIZE  10

typedef enum hdmi_colorspace {
	HDMI_COLORSPACE_RGB,
	HDMI_COLORSPACE_YUV422,
	HDMI_COLORSPACE_YUV444,
}HDMI_COLORSPACE_E;

typedef enum hdmi_scan_mode {
	HDMI_SCAN_MODE_NONE,
	HDMI_SCAN_MODE_OVERSCAN,
	HDMI_SCAN_MODE_UNDERSCAN,
}HDMI_SCAN_MODE_E;

typedef enum hdmi_colorimetry {
	HDMI_COLORIMETRY_NONE,
	HDMI_COLORIMETRY_ITU_601,
	HDMI_COLORIMETRY_ITU_709,
	HDMI_COLORIMETRY_UNKNOW,
}HDMI_COLORIMETRY_E;

typedef enum hdmi_picture_aspect {
	HDMI_PICTURE_ASPECT_NONE,
	HDMI_PICTURE_ASPECT_4_3,
	HDMI_PICTURE_ASPECT_16_9,
}HDMI_PICTURE_ASPECT_E;

typedef enum hdmi_active_aspect {
	HDMI_ACTIVE_ASPECT_16_9_TOP = 2,
	HDMI_ACTIVE_ASPECT_14_9_TOP = 3,
	HDMI_ACTIVE_ASPECT_16_9_CENTER = 4,
	HDMI_ACTIVE_ASPECT_PICTURE = 8,
	HDMI_ACTIVE_ASPECT_4_3 = 9,
	HDMI_ACTIVE_ASPECT_16_9 = 10,
	HDMI_ACTIVE_ASPECT_14_9 = 11,
	HDMI_ACTIVE_ASPECT_4_3_SP_14_9 = 13,
	HDMI_ACTIVE_ASPECT_16_9_SP_14_9 = 14,
	HDMI_ACTIVE_ASPECT_16_9_SP_4_3 = 15,
}HDMI_ACTIVE_ASPECT_E;

typedef enum hdmi_extended_colorimetry {
	HDMI_EXTENDED_COLORIMETRY_XV_YCC_601,
	HDMI_EXTENDED_COLORIMETRY_XV_YCC_709,
	HDMI_EXTENDED_COLORIMETRY_S_YCC_601,
	HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601,
	HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB,
}HDMI_EXTENDED_COLORIMETRY_E;

typedef enum hdmi_quantization_range {
	HDMI_QUANTIZATION_RANGE_DEFAULT,
	HDMI_QUANTIZATION_RANGE_LIMITED,
	HDMI_QUANTIZATION_RANGE_FULL,
}HDMI_QUANTIZATION_RANGE_E;

/* non-uniform picture scaling */
typedef enum hdmi_nups {
	HDMI_NUPS_UNKNOWN,
	HDMI_NUPS_HORIZONTAL,
	HDMI_NUPS_VERTICAL,
	HDMI_NUPS_BOTH,
}HDMI_NUPS_E;

typedef enum hdmi_ycc_quantization_range {
	HDMI_YCC_QUANTIZATION_RANGE_LIMITED,
	HDMI_YCC_QUANTIZATION_RANGE_FULL,
}HDMI_YCC_QUANTIZATION_RANGE_E;

typedef enum hdmi_content_type {
	HDMI_CONTENT_TYPE_NONE,
	HDMI_CONTENT_TYPE_PHOTO,
	HDMI_CONTENT_TYPE_CINEMA,
	HDMI_CONTENT_TYPE_GAME,
}HDMI_CONTENT_TYPE_E;

typedef struct hdmi_avi_infoframe {
	HDMI_INFOFRAME_TYPE_E type;
	DP_U8 version;
	DP_U8 length;
	HDMI_COLORSPACE_E colorspace;
	DP_BOOL active_info_valid;
	DP_BOOL horizontal_bar_valid;
	DP_BOOL vertical_bar_valid;
	HDMI_SCAN_MODE_E scan_mode;
	HDMI_COLORIMETRY_E colorimetry;
	HDMI_PICTURE_ASPECT_E picture_aspect;
	HDMI_ACTIVE_ASPECT_E active_aspect;
	DP_BOOL itc;
	HDMI_EXTENDED_COLORIMETRY_E extended_colorimetry;
	HDMI_QUANTIZATION_RANGE_E quantization_range;
	HDMI_NUPS_E nups;
	DP_U8 video_code;
	HDMI_YCC_QUANTIZATION_RANGE_E ycc_quantization_range;
	HDMI_CONTENT_TYPE_E content_type;
	DP_U8 pixel_repeat;
	DP_U16 top_bar;
	DP_U16 bottom_bar;
	DP_U16 left_bar;
	DP_U16 right_bar;
}HDMI_AVI_INFO_FRAME_S;

#endif
