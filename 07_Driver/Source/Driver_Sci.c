#include "Driver_Sci.h"

volatile struct SCI_REGS* const SciReg[SCI_PORT_End] = 
{
    &SciaRegs,
    &ScibRegs,
    &ScicRegs
};

void    sDrv_SetSci(unsigned int SCI_NO, unsigned int BaudRate, unsigned int CharSize, unsigned int Parity, unsigned int StopBit, signed int FIFOMode)
{
    // Check Sci Port
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    // Disable INT
    sSCI_RX_DIS( SCI_NO );
    sSCI_TX_DIS( SCI_NO );

    // Set Baud Rate
    sDrv_SetSci_Baud( SCI_NO, BaudRate );

    // Set Char size
    sDrv_SetSci_Char( SCI_NO, CharSize );

    // Set Parity
    sDrv_SetSci_Pary( SCI_NO, Parity );

    // Set Stop Bit
    sDrv_SetSci_FIFO( SCI_NO, FIFOMode );

    // Rest SCI, Clear Flag
    sSCI_Rest_EN( SCI_NO );
    asm(" RPT #7 || NOP");
    sSCI_Rest_DIS( SCI_NO );

    // Enable Rx Tx
    sSCI_RX_EN( SCI_NO );
    sSCI_TX_EN( SCI_NO );

    // Enable INT
    sSCI_RX_INT_EN( SCI_NO );
    sSCI_FIFORX_INT_EN( SCI_NO );
    // sSCI_TX_INT_EN( SCI_NO );
    // sSCI_FIFOTX_INT_EN( SCI_NO );
}

void    sDrv_SetSci_Baud(unsigned int SCI_NO, unsigned int BaudRate)
{
    unsigned int    Brr;
    unsigned long   Sci_Baud;

    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    switch (BaudRate) 
    {
        case eSCI_BAUD_2400:
            Sci_Baud = 2400;
            break;
        
        case eSCI_BAUD_4800:
            Sci_Baud = 4800;
            break;

        case eSCI_BAUD_9600:
            Sci_Baud = 9600;
            break;

        case eSCI_BAUD_14400:
            Sci_Baud = 14400;
            break;
        
        case eSCI_BAUD_19200:
            Sci_Baud = 19200;
            break;

        case eSCI_BAUD_38400:
            Sci_Baud = 38400;
            break;

        case eSCI_BAUD_57600:
            Sci_Baud = 57600;
            break;
        
        case eSCI_BAUD_115200:
            Sci_Baud = 115200;
            break;

        default:
            return;
    }

    Brr = (unsigned int)( (unsigned long)cSCICLK / ((unsigned long)Sci_Baud * 8) - 1) + 1;

    if(Brr == 0)
    {
        Brr = (unsigned int)(cSCICLK >> 4);
    }

    (*SciReg[SCI_NO]).SCIHBAUD.all = (unsigned int)(Brr >> 8);
    (*SciReg[SCI_NO]).SCILBAUD.all = (unsigned int)(Brr & 0x00FF);
}

void    sDrv_SetSci_Char(unsigned int SCI_NO, unsigned int CharSize)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    if( CharSize >= eSCI_Char_End)
    {
        return;
    }

    if( CharSize == eSCI_Char_Default)
    {
        CharSize = eSCI_Char_Eight;
    }

    (*SciReg[SCI_NO]).SCICCR.bit.SCICHAR = CharSize - 1;
}

void    sDrv_SetSci_Pary(unsigned int SCI_NO, unsigned int Parity)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    switch (Parity)
    {
        case eSCI_Parity_None:
            (*SciReg[SCI_NO]).SCICCR.bit.PARITYENA  = false;
            break;

        case eSCI_Parity_ODD:
            (*SciReg[SCI_NO]).SCICCR.bit.PARITYENA  = true;
            (*SciReg[SCI_NO]).SCICCR.bit.PARITY     = false;
            break;

        case eSCI_Parity_Even:
            (*SciReg[SCI_NO]).SCICCR.bit.PARITYENA  = true;
            (*SciReg[SCI_NO]).SCICCR.bit.PARITY     = true;
            break;

        default:
            break;
    }

}

void    sDrv_SetSci_Stop(unsigned int SCI_NO, unsigned int StopBit)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    if( StopBit >= eSCI_Stop_End)
    {
        return;
    }

    (*SciReg[SCI_NO]).SCICCR.bit.STOPBITS   = StopBit;
}

void    sDrv_SetSci_FIFO(unsigned int SCI_NO, unsigned int FIFOMode)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    if( FIFOMode >= eSCI_FIFO_End)
    {
        return;
    }

    if(FIFOMode == eSCI_FIFO_EN )
    {
        (*SciReg[SCI_NO]).SCIFFTX.bit.SCIFFENA      = 1;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFIFORESET   = 0;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFIFORESET    = 0;
        asm(" RPT #7 || NOP");

        (*SciReg[SCI_NO]).SCIFFTX.bit.SCIRST        = 1;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFIFORESET   = 1;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFFIL        = 0x00;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFFIENA      = 0;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFFINTCLR    = 1;

        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFIFORESET   = 1;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFIL        = 0x01;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFIENA      = 0;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFINTCLR    = 1;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFOVRCLR    = 1;
    }
    else 
    {
        (*SciReg[SCI_NO]).SCIFFTX.bit.SCIFFENA      = 0;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFIFORESET   = 0;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFIFORESET    = 0;
        asm(" RPT #7 || NOP");

        (*SciReg[SCI_NO]).SCIFFTX.bit.SCIRST        = 1;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFIFORESET   = 1;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFFIL        = 0x00;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFFIENA      = 0;
        (*SciReg[SCI_NO]).SCIFFTX.bit.TXFFINTCLR    = 1;

        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFIFORESET   = 1;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFIL        = 0x1F;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFIENA      = 0;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFINTCLR    = 1;
        (*SciReg[SCI_NO]).SCIFFRX.bit.RXFFOVRCLR    = 1; 
    }
}

void    sDrv_SetSci_TxData(unsigned int SCI_NO, unsigned int Data)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_TX_DATA(SCI_NO, Data);
}

unsigned int    sDrv_GetSci_RxData(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return 0;
    }

    return( sSCI_RX_DATA(SCI_NO) );
}

unsigned int    sDrv_GetSci_RxRdy(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return 0;
    }

    return(sSCI_RX_RDY_STATUS(SCI_NO));
}

unsigned int    sDrv_GetSci_TxRdy(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return 0;
    }

    return(sSCI_TX_RDY_STATUS(SCI_NO));
}

unsigned int    sDrv_GetSci_TxEmpty(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return 0;
    }

    return(sSCI_TX_EMPTY_STATUS(SCI_NO));
}

unsigned int    sDrv_GetSci_RxErr(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return 0;
    }

    return(sSCI_RX_ERR_STATUS(SCI_NO));
}

void    sDrv_SetSci_Rest_EN(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_Rest_EN(SCI_NO);
}

void    sDrv_SetSci_Rest_DIS(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_Rest_DIS(SCI_NO);
}

void    sDrv_SetSci_RxINT_EN(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_RX_INT_EN(SCI_NO);
}

void    sDrv_SetSci_RxINT_DIS(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_RX_INT_DIS(SCI_NO);
}

void    sDrv_SetSci_TxINT_EN(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_TX_INT_EN(SCI_NO);
}

void    sDrv_SetSci_TxINT_DIS(unsigned int SCI_NO)
{
    if (sSCI_NO_Check(SCI_NO) == false) 
    {
        return;
    }

    sSCI_TX_INT_DIS(SCI_NO);
}



