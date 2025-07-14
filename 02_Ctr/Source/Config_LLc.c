#include "UserHeader.h"

static  ConfigLLC_t t_LLcConfig;

void    sConfig_SetBatCurr(int Value)
{
    if(Value > cCHG_BAT_CURR_MAX)
    {
        return;
    }

    t_LLcConfig.i16BatCurrSet = Value;
}

void    sConfig_SetBatVolt(int Value)
{
    if(Value > cCHG_BAT_VOLT_MAX)
    {
        return;
    }

    t_LLcConfig.i16BatVoltSet = Value;
}

void    sConfig_BatCurrCal(int BatVolt)
{
    float f32TempA;

    f32TempA = t_LLcConfig.f32MainDcPowerLimit;
    UpLimit(f32TempA, t_LLcConfig.f32ChgDcPowerLimit);
    UpLimit(f32TempA, t_LLcConfig.f32LimitDcPowerLimit);

    f32TempA = __divf32(f32TempA,(float)BatVolt);
    UpDnLimit(f32TempA, 0, t_LLcConfig.f32ChgDcPowerLimit * 0.1f);

    t_LLcConfig.f32BatCurrSet = (float)t_LLcConfig.i16BatCurrSet;
    UpLimit(t_LLcConfig.f32BatCurrSet, f32TempA);
}

float sConfig_GetBatCurrControlSet(void)
{
    return t_LLcConfig.f32BatCurrSet;
}

int sConfig_GetBatCurrUserSet(void)
{
    return t_LLcConfig.i16BatCurrSet;
}

int sConfig_GetBatVoltSet(void)
{
    return t_LLcConfig.i16BatVoltSet;
}

void    sConfig_SetLLCTrancRaio(float Value)
{
    t_LLcConfig.f32LLCTrancRaio = Value;
}

float   sConfig_GetLLCTrancRaio(void)
{
    return t_LLcConfig.f32LLCTrancRaio;
}

