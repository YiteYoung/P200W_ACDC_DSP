#include "UserHeader.h"

#pragma CODE_SECTION(sInv_ISR,          ".TI.ramfunc");
#pragma CODE_SECTION(Inv_PwmXUp_ISR,    ".TI.ramfunc");

void sInv_ISR(void)
{
    static unsigned int u16ISRCnt;
    static unsigned int u16TestCnt;

    if(u16TestCnt++ > 24000)
    {
        u16TestCnt = 0;
        sDebugLed5_Run();
    }

    sADCSoftStart();

    sHalfWavePointCal();

    sSetSinCos();

    sAdc_Convert();

    sInv_Control();

    sSample_Filter();

    sSample_Accumulate();

    sSample_ZeroDeal(sPLL_GetGridCrossN2P(),sPLL_GetGridCrossP2N(),sPLL_GetInvCrossN2P(),sPLL_GetInvCrossP2N());

    sGrid_WaveLossDetect();

    unsigned char u8RxData;
    sSciRxISR(SCI_PORT_A, &u8RxData);
    sSciTxISR(SCI_PORT_A);

    sSciRxISR(SCI_PORT_B, &u8RxData);
    sSciTxISR(SCI_PORT_B);

    u16ISRCnt++;
    if( u16ISRCnt >= 4 )
    {
        u16ISRCnt = 0;
    }

    switch (u16ISRCnt) 
    {
        case 0: ServiceDog();  break;
        case 1:                 break;
        case 2:                 break;
        case 3:                 break;
        default:                break;
    }

}

interrupt void Inv_PwmXUp_ISR(void)
{
    sInv_ISR();

    EPwm6Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP3;
}

void sInitInterrupt(void)
{
    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    EALLOW;

    PieVectTable.EPWM6_INT  = &Inv_PwmXUp_ISR;

    EDIS;

    PieCtrlRegs.PIEIER3.bit.INTx6    = 1;

    IER |= M_INT3;

    EINT;
    ERTM;
}


