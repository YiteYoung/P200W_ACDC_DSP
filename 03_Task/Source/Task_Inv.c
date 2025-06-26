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

        // sSample_Cal_Freq(sGetInvPrdPoint(),cSwitchFreq,sSample_Get_Rms(ComVolt),&t_InvPort.u16Freq);

        sSample_Cal_Inv();
        sSample_Cal_InvLoad();
    }

    if( event & ((TASK_EVENT)1 << eInv_GridZero))
    {
        // sSample_Cal_Freq(sGetGridPrdPoint(),cSwitchFreq,sSample_Get_Rms(GridVolt),&t_GridPort.u16Freq);

        sSample_Cal_DC();
        sSample_Cal_Grid();
        sSample_Cal_GridLoad();
        

        // sGrid_PowerSoft();
        // sGrid_PowerLoop();
        // sGrid_PowerTrack();
    }
}
