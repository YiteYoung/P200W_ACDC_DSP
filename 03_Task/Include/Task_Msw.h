#ifndef _TASK_MSW_H
#define _TASK_MSW_H

#ifdef TASK_MSW_FUNCTION_G
#define TASK_MSW_FUNCTION extern
#else
#define TASK_MSW_FUNCTION
#endif

#include "BSP_OS.h"

TASK_MSW_FUNCTION       void            sMswTask        (void);
// TASK_MSW_FUNCTION       unsigned int    sGetPfcMode_EN  (void);

#endif
