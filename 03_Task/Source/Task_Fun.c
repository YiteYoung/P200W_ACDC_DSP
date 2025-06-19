#include "UserHeader.h"

void sFunTask(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioFun);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eFun_InvZero))
    {

    }
}
