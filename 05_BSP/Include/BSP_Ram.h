#ifndef _BSP_RAM_H
#define _BSP_RAM_H

#ifdef BSP_RAM_FUNCTION_G
#define BSP_RAM_FUNCTION extern
#else
#define BSP_RAM_FUNCTION
#endif

#include "f28x_project.h"

BSP_RAM_FUNCTION    void    sInitRam        (void);

#endif
