#ifndef _TASK_INV_H
#define _TASK_INV_H

#ifdef TASK_INV_FUNCTION_G
#define TASK_INV_FUNCTION extern
#else
#define TASK_INV_FUNCTION
#endif

#include "BSP_OS.h"

TASK_INV_FUNCTION       void    sInvTask        (void);

#endif
