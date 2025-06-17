#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "Driver_Adc.h"

#ifdef BSP_ADC_FUNCTION_G
#define BSP_ADC_FUNCTION extern
#else
#define BSP_ADC_FUNCTION
#endif

typedef struct
{
    signed int      i16Mid;
    float           f32Gain;
    float           f32Real;
    float           f32Real_Pre;
    float           f32Real_F;
    float           f32Real_AVG;
    unsigned int    u16AdcResult;

    signed int      i16Rms;
    signed int      i16Rms_100;
    float           f32Rms_Sum;
    float           f32Rms_Sum_B;
    float           f32Avg_Sum;
    unsigned int    u16SampleCnt;
    unsigned int    u16SampleCnt_B;
}ADC_Sample_t;

typedef struct
{
    ADC_Sample_t    BatVolt;
    ADC_Sample_t    ComVolt;
    ADC_Sample_t    OutVolt;
    ADC_Sample_t    GridVolt;
    ADC_Sample_t    BusVolt;

    ADC_Sample_t    BatCurr;
    ADC_Sample_t    IndCurr;
    ADC_Sample_t    OutCurr;
    ADC_Sample_t    GridCurr;
    ADC_Sample_t    LLCCurr;
}ADC_t;

#endif
