#ifndef _TASK_MSW_H
#define _TASK_MSW_H

#ifdef TASK_MSW_FUNCTION_G
#define TASK_MSW_FUNCTION extern
#else
#define TASK_MSW_FUNCTION
#endif

#include "BSP_OS.h"

#define cPowerOnMode    0
#define cStandByMode    1
#define cInvMode        2
#define cPfcMode        3
#define cTestMode       4
#define cFaultMode      5
#define cBootMode       6

typedef enum
{
    SYS_Off  = 0x00,

    LLC_Soft,
    RLY_Soft,
    Rly_OK,
    Pfc_Soft,
    Pfc_OK,
    Inv_Soft,
    Inv_OK,

    SYS_End
}SysStatus_e;

TASK_MSW_FUNCTION       void            sMswTask                (void);
TASK_MSW_FUNCTION       unsigned int    sMsw_GetPfcMode         (void);

#endif
