#ifndef _TASK_SCI_H
#define _TASK_SCI_H

#ifdef TASK_SCI_FUNCTION_G
#define TASK_SCI_FUNCTION extern
#else
#define TASK_SCI_FUNCTION
#endif

#include "BSP_OS.h"

TASK_SCI_FUNCTION       void    sSciTask        (void);

#endif
