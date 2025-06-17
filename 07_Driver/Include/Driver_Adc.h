#ifndef _DRIVER_ADC_H
#define _DRIVER_ADC_H

#include "f28x_project.h"

#define ADCARESULT_BASE 0x0B00
#define ADCCRESULT_BASE 0x0B40

#ifdef DRIVER_ADC_FUNCTION_G
#define DRIVER_ADC_FUNCTION extern
#else
#define DRIVER_ADC_FUNCTION
#endif

DRIVER_ADC_FUNCTION     void    sDrv_IninAdc        (void);   

#endif
