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

unsigned int    sProtect_GetTZFlag(TZ_ID_e Goal)
{
    if(Goal >= TZ_END)
    {
        return 0;
    }

    return ((t_InvProtect.u_TZFlag.TZFlag >> Goal) & 0x01);
}


