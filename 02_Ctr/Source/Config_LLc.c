#include "UserHeader.h"

static  ConfigLLC_t t_LLcConfig;

void    sConfig_InitLLC(void)
{
    t_LLcConfig.f32ChgDcCurrLimit = cCurr20A;

    t_LLcConfig.i16BatVoltSet = cVdc50V;
    t_LLcConfig.i16BatCurrSet = cCurr1A;

    t_LLcConfig.f32MainDcPowerLimit = cPower500W;
    t_LLcConfig.f32ChgDcPowerLimit = cPower500W;
    t_LLcConfig.f32LimitDcPowerLimit = cPower500W;
}

void    sConfig_LLCPowerLimit(void)
{
    float   f32PowerSet,f32PowerLimit;
    int     i16CurrLimit,i16CurrFed;
    int     i16PowerFed,Err;
    float   Ki;
    float   f32PowerLimitResult;


    if(t_LLcConfig.f32ChgAcCurrLimit <= cCurr10A && t_LLcConfig.f32ChgAcCurrLimit > 0)
    {
        sProtect_SetPfcCurr100(t_LLcConfig.f32ChgAcCurrLimit);
    }

    if( sRly_GetRlyOn(Rly_Out) == true)
    {
        f32PowerSet = Min2(t_LLcConfig.f32ChgDcPowerLimit - sSample_GetInvVA() * 0.1f, t_LLcConfig.f32MainDcPowerLimit);
        i16PowerFed = fabs(sSample_GetPfcWatt()) * 0.1f - fabs(sSample_GetInvWatt()) * 0.1f;
        DnLimit(i16PowerFed, cPower0W);
    }
    else
    {
        f32PowerSet = t_LLcConfig.f32MainDcPowerLimit;
        i16PowerFed = fabs(sSample_GetPfcWatt()) * 0.1f;
        DnLimit(i16PowerFed, cPower0W);
    }

    i16CurrLimit = t_LLcConfig.f32ChgAcCurrLimit;
    i16CurrFed   = sSample_GetRms(GridCurr);

    // 获取市电电压下的最大功率
    f32PowerLimit = sSample_GetRms(GridVolt) * i16CurrLimit * 0.01f;
    UpDnLimit(f32PowerLimit, cPower0W, cCHG_AC_POWER_MAX);

    f32PowerSet = Min2(f32PowerSet, f32PowerLimit);

    if(t_LLcConfig.f32PowerLimitRef > f32PowerSet + (float)cPower50W)
    {
        t_LLcConfig.f32PowerLimitRef -= (float)cPower50W;
    }
    else if(t_LLcConfig.f32PowerLimitRef < f32PowerSet - (float)cPower50W)
    {
        t_LLcConfig.f32PowerLimitRef += (float)cPower50W;
    }
    else 
    {
        t_LLcConfig.f32PowerLimitRef = f32PowerSet;
    }

    Err = i16CurrLimit - i16CurrFed;
    UpDnLimit(Err, -cCurr10A, cCurr10A);

    Ki = 0.01f;
    if      ( Err < -cCurr5A ) { Ki = 0.2f;     }
    else if ( Err < -cCurr2A ) { Ki = 0.05f;    }
    else if ( Err < cCurr0A )  { Ki = 0.01f;    }
    else if ( Err < cCurr2A )  { Ki = 0.025f;   }
    else                       { Ki = 0.05f;    }

    t_LLcConfig.f32CurrLimitErrSum -= (float)Err * Ki;
    UpDnLimit(t_LLcConfig.f32CurrLimitErrSum, 0.0f, t_LLcConfig.f32PowerLimitRef);

    Err = t_LLcConfig.f32PowerLimitRef - i16PowerFed;
    UpDnLimit(Err, -cPower1KW, cPower1KW);

    if      ( Err < -cPower100W )   { Ki = 0.1f;    }
    else if ( Err < -cPower200W )   { Ki = 0.05f;   }
    else if ( Err < cPower0W    )   { Ki = 0.01f;   }
    else if ( Err < cPower200W  )   { Ki = 0.025f;  }
    else                            { Ki = 0.05f;   }

    t_LLcConfig.f32PowerLimitErrSum -= (float)Err * Ki;
    UpDnLimit(t_LLcConfig.f32PowerLimitErrSum, 0.0f, t_LLcConfig.f32PowerLimitRef);

    f32PowerLimitResult = Max2(t_LLcConfig.f32PowerLimitErrSum, t_LLcConfig.f32CurrLimitErrSum);
    f32PowerLimitResult = t_LLcConfig.f32PowerLimitRef -   f32PowerLimitResult;
    UpDnLimit(f32PowerLimitResult, cPower10W, t_LLcConfig.f32PowerLimitRef);

    t_LLcConfig.f32LimitDcPowerLimit = f32PowerLimitResult * 0.93f;

}

void    sConfig_SetMainDcPowerLimit(float Value)
{
    if(Value < 0 || Value > cCHG_AC_POWER_MAX)
    {
        return;
    }

    t_LLcConfig.f32MainDcPowerLimit = Value;
}

float   sConfig_GetMainDcPowerLimit(void)
{
    return t_LLcConfig.f32MainDcPowerLimit;
}

void    sConfig_SetChgDcPowerLimit(float Value)
{
    if(Value < 0 || Value > cCHG_AC_POWER_MAX)
    {
        return;
    }

    t_LLcConfig.f32ChgDcPowerLimit = Value;
}

float   sConfig_GetChgDcPowerLimit(void)
{
    return t_LLcConfig.f32ChgDcPowerLimit;
}

void    sConfig_SetLimitDCPowerLimit(float Value)
{
    if(Value < 0 || Value > cCHG_AC_POWER_MAX)
    {
        return;
    }

    t_LLcConfig.f32LimitDcPowerLimit = Value;
}

float   sConfig_GetLimitDcPowerLimit(void)
{
    return t_LLcConfig.f32LimitDcPowerLimit;
}

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
    UpDnLimit(f32TempA, 0, t_LLcConfig.f32ChgDcCurrLimit * 0.1f);

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

