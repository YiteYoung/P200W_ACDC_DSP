#include "UserHeader.h"

void sInv_ISR(void)
{
    static unsigned int u16ISRCnt;

    sADCSoftStart();

    // sHalfWavePointCal();

    // sSetSinCos();

    sAdc_Convert();

    // sInv_Control();

    sSample_Filter();

    sSample_Accumulate();

    sSample_ZeroDeal(0,0,0,0);

    // sGridWaveLossDetect();

    unsigned char u8RxData;
    sSCiRxISR(SCI_PORT_A, &u8RxData);
    sSciTxISR(SCI_PORT_A);

    sSCiRxISR(SCI_PORT_B, &u8RxData);
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


