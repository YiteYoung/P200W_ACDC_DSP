#ifndef _PROTECT_LLC_H
#define _PROTECT_LLC_H

#ifdef PROTECT_LLC_FUNCTION_G
#define PROTECT_LLC_FUNCTION extern
#else
#define PROTECT_LLC_FUNCTION
#endif

typedef struct 
{
    float   f32ChgBatCurrLimit;
    float   f32ChgBatPowerLimit;
}LLCProtect_t;

PROTECT_LLC_FUNCTION    void                sProtect_DisableLLcTZ           (void);

PROTECT_LLC_FUNCTION    void                sProtect_SetChgBatCurrLimit     (unsigned long Value);
PROTECT_LLC_FUNCTION    unsigned long       sProtect_GetChgBatCurrLimit     (void);

PROTECT_LLC_FUNCTION    void                sProtect_SetChgBatPowerLimit    (unsigned long Value);
PROTECT_LLC_FUNCTION    unsigned long       sProtect_GetChgBatPowerLimit    (void);
#endif
