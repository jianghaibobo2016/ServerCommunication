// #include "dp_sd_type.h"

#ifndef __DP_SD_TYPE_H__
#define __DP_SD_TYPE_H__

typedef unsigned char 	DP_U8;
typedef unsigned short 	DP_U16;
typedef unsigned int 	DP_U32;
typedef signed char 	DP_S8;
typedef short 			DP_S16;
typedef int 			DP_S32;

#ifndef _M_IX86
typedef unsigned long long DP_U64;
typedef long long DP_S64;
#else
typedef __int64 DP_U64;
typedef __int64 DP_S64;
#endif
typedef char DP_CHAR;

typedef float           	DP_FLOAT;
typedef double          	DP_DOUBLE;
typedef void            	DP_VOID;

typedef void*            	DP_PVOID;

/*----------------------------------------------*
* const defination *
*----------------------------------------------*/
typedef enum {
	
	DP_FALSE = 0,
	DP_TRUE = 1,
} DP_BOOL;

#ifndef NULL
#define NULL 0L
#endif

#define DP_NULL 0L
#define DP_SUCCESS 0
#define DP_FAILURE (-1)

#endif
