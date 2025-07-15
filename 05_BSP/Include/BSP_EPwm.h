#ifndef _BSP_EPWM_H
#define _BSP_EPWM_H

#ifdef BSP_EPWM_FUNCTION_G
#define BSP_EPWM_FUNCTION   extern
#else
#define BSP_EPWM_FUNCTION
#endif

#include "f28x_project.h"

#if defined(CPU_FRQ_100MHZ)
#define cTBCLK                  100000000
#elif defined(CPU_FRQ_120MHZ)
#define cTBCLK                  120000000
#endif

#define cLSPCLK                 30000000   

#define DBCOUNT_us(x)           (unsigned int)((float)(x) * 120)

#define sFreq2Prd_Up(Freq)      ((unsigned int)((unsigned long)(cTBCLK    ) / ((unsigned long)((Freq) * 1000))))
#define sFreq2Prd_Dn(Freq)      ((unsigned int)((unsigned long)(cTBCLK    ) / ((unsigned long)((Freq) * 1000))))
#define sFreq2Prd_UpDn(Freq)    ((unsigned int)((unsigned long)(cTBCLK / 2) / ((unsigned long)((Freq) * 1000))))

#define LLC_Freq                100.0f      // KHz
#define INV_Freq                24.0f       // KHz
#define cLLCDBT_RED             0.6         // us
#define cLLCDBT_FED             0.6         // us
#define cInvInitDBT             0.7         // us
#define cSwPrdDiff              60


#define sPwm_LLC_LBH_Shut()     {EALLOW;EPwm1Regs.TZFRC.bit.OST = 1;EDIS;}
#define sPwm_LLC_LFW_Shut()     {EALLOW;EPwm2Regs.TZFRC.bit.OST = 1;EDIS;}
#define sPwm_LLC_HBH_Shut()     {EALLOW;EPwm3Regs.TZFRC.bit.OST = 1;EDIS;}
#define sPwm_LLC_HFW_Shut()     {EALLOW;EPwm4Regs.TZFRC.bit.OST = 1;EDIS;}
#define sPwm_INV_HF_Shut()      {EALLOW;EPwm5Regs.TZFRC.bit.OST = 1;EDIS;}
#define sPwm_INV_LF_Shut()      {EALLOW;EPwm6Regs.TZFRC.bit.OST = 1;EDIS;}

#define sPwm_LLC_LBH_Open()     {EALLOW;EPwm1Regs.TZCLR.bit.OST = 1;EDIS;}
#define sPwm_LLC_LFW_Open()     {EALLOW;EPwm2Regs.TZCLR.bit.OST = 1;EDIS;}
#define sPwm_LLC_HBH_Open()     {EALLOW;EPwm3Regs.TZCLR.bit.OST = 1;EDIS;}
#define sPwm_LLC_HFW_Open()     {EALLOW;EPwm4Regs.TZCLR.bit.OST = 1;EDIS;}
#define sPwm_INV_HF_Open()      {EALLOW;EPwm5Regs.TZCLR.bit.OST = 1;EDIS;}
#define sPwm_INV_LF_Open()      {EALLOW;EPwm6Regs.TZCLR.bit.OST = 1;EDIS;}


BSP_EPWM_FUNCTION       void        sInitEPwm       (void); 

#endif

