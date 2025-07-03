#ifndef _SAMPLE_H
#define _SAMPLE_H

#ifdef  SAMPLE_FUNCTION_G
#define SAMPLE_FUNCTION extern
#else
#define SAMPLE_FUNCTION
#endif

#include "BSP_Adc.h"
#include "UserMath.h"



typedef struct
{
    signed int      i16Rms;
    signed int      i16Rms_100;
    float           f32Rms_Sum;
    float           f32Rms_Sum_B;
    float           f32Avg_Sum;
    unsigned int    u16SampleCnt;
    unsigned int    u16SampleCnt_B;    
}Sample_Cal_t;

typedef struct
{
    unsigned long   u32Watt100;
    unsigned long   u32VA100;
    unsigned long   u32Curr100;

    unsigned long   u32LoadVA_Pre;
    unsigned long   u32LoadVA;
    unsigned long   u32LoadVA_F;

    unsigned long   u32LoadVar_Pre;
    unsigned long   u32LoadVar;
    unsigned long   u32LoadVar_F;

      signed long   i32LoadWatt_Pre;
      signed long   i32LoadWatt;
      signed long   i32LoadWatt_F;
    
      signed int    i16PF;

    unsigned int    u16PowerPercent;
    unsigned int    u16CurrPercent;

           float    f32LoadSum;
           float    f32LoadSum_B;

    unsigned int    u16LoadCnt;
    unsigned int    u16LoadCnt_B;
}Sample_Load_t;

typedef struct
{
    Sample_Cal_t    GridVolt;
    Sample_Cal_t    ComVolt;    
    Sample_Cal_t    OutVolt;    
    Sample_Cal_t    IndCurr;    
    Sample_Cal_t    OutCurr;    
    Sample_Cal_t    GridCurr;
    Sample_Cal_t    LLCCurr;

    Sample_Cal_t    BusVolt;
    Sample_Cal_t    BatVolt;
    Sample_Cal_t    BatCurr;

    UNLONG          un_BusVolt_F;
    UNLONG          un_BusVolt_AVG; 

    UNLONG          un_BatVolt_F;
    UNLONG          un_BatVolt_AVG;

    UNLONG          un_BatCurr_F;
    UNLONG          un_BatCurr_AVG;

    UNLONG          un_LLCFreq_AVG;   
}Sample_t;

typedef struct
{
    Sample_Load_t   InvLoad;
    Sample_Load_t   GridLoad;

    signed int      BatPower;
}Load_t;

SAMPLE_FUNCTION void            sSample_Filter          (void);
SAMPLE_FUNCTION void            sSample_Accumulate      (void);
SAMPLE_FUNCTION void            sSample_ZeroDeal        (unsigned int GridN2P,unsigned int GridP2N, unsigned int InvN2P, unsigned int InvP2N);
SAMPLE_FUNCTION void            sSample_Cal_DC          (void);
SAMPLE_FUNCTION void            sSample_Cal_Inv         (void);
SAMPLE_FUNCTION void            sSample_Cal_Grid        (void);
SAMPLE_FUNCTION void            sSample_Cal_InvLoad     (void);
SAMPLE_FUNCTION void            sSample_Cal_GridLoad    (void);

SAMPLE_FUNCTION unsigned int    sSample_Cal_Freq        (unsigned int PrdPoint,unsigned int SwitchFreq,signed int Volt);
SAMPLE_FUNCTION signed int      sSample_GetRms          (ADC_Sample_e Goal);

SAMPLE_FUNCTION signed int      sSample_GetInvWatt      (void);
SAMPLE_FUNCTION signed int      sSample_GetPfcWatt      (void);
#endif
