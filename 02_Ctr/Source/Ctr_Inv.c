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
    // if( sGetTZ1_OST_Act()   == true     ||\
    //     sGetTZ2_OST_Act()   == true     ||\
    //     sGetTrip4_OST_Act() == true     ||\
    //     sMsw_GetInvCmd()    == false    )
    if(1)
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
                // sConfig_InvVolt_Resh();
                // sConfig_InvFreq_Resh();

                // if( sMsw_GetInvCmd() == false )
                if(1)
                {
                    // sConfig_InvAngle();

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
                // if( sMsw_GetInvCmd() == true )
                if ( 1 )
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
