#include "f28x_project.h"
#include "BSP_EPwm.h"

inline  static  void    sInitEPwm1  (void);
inline  static  void    sInitEPwm2  (void);
inline  static  void    sInitEPwm3  (void);
inline  static  void    sInitEPwm4  (void);
inline  static  void    sInitEPwm5  (void);
inline  static  void    sInitEPwm6  (void);
inline  static  void    sInitEPwm7  (void);

void  sInitEPwm(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC        = 0;        // Stop all the TB Clocks
    EDIS;

    // AQ Software Force Load immediately
    EPwm1Regs.AQSFRC.bit.RLDCSF             = 3;
    EPwm2Regs.AQSFRC.bit.RLDCSF             = 3;
    EPwm3Regs.AQSFRC.bit.RLDCSF             = 3;
    EPwm4Regs.AQSFRC.bit.RLDCSF             = 3;
    EPwm5Regs.AQSFRC.bit.RLDCSF             = 3;
    EPwm6Regs.AQSFRC.bit.RLDCSF             = 3;
    EPwm7Regs.AQSFRC.bit.RLDCSF             = 3;

    // TZ
    EALLOW;
    EPwm1Regs.TZFRC.bit.OST                 = 1;
    EPwm2Regs.TZFRC.bit.OST                 = 1;
    EPwm3Regs.TZFRC.bit.OST                 = 1;
    EPwm4Regs.TZFRC.bit.OST                 = 1;
    EPwm5Regs.TZFRC.bit.OST                 = 1;
    EPwm6Regs.TZFRC.bit.OST                 = 1;
    EPwm7Regs.TZFRC.bit.OST                 = 1;
    EDIS;

    // Sync Output Select
    EPwm1Regs.EPWMSYNCOUTEN.all             = 0;
    EPwm2Regs.EPWMSYNCOUTEN.all             = 0;
    EPwm3Regs.EPWMSYNCOUTEN.all             = 0;
    EPwm4Regs.EPWMSYNCOUTEN.all             = 0;
    EPwm5Regs.EPWMSYNCOUTEN.all             = 0;
    EPwm6Regs.EPWMSYNCOUTEN.all             = 0;
    EPwm7Regs.EPWMSYNCOUTEN.all             = 0;

    // Sync Input Select
    EPwm1Regs.EPWMSYNCINSEL.bit.SEL         = 0;
    EPwm2Regs.EPWMSYNCINSEL.bit.SEL         = 0;
    EPwm3Regs.EPWMSYNCINSEL.bit.SEL         = 0;
    EPwm4Regs.EPWMSYNCINSEL.bit.SEL         = 0;
    EPwm5Regs.EPWMSYNCINSEL.bit.SEL         = 0;
    EPwm6Regs.EPWMSYNCINSEL.bit.SEL         = 0;
    EPwm7Regs.EPWMSYNCINSEL.bit.SEL         = 0;

    // PHSCounter Reg Load from Phase Reg Enable
    EPwm1Regs.TBCTL.bit.PHSEN               = 0;
    EPwm2Regs.TBCTL.bit.PHSEN               = 0;
    EPwm3Regs.TBCTL.bit.PHSEN               = 0;
    EPwm4Regs.TBCTL.bit.PHSEN               = 0;
    EPwm5Regs.TBCTL.bit.PHSEN               = 0;
    EPwm6Regs.TBCTL.bit.PHSEN               = 0;
    EPwm7Regs.TBCTL.bit.PHSEN               = 0;

    // Shadow to Active Period Register Load on SYNC event
    // 00: Shadow to Active Load of TBPRD occurs only when TBCTR = 0 (same as legacy).
    // 01: Shadow to Active Load of TBPRD occurs both when TBCTR = 0 and when SYNC occurs.
    // 10: Shadow to Active Load of TBPRD occurs only when a SYNC is received.
    // 11: Reserved
    EPwm1Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm2Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm3Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm4Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm5Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm6Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm7Regs.TBCTL2.bit.PRDLDSYNC          = 0;

    // This register controls which EPWMs are linked to other EPWM modules.
    EPwm1Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm1Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm1Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm2Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm2Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm2Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm3Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm3Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm3Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm4Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm4Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm4Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm5Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm5Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm5Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm6Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm6Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm6Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm7Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm7Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm7Regs.EPWMXLINK.bit.CMPBLINK        = 0;

    sInitEPwm1();
    sInitEPwm2();
    sInitEPwm3();
    sInitEPwm4();
    sInitEPwm5();
    sInitEPwm6();
    sInitEPwm7();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC        = 1;        // Enable all the TB Clocks
    EDIS;

}

inline  static  void    sInitEPwm1(void)
{
    // High Speed Time Base Clock Pre-Scale Bits
    // These bits determine part of the time-base clock prescale value.
    // TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV). This divisor emulates the HSPCLK in the TMS320x281x system as used on the Event Manager (EV) peripheral.
    // 000: /1
    // 001: /2 (default on reset)
    // 010: /4
    // 011: /6
    // 100: /8
    // 101: /10
    // 110: /12
    // 111: /14
    EPwm1Regs.TBCTL.bit.HSPCLKDIV           = 0;

    // Time Base Clock Pre-Scale Bits
    // These bits select the time base clock pre-scale value (TBCLK = EPWMCLK/(HSPCLKDIV * CLKDIV):
    // 000: /1 (default on reset)
    // 001: /2
    // 010: /4
    // 011: /8
    // 100: /16
    // 101: /32
    // 110: /64
    // 111: /128
    EPwm1Regs.TBCTL.bit.CLKDIV              = 0;

    // Clear TB Counter
    EPwm1Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm1Regs.TBPRD                         = Freq2Prd();

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm1Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm1Regs.TBCTL.bit.PHSDIR              = 1;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm1Regs.TBCTL.bit.CTRMODE             = 2;

    //
    EPwm1Regs.TBCTL.bit.PRDLD               = 0;

    //
    EPwm1Regs.CMPCTL.bit.SHDWAMODE          = 0;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE          = 0;

    //
    EPwm1Regs.CMPCTL.bit.LOADAMODE          = 0;
    EPwm1Regs.CMPCTL.bit.LOADBMODE          = 0;

    //
    EPwm1Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm1Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm1Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm1Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm1Regs.AQCTLA.bit.CAU                = 0;
    EPwm1Regs.AQCTLA.bit.CAD                = 0;
    EPwm1Regs.AQCTLA.bit.CBU                = 0;
    EPwm1Regs.AQCTLA.bit.CBD                = 0;
    EPwm1Regs.AQCTLA.bit.ZRO                = 2;
    EPwm1Regs.AQCTLA.bit.PRD                = 1;

    EPwm1Regs.AQCTLB.bit.CAU                = 0;
    EPwm1Regs.AQCTLB.bit.CAD                = 0;
    EPwm1Regs.AQCTLB.bit.CBU                = 0;
    EPwm1Regs.AQCTLB.bit.CBD                = 0;
    EPwm1Regs.AQCTLB.bit.ZRO                = 2;
    EPwm1Regs.AQCTLB.bit.PRD                = 1;

    // T1/T2 Event Source Select Bits
    // 0000: DCAEVT1
    // 0001: DCAEVT2
    // 0010: DCBEVT1
    // 0011: DCBEVT2
    // 0100: TZ1
    // 0101: TZ2
    // 0110: TZ3
    // 0111: EPWMxSYNCI
    // 1000: DCEVTFILT
    // Others: Reserved
    EPwm1Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm1Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm1Regs.AQCTLA2.bit.T1U               = 0;
    EPwm1Regs.AQCTLA2.bit.T1U               = 0;
    EPwm1Regs.AQCTLA2.bit.T1U               = 0;
    EPwm1Regs.AQCTLA2.bit.T1U               = 0;

    EPwm1Regs.AQCTLB2.bit.T1U               = 0;
    EPwm1Regs.AQCTLB2.bit.T1U               = 0;
    EPwm1Regs.AQCTLB2.bit.T1U               = 0;
    EPwm1Regs.AQCTLB2.bit.T1U               = 0;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm1Regs.AQCSFRC.bit.CSFA              = 0;
    EPwm1Regs.AQCSFRC.bit.CSFB              = 0;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm1Regs.DBCTL.bit.IN_MODE             = 0;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm1Regs.DBCTL.bit.SHDWDBREDMODE       = 1;
    EPwm1Regs.DBCTL.bit.SHDWDBFEDMODE       = 1;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm1Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm1Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm1Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm1Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm1Regs.DBCTL.bit.POLSEL              = 2;

    // 
    EPwm1Regs.DBCTL.bit.OUT_MODE            = 3;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm1Regs.DBCTL.bit.OUTSWAP             = 0;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm1Regs.DCAHTRIPSEL.all               = 0;
    EPwm1Regs.DCALTRIPSEL.all               = 0;
    EPwm1Regs.DCBHTRIPSEL.all               = 0;
    EPwm1Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm1Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_COMBINATION;
    EPwm1Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm1Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm1Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm1Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm1Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm1Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm1Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm1Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm1Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm1Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm1Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm1Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm1Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm1Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm1Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm1Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm1Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm1Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm1Regs.TZSEL.bit.OSHT1               = TZ_ENABLE;
    EPwm1Regs.TZSEL.bit.OSHT2               = TZ_ENABLE;
    EPwm1Regs.TZSEL.bit.OSHT3               = TZ_ENABLE;

    EPwm1Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm1Regs.TZSEL.bit.DCAEVT1             = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCAEVT2             = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCBEVT1             = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.DCBEVT2             = TZ_DISABLE;

    //
    EPwm1Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm1Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm1Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm1Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm1Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm1Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm1Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm1Regs.TZDCSEL.bit.DCAEVT1           = TZ_DCAH_HI;
    EPwm1Regs.TZDCSEL.bit.DCAEVT2           = TZ_DCAL_HI;
    EPwm1Regs.TZDCSEL.bit.DCBEVT1           = TZ_DCAH_HI;
    EPwm1Regs.TZDCSEL.bit.DCBEVT2           = TZ_DCAL_HI;

    //
    EPwm1Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm1Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm1Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm1Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm1Regs.ETCLR.all                     = 0xFFFF;
    EPwm1Regs.ETCLR.all                     = 0xFFFF;
    EPwm1Regs.ETCLR.all                     = 0xFFFF;
    EPwm1Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm1Regs.ETSEL.bit.INTEN               = 1;
    EPwm1Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm1Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm1Regs.ETSEL.bit.SOCAEN              = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm1Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm1Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm1Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm1Regs.ETPS.bit.SOCBPRD              = ET_2ST;


}

inline  static  void    sInitEPwm2(void)
{
    
}

inline  static  void    sInitEPwm3(void)
{
    
}

inline  static  void    sInitEPwm4(void)
{
    
}

inline  static  void    sInitEPwm5(void)
{
    
}

inline  static  void    sInitEPwm6(void)
{
    
}

inline  static  void    sInitEPwm7(void)
{
    
}
