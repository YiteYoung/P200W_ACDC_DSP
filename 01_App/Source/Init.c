#include "UserHeader.h"

void sInitDspConfig(void)
{
    InitSysCtrl();
    DELAY_US(50);

    sInitGpio();
    InitPieCtrl();
    InitPieVectTable();



}

void sInitSysConfig(void)
{

}

