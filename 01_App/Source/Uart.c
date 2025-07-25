#include "UserHeader.h"

#pragma DATA_SECTION(t_UartSet,     ".UartSet_App");
#pragma DATA_SECTION(sTxTable,      ".UartTable_App");
#pragma DATA_SECTION(sSampleTable,  ".SampleTable_App");
#pragma DATA_SECTION(sVofaTable,    ".VofaTable_App");

static signed int   sGetNull    (void)
{
    return 0;
}

static signed int   sSample_GetNull    (ADC_Sample_e Goal)
{
    return 0;
}

// static signed long  sGet32Null  (void)
// {
//     return 0;
// }

typedef     signed int  (*pData)        (void);
volatile const pData sTxTable[DATA_TABLE_SIZE] = 
{
    sConfig_GetInvSet,      sConfig_GetPfcSet,      sGetNull,       sGetNull,
    sGetNull,               sGetNull,               sGetNull,       sGetNull,
    sGetNull,               sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,

    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull,              sGetNull,      sGetNull,
    sGetNull,             sGetNull
};

typedef     signed int  (*pSampleData)  (ADC_Sample_e);
volatile const pSampleData sSampleTable[cSAMPLE_FUNCTION_NUM] = 
{
    sAdc_Geti16Real,    sSample_GetRms,     sSample_GetNull,        sSample_GetNull,
    sSample_GetNull,    sSample_GetNull,    sSample_GetNull,        sSample_GetNull
};

typedef     float (*pVofaData)  (void);
volatile const pVofaData sVofaTable[cVOFA_CHANEL_NUM] = 
{
    sInv_GetInvDLoopRef,    sInv_GetInvDLoopFed,     sInv_GetInvDLoopErr,        sInv_GetInvDLoopOut
};

typedef struct
{
    unsigned int Cmd;  // 0x00
    unsigned int Control;
    unsigned int InvVolt;
    unsigned int InvFreq;
    unsigned int ChgVolt;
    unsigned int ChgCurr;
    unsigned int ChgPower;
    unsigned int GridCurr;
    unsigned int GridPower;
    unsigned int Rsvd09;
    unsigned int Rsvd0A;
    unsigned int Rsvd0B;
    unsigned int Rsvd0C;
    unsigned int Rsvd0D;
    unsigned int Rsvd0E;
    unsigned int Rsvd0F;
    unsigned int Rsvd10;
    unsigned int Rsvd11;
    unsigned int Rsvd12;
    unsigned int Rsvd13;
}UartSet_t;
volatile static UartSet_t t_UartSet;

void    sUart_DealData(void)
{
    unsigned int u16Cmd,u16Control;

    asm(" SETC INTM ");

    u16Cmd              = t_UartSet.Cmd;
    u16Control          = t_UartSet.Control;
    t_UartSet.Cmd       = 0;
    t_UartSet.Control   = 0;

    asm(" CLRC INTM ");

    if( u16Cmd == eRxTx_RxCmd_Write )
    {
        sConfig_SetInvVolt(t_UartSet.InvVolt);
    }
    else if(u16Cmd == eRxTx_RxCmd_Control)
    {
        switch (u16Control) 
        {
            case eReg_Control_InvOn:        OSEventSend(cPrioMsw, eMsw_InvOn);          break;
            case eReg_Control_InvOff:       OSEventSend(cPrioMsw, eMsw_InvOff);         break;
            case eReg_Control_ChgOn:        OSEventSend(cPrioMsw, eMsw_ChgOn);          break;
            case eReg_Control_ChgOff:       OSEventSend(cPrioMsw, eMsw_ChgOff);         break;
            case eReg_Control_GridOn:       OSEventSend(cPrioMsw, eMsw_GridOn);         break;
            case eReg_Control_GridOff:      OSEventSend(cPrioMsw, eMsw_GridOff);        break;
            case eReg_Control_ClrFault:     OSEventSend(cPrioMsw, eMsw_ClrFault);       break;
            case eReg_Control_BootLoader:   OSEventSend(cPrioMsw, eMsw_Boot);           break;
            default: break;
        }
    }
}
