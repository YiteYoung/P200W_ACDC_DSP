#include "UserHeader.h"

static LLCProtect_t t_LLCProtect;

void    sProtect_SetChgBatCurrLimit(unsigned long Value)
{
    t_LLCProtect.f32ChgBatCurrLimit = Value;
}

unsigned long    sProtect_GetChgBatCurrLimit(void)
{
    return t_LLCProtect.f32ChgBatCurrLimit;
}

void    sProtect_SetChgAcPowerLimit(unsigned long Value)
{
    t_LLCProtect.f32ChgAcPowerLimit = Value;
}

unsigned long    sProtect_GetChgAcPowerLimit(void)
{
    return t_LLCProtect.f32ChgAcPowerLimit;
}

void    sProtect_SetChgBatPowerLimit(unsigned long Value)
{
    t_LLCProtect.f32ChgBatPowerLimit = Value;
}

unsigned long    sProtect_GetChgBatPowerLimit(void)
{
    return t_LLCProtect.f32ChgBatPowerLimit;
}

