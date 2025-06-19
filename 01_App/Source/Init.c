#include "UserHeader.h"

void sInitDspConfig(void)
{
    InitSysCtrl();
    DELAY_US(50);

    sInitRam();
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitFlash();

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

}

