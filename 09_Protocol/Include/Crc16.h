#ifndef _CRC16_H
#define _CRC16_H

#ifdef CRC16_FUNCTION_G
#define CRC16_FUNCTION extern
#else
#define CRC16_FUNCTION
#endif

#define CRC_Hi      1
#define CRC_Lo      0

typedef struct
{
    unsigned int byteL  : 8;
    unsigned int byteH  : 8;
}BYTE_t;

typedef union 
{
    BYTE_t          u8Type;
    unsigned int    u16Type;
}WORD_t;

CRC16_FUNCTION  unsigned int    sCRC16Cal           (WORD_t *arrayPtr, int byteNum);
CRC16_FUNCTION  unsigned int    sNetbus_Crc16       (unsigned int *DataPtr, unsigned long ByteLen, unsigned char HiLo_F);
CRC16_FUNCTION  unsigned int    sModbus_Crc16       (unsigned int *DataPtr, unsigned long ByteLen, unsigned char HiLo_F);
CRC16_FUNCTION  unsigned int    sModByteCrc16       (unsigned int *DataPtr, unsigned long ByteLen, unsigned char HiLo_F);
CRC16_FUNCTION  unsigned int    sModWordCrc16       (unsigned int *DataPtr, unsigned long ByteLen, unsigned char HiLo_F);
CRC16_FUNCTION  unsigned int    sModWordLrc16       (unsigned int *DataPtr, unsigned long ByteLen);
CRC16_FUNCTION  unsigned int    sModOneByOneCrc16   (unsigned int *CRCLo,   unsigned char *CRCHi,  unsigned char CalData, unsigned char HiLo_F);

#endif
