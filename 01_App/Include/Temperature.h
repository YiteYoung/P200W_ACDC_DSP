#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H

#ifdef TEMPERATURE_FUNCTION_G
#define TEMPERATURE_FUNCTION extern
#else
#define TEMPERATURE_FUNCTION
#endif

#include "BSP_Adc.h"

typedef struct
{
    signed int Temperature_NTC1;
    signed int Temperature_NTC2;
    signed int Temperature_NTC3;
}Temperature_t;

#endif
