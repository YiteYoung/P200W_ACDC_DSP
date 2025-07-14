#include "UserHeader.h"

void sInitDspConfig(void)
{
    InitSysCtrl();
    DELAY_US(50);

    sInitRam();
    // memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    // InitFlash();

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
    // sConfig_InitLLC();

    sGrid_InitDetectValue();
    // sProtect_InitBusVoltValue();

    sRly_Init();
}

