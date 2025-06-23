#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#ifdef PROTOCOL_FUNCTION_G
#define PROTOCOL_FUNCTION extern
#else
#define PROTOCOL_FUNCTION
#endif

#include    "BSP_Sci.h"
#include    "Crc16.h"

#undef      DATA_FIRST_LO
#define     DATA_FIRST_LO               (unsigned short)0

#undef      DATA_FIRST_HI
#define     DATA_FIRST_HI               (unsigned short)1

#undef      DATA_PART_LO
#define     DATA_PART_LO                0

#undef      DATA_PART_HI
#define     DATA_PART_HI                0

#undef      DATA_PART_ALL_LO
#define     DATA_PART_ALL_LO            0

#define     RX_HEAD_A                   0x55
#define     RX_HEAD_B                   0xAA

#define     TX_HEAD_A                   0xAA
#define     TX_HEAD_B                   0x55

// 高字节在前/低字节在前
#define     RX_DATA_PART_TYPE           DATA_FIRST_LO

// 保存数据长度定义
#define     DATA_INFO_MAX               20

// 接收数据长度定义
#define     DATA_RX_BUFF_MAX            100

// 发送数据长度定义
#define     DATA_TX_BUFF_MAX            180

// 字节长度
#define     SOI_BYTE_NUM                2
#define     LEN_BYTE_NUM                2
#define     CMD_BYTE_NUM                2
#define     REG_BYTE_NUM                2
#define     RLEN_BYTE_NUM               2
#define     CRC_BYTE_NUM                2

// 存储长度位置
#define     LEN_BYTE_ID                 ( SOI_BYTE_NUM )

// 起始地址
#define     SET_CMD_REG_START           eReg_SetData_Start     
#define     GET_CMD_REG_START           eReg_GetData_Start

// 结束地址
#define     SET_CMD_REG_END             eReg_SetData_END     
#define     GET_CMD_REG_END             eReg_GetData_END

// 地址最大长度
#define     MAX_SET_CMD_REG_LEN         ((unsigned int)SET_CMD_REG_END - SET_CMD_REG_START)
#define     MAX_GET_CMD_REG_LEN         ((unsigned int)GET_CMD_REG_END - GET_CMD_REG_START)

#define     cSet_CMD_REG_ADDR_OK(x)     ((x >= SET_CMD_REG_START) && (x < SET_CMD_REG_END))
#define     cGet_CMD_REG_ADDR_OK(x)     ((x >= GET_CMD_REG_START) && (x < GET_CMD_REG_END))

#define     DATA_TABLE_SIZE             50

typedef enum
{
    eReg_SetData_Start = 0x0000,
    eReg_SetData_01    = 0x0000,
    eReg_SetData_02,
    eReg_SetData_03,
    eReg_SetData_04,
    eReg_SetData_05,
    eReg_SetData_06,
    eReg_SetData_07,
    eReg_SetData_08,
    eReg_SetData_09,
    eReg_SetData_10,
    eReg_SetData_11,
    eReg_SetData_12,
    eReg_SetData_13,
    eReg_SetData_14,
    eReg_SetData_15,
    eReg_SetData_16,
    eReg_SetData_17,
    eReg_SetData_18,
    eReg_SetData_19,
    eReg_SetData_20,

    eReg_SetData_END,

    eReg_GetData_Start  = 0x0100,
    eReg_GetData_01     = 0x0100,
    eReg_GetData_02,
    eReg_GetData_03,
    eReg_GetData_04,
    eReg_GetData_05,
    eReg_GetData_06,
    eReg_GetData_07,
    eReg_GetData_08,
    eReg_GetData_09,
    eReg_GetData_10,
    eReg_GetData_11,
    eReg_GetData_12,
    eReg_GetData_13,
    eReg_GetData_14,
    eReg_GetData_15,
    eReg_GetData_16,
    eReg_GetData_17,
    eReg_GetData_18,
    eReg_GetData_19,
    eReg_GetData_20,

    eReg_GetData_END 
}Reg_Addr_e;

typedef enum
{
    eRxTx_Start     = 0,
    eRxTx_Soi,
    eRxTx_Len,
    eRxTx_Cmd,
    eRxTx_Reg,
    eRxTx_RegLen,
    eRxTx_Data,
    eRxTx_Check,
    eRxTx_Eoi,
    eRxTx_Err,

    eRxTx_End
}RxTx_State_e;

typedef enum
{
    eRxTx_Err_None      = 0,

    eRxTx_Err_Soi,
    eRxTx_Err_Len,
    eRxTx_Err_Cmd,
    eRxTx_Err_Reg,
    eRxTx_Err_RegLen,
    eRxTx_Err_Data,
    eRxTx_Err_Check,

    eRxTx_Err_End
}RxTx_Err_e ;

typedef enum
{
    eRxTx_RxCmd_None      = 0x00,

    eRxTx_RxCmd_Read,
    eRxTx_RxCmd_Write,
    eRxTx_RxCmd_UpGrade,
    eRxTx_RxCmd_Reply,

    eRxTx_RxCmd_End
}RxTx_RxCmd_e;

typedef enum
{
    eRxTx_Control_None      = 0x00,

    eRxTx_Control_Read,
    eRxTx_Control_Write,
    eRxTx_Control_InvOn,
    eRxTx_Control_InvOff,
    eRxTx_Control_ChgOn,
    eRxTx_Control_ChgOff,
    eRxTx_Control_GridOn,
    eRxTx_Control_GridOff,
    eRxTx_Control_AllOff,
    eRxTx_Control_ClrFault,

    eRxTx_Control_End
}RxTx_Control_e;

typedef enum
{
    eRxTx_ANS_OK    = 0xEEAA,
    eRXTX_ANS_NG    = 0x0000
}RxTX_Ans_e;

typedef struct
{
    unsigned int    RxStart     :1;     // 接收开始
    unsigned int    RxFrameOk   :1;     // 解析完成
    unsigned int    TxPackOk    :1;     // 发送数据打包完成
    unsigned int    WordPackOk  :1;     // 数据打包完成
    unsigned int    CrcCalEn    :1;     // crc计算使能

    unsigned int    Resv        :11;    // Resv
}RxTx_Flag_t;

typedef union
{
    unsigned int    all;
    RxTx_Flag_t     bit;
}RxTx_Flag_u;

typedef struct
{
    unsigned int    u16Soi[SOI_BYTE_NUM];
    unsigned int    u16Len;
    unsigned int    u16Cmd;
      signed int    i16Reg;
    unsigned int    u16RegLen;
    unsigned int    u16RegByteLen;
      signed int    i16DataInfo[DATA_INFO_MAX];
    unsigned int    u16DataCheck;
}RxData_t;


typedef struct
{
    RxData_t        t_RxData;
    RxTx_State_e    RxTx_Status;
    RxTx_Err_e      RxTx_ErrCode;
    RxTx_Flag_u     RxTx_Flag;

    unsigned int    u16AscByteLen;
    unsigned int    u16ThisWord;
    unsigned int    u16WordPart;
    unsigned int    u16WordNum;

    unsigned char   CrcLo;
    unsigned char   CrcHi;
    unsigned int    u16CheckCalValue;
    unsigned int    u16RxBuffLen;

    unsigned int    u16TxByteLen;
    unsigned int    u16TxWordLen;
    unsigned int    u16TxBuffLen;
    unsigned char   u8TxBuff[DATA_TX_BUFF_MAX];
}RxTx_t;

PROTOCOL_FUNCTION   void    sInitProtocol       (unsigned char SCI_NO, unsigned short bTxClrEn);
PROTOCOL_FUNCTION   void    sSci_RxTxDeal       (unsigned char SCI_NO, unsigned char RxData);  

#endif
