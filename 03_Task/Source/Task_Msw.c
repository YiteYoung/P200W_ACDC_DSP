#include "UserHeader.h"

void    sMswTask(void)
{
    static unsigned int MswInit_Ok;
    if(MswInit_Ok == false)
    {
        MswInit_Ok = true;


        OSEventClr(cPrioMsw);
    }

    
}

void    sPowerOnMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}
