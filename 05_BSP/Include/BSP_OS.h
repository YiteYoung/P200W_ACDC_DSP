#ifndef _BSP_OS_H
#define _BSP_OS_H

#ifdef BSP_OS_FUNCTION_G
#define BSP_OS_FUNCTION extern
#else
#define BSP_OS_FUNCTION
#endif

#define     TASK_MAX_NUM            4

#define     cPrioInv                0
#define     cPrioMsw                1
#define     cPrioFun                2
#define     cPrioSci                3

#define     eInitEvent              0
#define     eTimer                  0

#define     eMsw_Boot               1
#define     eMsw_Fault              2
#define     eMsw_ClrFault           3
#define     eMsw_Alarm              4
#define     eMsw_InvOn              5
#define     eMsw_InvOff             6
#define     eMsw_ChgOn              7
#define     eMsw_ChgOff             8
#define     eMsw_GridOn             9
#define     eMsw_GridOff            10
#define     eMsw_GridLost           11
#define     eMsw_GridRest           12

#define     eInv_InvZero            1
#define     eInv_GridZero           2

#define     eFun_InvZero            1
#define     eFun_GridZero           2

#define     eSci_SciAOff            1
#define     eSci_SciBOff            2

#define     OS_ENTER_CRITICAL()     asm(" SETC INTM")
#define     OS_EXIT_CRITICAL()      asm(" CLRC INTM")

typedef unsigned int INTOS;
typedef unsigned int TASK_EVENT;
typedef   signed int TASK_INT;

BSP_OS_FUNCTION     void        OSInit      (void);
BSP_OS_FUNCTION     TASK_EVENT  OSEventPend (INTOS Prio);
BSP_OS_FUNCTION     TASK_EVENT  OSEventGet  (INTOS Prio);
BSP_OS_FUNCTION     void        OSEventSend (INTOS Prio, INTOS EventId);
BSP_OS_FUNCTION     void        OSEventClr  (INTOS Prio);

#endif
