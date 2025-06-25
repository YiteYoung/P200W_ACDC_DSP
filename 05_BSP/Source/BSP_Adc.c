#include "BSP_Adc.h"

#define     cMaxMinLimit(x,Min,Max)     {   x = (x) >= (Max) ? (Max) : (x) ; x = (x) <= (Min) ? (Min) : (x) ; }

#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0

#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0

#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0
#define     cComVoltAdcResult           AdcaResultRegs.ADCRESULT0

static      ADC_t                       t_ADC;

void    sInitAdc(void)
{
    sDrv_IninAdc();
}

void    sAdc_Sample()
{
    float       f32ADSample;
    signed int  i16ADCData;

    // =========================== ADC A =================================== //
    // Com Volt
    t_ADC.ComVolt.u16AdcResult      =   cComVoltAdcResult;
    i16ADCData                      =   (int)t_ADC.ComVolt.u16AdcResult - t_ADC.ComVolt.i16Mid;
    f32ADSample                     =   (float)i16ADCData * t_ADC.ComVolt.f32Gain;
    t_ADC.ComVolt.f32Real           =   f32ADSample * 1.0f;
}

unsigned int sGetAdc_Result(ADC_Sample_e Goal)
{
    ADC_Real_t *pAdcResult;

    if(Goal >= eAdc_Sample_End)
    {
        return 0;
    }

    pAdcResult = &t_ADC.GridVolt + Goal;

    return pAdcResult->u16AdcResult;
}

float sGetAdc_Real(ADC_Sample_e Goal)
{
    ADC_Real_t *pAdcReal;

    if(Goal >= eAdc_Sample_End)
    {
        return 0.0f;
    }

    pAdcReal = &t_ADC.GridVolt + Goal;

    return pAdcReal->f32Real;
}

unsigned int sGetAdc_NTCResult(unsigned int NTC)
{
    unsigned int *pNTC;
    if(NTC > cADCNUM_NTC)
    {
        return 0;
    }

    pNTC = &t_ADC.NTC1 + NTC;

    return *pNTC;
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
