#ifndef _CONTROL_LLC_H
#define _CONTROL_LLC_H

#ifdef CONTROL_LLC_FUNCTION_G
#define CONTROL_LLC_FUNCTION extern
#else
#define CONTROL_LLC_FUNCTION
#endif

typedef enum 
{
    eLLC_FSM_OFF = 0,
    eLLC_FSM_SOFT,
    eLLC_FSM_RUN
}LLC_FSM_e;

typedef struct 
{
    unsigned int    u16Cnt;
    unsigned int    u16Act;
    unsigned int    u16EN;
}Burst_t;

typedef struct 
{
    float f32Out;    
    float f32Out1;
    float f32Out2;

    float f32Err0;
    float f32Err1;
    float f32Err2;

    float f32Ref;
    float f32Fed;
}STD_2P2Z_F_VARS;

typedef struct 
{
    unsigned int    u16OpenLoopEn;

    unsigned int    u16LLC_State;
    unsigned int    u16LLC_Prd;
    unsigned int    u16LLC_Ps;

    float           f32BusVoltSet;
    float           f32BusVoltRef;
    float           f32BusVoltStep;

    unsigned int    u16FastSoftFlag;
    unsigned int    u16FastStartCnt;

    float           f32DcgCurrLimit;
    float           f32DcgPiOutLimitUp;
    float           f32DcgPiOutLimitDn;

    float           f32ILoopK2_Ref;
    float           f32ILoopK3_Ref;

    float           f32BatCurrRef;
    float           f32ChgPiOutLimitUp;
    float           f32ChgPiOutLimitDn;

    float           f32PsLimit;
    float           f32PsLimitStep;

    Burst_t         t_ChgBurst;
    Burst_t         t_DcgBurst;

    unsigned int    u16PwmDelayCnt;
    unsigned int    u16PwmEn;

    STD_2P2Z_F_VARS t_ChgVoltLoop;
    STD_2P2Z_F_VARS t_ChgCurrLoop;
    STD_2P2Z_F_VARS t_DcgVoltLoop;
    STD_2P2Z_F_VARS t_DcgCurrLoop;
}LLcControl_t;

CONTROL_LLC_FUNCTION    void    sLLC_Control        (void);

#endif

