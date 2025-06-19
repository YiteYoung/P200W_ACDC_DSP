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

    }
}
