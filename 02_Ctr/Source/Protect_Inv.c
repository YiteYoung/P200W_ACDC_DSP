#include "UserHeader.h"

static InvProtect_t t_InvProtect;

void    sProtect_SetInvLoad100(unsigned long Value)
{
    t_InvProtect.t_Inv.u32Watt100 = Value;
    t_InvProtect.t_Inv.u32VA100 = Value;
}

unsigned long sProtect_GetInvLoad100(void)
{
    return     t_InvProtect.t_Inv.u32Watt100;
}    

void    sProtect_SetPfcLoad100(unsigned long Value)
{
    t_InvProtect.t_Pfc.u32Watt100 = Value;
    t_InvProtect.t_Pfc.u32VA100 = Value;
}

unsigned long sProtect_GetPfcLoad100(void)
{
    return     t_InvProtect.t_Pfc.u32Watt100;
}

void    sProtect_SetInvCurr100(unsigned long Value)
{
    t_InvProtect.t_Inv.u32Curr100 = Value;
}

unsigned long sProtect_GetInvCurr100(void)
{
    return     t_InvProtect.t_Inv.u32Curr100;
}    

void    sProtect_SetPfcCurr100(unsigned long Value)
{
    t_InvProtect.t_Pfc.u32Curr100 = Value;
}

unsigned long sProtect_GetPfcCurr100(void)
{
    return     t_InvProtect.t_Pfc.u32Curr100;
}

// ========================================================================= //
void    TZ_Deal(void)
{
    if( EPwm1Regs.TZOSTFLG.bit.OST1 == true || EPwm2Regs.TZOSTFLG.bit.OST1 == true ||\
        EPwm3Regs.TZOSTFLG.bit.OST1 == true || EPwm4Regs.TZOSTFLG.bit.OST1 == true ||\
        EPwm5Regs.TZOSTFLG.bit.OST1 == true || EPwm6Regs.TZOSTFLG.bit.OST1 == true )
    {
        t_InvProtect.u_TZFlag.bit.TZ1 = true;

        EALLOW;
        EPwm1Regs.TZOSTCLR.bit.OST1 = 1;
        EPwm2Regs.TZOSTCLR.bit.OST1 = 1;
        EPwm3Regs.TZOSTCLR.bit.OST1 = 1;
        EPwm4Regs.TZOSTCLR.bit.OST1 = 1;
        EPwm5Regs.TZOSTCLR.bit.OST1 = 1;
        EPwm6Regs.TZOSTCLR.bit.OST1 = 1;
        EDIS;
    }

    if( EPwm1Regs.TZOSTFLG.bit.OST2 == true || EPwm2Regs.TZOSTFLG.bit.OST2 == true ||\
        EPwm3Regs.TZOSTFLG.bit.OST2 == true || EPwm4Regs.TZOSTFLG.bit.OST2 == true ||\
        EPwm5Regs.TZOSTFLG.bit.OST2 == true || EPwm6Regs.TZOSTFLG.bit.OST2 == true )
    {
        t_InvProtect.u_TZFlag.bit.TZ2 = true;

        EALLOW;
        EPwm1Regs.TZOSTCLR.bit.OST2 = 1;
        EPwm2Regs.TZOSTCLR.bit.OST2 = 1;
        EPwm3Regs.TZOSTCLR.bit.OST2 = 1;
        EPwm4Regs.TZOSTCLR.bit.OST2 = 1;
        EPwm5Regs.TZOSTCLR.bit.OST2 = 1;
        EPwm6Regs.TZOSTCLR.bit.OST2 = 1;
        EDIS;
    }

    if( EPwm1Regs.TZOSTFLG.bit.DCAEVT1 == true || EPwm2Regs.TZOSTFLG.bit.DCAEVT1 == true ||\
        EPwm3Regs.TZOSTFLG.bit.DCAEVT1 == true || EPwm4Regs.TZOSTFLG.bit.DCAEVT1 == true ||\
        EPwm5Regs.TZOSTFLG.bit.DCAEVT1 == true || EPwm6Regs.TZOSTFLG.bit.DCAEVT1 == true ||\
        EPwm1Regs.TZOSTFLG.bit.DCBEVT1 == true || EPwm2Regs.TZOSTFLG.bit.DCBEVT1 == true ||\
        EPwm3Regs.TZOSTFLG.bit.DCBEVT1 == true || EPwm4Regs.TZOSTFLG.bit.DCBEVT1 == true ||\
        EPwm5Regs.TZOSTFLG.bit.DCBEVT1 == true || EPwm6Regs.TZOSTFLG.bit.DCBEVT1 == true )
    {
        t_InvProtect.u_TZFlag.bit.Trip4 = true;

        EALLOW;
        EPwm1Regs.TZOSTCLR.bit.DCAEVT1 = 1;
        EPwm2Regs.TZOSTCLR.bit.DCAEVT1 = 1;
        EPwm3Regs.TZOSTCLR.bit.DCAEVT1 = 1;
        EPwm4Regs.TZOSTCLR.bit.DCAEVT1 = 1;
        EPwm5Regs.TZOSTCLR.bit.DCAEVT1 = 1;
        EPwm6Regs.TZOSTCLR.bit.DCAEVT1 = 1;
        EPwm1Regs.TZOSTCLR.bit.DCBEVT1 = 1;
        EPwm2Regs.TZOSTCLR.bit.DCBEVT1 = 1;
        EPwm3Regs.TZOSTCLR.bit.DCBEVT1 = 1;
        EPwm4Regs.TZOSTCLR.bit.DCBEVT1 = 1;
        EPwm5Regs.TZOSTCLR.bit.DCBEVT1 = 1;
        EPwm6Regs.TZOSTCLR.bit.DCBEVT1 = 1;
        EDIS;
    }

    if( t_InvProtect.u_TZFlag.bit.TZ1 == true || t_InvProtect.u_TZFlag.bit.TZ2 == true ||\
        t_InvProtect.u_TZFlag.bit.Trip4 == true )
    {
        t_InvProtect.u_TZFlag.bit.SoftWare = true;
    }
}

void    sProtect_DisableTZ(void)
{
    EALLOW;

    EPwm5Regs.TZSEL.bit.CBC3 = TZ_DISABLE;
    EPwm5Regs.TZSEL.bit.OSHT1 = TZ_DISABLE;
    EPwm5Regs.TZSEL.bit.OSHT2 = TZ_DISABLE;
    EPwm5Regs.TZSEL.bit.DCAEVT2 = TZ_DISABLE;
    EPwm5Regs.TZDCSEL.bit.DCAEVT2 = TZ_EVT_DISABLE;

    EPwm6Regs.TZSEL.bit.OSHT3 = TZ_DISABLE;
    EPwm6Regs.TZSEL.bit.OSHT1 = TZ_DISABLE;
    EPwm6Regs.TZSEL.bit.OSHT2 = TZ_DISABLE;
    EPwm6Regs.TZSEL.bit.DCAEVT2 = TZ_DISABLE;
    EPwm6Regs.TZDCSEL.bit.DCAEVT2 = TZ_EVT_DISABLE;

    EDIS;
}


void    sProtect_SetTZFlag(TZ_ID_e Goal)
{
    if(Goal >= TZ_END)
    {
        return;
    }

    t_InvProtect.u_TZFlag.all |= (0x01 << Goal);
}

void    sProtect_ClrTZFlag(TZ_ID_e Goal)
{
    if(Goal >= TZ_END)
    {
        return;
    }

    t_InvProtect.u_TZFlag.all &= ~(Goal << 1);
}

unsigned int    sProtect_GetTZFlag(TZ_ID_e Goal)
{
    if(Goal >= TZ_END)
    {
        return 0;
    }

    return ((t_InvProtect.u_TZFlag.all >> Goal) & 0x01);
}


