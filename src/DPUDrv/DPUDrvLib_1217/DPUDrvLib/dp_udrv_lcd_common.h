#ifndef LCD_COMMON_H_
#define	LCD_COMMON_H_

#include "dp_udrv_type.h"

#define LCD_SUPPORT_LANGUAGE_ENGLISH	BIT0
#define LCD_SUPPORT_LANGUAGE_CHINESE	BIT1

typedef struct _LCD_CAP_S
{
	DP_U8 supportLanguage;
	DP_U8 maxColumnENG;// max Column in English mode 列
	DP_U8 maxRowENG;// max row in English mode	行
	DP_U8 maxColumnCHN;// max Column in Chinese mode 列
	DP_U8 maxRowCHN;// max row in Chinese mode 行
	DP_BOOL isSupportCursor;//是否支持自动光标
	DP_BOOL isSupportBacklightSet;
}LCD_CAP_S;

#endif
