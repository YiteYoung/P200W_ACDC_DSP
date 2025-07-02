#include "UserHeader.h"

static      GridProt_t          t_GridPort;

void    sGrid_InitDetectValue(void)
{
    t_GridPort.i16RmsHiLossValue    = cVac270V;
    t_GridPort.i16RmsLoLossValue    = cVac80V;
    t_GridPort.i16RmsHiRestValue    = cVac265V;
    t_GridPort.i16RmsLoRestValue    = cVac85V;

    t_GridPort.i16WaveHiLossValue   = cVac380V;
    t_GridPort.i16WaveLoLossValue   = cVac20V;
    t_GridPort.i16WaveHiRestValue   = cVac270V;
    t_GridPort.i16WaveLoRestValue   = cVac25V;

    t_GridPort.i16FreqHiLossValue   = cFreq65Hz;
    t_GridPort.i16FreqLoLossValue   = cFreq45Hz;
    t_GridPort.i16FreqHiRestValue   = cFreq64Hz;
    t_GridPort.i16FreqLoRestValue   = cFreq46Hz;
}

void    sGrid_StatusDeal(void)
{
    // Freq Err
    if( t_GridPort.t_Flag.FreqLoss == true )
    {
        sFault_SetAlarm(eAlarm_GridFreqErr);
    }
    else 
    {
        sFault_ClrAlarm(eAlarm_GridFreqErr);
    }

    // Grid Status Check
    if( t_GridPort.t_Flag.VoltLoss == true ||\
        t_GridPort.t_Flag.FreqLoss == true ||\
        t_GridPort.t_Flag.Waveloss == true ) 
    {
        t_GridPort.t_Flag.InWorkNow = false;
        sFault_SetAlarm(eAlarm_GridVoltErr);
    }
    else 
    {
        t_GridPort.t_Flag.InWorkNow = true;
        sFault_ClrAlarm(eAlarm_GridVoltErr);
    }

    // Grid Ok
    if( t_GridPort.t_Flag.InWorkNow == true && t_GridPort.t_Flag.InWorkPre == false)
    {
        t_GridPort.u16VoltRestDlyCnt        = 0;
        t_GridPort.t_Flag.VoltRestAct       = true;
    }

    // Grid NG
    if( t_GridPort.t_Flag.InWorkNow == false && t_GridPort.t_Flag.InWorkPre == true)
    {
        t_GridPort.u16VoltLossCnt           = 0;
        t_GridPort.t_Flag.VoltLoss          = true;

        t_GridPort.u16VoltLossDlyCnt        = 0;
        t_GridPort.t_Flag.VoltLossAct       = true;
    }
    t_GridPort.t_Flag.InWorkPre             = t_GridPort.t_Flag.InWorkNow;

    if( t_GridPort.t_Flag.VoltRestAct == true )
    {
        t_GridPort.u16VoltRestDlyCnt++;
        if( t_GridPort.u16VoltRestDlyCnt >= 3)
        {
            t_GridPort.u16VoltRestDlyCnt    = 0;
            t_GridPort.t_Flag.VoltRestAct   = false;
            OSEventSend(cPrioMsw,  eMsw_GridRest);
        }
    }
    else 
    {
        t_GridPort.u16VoltRestDlyCnt        = 0;
    }

    if( t_GridPort.t_Flag.VoltLossAct == true )
    {
        t_GridPort.u16VoltLossDlyCnt++;
        if( t_GridPort.u16VoltLossDlyCnt >= 3)
        {
            t_GridPort.u16VoltLossDlyCnt    = 0;
            t_GridPort.t_Flag.VoltLossAct   = false;
            OSEventSend(cPrioMsw,  eMsw_GridLost);
        }
    }
    else 
    {
        t_GridPort.u16VoltLossDlyCnt        = 0;
    }
}

void    sGrid_RmsCheck(void)
{
    int             CheckValue;
    unsigned int    FaultTimes,RecoverTimes;

    CheckValue      = sSample_GetRms(GridVolt);

    FaultTimes      = 10;
    RecoverTimes    = 125;

    if( t_GridPort.t_Flag.VoltLoss == false )
    {
        if( CheckValue > t_GridPort.i16RmsHiLossValue || CheckValue < t_GridPort.i16RmsLoLossValue )
        {
            t_GridPort.u16VoltLossCnt++;
        }
        else 
        {
            t_GridPort.u16VoltLossCnt   = 0;
        }

        if( t_GridPort.u16VoltLossCnt >= FaultTimes)
        {
            t_GridPort.u16VoltLossCnt   = 0;
            t_GridPort.t_Flag.VoltLoss  = true;
        }
    }
    else 
    {
        if( CheckValue <= t_GridPort.i16RmsHiRestValue && CheckValue >= t_GridPort.i16RmsLoRestValue )
        {
            t_GridPort.u16VoltLossCnt++;
        }
        else 
        {
            t_GridPort.u16VoltLossCnt   = 0;
        }

        if( t_GridPort.u16VoltLossCnt >= RecoverTimes)
        {
            t_GridPort.u16VoltLossCnt   = 0;
            t_GridPort.t_Flag.VoltLoss  = false;
        }
    }
}

void    sGrid_FreqCheck(void)
{
    int             CheckFreq,CheckVolt;
    unsigned int    FaultTimes,RecoverTimes;

    CheckVolt       = sSample_GetRms(GridVolt);
    CheckFreq       = t_GridPort.u16FreqReal;

    FaultTimes      = 10;
    RecoverTimes    = 125;

    if (CheckVolt < cVac50V) 
    {
        t_GridPort.u16FreqLossCnt   = 0;
        t_GridPort.t_Flag.FreqLoss  = false;
        return;
    }

    if( t_GridPort.t_Flag.FreqLoss == false )
    {
        if( CheckFreq > t_GridPort.i16FreqHiLossValue || CheckFreq < t_GridPort.i16FreqLoLossValue )
        {
            t_GridPort.u16FreqLossCnt++;
        }
        else 
        {
            t_GridPort.u16FreqLossCnt   = 0;
        }

        if( t_GridPort.u16FreqLossCnt >= FaultTimes)
        {
            t_GridPort.u16FreqLossCnt   = 0;
            t_GridPort.t_Flag.FreqLoss  = true;
        }
    }
    else 
    {
        if( CheckFreq <= t_GridPort.i16FreqHiRestValue && CheckFreq >= t_GridPort.i16FreqLoRestValue )
        {
            t_GridPort.u16FreqLossCnt++;
        }
        else 
        {
            t_GridPort.u16FreqLossCnt   = 0;
        }

        if( t_GridPort.u16FreqLossCnt >= RecoverTimes)
        {
            t_GridPort.u16FreqLossCnt   = 0;
            t_GridPort.t_Flag.FreqLoss  = false;
        }
    }
}

void    sGrid_ZeroRestDeal(void)
{
    if(t_GridPort.t_Flag.VoltRestAct == false)
    {
        return;
    }

    t_GridPort.u16VoltRestDlyCnt    = 0;
    t_GridPort.t_Flag.VoltRestAct   = false;
    OSEventSend(cPrioMsw, eMsw_GridRest);
}

void    sGrid_ZeroLossDeal(void)
{
    if(t_GridPort.t_Flag.VoltLossAct == false)
    {
        return;
    }

    t_GridPort.u16VoltLossDlyCnt    = 0;
    t_GridPort.t_Flag.VoltLossAct   = false;
    OSEventSend(cPrioMsw, eMsw_GridLost);
}

void    sGrid_WaveLossDetect(void)
{
    int i16DataA;
    int i16WaveLossTime,i16ZeroLossTime;
    int i16VoltLossValue;

    i16WaveLossTime     = cISRCnt5ms;
    i16ZeroLossTime     = cISRCnt15ms;
    i16VoltLossValue    = cVac80V;      //低压版80V,高压板150V

    i16DataA            = abs(sAdc_GetReal(GridVolt));

    if(t_GridPort.t_Flag.WaveAdcHiLoss == false)
    {
        if(i16DataA > t_GridPort.i16WaveHiLossValue)
        {
            t_GridPort.u32WaveAdcHiLossCnt++;
            if(t_GridPort.u32WaveAdcHiLossCnt >= i16WaveLossTime )
            {
                t_GridPort.u32WaveAdcHiLossCnt  = 0;
                t_GridPort.t_Flag.WaveAdcHiLoss = true;
            }
        }
        else 
        {
            if(t_GridPort.u32WaveAdcHiLossCnt > 0)
            {
                t_GridPort.u32WaveAdcHiLossCnt--;
            }
        }
    }
    else 
    {
        if( i16DataA < t_GridPort.i16WaveHiRestValue)
        {
            t_GridPort.u32WaveAdcHiLossCnt  += 5;
            if( t_GridPort.u32WaveAdcHiLossCnt >= cISRCnt1s )
            {
                t_GridPort.u32WaveAdcHiLossCnt  = 0;
                t_GridPort.t_Flag.WaveAdcHiLoss = false;
            }
        }
        else 
        {
            if(t_GridPort.u32WaveAdcHiLossCnt > 0)
            {
                t_GridPort.u32WaveAdcHiLossCnt--;
            }
        }
    }

    if(t_GridPort.t_Flag.WaveAdcLoLoss == false)
    {
        if(i16DataA > t_GridPort.i16WaveLoLossValue)
        {
            t_GridPort.u32WaveAdcLoLossCnt++;
            if(t_GridPort.u32WaveAdcLoLossCnt < i16WaveLossTime )
            {
                t_GridPort.u32WaveAdcLoLossCnt  = 0;
                t_GridPort.t_Flag.WaveAdcLoLoss = true;
            }
        }
        else 
        {
            if(t_GridPort.u32WaveAdcLoLossCnt > 0)
            {
                t_GridPort.u32WaveAdcLoLossCnt--;
            }
        }
    }
    else 
    {
        if( i16DataA < t_GridPort.i16WaveLoRestValue)
        {
            t_GridPort.u32WaveAdcLoLossCnt  += 5;
            if( t_GridPort.u32WaveAdcLoLossCnt >= cISRCnt1s )
            {
                t_GridPort.u32WaveAdcLoLossCnt  = 0;
                t_GridPort.t_Flag.WaveAdcLoLoss = false;
            }
        }
        else 
        {
            if( i16DataA < cVac5V )
            {
                if(t_GridPort.u32WaveAdcLoLossCnt > 0)
                {
                    t_GridPort.u32WaveAdcLoLossCnt--;
                }                
            }

        }
    }

    if(t_GridPort.t_Flag.WavePosLoss == false)
    {
        if( i16DataA >= i16VoltLossValue )
        {
            t_GridPort.u32WavePosLossCnt    = 0;
        }
        else 
        {
            t_GridPort.u32WavePosLossCnt++;
        }

        if(t_GridPort.u32WavePosLossCnt >= cISRCnt10ms)
        {
            t_GridPort.u32WavePosLossCnt    = 0;
            t_GridPort.t_Flag.WavePosLoss   = true;

            t_GridPort.u16WaveZeroRestCnt   = 0;
            t_GridPort.t_Flag.WaveZeroLoss  = true;
        }
    }
    else 
    {
        t_GridPort.u32WavePosLossCnt++;
        if( t_GridPort.u32WavePosLossCnt >= cISRCnt200ms )
        {
            t_GridPort.u32WavePosLossCnt    = 0;
            t_GridPort.t_Flag.WavePosLoss   = false;
        }
    }

    t_GridPort.t_Flag.VoltPosNow = sPLL_GetGridPos();
    if(t_GridPort.t_Flag.VoltPosPre != t_GridPort.t_Flag.VoltPosNow)
    {
        t_GridPort.u16WaveZeroLostCnt       = 0;
    }
    t_GridPort.t_Flag.VoltPosPre = t_GridPort.t_Flag.VoltPosNow;

    t_GridPort.u16WaveZeroLostCnt++;
    if(t_GridPort.u16WaveZeroLostCnt > 1000)
    {
        t_GridPort.u16WaveZeroLostCnt       = 1000;
    }

    if( t_GridPort.t_Flag.WaveZeroLoss == false )
    {
        if( t_GridPort.u16WaveZeroLostCnt >= i16ZeroLossTime )
        {
            t_GridPort.t_Flag.WaveZeroLoss  = true;
        }
    }
    else 
    {
        t_GridPort.u16WaveZeroRestCnt++;
        if(t_GridPort.u16WaveZeroRestCnt > cISRCnt200ms)
        {
            t_GridPort.u16WaveZeroRestCnt   = cISRCnt200ms;
        }

        if( t_GridPort.u16WaveZeroLostCnt < 5 && t_GridPort.u16WaveZeroRestCnt >= cISRCnt200ms)
        {
            t_GridPort.u16WaveZeroRestCnt   = 0;
            t_GridPort.t_Flag.WaveZeroLoss  = false;
        }
    }

    t_GridPort.t_Flag.Waveloss =    t_GridPort.t_Flag.WaveAdcHiLoss |
                                    t_GridPort.t_Flag.WaveAdcLoLoss |
                                    t_GridPort.t_Flag.WaveZeroLoss  |
                                    t_GridPort.t_Flag.WavePosLoss;

    if( t_GridPort.t_Flag.Waveloss == true )
    {
        sFault_SetAlarm(eAlarm_GridVoltErr);
        if( sMsw_GetPfcMode() == true)
        {
            OSEventSend(cPrioMsw, eMsw_GridLost);
        }
    }
}

void    sGrid_FreqCal(unsigned int Freq)
{
    t_GridPort.u16FreqReal = Freq;
}

signed int    sGrid_GetFreq(void)
{
    return t_GridPort.u16FreqReal;
}
