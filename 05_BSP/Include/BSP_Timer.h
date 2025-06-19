#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#ifdef BSP_TIMER_FUNCTION_G
#define BSP_TIMER_FUNCTION  extern
#else
#define BSP_TIMER_FUNCTION
#endif

#include "f28x_project.h"

BSP_TIMER_FUNCTION  void            sInitTimer      (void);
BSP_TIMER_FUNCTION  void            sSYS_Time       (void);

BSP_TIMER_FUNCTION  unsigned int    sGetTime_1ms    (void);
BSP_TIMER_FUNCTION  unsigned int    sGetTime_2ms    (void);
BSP_TIMER_FUNCTION  unsigned int    sGetTime_4ms    (void);
BSP_TIMER_FUNCTION  unsigned int    sGetTime_10ms   (void);
BSP_TIMER_FUNCTION  unsigned int    sGetTime_20ms   (void);
BSP_TIMER_FUNCTION  unsigned int    sGetTime_100ms  (void);
BSP_TIMER_FUNCTION  unsigned int    sGetTime_1s     (void);

#endif
