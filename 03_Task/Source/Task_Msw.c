#include "UserHeader.h"

void                sPowerOnMode        (void);
void                sStandByMode        (void);
void                sInvMode            (void);
void                sPfcMode            (void);
void                sFaultMode          (void);
void                sTestMode           (void);

void                sRlySoft            (void);
void                sInvSoft            (void);
void                sPfcSoft            (void);
void                sChgSoft            (void);
void                sDcgSoft            (void);

void                sFastInv            (void);
void                sFastPfc            (void);

void                sByPass             (void);

unsigned char       sPfc_OnChk          (void);
unsigned char       sInv_OnChk          (void);
unsigned char       sBypass_OnChk       (void);
unsigned char       sEv_OnChk           (void);

void                sSetPfcMode         (void);
void                sSetInvMode         (void);

void                sInvOutRly          (void);
void                sEvOutRly           (void);

void                sOff_Inv            (void);

static signed int   i16NowSYSMode;
static signed int   i16PreSYSMode;
static signed int   i16BakSYSMode;

static signed int   i16SYSStauts;
// static signed int   i16ShutDownFlag;


void    sMswTask(void)
{
    static unsigned int MswInit_Ok;
    if(MswInit_Ok == false)
    {
        MswInit_Ok = true;

        i16NowSYSMode   = cPowerOnMode;
        i16PreSYSMode   = i16NowSYSMode;
        i16BakSYSMode   = i16NowSYSMode;

        i16SYSStauts    = SYS_Off;

        OSEventClr(cPrioMsw);
    }

    if( i16BakSYSMode != i16NowSYSMode )
    {
        i16BakSYSMode = i16NowSYSMode;

        OSEventClr(cPrioMsw);
    }

    if( i16NowSYSMode == cPowerOnMode )
    {
        sPowerOnMode();
    }
    else if( i16NowSYSMode == cStandByMode )
    {
        sStandByMode();
    }
    else if( i16NowSYSMode == cInvMode  )
    {
        sInvMode();
    }
    else if( i16NowSYSMode == cPfcMode  )
    {
        sPfcMode();
    }
    else if( i16NowSYSMode == cTestMode )  
    {
        sTestMode();   
    }
    else if( i16NowSYSMode == cBootMode )
    {
        // sBootMode();
    }
    else 
    {
        sFaultMode();
    }

    
}

void    sPowerOnMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sStandByMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sInvMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sPfcMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sFaultMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}

void    sTestMode(void)
{
    TASK_EVENT event;

    event = OSEventPend(cPrioMsw);

    if( event == eInitEvent )
    {

    }

    if( event & ((TASK_EVENT)1 << eMsw_InvOn))
    {

    }
}





unsigned int    sMsw_GetNowMode(void)
{
    return i16NowSYSMode;
}

unsigned int    sMsw_GetPreMode(void)
{
    return i16PreSYSMode;
}

unsigned int    sMsw_GetStatus(void)
{
    return i16SYSStauts;
}

unsigned int    sMsw_GetPfcMode(void)
{
    if( i16NowSYSMode == cPfcMode)
    {
        return true;
    }
    else
    {
        return false;
    }
}
