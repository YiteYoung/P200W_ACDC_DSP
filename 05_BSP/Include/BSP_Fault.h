#ifndef _BSP_FAULT_H
#define _BSP_FAULT_H

#ifdef BSP_FAULT_FUNCTION_G
#define BSP_FAULT_FUNCTION    extern
#else
#define BSP_FAULT_FUNCTION
#endif

#ifndef true
#define true    1
#endif

#ifndef false
#define false   0
#endif

typedef struct
{
    unsigned int    Lo;
    unsigned int    Hi;
}HARF_t;

typedef enum
{
    eAlarm_Start = 0,

    eAlarm_BatVoltHi,       // 00
    eAlarm_BatVoltLo,       // 01
    eAlarm_GridVoltErr,     // 02
    eAlarm_GridFreqErr,     // 03

    eAlarm_BusLow,          // 04
    eAlarm_SciACommErr,     // 05
    eAlarm_SciBCommErr,     // 06
    eAlarm_NtcLost,         // 07

    eAlarm_WaitParaSet,     // 08
    eAlarm_TemperatureHi,   // 09
    eAlarm_TemperatureLo,   // 10
    eAlarm_OverLoad,        // 11

    eAlarm_bResv12,
    eAlarm_bResv13,
    eAlarm_bResv14,
    eAlarm_bResv15,

    eAlarm_bResv16,
    eAlarm_bResv17,
    eAlarm_bResv18,
    eAlarm_bResv19,

    eAlarm_bResv20,
    eAlarm_bResv21,
    eAlarm_bResv22,
    eAlarm_bResv23,

    eAlarm_bResv24,
    eAlarm_bResv25,
    eAlarm_bResv26,
    eAlarm_bResv27,

    eAlarm_bResv28,
    eAlarm_bResv29,
    eAlarm_bResv30,
    eAlarm_bResv31,

    eAlarm_End
}Alarm_e;

typedef struct
{
     unsigned int BatVoltHi     :1;         // 00
     unsigned int BatVoltLo     :1;         // 01
     unsigned int GridVoltErr   :1;         // 02
     unsigned int GridFreqErr   :1;         // 03

     unsigned int BusLow        :1;         // 04
     unsigned int SciACommErr   :1;         // 05
     unsigned int SciBCommErr   :1;         // 06
     unsigned int NtcLost       :1;         // 07

     unsigned int WaitParaSet   :1;         // 08
     unsigned int TemperatureHi :1;         // 09
     unsigned int TemperatureLo :1;         // 10
     unsigned int OverLoad      :1;         // 11

     unsigned int bResv12       :1;
     unsigned int bResv13       :1;
     unsigned int bResv14       :1;
     unsigned int bResv15       :1;

     unsigned int bResv16       :1;
     unsigned int bResv17       :1;
     unsigned int bResv18       :1;
     unsigned int bResv19       :1;

     unsigned int bResv20       :1;
     unsigned int bResv21       :1;
     unsigned int bResv22       :1;
     unsigned int bResv23       :1;

     unsigned int bResv24       :1;
     unsigned int bResv25       :1;
     unsigned int bResv26       :1;
     unsigned int bResv27       :1;

     unsigned int bResv28       :1;
     unsigned int bResv29       :1;
     unsigned int bResv30       :1;
     unsigned int bResv31       :1;   
}Alarm_t;

typedef union
{
    unsigned long   AlarmCode;
    HARF_t          Harf;
    Alarm_t         bit;
}Alarm_u;

typedef enum
{
    eFault_Start = 0,

    eFault_BatVoltHi,       // 00
    eFault_BatVoltLo,       // 01
    eFault_GridVoltErr,     // 02
    eFault_GridFreqErr,     // 03

    eFault_BusLow,          // 04
    eFault_SciACommErr,     // 05
    eFault_SciBCommErr,     // 06
    eFault_NtcLost,         // 07

    eFault_WaitParaSet,     // 08
    eFault_TemperatureHi,   // 09
    eFault_TemperatureLo,   // 10
    eFault_OverLoad,        // 11

    eFault_bResv12,
    eFault_bResv13,
    eFault_bResv14,
    eFault_bResv15,

    eFault_bResv16,
    eFault_bResv17,
    eFault_bResv18,
    eFault_bResv19,

    eFault_bResv20,
    eFault_bResv21,
    eFault_bResv22,
    eFault_bResv23,

    eFault_bResv24,
    eFault_bResv25,
    eFault_bResv26,
    eFault_bResv27,

    eFault_bResv28,
    eFault_bResv29,
    eFault_bResv30,
    eFault_bResv31,

    eFault_End
}Fault_e;

typedef struct
{
    unsigned int bResv0         :1;
    unsigned int bResv1         :1;         // 00
    unsigned int bResv2         :1;         // 01
    unsigned int bResv3         :1;         // 02

    unsigned int bResv4         :1;         // 03
    unsigned int bResv5         :1;         // 04
    unsigned int bResv6         :1;         // 05
    unsigned int bResv7         :1;         // 06

    unsigned int bResv8         :1;         // 07
    unsigned int bResv9         :1;         // 08
    unsigned int bResv10        :1;         // 09
    unsigned int bResv11        :1;         // 10

    unsigned int bResv12        :1;
    unsigned int bResv13        :1;
    unsigned int bResv14        :1;
    unsigned int bResv15        :1;

    unsigned int bResv16        :1;
    unsigned int bResv17        :1;
    unsigned int bResv18        :1;
    unsigned int bResv19        :1;

    unsigned int bResv20        :1;
    unsigned int bResv21        :1;
    unsigned int bResv22        :1;
    unsigned int bResv23        :1;

    unsigned int bResv24        :1;
    unsigned int bResv25        :1;
    unsigned int bResv26        :1;
    unsigned int bResv27        :1;

    unsigned int bResv28        :1;
    unsigned int bResv29        :1;
    unsigned int bResv30        :1;
    unsigned int bResv31        :1;   
}Fault_t;

typedef union
{
    unsigned long   FaultCode;
    HARF_t          Harf;
    Fault_t         bit;
}Fault_u;

typedef struct
{
    Alarm_u         u_Alarm;
    Fault_u         u_Fault;
}Alarm_Fault_t;

BSP_FAULT_FUNCTION    void            sFault_ClrAllFault              (void);
BSP_FAULT_FUNCTION    void            sFault_ClrAllAlarm              (void);

BSP_FAULT_FUNCTION    unsigned long   sFault_GetFaultCode_All         (void);
BSP_FAULT_FUNCTION    signed int      sFault_GetFaultCode_Lo          (void);
BSP_FAULT_FUNCTION    signed int      sFault_GetFaultCode_Hi          (void);

BSP_FAULT_FUNCTION    unsigned long   sFault_GetAlarmCode_All         (void);
BSP_FAULT_FUNCTION    signed int      sFault_GetAlarmCode_Lo          (void);
BSP_FAULT_FUNCTION    signed int      sFault_GetAlarmCode_Hi          (void);

BSP_FAULT_FUNCTION    unsigned char   sFault_GetFaultBit              (unsigned int eFualtId);
BSP_FAULT_FUNCTION    unsigned char   sFault_GetAlarmBit              (unsigned int eAlarmId);

BSP_FAULT_FUNCTION    void            sFault_SetFault                 (unsigned int eFaultId);
BSP_FAULT_FUNCTION    void            sFault_ClrFault                 (unsigned int eFaultId);

BSP_FAULT_FUNCTION    void            sFault_SetAlarm                 (unsigned int eAlarmId);
BSP_FAULT_FUNCTION    void            sFault_ClrAlarm                 (unsigned int eAlarmId);

#endif
