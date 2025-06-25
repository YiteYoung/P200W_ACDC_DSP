#ifndef _BSP_CMPSS_H
#define _BSP_CMPSS_H

#include "Driver_Cmpss.h"
#include "BSP_Adc.h"

#ifdef BSP_CMPSS_FUNCTION_G
#define BSP_CMPSS_FUNCTION extern
#else
#define BSP_CMPSS_FUNCTION
#endif

#ifndef ARRAY_SIZE
#define sARRAY_SIZE(x)      (sizeof(x) / sizeof( (x)[0]))
#endif

BSP_CMPSS_FUNCTION  void    sInitCmpss  (void);

#endif
