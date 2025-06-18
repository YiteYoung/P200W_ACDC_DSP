#ifndef _BSP_SCI_H
#define _BSP_SCI_H

#include "Driver_Sci.h"

#ifdef BSP_SCI_FUNCTION_G
#define BSP_SCI_FUNCTION extern
#else
#define BSP_SCI_FUNCTION
#endif

#define cRx_MaxBuff_Num     150

typedef enum
{
    eQBuf_Normal = 0,
    eQBuf_Full,
    eQBuf_Empty,
    eQBuf_Status_END
}SciQueuStatus_e;

typedef enum
{
    eSci_Ready = 0,
    eSciRx_Busy,
    eSciRx_Empty,
    eSciTx_Busy,
    eSci_IDErr,
    eSciRxTx_Status_END
}SciRxTxStatus_e;


BSP_SCI_FUNCTION   void             sInitSci            (void);
BSP_SCI_FUNCTION    void            sSciRest            (unsigned int SCI_NO);
BSP_SCI_FUNCTION    unsigned char   sSCiRxISR           (unsigned int SCI_NO, unsigned char *RxData);
BSP_SCI_FUNCTION    void            sSciTxISR           (unsigned int SCI_NO);
BSP_SCI_FUNCTION    unsigned char   sSciRead            (unsigned int SCI_NO, unsigned char *pBuff);
BSP_SCI_FUNCTION    unsigned char   sSciWrite           (unsigned int SCI_NO, unsigned char *pStart, unsigned int TxLen);
BSP_SCI_FUNCTION    void            sSciDeathAutoRest   (unsigned int SCI_NO,unsigned int GATTime, unsigned int RestTime);
BSP_SCI_FUNCTION    unsigned char   sGetSciDeathFlag    (unsigned int SCI_NO);
BSP_SCI_FUNCTION    void            sClrSciDeathFlag    (unsigned int SCI_NO);
BSP_SCI_FUNCTION    unsigned char   sGetSciFrameStart   (unsigned int SCI_NO);
BSP_SCI_FUNCTION    void            sClrSciFrameStart   (unsigned int SCI_NO);
BSP_SCI_FUNCTION    unsigned char   sGetSciFrameStop    (unsigned int SCI_NO);
BSP_SCI_FUNCTION    void            sClrSciFrameStop    (unsigned int SCI_NO);

#endif
