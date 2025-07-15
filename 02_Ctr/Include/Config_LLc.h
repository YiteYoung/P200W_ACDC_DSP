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
    signed int  i16BatCurrSet;              // 上位机设置
    float       f32BatCurrSet;              // 控制使用

    signed int  i16BatVoltSet;

    float       f32ChgAcCurrLimit;          // 充电电流限制
    float       f32ChgDcCurrLimit;          // 电池电流限制

    float       f32LLCTrancRaio;

    float       f32MainDcPowerLimit;        //主控限电池侧功率
    float       f32ChgDcPowerLimit;         //默认限电池侧功率
    float       f32LimitDcPowerLimit;       //功率环限电池侧功率 
}ConfigLLC_t;

CONFIG_LLC_FUNCTION void    sConfig_InitLLC                     (void);

CONFIG_LLC_FUNCTION void    sConfig_SetBatCurr                  (int Value);
CONFIG_LLC_FUNCTION void    sConfig_SetBatVolt                  (int Value);

CONFIG_LLC_FUNCTION void    sConfig_BatCurrCal                  (int BatVolt);
CONFIG_LLC_FUNCTION float   sConfig_GetBatCurrControlSet        (void);

CONFIG_LLC_FUNCTION int     sConfig_GetBatCurrUserSet           (void);
CONFIG_LLC_FUNCTION int     sConfig_GetBatVoltSet               (void);

CONFIG_LLC_FUNCTION void    sConfig_SetLLCTrancRaio             (float Value);
CONFIG_LLC_FUNCTION float   sConfig_GetLLCTrancRaio             (void);

CONFIG_LLC_FUNCTION int     sLLC_GetFastStartCnt                (void);

#endif

