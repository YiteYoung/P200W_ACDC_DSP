#include "BSP_OS.h"

TASK_EVENT  OS_TASK_EVEN[TASK_MAX_NUM];

void    OSInit(void)
{
    unsigned int i;

    for (i = 0; i < TASK_MAX_NUM; i++) 
    {
        OS_TASK_EVEN[i] = 0;
    }
}

TASK_EVENT  OSEventPend(INTOS Prio)
{
    TASK_EVENT  event;

    if(Prio >= TASK_MAX_NUM)
    {
        return 1;
    }

    event = OS_TASK_EVEN[Prio];

    OS_TASK_EVEN[Prio] = 1;

    return event;
}

TASK_EVENT  OSEventGet(INTOS Prio)
{
    TASK_EVENT  event;

    if(Prio >= TASK_MAX_NUM)
    {
        return 1;
    }

    event = OS_TASK_EVEN[Prio];

    return event;
}

void    OSEventSend(INTOS Prio, INTOS EventID)
{
    TASK_EVENT  event;

    if( Prio >= TASK_MAX_NUM || EventID >= 16)
    {
        return;
    }

    event = (TASK_EVENT)1 << EventID;

    OS_TASK_EVEN[Prio] |= event;
}

void    OSEventClr(INTOS Prio)
{
    if( Prio >= TASK_MAX_NUM)
    {
        return;
    }

    OS_TASK_EVEN[Prio] = 0;
}
