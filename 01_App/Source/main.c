// Included Files
#include "UserHeader.h"


//
// Main
//
void main(void)
{
    sInitDspConfig();
    DELAY_US(5000);

    sInitSysConfig();
    DELAY_US(5000);

    while (true) 
    {
        sSYS_Time();
        
        sMswTask();

        sInvTask();

        sFunTask();

        sSciTask();
    }
}

//
// End of File
//
