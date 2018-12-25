#ifndef DP_UDRV_LCD_H_
#define DP_UDRV_LCD_H_


#include <dp_udrv_lcd_common.h>
#include <dp_udrv_type.h>
#define DP_UDRV_LCD_TYPE_LCD1602	1
#define DP_UDRV_LCD_ZJYOLED_12864	2

DP_S32 DP_UDRV_LCD_Init();
DP_S32 DP_UDRV_LCD_DeInit();
DP_S32 DP_UDRV_LCD_GetChipType(DP_U32 *pChipType);
DP_S32 DP_UDRV_LCD_GetCap(LCD_CAP_S *pCap); 
DP_S32 DP_UDRV_LCD_GetFWVersion(DP_U32 *pVersion);
DP_S32 DP_UDRV_LCD_CleanScreen();
DP_S32 DP_UDRV_LCD_Printf(DP_S32 x,DP_S32 y,char *fmt,...);
DP_S32 DP_UDRV_LCD_ShowString(DP_S32 x,DP_S32 y,const DP_CHAR * showBuf,DP_U16 showBufLen);
DP_S32 DP_UDRV_LCD_EnableCursor(DP_U8 flag);
DP_S32 DP_UDRV_LCD_SetCursor(DP_S32 x,DP_S32 y);
DP_S32 DP_UDRV_LCD_SetBackLight(DP_U8  flag);
DP_S32 DP_UDRV_LCD_SetExternAttr(void* pAttr);  


#endif

