#include "UserHeader.h"

void    sInvVoltLoop        (void);
void    sInvCurrLoop        (void);
void    sInvSpwm            (void);

void    sBatCCCVLoop        (void);

void    sPfcVoltLoop        (void);
void    sPfcCurrLoop        (void);
void    sPfcSpwm            (void);

void    sLoopClear          (void);
// void    sInv_PosWaveClr     (void);

void    sPfcBurst_Check     (void);
void    sISR_TZDeal         (void);

void    sPwm_Enable         (void);
void    sPwm_Disable        (void);

static  Control_t           t_Control;

void    sInv_Control(void)
{
    if( sGetTZ1_OST_Act()   == true     ||\
        sGetTZ2_OST_Act()   == true     ||\
        sGetTrip4_OST_Act() == true     ||\
        sMsw_GetInvCmd()    == false    )
    {
        t_Control.t_Flag.InvFsm = eINV_FSM_OFF;
        sPwm_Disable();
        // sInv_PosWaveClr();
        sLoopClear();
    }
    else 
    {
        switch (t_Control.t_Flag.InvFsm) 
        {
            case eINV_FSM_OFF:
            {
                sConfig_InvVolt_Resh();
                sConfig_InvFreq_Resh();

                if( sMsw_GetInvCmd() == false )
                {
                    sConfig_InvAngle();

                    EALLOW;
                    EPwm6Regs.TZSEL.bit.OSHT1   = TZ_DISABLE;
                    EDIS;
                }
                else
                {
                    EALLOW;
                    EPwm6Regs.TZSEL.bit.OSHT1   = TZ_ENABLE;
                    EDIS;
                }

                t_Control.t_Flag.InvFsm = eINV_FSM_INIT;                
            }
                break;

            case eINV_FSM_INIT:
            {
                if( sPLL_GetInvCrossN2P() == true )
                {
                    t_Control.t_Flag.InvFsm = eINV_FSM_RUN;
                }
            }
                break;

            case eINV_FSM_RUN:
            {
                if( sMsw_GetInvCmd() == true )
                {
                    sPfcBurst_Check();

                    if( t_Control.t_Pfc.u16Burst_Act == false )
                    {
                        sPfcVoltLoop();
                        sPfcCurrLoop();
                        sPfcSpwm();
                    }
                }
                else
                {
                    sInvVoltLoop();
                    sInvCurrLoop();
                    sInvSpwm();
                }

                sISR_TZDeal();

                sPwm_Enable();
            }
                break;

            default:
                t_Control.t_Flag.InvFsm = eINV_FSM_OFF;
                sPwm_Disable();
                // sInv_PosWaveClr();
                sLoopClear();
                break;
        }
    } 
}

void    sInvVoltLoop(void)
{
    t_Control.t_Inv.t_PI.Volt_ID.i16Ref         = t_Control.t_Inv.i16InvVoltRef;
    t_Control.t_Inv.t_PI.Volt_IQ.i16Ref         = 0;
    UpDnLimit(t_Control.t_Inv.t_PI.Volt_ID.i16Ref, 0, cVac340V);

    t_Control.t_Inv.t_PI.Volt_ID.i16Fed         = sPLL_GetInvVoltD();
    t_Control.t_Inv.t_PI.Volt_IQ.i16Fed         = sPLL_GetInvVoltQ();

    t_Control.t_Inv.t_PI.Volt_ID.i16Err         = t_Control.t_Inv.t_PI.Volt_ID.i16Ref - t_Control.t_Inv.t_PI.Volt_ID.i16Fed;
    t_Control.t_Inv.t_PI.Volt_IQ.i16Err         = t_Control.t_Inv.t_PI.Volt_IQ.i16Ref - t_Control.t_Inv.t_PI.Volt_IQ.i16Fed;
    UpDnLimit(t_Control.t_Inv.t_PI.Volt_ID.i16Err, -cVac40V, cVac40V);
    UpDnLimit(t_Control.t_Inv.t_PI.Volt_IQ.i16Err, -cVac40V, cVac40V);

    t_Control.t_Inv.t_PI.Volt_ID.i32UpSum       = (long)t_Control.t_Inv.t_PI.Volt_ID.i16Err * t_Control.t_Inv.t_PI.Volt_ID.i16Kp;
    t_Control.t_Inv.t_PI.Volt_IQ.i32UpSum       = (long)t_Control.t_Inv.t_PI.Volt_IQ.i16Err * t_Control.t_Inv.t_PI.Volt_IQ.i16Kp;

    t_Control.t_Inv.t_PI.Volt_ID.i32UiSum       +=  (long)t_Control.t_Inv.t_PI.Volt_ID.i16Err * t_Control.t_Inv.t_PI.Volt_ID.i16Ki;
    t_Control.t_Inv.t_PI.Volt_IQ.i32UiSum       +=  (long)t_Control.t_Inv.t_PI.Volt_IQ.i16Err * t_Control.t_Inv.t_PI.Volt_IQ.i16Ki;

    UpDnLimit(t_Control.t_Inv.t_PI.Volt_ID.i32UiSum, (long)(-10240000), (long)10240000);
    UpDnLimit(t_Control.t_Inv.t_PI.Volt_IQ.i32UiSum, (long)(-10240000), (long)10240000);

    t_Control.t_Inv.t_PI.Volt_ID.i16PIOut       = (int)((t_Control.t_Inv.t_PI.Volt_ID.i32UpSum + t_Control.t_Inv.t_PI.Volt_ID.i32UiSum) >> 10 );
    t_Control.t_Inv.t_PI.Volt_IQ.i16PIOut       = (int)((t_Control.t_Inv.t_PI.Volt_IQ.i32UpSum + t_Control.t_Inv.t_PI.Volt_IQ.i32UiSum) >> 10 );

    UpDnLimit(t_Control.t_Inv.t_PI.Volt_ID.i16PIOut, -10000, 10000);
    UpDnLimit(t_Control.t_Inv.t_PI.Volt_IQ.i16PIOut, -10000, 10000);   
}

void    sInvCurrLoop(void)
{
    long    i32AlfaRef;
    long    i32WcVd,i32WcVq,i32WcAlfa;
    int     CosX,SinX;
    int     Volt_D,Volt_Q;

    Volt_D = sPLL_GetInvVoltD();
    Volt_Q = sPLL_GetInvVoltQ();

    CosX = sPLL_GetCos();
    SinX = sPLL_GetSin();

    // D = Vd - Wc * Vq
    // Q = Vq + Wc * Vd
    i32WcVd     = (long)(((long)Volt_Q * t_Control.t_Inv.i16WC_Q14) >> 14);
    i32WcVq     = (long)(((long)Volt_D * t_Control.t_Inv.i16WC_Q14) >> 14);
    i32WcAlfa   = (long)i32WcVd * CosX + (long)i32WcVq * SinX;

    // DQ --> Alfa Beta
    // Alfa = Vd * Cos - Vq * Sin
    // Beta = Vd * Sin + Vq * Cos
    i32AlfaRef  =   (long)t_Control.t_Inv.t_PI.Volt_ID.i16PIOut * CosX \
                    - (long)t_Control.t_Inv.t_PI.Volt_IQ.i16PIOut * SinX\
                    - i32WcAlfa;

    t_Control.t_Inv.i16VoltLoopOutRef = (int)(i32AlfaRef >> 14);

    t_Control.t_Inv.t_PI.IndCurr.i16Ref = t_Control.t_Inv.i16VoltLoopOutRef + (int)(sAdc_GetReal(OutCurr) * 9.5f);
    UpDnLimit(t_Control.t_Inv.t_PI.IndCurr.i16Ref, -12000, 12000);

    t_Control.t_Inv.t_PI.IndCurr.i16Fed = (int)(sAdc_GetReal(IndCurr) * 10.0f);

    t_Control.t_Inv.t_PI.IndCurr.i16Err = t_Control.t_Inv.t_PI.IndCurr.i16Ref - t_Control.t_Inv.t_PI.IndCurr.i16Fed;
    UpDnLimit(t_Control.t_Inv.t_PI.IndCurr.i16Err, -8000, 8000);

    t_Control.t_Inv.t_PI.IndCurr.i32UpSum = (long)t_Control.t_Inv.t_PI.IndCurr.i16Err * t_Control.t_Inv.t_PI.IndCurr.i16Kp;

    t_Control.t_Inv.t_PI.IndCurr.i16PIOut = (int)((t_Control.t_Inv.t_PI.IndCurr.i32UpSum) >> 12 );
    UpDnLimit(t_Control.t_Inv.t_PI.IndCurr.i16PIOut, -cVac50V, cVac50V);

    if(t_Control.t_Flag.OpenLoop == true)
    {
        t_Control.t_Inv.t_PI.IndCurr.i16PIOut = 0;
    }
}

void    sInvSpwm(void)
{
    int     i16FeedForward;
    int     i16TempA;
    int     i16CMPAMINTemp,i16CMPAMAXTemp;
    long    i32TempA;
    int     CosX;

    CosX = sPLL_GetCos();

    t_Control.t_Var.i16Spwm_Vbus     = sAdc_GetReal(BusVolt);

    if(t_Control.t_Flag.OpenLoop == true)
    {
        t_Control.t_Var.i16Spwm_Vbus = cVdc400V;
    }
    UpDnLimit(t_Control.t_Var.i16Spwm_Vbus, cVdc5V, cVdc520V);

    t_Control.t_Var.i16Spwm_DEAD = 0;

    t_Control.t_Var.u16Spwm_TBPRD = EPwm6Regs.TBPRD;
    if(t_Control.t_Var.u16Spwm_TBPRD > 30000 )
    {
        t_Control.t_Var.u16Spwm_TBPRD = 30000;
    }

    i16FeedForward = (int)((t_Control.t_Inv.i16InvVoltSet * CosX) >> 14);
    t_Control.t_Var.i16Spwm_Ref = i16FeedForward + t_Control.t_Inv.t_PI.IndCurr.i16PIOut;

    t_Control.t_Var.i16Spwm_Set = abs(t_Control.t_Var.i16Spwm_Ref);
    UpDnLimit(t_Control.t_Var.i16Spwm_Ref, cVdc0V, t_Control.t_Var.i16Spwm_Vbus);

    i32TempA = (long)t_Control.t_Var.i16Spwm_Set * t_Control.t_Var.u16Spwm_TBPRD;
    i32TempA = (long)__divf32(i32TempA, (float)t_Control.t_Var.i16Spwm_Vbus);
    i16TempA = i32TempA + t_Control.t_Var.i16Spwm_DEAD;

    if( sPLL_GetInvPos() == true )
    {
        t_Control.t_Var.i16Spwm_H_CMP = i16TempA;
        t_Control.t_Var.i16Spwm_L_CMP = 0;
    }
    else 
    {
        t_Control.t_Var.i16Spwm_H_CMP = t_Control.t_Var.u16Spwm_TBPRD - i16TempA;
        t_Control.t_Var.i16Spwm_L_CMP = t_Control.t_Var.u16Spwm_TBPRD;
    }

    if( sPLL_GetInvCrossN2P() == true || sPLL_GetInvCrossP2N() == true )
    {
        // 只封高频管子，不能封高频，否则感性负载出现过零尖峰
        sPwm_INV_LF_Shut();
        // sPwm_INV_HF_Shut();

        t_Control.t_Var.u16LFIGBT_DelayCnt = 0;
        t_Control.t_Var.u16IGBT_EN_DLY     = 0;
    }

    if( sPLL_GetInvCrossWave() == true )
    {
        t_Control.t_Inv.u16VoltPos_Cnt++;
        if( t_Control.t_Inv.u16VoltPos_Cnt < 3)
        {
            i16CMPAMINTemp = 150;
        }
        else 
        {
            t_Control.t_Inv.u16VoltPos_Cnt = 0;

            sPLL_ClrInvCrossWave();

            i16CMPAMINTemp = cINV_CMPA_DBT_MIN;
        }
    }
    else
    {
        t_Control.t_Inv.u16VoltPos_Cnt = 0;
        i16CMPAMINTemp = cINV_CMPA_DBT_MIN;
    }

    if( t_Control.t_Var.i16Spwm_H_CMP < 40 )
    {
        t_Control.t_Var.i16Spwm_H_CMP = 20;
    }
    else if( t_Control.t_Var.i16Spwm_H_CMP < cINV_CMPA_MIN ) 
    {
        t_Control.t_Var.i16Spwm_H_CMP = cINV_CMPA_MIN;
    }

    i16CMPAMAXTemp = t_Control.t_Var.u16Spwm_TBPRD - i16CMPAMINTemp;
    UpDnLimit(t_Control.t_Var.i16Spwm_H_CMP, i16CMPAMINTemp, i16CMPAMAXTemp);

    EPwm5Regs.CMPA.bit.CMPA = t_Control.t_Var.i16Spwm_H_CMP;
    EPwm6Regs.CMPA.bit.CMPA = t_Control.t_Var.i16Spwm_L_CMP;
}

void    sBatCCCVLoop(void)
{
    int i16BatCurrFed,i16BatVoltFed;
    int i16BatCurrRef,i16BatVoltRef;
    int i16BatCurrTemp;
    int i16GridPowerGiv;

    i16GridPowerGiv = sConfig_GetSelfPower();

    if( i16GridPowerGiv >= 0 )
    {
        t_Control.t_Pfc.BatCCCVState        = eBat_CC_Mode;
        t_Control.t_Pfc.i16BatCCCVPIOut     = 0;

        t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum = 0; 
        t_Control.t_Pfc.t_PI.Bat_CV.i32UpSum = 0;

        return;
    }

    i16BatCurrFed = sSample_GetRms(BatCurr);
    i16BatVoltFed = sSample_GetRms(BatVolt);

    i16BatCurrRef = sConfig_GetBatCurrGiv();
    i16BatVoltRef = sConfig_GetBatVoltGiv();

    if( t_Control.t_Pfc.BatCCCVState == eBat_CC_Mode )
    {
        if(i16BatVoltFed > i16BatVoltRef)
        {
            t_Control.t_Pfc.u16BatCCCVSWCnt++;
            if(t_Control.t_Pfc.u16BatCCCVSWCnt >= 2)
            {
                t_Control.t_Pfc.BatCCCVState    = eBat_CV_Mode;
                t_Control.t_Pfc.u16BatCCCVSWCnt = 0;

                t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum = t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum;
            }
        }
        else
        {
            t_Control.t_Pfc.u16BatCCCVSWCnt = 0;
        }
    }
    else
    {
        if(i16BatCurrFed > i16BatCurrRef)
        {
            t_Control.t_Pfc.u16BatCCCVSWCnt++;
            if(t_Control.t_Pfc.u16BatCCCVSWCnt >= 2)
            {
                t_Control.t_Pfc.BatCCCVState    = eBat_CC_Mode;
                t_Control.t_Pfc.u16BatCCCVSWCnt = 0;

                t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum = t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum;
            }
        }
        else
        {
            t_Control.t_Pfc.u16BatCCCVSWCnt = 0;
        }
    }

    if( t_Control.t_Pfc.BatCCCVState == eBat_CC_Mode )
    {
        // CP
        i16BatCurrTemp = ( labs(sConfig_GetBatPowerLimit()) * 100) / i16BatVoltFed;

        if(i16BatCurrTemp < i16BatCurrRef)
        {
            i16BatCurrRef = i16BatCurrTemp;
        }

        // CC
        t_Control.t_Pfc.t_PI.Bat_CC.i16Ref = i16BatCurrRef;

        t_Control.t_Pfc.t_PI.Bat_CC.i16Fed = i16BatCurrFed;

        t_Control.t_Pfc.t_PI.Bat_CC.i16Err = t_Control.t_Pfc.t_PI.Bat_CC.i16Ref  - t_Control.t_Pfc.t_PI.Bat_CC.i16Fed;
        UpDnLimit(t_Control.t_Pfc.t_PI.Bat_CC.i16Err, -cCurr30A, cCurr30A);

        t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum += (long)t_Control.t_Pfc.t_PI.Bat_CC.i16Err * t_Control.t_Pfc.t_PI.Bat_CC.i16Ki;
        UpDnLimit(t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum, ((long)-cPower1800W << 10), 0);

        t_Control.t_Pfc.i16BatCCCVPIOut = (int)(t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum >> 10);
    }
    else 
    {
        // CV
        t_Control.t_Pfc.t_PI.Bat_CV.i16Ref = i16BatVoltRef;

        t_Control.t_Pfc.t_PI.Bat_CV.i16Fed = i16BatVoltFed;

        t_Control.t_Pfc.t_PI.Bat_CV.i16Err = t_Control.t_Pfc.t_PI.Bat_CV.i16Ref  - t_Control.t_Pfc.t_PI.Bat_CV.i16Fed;
        UpDnLimit(t_Control.t_Pfc.t_PI.Bat_CV.i16Err, -cVdc2V, cVdc2V);

        t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum += (long)t_Control.t_Pfc.t_PI.Bat_CV.i16Err * t_Control.t_Pfc.t_PI.Bat_CV.i16Ki;
        UpDnLimit(t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum, ((long)-cPower1800W << 10), 0);

        t_Control.t_Pfc.i16BatCCCVPIOut = (int)(t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum >> 10);
    }
}

void    sPfcPowerLoop(void)
{
    int ForwardVolt,PowerRef;
    int i16GridPowerGiv;
    int LoadWatt;

    if( sMsw_Pfc2Grid() == true || sMsw_PfcSoftOk() == true )
    {
        t_Control.t_Pfc.BatCCCVState    = eBat_CC_Mode;
        t_Control.t_Pfc.t_PI.GridPower.i16PIOut = 0;

        t_Control.t_Pfc.i16BatCCCVPIOut = 0;

        t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum = 0;
        t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum = 0;

        return;
    }

    i16GridPowerGiv = sConfig_GetSelfPower();

    sBatCCCVLoop();

    PowerRef = sConfig_GetControlPower() - t_Control.t_Pfc.i16BatCCCVPIOut;

    // 根据实际调试效果测量出的前馈电压，从而减小环路的运算
    if( PowerRef >= 0)
    {
        ForwardVolt = -0.01111f * PowerRef; // (线性关系)
    }
    else 
    {
        ForwardVolt = -0.03055f * PowerRef;
    }
    UpDnLimit(ForwardVolt, -cVdc5V, cVdc5V);

    // 充电为负
    if( i16GridPowerGiv >= cPower0W)
    {
        t_Control.t_Pfc.t_PI.GridPower.i16Ref = Min2(PowerRef, sConfig_GetGridPowerLiomit());
    }
    else
    {
        t_Control.t_Pfc.t_PI.GridPower.i16Ref = Max2(PowerRef, - sConfig_GetGridPowerLiomit());
    }
    UpDnLimit(t_Control.t_Pfc.t_PI.GridPower.i16Ref, -cPower1800W, cPower1800W);

    if( sRly_GetRlyOn(Rly_Out) == true )
    {
        LoadWatt = (int)(sSample_GetGridWatt() * 0.1f + sSample_GetInvWatt() * 0.1f);
    }
    else
    {
        LoadWatt = (int)(sSample_GetGridWatt() * 0.1f);
    }

    t_Control.t_Pfc.t_PI.GridPower.i16Fed = LoadWatt;

    t_Control.t_Pfc.t_PI.GridPower.i16Err = t_Control.t_Pfc.t_PI.GridPower.i16Ref - t_Control.t_Pfc.t_PI.GridPower.i16Fed;
    UpDnLimit(t_Control.t_Pfc.t_PI.GridPower.i16Err, -cPower500W, cPower500W);

    t_Control.t_Pfc.t_PI.GridPower.i32UpSum = (long)t_Control.t_Pfc.t_PI.GridPower.i16Err * t_Control.t_Pfc.t_PI.GridPower.i16Kp;

    t_Control.t_Pfc.t_PI.GridPower.i32UiSum += (long)t_Control.t_Pfc.t_PI.GridPower.i16Err * t_Control.t_Pfc.t_PI.GridPower.i16Ki;
    UpDnLimit(t_Control.t_Pfc.t_PI.GridPower.i16Err, (long)- cVdc20V << 14, (long)cVdc20V << 14);

    t_Control.t_Pfc.t_PI.GridPower.i16PIOut = (int)((t_Control.t_Pfc.t_PI.GridPower.i32UpSum + t_Control.t_Pfc.t_PI.GridPower.i32UiSum) >> 14);
    t_Control.t_Pfc.t_PI.GridPower.i16PIOut += ForwardVolt;
    UpDnLimit(t_Control.t_Pfc.t_PI.GridPower.i16PIOut, t_Control.t_Pfc.t_PI.GridPower.i16PIOutMin, t_Control.t_Pfc.t_PI.GridPower.i16PIOutMax);
}

void    sPfcVoltLoop(void)
{
    // Ref
    t_Control.t_Pfc.t_PI.BusVolt.i16Ref = t_Control.t_Pfc.i16PfcVoltRef - t_Control.t_Pfc.t_PI.GridPower.i16PIOut;
    UpDnLimit(t_Control.t_Pfc.t_PI.BusVolt.i16Ref, cVdc0V, cCHG_BUS_VOLT_REF_MAX);

    // Fed
    t_Control.t_Pfc.t_PI.BusVolt.i16Fed = (int)sAdc_GetReal(BusVolt);

    // BandStop
    t_Control.t_Pfc.t_PI.BusVolt.i16Fed = t_Control.t_Pfc.t_PI.BusVolt.i16Fed - sBandStop_Refresh((float)t_Control.t_Pfc.t_PI.BusVolt.i16Fed);

    // Err
    t_Control.t_Pfc.t_PI.BusVolt.i16Err = t_Control.t_Pfc.t_PI.BusVolt.i16Ref - t_Control.t_Pfc.t_PI.BusVolt.i16Fed;
    UpDnLimit(t_Control.t_Pfc.t_PI.BusVolt.i16Err, -cVdc30V, cVdc30V);

    // 过冲补丁
    if( t_Control.t_Pfc.t_PI.BusVolt.i16Err < -cVdc10V )
    {
        if( t_Control.t_Pfc.t_PI.BusVolt.i32UiSum > 0)
        {
            t_Control.t_Pfc.t_PI.BusVolt.i32UiSum = t_Control.t_Pfc.t_PI.BusVolt.i32UiSum >> 1;
        }
    }

    // P
    t_Control.t_Pfc.t_PI.BusVolt.i32UpSum = (long)t_Control.t_Pfc.t_PI.BusVolt.i16Err * t_Control.t_Pfc.t_PI.BusVolt.i16Kp;

    // I
    t_Control.t_Pfc.t_PI.BusVolt.i32UiSum += (long)t_Control.t_Pfc.t_PI.BusVolt.i16Err * t_Control.t_Pfc.t_PI.BusVolt.i16Ki;
    UpDnLimit(t_Control.t_Pfc.t_PI.BusVolt.i32UiSum, ((long)t_Control.t_Pfc.t_PI.BusVolt.i16PIOutMin << 10 ),((long)t_Control.t_Pfc.t_PI.BusVolt.i16PIOutMax << 10));

    t_Control.t_Pfc.t_PI.BusVolt.i16PIOut =  (int)((t_Control.t_Pfc.t_PI.BusVolt.i32UpSum + t_Control.t_Pfc.t_PI.BusVolt.i32UiSum) >> 10);
    UpDnLimit(t_Control.t_Pfc.t_PI.BusVolt.i16PIOut, t_Control.t_Pfc.t_PI.BusVolt.i16PIOutMin, t_Control.t_Pfc.t_PI.BusVolt.i16PIOutMax);
}

void    sPfcCurrLoop(void)
{
    long    i32TempA,i32TempB;
    long    i32AlfaRef;
    int     i16CosX;

    i16CosX = sPLL_GetCos();
    // Ref
    i32TempA = t_Control.t_Pfc.t_PI.BusVolt.i16PIOut;
    i32TempB = ((long)t_Control.t_Pfc.t_PI.BusVolt.i16PIOut * i16CosX) >> 18;

    i32TempA += i32TempB;

    // DQ --> Alfa
    // Alfa = Vd * Cos - Vq * Sin
    // Beta = Vd * Sin + Vq * Cos
    i32AlfaRef  = (long)i32TempA * i16CosX;
    t_Control.t_Pfc.t_PI.PfcCurr.i16Ref = (int)(i32AlfaRef >> 14);
    UpDnLimit(t_Control.t_Pfc.t_PI.PfcCurr.i16Ref, -4000, 4000);

    // Fed
    t_Control.t_Pfc.t_PI.PfcCurr.i16Fed = (int)(sAdc_GetReal(IndCurr) * -10.0f);

    // Err
    t_Control.t_Pfc.t_PI.PfcCurr.i16Err = t_Control.t_Pfc.t_PI.PfcCurr.i16Ref - t_Control.t_Pfc.t_PI.PfcCurr.i16Fed;
    UpDnLimit(t_Control.t_Pfc.t_PI.PfcCurr.i16Err, -2000, 2000);

    // P
    t_Control.t_Pfc.t_PI.PfcCurr.i32UpSum = (long)t_Control.t_Pfc.t_PI.PfcCurr.i16Err * t_Control.t_Pfc.t_PI.PfcCurr.i16Kp;

    // Out
    t_Control.t_Pfc.t_PI.PfcCurr.i16PIOut = (int)(t_Control.t_Pfc.t_PI.PfcCurr.i32UpSum >> 12);
    UpDnLimit(t_Control.t_Pfc.t_PI.PfcCurr.i16PIOut, t_Control.t_Pfc.t_PI.PfcCurr.i16PIOutMin, t_Control.t_Pfc.t_PI.PfcCurr.i16PIOutMax);
}

void    sPfcBurst_Check()
{
    if( sMsw_GetPfc2Grid() == true )
    {
        t_Control.t_Pfc.u16Burst_Act = false;
        return;
    }

    int i16BusVoltSet,i16BusVolt;

    i16BusVoltSet = sConfig_BusVoltGiv();
    t_Control.t_Pfc.i16Burst_ValueHi = i16BusVoltSet + cVdc30V;
    t_Control.t_Pfc.i16Burst_VauleLo = i16BusVoltSet + cVdc25V;

    UpLimit(t_Control.t_Pfc.i16Burst_ValueHi, cCHG_BUS_VOLT_BURST_MAX);
    DnLimit(t_Control.t_Pfc.i16Burst_VauleLo, cCHG_BUS_VOLT_REF_MIN);

    i16BusVolt = (int)sAdc_GetReal(BusVolt);

    if( t_Control.t_Pfc.u16Burst_Act == false )
    {
        if( i16BusVolt >= t_Control.t_Pfc.i16Burst_ValueHi || i16BusVolt >= cCHG_BUS_VOLT_BURST_MAX)
        {
            t_Control.t_Pfc.u16Burst_Cnt++;
        }
        else 
        {
            t_Control.t_Pfc.u16Burst_Cnt = 0;
        }

        if(t_Control.t_Pfc.u16Burst_Cnt >= 5 )
        {
            t_Control.t_Pfc.u16Burst_Cnt = 0;
            t_Control.t_Pfc.u16Burst_Act = true;
        }
    }
    else
    {
        if( i16BusVolt <= t_Control.t_Pfc.i16Burst_ValueHi && i16BusVolt <= cCHG_BUS_VOLT_BURST_MAX - cCHG_BUS_VOLT_BURST_BACK)
        {
            t_Control.t_Pfc.u16Burst_Cnt++;
        }
        else 
        {
            t_Control.t_Pfc.u16Burst_Cnt = 0;
        }

        if(t_Control.t_Pfc.u16Burst_Cnt >= 24 )
        {
            t_Control.t_Pfc.u16Burst_Cnt = 0;
            t_Control.t_Pfc.u16Burst_Act = false;
        }
    }
}

void    sPfcSpwm(void)
{
    int     i16TempA,i16TempB;
    long    i32TempA;

    t_Control.t_Var.i16Spwm_Vbus = t_Control.t_Pfc.t_PI.BusVolt.i16Ref;
    UpDnLimit(t_Control.t_Var.i16Spwm_Vbus, cVdc5V, cVdc520V);

    t_Control.t_Var.u16Spwm_TBPRD = EPwm6Regs.TBPRD;
    if(t_Control.t_Var.u16Spwm_TBPRD > 30000 )
    {
        t_Control.t_Var.u16Spwm_TBPRD = 30000;
    }

    // 添加并网输入微分
    i16TempA = (int)sAdc_GetReal(ComVolt);
    i16TempA -= t_Control.t_Pfc.t_PI.PfcCurr.i16PIOut;
    t_Control.t_Var.i16Spwm_Ref = i16TempA;

    t_Control.t_Var.i16Spwm_Set = labs(t_Control.t_Var.i16Spwm_Ref);
    UpDnLimit(t_Control.t_Var.i16Spwm_Ref, cVdc0V, t_Control.t_Var.i16Spwm_Vbus);

    i32TempA = (long)t_Control.t_Var.i16Spwm_Set * t_Control.t_Var.u16Spwm_TBPRD;
    i32TempA = (long)__divf32(i32TempA, (float)t_Control.t_Var.i16Spwm_Vbus);

    // 优化iTHD的死区补偿
    i16TempB = labs(t_Control.t_Pfc.t_PI.BusVolt.i16PIOut);
    t_Control.t_Var.i16Spwm_DEAD = (int)(((float)i16TempB * 0.167f) - 33.0f);

    if( t_Control.t_Pfc.t_PI.BusVolt.i16PIOut > 0 )
    {
        i32TempA -= (long)t_Control.t_Var.i16Spwm_DEAD;
    }
    else
    {
        i32TempA += (long)t_Control.t_Var.i16Spwm_DEAD;
    }
    UpDnLimit(i32TempA, 0, t_Control.t_Var.u16Spwm_TBPRD);
    i16TempA = i32TempA;

    if( sPLL_GetPosWaveEn() == true )
    {
        t_Control.t_Var.i16Spwm_H_CMP = i16TempA;
        t_Control.t_Var.i16Spwm_L_CMP = 0;
    }
    else 
    {
        t_Control.t_Var.i16Spwm_H_CMP = t_Control.t_Var.u16Spwm_TBPRD - i16TempA;
        t_Control.t_Var.i16Spwm_L_CMP = t_Control.t_Var.u16Spwm_TBPRD;
    }

    if( sPLL_GetGridCrossN2P() == true || sPLL_GetGridCrossP2N() == true )
    {
        sPwm_INV_LF_Shut();

        t_Control.t_Var.u16LFIGBT_DelayCnt  = 0;
        t_Control.t_Var.u16IGBT_EN_DLY      = 0;
    }

    if( t_Control.t_Var.i16Spwm_H_CMP < 40 )
    {
        t_Control.t_Var.i16Spwm_H_CMP = 5;
    }
    else if( t_Control.t_Var.i16Spwm_H_CMP < cINV_CMPA_MIN )
    {
        t_Control.t_Var.i16Spwm_H_CMP = cINV_CMPA_MIN;
    }
    UpDnLimit(t_Control.t_Var.i16Spwm_H_CMP, cINV_CMPA_DBT_MIN, cINV_CMPA_MAX);

    EPwm5Regs.CMPA.bit.CMPA = t_Control.t_Var.i16Spwm_H_CMP;
    EPwm6Regs.CMPA.bit.CMPA = t_Control.t_Var.i16Spwm_L_CMP;
}

void    sInv_PosWaveClr(void)
{
    t_Control.t_Var.u16IGBT_EN_DLY      = 0;
    // t_Control.
}

void    sLoopClear(void)
{
    t_Control.t_Inv.t_PI.Volt_ID.i16Ref         = 0;
    t_Control.t_Inv.t_PI.Volt_ID.i16Fed         = 0;
    t_Control.t_Inv.t_PI.Volt_ID.i16Err         = 0;
    t_Control.t_Inv.t_PI.Volt_ID.i32UpSum       = 0;
    t_Control.t_Inv.t_PI.Volt_ID.i32UiSum       = 0;
    t_Control.t_Inv.t_PI.Volt_ID.i16PIOut       = 0;

    t_Control.t_Inv.t_PI.Volt_IQ.i16Ref         = 0;
    t_Control.t_Inv.t_PI.Volt_IQ.i16Fed         = 0;
    t_Control.t_Inv.t_PI.Volt_IQ.i16Err         = 0;
    t_Control.t_Inv.t_PI.Volt_IQ.i32UpSum       = 0;
    t_Control.t_Inv.t_PI.Volt_IQ.i32UiSum       = 0;
    t_Control.t_Inv.t_PI.Volt_IQ.i16PIOut       = 0;

    t_Control.t_Inv.t_PI.IndCurr.i16Ref         = 0;
    t_Control.t_Inv.t_PI.IndCurr.i16Fed         = 0;
    t_Control.t_Inv.t_PI.IndCurr.i16Err         = 0;
    t_Control.t_Inv.t_PI.IndCurr.i32UpSum       = 0;
    t_Control.t_Inv.t_PI.IndCurr.i32UiSum       = 0;
    t_Control.t_Inv.t_PI.IndCurr.i16PIOut       = 0;

    t_Control.t_Pfc.t_PI.GridPower.i16Ref       = 0;
    t_Control.t_Pfc.t_PI.GridPower.i16Fed       = 0;
    t_Control.t_Pfc.t_PI.GridPower.i16Err       = 0;
    t_Control.t_Pfc.t_PI.GridPower.i32UpSum     = 0;
    t_Control.t_Pfc.t_PI.GridPower.i32UiSum     = 0;
    t_Control.t_Pfc.t_PI.GridPower.i16PIOut     = 0;

    t_Control.t_Pfc.t_PI.BusVolt.i16Ref         = 0;
    t_Control.t_Pfc.t_PI.BusVolt.i16Fed         = 0;
    t_Control.t_Pfc.t_PI.BusVolt.i16Err         = 0;
    t_Control.t_Pfc.t_PI.BusVolt.i32UpSum       = 0;
    t_Control.t_Pfc.t_PI.BusVolt.i32UiSum       = 0;
    t_Control.t_Pfc.t_PI.BusVolt.i16PIOut       = 0;

    t_Control.t_Pfc.t_PI.PfcCurr.i16Ref         = 0;
    t_Control.t_Pfc.t_PI.PfcCurr.i16Fed         = 0;
    t_Control.t_Pfc.t_PI.PfcCurr.i16Err         = 0;
    t_Control.t_Pfc.t_PI.PfcCurr.i32UpSum       = 0;
    t_Control.t_Pfc.t_PI.PfcCurr.i32UiSum       = 0;
    t_Control.t_Pfc.t_PI.PfcCurr.i16PIOut       = 0;

    t_Control.t_Pfc.t_PI.Bat_CC.i16Ref           = 0;
    t_Control.t_Pfc.t_PI.Bat_CC.i16Fed           = 0;
    t_Control.t_Pfc.t_PI.Bat_CC.i16Err           = 0;
    t_Control.t_Pfc.t_PI.Bat_CC.i32UpSum         = 0;
    t_Control.t_Pfc.t_PI.Bat_CC.i32UiSum         = 0;
    t_Control.t_Pfc.t_PI.Bat_CC.i16PIOut         = 0;

    t_Control.t_Pfc.t_PI.Bat_CV.i16Ref           = 0;
    t_Control.t_Pfc.t_PI.Bat_CV.i16Fed           = 0;
    t_Control.t_Pfc.t_PI.Bat_CV.i16Err           = 0;
    t_Control.t_Pfc.t_PI.Bat_CV.i32UpSum         = 0;
    t_Control.t_Pfc.t_PI.Bat_CV.i32UiSum         = 0;
    t_Control.t_Pfc.t_PI.Bat_CV.i16PIOut         = 0;

    t_Control.t_Var.i16Spwm_Set                 = 0;
    t_Control.t_Var.i16Spwm_H_CMP               = 0;
    t_Control.t_Var.i16Spwm_DEAD                = 0;

    t_Control.t_Pfc.u16Burst_EN                 = false;
    t_Control.t_Pfc.u16Burst_Act                = false;
}
