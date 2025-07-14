#ifndef _BSP_RLY_H
#define _BSP_RLY_H

#include "BSP_Gpio.h"

#ifdef  BSP_RLY_FUNCTION_G
#define BSP_RLY_FUNCTION extern
#else
#define BSP_RLY_FUNCTION
#endif

#define cRLY_ONOK_TIME                  800             // 判定继电器稳定
#define cRLY_PWM_TIME                   1000            // 继电器常高时间(时间到达后转PWN)

#define sRly_IdCheck(x)                 ((x) >= Rly_END ? 0:1)

#define sRly_BreakGridL()               GpioDataRegs.GPBSET.bit.GPIO32              = 1;
#define sRly_CloseGridL()               GpioDataRegs.GPBCLEAR.bit.GPIO32            = 1;

#define sRly_BreakGridN()               GpioDataRegs.GPBSET.bit.GPIO33              = 1;
#define sRly_CloseGridN()               GpioDataRegs.GPBCLEAR.bit.GPIO33            = 1;

#define sRly_BreakSoftR()               GpioDataRegs.GPBSET.bit.GPIO39              = 1;
#define sRly_CloseSoftR()               GpioDataRegs.GPBCLEAR.bit.GPIO39            = 1;

#define sRly_BreakOut()                 GpioDataRegs.GPBSET.bit.GPIO40              = 1;
#define sRly_CloseOut()                 GpioDataRegs.GPBCLEAR.bit.GPIO40            = 1;

typedef enum 
{
    Rly_Out = 0,
    Rly_SoftR,
    Rly_Grid,
    Rly_EV,
    Rly_END
}Rly_e;

typedef struct 
{
    unsigned int    Grid_On         :1;     //
    unsigned int    Soft_On         :1;     //
    unsigned int    Out_On          :1;     //
    unsigned int    EV_On           :1;     //

    unsigned int    Grid_Ok         :1;
    unsigned int    Soft_Ok         :1;
    unsigned int    Out_Ok          :1;
    unsigned int    EV_Ok           :1;

    unsigned int    Grid_PWMEN      :1;
    unsigned int    Soft_PWMEN      :1;
    unsigned int    Out_PWMEN       :1;
    unsigned int    EV_PWMEN        :1;  

    unsigned int    Resv            :4;
}RlyFlag_t;

typedef union 
{
    unsigned int    all;
    RlyFlag_t       bit;
}RlyFlag_u;

typedef struct 
{
    unsigned int    u16RlyTimeCnt[Rly_END];

    RlyFlag_u       Flag;

    unsigned int    PWM_EN;
}Rly_t;

BSP_RLY_FUNCTION void       sRly_Init            (void);
BSP_RLY_FUNCTION void       sRly_Task           (void);
BSP_RLY_FUNCTION void       sRly_Close          (Rly_e Id);
BSP_RLY_FUNCTION void       sRly_Break          (Rly_e Id);
BSP_RLY_FUNCTION void       sRly_BreakAll       (void);
BSP_RLY_FUNCTION int        sRly_GetFlag        (void);
BSP_RLY_FUNCTION int        sRly_GetRlyOn       (Rly_e Id);
BSP_RLY_FUNCTION int        sRly_GetRlyOk       (Rly_e Id);

#endif
