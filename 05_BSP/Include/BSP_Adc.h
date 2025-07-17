#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "Driver_Adc.h"

#ifdef BSP_ADC_FUNCTION_G
#define BSP_ADC_FUNCTION extern
#else
#define BSP_ADC_FUNCTION
#endif

#define     cADCNUM_NTC             3
#define     cADCNUM_CURR            5
#define     cADCNUM_VOLT            5

#define     cADC_RAIO               10.0f

#define     cADC_REF                3.3f
#define     cDAC_REF                3.30f
#define     cADC_REF_USER           3.27422f

#define     cADC_MAX_RANGE          4096.0f
#define     cDAC_MAX_RANGE          4096.0f
#define     cDAC_MID_RANGE          ((cADC_REF      * 0.5f * cDAC_MAX_RANGE) / (cDAC_REF))
#define     cDAC_MID_RANGE_USER     ((cADC_REF_USER * 0.5f * cDAC_MAX_RANGE) / (cDAC_REF))

#define     cLLC_RES_OCP_10A          10.0f
#define     cPFC_IND_OCP_10A          10.0f

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

#define     cLLC_RES_OCP_DAC_10A    ((cLLC_RES_OCP_10A * cI_LLC_V_1A * cDAC_MAX_RANGE) / cDAC_REF)
#define     cPFC_IND_OCP_DAC_10A    ((cPFC_IND_OCP_10A * cI_Ind_V_1A * cDAC_MAX_RANGE) / cDAC_REF)
 
#define     sADCSoftStart()         { AdcaRegs.ADCSOCFRC1.all = 0x0000; AdccRegs.ADCSOCFRC1.all = 0x0000; }


typedef enum
{
    eAdc_Sample_Start       = 0,

    eAdc_Sample_AC_Start    = 0,
    GridVolt                = 0,
    ComVolt                 = 1,
    OutVolt                 = 2,
    IndCurr                 = 3,    
    OutCurr                 = 4,
    GridCurr                = 5,
    LLCCurr                 = 6,
    eAdc_Sample_AC_End      = 7,

    eAdc_Sample_DC_Start    = 7,
    BusVolt                 = 7,
    BatVolt                 = 8,
    BatCurr                 = 9,
    eAdc_Sample_DC_End      = 10,
    eAdc_Sample_End         = 10
}ADC_Sample_e;

typedef struct
{
    signed int      i16Mid;
    float           f32Gain;
    float           f32Real;
    float           f32Real_Pre;
    float           f32Real_F;
    float           f32Real_AVG;
    unsigned int    u16AdcResult;
}ADC_Real_t;

typedef struct
{
    ADC_Real_t      GridVolt;
    ADC_Real_t      ComVolt;    
    ADC_Real_t      OutVolt;    
    ADC_Real_t      IndCurr;    
    ADC_Real_t      OutCurr;    
    ADC_Real_t      GridCurr;
    ADC_Real_t      LLCCurr;

    ADC_Real_t      BusVolt;
    ADC_Real_t      BatVolt;
    ADC_Real_t      BatCurr;

    unsigned int    NTC1;
    unsigned int    NTC2;
    unsigned int    NTC3;
}ADC_t;

BSP_ADC_FUNCTION    void            sInitAdc                (void);
BSP_ADC_FUNCTION    void            sAdc_InitGain           (void);
BSP_ADC_FUNCTION    void            sAdc_Convert            (void);
BSP_ADC_FUNCTION    unsigned int    sAdc_GetResult          (ADC_Sample_e Goal);
BSP_ADC_FUNCTION    float           sAdc_GetReal            (ADC_Sample_e Goal);
BSP_ADC_FUNCTION    int             sAdc_Geti16Real         (ADC_Sample_e Goal);
BSP_ADC_FUNCTION    unsigned int    sAdc_GetNTCResult       (unsigned int NTC);
BSP_ADC_FUNCTION    ADC_Real_t*     sAdc_GetRealStr         (ADC_Sample_e Goal);

BSP_ADC_FUNCTION    void            sAdc_Calibration        (void);

#endif
