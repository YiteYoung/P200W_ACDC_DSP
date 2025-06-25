#include "UserHeader.h"

void sInvTask(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioInv);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eInv_InvZero))
    {
        // sInv_PowerLimit();

        // sInvFreqCal();

        sSample_Cal_Inv();
        sSample_Cal_InvLoad();
    }

    if( event & ((TASK_EVENT)1 << eInv_GridZero))
    {
        // sGridFreqCal();

        sSample_Cal_Grid();
        sSample_Cal_GridLoad();

        // sGrid_PowerSoft();
        // sGrid_PowerLoop();
        // sGrid_PowerTrack();
    }
}
