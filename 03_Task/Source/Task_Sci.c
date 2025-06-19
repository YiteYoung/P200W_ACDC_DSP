#include "UserHeader.h"

void    sSciTask(void)
{
    unsigned char bRxData;

    TASK_EVENT event;

    event = OSEventPend(cPrioSci);

    if( event == eInitEvent)
    {

    }

    if(event & (TASK_EVENT)1 << eTimer)
    {
        if(sGetTime_1ms() == false)
        {
            return;
        }

        sSciDeathAutoRest(SCI_PORT_A, 30, 15000);
        while(sSciRead(SCI_PORT_A, &bRxData) == eSci_Ready)
        {
            // sSci_Main_RxTxDeal(SCI_PORT_A,bRxData);
        }
    }
}
