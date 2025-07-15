#include "UserHeader.h"

static LLCProtect_t t_LLCProtect;

void    sProtect_DisableLLcTZ(void)
{
    EALLOW;

    EPwm1Regs.TZSEL.bit.OSHT1 = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.OSHT2 = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.OSHT3 = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCAEVT1 = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCAEVT2 = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCBEVT1 = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCBEVT2 = TZ_DISABLE;

    EPwm2Regs.TZSEL.bit.OSHT1 = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.OSHT2 = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.OSHT3 = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.DCAEVT1 = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.DCAEVT2 = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.DCBEVT1 = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.DCBEVT2 = TZ_DISABLE;

    EPwm3Regs.TZSEL.bit.OSHT1 = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.OSHT2 = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.OSHT3 = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.DCAEVT1 = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.DCAEVT2 = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.DCBEVT1 = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.DCBEVT2 = TZ_DISABLE;

    EPwm4Regs.TZSEL.bit.OSHT1 = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.OSHT2 = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.OSHT3 = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.DCAEVT1 = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.DCAEVT2 = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.DCBEVT1 = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.DCBEVT2 = TZ_DISABLE;
    EDIS;
}

void    sProtect_InitBatVoltThreshold(void)
{
    t_LLCProtect.i16BatVoltHLoss = cCHG_BAT_VOLT_MAX + cVdc1V;
    t_LLCProtect.i16BatVoltHBack = t_LLCProtect.i16BatVoltHLoss - cVdc2V;

    t_LLCProtect.i16BatVoltLLoss = cCHG_BAT_VOLT_MIN - cVdc1V;
    t_LLCProtect.i16BatVoltLBack = t_LLCProtect.i16BatVoltLLoss + cVdc2V;
}

void    sProtect_SetChgBatCurrLimit(unsigned long Value)
{
    t_LLCProtect.f32ChgBatCurrLimit = Value;
}

unsigned long    sProtect_GetChgBatCurrLimit(void)
{
    return t_LLCProtect.f32ChgBatCurrLimit;
}

void    sProtect_SetChgBatPowerLimit(unsigned long Value)
{
    t_LLCProtect.f32ChgBatPowerLimit = Value;
}

unsigned long    sProtect_GetChgBatPowerLimit(void)
{
    return t_LLCProtect.f32ChgBatPowerLimit;
}

