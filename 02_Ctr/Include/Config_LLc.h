#ifndef _CONFIG_LLC_H
#define _CONFIG_LLC_H

#ifdef CONFIG_LLC_FUNCTION_G
#define CONFIG_LLC_FUNCTION extern
#else
#define CONFIG_LLC_FUNCTION
#endif

#define cCHG_BAT_POWER_MAX          cPower1800W

#define cCHG_BAT_CURR_MAX           cCurr10A

#define cCHG_BAT_VOLT_MAX           cVdc60V
#define cCHG_BAT_VOLT_MIN           cVdc40V

#define cDCG_BUS_VLOT_BURST_MAX     cVdc500V
#define cDCG_BUS_VOLT_BURST_BAK     cVdc5V

typedef struct  
{
    signed int  i16BatCurrSet;
    signed int  i16BatVoltSet;

    float       f32LLCTrancRaio;
}ConfigLLC_t;

CONFIG_LLC_FUNCTION int     sConfig_GetBatCurrSet        (void);
CONFIG_LLC_FUNCTION int     sConfig_GetBatVoltSet        (void);

CONFIG_LLC_FUNCTION void    sConfig_SetLLCTrancRaio      (float Value);
CONFIG_LLC_FUNCTION float   sConfig_GetLLCTrancRaio      (void);

CONFIG_LLC_FUNCTION int     sLLC_GetFastStartCnt         (void);

#endif

