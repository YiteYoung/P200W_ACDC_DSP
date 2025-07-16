#include "UserHeader.h"

void sFunTask(void)
{
    static unsigned int     u16LoopCnt;
    TASK_EVENT              event;

    event = OSEventPend(cPrioFun);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eFun_InvZero))
    {
        sGrid_ZeroRestDeal();
    }

    if( event & ((TASK_EVENT)1 << eFun_GridZero))
    {
        sGrid_ZeroLossDeal();
    }

    if( event & ((TASK_EVENT)1 << eTimer))
    {
        if (sGetTime_1s() == true)
        {
            sDebugLed_Run();
        }
        
        sUart_DealData();

        if (sGetTime_1ms() == true)
        {
            sRly_Task();
        }

        if (sGetTime_4ms() == false) 
        {
            return;
        }



        u16LoopCnt++;
        if( u16LoopCnt >= 5)
        {
            u16LoopCnt = 0;
        }

        switch (u16LoopCnt) 
        {
            case 0:
                break;

            case 1:
                sGrid_StatusDeal    ();     // 市电状态检测
                sGrid_RmsCheck      ();     // 市电有效值检测
                sGrid_FreqCheck     ();     // 市电频率检测       
                break;

            case 2:
                break;

            case 3:
                break;

            default:
                u16LoopCnt = 0;
                break;
        }
    }
}
