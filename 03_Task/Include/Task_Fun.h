#ifndef _TASK_FUN_H
#define _TASK_FUN_H

#ifdef TASK_FUN_FUNCTION_G
#define TASK_FUN_FUNCTION extern
#else
#define TASK_FUN_FUNCTION
#endif

#include "BSP_OS.h"

TASK_FUN_FUNCTION       void    sFunTask        (void);


#endif
