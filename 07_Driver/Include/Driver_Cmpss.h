#ifndef _DRIVER_CMPSS_H
#define _DRIVER_CMPSS_H

#include "f28x_project.h"

#ifdef DRIVER_CMPSS_FUNCTION_G
#define DRIVER_CMPSS_FUNCTION extern
#else
#define DRIVER_CMPSS_FUNCTION
#endif

#define sCMP_CH_Check(x)                ((x) >= CMP_CH_END ? 0 : 1)

#define cMaxDACValue                    4096

typedef enum
{
    CMP_CH_01   = 0,
    CMP_CH_02,
    CMP_CH_03,
    CMP_CH_04,

    CMP_CH_END
}CMPCH_e;

typedef enum
{
    eCMP_INV_DIS = 0,
    eCMP_INV_EN,

    eCMP_INV_END
}CMPINV_e;

typedef enum
{
    eCMP_OUT_ASYN = 0,
    eCMP_OUT_SYN,
    eCMP_OUT_FILITER,
    eCMP_OUT_LATCH,

    eCMP_OUT_END
}CMPOUT_e;

typedef enum
{
    eCMP_PHYS_NONE = 0,
    eCMP_PHYS_x1_10mV,
    eCMP_PHYS_x2_20mV,
    eCMP_PHYS_x3_30mV,
    eCMP_PHYS_x4_40mV,

    eCMP_PHYS_END
}CMPPHYS_e;

typedef enum
{
    A2_C9 = 0,
    A11_C0,
    A6,
    A15_C7,
    A1
}CMP1_HiLo_P_PIN_e;

typedef enum
{
    A4_C14_CMPSS2 = 0,
    A12_C1_CMPSS2,
    A9_C8_CMPSS2,
    A10_C10_CMPSS2,
    A8_C11_CMPSS2,
    TempSensor
}CMP2_HiLo_P_PIN_e;

typedef enum
{
    C6 = 0,
    A5_C2,
    A0_C15_CMP1_DACL,
    A3_C5,
    A14_C4
}CMP3_HiLo_P_PIN_e;

typedef enum
{
    A9_C8_CMPSS4 = 0,
    A7_C3_CMPSS4,
    A12_C1_CMPSS4,
    A4_C14_CMPSS4,
    A8_C11_CMPSS4
}CMP4_HiLo_P_PIN_e;

typedef enum
{
    A15_C7_N = 0,
    A11_C0_N
}CMP1_HiLo_N_PIN_e;

typedef enum
{
    A10_C10_N = 0,
    A12_C1_N
}CMP2_HiLo_N_PIN_e;

typedef enum
{
    A3_C5_N = 0,
    A5_C2_N
}CMP3_HiLo_N_PIN_e;

typedef enum
{
    A4_C14_N = 0,
    A7_C3_N,
}CMP4_HiLo_N_PIN_e;

typedef struct
{
    unsigned int    CMP_CH;         // CMP CH

    unsigned int    DAC_Hi_Value;   // Hi DAC Value
    unsigned int    DAC_Lo_Value;   // Lo DAC Value

    CMPINV_e        CMP_Hi_INV;     // Hi CMP INV Enable/Disable
    CMPINV_e        CMP_Lo_INV;     // Lo CMO INV Enable/Disable
    CMPOUT_e        PWMXBAR_Hi_Sel; // Hi CMP Out to EPWM XBar
    CMPOUT_e        PWMXBAR_Lo_Sel; // Lo CMP Out to EPWM XBar
    CMPOUT_e        OUTXBAR_Hi_Sel; // Hi CMP Out to OutPut XBar        
    CMPOUT_e        OUTXBAR_Lo_Sel; // Lo CMP Out to OutPut XBar

    CMPPHYS_e       CMP_PHYS;       // Set Hysteresis

    unsigned int    CMP_PIN_Hi_Sel; // CMP Hi Pos Input
    unsigned int    CMP_PIN_Lo_Sel; // CMP Lo Pos Input

    unsigned int    CLK_Prescale;   // Filiter sample clock prescale
    unsigned int    SAMP_WIN;       // Filiter sample window size
    unsigned int    Thre_SH;        // Filiter majority voting threshold
}CMPCONFIG_t;

DRIVER_CMPSS_FUNCTION   void            sDrv_InitCmpss          (CMPCONFIG_t t_CMPSS_Para);
DRIVER_CMPSS_FUNCTION   void            sDrv_RegCmpss           (CMPCONFIG_t t_CMPSS_Para);
DRIVER_CMPSS_FUNCTION   void            sDrv_RegCmpss_Lite      (CMPCONFIG_t t_CMPSS_Para);

DRIVER_CMPSS_FUNCTION   void            sDrv_SetCmpss_DAC       (unsigned int CH, unsigned int DAC_Hi, unsigned int DAC_Lo);

DRIVER_CMPSS_FUNCTION   unsigned int    sDrv_GetHiLatchSTS      (unsigned int CH);
DRIVER_CMPSS_FUNCTION   unsigned int    sDrv_GetLoLatchSTS      (unsigned int CH);
DRIVER_CMPSS_FUNCTION   void            sDrv_ClrHiLatchSTS      (unsigned int CH);
DRIVER_CMPSS_FUNCTION   void            sDrv_ClrLoLatchSTS      (unsigned int CH);

#endif
