#ifndef _BSP_XBAR_H
#define _BSP_XBAR_H

#ifdef BSP_XBAR_FUNCTION_G
#define BSP_XBAR_FUNCTION extern
#else
#define BSP_XBAR_FUNCTION
#endif

#include "f28x_project.h"

BSP_XBAR_FUNCTION   void    sInitInputXbar      (void);
BSP_XBAR_FUNCTION   void    sInitOutputXbar     (void);
BSP_XBAR_FUNCTION   void    sInitEPwmXbar       (void);

#endif

