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
    EPwm1Regs.EPWMSYNCOUTEN.bit.SWEN        = SYNC_OUT_SRC_ENABLE;
    EPwm1Regs.EPWMSYNCOUTEN.bit.ZEROEN      = SYNC_OUT_SRC_ENABLE;
    EPwm1Regs.EPWMSYNCOUTEN.bit.CMPBEN      = SYNC_OUT_SRC_DISABLE;
    EPwm1Regs.EPWMSYNCOUTEN.bit.CMPCEN      = SYNC_OUT_SRC_DISABLE;
    EPwm1Regs.EPWMSYNCOUTEN.bit.DCAEVT1EN   = SYNC_OUT_SRC_DISABLE;
    EPwm1Regs.EPWMSYNCOUTEN.bit.DCBEVT1EN   = SYNC_OUT_SRC_DISABLE;

    EPwm2Regs.EPWMSYNCOUTEN.all             = SYNC_OUT_SRC_DISABLE;
    EPwm3Regs.EPWMSYNCOUTEN.all             = SYNC_OUT_SRC_DISABLE;
    EPwm4Regs.EPWMSYNCOUTEN.all             = SYNC_OUT_SRC_DISABLE;
    EPwm5Regs.EPWMSYNCOUTEN.all             = SYNC_OUT_SRC_DISABLE;

    EPwm6Regs.EPWMSYNCOUTEN.bit.SWEN        = SYNC_OUT_SRC_ENABLE;
    EPwm6Regs.EPWMSYNCOUTEN.bit.ZEROEN      = SYNC_OUT_SRC_ENABLE;
    EPwm6Regs.EPWMSYNCOUTEN.bit.CMPBEN      = SYNC_OUT_SRC_DISABLE;
    EPwm6Regs.EPWMSYNCOUTEN.bit.CMPCEN      = SYNC_OUT_SRC_DISABLE;
    EPwm6Regs.EPWMSYNCOUTEN.bit.DCAEVT1EN   = SYNC_OUT_SRC_DISABLE;
    EPwm6Regs.EPWMSYNCOUTEN.bit.DCBEVT1EN   = SYNC_OUT_SRC_DISABLE;

    EPwm7Regs.EPWMSYNCOUTEN.all             = SYNC_OUT_SRC_DISABLE;

    // Sync Input Select
    EPwm1Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_DISABLE_ALL;
    EPwm2Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_SYNCOUT_EPWM1;
    EPwm3Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_SYNCOUT_EPWM1;
    EPwm4Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_SYNCOUT_EPWM1;
    EPwm5Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_SYNCOUT_EPWM6;
    EPwm6Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_DISABLE_ALL;
    EPwm7Regs.EPWMSYNCINSEL.bit.SEL         = SYNC_IN_SRC_DISABLE_ALL;

    // PHSCounter Reg Load from Phase Reg Enable
    EPwm1Regs.TBCTL.bit.PHSEN               = TB_DISABLE;
    EPwm2Regs.TBCTL.bit.PHSEN               = TB_ENABLE;
    EPwm3Regs.TBCTL.bit.PHSEN               = TB_ENABLE;
    EPwm4Regs.TBCTL.bit.PHSEN               = TB_ENABLE;
    EPwm5Regs.TBCTL.bit.PHSEN               = TB_ENABLE;
    EPwm6Regs.TBCTL.bit.PHSEN               = TB_DISABLE;
    EPwm7Regs.TBCTL.bit.PHSEN               = TB_DISABLE;

    // Shadow to Active Period Register Load on SYNC event
    // 00: Shadow to Active Load of TBPRD occurs only when TBCTR = 0 (same as legacy).
    // 01: Shadow to Active Load of TBPRD occurs both when TBCTR = 0 and when SYNC occurs.
    // 10: Shadow to Active Load of TBPRD occurs only when a SYNC is received.
    // 11: Reserved
    EPwm1Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm2Regs.TBCTL2.bit.PRDLDSYNC          = 2;
    EPwm3Regs.TBCTL2.bit.PRDLDSYNC          = 2;
    EPwm4Regs.TBCTL2.bit.PRDLDSYNC          = 2;
    EPwm5Regs.TBCTL2.bit.PRDLDSYNC          = 2;
    EPwm6Regs.TBCTL2.bit.PRDLDSYNC          = 0;
    EPwm7Regs.TBCTL2.bit.PRDLDSYNC          = 0;

    // This register controls which EPWMs are linked to other EPWM modules.
    EPwm1Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm1Regs.EPWMXLINK.bit.CMPALINK        = 0;
    EPwm1Regs.EPWMXLINK.bit.CMPBLINK        = 0;
    EPwm2Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm2Regs.EPWMXLINK.bit.CMPALINK        = 1;
    EPwm2Regs.EPWMXLINK.bit.CMPBLINK        = 1;
    EPwm3Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm3Regs.EPWMXLINK.bit.CMPALINK        = 2;
    EPwm3Regs.EPWMXLINK.bit.CMPBLINK        = 2;
    EPwm4Regs.EPWMXLINK.bit.TBPRDLINK       = 0;
    EPwm4Regs.EPWMXLINK.bit.CMPALINK        = 3;
    EPwm4Regs.EPWMXLINK.bit.CMPBLINK        = 3;
    EPwm5Regs.EPWMXLINK.bit.TBPRDLINK       = 5;
    EPwm5Regs.EPWMXLINK.bit.CMPALINK        = 4;
    EPwm5Regs.EPWMXLINK.bit.CMPBLINK        = 4;
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
    EPwm1Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm1Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm1Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm1Regs.TBPRD                         = sFreq2Prd_UpDn(LLC_Freq);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm1Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm1Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm1Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm1Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm1Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm1Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO;

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
    EPwm1Regs.AQCTLA.bit.CAU                = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAD                = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CBU                = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CBD                = AQ_SET;
    EPwm1Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.PRD                = AQ_NO_ACTION;

    EPwm1Regs.AQCTLB.bit.CAU                = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CBU                = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBD                = AQ_SET;
    EPwm1Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.PRD                = AQ_NO_ACTION;

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
    EPwm1Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm1Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm1Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm1Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm1Regs.DBCTL.bit.IN_MODE             = DBA_RED_DBB_FED;

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
    EPwm1Regs.DBCTL.bit.POLSEL              = DB_ACTV_HIC;

    // 
    EPwm1Regs.DBCTL.bit.OUT_MODE            = DB_FULL_ENABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm1Regs.DBCTL.bit.OUTSWAP             = 0;

    EPwm1Regs.DBFED.all                     = DBCOUNT_us(cLLCDBT_RED);
    EPwm1Regs.DBRED.all                     = DBCOUNT_us(cLLCDBT_RED);

    EPwm1Regs.CMPA.bit.CMPA                 = 10;
    EPwm1Regs.CMPB.bit.CMPB                 = 650;


    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm1Regs.DCAHTRIPSEL.all               = 0x98;
    EPwm1Regs.DCALTRIPSEL.all               = 0;
    EPwm1Regs.DCBHTRIPSEL.all               = 0x98;
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
    EPwm1Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm1Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm1Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm1Regs.TZSEL.bit.DCAEVT1             = TZ_ENABLE;
    EPwm1Regs.TZSEL.bit.DCAEVT2             = TZ_ENABLE;
    EPwm1Regs.TZSEL.bit.DCBEVT1             = TZ_ENABLE;
    EPwm1Regs.TZSEL.bit.DCBEVT2             = TZ_ENABLE;

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
    EPwm1Regs.ETSEL.bit.SOCAEN              = 0;
    EPwm1Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm1Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm1Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm1Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm1Regs.ETPS.bit.SOCBPRD              = ET_2ST;


}

inline  static  void    sInitEPwm2(void)
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
    EPwm2Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm2Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm2Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm2Regs.TBPRD                         = sFreq2Prd_UpDn(LLC_Freq);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm2Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm2Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm2Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm2Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm2Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm2Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO;

    //
    EPwm2Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm2Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm2Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm2Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm2Regs.AQCTLA.bit.CAU                = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAD                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBU                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD                = AQ_CLEAR;

    EPwm2Regs.AQCTLB.bit.CAU                = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CBU                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CBD                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.PRD                = AQ_CLEAR;

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
    EPwm2Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm2Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm2Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm2Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm2Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm2Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm2Regs.DBCTL.bit.IN_MODE             = DBA_RED_DBB_FED;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm2Regs.DBCTL.bit.SHDWDBREDMODE       = 1;
    EPwm2Regs.DBCTL.bit.SHDWDBFEDMODE       = 1;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm2Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm2Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm2Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm2Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm2Regs.DBCTL.bit.POLSEL              = DB_ACTV_HIC;

    // 
    EPwm2Regs.DBCTL.bit.OUT_MODE            = DB_FULL_ENABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm2Regs.DBCTL.bit.OUTSWAP             = 0;

    EPwm2Regs.DBFED.all                     = DBCOUNT_us(cLLCDBT_RED);
    EPwm2Regs.DBRED.all                     = DBCOUNT_us(cLLCDBT_FED);

    EPwm2Regs.CMPA.bit.CMPA                 = 650;
    EPwm2Regs.CMPB.bit.CMPB                 = 10;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm2Regs.DCAHTRIPSEL.all               = 0x98;
    EPwm2Regs.DCALTRIPSEL.all               = 0;
    EPwm2Regs.DCBHTRIPSEL.all               = 0x98;
    EPwm2Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_COMBINATION;
    EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm2Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm2Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm2Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm2Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm2Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm2Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm2Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm2Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm2Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm2Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm2Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm2Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm2Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm2Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm2Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm2Regs.TZSEL.bit.OSHT1               = TZ_ENABLE;
    EPwm2Regs.TZSEL.bit.OSHT2               = TZ_ENABLE;
    EPwm2Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm2Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm2Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm2Regs.TZSEL.bit.DCAEVT1             = TZ_ENABLE;
    EPwm2Regs.TZSEL.bit.DCAEVT2             = TZ_ENABLE;
    EPwm2Regs.TZSEL.bit.DCBEVT1             = TZ_ENABLE;
    EPwm2Regs.TZSEL.bit.DCBEVT2             = TZ_ENABLE;

    //
    EPwm2Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm2Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm2Regs.TZDCSEL.bit.DCAEVT1           = TZ_DCAH_HI;
    EPwm2Regs.TZDCSEL.bit.DCAEVT2           = TZ_DCAL_HI;
    EPwm2Regs.TZDCSEL.bit.DCBEVT1           = TZ_DCAH_HI;
    EPwm2Regs.TZDCSEL.bit.DCBEVT2           = TZ_DCAL_HI;

    //
    EPwm2Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm2Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm2Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm2Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm2Regs.ETCLR.all                     = 0xFFFF;
    EPwm2Regs.ETCLR.all                     = 0xFFFF;
    EPwm2Regs.ETCLR.all                     = 0xFFFF;
    EPwm2Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm2Regs.ETSEL.bit.INTEN               = 0;
    EPwm2Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm2Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm2Regs.ETSEL.bit.SOCAEN              = 0;
    EPwm2Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm2Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm2Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm2Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm2Regs.ETPS.bit.SOCBPRD              = ET_2ST;
}

inline  static  void    sInitEPwm3(void)
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
    EPwm3Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm3Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm3Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm3Regs.TBPRD                         = sFreq2Prd_UpDn(LLC_Freq);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm3Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm3Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm3Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm3Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm3Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm3Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO;

    //
    EPwm3Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm3Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm3Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm3Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm3Regs.AQCTLA.bit.CAU                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAD                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBU                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD                = AQ_SET;
    EPwm3Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD                = AQ_CLEAR;

    EPwm3Regs.AQCTLB.bit.CAU                = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CBU                = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CBD                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.PRD                = AQ_NO_ACTION;

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
    EPwm3Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm3Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm3Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm3Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm3Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm3Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm3Regs.DBCTL.bit.IN_MODE             = DBA_RED_DBB_FED;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm3Regs.DBCTL.bit.SHDWDBREDMODE       = 1;
    EPwm3Regs.DBCTL.bit.SHDWDBFEDMODE       = 1;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm3Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm3Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm3Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm3Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm3Regs.DBCTL.bit.POLSEL              = DB_ACTV_HIC;

    // 
    EPwm3Regs.DBCTL.bit.OUT_MODE            = DB_FULL_ENABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm3Regs.DBCTL.bit.OUTSWAP             = 0;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm3Regs.DCAHTRIPSEL.all               = 0x98;
    EPwm3Regs.DCALTRIPSEL.all               = 0;
    EPwm3Regs.DCBHTRIPSEL.all               = 0x98;
    EPwm3Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_COMBINATION;
    EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm3Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm3Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm3Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm3Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm3Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm3Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm3Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm3Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm3Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm3Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm3Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm3Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm3Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm3Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm3Regs.TZSEL.bit.OSHT1               = TZ_ENABLE;
    EPwm3Regs.TZSEL.bit.OSHT2               = TZ_ENABLE;
    EPwm3Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm3Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm3Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm3Regs.TZSEL.bit.DCAEVT1             = TZ_ENABLE;
    EPwm3Regs.TZSEL.bit.DCAEVT2             = TZ_ENABLE;
    EPwm3Regs.TZSEL.bit.DCBEVT1             = TZ_ENABLE;
    EPwm3Regs.TZSEL.bit.DCBEVT2             = TZ_ENABLE;

    //
    EPwm3Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm3Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm3Regs.TZDCSEL.bit.DCAEVT1           = TZ_DCAH_HI;
    EPwm3Regs.TZDCSEL.bit.DCAEVT2           = TZ_DCAL_HI;
    EPwm3Regs.TZDCSEL.bit.DCBEVT1           = TZ_DCAH_HI;
    EPwm3Regs.TZDCSEL.bit.DCBEVT2           = TZ_DCAL_HI;

    //
    EPwm3Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm3Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm3Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm3Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm3Regs.ETCLR.all                     = 0xFFFF;
    EPwm3Regs.ETCLR.all                     = 0xFFFF;
    EPwm3Regs.ETCLR.all                     = 0xFFFF;
    EPwm3Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm3Regs.ETSEL.bit.INTEN               = 0;
    EPwm3Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm3Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm3Regs.ETSEL.bit.SOCAEN              = 0;
    EPwm3Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm3Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm3Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm3Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm3Regs.ETPS.bit.SOCBPRD              = ET_2ST;
}

inline  static  void    sInitEPwm4(void)
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
    EPwm4Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm4Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm4Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm4Regs.TBPRD                         = sFreq2Prd_UpDn(LLC_Freq);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm4Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm4Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm4Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm4Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm4Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm4Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO;

    //
    EPwm4Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm4Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm4Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm4Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm4Regs.AQCTLA.bit.CAU                = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBU                = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CBD                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD                = AQ_NO_ACTION;

    EPwm4Regs.AQCTLB.bit.CAU                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CBU                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CBD                = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.PRD                = AQ_SET;

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
    EPwm4Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm4Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm4Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm4Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm4Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm4Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm4Regs.DBCTL.bit.IN_MODE             = DBA_RED_DBB_FED;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm4Regs.DBCTL.bit.SHDWDBREDMODE       = 1;
    EPwm4Regs.DBCTL.bit.SHDWDBFEDMODE       = 1;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm4Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm4Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm4Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm4Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm4Regs.DBCTL.bit.POLSEL              = DB_ACTV_HIC;

    // 
    EPwm4Regs.DBCTL.bit.OUT_MODE            = DB_FULL_ENABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm4Regs.DBCTL.bit.OUTSWAP             = 0;

    EPwm4Regs.DBFED.all                     = DBCOUNT_us(cLLCDBT_FED);
    EPwm4Regs.DBRED.all                     = DBCOUNT_us(cLLCDBT_RED);

    EPwm4Regs.CMPA.bit.CMPA                 = 10;
    EPwm4Regs.CMPB.bit.CMPB                 = 650;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm4Regs.DCAHTRIPSEL.all               = 0x98;
    EPwm4Regs.DCALTRIPSEL.all               = 0;
    EPwm4Regs.DCBHTRIPSEL.all               = 0x08;
    EPwm4Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm4Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm4Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_COMBINATION;
    EPwm4Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm4Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm4Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm4Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm4Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm4Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm4Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm4Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm4Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm4Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm4Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm4Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm4Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm4Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm4Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm4Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm4Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm4Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm4Regs.TZSEL.bit.OSHT1               = TZ_ENABLE;
    EPwm4Regs.TZSEL.bit.OSHT2               = TZ_ENABLE;
    EPwm4Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm4Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm4Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm4Regs.TZSEL.bit.DCAEVT1             = TZ_ENABLE;
    EPwm4Regs.TZSEL.bit.DCAEVT2             = TZ_ENABLE;
    EPwm4Regs.TZSEL.bit.DCBEVT1             = TZ_ENABLE;
    EPwm4Regs.TZSEL.bit.DCBEVT2             = TZ_ENABLE;

    //
    EPwm4Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm4Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm4Regs.TZDCSEL.bit.DCAEVT1           = TZ_DCAH_HI;
    EPwm4Regs.TZDCSEL.bit.DCAEVT2           = TZ_DCAL_HI;
    EPwm4Regs.TZDCSEL.bit.DCBEVT1           = TZ_DCAH_HI;
    EPwm4Regs.TZDCSEL.bit.DCBEVT2           = TZ_DCAL_HI;

    //
    EPwm4Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm4Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm4Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm4Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm4Regs.ETCLR.all                     = 0xFFFF;
    EPwm4Regs.ETCLR.all                     = 0xFFFF;
    EPwm4Regs.ETCLR.all                     = 0xFFFF;
    EPwm4Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm4Regs.ETSEL.bit.INTEN               = 0;
    EPwm4Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm4Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm4Regs.ETSEL.bit.SOCAEN              = 0;
    EPwm4Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm4Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm4Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm4Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm4Regs.ETPS.bit.SOCBPRD              = ET_2ST;
}

inline  static  void    sInitEPwm5(void)
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
    EPwm5Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm5Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm5Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm5Regs.TBPRD                         = sFreq2Prd_UpDn(INV_Freq);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm5Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm5Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm5Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm5Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm5Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm5Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO_PRD;
    EPwm5Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO_PRD;

    //
    EPwm5Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm5Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm5Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm5Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm5Regs.AQCTLA.bit.CAU                = AQ_CLEAR;
    EPwm5Regs.AQCTLA.bit.CAD                = AQ_SET;
    EPwm5Regs.AQCTLA.bit.CBU                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.CBD                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.PRD                = AQ_NO_ACTION;

    EPwm5Regs.AQCTLB.bit.CAU                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.CBU                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.CBD                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.PRD                = AQ_NO_ACTION;

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
    EPwm5Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm5Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm5Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm5Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm5Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm5Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm5Regs.DBCTL.bit.IN_MODE             = DBA_ALL;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm5Regs.DBCTL.bit.SHDWDBREDMODE       = 0;
    EPwm5Regs.DBCTL.bit.SHDWDBFEDMODE       = 0;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm5Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm5Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm5Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm5Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm5Regs.DBCTL.bit.POLSEL              = DB_ACTV_HIC;

    // 
    EPwm5Regs.DBCTL.bit.OUT_MODE            = DB_FULL_ENABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm5Regs.DBCTL.bit.OUTSWAP             = 0;

    EPwm5Regs.DBFED.all                     = DBCOUNT_us(cInvInitDBT);
    EPwm5Regs.DBRED.all                     = DBCOUNT_us(cInvInitDBT);

    EPwm5Regs.CMPA.bit.CMPA                 = EPwm5Regs.TBPRD >> 1;
    EPwm5Regs.CMPB.bit.CMPB                 = EPwm5Regs.TBPRD >> 1;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm5Regs.DCAHTRIPSEL.all               = 0x18;
    EPwm5Regs.DCALTRIPSEL.all               = 0;
    EPwm5Regs.DCBHTRIPSEL.all               = 0;
    EPwm5Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm5Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm5Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_TRIPIN4;
    EPwm5Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm5Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm5Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm5Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm5Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm5Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm5Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm5Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm5Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm5Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm5Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm5Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm5Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm5Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm5Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm5Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm5Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm5Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm5Regs.TZSEL.bit.OSHT1               = TZ_ENABLE;
    EPwm5Regs.TZSEL.bit.OSHT2               = TZ_ENABLE;
    EPwm5Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm5Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm5Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm5Regs.TZSEL.bit.CBC3                = TZ_ENABLE;

    EPwm5Regs.TZSEL.bit.DCAEVT1             = TZ_ENABLE;
    EPwm5Regs.TZSEL.bit.DCAEVT2             = TZ_ENABLE;
    EPwm5Regs.TZSEL.bit.DCBEVT1             = TZ_DISABLE;
    EPwm5Regs.TZSEL.bit.DCBEVT2             = TZ_DISABLE;

    //
    EPwm5Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm5Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm5Regs.TZDCSEL.bit.DCAEVT1           = TZ_DCAH_HI;
    EPwm5Regs.TZDCSEL.bit.DCAEVT2           = TZ_DCAL_HI;
    EPwm5Regs.TZDCSEL.bit.DCBEVT1           = TZ_EVT_DISABLE;
    EPwm5Regs.TZDCSEL.bit.DCBEVT2           = TZ_EVT_DISABLE;

    //
    EPwm5Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm5Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm5Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm5Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm5Regs.ETCLR.all                     = 0xFFFF;
    EPwm5Regs.ETCLR.all                     = 0xFFFF;
    EPwm5Regs.ETCLR.all                     = 0xFFFF;
    EPwm5Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm5Regs.ETSEL.bit.INTEN               = 1;
    EPwm5Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm5Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm5Regs.ETSEL.bit.SOCAEN              = 1;
    EPwm5Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm5Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm5Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm5Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm5Regs.ETPS.bit.SOCBPRD              = ET_2ST;
}

inline  static  void    sInitEPwm6(void)
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
    EPwm6Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm6Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm6Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm6Regs.TBPRD                         = sFreq2Prd_UpDn(INV_Freq);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm6Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm6Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm6Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm6Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm6Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm6Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO_PRD;
    EPwm6Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO_PRD;

    //
    EPwm6Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm6Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm6Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm6Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm6Regs.AQCTLA.bit.CAU                = AQ_CLEAR;
    EPwm6Regs.AQCTLA.bit.CAD                = AQ_SET;
    EPwm6Regs.AQCTLA.bit.CBU                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA.bit.CBD                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA.bit.PRD                = AQ_NO_ACTION;

    EPwm6Regs.AQCTLB.bit.CAU                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.CBU                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.CBD                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.PRD                = AQ_NO_ACTION;

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
    EPwm6Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm6Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm6Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm6Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm6Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm6Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm6Regs.DBCTL.bit.IN_MODE             = DBA_ALL;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm6Regs.DBCTL.bit.SHDWDBREDMODE       = 0;
    EPwm6Regs.DBCTL.bit.SHDWDBFEDMODE       = 0;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm6Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm6Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm6Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm6Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm6Regs.DBCTL.bit.POLSEL              = DB_ACTV_HIC;

    // 
    EPwm6Regs.DBCTL.bit.OUT_MODE            = DB_FULL_ENABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm6Regs.DBCTL.bit.OUTSWAP             = 0;

    EPwm6Regs.DBFED.all                     = DBCOUNT_us(cInvInitDBT);
    EPwm6Regs.DBRED.all                     = DBCOUNT_us(cInvInitDBT);

    EPwm6Regs.CMPA.bit.CMPA                 = EPwm6Regs.TBPRD >> 1;
    EPwm6Regs.CMPB.bit.CMPB                 = cSwPrdDiff;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm6Regs.DCAHTRIPSEL.all               = 0x18;
    EPwm6Regs.DCALTRIPSEL.all               = 0;
    EPwm6Regs.DCBHTRIPSEL.all               = 0;
    EPwm6Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm6Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm6Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_TRIPIN4;
    EPwm6Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm6Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm6Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm6Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm6Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm6Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm6Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm6Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm6Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm6Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm6Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm6Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm6Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm6Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm6Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm6Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm6Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm6Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm6Regs.TZSEL.bit.OSHT1               = TZ_ENABLE;
    EPwm6Regs.TZSEL.bit.OSHT2               = TZ_ENABLE;
    EPwm6Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm6Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm6Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm6Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm6Regs.TZSEL.bit.DCAEVT1             = TZ_ENABLE;
    EPwm6Regs.TZSEL.bit.DCAEVT2             = TZ_ENABLE;
    EPwm6Regs.TZSEL.bit.DCBEVT1             = TZ_DISABLE;
    EPwm6Regs.TZSEL.bit.DCBEVT2             = TZ_DISABLE;

    //
    EPwm6Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm6Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm6Regs.TZDCSEL.bit.DCAEVT1           = TZ_DCAH_HI;
    EPwm6Regs.TZDCSEL.bit.DCAEVT2           = TZ_DCAL_HI;
    EPwm6Regs.TZDCSEL.bit.DCBEVT1           = TZ_EVT_DISABLE;
    EPwm6Regs.TZDCSEL.bit.DCBEVT2           = TZ_EVT_DISABLE;

    //
    EPwm6Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm6Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm6Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm6Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm6Regs.ETCLR.all                     = 0xFFFF;
    EPwm6Regs.ETCLR.all                     = 0xFFFF;
    EPwm6Regs.ETCLR.all                     = 0xFFFF;
    EPwm6Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm6Regs.ETSEL.bit.INTEN               = 1;
    EPwm6Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm6Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm6Regs.ETSEL.bit.SOCAEN              = 0;
    EPwm6Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm6Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm6Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm6Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm6Regs.ETPS.bit.SOCBPRD              = ET_2ST;
}

inline  static  void    sInitEPwm7(void)
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
    EPwm7Regs.TBCTL.bit.HSPCLKDIV           = TB_DIV1;

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
    EPwm7Regs.TBCTL.bit.CLKDIV              = TB_DIV1;

    // Clear TB Counter
    EPwm7Regs.TBCTR                         = 0;

    // Config PWM Freq
    EPwm7Regs.TBPRD                         = sFreq2Prd_UpDn(10.0f);

    // These bits set time-base counter phase of the selected ePWM relative to the time-base that is supplying the synchronization input signal.
    // If TBCTL[PHSEN] = 0, then the synchronization event is ignored and the time-base counter is not loaded with the phase.
    // If TBCTL[PHSEN] = 1, then the time-base counter (TBCTR) will be loaded with the phase (TBPHS) when a synchronization event occurs. 
    // The synchronization event can be initiated by the input synchronization signal (EPWMxSYNCI) or by a software forced synchronization.
    EPwm7Regs.TBPHS.bit.TBPHS               = 0;

    // This bit is only used when the time-base counter is configured in the up-down-count mode. 
    // The PHSDIR bit indicates the direction the time-base counter (TBCTR) will count after a synchronization
    // event occurs and a new phase value is loaded from the phase (TBPHS) register. 
    // This is irrespective of the direction of the counter before the synchronization event.. 
    EPwm7Regs.TBCTL.bit.PHSDIR              = TB_UP;

    // The time-base counter mode is normally configured once and not changed during normal operation. If you change the mode of the counter, 
    // the change will take effect at the next TBCLK edge and the current counter value shall increment or decrement from the value before the mode change. 
    // These bits set the time-base counter mode of operation as follows:
    // 00: Up-count mode
    // 01: Down-count mode
    // 10: Up-down count mode
    // 11: Freeze counter operation (default on reset)
    EPwm7Regs.TBCTL.bit.CTRMODE             = TB_COUNT_UPDOWN;

    //
    EPwm7Regs.TBCTL.bit.PRDLD               = TB_SHADOW;

    //
    EPwm7Regs.CMPCTL.bit.SHDWAMODE          = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.SHDWBMODE          = CC_SHADOW;

    //
    EPwm7Regs.CMPCTL.bit.LOADAMODE          = CC_CTR_ZERO_PRD;
    EPwm7Regs.CMPCTL.bit.LOADBMODE          = CC_CTR_ZERO_PRD;

    //
    EPwm7Regs.AQCTL.bit.SHDWAQAMODE         = 0;
    EPwm7Regs.AQCTL.bit.SHDWAQBMODE         = 0;

    //
    EPwm7Regs.AQCTL.bit.LDAQAMODE           = 0;
    EPwm7Regs.AQCTL.bit.LDAQBMODE           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, and a high signal will be forced low.
    EPwm7Regs.AQCTLA.bit.CAU                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.CAD                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.CBU                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.CBD                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.ZRO                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.PRD                = AQ_NO_ACTION;

    EPwm7Regs.AQCTLB.bit.CAU                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.CAD                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.CBU                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.CBD                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.ZRO                = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.PRD                = AQ_NO_ACTION;

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
    EPwm7Regs.AQTSRCSEL.bit.T1SEL           = 0;
    EPwm7Regs.AQTSRCSEL.bit.T2SEL           = 0;

    // Note: By definition, in count up-down mode when the counter equals 0 the direction is defined as 1 or counting up.
    // 00: Do nothing (action disabled)
    // 01: Clear: force EPWMxA output low.
    // 10: Set: force EPWMxA output high.
    // 11: Toggle EPWMxA output: low output signal will be forced high, 
    // and a high signal will be forced low.
    EPwm7Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA2.bit.T1U               = AQ_NO_ACTION;

    EPwm7Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB2.bit.T1U               = AQ_NO_ACTION;

    // Continuous Software Force on Output A/B
    // In immediate mode, a continuous force takes effect on the next TBCLK edge. 
    // In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into the active register.
    // 00: Software forcing is disabled and has no effect
    // 01: Forces a continuous low on Output
    // 10: Forces a continuous high on Output
    // 11: Software forcing is disabled and has no effect
    EPwm7Regs.AQCSFRC.bit.CSFA              = AQ_NO_ACTION;
    EPwm7Regs.AQCSFRC.bit.CSFB              = AQ_NO_ACTION;

    // This allows you to select the input source to the falling-edge and rising-edge delay. 
    // To produce classical dead-band waveforms the default is EPWMxA In is the source for both falling and rising-edge delays.
    // 00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    // 01: EPWMxB In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxA In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 10: EPWMxA In (from the action-qualifier) is the source for risingedge delayed signal.EPWMxB In (from the action-qualifier) is the source for falling-edge delayed signal.
    // 11: EPWMxB In (from the action-qualifier) is the source for both rising-edge delay and falling-edge delayed signal.
    EPwm7Regs.DBCTL.bit.IN_MODE             = DBA_ALL;

    // RED Dead-Band Load Mode
    // 0: Immediate mode. Only the active DBRED register is used. All writes/reads via the CPU directly access the active register for immediate 'RED dead-band action.'
    // 1: Shadow mode. Operates as a double buffer. All writes via the CPU access Shadow register. Default at Reset is Immediate mode (for compatibility with legacy).
    EPwm7Regs.DBCTL.bit.SHDWDBREDMODE       = 0;
    EPwm7Regs.DBCTL.bit.SHDWDBFEDMODE       = 0;

    // Active DBFED/DBRED Load from Shadow Select Mode
    // 00: Load on Counter = 0 (CNT_eq)
    // 01: Load on Counter = Period (PRD_eq)
    // 10: Load on either Counter = 0, or Counter = Period
    // 11: Freeze (no loads possible)
    // Note: has no effect in Immediate mode.
    EPwm7Regs.DBCTL.bit.LOADREDMODE         = 0;
    EPwm7Regs.DBCTL.bit.LOADFEDMODE         = 0;

    // Half Cycle Clocking Enable Bit
    // 0: Full cycle clocking enabled. The dead-band counters are clocked at the TBCLK rate.
    // 1: Half cycle clocking enabled. The dead-band counters are clocked at TBCLK*2.
    EPwm7Regs.DBCTL.bit.HALFCYCLE           = 0;


    // Dead Band Dual-Edge B Mode Control (S8 switch)
    // 0: Rising edge delay applied to InA/InB as selected by S4 switch (IN-MODE bits) on A signal path only. 
    // Falling edge delay applied to InA/InB as selected by S5 switch (INMODE bits) on B signal path  only.
    // 1: Rising edge delay and falling edge delay applied to source selected by S4 switch (INMODE bits) and output to B signal path only. 
    // Note: When this bit is set to 1, user should always either set 
    // OUT_MODE bits such that Apath = InA OR OUTSWAP bits such that 
    // OutA=Bpath
    // otherwise, OutA will be invalid.
    EPwm7Regs.DBCTL.bit.DEDB_MODE           = 0;

    // Polarity Select Control
    // Bit 3 controls the S3 switch and bit 2 controls the S2 switch. 
    // This allows you to selectively invert one of the delayed signals before it is sent out of the dead-band submodule. The following descriptions 
    // correspond to classical upper/lower switch control as found in one leg of a digital motor control inverter. These assume that DBCTL[OUT_MODE] = 1,1 and DBCTL[IN_MODE] = 0x0. 
    // Other enhanced modes are also possible, but not regarded as typical usage modes.
    // 00: Active high (AH) mode. Neither EPWMxA nor EPWMxB is inverted (default).
    // 01: Active low complementary (ALC) mode. EPWMxA is inverted.
    // 10: Active high complementary (AHC). EPWMxB is inverted.
    // 11: Active low (AL) mode. Both EPWMxA and EPWMxB are inverted.
    EPwm7Regs.DBCTL.bit.POLSEL              = DB_ACTV_HI;

    // 
    EPwm7Regs.DBCTL.bit.OUT_MODE            = DB_DISABLE;

    // Dead Band Output Swap Control
    // Bit 13 controls the S6 switch and bit 12 controls the S7 switch.
    // 00: OutA and OutB signals are as defined by OUT-MODE bits.
    // 01: OutA = A-path as defined by OUT-MODE bits.OutB = A-path as defined by OUT-MODE bits (rising edge delay or delay-bypassed A signal path).
    // 10: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = B-path as defined by OUT-MODE bits.
    // 11: OutA = B-path as defined by OUT-MODE bits (falling edge delay or delay-bypassed B signal path).OutB = A-path as defined by OUT-MODE bits
    // (rising edge delay or delay-bypassed A signal path)
    EPwm7Regs.DBCTL.bit.OUTSWAP             = 0;

    EALLOW;
    // Digital Compare
    // Digital Compare AH/AL Combination Trip Select
    EPwm7Regs.DCAHTRIPSEL.all               = 0;
    EPwm7Regs.DCALTRIPSEL.all               = 0;
    EPwm7Regs.DCBHTRIPSEL.all               = 0;
    EPwm7Regs.DCALTRIPSEL.all               = 0;

    // Digital Compare Trip Select
    EPwm7Regs.DCTRIPSEL.bit.DCAHCOMPSEL     = DC_COMBINATION;
    EPwm7Regs.DCTRIPSEL.bit.DCALCOMPSEL     = DC_COMBINATION;
    EPwm7Regs.DCTRIPSEL.bit.DCBHCOMPSEL     = DC_COMBINATION;
    EPwm7Regs.DCTRIPSEL.bit.DCBLCOMPSEL     = DC_COMBINATION;

    // Digital Compare A/B control
    EPwm7Regs.DCACTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm7Regs.DCACTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm7Regs.DCACTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm7Regs.DCACTL.bit.EVT1SYNCE          = 0;
    EPwm7Regs.DCACTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm7Regs.DCACTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    EPwm7Regs.DCBCTL.bit.EVT1SRCSEL         = DC_EVT1;
    EPwm7Regs.DCBCTL.bit.EVT1FRCSYNCSEL     = DC_EVT_ASYNC;
    EPwm7Regs.DCBCTL.bit.EVT1SOCE           = DC_SOC_ENABLE;
    EPwm7Regs.DCBCTL.bit.EVT1SYNCE          = 0;
    EPwm7Regs.DCBCTL.bit.EVT2SRCSEL         = DC_EVT2;
    EPwm7Regs.DCBCTL.bit.EVT2FRCSYNCSEL     = DC_EVT_ASYNC;

    // Digital Compare Filiter Control
    EPwm7Regs.DCFCTL.all                    = 0;

    // Digital Compare Capture Control
    EPwm7Regs.DCCAPCTL.all                  = 0;

    // Digital Compare Filter Offset
    EPwm7Regs.DCFOFFSET                     = 0;

    // Digital Compare Filter Window
    EPwm7Regs.DCFWINDOW                     = 0;
    EDIS;

    // Trip Zone of Epwm
    EALLOW;
    EPwm7Regs.TZSEL.bit.OSHT1               = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.OSHT2               = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.OSHT3               = TZ_DISABLE;

    EPwm7Regs.TZSEL.bit.CBC1                = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.CBC2                = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.CBC3                = TZ_DISABLE;

    EPwm7Regs.TZSEL.bit.DCAEVT1             = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.DCAEVT2             = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.DCBEVT1             = TZ_DISABLE;
    EPwm7Regs.TZSEL.bit.DCBEVT2             = TZ_DISABLE;

    //
    EPwm7Regs.TZCTL.bit.TZA                 = TZ_FORCE_LO;
    EPwm7Regs.TZCTL.bit.TZB                 = TZ_FORCE_LO;
    EPwm7Regs.TZCTL.bit.DCAEVT1             = TZ_FORCE_LO;
    EPwm7Regs.TZCTL.bit.DCAEVT2             = TZ_FORCE_LO;
    EPwm7Regs.TZCTL.bit.DCBEVT1             = TZ_FORCE_LO;
    EPwm7Regs.TZCTL.bit.DCBEVT2             = TZ_FORCE_LO;

    //
    EPwm7Regs.TZEINT.bit.OST                = TZ_DISABLE;

    //
    EPwm7Regs.TZDCSEL.bit.DCAEVT1           = TZ_EVT_DISABLE;
    EPwm7Regs.TZDCSEL.bit.DCAEVT2           = TZ_EVT_DISABLE;
    EPwm7Regs.TZDCSEL.bit.DCBEVT1           = TZ_EVT_DISABLE;
    EPwm7Regs.TZDCSEL.bit.DCBEVT2           = TZ_EVT_DISABLE;

    //
    EPwm7Regs.TZCLR.bit.CBCPULSE            = 0;
    EDIS;

    //  
    EPwm7Regs.ETCNTINITCTL.bit.INTINITFRC   = 1;
    EPwm7Regs.ETCNTINITCTL.bit.SOCAINITFRC  = 1;
    EPwm7Regs.ETCNTINITCTL.bit.SOCBINITFRC  = 1;

    //
    EPwm7Regs.ETCLR.all                     = 0xFFFF;
    EPwm7Regs.ETCLR.all                     = 0xFFFF;
    EPwm7Regs.ETCLR.all                     = 0xFFFF;
    EPwm7Regs.ETCLR.all                     = 0xFFFF;

    //
    EPwm7Regs.ETSEL.bit.INTEN               = 0;
    EPwm7Regs.ETSEL.bit.INTSEL              = ET_CTR_PRD;
    EPwm7Regs.ETPS.bit.INTPRD               = ET_1ST;

    //
    EPwm7Regs.ETSEL.bit.SOCAEN              = 0;
    EPwm7Regs.ETSEL.bit.SOCASEL             = ET_CTRU_CMPB;
    EPwm7Regs.ETPS.bit.SOCAPRD              = ET_1ST;

    //
    // EPwm7Regs.ETSEL.bit.SOCBEN              = 0;
    // EPwm7Regs.ETSEL.bit.SOCBSEL             = ET_CTRU_CMPB;
    // EPwm7Regs.ETPS.bit.SOCBPRD              = ET_2ST;
}
