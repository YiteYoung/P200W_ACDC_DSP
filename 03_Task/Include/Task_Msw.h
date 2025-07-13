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

typedef struct 
{
    unsigned int    InvCmd      :1;     // 控制INV & PFC
    unsigned int    ChgCmd      :1;     // LLC充电
    unsigned int    DcgCmd      :1;     // LLC放电
    unsigned int    GridCmd     :1;

    unsigned int    Pfc2GirdCmd :1;
    unsigned int    EvCmd       :1;
    unsigned int    Fast_Inv    :1;
    unsigned int    Fast_Pfc    :1;

    unsigned int    Resv        :8;
}Cmd_t;

typedef union 
{
    unsigned int all;
    Cmd_t        bit;
}Cmd_u;

typedef struct 
{
    unsigned int    InvSoftStart    :1;
    unsigned int    InvSoftOk       :1;
    unsigned int    PfcSoftStart    :1;
    unsigned int    PfcSoftOk       :1;

    unsigned int    LLCSoftStart    :1;
    unsigned int    LLCSoftOk       :1;
    unsigned int    RlySoftStart    :1;
    unsigned int    RlySoftOk       :1;

    unsigned int    InvSoftErr      :1;
    unsigned int    PfcSoftErr      :1;
    unsigned int    LLCSoftErr      :1;
    unsigned int    RlySoftErr      :1;

    unsigned int    InvRefOk        :1;
    unsigned int    PfcRefOk        :1;
    unsigned int    Resv            :2;
}Msw_Flag_t;

typedef struct 
{
    unsigned int    u16PowerOnCnt;
    unsigned int    u16StandByCnt;
    unsigned int    u16InvModeCnt;
    unsigned int    u16PfcModeCnt;

    unsigned int    u16FaultModeRestCnt;
    unsigned int    u16FaultModeAutoCnt;

    unsigned int    u16InvSoftErrCnt;
    unsigned int    u16PfcSoftErrCnt;
    unsigned int    u16LLCSoftErrCnt;
    unsigned int    u16RlySoftErrCnt;

    unsigned int    u16InvSoftOkChkCnt;
    unsigned int    u16PfcSoftOkChkCnt;
    unsigned int    u16LLCSoftOkChkCnt;
    unsigned int    u16RltSoftOkChkCnt;

    unsigned int    u16EvRlyWaitCnt;
    unsigned int    u16OutRlyWaitCnt;
    unsigned int    u16SoftRlyWaitCnt;
    unsigned int    u16GridRlyWaitCnt;
}Msw_Cnt_t;

typedef struct 
{
    Cmd_u           u_Cmd;

    Msw_Flag_t      t_MswFlag;
    Msw_Cnt_t       t_Cnt;

    unsigned int    WaitLLCTime;

    unsigned int    PfcModeEn;
    
}Msw_t;

TASK_MSW_FUNCTION       void            sMswTask                (void);
TASK_MSW_FUNCTION       unsigned int    sMsw_GetPfcMode         (void);
TASK_MSW_FUNCTION       unsigned int    sMsw_GetPfc2Grid        (void);


#endif
