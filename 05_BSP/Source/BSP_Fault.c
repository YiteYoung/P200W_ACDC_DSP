#include "BSP_Fault.h"

#define sSetBIT(x,n)        ( (x) |=  ( (unsigned long)1 << (n)) )
#define sClrBIT(x,n)        ( (x) &= ~( (unsigned long)1 << (n)) )

static Alarm_Fault_t    t_FaultAlarm;

void            sFault_ClrAllFault      (void)          { t_FaultAlarm.u_Fault.FaultCode = 0x0000; }
void            sFault_ClrAllAlarm      (void)          { t_FaultAlarm.u_Alarm.AlarmCode = 0x0000; }

unsigned long   sFault_GetFaultCode_All (void)          { return t_FaultAlarm.u_Fault.FaultCode;}
signed int      sFault_GetFaultCode_Lo  (void)          { return t_FaultAlarm.u_Fault.Harf.Lo;}
signed int      sFault_GetFaultCode_Hi  (void)          { return t_FaultAlarm.u_Fault.Harf.Hi;}

unsigned long   sFault_GetAlarmCode_All (void)          { return t_FaultAlarm.u_Alarm.AlarmCode;}
signed int      sFault_GetAlarmCode_Lo  (void)          { return t_FaultAlarm.u_Alarm.Harf.Lo;}
signed int      sFault_GetAlarmCode_Hi  (void)          { return t_FaultAlarm.u_Alarm.Harf.Hi;}

unsigned char   sFault_GetFaultBit(unsigned int eFaultId)
{
    unsigned int u16TempA;
    if( (eFaultId <= eFault_Start) || (eFaultId >= eFault_End) )
    {
        return false;
    }

    u16TempA = (t_FaultAlarm.u_Fault.FaultCode >> (eFaultId - 1)) & 0x0001;

    if( u16TempA != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}    

unsigned char   sAlarm_GetAlarmBit(unsigned int eAlarmId)
{
    unsigned int u16TempA;
    if( (eAlarmId <= eAlarm_Start) || (eAlarmId >= eAlarm_End) )
    {
        return false;
    }

    u16TempA = (t_FaultAlarm.u_Alarm.AlarmCode >> (eAlarmId - 1)) & 0x0001;

    if( u16TempA != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void sFault_SetFault(unsigned int eFaultId)
{
    if( (eFaultId <= eFault_Start) || (eFaultId >= eFault_End) )
    {
        return;
    }

    sSetBIT(t_FaultAlarm.u_Fault.FaultCode, (eFaultId - 1));
}

void sFault_ClrFault(unsigned int eFaultId)
{
    if( (eFaultId <= eFault_Start) || (eFaultId >= eFault_End) )
    {
        return;
    }

    sClrBIT(t_FaultAlarm.u_Fault.FaultCode, (eFaultId - 1));
}

void sFault_SetAlarm(unsigned int eAlarmId)
{
    if( (eAlarmId <= eAlarm_Start) || (eAlarmId >= eAlarm_End) )
    {
        return;
    }

    sSetBIT(t_FaultAlarm.u_Alarm.AlarmCode, (eAlarmId - 1));
}

void sFault_ClrAlarm(unsigned int eAlarmId)
{
    if( (eAlarmId <= eAlarm_Start) || (eAlarmId >= eAlarm_End) )
    {
        return;
    }

    sClrBIT(t_FaultAlarm.u_Alarm.AlarmCode, (eAlarmId - 1));
}

