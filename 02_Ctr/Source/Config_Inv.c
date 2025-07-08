#include "UserHeader.h"

static  ConfigInv_t t_InvConfig;

void    sConfig_InitInv(void)
{
    t_InvConfig.t_Inv.i16VoltSet = (int)((float)cVac120V * 1.4142f);

    t_InvConfig.t_Inv.i16FreqSet = Freq50Hz;

    // Loop parameter set
    sConfig_InitInvControl();

    // Loop parameter set
    sConfig_InitPfcControl();

    // Freq parameter set
    sConfig_InvFreqResh();
}

void    sConfig_InitInvControl(void)
{
    Inv_t *pInv;

    pInv = sInv_GetInvCtrPtr();

    pInv->i16InvVoltSet = t_InvConfig.t_Inv.i16VoltSet;        // Ex:220 * 14.142

    pInv->t_PI.Volt_ID.i16Kp        = 1024;
    pInv->t_PI.Volt_ID.i16Ki        = 10;
    pInv->t_PI.Volt_ID.i16PIOutMax  = 2000;
    pInv->t_PI.Volt_ID.i16PIOutMin  = -2000;

    pInv->t_PI.Volt_IQ.i16Kp        = 1024;
    pInv->t_PI.Volt_IQ.i16Ki        = 10;
    pInv->t_PI.Volt_IQ.i16PIOutMax  = 1000;
    pInv->t_PI.Volt_IQ.i16PIOutMin  = -1000;
    
    pInv->t_PI.IndCurr.i16Kp        = 256;
    pInv->t_PI.IndCurr.i16Ki        = 8;
    pInv->t_PI.IndCurr.i16PIOutMax  = cVac20V;
    pInv->t_PI.IndCurr.i16PIOutMin  = -cVac20V;

    pInv->t_PI.InvLimit.i16Kp       = 256;
    pInv->t_PI.InvLimit.i16Ki       = 8;

    pInv->t_PI.InvLimit.i16PIOutMax = (int)((float)cVac40V * 1.4142f);
    pInv->t_PI.InvLimit.i16PIOutMin = 0;
}

void    sConfig_InitPfcControl(void)
{
    Pfc_t *pPfc;

    pPfc = sInv_GetPfcCtrPtr();
    
    pPfc->t_PI.BusVolt.i16Kp            = 12000;
    pPfc->t_PI.BusVolt.i16Ki            = 5;
    pPfc->t_PI.BusVolt.i16PIOutMax      = 5000;
    pPfc->t_PI.BusVolt.i16PIOutMin      = -5000;

    pPfc->t_PI.PfcCurr.i16Kp            = 3000;
    pPfc->t_PI.PfcCurr.i16Ki            = 10;
    pPfc->t_PI.PfcCurr.i16PIOutMax      = cVac50V;
    pPfc->t_PI.PfcCurr.i16PIOutMin      = -cVac50V;
    
    pPfc->t_PI.GridPower.i16Kp          = 2500;
    pPfc->t_PI.GridPower.i16Ki          = 512;
    pPfc->t_PI.GridPower.i16PIOutMax    = cVac40V;
    pPfc->t_PI.GridPower.i16PIOutMin    = -cVac20V;

    pPfc->t_PI.Bat_CC.i16Ki             = 300;
    pPfc->t_PI.Bat_CV.i16Ki             = 1000;
}

void    sConfig_InvFreqResh(void)
{
    Inv_t   *pInv;
    PLL_t   *pPLL;

    pInv = sInv_GetInvCtrPtr();
    pPLL = sPLL_GetPtr();

    switch (t_InvConfig.t_Inv.i16FreqSet) 
    {
        case Freq50Hz:
            pPLL->i32TsPoint = 0x5A000000;
            pPLL->i32InvAngleBase = cInvAngleBase50Hz;
            pInv->i16WC_Q14 = 41;

            sSOGI_Init(cFreq50Hz,cSwitchFreq,1);

            sBandStop_Init(1.0f, 40.0f, 3.1415926f * 2.0f * 50.0f * 2.0f);
            break;

        case Freq60Hz:
            pPLL->i32TsPoint = 0x5A000000;
            pPLL->i32InvAngleBase = cInvAngleBase60Hz;
            pInv->i16WC_Q14 = 49;       // 2 * pi * f * C

            sSOGI_Init(cFreq50Hz,cSwitchFreq,1);

            sBandStop_Init(1.0f, 40.0f, 3.1415926f * 2.0f * 60.0f * 2.0f);
            break;

        default:
            pPLL->i32TsPoint = 0x5A000000;
            pPLL->i32InvAngleBase = cInvAngleBase50Hz;
            pInv->i16WC_Q14 = 41;

            sSOGI_Init(cFreq50Hz,cSwitchFreq,1);

            sBandStop_Init(1.0f, 40.0f, 3.1415926f * 2.0f * 50.0f * 2.0f);
            break;
    }
}


void    sConfig_InvResh(void)
{
    Inv_t *pInv;

    pInv = sInv_GetInvCtrPtr();

    pInv->i16InvVoltSet = t_InvConfig.t_Inv.i16VoltSet;        // Ex:220 * 14.142

    sProtect_SetInvLoad100(cDCG_AC_POWER_MAX);
    sProtect_SetPfcLoad100(cCHG_AC_POWER_MAX);

    sProtect_SetInvCurr100(cDCG_AC_CURR_MAX);
    sProtect_SetPfcCurr100(cCHG_AC_CURR_MAX);

    sProtect_SetChgBatCurrLimit(cCHG_BAT_CURR_MAX);
    sProtect_SetChgBatPowerLimit(cPower200W);
    sProtect_SetChgAcPowerLimit(cPower500W);

    sConfig_InvFreqResh();
}


int    sConfig_GetPfcSet(void)
{
    return t_InvConfig.t_Pfc.i16PfcVoltSet;
}
