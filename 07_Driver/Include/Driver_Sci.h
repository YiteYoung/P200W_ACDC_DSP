#ifndef _DRIVER_SCI_H
#define _DRIVER_SCI_H

#include "f28x_project.h"

#ifdef DRIVER_SCI_FUNCTION_G
#define DRIVER_SCI_FUNCTION extern
#else
#define DRIVER_SCI_FUNCTION
#endif

#define cSCICLK 30000000

#define sSCI_NO_Check(x)        (((x) >= SCI_PORT_End ) ? 0 : 1)

#define cMAX_SCI_NO             SCI_PORT_End

#define cSCI_FIFO_DEPTH         16

#define sSCI_Rest_EN(x)         { (*SciReg[(x)]).SCICTL1.bit.SWRESET    = false;    }
#define sSCI_Rest_DIS(x)        { (*SciReg[(x)]).SCICTL1.bit.SWRESET    = true;     }

#define sSCI_RX_EN(x)           { (*SciReg[(x)]).SCICTL1.bit.RXENA      = true;     }
#define sSCI_RX_DIS(x)          { (*SciReg[(x)]).SCICTL1.bit.RXENA      = false;    }

#define sSCI_TX_EN(x)           { (*SciReg[(x)]).SCICTL1.bit.TXENA      = true;     }
#define sSCI_TX_DIS(x)          { (*SciReg[(x)]).SCICTL1.bit.TXENA      = false;    }

#define sSCI_RX_INT_EN(x)       { (*SciReg[(x)]).SCICTL2.bit.RXBKINTENA = true;     }
#define sSCI_RX_INT_DIS(x)      { (*SciReg[(x)]).SCICTL2.bit.RXBKINTENA = false;    }

#define sSCI_TX_INT_EN(x)       { (*SciReg[(x)]).SCICTL2.bit.TXINTENA   = true;     }
#define sSCI_TX_INT_DIS(x)      { (*SciReg[(x)]).SCICTL2.bit.TXINTENA   = false;    }

#define sSCI_FIFORX_INT_EN(x)   { (*SciReg[(x)]).SCIFFRX.bit.RXFFIENA   = true;     }
#define sSCI_FIFORX_INT_DIS(x)  { (*SciReg[(x)]).SCIFFRX.bit.RXFFIENA   = false;    }

#define sSCI_FIFOTX_INT_EN(x)   { (*SciReg[(x)]).SCIFFTX.bit.TXFFIENA   = true;     }
#define sSCI_FIFOTX_INT_DIS(x)  { (*SciReg[(x)]).SCIFFTX.bit.TXFFIENA   = false;    }

#define sSCI_LOOPBK_EN(x)       { (*SciReg[(x)]).SCICCR.bit.LOOPBKENA   = true;     }
#define sSCI_LOOPBK_DIS(x)      { (*SciReg[(x)]).SCICCR.bit.LOOPBKENA   = false;    }

#define sSCI_CLR_FIFO_RX_OV(x)  { (*SciReg[(x)]).SCIFFRX.bit.RXFFOVRCLR = true;     }

#define sSCI_CLR_FIFO_RX_INT(x) { (*SciReg[(x)]).SCIFFRX.bit.RXFFINTCLR = true;     }
#define sSCI_CLR_FIFO_TX_INT(x) { (*SciReg[(x)]).SCIFFTX.bit.TXFFINTCLR = true;     }

#define sSCI_Rest_RXFIFO_EN(x)  { (*SciReg[(x)]).SCIFFRX.bit.RXFIFORESET = false;   }
#define sSCI_Rest_RXFIFO_DIS(x) { (*SciReg[(x)]).SCIFFRX.bit.RXFIFORESET = true;    }

#define sSCI_Rest_TXFIFO_EN(x)  { (*SciReg[(x)]).SCIFFTX.bit.TXFIFORESET = false;   }
#define sSCI_Rest_TXFIFO_DIS(x) { (*SciReg[(x)]).SCIFFTX.bit.TXFIFORESET = true;    }

#define sSCI_TX_EMPTY_STATUS(x)   (*SciReg[(x)]).SCICTL2.bit.TXEMPTY                

#define sSCI_RX_ERR_STATUS(x)     (*SciReg[(x)]).SCIRXST.bit.RXERROR                

#define sSCI_RX_RDY_STATUS(x)     (*SciReg[(x)]).SCIRXST.bit.RXRDY                  
#define sSCI_TX_RDY_STATUS(x)     (*SciReg[(x)]).SCICTL2.bit.TXRDY                  

#define sSCI_RX_FFST_STATUS(x)    (*SciReg[(x)]).SCIFFRX.bit.RXFFST                 
#define sSCI_TX_FFST_STATUS(x)    (*SciReg[(x)]).SCIFFTX.bit.TXFFST                 

#define sSCI_RX_DATA(x)           (*SciReg[(x)]).SCIRXBUF.bit.SAR                   
#define sSCI_TX_DATA(x,D)       { (*SciReg[(x)]).SCITXBUF.bit.TXDT       = (D);     }

typedef enum
{
    SCI_PORT_A = 0,
    SCI_PORT_B,
    SCI_PPRT_C,

    SCI_PORT_End
}SciPort_e;

typedef enum 
{
    eSCI_BAUD_2400 = 0,
    eSCI_BAUD_4800,
    eSCI_BAUD_9600,
    eSCI_BAUD_14400,
    eSCI_BAUD_19200,
    eSCI_BAUD_38400,
    eSCI_BAUD_57600,
    eSCI_BAUD_115200,

    eSCI_BAUD_End
}SciBaud_e;

typedef enum
{
    eSCI_Stop_One = 0,
    eSCI_Stop_Two,

    eSCI_Stop_End
}SicStop_e;

typedef enum
{
    eSCI_Char_Default = 0,
    eSCI_Char_One,
    eSCI_Char_Two,
    eSCI_Char_Three,
    eSCI_Char_Four,
    eSCI_Char_Five,
    eSCI_Char_Six,
    eSCI_Char_Seven,
    eSCI_Char_Eight,

    eSCI_Char_End,
}SciChar_e;

typedef enum
{
    eSCI_FIFO_DIS = 0,
    eSCI_FIFO_EN,

    eSCI_FIFO_End
}SciFIFOMode_e;

typedef enum
{
    eSCI_Parity_None = 0,
    eSCI_Parity_ODD,
    eSCI_Parity_Even,

    eSCI_Parity_End
}SciParity_e;

DRIVER_SCI_FUNCTION     void            sDrv_SetSci             (unsigned int SCI_NO, unsigned int BaudRate, unsigned int CharSize, unsigned int Parity, unsigned int StopBit, signed int FIFOMode);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_Baud        (unsigned int SCI_NO, unsigned int BaudRate);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_Char        (unsigned int SCI_NO, unsigned int CharSize);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_Pary        (unsigned int SCI_NO, unsigned int Parity);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_Stop        (unsigned int SCI_NO, unsigned int StopBit);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_FIFO        (unsigned int SCI_NO, unsigned int FIFOMode);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_TxData      (unsigned int SCI_NO, unsigned int Data);
DRIVER_SCI_FUNCTION     unsigned int    sDrv_GetSci_RxData      (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     unsigned int    sDrv_GetSci_RxRdy       (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     unsigned int    sDrv_GetSci_TxRdy       (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     unsigned int    sDrv_GetSci_TxEmpty     (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     unsigned int    sDrv_GetSci_RxErr       (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_Rest_EN     (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_Rest_DIS    (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_RxINT_EN    (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_RxINT_DIS   (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_TxINT_EN    (unsigned int SCI_NO);
DRIVER_SCI_FUNCTION     void            sDrv_SetSci_TxINT_DIS   (unsigned int SCI_NO);    

#endif
