#include "UserHeader.h"

void                sPowerOnMode        (void);
void                sStandByMode        (void);
void                sInvMode            (void);
void                sPfcMode            (void);
void                sFaultMode          (void);
void                sTestMode           (void);

void                sRlySoft            (void);
void                sInvSoft            (void);
void                sPfcSoft            (void);
void                sChgSoft            (void);
void                sDcgSoft            (void);

void                sFastInv            (void);
void                sFastPfc            (void);

void                sByPass             (void);

unsigned char       sPfc_OnChk          (void);
unsigned char       sInv_OnChk          (void);
unsigned char       sBypass_OnChk       (void);
unsigned char       sEv_OnChk           (void);

void                sSetPfcMode         (void);
void                sSetInvMode         (void);

void                sInvOutRly          (void);
void                sEvOutRly           (void);

void                sMsw_SysOff         (void);
void                sMsw_IgbtOff        (void);
void                sMsw_InvOff         (void);
void                sMsw_SysOff_ISR     (void);

static Msw_t        t_Msw;
static signed int   i16NowSYSMode;
static signed int   i16PreSYSMode;
static signed int   i16BakSYSMode;

static signed int   i16SYSStauts;
// static signed int   i16ShutDownFlag;


void    sMswTask(void)
{
    static unsigned int MswInit_Ok;
    if(MswInit_Ok == false)
    {
        MswInit_Ok = true;

        i16NowSYSMode   = cPowerOnMode;
        i16PreSYSMode   = i16NowSYSMode;
        i16BakSYSMode   = i16NowSYSMode;

        i16SYSStauts    = SYS_Off;

        OSEventClr(cPrioMsw);
    }

    if( i16BakSYSMode != i16NowSYSMode )
    {
        i16BakSYSMode = i16NowSYSMode;

        OSEventClr(cPrioMsw);
    }

    if( i16NowSYSMode == cPowerOnMode )
    {
        sPowerOnMode();
    }
    else if( i16NowSYSMode == cStandByMode )
    {
        sStandByMode();
    }
    else if( i16NowSYSMode == cInvMode  )
    {
        sInvMode();
    }
    else if( i16NowSYSMode == cPfcMode  )
    {
        sPfcMode();
    }
    else if( i16NowSYSMode == cTestMode )  
    {
        sTestMode();   
    }
    else if( i16NowSYSMode == cBootMode )
    {
        // sBootMode();
    }
    else 
    {
        sFaultMode();
    }

    
}

void    sPowerOnMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {
        t_Msw.u_Cmd.all = 0;

        t_Msw.t_Cnt.u16PowerOnCnt = 0;

        sMsw_SysOff();
    }

    if( event & ((TASK_EVENT)1 << eMsw_Boot))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd  = false;
        t_Msw.u_Cmd.bit.ChgCmd  = false;
        t_Msw.u_Cmd.bit.DcgCmd  = false;

        i16PreSYSMode           = i16NowSYSMode;
        i16NowSYSMode           = cBootMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        if(sGetTime_10ms() == false)
        {
            return;
        }

        if( sFault_GetAlarmBit(eAlarm_WaitParaSet) == true )
        {
            return;
        }

        t_Msw.t_Cnt.u16PowerOnCnt++;
        if(t_Msw.t_Cnt.u16PowerOnCnt >= 10)
        {
            t_Msw.t_Cnt.u16PowerOnCnt = 0;

            i16PreSYSMode           = i16NowSYSMode;
            i16NowSYSMode           = cStandByMode;
            return;
        }
    }
}

void    sStandByMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sInvMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sPfcMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sFaultMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {
        t_Msw.t_Cnt.u16FaultModeRestCnt = 0;
        t_Msw.t_Cnt.u16FaultModeAutoCnt = 0;

        t_Msw.u_Cmd.bit.InvCmd          = false;
        t_Msw.u_Cmd.bit.ChgCmd          = false;
        t_Msw.u_Cmd.bit.DcgCmd          = false;

        t_Msw.u_Cmd.bit.Fast_Inv        = false;
        t_Msw.u_Cmd.bit.Fast_Pfc        = false;

        sMsw_SysOff();

        t_Msw.PfcModeEn                 = false;
    }

    if( event & ((TASK_EVENT)1 << eMsw_Boot))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd  = false;
        t_Msw.u_Cmd.bit.ChgCmd  = false;
        t_Msw.u_Cmd.bit.DcgCmd  = false;

        i16PreSYSMode           = i16NowSYSMode;
        i16NowSYSMode           = cBootMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eMsw_ClrFault))
    {
        if(sFault_GetFaultCode_All() != 0)
        {
            sFault_ClrAllFault();

            t_Msw.t_Cnt.u16FaultModeRestCnt = 0;
        }
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        if(sGetTime_10ms() == false)
        {
            return;
        }

        if( sFault_GetFaultCode_All() == 0)
        {
            t_Msw.t_Cnt.u16FaultModeRestCnt++;
            if(t_Msw.t_Cnt.u16FaultModeRestCnt >= 200)
            {
                t_Msw.t_Cnt.u16FaultModeRestCnt = 0;

                i16PreSYSMode = i16NowSYSMode;
                i16NowSYSMode = cStandByMode;
                return;
            }
        }
        else
        {
            t_Msw.t_Cnt.u16FaultModeRestCnt = 0;
        }

        if( sFault_GetFaultCode_All() != 0 && sFault_GetFaultBit(eFault_TemperatureHi) == false)
        {
            t_Msw.t_Cnt.u16FaultModeAutoCnt++;
            if(t_Msw.t_Cnt.u16FaultModeAutoCnt >= 2800 )
            {
                t_Msw.t_Cnt.u16FaultModeAutoCnt = 0;

                sFault_ClrAllFault();
            }
            else if(t_Msw.t_Cnt.u16FaultModeAutoCnt == 800)
            {
                sFault_ClrFault(eFault_BatVoltLo);
            }
        }
        else
        {
            t_Msw.t_Cnt.u16FaultModeAutoCnt = 0;
        }
        
    }
}

void    sTestMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}


void    sMsw_SysOff(void)
{
    OS_ENTER_CRITICAL();

    sInv_SetInvEN(false);
    sLLC_SetChgEN(false);
    sLLC_SetDcgEN(false);

    sInv_SetInvOpenLoop(false);

    // sRly_Break(Soft);
    // sRly_Break(Grid);
    // sRly_Break(EV);

    if( t_Msw.u_Cmd.bit.Fast_Inv == false && t_Msw.u_Cmd.bit.Fast_Pfc == false )
    {
        // sRly_Break(Out);
    }

    sInv_SetInvRef(0);
    sInv_SetPfcRef(0);

    t_Msw.PfcModeEn = false;

    i16SYSStauts = SYS_Off;

    t_Msw.t_MswFlag.InvSoftOk = false;
    t_Msw.t_MswFlag.PfcSoftOk = false;
    t_Msw.t_MswFlag.LLCSoftOk = false;
    t_Msw.t_MswFlag.RltSoftOk = false;

    t_Msw.t_MswFlag.InvSoftStart = false;
    t_Msw.t_MswFlag.PfcSoftStart = false;
    t_Msw.t_MswFlag.LLCSoftStart = false;
    t_Msw.t_MswFlag.RlySoftStart = false;

    if(i16NowSYSMode == cInvMode)
    {
        t_Msw.u_Cmd.bit.EvCmd = false;
    }

    OS_EXIT_CRITICAL();
}

void    sMsw_IgbtOff(void)
{
    OS_ENTER_CRITICAL();

    sInv_SetInvEN(false);
    sLLC_SetChgEN(false);
    sLLC_SetDcgEN(false);

    sInv_SetInvOpenLoop(false);

    sInv_SetInvRef(0);
    sInv_SetPfcRef(0);

    i16SYSStauts = SYS_Off;

    t_Msw.t_MswFlag.InvSoftOk = false;
    t_Msw.t_MswFlag.PfcSoftOk = false;
    t_Msw.t_MswFlag.LLCSoftOk = false;
    t_Msw.t_MswFlag.RltSoftOk = false;

    t_Msw.t_MswFlag.InvSoftStart = false;
    t_Msw.t_MswFlag.PfcSoftStart = false;
    t_Msw.t_MswFlag.LLCSoftStart = false;
    t_Msw.t_MswFlag.RlySoftStart = false;


    t_Msw.u_Cmd.bit.EvCmd = false;

    OS_EXIT_CRITICAL();
}

void    sMsw_InvOff(void)
{
    // sRly_Break(Out);
    // sRly_Break(EV);
}

void    sMsw_SysOff_ISR(void)
{
    sInv_SetInvEN(false);
    sLLC_SetChgEN(false);
    sLLC_SetDcgEN(false);

    sInv_SetInvOpenLoop(false);

    sInv_SetInvRef(0);
    sInv_SetPfcRef(0);

    i16SYSStauts = SYS_Off;

    t_Msw.t_MswFlag.InvSoftOk = false;
    t_Msw.t_MswFlag.PfcSoftOk = false;
    t_Msw.t_MswFlag.LLCSoftOk = false;
    t_Msw.t_MswFlag.RltSoftOk = false;

    t_Msw.t_MswFlag.InvSoftStart = false;
    t_Msw.t_MswFlag.PfcSoftStart = false;
    t_Msw.t_MswFlag.LLCSoftStart = false;
    t_Msw.t_MswFlag.RlySoftStart = false;

    t_Msw.u_Cmd.bit.EvCmd = false;
}



unsigned int    sMsw_GetNowMode(void)
{
    return i16NowSYSMode;
}

unsigned int    sMsw_GetPreMode(void)
{
    return i16PreSYSMode;
}

unsigned int    sMsw_GetStatus(void)
{
    return i16SYSStauts;
}

unsigned int    sMsw_GetPfcMode(void)
{
    return t_Msw.PfcModeEn;
}

unsigned int    sMsw_GetPfc2Grid(void)
{
    return t_Msw.u_Cmd.bit.Pfc2GirdCmd;
}
