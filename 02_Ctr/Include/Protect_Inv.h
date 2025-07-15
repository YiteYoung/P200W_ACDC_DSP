#ifndef _PROTECT_INV_H
#define _PROTECT_INV_H

#ifdef PROTECT_INV_FUNCTION_G
#define PROTECT_INV_FUNCTION extern
#else
#define PROTECT_INV_FUNCTION
#endif

typedef struct 
{
    unsigned long   u32Watt100;
    unsigned long   u32VA100;
    unsigned long   u32Curr100;
}Load100_t;

typedef enum 
{
    TZ1 = 0,
    TZ2,
    TZ3,
    Trip4,
    SoftWare,
    Cmpss1,
    Cmpss4,
    TZ_END
}TZ_ID_e;

typedef struct 
{
    unsigned int    TZ1         :1;
    unsigned int    TZ2         :1;
    unsigned int    TZ3         :1;
    unsigned int    Trip4       :1;

    unsigned int    Cmpss1      :1;
    unsigned int    Cmpss4      :1;
    unsigned int    SoftWare    :1;

    unsigned int    Resv        :9;
}TZ_Flag_t;

typedef union 
{
    unsigned int    all;
    TZ_Flag_t       bit;
}TZ_Flag_u;

typedef struct 
{
    TZ_Flag_u   u_TZFlag;

    signed int  i16BusProtectVoltH;
    signed int  i16BusProtectVoltL;

}InvProtect_t;

typedef struct 
{
    Load100_t   t_Inv;
    Load100_t   t_Pfc;

    unsigned int    f32ChgAcCurrLimit;
    unsigned int    f32ChgAcPowerLimit;
}InvLimit_t;



PROTECT_INV_FUNCTION    void                sProtect_SetInvLoad100          (unsigned long Value);
PROTECT_INV_FUNCTION    unsigned long       sProtect_GetInvLoad100          (void);

PROTECT_INV_FUNCTION    void                sProtect_SetPfcLoad100          (unsigned long Value);
PROTECT_INV_FUNCTION    unsigned long       sProtect_GetPfcLoad100          (void);

PROTECT_INV_FUNCTION    void                sProtect_SetInvCurr100          (unsigned long Value);
PROTECT_INV_FUNCTION    unsigned long       sProtect_GetInvCurr100          (void);

PROTECT_INV_FUNCTION    void                sProtect_InitBusVoltThreshold   (void);

PROTECT_INV_FUNCTION    void                sLimit_SetChgAcCurr             (unsigned long Value);
PROTECT_INV_FUNCTION    unsigned long       sLimit_GetChgAcCurr             (void);

PROTECT_INV_FUNCTION    void                sLimit_SetChgAcPower            (unsigned long Value);
PROTECT_INV_FUNCTION    unsigned long       sLimit_GetChgAcPower            (void);

PROTECT_INV_FUNCTION    void                sProtect_SetPfcCurr100          (unsigned long Value);
PROTECT_INV_FUNCTION    unsigned long       sProtect_GetPfcCurr100          (void);

PROTECT_INV_FUNCTION    void                TZ_Deal                         (void);
PROTECT_INV_FUNCTION    void                sProtect_DisableInvTZ           (void);
PROTECT_INV_FUNCTION    void                sProtect_SetTZFlag              (TZ_ID_e Goal);
PROTECT_INV_FUNCTION    void                sProtect_ClrTZFlag              (TZ_ID_e Goal);
PROTECT_INV_FUNCTION    unsigned int        sProtect_GetTZFlag              (TZ_ID_e Goal);

#endif
