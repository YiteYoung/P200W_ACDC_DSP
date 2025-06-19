#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "Driver_Adc.h"

#ifdef BSP_ADC_FUNCTION_G
#define BSP_ADC_FUNCTION extern
#else
#define BSP_ADC_FUNCTION
#endif

#define     cADC_RAIO               10.0f

#define     cADC_REF                3.3f
#define     cDAC_REF                3.30f
#define     cADC_REF_USER           3.27422f

#define     cADC_MAX_RANGE          4096.0f
#define     cDAC_MAX_RANGE          4096.0f
#define     cDAC_MID_RANGE          ((cADC_REF      * 0.5f * cDAC_MAX_RANGE) / (cDAC_REF))
#define     cDAC_MID_RANGE_USER     ((cADC_REF_USER * 0.5f * cDAC_MAX_RANGE) / (cDAC_REF))

#define     cLLC_IND_OCP_A          10.0f
#define     cPFC_IND_OCP_A          10.0f

#define     cV_Com_V_1V             (20.0f / (2200.0f * 3.0f + 0.1f))       // Com Volt
#define     cV_Grid_V_1V            (20.0f / (2200.0f * 3.0f + 0.1f))       // Grid Volt
#define     cV_Inv_V_1V             (20.0f / (2200.0f * 3.0f + 0.1f))       // InvOut Volt
#define     cV_Bus_V_1V             (20.0f / (2200.0f * 3.0f + 0.1f))       // Bus Volt
#define     cV_Bat_V_1V             (20.0f / (2200.0f * 3.0f + 0.1f))       // Bat Volt

#define     cI_Grid_V_1A            (20.0f / (2200.0f * 3.0f + 0.1f))       // Grid Curr
#define     cI_Inv_V_1A             (20.0f / (2200.0f * 3.0f + 0.1f))       // InvOut Curr
#define     cI_Ind_V_1A             (20.0f / (2200.0f * 3.0f + 0.1f))       // PfcInd_Curr
#define     cI_LLC_V_1A             (20.0f / (2200.0f * 3.0f + 0.1f))       // LLc_Curr
#define     cI_Bat_V_1A             (20.0f / (2200.0f * 3.0f + 0.1f))       // Bat_Curr

#define     cComVolt_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cV_Com_V_1V * cADC_MAX_RANGE))
#define     cInvVolt_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cV_Inv_V_1V * cADC_MAX_RANGE))
#define     cGridVolt_ADC_Gain      ((cADC_REF * cADC_RAIO) / (cV_Grid_V_1V * cADC_MAX_RANGE))
#define     cBusVolt_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cV_Bus_V_1V * cADC_MAX_RANGE))
#define     cBatVolt_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cV_Bat_V_1V * cADC_MAX_RANGE))

#define     cGridCurr_ADC_Gain      ((cADC_REF * cADC_RAIO) / (cI_Grid_V_1A * cADC_MAX_RANGE))
#define     cInvCurr_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cI_Inv_V_1A * cADC_MAX_RANGE))
#define     cIndCurr_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cI_Ind_V_1A * cADC_MAX_RANGE))
#define     cLLcCurr_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cI_LLC_V_1A * cADC_MAX_RANGE))
#define     cBatCurr_ADC_Gain       ((cADC_REF * cADC_RAIO) / (cI_Bat_V_1A * cADC_MAX_RANGE))
 
#define     sADCSoftStart()         { AdcaRegs.ADCSOCFRC1.all = 0x0000; AdccRegs.ADCSOCFRC1.all = 0x0000; }


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

BSP_ADC_FUNCTION    void    sInitAdc    (void);
BSP_ADC_FUNCTION    void    sAdcSample  (void);

#endif
