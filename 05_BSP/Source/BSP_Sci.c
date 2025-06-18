#include "BSP_Sci.h"

typedef struct{
    unsigned char   *pIn;
    unsigned char   *pOut;
    unsigned char   *pStart;
    unsigned int    Length;
    unsigned int    Size;
}QUEUE;

typedef struct{
    unsigned char   TxStatus;
    unsigned int    TxLength;
    unsigned char   *pbTx;
    QUEUE           *pqRx;

    unsigned char   RxSuccess;
    unsigned char   TxSuccess;
    unsigned char   TxLastData;
    unsigned char   FrameStat;
    unsigned char   FrameStop;
    unsigned char   RxBuff[cRx_MaxBuff_Num];

    unsigned char   SciDeath;
    unsigned int    RxDeathCnt;
    unsigned int    TxDeathCnt;

    unsigned int    FrameStartCnt;
}SciStruct;

static SciStruct    SciList[cMAX_SCI_NO];
static QUEUE        QList[cMAX_SCI_NO];

volatile struct SCI_REGS* const SciReg[SCI_PORT_End] = 
{
    &SciaRegs,
    &ScibRegs,
    &ScicRegs
};

void            sQSciInit   (unsigned int SCI_NO, unsigned int RxSize);
void            sQInit      (QUEUE *pq, unsigned char *pStart, unsigned int Size);
unsigned char   sQDataIn    (QUEUE *pq, unsigned char bData);
unsigned char   sQDataOut   (QUEUE *pq, unsigned char *pData);

void    sInitSci(void)
{
    sDrv_SetSci(SCI_PORT_A, eSCI_BAUD_4800, eSCI_Char_Eight, eSCI_Parity_None, eSCI_Stop_One, eSCI_FIFO_DIS);
    sDrv_SetSci(SCI_PORT_B, eSCI_BAUD_4800, eSCI_Char_Eight, eSCI_Parity_None, eSCI_Stop_One, eSCI_FIFO_DIS);

    sQSciInit(SCI_PORT_A,cRx_MaxBuff_Num);
    sQSciInit(SCI_PORT_B,cRx_MaxBuff_Num);
}

void    sSciRest(unsigned int SCI_NO)
{
    unsigned char bData;

    if( sSCI_NO_Check(SCI_NO) == false)
    {
        return;
    }

    sSCI_RX_DIS(SCI_NO);
    sSCI_TX_DIS(SCI_NO);

    sSCI_RX_INT_DIS(SCI_NO);
    sSCI_TX_INT_DIS(SCI_NO);

    bData = sSCI_RX_DATA(SCI_NO);
    bData += 0;

    sQSciInit(SCI_NO, cRx_MaxBuff_Num);

    sSCI_Rest_RXFIFO_EN(SCI_NO);
    sSCI_Rest_TXFIFO_EN(SCI_NO);
    asm(" RPT #7 || NOP ");
    sSCI_Rest_RXFIFO_DIS(SCI_NO);
    sSCI_Rest_TXFIFO_DIS(SCI_NO);

    sSCI_Rest_EN((SCI_NO));
    asm(" RPT #7 || NOP ");
    sSCI_Rest_DIS(SCI_NO);

    sSCI_RX_EN(SCI_NO);
    sSCI_TX_EN(SCI_NO);

    sSCI_RX_INT_EN(SCI_NO);
    sSCI_FIFORX_INT_EN(SCI_NO);
    sSCI_TX_INT_EN(SCI_NO);
    sSCI_FIFOTX_INT_EN(SCI_NO);
}

unsigned char sSCiRxISR(unsigned int SCI_NO, unsigned char *RxData)
{
    QUEUE           *pq;
    SciStruct       *pSci;
    unsigned char   bData,ByteNum;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return false;
    }

    pSci    = &SciList[SCI_NO];
    pq      = pSci->pqRx;

    ByteNum = sSCI_RX_FFST_STATUS(SCI_NO);

    sSCI_CLR_FIFO_RX_OV(SCI_NO);
    sSCI_CLR_FIFO_RX_INT(SCI_NO);

    if( sSCI_RX_RDY_STATUS(SCI_NO) == true || ByteNum > 0)
    {
        bData       = sSCI_RX_DATA(SCI_NO);
        (*RxData)   = bData;
        sQDataIn(pq, bData);
        return true;
    }
    else
    {
        return false;
    }

}

void    sSciTxISR(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return;
    }

    pSci = &SciList[SCI_NO];

    sSCI_CLR_FIFO_TX_INT(SCI_NO);

    if( sSCI_TX_RDY_STATUS(SCI_NO) == true )
    {
        if( pSci->TxLength == 0)
        {
            pSci->TxStatus = eSci_Ready;
            sSCI_TX_INT_DIS(SCI_NO);
            sSCI_FIFOTX_INT_DIS((SCI_NO));
        }
        else
        {
            sSCI_TX_DATA(SCI_NO,*(pSci->pbTx));
            (pSci->pbTx)++;
            (pSci->TxLength)--;
        }
    }
}

unsigned char sSciRead(unsigned int SCI_NO, unsigned char *pBuff)
{
    QUEUE           *pq;
    SciStruct       *pSci;
    unsigned char   Status;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return(eSci_IDErr);
    }

    pSci    = &SciList[SCI_NO];
    pq      = pSci->pqRx;

    asm(" SETC INTM ");
    Status  = sQDataOut( pq, pBuff);
    asm(" CLRC INTM ");

    if(Status == eQBuf_Empty)
    {
        return eSciRx_Empty;
    }
    else
    {
        pSci->RxSuccess = true;
        return eSci_Ready;
    }
}

unsigned char   sSciWrite(unsigned int SCI_NO, unsigned char *pStart, unsigned int TxLen)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return eSci_IDErr;
    }

    pSci = &SciList[SCI_NO];

    if( pSci->TxStatus == eSciTx_Busy )
    {
        return eSciTx_Busy;
    }

    asm(" SETC INTM ");
    pSci->pbTx      = pStart;
    pSci->TxLength  = TxLen;
    pSci->TxStatus  = eSciTx_Busy;
    pSci->TxSuccess = true;

    sSCI_TX_INT_EN(SCI_NO);
    sSCI_FIFOTX_INT_EN(SCI_NO);
    sSCI_TX_DATA(SCI_NO, *(pSci->pbTx));
    (pSci->pbTx)++;
    pSci->TxLength--;
    asm(" CLRC INITM ");

    return eSci_Ready;
}

void    sSciDeathAutoRest(unsigned int SCI_NO,unsigned int GATTime, unsigned int RestTime)
{
    SciStruct           *pSci;
    unsigned char       SciDeathAct;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return;
    }

    pSci = &SciList[SCI_NO];

    pSci->FrameStartCnt++;
    if( pSci->RxSuccess == true)
    {
        pSci->FrameStartCnt = 0;
        pSci->FrameStat     = false;
    }

    if(pSci->FrameStartCnt >= GATTime)
    {
        pSci->FrameStartCnt = 0;
        if( pSci->FrameStat == false)
        {
            pSci->FrameStat = true;
            pSci->FrameStop = true;
        }
    }

    pSci->RxDeathCnt++;
    pSci->TxDeathCnt++;

    if(pSci->RxSuccess == true)
    {
        pSci->RxSuccess     = false;
        pSci->RxDeathCnt    = 0;
    }

    if(pSci->TxSuccess == true)
    {
        pSci->TxSuccess = false;
        pSci->SciDeath  = false;

        pSci->TxDeathCnt    = 0;
    }

    if(pSci->RxDeathCnt >= RestTime)        pSci->RxDeathCnt = RestTime;
    if(pSci->TxDeathCnt >= RestTime)        pSci->TxDeathCnt = RestTime;

    if( pSci->RxDeathCnt >= RestTime        ||\
        pSci->TxDeathCnt >= RestTime        ||\
        sSCI_RX_ERR_STATUS(SCI_NO == true)  )
    {
        SciDeathAct = false;
        if( pSci->RxDeathCnt >= RestTime   ||
            pSci->TxDeathCnt >= RestTime   )
        {
            pSci->RxDeathCnt = 0;
            pSci->TxDeathCnt = 0;

            SciDeathAct      = true;
        }

        sSciRest(SCI_NO);

        if (SciDeathAct == true) 
        {
            pSci->SciDeath = true;
        }
    }

}

unsigned char sGetSciDeathFlag(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return false;
    }

    pSci = &SciList[SCI_NO];

    return( pSci->SciDeath );
}

void    sClrSciDeathFlag(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return;
    }

    pSci = &SciList[SCI_NO];

    pSci->SciDeath = false;
}

unsigned char sGetSciFrameStart(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return false;
    }

    pSci = &SciList[SCI_NO];

    return( pSci->FrameStat );
}

void sClrSciFrameStart(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return;
    }

    pSci = &SciList[SCI_NO];

    pSci->FrameStat = false;
}

unsigned char sGetSciFrameStop(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return false;
    }

    pSci = &SciList[SCI_NO];

    return pSci->FrameStop;
}

void    sClrSciFrameStop(unsigned int SCI_NO)
{
    SciStruct *pSci;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return;
    }

    pSci = &SciList[SCI_NO];

    pSci->FrameStop = false;
}

void sQSciInit(unsigned int SCI_NO, unsigned int RxSize)
{
    SciStruct   *pSci;
    QUEUE       *pq;

    if (sSCI_NO_Check(SCI_NO)) 
    {
        return;
    }

    pSci                = &SciList[SCI_NO];
    pSci->pqRx          = &QList[SCI_NO];
    pq                  = pSci->pqRx;

    sQInit(pq, (unsigned char *)(pSci->RxBuff), RxSize);

    pSci->TxStatus      = eSci_Ready;
    pSci->TxLength      = 0;

    pSci->RxSuccess     = false;
    pSci->TxSuccess     = false;
    pSci->TxLastData    = false;
    pSci->FrameStat     = false;
    pSci->FrameStop     = false;

    pSci->FrameStartCnt = 0;

    if(SCI_NO == SCI_PORT_A)
    {
        asm(" NOP ");
    }
}

void    sQInit(QUEUE *pq, unsigned char *pStart, unsigned int Size)
{
    pq->pIn     = pStart;
    pq->pOut    = pStart;
    pq->pStart  = pStart;
    pq->Length  = 0;
    pq->Size    = Size;
}

unsigned char sQDataIn(QUEUE *pq, unsigned char bData)
{
    if(pq->Length == pq->Size)
    {
        if( pq->pIn == pq->pStart)
        {
            *(pq->pStart + pq->Size - 1) = bData;
        }
        else
        {
            *(pq->pIn - 1) = bData;
        }

        return eQBuf_Full;
    }
    else
    {
        *(pq->pIn) = bData;
        pq->Length++;
        if(pq->pIn == (pq->pStart + pq->Size - 1))
        {
            pq->pIn = pq->pStart;
        }
        else
        {
            pq->pIn++;
        }

        return eQBuf_Normal;
    }
}

unsigned char sQDataOut(QUEUE *pq, unsigned char *pData)
{
    if(pq->Length == 0)
    {
        return eQBuf_Empty;
    }

    *pData = *(pq->pOut);
    pq->Length--;
    if(pq->pOut == (pq->pStart + pq->Size - 1))
    {
        pq->pOut = pq->pStart;
    }
    else
    {
        pq->pOut++;
    }

    return eQBuf_Normal;
}



