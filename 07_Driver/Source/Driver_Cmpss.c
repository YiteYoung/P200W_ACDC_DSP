#include "Driver_Cmpss.h"

volatile struct CMPSS_REGS* const CMP_CH[1] = 
{
    &Cmpss1Regs
};

volatile struct CMPSS_LITE_REGS* const CMP_LITE_CH[CMP_CH_END - 1] =
{
    &CmpssLite2Regs,
    &CmpssLite3Regs,
    &CmpssLite4Regs
};

void    sDrv_InitCmpss(CMPCONFIG_t t_CMPSS_Para)
{
    unsigned int CH;

    CH = t_CMPSS_Para.CMP_CH;

    if( sCMP_CH_Check(CH) == 0 )
    {
        return;
    }

    switch( CH )
    {
        case CMP_CH_01:
            sDrv_RegCmpss(t_CMPSS_Para);
            break;

        case CMP_CH_02:
        case CMP_CH_03:
        case CMP_CH_04:
            sDrv_RegCmpss_Lite(t_CMPSS_Para);
            break;

        default:
            break;
    }
}

void    sDrv_RegCmpss(CMPCONFIG_t t_CMPSS_Para)
{
    unsigned int    CH,DAC_Hi,DAC_Lo;

    CH = t_CMPSS_Para.CMP_CH;

    if( sCMP_CH_Check(CH) == 0 )
    {
        return;
    }

    DAC_Hi = t_CMPSS_Para.DAC_Hi_Value;
    DAC_Lo = t_CMPSS_Para.DAC_Lo_Value;
    if(DAC_Hi > cMaxDACValue)   DAC_Hi = cMaxDACValue;
    if(DAC_Lo > cMaxDACValue)   DAC_Lo = cMaxDACValue;

    EALLOW;
    (*CMP_CH[CH]).DACHVALS.bit.DACVAL     = DAC_Hi;
    (*CMP_CH[CH]).DACLVALS.bit.DACVAL     = DAC_Lo;

    (*CMP_CH[CH]).COMPCTL.bit.COMPHSOURCE       = 0;
    (*CMP_CH[CH]).COMPCTL.bit.COMPHINV          = t_CMPSS_Para.CMP_Hi_INV & 0x0001;
    (*CMP_CH[CH]).COMPCTL.bit.ASYNCHEN          = 0;

    (*CMP_CH[CH]).COMPCTL.bit.COMPLSOURCE       = 0;
    (*CMP_CH[CH]).COMPCTL.bit.COMPLINV          = t_CMPSS_Para.CMP_Lo_INV & 0x0001;
    (*CMP_CH[CH]).COMPCTL.bit.ASYNCLEN          = 0;

    (*CMP_CH[CH]).COMPDACHCTL.bit.DACSOURCE     = 0;
    (*CMP_CH[CH]).COMPDACHCTL.bit.RAMPSOURCE    = 0;
    (*CMP_CH[CH]).COMPDACHCTL.bit.RAMPLOADSEL   = 0;
    (*CMP_CH[CH]).COMPDACHCTL.bit.SWLOADSEL     = 0;
    (*CMP_CH[CH]).COMPDACHCTL.bit.BLANKEN       = 0;
    (*CMP_CH[CH]).COMPDACHCTL.bit.BLANKSOURCE   = 0;
    (*CMP_CH[CH]).COMPDACHCTL.bit.FREESOFT      = 0;

    (*CMP_CH[CH]).CTRIPHFILCLKCTL               = t_CMPSS_Para.CLK_Prescale;
    (*CMP_CH[CH]).CTRIPHFILCTL.bit.SAMPWIN      = t_CMPSS_Para.SAMP_WIN & 0x001F;
    (*CMP_CH[CH]).CTRIPHFILCTL.bit.THRESH       = t_CMPSS_Para.Thre_SH & 0x001F;
    (*CMP_CH[CH]).CTRIPHFILCTL.bit.FILINIT      = 1;

    (*CMP_CH[CH]).CTRIPLFILCLKCTL               = t_CMPSS_Para.CLK_Prescale;
    (*CMP_CH[CH]).CTRIPLFILCTL.bit.SAMPWIN      = t_CMPSS_Para.SAMP_WIN & 0x001F;
    (*CMP_CH[CH]).CTRIPLFILCTL.bit.THRESH       = t_CMPSS_Para.Thre_SH & 0x001F;
    (*CMP_CH[CH]).CTRIPLFILCTL.bit.FILINIT      = 1;

    (*CMP_CH[CH]).COMPCTL.bit.CTRIPHSEL         = t_CMPSS_Para.PWMXBAR_Hi_Sel & 0x0003;
    (*CMP_CH[CH]).COMPCTL.bit.CTRIPLSEL         = t_CMPSS_Para.PWMXBAR_Lo_Sel & 0x0003;
    (*CMP_CH[CH]).COMPCTL.bit.CTRIPOUTHSEL      = t_CMPSS_Para.OUTXBAR_Hi_Sel & 0x0003;
    (*CMP_CH[CH]).COMPCTL.bit.CTRIPOUTHSEL      = t_CMPSS_Para.OUTXBAR_Lo_Sel & 0x0003;

    (*CMP_CH[CH]).COMPHYSCTL.bit.COMPHYS        = t_CMPSS_Para.CMP_PHYS & 0x000F;

    (*CMP_CH[CH]).COMPSTSCLR.bit.HLATCHCLR      = 1;
    (*CMP_CH[CH]).COMPSTSCLR.bit.LLATCHCLR      = 1;

    (*CMP_CH[CH]).COMPCTL.bit.COMPDACE          = 1;

    switch (CH) 
    {
        case CMP_CH_01:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP1HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP1LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        case CMP_CH_02:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP2HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP2LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        case CMP_CH_03:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP3HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP3LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        case CMP_CH_04:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP4HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP4LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        default:
            break;
    }

    asm(" RPT #100 || NOP");
    (*CMP_CH[CH]).COMPSTSCLR.bit.HLATCHCLR      = 1;
    (*CMP_CH[CH]).COMPSTSCLR.bit.LLATCHCLR      = 1;
    EDIS;
}

void sDrv_RegCmpss_Lite(CMPCONFIG_t t_CMPSS_Para)
{
    unsigned int    CH,DAC_Hi,DAC_Lo;

    CH = t_CMPSS_Para.CMP_CH;

    if( sCMP_CH_Check(CH) == 0 || CH <= CMP_CH_01)
    {
        return;
    }

    CH = t_CMPSS_Para.CMP_CH - 1;

    DAC_Hi = t_CMPSS_Para.DAC_Hi_Value;
    DAC_Lo = t_CMPSS_Para.DAC_Lo_Value;
    if(DAC_Hi > cMaxDACValue)   DAC_Hi = cMaxDACValue;
    if(DAC_Lo > cMaxDACValue)   DAC_Lo = cMaxDACValue;

    EALLOW;
    (*CMP_LITE_CH[CH]).DACHVALS.bit.DACVAL          = DAC_Hi;
    (*CMP_LITE_CH[CH]).DACLVALS.bit.DACVAL          = DAC_Lo;

    (*CMP_LITE_CH[CH]).COMPCTL.bit.COMPDACE          = 0;

    (*CMP_LITE_CH[CH]).COMPCTL.bit.COMPHSOURCE       = 0;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.COMPHINV          = t_CMPSS_Para.CMP_Hi_INV & 0x0001;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.ASYNCHEN          = 0;

    (*CMP_LITE_CH[CH]).COMPCTL.bit.COMPLSOURCE       = 0;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.COMPLINV          = t_CMPSS_Para.CMP_Lo_INV & 0x0001;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.ASYNCLEN          = 0;

    (*CMP_LITE_CH[CH]).COMPDACHCTL.bit.RAMPSOURCE    = 0;
    (*CMP_LITE_CH[CH]).COMPDACHCTL.bit.SWLOADSEL     = 0;
    (*CMP_LITE_CH[CH]).COMPDACHCTL.bit.BLANKEN       = 0;
    (*CMP_LITE_CH[CH]).COMPDACHCTL.bit.BLANKSOURCE   = 0;

    (*CMP_LITE_CH[CH]).CTRIPHFILCLKCTL               = t_CMPSS_Para.CLK_Prescale;
    (*CMP_LITE_CH[CH]).CTRIPHFILCTL.bit.SAMPWIN      = t_CMPSS_Para.SAMP_WIN & 0x001F;
    (*CMP_LITE_CH[CH]).CTRIPHFILCTL.bit.THRESH       = t_CMPSS_Para.Thre_SH & 0x001F;
    (*CMP_LITE_CH[CH]).CTRIPHFILCTL.bit.FILINIT      = 1;

    (*CMP_LITE_CH[CH]).CTRIPLFILCLKCTL               = t_CMPSS_Para.CLK_Prescale;
    (*CMP_LITE_CH[CH]).CTRIPLFILCTL.bit.SAMPWIN      = t_CMPSS_Para.SAMP_WIN & 0x001F;
    (*CMP_LITE_CH[CH]).CTRIPLFILCTL.bit.THRESH       = t_CMPSS_Para.Thre_SH & 0x001F;
    (*CMP_LITE_CH[CH]).CTRIPLFILCTL.bit.FILINIT      = 1;

    (*CMP_LITE_CH[CH]).COMPCTL.bit.CTRIPHSEL         = t_CMPSS_Para.PWMXBAR_Hi_Sel & 0x0003;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.CTRIPLSEL         = t_CMPSS_Para.PWMXBAR_Lo_Sel & 0x0003;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.CTRIPOUTHSEL      = t_CMPSS_Para.OUTXBAR_Hi_Sel & 0x0003;
    (*CMP_LITE_CH[CH]).COMPCTL.bit.CTRIPOUTHSEL      = t_CMPSS_Para.OUTXBAR_Lo_Sel & 0x0003;

    (*CMP_LITE_CH[CH]).COMPHYSCTL.bit.COMPHYS        = t_CMPSS_Para.CMP_PHYS & 0x000F;

    (*CMP_LITE_CH[CH]).COMPSTSCLR.bit.HLATCHCLR      = 1;
    (*CMP_LITE_CH[CH]).COMPSTSCLR.bit.LLATCHCLR      = 1;

    (*CMP_LITE_CH[CH]).COMPCTL.bit.COMPDACE          = 1;

    switch (CH) 
    {
        case CMP_CH_01:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP1HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP1LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        case CMP_CH_02:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP2HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP2LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        case CMP_CH_03:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP3HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP3LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        case CMP_CH_04:
            AnalogSubsysRegs.CMPHPMXSEL.bit.CMP4HPMXSEL = t_CMPSS_Para.CMP_PIN_Hi_Sel & 0x0007;
            AnalogSubsysRegs.CMPLPMXSEL.bit.CMP4LPMXSEL = t_CMPSS_Para.CMP_PIN_Lo_Sel & 0x0007;
            break;

        default:
            break;
    }

    asm(" RPT #100 || NOP");
    (*CMP_LITE_CH[CH]).COMPSTSCLR.bit.HLATCHCLR      = 1;
    (*CMP_LITE_CH[CH]).COMPSTSCLR.bit.LLATCHCLR      = 1;
    EDIS;    
}

void    sDrv_SetCmpss_DAC(unsigned int CH, unsigned int DAC_Hi, unsigned int DAC_Lo)
{
    if(sCMP_CH_Check(CH) == 0)
    {
        return;
    }

    if(DAC_Hi > cMaxDACValue)   DAC_Hi = cMaxDACValue;
    if(DAC_Lo > cMaxDACValue)   DAC_Lo = cMaxDACValue;

    switch (CH) 
    {
        case CMP_CH_01:
            Cmpss1Regs.DACHVALS.bit.DACVAL          = DAC_Hi;
            Cmpss1Regs.DACLVALS.bit.DACVAL          = DAC_Lo;
            break;

        case CMP_CH_02:
            CmpssLite2Regs.DACHVALS.bit.DACVAL      = DAC_Hi;
            CmpssLite2Regs.DACLVALS.bit.DACVAL      = DAC_Lo;
            break;

        case CMP_CH_03:
            CmpssLite3Regs.DACHVALS.bit.DACVAL      = DAC_Hi;
            CmpssLite3Regs.DACLVALS.bit.DACVAL      = DAC_Lo;
            break;

        case CMP_CH_04:
            CmpssLite4Regs.DACHVALS.bit.DACVAL      = DAC_Hi;
            CmpssLite4Regs.DACLVALS.bit.DACVAL      = DAC_Lo;
            break;

        default:
            break;
    }
}

unsigned int    sDrv_GetHiLatchSTS(unsigned int CH)
{
    if(sCMP_CH_Check(CH) == 0)
    {
        return 0;
    }

    switch (CH) 
    {
        case CMP_CH_01:
            return( Cmpss1Regs.COMPSTS.bit.COMPHLATCH);

        case CMP_CH_02:
            return( CmpssLite2Regs.COMPSTS.bit.COMPHLATCH);

        case CMP_CH_03:
            return( CmpssLite3Regs.COMPSTS.bit.COMPHLATCH);

        case CMP_CH_04:
            return( CmpssLite4Regs.COMPSTS.bit.COMPHLATCH);

        default:
            return(0);
    }
}

unsigned int    sDrv_GetLoLatchSTS(unsigned int CH)
{
    if(sCMP_CH_Check(CH) == 0)
    {
        return 0;
    }

    switch (CH) 
    {
        case CMP_CH_01:
            return( Cmpss1Regs.COMPSTS.bit.COMPLLATCH);

        case CMP_CH_02:
            return( CmpssLite2Regs.COMPSTS.bit.COMPLLATCH);

        case CMP_CH_03:
            return( CmpssLite3Regs.COMPSTS.bit.COMPLLATCH);

        case CMP_CH_04:
            return( CmpssLite4Regs.COMPSTS.bit.COMPLLATCH);

        default:
            return(0);
    }
}


void    sDrv_ClrHiLatchSTS(unsigned int CH)
{
    if(sCMP_CH_Check(CH) == 0)
    {
        return;
    }

    switch (CH) 
    {
        case CMP_CH_01:
            Cmpss1Regs.COMPSTSCLR.bit.HLATCHCLR = 1;
            break;

        case CMP_CH_02:
            CmpssLite2Regs.COMPSTSCLR.bit.HLATCHCLR = 1;
            break;

        case CMP_CH_03:
            CmpssLite3Regs.COMPSTSCLR.bit.HLATCHCLR = 1;
            break;

        case CMP_CH_04:
            CmpssLite4Regs.COMPSTSCLR.bit.HLATCHCLR = 1;

        default:
            break;
    }
}

void    sDrv_ClrLoLatchSTS(unsigned int CH)
{
    if(sCMP_CH_Check(CH) == 0)
    {
        return;
    }

    switch (CH) 
    {
        case CMP_CH_01:
            Cmpss1Regs.COMPSTSCLR.bit.LLATCHCLR = 1;
            break;

        case CMP_CH_02:
            CmpssLite2Regs.COMPSTSCLR.bit.LLATCHCLR = 1;
            break;

        case CMP_CH_03:
            CmpssLite3Regs.COMPSTSCLR.bit.LLATCHCLR = 1;
            break;

        case CMP_CH_04:
            CmpssLite4Regs.COMPSTSCLR.bit.LLATCHCLR = 1;

        default:
            break;
    }
}
