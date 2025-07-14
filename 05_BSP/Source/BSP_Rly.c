#include "BSP_Rly.h"

static Rly_t t_Rly;

void    sRly_Init(void)
{
    t_Rly.Flag.all = 0;

    int i;
    for (i = Rly_Out; i < Rly_END; i++) 
    {
        t_Rly.u16RlyTimeCnt[i] = 0;
    }
}

void    sRly_Task(void)
{
    int i;
    for (i = Rly_Out; i < Rly_END; i++) 
    {
        if( ((t_Rly.Flag.all >> i) & 0x01 ) != true)
        {
            continue;
        }
        
        t_Rly.u16RlyTimeCnt[i]++;
        if(t_Rly.u16RlyTimeCnt[i] >= cRLY_ONOK_TIME)
        {
            t_Rly.Flag.all |= 1 << (i + 4);
        }

        t_Rly.u16RlyTimeCnt[i]++;
        if(t_Rly.u16RlyTimeCnt[i] >= cRLY_PWM_TIME)
        {
            t_Rly.Flag.all |= 1 << (i + 8);
        }
    }

    if ( ((t_Rly.Flag.all >> 8) ^ (t_Rly.Flag.all & 0x0F))  == true)
    {
        t_Rly.PWM_EN = false;
    }
    else
    {
        t_Rly.PWM_EN = true;
    }
}

void    sRly_Close(Rly_e Id)
{
    if(sRly_IdCheck(Id) == false)
    {
        return;
    }

    t_Rly.Flag.all |= 0x01 << Id;
    t_Rly.Flag.all &= ~(0x111 << Id);
    t_Rly.u16RlyTimeCnt[Id] = 0;

#ifndef RLY_TYPE_PWM
    switch (Id) 
    {
        case    Rly_Grid:
            sRly_CloseGridL();
            sRly_CloseGridN();
            break;

        case    Rly_SoftR:
            sRly_CloseGridL();
            sRly_CloseGridN();
            sRly_CloseSoftR();
            break;

        case    Rly_Out:
            sRly_CloseOut();
            break;

        case    Rly_EV:
        default:
            break;
    }
#endif
}

void    sRly_Break(Rly_e Id)
{
    if(sRly_IdCheck(Id) == false)
    {
        return;
    }

    t_Rly.Flag.all &= ~(0x111 << Id);
    t_Rly.u16RlyTimeCnt[Id] = 0;

    
#ifndef RLY_TYPE_PWM
    switch (Id) 
    {
        case    Rly_Grid:
            sRly_BreakGridL();
            sRly_BreakGridN();
            break;

        case    Rly_SoftR:
            sRly_BreakSoftR();
            break;

        case    Rly_Out:
            sRly_BreakOut();
            break;

        case    Rly_EV:
        default:
            break;
    }
#endif
}

void    sRly_BreakAll(void)
{
    int i;
    t_Rly.Flag.all = 0;
    t_Rly.PWM_EN = false;
    for(i = 0; i < Rly_END; i++)
    {
        t_Rly.u16RlyTimeCnt[i] = 0;
    }

#ifndef RLY_TYPE_PWM
    sRly_BreakGridL();
    sRly_BreakGridN();
    sRly_BreakGridL();
    sRly_BreakGridN();
    sRly_BreakSoftR();
    sRly_BreakOut();
#endif
}

int sRly_GetFlag(void)
{
    return t_Rly.Flag.all;
}

int sRly_GetRlyOn(Rly_e Id)
{
    return (t_Rly.Flag.all >> Id) & 0x01;
}

int sRly_GetRlyOk(Rly_e Id)
{
    return (t_Rly.Flag.all >> (Id + 4)) & 0x01;
}
