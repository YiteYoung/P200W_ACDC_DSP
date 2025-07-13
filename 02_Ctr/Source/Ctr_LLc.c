#include "UserHeader.h"

static  LLcControl_t    t_LLcControl;

void    sLLC_LoopClear          (void);

void    sLLC_ChgSoftStart       (void);
void    sLLC_ChgControlLoop     (void);
void    sLLC_ChgBurstChk        (void);
void    sLLC_ChgPwmDeal         (void);

void    sLLC_DcgSoftStart       (void);
void    sLLC_DcgControlLoop     (void);
void    sLLC_DcgBurstChk        (void);
void    sLLC_DcgPwmDeal         (void);

void    sLLC_SetPwmChgMode      (void);
void    sLLC_SetPwmDcgMode      (void);

void    sLLC_Control(void)
{
    // static unsigned int Cnt;
    
    // 区别与CLA采样计算(考虑采样时间及位置)
    // sLLC_Sample();

    if(     sProtect_GetTZFlag(TZ1) == true     ||\
            sProtect_GetTZFlag(TZ2) == true     ||\
            sProtect_GetTZFlag(Cmpss4) == true  ||\
        (   t_LLcControl.ChgEN == false && t_LLcControl.DcgEN== false ))
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
    else if (t_LLcControl.ChgEN == true) 
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
    } 
    else if(t_LLcControl.DcgEN == true) 
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

    f32TempA = sConfig_GetBatCurrSet();

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
    float f32TempA;
    float f32TempB;
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
        t_LLcControl.t_ChgVoltLoop.f32Ref = f32TempA + f32TempB;
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
    // unsigned int    u16Temp;
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

void    sLLC_DcgSoftStart(void)
{
    float f32TempA,f32TempB;
    f32TempA = 0.55f * sAdc_GetReal(BatCurr) - (float)cVdc5V;
    f32TempB = sConfig_GetLLCTrancRaio() * sSample_GetRms(BatVolt) + f32TempA;
    UpDnLimit(f32TempB, cVdc320V, cVdc490V);

    t_LLcControl.f32BusVoltSet = f32TempB;

    t_LLcControl.u16FastStartCnt++;
    if( t_LLcControl.u16FastStartCnt >= 1600)
    {
        t_LLcControl.u16FastStartCnt = 1600;
    }

    if(t_LLcControl.u16FastSoftFlag == true)
    {
        if(t_LLcControl.u16FastStartCnt <= 640)
        {
            t_LLcControl.f32DcgPiOutLimitUp += t_LLcControl.u16LLC_Prd * 0.001f;
            t_LLcControl.f32DcgPiOutLimitUp -= t_LLcControl.u16LLC_Prd * 0.001f;
            UpLimit(t_LLcControl.f32DcgPiOutLimitUp, cPWM_PRD_80K);
            DnLimit(t_LLcControl.f32DcgPiOutLimitUp, 0.0f);
        }
        else if(t_LLcControl.u16LLC_Prd < cPWM_PRD_80K) 
        {
            t_LLcControl.u16LLC_Prd += 1;
            UpLimit(t_LLcControl.u16LLC_Prd, cPWM_PRD_80K);
        }
        else
        {
            t_LLcControl.f32DcgPiOutLimitUp += t_LLcControl.u16LLC_Prd * 0.01f;
            t_LLcControl.f32DcgPiOutLimitUp -= t_LLcControl.u16LLC_Prd * 0.001f;
            UpLimit(t_LLcControl.f32DcgPiOutLimitUp, cPWM_PRD_80K);
            DnLimit(t_LLcControl.f32DcgPiOutLimitUp, 0.0f);
        }
    }

    if(t_LLcControl.f32BusVoltRef < t_LLcControl.f32BusVoltSet - (float)cVdc0V1)
    {
        t_LLcControl.f32BusVoltRef += t_LLcControl.f32BusVoltStep;
        if( t_LLcControl.f32BusVoltRef >= t_LLcControl.f32BusVoltSet)
        {
            t_LLcControl.f32BusVoltRef = t_LLcControl.f32BusVoltSet;
        }
    }
    else if(t_LLcControl.f32BusVoltRef > t_LLcControl.f32BusVoltSet + (float)cVdc0V1)
    {
        t_LLcControl.f32BusVoltRef -= t_LLcControl.f32BusVoltStep;
        if( t_LLcControl.f32BusVoltRef <= t_LLcControl.f32BusVoltSet)
        {
            t_LLcControl.f32BusVoltRef = t_LLcControl.f32BusVoltSet;
        }
    }

    if( t_LLcControl.u16LLC_State == eLLC_FSM_SOFT )
    {
        if( t_LLcControl.f32BusVoltSet - t_LLcControl.f32BusVoltRef < (float)cVdc1V &&\
            t_LLcControl.f32BusVoltRef - sAdc_GetReal(BusVolt) < (float)cVdc5V      &&\
            t_LLcControl.f32DcgPiOutLimitUp >= (float)cPWM_PRD_80K                  )
        {
            t_LLcControl.u16LLC_State = eLLC_FSM_RUN;

            t_LLcControl.f32DcgPiOutLimitUp = (float)cPWM_PRD_80K;
            t_LLcControl.f32DcgPiOutLimitDn = 0.0f;
            t_LLcControl.f32DcgCurrLimit    = 200.0f;

            t_LLcControl.u16FastSoftFlag    = false;
            t_LLcControl.f32BusVoltStep     = 0.1f;
        }
    }
}

void    sLLC_DcgControlLoop(void)
{
    t_LLcControl.t_DcgVoltLoop.f32Ref = t_LLcControl.f32BusVoltRef;
    t_LLcControl.t_DcgVoltLoop.f32Fed = sSample_GetRms(BusVolt);
    t_LLcControl.t_DcgVoltLoop.f32Err2 = t_LLcControl.t_DcgVoltLoop.f32Err1;
    t_LLcControl.t_DcgVoltLoop.f32Err1 = t_LLcControl.t_DcgVoltLoop.f32Err0;
    t_LLcControl.t_DcgVoltLoop.f32Err0 = t_LLcControl.t_DcgVoltLoop.f32Ref - t_LLcControl.t_DcgVoltLoop.f32Fed;
    UpDnLimit(t_LLcControl.t_DcgVoltLoop.f32Err0, (float)-cVdc40V, (float)cVdc40V);
    t_LLcControl.t_DcgVoltLoop.f32Out2 = t_LLcControl.t_DcgVoltLoop.f32Out1;
    t_LLcControl.t_DcgVoltLoop.f32Out1 = t_LLcControl.t_DcgVoltLoop.f32Out;

    t_LLcControl.t_DcgVoltLoop.f32Out  =        t_LLcControl.t_DcgVoltLoop.f32Out1 \
                                            +   1.002f * t_LLcControl.t_DcgVoltLoop.f32Err0 \
                                            -   0.998f * t_LLcControl.t_DcgVoltLoop.f32Err1;
    
    UpDnLimit(t_LLcControl.t_DcgVoltLoop.f32Out, (float)-cCurr10A, t_LLcControl.f32DcgCurrLimit);

    float f32TempA;
    float f32CurrLoop_K2,f32CurrLoop_K3;
    f32TempA = fabs(sAdc_GetReal(BatCurr));

    if(f32TempA < cCurr10A )
    {
        f32CurrLoop_K2 = 1.588;
        f32CurrLoop_K3 = 1.412;
    }
    else
    {
        f32CurrLoop_K2 = 1.503f;
        f32CurrLoop_K3 = 1.497f;
    }

    if(t_LLcControl.f32ILoopK2_Ref < f32CurrLoop_K2) { t_LLcControl.f32ILoopK2_Ref += 0.001f;}
    else if( t_LLcControl.f32ILoopK2_Ref > f32CurrLoop_K2) {t_LLcControl.f32ILoopK2_Ref -= 0.001f;}

    if(t_LLcControl.f32ILoopK3_Ref < f32CurrLoop_K3) { t_LLcControl.f32ILoopK3_Ref += 0.001f;}
    else if( t_LLcControl.f32ILoopK3_Ref > f32CurrLoop_K3) {t_LLcControl.f32ILoopK3_Ref -= 0.001f;}

    t_LLcControl.t_DcgCurrLoop.f32Ref = t_LLcControl.t_DcgVoltLoop.f32Out;
    t_LLcControl.t_DcgCurrLoop.f32Fed = sAdc_GetReal(BatCurr);
    t_LLcControl.t_DcgCurrLoop.f32Err2 = t_LLcControl.t_DcgCurrLoop.f32Err1;
    t_LLcControl.t_DcgCurrLoop.f32Err1 = t_LLcControl.t_DcgCurrLoop.f32Err0;
    t_LLcControl.t_DcgCurrLoop.f32Err0 = t_LLcControl.t_DcgCurrLoop.f32Ref - t_LLcControl.t_DcgCurrLoop.f32Fed;
    UpDnLimit(t_LLcControl.t_DcgCurrLoop.f32Err0, (float)-cCurr20A, (float)cCurr20A);
    t_LLcControl.t_DcgCurrLoop.f32Out2 = t_LLcControl.t_DcgCurrLoop.f32Out1;
    t_LLcControl.t_DcgCurrLoop.f32Out1 = t_LLcControl.t_DcgCurrLoop.f32Out;

    t_LLcControl.t_DcgCurrLoop.f32Out  =        t_LLcControl.t_DcgCurrLoop.f32Out1 \
                                            +   t_LLcControl.f32ILoopK2_Ref * t_LLcControl.t_DcgCurrLoop.f32Err0 \
                                            -   t_LLcControl.f32ILoopK3_Ref * t_LLcControl.t_DcgCurrLoop.f32Err1;
    
    UpDnLimit(t_LLcControl.t_DcgCurrLoop.f32Out, t_LLcControl.f32DcgPiOutLimitDn, t_LLcControl.f32DcgPiOutLimitUp);
}

void    sLLC_DcgPwmDeal(void)
{
    unsigned int u16Temp;

    unsigned int riseEdgeDly = 72;
    unsigned int fallDegePre = 1;

    u16Temp = (unsigned int)t_LLcControl.t_DcgCurrLoop.f32Out;

    if(u16Temp < t_LLcControl.u16LLC_Prd)   { t_LLcControl.u16LLC_Ps = t_LLcControl.u16LLC_Prd - u16Temp;}
    else                                    { t_LLcControl.u16LLC_Ps = 0;}

    t_LLcControl.f32PsLimit -= t_LLcControl.f32PsLimitStep;
    DnLimit(t_LLcControl.f32PsLimit, 0.60f);
    UpDnLimit(t_LLcControl.u16LLC_Ps, 0, t_LLcControl.f32PsLimit * t_LLcControl.u16LLC_Prd);

    EPwm1Regs.TBPRD         =   t_LLcControl.u16LLC_Prd;
    EPwm1Regs.CMPB.bit.CMPB =   (t_LLcControl.u16LLC_Prd >> 1) + (t_LLcControl.u16LLC_Ps >> 1);

    EPwm2Regs.CMPA.bit.CMPA =   t_LLcControl.u16LLC_Ps;
    EPwm2Regs.CMPB.bit.CMPB =   t_LLcControl.u16LLC_Prd - t_LLcControl.u16LLC_Ps;

    EPwm3Regs.CMPA.bit.CMPA =   t_LLcControl.u16LLC_Prd - fallDegePre;
    EPwm3Regs.CMPB.bit.CMPB =   fallDegePre;
    EPwm3Regs.DBFED.all     =   t_LLcControl.u16LLC_Ps + riseEdgeDly;
    EPwm3Regs.DBRED.all     =   t_LLcControl.u16LLC_Ps + riseEdgeDly;

    EPwm4Regs.CMPA.bit.CMPA =   t_LLcControl.u16LLC_Prd - fallDegePre;
    EPwm4Regs.CMPB.bit.CMPB =   fallDegePre;
    EPwm4Regs.DBFED.all     =   t_LLcControl.u16LLC_Ps + riseEdgeDly;
    EPwm4Regs.DBRED.all     =   t_LLcControl.u16LLC_Ps + riseEdgeDly;
}


void    sLLC_DcgBurstChk(void)
{
    float f32TempA,f32TempB;

    if( t_LLcControl.u16FastSoftFlag == true && t_LLcControl.u16LLC_State == eLLC_FSM_RUN )
    {
        f32TempA = fabs(t_LLcControl.f32BusVoltRef - t_LLcControl.f32BusVoltSet);
        if( t_LLcControl.f32DcgPiOutLimitUp >= (float)cPWM_PRD_80K && f32TempA < cVdc1V )
        {
            t_LLcControl.f32DcgPiOutLimitUp = (float)cPWM_PRD_80K;
            t_LLcControl.f32DcgPiOutLimitDn = 0.0f;
            t_LLcControl.f32DcgCurrLimit    = 200.0f;

            t_LLcControl.u16FastSoftFlag    = false;
            t_LLcControl.f32BusVoltStep     = 0.1f;
        }
    }

    if( t_LLcControl.t_DcgBurst.u16Act== true )
    {
        f32TempA = t_LLcControl.f32BusVoltSet;
        if(t_LLcControl.u16LLC_State == eLLC_FSM_RUN)
        {
            f32TempA = cVdc360V;
        }
    }
    else
    {
        f32TempA = (float)sConfig_GetLLCTrancRaio() * sSample_GetRms(BatVolt);
    }

    f32TempB = sAdc_GetReal(BusVolt);

    if( t_LLcControl.t_DcgBurst.u16EN == false)
    {
        if( sProtect_GetTZFlag(Cmpss4) == false && t_LLcControl.u16PwmEn == true)
        {
            EALLOW;
            EPwm1Regs.TZCLR.bit.OST = 1;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            EPwm4Regs.TZFRC.bit.OST = 1;
            EDIS;
        }
        else
        {
            EALLOW;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            EPwm4Regs.TZFRC.bit.OST = 1;
            EDIS;
        }

        if( f32TempB > (f32TempA - (float)cVdc0V5) || f32TempB > (float)cDCG_BUS_VLOT_BURST_MAX)
        {
            t_LLcControl.t_DcgBurst.u16Cnt++;
            if(t_LLcControl.t_DcgBurst.u16Cnt >= 2)
            {
                t_LLcControl.t_DcgBurst.u16Cnt = 0;
                t_LLcControl.t_DcgBurst.u16EN = true;
            }
        }
        else
        {
            t_LLcControl.t_DcgBurst.u16Cnt = 0;
        }
    }
    else
    {
            EALLOW;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            EPwm4Regs.TZFRC.bit.OST = 1;
            EDIS;

        if( f32TempB < (f32TempA - (float)cVdc10V) && f32TempB <= (float)cDCG_BUS_VLOT_BURST_MAX - cDCG_BUS_VOLT_BURST_BAK)
        {
            t_LLcControl.t_DcgBurst.u16Cnt++;
            if( t_LLcControl.t_DcgBurst.u16Cnt >= 2)
            {
                t_LLcControl.t_DcgBurst.u16Cnt = 0;
                t_LLcControl.t_DcgBurst.u16EN = false;

                if(t_LLcControl.u16LLC_State == eLLC_FSM_RUN)
                {
                    t_LLcControl.u16FastSoftFlag = true;
                    t_LLcControl.f32BusVoltStep  = cVdc5V;

                    t_LLcControl.u16LLC_Prd      = cPWM_PRD_80K;

                    t_LLcControl.f32DcgCurrLimit = 200.0f;
                    t_LLcControl.f32DcgPiOutLimitUp = (float)t_LLcControl.u16LLC_Prd * 0.55f;
                    t_LLcControl.f32DcgPiOutLimitDn = (float)t_LLcControl.u16LLC_Prd * 0.55f;
                }
            }
        }
        else
        {
            t_LLcControl.t_DcgBurst.u16Cnt = 0;
        }
    }
}

void    sLLC_SetPwmChgMode(void)
{
    EALLOW;
    EPwm1Regs.TZFRC.bit.OST = 1;
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    EPwm4Regs.TZFRC.bit.OST = 1;

    EPwm3Regs.AQCTLA.bit.CAU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.PRD = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm3Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.PRD = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

    EPwm3Regs.DBFED.all      = 72;
    EPwm3Regs.DBRED.all      = 72;

    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD = AQ_CLEAR;

    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CBD = AQ_CLEAR;

    EPwm4Regs.DBFED.all      = 72;
    EPwm4Regs.DBRED.all      = 72;


    EPwm1Regs.AQCTLA.bit.CAU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CBD = AQ_CLEAR;

    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

    EPwm1Regs.DBFED.all      = 72;
    EPwm1Regs.DBRED.all      = 72;

    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD = AQ_CLEAR;

    EPwm2Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.PRD = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CBD = AQ_SET;

    EPwm2Regs.DBFED.all      = 72;
    EPwm2Regs.DBRED.all      = 72;

    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 9;
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL = 9;
    
    EDIS;
}

void    sLLC_SetPwmDcgMode(void)
{
    EALLOW;
    EPwm1Regs.TZFRC.bit.OST = 1;
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    EPwm4Regs.TZFRC.bit.OST = 1;

    EPwm1Regs.AQCTLA.bit.CAU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm1Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.PRD = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

    EPwm1Regs.DBFED.all      = 72;
    EPwm1Regs.DBRED.all      = 72;

    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD = AQ_CLEAR;

    EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CBD = AQ_CLEAR;

    EPwm2Regs.DBFED.all      = 72;
    EPwm2Regs.DBRED.all      = 72;


    EPwm3Regs.AQCTLA.bit.CAU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD = AQ_CLEAR;

    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

    EPwm3Regs.DBFED.all      = 72;
    EPwm3Regs.DBRED.all      = 72;

    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm4Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD = AQ_CLEAR;

    EPwm4Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.PRD = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CBD = AQ_SET;

    EPwm4Regs.DBFED.all      = 72;
    EPwm4Regs.DBRED.all      = 72;

    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 5;
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL = 5;
    
    EDIS;
}

void    sLLC_SetDcgEN(unsigned int EN)
{
    if(EN > 1 || t_LLcControl.ChgEN == true)
    {
        t_LLcControl.DcgEN = false;
        return;
    }

    t_LLcControl.DcgEN = EN;
}

void    sLLC_SetChgEN(unsigned int EN)
{
    if(EN > 1 || t_LLcControl.DcgEN == true)
    {
        t_LLcControl.ChgEN = false;
        return;
    }

    t_LLcControl.ChgEN = EN;
}


void    sLLC_SetOpenLoop    (unsigned int EN)
{
    if(EN > 1)
    {
        t_LLcControl.u16OpenLoopEn = false;
        return;
    }

    t_LLcControl.u16OpenLoopEn = EN;   
}

int     sLLC_GetControlFsm(void)
{
    return t_LLcControl.u16LLC_State;
}

int     sLLC_GetFastStartCnt(void)
{
    return t_LLcControl.u16FastStartCnt;
}

