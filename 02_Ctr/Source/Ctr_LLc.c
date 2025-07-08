#include "UserHeader.h"

static  LLcControl_t    t_LLcControl;

void    sLLC_LoopClear          (void);
void    sLLC_ChgSoftStart       (void);
void    sLLC_ChgControlLoop     (void);
void    sLLC_ChgBurstChk        (void);
void    sLLC_ChgPwmDeal         (void);

void    sLLC_Control(void)
{
    static unsigned int Cnt;
    
    // 区别与CLA采样计算(考虑采样时间及位置)
    // sLLC_Sample();

    if(     sProtect_GetTZFlag(TZ1) == true     ||\
            sProtect_GetTZFlag(TZ2) == true     ||\
            sProtect_GetTZFlag(Cmpss4) == true  ||\
        (   sMsw_GetChgCmd() == false && sMsw_GetDcgCmd() == false ))
    {
        t_LLcControl.u16LLC_State = eLLC_FSM_OFF;

        EALLOW;
        EPwm1Regs.TZFRC.bit.OST     = 1;
        EPwm2Regs.TZFRC.bit.OST     = 1;
        EPwm3Regs.TZFRC.bit.OST     = 1;
        EPwm4Regs.TZFRC.bit.OST     = 1;
        EDIS;

        sLLC_LoopClear();
    }
    else if (sMsw_GetChgCmd() == true) 
    {
        if( t_LLcControl.u16LLC_State == eLLC_FSM_OFF )
        {
            sLLC_SetPwmChgMode();

            t_LLcControl.f32PsLimit = 0.6f;     // 20% duty
            t_LLcControl.f32PsLimitStep = 0.00004f;

            t_LLcControl.u16LLC_State = eLLC_FSM_SOFT;
        }
        else
        {
            t_LLcControl.u16PwmDelayCnt++;
            if(t_LLcControl.u16PwmDelayCnt >= 2)
            {
                t_LLcControl.u16PwmDelayCnt = 0;
                t_LLcControl.u16PwmEn = true;
            }

            sLLC_ChgSoftStart();
            sLLC_ChgControlLoop();
            sLLC_ChgBurstChk();
            sLLC_ChgPwmDeal();
        }
    } else if(sMsw_GetDcgCmd() == true) 
    {
        if( t_LLcControl.u16LLC_State == eLLC_FSM_OFF )
        {
            sLLC_SetPwmDcgMode();

            t_LLcControl.f32PsLimit = 0.98f;     // 20% duty
            t_LLcControl.f32PsLimitStep = 0.00001f;

            if( sSample_GetRms(BusVolt) >= cVdc200V)
            {
                t_LLcControl.u16FastSoftFlag = true;
                t_LLcControl.f32BusVoltStep  = cVdc5V;

                t_LLcControl.u16LLC_Prd      = cPWM_PRD_80K;

                t_LLcControl.f32DcgCurrLimit = 200.0f;
                t_LLcControl.f32DcgPiOutLimitUp = (float)t_LLcControl.u16LLC_Prd * 0.55f;
                t_LLcControl.f32DcgPiOutLimitDn = (float)t_LLcControl.u16LLC_Prd * 0.55f;
            }
            else
            {
                t_LLcControl.u16FastSoftFlag = false;
                t_LLcControl.f32BusVoltStep  = cVdc1V;

                t_LLcControl.u16LLC_Prd      = cPWM_PRD_80K;

                t_LLcControl.f32DcgCurrLimit = 200.0f;
                t_LLcControl.f32DcgPiOutLimitUp = (float)t_LLcControl.u16LLC_Prd;
                t_LLcControl.f32DcgPiOutLimitDn = 0.0f;
            }

            t_LLcControl.u16LLC_State = eLLC_FSM_SOFT;
        }
        else
        {
            t_LLcControl.u16PwmDelayCnt++;
            if(t_LLcControl.u16PwmDelayCnt >= 2)
            {
                t_LLcControl.u16PwmDelayCnt = 0;
                t_LLcControl.u16PwmEn = true;
            }

            sLLC_DcgSoftStart();
            sLLC_DcgControlLoop();
            sLLC_DcgBurstChk();
            sLLC_DcgPwmDeal();
        }
    }
    else
    {
        t_LLcControl.u16LLC_State = eLLC_FSM_OFF;

        EALLOW;
        EPwm1Regs.TZFRC.bit.OST     = 1;
        EPwm2Regs.TZFRC.bit.OST     = 1;
        EPwm3Regs.TZFRC.bit.OST     = 1;
        EPwm4Regs.TZFRC.bit.OST     = 1;
        EDIS;

        sLLC_LoopClear();
    }
}

void sLLC_LoopClear(void)
{
    t_LLcControl.f32BatCurrRef = cCurr1A;

    t_LLcControl.t_ChgVoltLoop.f32Out   = 0.0f;
    t_LLcControl.t_ChgVoltLoop.f32Out1  = 0.0f;
    t_LLcControl.t_ChgVoltLoop.f32Out2  = 0.0f;
    t_LLcControl.t_ChgVoltLoop.f32Err1  = 0.0f;
    t_LLcControl.t_ChgVoltLoop.f32Err2  = 0.0f;

    t_LLcControl.t_ChgCurrLoop.f32Out   = 0.0f;
    t_LLcControl.t_ChgCurrLoop.f32Out1  = 0.0f;
    t_LLcControl.t_ChgCurrLoop.f32Out2  = 0.0f;
    t_LLcControl.t_ChgCurrLoop.f32Err1  = 0.0f;
    t_LLcControl.t_ChgCurrLoop.f32Err2  = 0.0f;

    t_LLcControl.f32ChgPiOutLimitUp     = 0.0f;
    t_LLcControl.f32ChgPiOutLimitDn     = 0.0f;
    
    t_LLcControl.f32BusVoltSet          = cVdc200V;
    t_LLcControl.f32BusVoltRef          = (float)sAdc_GetReal(BusVolt);

    t_LLcControl.t_DcgVoltLoop.f32Ref   = t_LLcControl.f32BusVoltRef;
    t_LLcControl.t_DcgVoltLoop.f32Fed   = (float)sSample_GetRms(BusVolt);

    t_LLcControl.t_DcgVoltLoop.f32Out   = 0.0f;
    t_LLcControl.t_DcgVoltLoop.f32Out1  = 0.0f;
    t_LLcControl.t_DcgVoltLoop.f32Out2  = 0.0f;
    t_LLcControl.t_DcgVoltLoop.f32Err1  = 0.0f;
    t_LLcControl.t_DcgVoltLoop.f32Err2  = 0.0f;

    t_LLcControl.t_DcgCurrLoop.f32Ref   = 0.0f;

    t_LLcControl.t_DcgCurrLoop.f32Out   = 0.0f;
    t_LLcControl.t_DcgCurrLoop.f32Out1  = 0.0f;
    t_LLcControl.t_DcgCurrLoop.f32Out2  = 0.0f;
    t_LLcControl.t_DcgCurrLoop.f32Err1  = 0.0f;
    t_LLcControl.t_DcgCurrLoop.f32Err2  = 0.0f;

    t_LLcControl.f32ILoopK2_Ref         = 1.547f;
    t_LLcControl.f32ILoopK3_Ref         = 1.453f;

    t_LLcControl.u16LLC_Prd             = cPWM_PRD_80K;
    t_LLcControl.f32DcgCurrLimit        = 200.0f;
    t_LLcControl.f32DcgPiOutLimitUp     = (float)t_LLcControl.u16LLC_Prd;
    t_LLcControl.f32DcgPiOutLimitDn     = 0.0f;

    t_LLcControl.u16FastSoftFlag        = false;
    t_LLcControl.u16FastStartCnt        = 0;
    t_LLcControl.f32BusVoltStep         = 0.1f;

    t_LLcControl.t_ChgBurst.u16EN       = false;
    t_LLcControl.t_ChgBurst.u16Act      = false;
    t_LLcControl.t_ChgBurst.u16Cnt      = 0;

    t_LLcControl.t_DcgBurst.u16EN       = false;
    t_LLcControl.t_DcgBurst.u16Act      = false;
    t_LLcControl.t_DcgBurst.u16Cnt      = 0;

    t_LLcControl.u16PwmEn               = false;
    t_LLcControl.u16PwmDelayCnt         = 0;
}

void    sLLC_ChgSoftStart(void)
{
    float f32TempA;

    f32TempA = sConfig_GetBatCurrGiv();

    // 充电电流给定软启动
    if( t_LLcControl.f32BatCurrRef < f32TempA - 0.01f )
    { 
        t_LLcControl.f32BatCurrRef += 0.006f;
    }
    else if( t_LLcControl.f32BatCurrRef > f32TempA + 0.01f )
    {
        t_LLcControl.f32BatCurrRef -= 0.006f;
    }
    else
    {
        t_LLcControl.f32BatCurrRef = f32TempA;
    }

    t_LLcControl.f32ChgPiOutLimitUp += 0.05f;

    if( t_LLcControl.f32ChgPiOutLimitUp >= (float)cPWM_PRD_50K)
    {
        t_LLcControl.f32ChgPiOutLimitUp = (float)cPWM_PRD_50K;
    }

    if(t_LLcControl.u16LLC_State == eLLC_FSM_SOFT)
    {
        if(sSample_GetRms(BatVolt) >= cVdc40V && t_LLcControl.f32ChgPiOutLimitUp >= (float)cPWM_PRD_70K * 0.95f)
        {
            t_LLcControl.u16LLC_State = eLLC_FSM_RUN;
        }
    }

    // 并网部分
    // To Be Continute
}

void    sLLC_ChgControlLoop(void)
{
    float f32TempA,f32TempB;
    // 并网部分
    // To Be Continute
    // if( )
    // {

    // }
    // else
    {
        // 获取BMS的电压，从而进入CV环
        f32TempB= 0.0f;    // sGetBmsChgLimitOut();

        f32TempA = sConfig_GetBatVoltSet();
        t_LLcControl.t_ChgVoltLoop.f32Ref = f32TempB + f32TempB;
        UpDnLimit(t_LLcControl.t_ChgVoltLoop.f32Ref, 0.0f, cVdc60V);

        t_LLcControl.t_ChgVoltLoop.f32Fed = sAdc_GetReal(BatVolt);
        t_LLcControl.t_ChgVoltLoop.f32Err1 = t_LLcControl.t_ChgVoltLoop.f32Err0;
        t_LLcControl.t_ChgVoltLoop.f32Err0 = t_LLcControl.t_ChgVoltLoop.f32Ref - t_LLcControl.t_ChgVoltLoop.f32Fed;
        UpDnLimit(t_LLcControl.t_ChgVoltLoop.f32Err0, (float)-cVdc10V, (float)cVdc10V);
        t_LLcControl.t_ChgVoltLoop.f32Out1 = t_LLcControl.t_ChgVoltLoop.f32Out;

        t_LLcControl.t_ChgVoltLoop.f32Out   =     t_LLcControl.t_ChgVoltLoop.f32Out1                \
                                                + 2.062831853f * t_LLcControl.t_ChgVoltLoop.f32Err0 \
                                                - 1.937168147f * t_LLcControl.t_ChgVoltLoop.f32Err1 ;
        UpDnLimit(t_LLcControl.t_ChgVoltLoop.f32Out, (float)-cCurr10A, t_LLcControl.f32BatCurrRef);

        t_LLcControl.t_ChgCurrLoop.f32Ref = t_LLcControl.t_ChgVoltLoop.f32Out;
        t_LLcControl.t_ChgCurrLoop.f32Fed = sAdc_GetReal(BatCurr);
        t_LLcControl.t_ChgCurrLoop.f32Err2 = t_LLcControl.t_ChgCurrLoop.f32Err1;
        t_LLcControl.t_ChgCurrLoop.f32Err1 = t_LLcControl.t_ChgCurrLoop.f32Err0;
        t_LLcControl.t_ChgCurrLoop.f32Err0 = t_LLcControl.t_ChgCurrLoop.f32Ref - t_LLcControl.t_ChgCurrLoop.f32Fed;
        UpDnLimit(t_LLcControl.t_ChgCurrLoop.f32Err0, (float)-cCurr10A, (float)cCurr10A);
        t_LLcControl.t_ChgCurrLoop.f32Out2 = t_LLcControl.t_ChgCurrLoop.f32Out1;
        t_LLcControl.t_ChgCurrLoop.f32Out1 = t_LLcControl.t_ChgCurrLoop.f32Out;

        t_LLcControl.t_ChgCurrLoop.f32Out   =     t_LLcControl.t_ChgCurrLoop.f32Out1                \
                                                + 1.523561945f * t_LLcControl.t_ChgCurrLoop.f32Err0 \
                                                - 1.476438055f * t_LLcControl.t_ChgCurrLoop.f32Err1 ;
        UpDnLimit(t_LLcControl.t_ChgCurrLoop.f32Out, t_LLcControl.f32ChgPiOutLimitDn, t_LLcControl.f32ChgPiOutLimitUp);

        if( t_LLcControl.t_ChgBurst.u16EN == true)
        {
            t_LLcControl.t_ChgVoltLoop.f32Err1  = 0.0f;
            t_LLcControl.t_ChgVoltLoop.f32Out   = 0.0f;
            t_LLcControl.t_ChgCurrLoop.f32Err1  = 0.0f;
            t_LLcControl.t_ChgCurrLoop.f32Err2  = 0.0f;
            t_LLcControl.t_ChgCurrLoop.f32Out   = 0.0f;
            t_LLcControl.t_ChgCurrLoop.f32Out1  = 0.0f;
            t_LLcControl.t_ChgCurrLoop.f32Out2  = 0.0f;
        }
    }
}

void    sLLC_ChgBurstChk(void)
{
    // To do:并网不开Burst

    if( t_LLcControl.t_ChgBurst.u16Act == false )
    {
        if( sAdc_GetReal(BatVolt) > sConfig_GetBatVoltSet() + cVdc3V    ||\
            sAdc_GetReal(BatVolt) > cCHG_BAT_VOLT_MAX - cVdc0V5         ) 
        {
            t_LLcControl.t_ChgBurst.u16Cnt++;
            if( t_LLcControl.t_ChgBurst.u16Cnt >= 2)
            {
                t_LLcControl.t_ChgBurst.u16Act = true;
                t_LLcControl.t_ChgBurst.u16Cnt = 0;
            }
        }
        else 
        {
            t_LLcControl.t_ChgBurst.u16Cnt = 0;
        }
    }
    else
    {
        if( sAdc_GetReal(BatVolt) <= sConfig_GetBatVoltSet() ) 
        {
            t_LLcControl.t_ChgBurst.u16Cnt++;
            if( t_LLcControl.t_ChgBurst.u16Cnt >= 2)
            {
                t_LLcControl.t_ChgBurst.u16Act = false;
                t_LLcControl.t_ChgBurst.u16Cnt = 0;
            }
        }
        else 
        {
            t_LLcControl.t_ChgBurst.u16Cnt = 0;
        }
    }

    if(t_LLcControl.t_ChgBurst.u16EN == true)
    {
        if(t_LLcControl.t_ChgCurrLoop.f32Out < (float) cPWM_PRD_100K || t_LLcControl.t_ChgBurst.u16Act == true)
        {
            t_LLcControl.t_ChgBurst.u16EN = false;

            EALLOW;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            EPwm4Regs.TZFRC.bit.OST = 1;
            EDIS;
        }
    }
    else 
    {
        if( t_LLcControl.t_ChgCurrLoop.f32Out > (float)cPWM_PRD_100K * 0.12f    &&\
            t_LLcControl.t_ChgBurst.u16Act == false                             &&\
            t_LLcControl.u16PwmEn == true                                       )
        {
            t_LLcControl.t_ChgBurst.u16EN = true;

            EALLOW;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZCLR.bit.OST = 1;
            EPwm4Regs.TZCLR.bit.OST = 1;
            EDIS;
        }
    }
}

void    sLLC_ChgPwmDeal(void)
{
    unsigned int    u16Temp;
    unsigned int    riseEdgeDly;
    unsigned int    fallDegePre;
    unsigned int    PsLimit;

    riseEdgeDly = 72;
    fallDegePre = 1;

    // To do :并网发波
    // if( OnGrid )
    // {

    // }
    // else
    {
        t_LLcControl.u16LLC_Prd = (unsigned int)t_LLcControl.t_ChgCurrLoop.f32Out;

        if(t_LLcControl.u16LLC_Prd < cPWM_PRD_100K)
        {
            t_LLcControl.u16LLC_Ps = cPWM_PRD_100K - t_LLcControl.u16LLC_Prd;
        }
        else
        {
            t_LLcControl.u16LLC_Ps = 0;
        }

        if( t_LLcControl.u16OpenLoopEn == true )
        {
            t_LLcControl.u16LLC_Prd = cPWM_PRD_80K;
            t_LLcControl.u16LLC_Ps  = 0;

            EALLOW;
            EPwm1Regs.TZCLR.bit.OST = 1;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm3Regs.TZCLR.bit.OST = 1;
            EPwm4Regs.TZCLR.bit.OST = 1;
            EDIS;
        }

        PsLimit = (unsigned int)(0.6f * (float)cPWM_PRD_100K);
        UpDnLimit(t_LLcControl.u16LLC_Ps, 0, PsLimit);

        UpDnLimit(t_LLcControl.u16LLC_Prd, cPWM_PRD_100K, cPWM_PRD_50K);
    }

    EPwm1Regs.TBPRD = t_LLcControl.u16LLC_Prd;
    EPwm3Regs.CMPB.bit.CMPB = 1;

    EPwm4Regs.CMPA.bit.CMPA = t_LLcControl.u16LLC_Ps;
    EPwm4Regs.CMPB.bit.CMPB = t_LLcControl.u16LLC_Prd - t_LLcControl.u16LLC_Ps;

    EPwm1Regs.CMPA.bit.CMPA = t_LLcControl.u16LLC_Prd - fallDegePre;
    EPwm1Regs.CMPB.bit.CMPB = fallDegePre;

    EPwm1Regs.DBFED.all     = t_LLcControl.u16LLC_Ps + riseEdgeDly;
    EPwm1Regs.DBRED.all     = t_LLcControl.u16LLC_Ps + riseEdgeDly;

    EPwm2Regs.CMPA.bit.CMPA = t_LLcControl.u16LLC_Prd - fallDegePre;
    EPwm2Regs.CMPB.bit.CMPB = fallDegePre;

    EPwm2Regs.DBFED.all     = t_LLcControl.u16LLC_Ps + riseEdgeDly;
    EPwm2Regs.DBRED.all     = t_LLcControl.u16LLC_Ps + riseEdgeDly;    
}


