#ifndef _SAMPLE_H
#define _SAMPLE_H

#ifdef  SAMPLE_FUNCTION_G
#define SAMPLE_FUNCTION extern
#else
#define SAMPLE_FUNCTION
#endif

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
    Sample_Cal_t    BatVolt;
    Sample_Cal_t    ComVolt;
    Sample_Cal_t    OutVolt;
    Sample_Cal_t    GridVolt;
    Sample_Cal_t    BusVolt;

    Sample_Cal_t    BatCurr;
    Sample_Cal_t    IndCurr;
    Sample_Cal_t    OutCurr;
    Sample_Cal_t    GridCurr;
    Sample_Cal_t    LLCCurr;
}Sample_t;


SAMPLE_FUNCTION void            sSample_CalSum      (void);

SAMPLE_FUNCTION unsigned int    sSample_RmsCal      (float *SampleSum,unsigned int *SampleCnt);
SAMPLE_FUNCTION unsigned int    sSample_AvgCal      (float *SampleSum,unsigned int *SampleCnt);

SAMPLE_FUNCTION unsigned long   sSample_VACal       (unsigned int VoltRms,unsigned int CurrRms, unsigned long *VAPre);
SAMPLE_FUNCTION   signed long   sSample_WattCal     (float *SampleSum,unsigned int *SampleCnt,signed long *WattPre);
SAMPLE_FUNCTION unsigned long   sSample_VARCal      (signed long LoadWatt,unsigned long LoadVA);

SAMPLE_FUNCTION          void   sSample_LoadFilter  (signed long Watt,signed long *Watt_F,unsigned long VA,unsigned long *VA_F,unsigned int Filter);
SAMPLE_FUNCTION          void   sSample_PercentCal  (signed long LoadWatt,unsigned long LoadWatt100,unsigned long LoadVA,unsigned long LoadVA100,signed int *LoadPercent);
SAMPLE_FUNCTION          void   sSample_PFCal       (signed long LoadWatt,unsigned long LoadVA,signed int *LoadPF);

#endif
