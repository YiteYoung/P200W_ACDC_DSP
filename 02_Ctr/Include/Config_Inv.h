#ifndef _CONFIG_INV_H
#define _CONFIG_INV_H

#ifdef CONFIG_INV_FUNCTION_G
#define CONFIG_INV_FUNCTION extern
#else
#define CONFIG_INV_FUNCTION
#endif

#define cCHG_BUS_VOLT_REF_MAX       cVdc490V
#define cCHG_BUS_VOLT_REF_MIN       cVdc320V

#define cCHG_BUS_VOLT_BURST_MAX     cVdc500V
#define cCHG_BUS_VOLT_BURST_BACK    cVdc5V

#define cCHG_AC_POWER_MAX           cPower1800W
#define cCHG_AC_CURR_MAX            cCurr10A

#define cDCG_AC_CURR_MAX            cCurr10A

#define cDCG_AC_POWER_MAX           cPower500W
// #define 


typedef enum 
{
    Volt100V = 0,
    Volt110V,
    Volt120V,

    Volt220V,
    Volt230V,
    Volt240V
}VoltLevel_e;

typedef enum 
{
    Freq50Hz = 0,
    Freq60Hz,
}FreqLevel_e;

typedef struct 
{
    signed int i16VoltSet;
    signed int i16FreqSet;

    signed int i16CurrLimit;
    signed int i16PowerLimit;
}Inv_Config_t;

typedef struct 
{
    signed int i16PfcVoltSet;

    signed int i16CurrLimit;
    signed int i16PowerLimit;


}Pfc_Config_t;

typedef struct 
{
    Inv_Config_t t_Inv;
    Pfc_Config_t t_Pfc;
}ConfigInv_t;

CONFIG_INV_FUNCTION void    sConfig_InitInv                 (void);
CONFIG_INV_FUNCTION void    sConfig_InitInvControl          (void);
CONFIG_INV_FUNCTION void    sConfig_InitPfcControl          (void);
CONFIG_INV_FUNCTION void    sConfig_InvFreqResh             (void);
CONFIG_INV_FUNCTION void    sConfig_InvResh                 (void);

CONFIG_INV_FUNCTION void     sConfig_SetInvVolt              (int Volt);
CONFIG_INV_FUNCTION void     sConfig_SetPfcVolt              (int Volt);

CONFIG_INV_FUNCTION int     sConfig_GetInvSet               (void);
CONFIG_INV_FUNCTION int     sConfig_GetPfcSet               (void);


#endif
