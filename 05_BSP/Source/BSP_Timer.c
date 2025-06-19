#include "BSP_Timer.h"

typedef struct
{
    unsigned int    PowerOnOk   : 1;
    unsigned int    Time_1ms    : 1;
    unsigned int    Time_2ms    : 1;
    unsigned int    Time_4ms    : 1;
    unsigned int    Time_10ms   : 1;
    unsigned int    Time_20ms   : 1;
    unsigned int    Time_100ms  : 1;
    unsigned int    Time_1s     : 1;  
    unsigned int    Resv        : 8; 
}TimeFlag_t;

typedef struct
{
    TimeFlag_t      t_Flag;

    unsigned int    u16Nor_1msCnt;
    unsigned int    u16Nor_2msCnt;
    unsigned int    u16Nor_4msCnt;
    unsigned int    u16Nor_10msCnt;
    unsigned int    u16Nor_20msCnt;
    unsigned int    u16Nor_100msCnt;
    unsigned int    u16Nor_1sCnt;

    unsigned int    u16PowerOnCnt;
}Time_t;

static  Time_t      t_Time;

void    sInitTimer(void)
{
    InitCpuTimers();

    ConfigCpuTimer(&CpuTimer1, 120, 1000);
    ConfigCpuTimer(&CpuTimer2, 120, 1000);
}

static void sIncTime(unsigned int *TimeFlag,unsigned int *TimeCnt,unsigned int Gain)
{
    *TimeFlag = false;
    *TimeCnt++;

    if(*TimeCnt >= Gain)
    {
        *TimeCnt = 0;
        *TimeFlag = true;
    }
}

void    sSYS_Time(void)
{
    // 1ms
    t_Time.t_Flag.Time_1ms  = false;
    if( CpuTimer1.RegsAddr->TCR.bit.TIF == 1)
    {
        CpuTimer1.RegsAddr->TCR.bit.TIF = 1;
        t_Time.t_Flag.Time_1ms          = true;
    }

    t_Time.t_Flag.Time_2ms = false;
    if( CpuTimer2.RegsAddr->TCR.bit.TIF == 1)
    {
        CpuTimer2.RegsAddr->TCR.bit.TIF = 1;
        t_Time.t_Flag.Time_2ms          = true;
    }

    // t_Time.t_Flag.Time_4ms = false;
    // if (t_Time.t_Flag.Time_2ms == true) 
    // {
    //     t_Time.u16Nor_4msCnt++;
    //     if( t_Time.u16Nor_4msCnt >= 2)
    //     {
    //         t_Time.u16Nor_4msCnt = 0;
    //         t_Time.t_Flag.Time_4ms = true;
    //     }
    // }

    if(t_Time.t_Flag.Time_2ms == true)
    {
        sIncTime(&t_Time.u16Nor_4msCnt,&t_Time.u16Nor_4msCnt,(unsigned int) (4 / 2));
        sIncTime(&t_Time.u16Nor_10msCnt,&t_Time.u16Nor_10msCnt,(unsigned int) (10 / 2));
        sIncTime(&t_Time.u16Nor_20msCnt,&t_Time.u16Nor_20msCnt,(unsigned int) (20 / 2));
        sIncTime(&t_Time.u16Nor_100msCnt,&t_Time.u16Nor_100msCnt,(unsigned int) (100 / 2));
        sIncTime(&t_Time.u16Nor_1sCnt,&t_Time.u16Nor_1sCnt,(unsigned int) (1000 / 2));
    }

    if( t_Time.t_Flag.PowerOnOk == false)
    {
        if(t_Time.t_Flag.Time_10ms == true)
        {
            t_Time.u16PowerOnCnt++;
            if(t_Time.u16PowerOnCnt >= 500)
            {
                t_Time.u16PowerOnCnt    = 0;
                t_Time.t_Flag.PowerOnOk = true;
            }
        }
    }
}

unsigned int    sGetTime_1ms    (void)          {return t_Time.t_Flag.Time_1ms;}
unsigned int    sGetTime_2ms    (void)          {return t_Time.t_Flag.Time_2ms;}
unsigned int    sGetTime_4ms    (void)          {return t_Time.t_Flag.Time_4ms;}
unsigned int    sGetTime_10ms   (void)          {return t_Time.t_Flag.Time_10ms;}
unsigned int    sGetTime_20ms   (void)          {return t_Time.t_Flag.Time_20ms;}
unsigned int    sGetTime_100ms  (void)          {return t_Time.t_Flag.Time_100ms;}
unsigned int    sGetTime_1s     (void)          {return t_Time.t_Flag.Time_1s;}
