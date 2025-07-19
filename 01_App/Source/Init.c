#include "UserHeader.h"

void sInitDspConfig(void)
{
    InitSysCtrl();
    DELAY_US(50);

    sInitGpio();
    sInitInterrupt();
    sInitSci();
    sInitAdc();
    sInitCmpss();
    sInitInputXbar();
    sInitOutputXbar();
    sInitEPwmXbar();
    sInitEPwm();
    sInitTimer();
}

void sInitSysConfig(void)
{
    sAdc_Calibration();
    sAdc_InitGain();

    sLimit_SetChgAcCurr(cCurr10A);
    sLimit_SetChgAcPower(cPower500W);

    sConfig_InitInv();
    sConfig_InitLLC();

    sGrid_InitThreshold();
    
    sProtect_InitBatVoltThreshold();
    sProtect_InitBusVoltThreshold();

    sRly_Init();
}

