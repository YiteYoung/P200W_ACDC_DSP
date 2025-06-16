#include "f28x_project.h"
#include "BSP_Xbar.h"

void    sInitInputXbar(void)
{
    EALLOW;

    InputXbarRegs.INPUT1SELECT  = 13;
    InputXbarRegs.INPUT2SELECT  = 16;
    InputXbarRegs.INPUT3SELECT  = 17;

    EDIS; 
}

void    sInitOutputXbar(void)
{
    // EALLOW;

    // OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX4  = 3;
    // OutputXbarRegs.OUTPUTINV.bit.OUTPUT3        = 0;
    // OutputXbarRegs.OUTPUT3MUXENABLE.bit.MUX4    = 0;

    // EDIS;
}

void    sInitEPwmXbar(void)
{
    // EALLOW;

    // EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX0      = 1;
    // EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX0        = 1;
    // EPwmXbarRegs.TRIPOUTINV.bit.TRIP4           = 1;

    // EDIS;
}

