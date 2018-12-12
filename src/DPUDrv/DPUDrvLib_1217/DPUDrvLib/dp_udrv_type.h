#ifndef __DP_UDRV_TYPE_H__
#define __DP_UDRV_TYPE_H__

#include <stdio.h>

/* st type param define */
typedef signed char			DP_S8;
typedef unsigned char		DP_U8;

typedef short		        DP_S16;
typedef unsigned short		DP_U16;

typedef int		        DP_S32;
typedef unsigned int		DP_U32;

typedef long long           DP_S64;
typedef unsigned long long	DP_U64;

typedef size_t		        DP_SIZE_T;
typedef float           	DP_FLOAT;
typedef double          	DP_DOUBLE;
typedef void            	DP_VOID;

/* st type point param define */
typedef void*            	DP_PVOID;

//typedef enum
//{
//    DP_FALSE    = 0,
//    DP_TRUE     = 1,
//} DP_BOOL;

#define DP_BIT0	(1<<0)
#define DP_BIT1	(1<<1)
#define DP_BIT2	(1<<2)
#define DP_BIT3	(1<<3)
#define DP_BIT4	(1<<4)
#define DP_BIT5	(1<<5)
#define DP_BIT6	(1<<6)
#define DP_BIT7	(1<<7)

#define DP_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif
