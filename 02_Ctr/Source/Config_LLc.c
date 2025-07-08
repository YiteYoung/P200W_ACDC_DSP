#include "UserHeader.h"

static  ConfigLLC_t t_LLcConfig;

int    sConfig_GetBatCurrSet(void)
{
    return t_LLcConfig.i16BatCurrSet;
}


int    sConfig_GetBatVoltSet(void)
{
    return t_LLcConfig.i16BatVoltSet;
}

