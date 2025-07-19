#include "BSP_Adc.h"

#pragma CODE_SECTION(sAdc_Convert,".TI.ramfunc");

#define     cMaxMinLimit(x,Min,Max)     {   x = (x) >= (Max) ? (Max) : (x) ; x = (x) <= (Min) ? (Min) : (x) ; }

#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cInvVoltAdcResult           AdcaResultRegs.ADCRESULT1
#define     cInvCurrAdcResult           AdcaResultRegs.ADCRESULT2
#define     cGridVoltAdcResult          AdcaResultRegs.ADCRESULT3
#define     cGridCurrAdcResult          AdcaResultRegs.ADCRESULT4
#define     cIndCurrZeroAdcResult       AdcaResultRegs.ADCRESULT5
#define     cIndCurrPrdAdcResult        AdcaResultRegs.ADCRESULT6

#define     cNTC1AdcResult              AdcaResultRegs.ADCRESULT7
#define     cNTC2AdcResult              AdcaResultRegs.ADCRESULT8
#define     cNTC3AdcResult              AdcaResultRegs.ADCRESULT9

#define     cBatVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cBatCurrAdcResult           AdcaResultRegs.ADCRESULT1
#define     cBusVoltAdcResult           AdcaResultRegs.ADCRESULT2
#define     cLLcCurrAdcResult           AdcaResultRegs.ADCRESULT3

static      ADC_t                       t_ADC;

void    sInitAdc(void)
{
    sDrv_IninAdc();
}

void    sAdc_InitGain(void)
{
    t_ADC.GridVolt.f32Gain  = cGridVolt_ADC_Gain;
    t_ADC.GridCurr.f32Gain  = cGridCurr_ADC_Gain;

    t_ADC.ComVolt.f32Gain   = cComVolt_ADC_Gain;
    t_ADC.IndCurr.f32Gain   = cIndCurr_ADC_Gain;

    t_ADC.OutVolt.f32Gain   = cInvVolt_ADC_Gain;
    t_ADC.OutCurr.f32Gain   = cInvCurr_ADC_Gain;

    t_ADC.BatVolt.f32Gain   = cBatVolt_ADC_Gain;
    t_ADC.BatCurr.f32Gain   = cBatCurr_ADC_Gain;

    t_ADC.BusVolt.f32Gain   = cBusVolt_ADC_Gain;
    t_ADC.LLCCurr.f32Gain   = cLLcCurr_ADC_Gain;

}

void    sAdc_Convert()
{
    float       f32ADSample;
    signed int  i16ADCData;

    t_ADC.ComVolt.u16AdcResult      =   cComVoltAdcResult;
    i16ADCData                      =   (int)t_ADC.ComVolt.u16AdcResult - t_ADC.ComVolt.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.ComVolt.f32Gain;
    t_ADC.ComVolt.f32Real           =   f32ADSample * 1.0f;

    t_ADC.GridVolt.u16AdcResult      =   cGridVoltAdcResult;
    i16ADCData                      =   (int)t_ADC.GridVolt.u16AdcResult - t_ADC.GridVolt.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.GridVolt.f32Gain;
    t_ADC.GridVolt.f32Real           =   f32ADSample * 1.0f;

    t_ADC.OutVolt.u16AdcResult      =   cInvVoltAdcResult;
    i16ADCData                      =   (int)t_ADC.OutVolt.u16AdcResult - t_ADC.OutVolt.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.OutVolt.f32Gain;
    t_ADC.OutVolt.f32Real           =   f32ADSample * 1.0f;

    t_ADC.BatVolt.u16AdcResult      =   cBatVoltAdcResult;
    i16ADCData                      =   (int)t_ADC.BatVolt.u16AdcResult - t_ADC.BatVolt.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.BatVolt.f32Gain;
    t_ADC.BatVolt.f32Real           =   f32ADSample * 1.0f;

    t_ADC.BusVolt.u16AdcResult      =   cBusVoltAdcResult;
    i16ADCData                      =   (int)t_ADC.BusVolt.u16AdcResult - t_ADC.BusVolt.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.BusVolt.f32Gain;
    t_ADC.BusVolt.f32Real           =   f32ADSample * 1.0f;

    t_ADC.GridCurr.u16AdcResult      =   cGridCurrAdcResult;
    i16ADCData                      =   (int)t_ADC.GridCurr.u16AdcResult - t_ADC.GridCurr.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.GridCurr.f32Gain;
    t_ADC.GridCurr.f32Real           =   f32ADSample * 1.0f;

    t_ADC.OutCurr.u16AdcResult      =   cInvCurrAdcResult;
    i16ADCData                      =   (int)t_ADC.OutCurr.u16AdcResult - t_ADC.OutCurr.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.OutCurr.f32Gain;
    t_ADC.OutCurr.f32Real           =   f32ADSample * 1.0f;

    t_ADC.BatCurr.u16AdcResult      =   cBatCurrAdcResult;
    i16ADCData                      =   (int)t_ADC.BatCurr.u16AdcResult - t_ADC.BatCurr.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.BatCurr.f32Gain;
    t_ADC.BatCurr.f32Real           =   f32ADSample * 1.0f;

    t_ADC.IndCurr.u16AdcResult      =   (cIndCurrZeroAdcResult + cIndCurrPrdAdcResult) >> 1;
    i16ADCData                      =   (int)t_ADC.IndCurr.u16AdcResult - t_ADC.IndCurr.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.IndCurr.f32Gain;
    t_ADC.IndCurr.f32Real           =   f32ADSample * 1.0f;

    t_ADC.LLCCurr.u16AdcResult      =   cLLcCurrAdcResult;
    i16ADCData                      =   (int)t_ADC.LLCCurr.u16AdcResult - t_ADC.LLCCurr.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.LLCCurr.f32Gain;
    t_ADC.LLCCurr.f32Real           =   f32ADSample * 1.0f;

    t_ADC.NTC1                      = cNTC1AdcResult;
    t_ADC.NTC2                      = cNTC2AdcResult;
    t_ADC.NTC3                      = cNTC3AdcResult;
}

unsigned int sAdc_GetResult(ADC_Sample_e Goal)
{
    ADC_Real_t *pAdcResult;

    if(Goal >= eAdc_Sample_End)
    {
        return 0;
    }

    pAdcResult = &t_ADC.GridVolt + Goal;

    return pAdcResult->u16AdcResult;
}

float sAdc_GetReal(ADC_Sample_e Goal)
{
    ADC_Real_t *pAdcReal;

    if(Goal >= eAdc_Sample_End)
    {
        return 0.0f;
    }

    pAdcReal = &t_ADC.GridVolt + Goal;

    return pAdcReal->f32Real;
}

int sAdc_Geti16Real(ADC_Sample_e Goal)
{
    ADC_Real_t *pAdcReal;

    if(Goal >= eAdc_Sample_End)
    {
        return 0;
    }

    pAdcReal = &t_ADC.GridVolt + Goal;

    return (int)pAdcReal->f32Real;
}

unsigned int sAdc_GetNTCResult(unsigned int NTC)
{
    unsigned int *pNTC;
    if(NTC > cADCNUM_NTC)
    {
        return 0;
    }

    pNTC = &t_ADC.NTC1 + NTC;

    return *pNTC;
}

ADC_Real_t* sAdc_GetRealStr(ADC_Sample_e Goal)
{
    if(Goal >= eAdc_Sample_End)
    {
        return &t_ADC.GridVolt;
    }

    return &t_ADC.GridVolt + Goal;
}

void    sAdc_Calibration(void)
{
    volatile    unsigned int    *wP1;
                unsigned long   u32ChannelSum[16];
                unsigned int    u16SampleCnt;
                  signed int    k,i16ADCZero,MinValue,MaxValue;
                unsigned int    u16WaitADCnt;
                unsigned int    TridSelBack[16];

    volatile    union           ADCSOC0CTL_REG  *AdcSocCtlReg;

    MinValue = 1923;    // 1.55V
    MaxValue = 2172;    // 1.75V

    // Delay
    DELAY_US(2000);

    wP1 = (volatile unsigned int *)ADCARESULT_BASE;

    // Set Soc Trid Sel
    AdcSocCtlReg = &AdcaRegs.ADCSOC0CTL;
    for (k = 0; k < 16; k++) 
    {
        // Back Soc Trid Sel
        TridSelBack[k] = (*AdcSocCtlReg).bit.TRIGSEL;

        // Soc Trid by Software
        EALLOW;
        (*AdcSocCtlReg).bit.TRIGSEL = 0;
        EDIS;

        // Soc Trid Ptr
        AdcSocCtlReg += 2;
    }

    // Delay
    DELAY_US(2000);

    // Clear buffer
    for (k = 0; k < 16; k++) 
    {
        u32ChannelSum[k] = 0;
    }

    // ADC Start
    for (u16SampleCnt = 0; u16SampleCnt < 32; u16SampleCnt++)
    {
        // Start AD Convert
        AdcaRegs.ADCSOCFRC1.all = 0xFFFF;

        DELAY_US(20);

        for (u16WaitADCnt = 0; u16WaitADCnt < 30 && AdcaRegs.ADCCTL1.bit.ADCBSY == 1; u16WaitADCnt++)
        {
            // max wait 3us
            asm(" RPT #10 || NOP");
        } 

        DELAY_US(2);

        for (k = 0; k < 16; k++) 
        {
            // Read & Sum AD Result
            u32ChannelSum[k] += ((*(wP1 + k)) & 0xFFFF);
        }
    }

    i16ADCZero  = (signed int)(u32ChannelSum[0] >> 5);
    cMaxMinLimit(i16ADCZero, MinValue, MaxValue);
    t_ADC.ComVolt.i16Mid = i16ADCZero;

    AdcSocCtlReg = &AdcaRegs.ADCSOC0CTL;
    for (k = 0; k < 16; k++)
    {
        // Back SOC Trip Sel
        EALLOW;
        (*AdcSocCtlReg).bit.TRIGSEL = TridSelBack[k];

        // SOC Trid Ptr
        AdcSocCtlReg += 2;
    }


    // =========================== ADC C =================================== //
    // Delay
    DELAY_US(2000);

    wP1 = (volatile unsigned int *)ADCCRESULT_BASE;

    // Set Soc Trid Sel
    AdcSocCtlReg = &AdccRegs.ADCSOC0CTL;
    for (k = 0; k < 16; k++) 
    {
        // Back Soc Trid Sel
        TridSelBack[k] = (*AdcSocCtlReg).bit.TRIGSEL;

        // Soc Trid by Software
        EALLOW;
        (*AdcSocCtlReg).bit.TRIGSEL = 0;
        EDIS;

        // Soc Trid Ptr
        AdcSocCtlReg += 2;
    }

    // Delay
    DELAY_US(2000);

    // Clear buffer
    for (k = 0; k < 16; k++) 
    {
        u32ChannelSum[k] = 0;
    }

    // ADC Start
    for (u16SampleCnt = 0; u16SampleCnt < 32; u16SampleCnt++)
    {
        // Start AD Convert
        AdcaRegs.ADCSOCFRC1.all = 0xFFFF;

        DELAY_US(20);

        for (u16WaitADCnt = 0; u16WaitADCnt < 30 && AdcaRegs.ADCCTL1.bit.ADCBSY == 1; u16WaitADCnt++)
        {
            // max wait 3us
            asm(" RPT #10 || NOP");
        } 

        DELAY_US(2);

        for (k = 0; k < 16; k++) 
        {
            // Read & Sum AD Result
            u32ChannelSum[k] += ((*(wP1 + k)) & 0xFFFF);
        }
    }

    i16ADCZero  = (signed int)(u32ChannelSum[0] >> 5);
    cMaxMinLimit(i16ADCZero, MinValue, MaxValue);
    t_ADC.ComVolt.i16Mid = i16ADCZero;

    AdcSocCtlReg = &AdccRegs.ADCSOC0CTL;
    for (k = 0; k < 16; k++)
    {
        // Back SOC Trip Sel
        EALLOW;
        (*AdcSocCtlReg).bit.TRIGSEL = TridSelBack[k];

        // SOC Trid Ptr
        AdcSocCtlReg += 2;
    }    
}
