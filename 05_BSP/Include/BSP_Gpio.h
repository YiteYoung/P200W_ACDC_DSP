#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "f28x_project.h"

#ifdef BSP_GPIO_FUNCTION_G
#define BSP_GPIO_FUNCTION extern
#else
#define BSP_GPIO_FUNCTION
#endif

// P200W
#define sDebugLed_Hi()        GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
#define sDebugLed_Lo()        GpioDataRegs.GPASET.bit.GPIO20 = 1;
#define sDebugLed_Run()       GpioDataRegs.GPATOGGLE.bit.GPIO20 = 1;

// Launch
#define sDebugLed4_Hi()        GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
#define sDebugLed4_Lo()        GpioDataRegs.GPASET.bit.GPIO20 = 1;
#define sDebugLed4_Run()       GpioDataRegs.GPATOGGLE.bit.GPIO20 = 1;

// Launch
#define sDebugLed5_Hi()        GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
#define sDebugLed5_Lo()        GpioDataRegs.GPASET.bit.GPIO22 = 1;
#define sDebugLed5_Run()       GpioDataRegs.GPATOGGLE.bit.GPIO22 = 1;

BSP_GPIO_FUNCTION   void    sInitGpio      (void);


#endif
