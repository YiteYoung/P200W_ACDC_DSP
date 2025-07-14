#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "f28x_project.h"

#ifdef BSP_GPIO_FUNCTION_G
#define BSP_GPIO_FUNCTION extern
#else
#define BSP_GPIO_FUNCTION
#endif

BSP_GPIO_FUNCTION   void    sInitGpio      (void);


#endif
