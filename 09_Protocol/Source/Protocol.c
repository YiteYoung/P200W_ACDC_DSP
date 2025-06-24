#include "Protocol.h"

typedef struct
{
    unsigned int Address_Test;  // 0x00
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
}RxTx_Set_t;

typedef                 signed int      (*pRxTxData)(void);

static                  RxTx_Set_t      t_Set;
volatile static const   pRxTxData       sTxTable[DATA_TABLE_SIZE];
static                  RxTx_t          t_RxTx;


void                    sSci_RxDeal     (unsigned char SCI_NO, unsigned char RxData, volatile RxTx_t *pRxTx);
void                    sSci_TxDeal     (unsigned char SCI_NO, volatile RxTx_t *pRxTx);

void                    sSci_GetData    (volatile RxTx_t *pRxTx);
void                    sSci_SetData    (volatile RxTx_t *pRxTx);
void                    sSci_UpGrade    (volatile RxTx_t *pRxTx);

static void             sAdd_Tx_Head    (volatile RxTx_t *pRxTx, RxTx_RxCmd_e eTxCmd);
static void             sAdd_Crc        (volatile RxTx_t *pRxTx);

static void             sRx_DataPack    (volatile RxTx_t *pRxTx,unsigned int RxData, unsigned short DataFirst);
static unsigned char    sRx_WordData    (volatile RxTx_t *pRxTx,unsigned int RxData, volatile unsigned int *pData);

void    sInitProtocol(unsigned char SCI_NO, unsigned short bTxClrEn)
{
    unsigned int    i;

    volatile RxTx_t *pRxTx;

    pRxTx = &t_RxTx;

    pRxTx->RxTx_Status                  = eRxTx_Start;

    pRxTx->RxTx_ErrCode                 = eRxTx_Err_None;

    pRxTx->RxTx_Flag.bit.RxStart        = false;
    pRxTx->RxTx_Flag.bit.RxFrameOk      = false;
    pRxTx->RxTx_Flag.bit.TxPackOk       = false;
    pRxTx->RxTx_Flag.bit.WordPackOk     = false;
    pRxTx->RxTx_Flag.bit.CrcCalEn       = false;

    pRxTx->u16WordPart                  = DATA_PART_LO;
    pRxTx->u16WordNum                   = 0;
    pRxTx->u16ThisWord                  = 0;
    pRxTx->u16AscByteLen                = 0;

    pRxTx->CrcLo                        = 0xFF;
    pRxTx->CrcHi                        = 0xFF;
    pRxTx->u16RxBuffLen                 = 0;
    pRxTx->u16CheckCalValue             = 0;

    pRxTx->t_RxData.u16Len              = 0;
    pRxTx->t_RxData.u16Cmd              = 0;
    pRxTx->t_RxData.i16Reg              = 0;
    pRxTx->t_RxData.u16RegLen           = 0;
    pRxTx->t_RxData.u16RegByteLen       = 0;
    pRxTx->t_RxData.u16DataCheck        = 0;

    for (i = 0; i < SOI_BYTE_NUM; i++)
    {
        pRxTx->t_RxData.u16Soi[i]       = 0;
    }

    for (i = 0; i < DATA_INFO_MAX; i++) 
    {
        pRxTx->t_RxData.i16DataInfo[i]  = 0;
    }

    if(bTxClrEn == true)
    {
        pRxTx->u16TxBuffLen             = 0;
        pRxTx->u16TxByteLen             = 0;
        pRxTx->u16TxWordLen             = 0;

        for (i = 0; i < DATA_TX_BUFF_MAX; i++) 
        {
            pRxTx->u8TxBuff[i]          = 0;
        }
    }

}

void    sSci_RxTxDeal(unsigned char SCI_NO, unsigned char RxData)
{
    volatile RxTx_t *pRxTx;
    pRxTx = &t_RxTx;

    if(pRxTx->RxTx_Flag.bit.RxFrameOk == false)
    {
        sSci_RxDeal(SCI_NO,RxData,pRxTx);
    }
    else
    {
        sSci_TxDeal(SCI_NO,pRxTx);
    }

    if( pRxTx->RxTx_Flag.bit.TxPackOk == true)
    {
        sSciWrite(SCI_NO,(unsigned char *)(pRxTx->u8TxBuff), pRxTx->u16TxBuffLen);
    }
}

void    sSci_RxDeal(unsigned char SCI_NO, unsigned char RxData, volatile RxTx_t *pRxTx)
{
    unsigned LastReg;

    // 等待前帧完成，不解析新帧
    if(pRxTx->RxTx_Flag.bit.RxFrameOk == true)
    {
        return;
    }

    // 判断起始字节
    if( pRxTx->RxTx_Flag.bit.RxStart == false && sGetSciFrameStart(SCI_NO) == true )
    {
        if( RxData == RX_HEAD_A)
        {
            pRxTx->u16AscByteLen            = SOI_BYTE_NUM;
            pRxTx->u16WordNum               = 0;
            pRxTx->u16WordPart              = DATA_PART_ALL_LO;
            pRxTx->u16ThisWord              = 0x0000;
            pRxTx->RxTx_Flag.bit.WordPackOk = false;
            pRxTx->RxTx_Flag.bit.RxStart    = true;
            pRxTx->RxTx_Status              = eRxTx_Soi;
            pRxTx->RxTx_ErrCode             = eRxTx_Err_None;

            pRxTx->CrcLo                    = 0xFF;
            pRxTx->CrcHi                    = 0xFF;
            pRxTx->u16RxBuffLen             = 0;
            pRxTx->u16CheckCalValue         = 0;
            pRxTx->RxTx_Flag.bit.CrcCalEn   = true;
        }
    }

    // 接收到帧头，开始计算CRC
    if(pRxTx->RxTx_Flag.bit.RxStart == true)
    {
        pRxTx->u16RxBuffLen++;
        if(pRxTx->u16TxBuffLen >= DATA_RX_BUFF_MAX)
        {
            pRxTx->u16RxBuffLen = DATA_RX_BUFF_MAX;
            pRxTx->RxTx_Status  = eRxTx_Err;
            pRxTx->RxTx_ErrCode = eRxTx_Err_Len;
        }
        else
        {
            if(pRxTx->RxTx_Flag.bit.CrcCalEn == true)
            {
                pRxTx->u16CheckCalValue = sModOneByOneCrc16((unsigned int *)&pRxTx->CrcLo,(unsigned char *)&pRxTx->CrcHi,RxData,CRC_Hi);
            }
            else
            {
                pRxTx->CrcLo    = 0xFF;
                pRxTx->CrcHi    = 0xFF;
            }
        }
    }

    // 解析协议数据
    switch (pRxTx->RxTx_Status) 
    {
        case eRxTx_Soi:
            if(pRxTx->u16WordNum >= SOI_BYTE_NUM - 1)
            {
                pRxTx->u16WordNum = SOI_BYTE_NUM - 1;
            }

            if (sRx_WordData(pRxTx, RxData, &pRxTx->t_RxData.u16Soi[pRxTx->u16WordNum]) == true) 
            {
                pRxTx->u16WordNum++;
            }
            break;

        case eRxTx_Len:
            sRx_WordData(pRxTx, RxData, &pRxTx->t_RxData.u16Len);
            break;

        case eRxTx_Cmd:
            sRx_WordData(pRxTx, RxData, &pRxTx->t_RxData.u16Cmd);
            break;

        case eRxTx_Reg:
            sRx_WordData(pRxTx, RxData, (volatile unsigned int *)&pRxTx->t_RxData.i16Reg);
            break;

        case eRxTx_RegLen:
            if(sRx_WordData(pRxTx, RxData, &pRxTx->t_RxData.u16RegByteLen) == true)
            {
                pRxTx->t_RxData.u16RegLen   = pRxTx->t_RxData.u16RegByteLen >> 1;
            }
            break;

        case eRxTx_Data:
            if(pRxTx->u16WordNum >= (DATA_INFO_MAX - 1))
            {
                pRxTx->u16WordNum = DATA_INFO_MAX - 1;
            }

            if(sRx_WordData(pRxTx, RxData, (volatile unsigned int *)&pRxTx->t_RxData.u16Soi[pRxTx->u16WordNum]) == true)
            {
                pRxTx->u16WordNum++;
            }
            break;

        case eRxTx_Check:
            sRx_WordData(pRxTx, RxData, &pRxTx->t_RxData.u16DataCheck);
            break;

        case eRxTx_Eoi:
            sInitProtocol(SCI_NO, false);
            break;

        case eRxTx_Err:
            sInitProtocol(SCI_NO, false);
            break;
            
        default:
            sInitProtocol(SCI_NO, false);
            return;     
    }

    // 单个状态解析完成
    if(pRxTx->u16AscByteLen <= 0)
    {
        pRxTx->u16WordNum               = 0;
        pRxTx->u16ThisWord              = 0x0000;
        pRxTx->RxTx_Flag.bit.WordPackOk = false;
    }
    else
    {
        switch (pRxTx->RxTx_Status) 
        {
            case eRxTx_Soi:
                if( pRxTx->t_RxData.u16Soi[0] == RX_HEAD_A &&\
                    pRxTx->t_RxData.u16Soi[1] == RX_HEAD_B  )
                {
                    pRxTx->u16AscByteLen    = LEN_BYTE_NUM;
                    pRxTx->u16WordPart      = DATA_PART_LO;
                    pRxTx->RxTx_Status      = eRxTx_Len;
                }
                else
                {
                    pRxTx->RxTx_Status      = eRxTx_Err;
                    pRxTx->RxTx_ErrCode     = eRxTx_Err_Soi;
                }
                break;

            case eRxTx_Len:
                if(pRxTx->t_RxData.u16Len >= (CMD_BYTE_NUM + CRC_BYTE_NUM))
                {
                    pRxTx->u16AscByteLen    = CMD_BYTE_NUM;
                    pRxTx->u16WordPart      = DATA_PART_LO;
                    pRxTx->RxTx_Status      = eRxTx_Cmd;
                }
                else
                {
                    pRxTx->RxTx_Status      = eRxTx_Err;
                    pRxTx->RxTx_ErrCode     = eRxTx_Err_Len;
                }
                break;

            case eRxTx_Cmd:
                if( pRxTx->t_RxData.u16Cmd > eRxTx_RxCmd_None && pRxTx->t_RxData.u16Cmd < eRxTx_RxCmd_End )
                {
                    pRxTx->u16AscByteLen    = REG_BYTE_NUM;
                    pRxTx->u16WordPart      = DATA_PART_LO;
                    pRxTx->RxTx_Status      = eRxTx_Reg;
                }
                else
                {
                    pRxTx->RxTx_Status      = eRxTx_Err;
                    pRxTx->RxTx_ErrCode     = eRxTx_Err_Cmd;
                }

            case eRxTx_Reg:
                if(pRxTx->t_RxData.u16Cmd == eRxTx_RxCmd_Write)
                {
                    if(cSet_CMD_REG_ADDR_OK(pRxTx->t_RxData.i16Reg))
                    {
                        pRxTx->u16AscByteLen    = RLEN_BYTE_NUM;
                        pRxTx->u16WordPart      = DATA_PART_LO;
                        pRxTx->RxTx_Status      = eRxTx_RegLen;
                    }
                    else
                    {
                        pRxTx->RxTx_Status      = eRxTx_Err;
                        pRxTx->RxTx_ErrCode     = eRxTx_Err_Reg;
                    }
                }
                else if(pRxTx->t_RxData.u16Cmd == eRxTx_RxCmd_Read)
                {
                    if(cGet_CMD_REG_ADDR_OK(pRxTx->t_RxData.i16Reg))
                    {
                        pRxTx->u16AscByteLen    = RLEN_BYTE_NUM;
                        pRxTx->u16WordPart      = DATA_PART_LO;
                        pRxTx->RxTx_Status      = eRxTx_RegLen;
                    }
                    else
                    {
                        pRxTx->RxTx_Status      = eRxTx_Err;
                        pRxTx->RxTx_ErrCode     = eRxTx_Err_Reg;
                    }
                }
                else
                {
                    pRxTx->u16AscByteLen    = RLEN_BYTE_NUM;
                    pRxTx->u16WordPart      = DATA_PART_LO;
                    pRxTx->RxTx_Status      = eRxTx_RegLen; 
                }
                break;

            case eRxTx_RegLen:
                LastReg = pRxTx->t_RxData.i16Reg + pRxTx->t_RxData.u16RegLen - 1;
                if( pRxTx->t_RxData.u16Cmd == eRxTx_RxCmd_Write )
                {
                    if( pRxTx->t_RxData.u16RegLen > 0                       &&\
                        pRxTx->t_RxData.u16RegLen <= MAX_SET_CMD_REG_LEN    &&\
                        LastReg < SET_CMD_REG_END                           )
                    {
                        pRxTx->u16AscByteLen = pRxTx->t_RxData.u16RegLen * 2;
                        pRxTx->u16WordPart   = DATA_PART_LO;
                        pRxTx->RxTx_Status   = eRxTx_Data;
                    }
                    else
                    {
                        pRxTx->RxTx_Status   = eRxTx_Err;
                        pRxTx->RxTx_ErrCode  = eRxTx_Err_RegLen;
                    }
                }
                else if( pRxTx->t_RxData.u16Cmd == eRxTx_RxCmd_Read )
                {
                    if( pRxTx->t_RxData.u16RegLen > 0                       &&\
                        pRxTx->t_RxData.u16RegLen <= MAX_GET_CMD_REG_LEN    &&\
                        LastReg < GET_CMD_REG_END                           )
                    {
                        pRxTx->RxTx_Flag.bit.CrcCalEn   = false;

                        pRxTx->u16AscByteLen            = CRC_BYTE_NUM;
                        pRxTx->u16WordPart              = DATA_PART_LO;
                        pRxTx->RxTx_Status              = eRxTx_Check;
                    }
                    else
                    {
                        pRxTx->RxTx_Status              = eRxTx_Err;
                        pRxTx->RxTx_ErrCode             = eRxTx_Err_RegLen;
                    }
                }
                else
                {
                    pRxTx->u16AscByteLen                = pRxTx->t_RxData.u16RegLen * 2;
                    pRxTx->u16WordPart                  = DATA_PART_LO;
                    pRxTx->RxTx_Status                  = eRxTx_Data;
                }
                break;


            case eRxTx_Data:
                pRxTx->RxTx_Flag.bit.CrcCalEn           = false;

                pRxTx->u16AscByteLen                    = CRC_BYTE_NUM;
                pRxTx->u16WordPart                      = DATA_PART_LO;
                pRxTx->RxTx_Status                      = eRxTx_Check;
                break;

            case eRxTx_Check:
                if(pRxTx->u16CheckCalValue == pRxTx->t_RxData.u16DataCheck)
                {
                    pRxTx->u16WordNum                   = 0;
                    pRxTx->u16AscByteLen                = 0;
                    pRxTx->RxTx_Flag.bit.RxStart        = false;
                    pRxTx->RxTx_Status                  = eRxTx_Eoi;
                }
                else
                {
                    pRxTx->RxTx_Status                  = eRxTx_Err;
                }
                break;

            case eRxTx_Eoi:
                sInitProtocol(SCI_NO, false);
                break;

            case eRxTx_Err:
                sInitProtocol(SCI_NO, false);
                break;

            default:
                sInitProtocol(SCI_NO, false);
        }
    }

    if(pRxTx->RxTx_Status == eRxTx_Err )
    {
        sInitProtocol(SCI_NO, false);
        return;
    }

    if(pRxTx->RxTx_Status == eRxTx_Eoi)
    {
        pRxTx->RxTx_Flag.bit.RxFrameOk = true;
        return;
    }
}

void    sSci_TxDeal(unsigned char SCI_NO, volatile RxTx_t *pRxTx)
{
    if(pRxTx->RxTx_Flag.bit.RxFrameOk == false)
    {
        return;
    }

    switch (pRxTx->t_RxData.u16Cmd) 
    {
        case eRxTx_RxCmd_Read:
            sSci_GetData(pRxTx);
            break;

        case eRxTx_RxCmd_Write:
            sSci_SetData(pRxTx);
            break;

        case eRxTx_RxCmd_UpGrade:
            sSci_UpGrade(pRxTx);
            break;

        case eRxTx_RxCmd_Reply:
            break;

        default :
            pRxTx->u16TxBuffLen = 0;
            break;
    }

    if( pRxTx->u16TxBuffLen > 0 && pRxTx->u16TxBuffLen <= DATA_TX_BUFF_MAX - 2)\
    {
        sAdd_Crc(pRxTx);

        pRxTx->RxTx_Flag.bit.TxPackOk = true;
    }   
    else 
    {
        pRxTx->RxTx_Flag.bit.TxPackOk = false;
    }

    // 打包成功均需初始化(含失败)
    if(pRxTx->RxTx_Flag.bit.TxPackOk == true)
    {
        sInitProtocol(SCI_NO, false);
        pRxTx->RxTx_Flag.bit.TxPackOk = true;           // 必须置位，sInit会清掉该标志位
    }
    else 
    {
        sInitProtocol(SCI_NO, true);
    }
}

void    sSci_GetData(volatile RxTx_t *pRxTx)
{
    unsigned int i,index,u16Data;

    pRxTx->u16TxBuffLen = 0;

    if(pRxTx->t_RxData.i16Reg < GET_CMD_REG_START || pRxTx->t_RxData.i16Reg >= GET_CMD_REG_END)
    {
        return;
    }

    sAdd_Tx_Head(pRxTx, eRxTx_RxCmd_Read);

    if( pRxTx->RxTx_ErrCode == eRxTx_Err_Check)
    {
        u16Data = eRXTX_ANS_NG;
        pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data      ) & 0x00FF);
        pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data >> 8 ) & 0x00FF);
        return;
    }

    for (i = 0; i < pRxTx->t_RxData.u16RegLen; i++) 
    {
        index = pRxTx->t_RxData.i16Reg + i - GET_CMD_REG_START;
        if( index >= MAX_GET_CMD_REG_LEN)
        {
            index = MAX_GET_CMD_REG_LEN - 1;
        }

        u16Data = sTxTable[index]();
        pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data      ) & 0x00FF);
        pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data >> 8 ) & 0x00FF);
    }
}

void    sSci_SetData(volatile RxTx_t *pRxTx)
{
    unsigned int    i,index,u16Data;
    unsigned int    *pData;
    int             SetValue;

    pRxTx->u16TxBuffLen = 0;

    if(pRxTx->t_RxData.i16Reg < SET_CMD_REG_START || pRxTx->t_RxData.i16Reg >= SET_CMD_REG_END)
    {
        return;
    }

    for (i = 0; i < pRxTx->t_RxData.u16RegLen; i++)
    {
        index = pRxTx->t_RxData.i16Reg + i;
        if( index >= SET_CMD_REG_END)
        {
            index = SET_CMD_REG_END - 1;
        }

        if(index == eReg_SetData_01)
        {
            SetValue = pRxTx->t_RxData.i16DataInfo[i];
            if( SetValue <= eRxTx_Control_None || SetValue >= eRxTx_Control_End )
            {
                t_Set.Control = eRxTx_Control_None;
            }
            else
            {
                t_Set.Control = SetValue;
            }
        }
        else 
        {
            t_Set.Control = eRxTx_Control_None;

            pData = &t_Set.InvVolt + i;
            *pData = (unsigned int)pRxTx->t_RxData.i16DataInfo[i];
        }
    }

    sAdd_Tx_Head(pRxTx, eRxTx_RxCmd_Write);

    u16Data = eRxTx_ANS_OK;
    if(pRxTx->RxTx_ErrCode == eRxTx_Err_Check)
    {
        u16Data = eRXTX_ANS_NG;
    }
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data      ) & 0x00FF);
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data >> 8 ) & 0x00FF);
}

void    sSci_UpGrade(volatile RxTx_t *pRxTx)
{
    unsigned int    u16Data;

    pRxTx->u16TxBuffLen = 0;

    if(pRxTx->t_RxData.i16Reg != 0x0A || pRxTx->t_RxData.u16RegLen != 1)
    {
        return;
    }

    u16Data = pRxTx->t_RxData.i16DataInfo[0];
    if(u16Data == 0x55AA)
    {
        t_Set.Control = eRxTx_Control_BootLoader;
    }
    else 
    {
        return;
    }

    sAdd_Tx_Head(pRxTx, eRxTx_RxCmd_UpGrade);

    u16Data = eRxTx_ANS_OK;
    if(pRxTx->RxTx_ErrCode == eRxTx_Err_Check)
    {
        u16Data = eRXTX_ANS_NG;
    }
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data      ) & 0x00FF);
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  = (unsigned char)((u16Data >> 8 ) & 0x00FF);
}

void    sAdd_Tx_Head(volatile RxTx_t *pRxTx,RxTx_RxCmd_e eTxCmd)
{
    unsigned int    u16Data;

    // 帧头
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   TX_HEAD_A;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   TX_HEAD_B;

    // Len
    u16Data = 0;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data       ) & 0x00FF;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data >>  8 ) & 0x00FF;

    // Cmd
    u16Data = eTxCmd;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data       ) & 0x00FF;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data >>  8 ) & 0x00FF;

    // Reg
    u16Data = pRxTx->t_RxData.i16Reg;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data       ) & 0x00FF;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data >>  8 ) & 0x00FF;

    // Reg Len
    u16Data = pRxTx->t_RxData.u16RegLen << 1;
    if(pRxTx->RxTx_ErrCode == eRxTx_Err_Check)
    {
        u16Data = 2;
    }
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data       ) & 0x00FF;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data >>  8 ) & 0x00FF;
}

void    sAdd_Crc(volatile RxTx_t *pRxTx)
{
    unsigned int u16Data;

    if(pRxTx->u16TxBuffLen > (LEN_BYTE_ID + LEN_BYTE_NUM))
    {
        u16Data = pRxTx->u16TxBuffLen - LEN_BYTE_ID - LEN_BYTE_NUM;
    }
    else
    {
        u16Data = 0;
    }
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data       ) & 0x00FF;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data >>  8 ) & 0x00FF;

    u16Data = sModByteCrc16((unsigned int *)pRxTx->u8TxBuff, pRxTx->u16TxBuffLen,CRC_Hi);
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data       ) & 0x00FF;
    pRxTx->u8TxBuff[pRxTx->u16TxBuffLen++]  =   (unsigned char)(u16Data >>  8 ) & 0x00FF;
}

static void sRx_DataPack(volatile RxTx_t *pRxTx,unsigned int RxData, unsigned short DataFirst)
{
    unsigned int rxHex;

    rxHex = RxData;

    switch (pRxTx->u16WordPart) 
    {
        case DATA_PART_HI:
            pRxTx->u16ThisWord      |=  (rxHex << 8);
            pRxTx->u16WordPart      =   DATA_PART_LO;
            if(DataFirst == DATA_FIRST_LO)
            {
                pRxTx->RxTx_Flag.bit.WordPackOk = true;
            }
            break;

        case DATA_PART_LO:
            pRxTx->u16ThisWord      |=  (rxHex << 8);
            pRxTx->u16WordPart      =   DATA_PART_HI;
            if(DataFirst == DATA_FIRST_HI)
            {
                pRxTx->RxTx_Flag.bit.WordPackOk = true;
            }
            break;

        case DATA_PART_ALL_LO:
            pRxTx->u16ThisWord      |=  (rxHex << 8);
            pRxTx->u16WordPart      =   DATA_PART_ALL_LO;
            pRxTx->RxTx_Flag.bit.WordPackOk = true;
            break;

        default:
            break;
    }

    if(pRxTx->u16AscByteLen <= 0)
    {
        pRxTx->RxTx_Flag.bit.WordPackOk = true;
    }
}

static unsigned char    sRx_WordData(volatile RxTx_t *pRxTx,unsigned int RxData, volatile unsigned int *pData)
{
    sRx_DataPack(pRxTx, RxData, RX_DATA_PART_TYPE);

    if(pRxTx->RxTx_Flag.bit.WordPackOk == true)
    {
        *pData                          = pRxTx->u16ThisWord;
        pRxTx->u16ThisWord              = 0x0000;
        pRxTx->RxTx_Flag.bit.WordPackOk = false;
        return true;
    }

    return false;
}
