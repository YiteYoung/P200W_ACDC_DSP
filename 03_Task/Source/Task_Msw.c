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

void                sFast_Inv           (void);
void                sFast_Pfc           (void);

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
        t_Msw.u_Cmd.bit.InvCmd  = false;
        t_Msw.u_Cmd.bit.ChgCmd  = false;
        t_Msw.u_Cmd.bit.DcgCmd  = false;
        t_Msw.u_Cmd.bit.EvCmd   = false;

        t_Msw.u_Cmd.bit.Fast_Inv = false;
        t_Msw.u_Cmd.bit.Fast_Pfc = false;

        t_Msw.t_Cnt.u16StandByCnt = 0;
        return;
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

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {
        if(sBypass_OnChk() == true)
        {
            t_Msw.u_Cmd.bit.InvCmd  = true;
            t_Msw.u_Cmd.bit.EvCmd   = false;
            t_Msw.u_Cmd.bit.DcgCmd  = false;
        }
        else  if( sInv_OnChk() == true )
        {
            t_Msw.u_Cmd.bit.InvCmd  = true;
            t_Msw.u_Cmd.bit.EvCmd   = false;

            t_Msw.u_Cmd.bit.DcgCmd  = true;
            t_Msw.u_Cmd.bit.ChgCmd  = false;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_EvOn))
    {
        if(sEv_OnChk() == true)
        {
            t_Msw.u_Cmd.bit.EvCmd = true;
            t_Msw.u_Cmd.bit.InvCmd = true;
            t_Msw.u_Cmd.bit.ChgCmd = false;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOff))
    {
        t_Msw.u_Cmd.bit.InvCmd  = false;
        t_Msw.u_Cmd.bit.DcgCmd  = false;
        t_Msw.u_Cmd.bit.EvCmd   = false;
    }

    if( event & ((TASK_EVENT)1 << eMsw_ChgOn))
    {
        if(sPfc_OnChk() == true && t_Msw.u_Cmd.bit.EvCmd == false)
        {
            t_Msw.u_Cmd.bit.ChgCmd = true;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_ChgOff))
    {
        t_Msw.u_Cmd.bit.ChgCmd = false;
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        if(sGetTime_10ms() == false)
        {
            return;
        }

        t_Msw.t_Cnt.u16StandByCnt++;
        if(t_Msw.t_Cnt.u16StandByCnt >= 100)
        {
            t_Msw.t_Cnt.u16StandByCnt = 100;
        }
        else
        {
            return;
        }

        if( sFault_GetFaultCode_All() != 0)
        {
            t_Msw.t_Cnt.u16StandByCnt = 0;

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cFaultMode;
            return;
        }

        if(t_Msw.u_Cmd.bit.ChgCmd == true)
        {
            t_Msw.t_Cnt.u16StandByCnt = 0;

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cPfcMode;
            return;
        }

        if(t_Msw.u_Cmd.bit.InvCmd == true)
        {
            t_Msw.t_Cnt.u16StandByCnt = 0;

            if( sBypass_OnChk() == true && t_Msw.u_Cmd.bit.EvCmd == false)
            {
                i16PreSYSMode = i16NowSYSMode;
                i16NowSYSMode = cPfcMode;
                return;
            }

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cInvMode;
            return;
        }
    }
}

void    sInvMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {
        t_Msw.PfcModeEn = false;

        t_Msw.t_Cnt.u16InvModeCnt       = 0;

        t_Msw.t_Cnt.u16EvRlyWaitCnt     = 0;
        t_Msw.t_Cnt.u16OutRlyWaitCnt    = 0;
        t_Msw.t_Cnt.u16SoftRlyWaitCnt   = 0;

        t_Msw.t_Cnt.u16InvSoftErrCnt    = 0;

        t_Msw.t_Cnt.u16LLCSoftErrCnt    = 0;
        t_Msw.t_Cnt.u16LLCSoftOkChkCnt  = 0;

        t_Msw.t_MswFlag.InvRefOk        = 0;
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

    if( event & ((TASK_EVENT)1 << eMsw_Fault))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd = false;

        i16PreSYSMode = i16NowSYSMode;
        i16NowSYSMode = cFaultMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eMsw_Alarm))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd = false;

        i16PreSYSMode = i16NowSYSMode;
        i16NowSYSMode = cStandByMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOff))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd = false;

        i16PreSYSMode = i16NowSYSMode;
        i16NowSYSMode = cStandByMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eMsw_ChgOn))
    {
        if(sBypass_OnChk() == true && t_Msw.u_Cmd.bit.EvCmd == false)
        {
            t_Msw.u_Cmd.bit.Fast_Pfc = true;
            t_Msw.u_Cmd.bit.ChgCmd = true;

            sMsw_SysOff();

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cPfcMode;
            return;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_GridRest))
    {
        if(sBypass_OnChk() == true && t_Msw.u_Cmd.bit.EvCmd == false)
        {
            t_Msw.u_Cmd.bit.Fast_Pfc = true;

            sMsw_SysOff();

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cPfcMode;
            return;
        }
    }

    if(t_Msw.t_MswFlag.InvSoftOk == true && sFault_GetAlarmBit(eAlarm_BusLow) == true)
    {
        sMsw_SysOff();
        sFault_SetFault(eFault_BusLow);

        t_Msw.u_Cmd.bit.DcgCmd = false;

        i16PreSYSMode = i16NowSYSMode;
        i16NowSYSMode = cFaultMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        t_Msw.t_Cnt.u16InvModeCnt++;
        if(t_Msw.t_Cnt.u16InvModeCnt >= 3)
        {
            t_Msw.t_Cnt.u16InvModeCnt = 3;
        }
        else 
        {
            return;
        }

        if(sBypass_OnChk() == true && t_Msw.u_Cmd.bit.EvCmd == false)
        {
            t_Msw.u_Cmd.bit.Fast_Pfc = true;

            sMsw_SysOff();

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cPfcMode;
            return;
        }

        if( i16SYSStauts == Inv_OK)
        {
            if(sGetTime_10ms() == true)
            {
                // sInv_InvVoltSoft(sConfig_GetInvSet(),cVdc1V);
            }

            if( t_Msw.u_Cmd.bit.EvCmd == true)
            {
                sEvOutRly();
                return;
            }

            if(t_Msw.u_Cmd.bit.InvCmd == true)
            {
                sInvOutRly();
            }
        }
        else
        {
            if( t_Msw.t_MswFlag.LLCSoftOk == false && t_Msw.u_Cmd.bit.DcgCmd == true)
            {
                sDcgSoft();
                return;
            }

            if(t_Msw.t_MswFlag.InvSoftOk == false && t_Msw.u_Cmd.bit.InvCmd == true)
            {
                sInvSoft();
                return;
            }
        }
    }
}

void    sPfcMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {
        t_Msw.PfcModeEn = true;

        t_Msw.t_Cnt.u16PfcModeCnt = 0;
        t_Msw.t_Cnt.u16PfcSoftOkChkCnt = 0;
        t_Msw.t_Cnt.u16PfcSoftErrCnt = 0;

        t_Msw.t_Cnt.u16GridRlyWaitCnt = 0;
        t_Msw.t_Cnt.u16SoftRlyWaitCnt = 0;

        t_Msw.t_Cnt.u16LLCSoftErrCnt = 0;

        t_Msw.t_MswFlag.PfcRefOk = false;
        t_Msw.u_Cmd.bit.Fast_Inv = false;

        // sRly_Break(EV);

        t_Msw.u_Cmd.bit.EvCmd = false;
        t_Msw.u_Cmd.bit.DcgCmd = false;
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

    if( event & ((TASK_EVENT)1 << eMsw_Fault))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd  = false;
        t_Msw.u_Cmd.bit.ChgCmd  = false;
        t_Msw.u_Cmd.bit.DcgCmd  = false;

        i16PreSYSMode = i16NowSYSMode;
        i16NowSYSMode = cFaultMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eMsw_Alarm))
    {
        sMsw_SysOff();

        t_Msw.u_Cmd.bit.InvCmd  = false;
        t_Msw.u_Cmd.bit.ChgCmd  = false;
        t_Msw.u_Cmd.bit.DcgCmd  = false;

        i16PreSYSMode = i16NowSYSMode;
        i16NowSYSMode = cStandByMode;
        return;
    }

    if( event & ((TASK_EVENT)1 << eMsw_EvOn))
    {
        if(sEv_OnChk() == true)
        {
            t_Msw.u_Cmd.bit.Fast_Inv = true;
            t_Msw.u_Cmd.bit.EvCmd = true;
            t_Msw.u_Cmd.bit.InvCmd = true;
            t_Msw.u_Cmd.bit.ChgCmd = false;

            sMsw_SysOff();
            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cInvMode;
            return;
        }
    }


    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {
        if(sBypass_OnChk() == true)
        {
            t_Msw.u_Cmd.bit.InvCmd = true;
        }
        else
        {
            t_Msw.u_Cmd.bit.Fast_Inv = true;

            t_Msw.u_Cmd.bit.ChgCmd = false;
            t_Msw.u_Cmd.bit.InvCmd = true;
            t_Msw.u_Cmd.bit.EvCmd   = false;

            sMsw_SysOff();
            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cInvMode;
            return;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_GridLost))
    {
        if(t_Msw.u_Cmd.bit.InvCmd == true && sInv_OnChk() == true)
        {
            t_Msw.u_Cmd.bit.Fast_Inv = true;

            t_Msw.u_Cmd.bit.ChgCmd = false;
            t_Msw.u_Cmd.bit.EvCmd   = false;

            sMsw_SysOff();
            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cInvMode;
            return;
        }
        else
        {
            sMsw_SysOff();
            t_Msw.u_Cmd.bit.ChgCmd = false;

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cStandByMode;
            return;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_ChgOff))
    {
        t_Msw.u_Cmd.bit.ChgCmd = false;

        if(t_Msw.u_Cmd.bit.InvCmd == true)
        {
            sMsw_IgbtOff();

            t_Msw.t_Cnt.u16PfcSoftOkChkCnt  = 0;
            t_Msw.t_Cnt.u16PfcSoftErrCnt    = 0;

            t_Msw.t_Cnt.u16GridRlyWaitCnt = 0;
            t_Msw.t_Cnt.u16SoftRlyWaitCnt = 0;

            t_Msw.t_MswFlag.PfcRefOk = false;
        }
        else
        {
            sMsw_SysOff();

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cStandByMode;
            return;
        }
    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOff))
    {
        if(t_Msw.u_Cmd.bit.ChgCmd == true)
        {
            t_Msw.u_Cmd.bit.InvCmd = false;
        }
        else
        {
            sMsw_SysOff();

            t_Msw.u_Cmd.bit.ChgCmd = false;
            t_Msw.u_Cmd.bit.InvCmd = false;

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cStandByMode;
            return;
        }
    }

    if(t_Msw.t_MswFlag.PfcSoftOk == true && sFault_GetAlarmBit(eAlarm_BusLow) == true)
    {
        if(t_Msw.u_Cmd.bit.InvCmd == true)
        {
            t_Msw.u_Cmd.bit.ChgCmd = false;

            t_Msw.t_Cnt.u16PfcSoftErrCnt    = 0;
            t_Msw.t_Cnt.u16PfcSoftOkChkCnt  = 0;

            t_Msw.t_Cnt.u16GridRlyWaitCnt   = 0;
            t_Msw.t_Cnt.u16SoftRlyWaitCnt   = 0;

            t_Msw.t_MswFlag.PfcRefOk        = false;

        }
        else
        {
            sMsw_SysOff();

            t_Msw.u_Cmd.bit.ChgCmd = false;
            t_Msw.u_Cmd.bit.InvCmd = false;

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cStandByMode;
            return;
        }
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        t_Msw.t_Cnt.u16PfcModeCnt++;
        if(t_Msw.t_Cnt.u16PfcModeCnt >= 3)
        {
            t_Msw.t_Cnt.u16PfcModeCnt = 3;
        }
        else
        {
            return;
        }

        sByPass();

        sRlySoft();

        // sPfc2Grid_Check(800);

        if(t_Msw.u_Cmd.bit.InvCmd == false && t_Msw.u_Cmd.bit.ChgCmd == false)
        {
            sMsw_SysOff();

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cStandByMode;
            return;
        }

        if(i16SYSStauts == Pfc_OK)
        {
            if(sGetTime_4ms() == true)
            {
                // sConfig_PfcVotCal(sSample_GetRms(BatVolt),sSample_GetRms(GridVolt),sSample_GetRms(BatCurr));
                // sInv_PfcVoltSoft(sConfig_GetPfcSet(),cVdc5V);

                // sGrid_CurrCal(sProtect_GetChgAcPowerLimit());
                // sGrid_CurrSoft(set,cCurr1A);
                // sGrid_CurrLimit();

                // sLLC_BatVoltSoft(sConfig_GetBatVoltSet(),cVdc0V5);
                // sConfig_BatCurrCal(sSample_GetRms(BatVolt));
            }
        }
        else
        {
            if(t_Msw.t_MswFlag.PfcSoftOk == false)
            {
                sPfcSoft();
                return;
            }

            if(  t_Msw.u_Cmd.bit.ChgCmd == true && t_Msw.t_MswFlag.LLCSoftOk == false )
            {
                sChgSoft();
                return;
            }  
        }
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
        t_Msw.u_Cmd.all = 0;

        sInv_SetInvOpenLoop(true);
        sLLC_SetOpenLoop(true);

        sProtect_DisableInvTZ();
        sProtect_DisableLLcTZ();
    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {
        t_Msw.u_Cmd.bit.InvCmd = true;
    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOff))
    {
        t_Msw.u_Cmd.bit.InvCmd = false;
    }

    if( event & ((TASK_EVENT)1 << eMsw_ChgOn))
    {
        t_Msw.u_Cmd.bit.ChgCmd = true;
    }

    if( event & ((TASK_EVENT)1 << eMsw_ChgOff))
    {
        t_Msw.u_Cmd.bit.ChgCmd = false;
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        sInv_SetInvEN(t_Msw.u_Cmd.bit.InvCmd );

        if(t_Msw.u_Cmd.bit.InvCmd == false)
        {
            sLLC_SetChgEN(t_Msw.u_Cmd.bit.ChgCmd);
        }

        // To do:RLY Test
    }
}

void    sRlySoft(void)
{
    signed int i16TempA;

    if(t_Msw.t_MswFlag.RlySoftOk == true)
    {
        t_Msw.t_MswFlag.RlySoftStart = false;
        return;
    }

    if(t_Msw.t_MswFlag.RlySoftStart == false)
    {
        i16SYSStauts = RLY_Soft;
        t_Msw.t_MswFlag.RlySoftStart = true;
    }

    if(t_Msw.u_Cmd.bit.Fast_Pfc == true)
    {
        sFast_Pfc();
        return;
    }

    // if(sRly_GetRlyOn(Rly_Grid) == true && sRly_GetRlyOn(Rly_SoftR) == false)
    // {
    //     i16SYSStauts = Rly_OK;
    //     t_Msw.t_MswFlag.RlySoftStart = false;
    //     t_Msw.t_MswFlag.RlySoftOk    = true;

    //     t_Msw.t_Cnt.u16SoftRlyWaitCnt = 0;
    //     t_Msw.t_Cnt.u16GridRlyWaitCnt = 0;
    //     return;
    // }

    // if(sRly_GetRlyOn(Rly_SoftR) == false)
    // {
    //     sRly_Close(Rly_SoftR);

    //     t_Msw.t_Cnt.u16SoftRlyWaitCnt = 0;
    //     t_Msw.t_Cnt.u16GridRlyWaitCnt = 0;
    //     return;
    // }

    if(sGetTime_1ms() == false)     {return;}

    t_Msw.t_Cnt.u16GridRlyWaitCnt++;
    if(t_Msw.t_Cnt.u16GridRlyWaitCnt >= 1000)
    {
        t_Msw.t_Cnt.u16GridRlyWaitCnt = 1000;
        i16TempA = sSample_GetRms(GridVolt);
        i16TempA = (int)((float)i16TempA * 1.10f);
        if(sSample_GetRms(BusVolt) > i16TempA && sSample_GetRms(BusVolt) < cCHG_BUS_VOLT_BURST_MAX)
        {
            // if( sRly_GetRlyOn(Rly_Grid) == false)
            // {
            //     sRly_Close(Rly_Grid);
            //     t_Msw.t_Cnt.u16SoftRlyWaitCnt = 0;
            // }
        }
        else
        {
            sMsw_SysOff();

            t_Msw.u_Cmd.bit.InvCmd = false;
            t_Msw.u_Cmd.bit.ChgCmd = false;

            t_Msw.t_MswFlag.RlySoftStart = false;
            t_Msw.t_MswFlag.PfcSoftErr   = true;

            i16PreSYSMode = i16NowSYSMode;
            i16NowSYSMode = cFaultMode;
            return;
        }

        t_Msw.t_Cnt.u16SoftRlyWaitCnt++;
        if(t_Msw.t_Cnt.u16SoftRlyWaitCnt >= 30)
        {
            // sRly_Break(Rly_SoftR);

            i16SYSStauts = Rly_OK;

            t_Msw.t_MswFlag.RlySoftStart = false;
            t_Msw.t_MswFlag.RlySoftOk = true;

            t_Msw.t_Cnt.u16SoftRlyWaitCnt = 0;
            t_Msw.t_Cnt.u16GridRlyWaitCnt = 0;
        }
    }
}

void    sInvSoft(void)
{

}

void    sPfcSoft(void)
{

}

void    sChgSoft(void)
{

}

void    sDcgSoft(void)
{

}

void    sFast_Inv(void)
{

}

void    sFast_Pfc(void)
{

}

void    sByPass(void)
{

}

unsigned char    sPfc_OnChk(void)
{
    return false;
}

unsigned char    sInv_OnChk(void)
{
    return false;
}

unsigned char    sEv_OnChk(void)
{
    return false;
}

unsigned char    sBypass_OnChk(void)
{
    return false;
}


void    sInvOutRly(void)
{

}

void    sEvOutRly(void)
{

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
    t_Msw.t_MswFlag.RlySoftOk = false;

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
    t_Msw.t_MswFlag.RlySoftOk = false;

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
    t_Msw.t_MswFlag.RlySoftOk = false;

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
