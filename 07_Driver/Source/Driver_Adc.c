#include "Driver_Adc.h"

void    sDrv_IninAdc(void)
{
    unsigned int    ACQP_A;
    // unsigned int    ACQP_B;
    unsigned int    ACQP_C;

    // Set Vref as internal
    SetVREF(ADC_ADCA, ADC_INTERNAL, ADC_VREF3P3);
    // SetVREF(ADC_ADCB, ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADC_ADCC, ADC_INTERNAL, ADC_VREF3P3);

    EALLOW;

    // Write config
    AdcaRegs.ADCCTL2.bit.PRESCALE           = 2;  // Set ADCCLK divier to SYSCLK/2
    // AdcbRegs.ADCCTL2.bit.PRESCALE           = 2;  // Set ADCCLK divier to SYSCLK/2
    AdccRegs.ADCCTL2.bit.PRESCALE           = 2;  // Set ADCCLK divier to SYSCLK/2

    // Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS        = 1;  // Interrupt pulse generation occus at the end of the conversion
    // AdcbRegs.ADCCTL1.bit.INTPULSEPOS        = 1;  // Interrupt pulse generation occus at the end of the conversion
    AdccRegs.ADCCTL1.bit.INTPULSEPOS        = 1;  // Interrupt pulse generation occus at the end of the conversion

    // Power up the ADCs
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ           = 1;
    // AdcbRegs.ADCCTL1.bit.ADCPWDNZ           = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ           = 1;

    // Delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    // Determine minimum acquisition window (in sysclks) based on resolution
    ACQP_A  =   7;
    // ACQP_B  =   7;
    ACQP_C  =   7;

    // =================================== ADC_A =========================== //
    // SOCx Channel Select
    AdcaRegs.ADCSOC0CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC1CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC2CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC3CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC4CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC5CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC6CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC7CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC8CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC9CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC10CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC11CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC12CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC13CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC14CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdcaRegs.ADCSOC15CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA

    // Set ADCINT1/2 Trigger Socx
    // 00: No ADCINT trigger SOCx
    // 01: ADCINT1 Trigger SOCx
    // 10: ADCINT2 Trigger SOCx
    AdcaRegs.ADCINTSOCSEL1.bit.SOC0         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC1         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC2         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC3         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC4         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC5         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC6         = 0;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC7         = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC8         = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC9         = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC10        = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC11        = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC12        = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC13        = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC14        = 0;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC15        = 0;

    //  Set SOCx Trigger Source
    //  00h ADCTRIG0 - Software only
    //  01h ADCTRIG1 - CPU1 Timer 0, TINT0n
    //  02h ADCTRIG2 - CPU1 Timer 1, TINT1n
    //  03h ADCTRIG3 - CPU1 Timer 2, TINT2n
    //  04h ADCTRIG4 - GPIO, Input X-Bar INPUT5
    //  05h ADCTRIG5 - ePWM1, ADCSOCA
    //  06h ADCTRIG6 - ePWM1, ADCSOCB
    //  07h ADCTRIG7 - ePWM2, ADCSOCA
    //  08h ADCTRIG8 - ePWM2, ADCSOCB
    //  09h ADCTRIG9 - ePWM3, ADCSOCA
    //  0Ah ADCTRIG10 - ePWM3, ADCSOCB
    //  0Bh ADCTRIG11 - ePWM4, ADCSOCA
    //  0Ch ADCTRIG12 - ePWM4, ADCSOCB
    //  0Dh ADCTRIG13 - ePWM5, ADCSOCA
    //  0Eh ADCTRIG14 - ePWM5, ADCSOCB
    //  0Fh ADCTRIG15 - ePWM6, ADCSOCA
    //  10h ADCTRIG16 - ePWM6, ADCSOCB
    //  11h ADCTRIG17 - ePWM7, ADCSOCA
    //  12h ADCTRIG18 - ePWM7, ADCSOCB
    //  13h - 3Fh - Reserved
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC7CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC8CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC9CTL.bit.TRIGSEL         = 0;
    AdcaRegs.ADCSOC10CTL.bit.TRIGSEL        = 0;
    AdcaRegs.ADCSOC11CTL.bit.TRIGSEL        = 0;
    AdcaRegs.ADCSOC12CTL.bit.TRIGSEL        = 0;
    AdcaRegs.ADCSOC13CTL.bit.TRIGSEL        = 0;
    AdcaRegs.ADCSOC14CTL.bit.TRIGSEL        = 0;
    AdcaRegs.ADCSOC15CTL.bit.TRIGSEL        = 0;

    // Set SH Window = (ACQPS + 1) * SYSCLK
    AdcaRegs.ADCSOC0CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC1CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC2CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC3CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC4CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC5CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC6CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC7CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC8CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC9CTL.bit.ACQPS           = ACQP_A;
    AdcaRegs.ADCSOC10CTL.bit.ACQPS          = ACQP_A;
    AdcaRegs.ADCSOC11CTL.bit.ACQPS          = ACQP_A;
    AdcaRegs.ADCSOC12CTL.bit.ACQPS          = ACQP_A;
    AdcaRegs.ADCSOC13CTL.bit.ACQPS          = ACQP_A;
    AdcaRegs.ADCSOC14CTL.bit.ACQPS          = ACQP_A;
    AdcaRegs.ADCSOC15CTL.bit.ACQPS          = ACQP_A;

    // Set SOC Priority
    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY   = 16;   // All Soc are defined as high priority

    // Burst Mode Set
    AdcaRegs.ADCBURSTCTL.bit.BURSTEN        = 0;
    AdcaRegs.ADCBURSTCTL.bit.BURSTSIZE      = 0;
    AdcaRegs.ADCBURSTCTL.bit.BURSTTRIGSEL   = 0;

    // ADC PPBx Set
    AdcaRegs.ADCPPB1CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdcaRegs.ADCPPB1CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdcaRegs.ADCPPB1OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdcaRegs.ADCPPB1OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdcaRegs.ADCPPB1TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdcaRegs.ADCPPB1TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value
    
    AdcaRegs.ADCPPB2CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdcaRegs.ADCPPB2CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdcaRegs.ADCPPB2OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdcaRegs.ADCPPB2OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdcaRegs.ADCPPB2TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdcaRegs.ADCPPB2TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value

    AdcaRegs.ADCPPB3CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdcaRegs.ADCPPB3CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdcaRegs.ADCPPB3OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdcaRegs.ADCPPB3OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdcaRegs.ADCPPB3TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdcaRegs.ADCPPB3TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value

    AdcaRegs.ADCPPB4CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdcaRegs.ADCPPB4CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdcaRegs.ADCPPB4OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdcaRegs.ADCPPB4OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdcaRegs.ADCPPB4TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdcaRegs.ADCPPB4TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value

    AdcaRegs.ADCEVTSEL.all                  = 0;    // Post Processing Block x Limit HI/LO Zero Crossing Event Disable
    AdcaRegs.ADCEVTINTSEL.all               = 0;    // Post processing Block x Limit HI/LO Zero Crossing Interrupt Disable

    // Clear PPBx Zero Crossing Limit HI/LO Flag
    AdcaRegs.ADCEVTCLR.all                  = 0xFFFF;

    // Clear SOC Over Flag
    AdcaRegs.ADCSOCOVFCLR1.all              = 0xFFFF;

    // INT Set
    AdcaRegs.ADCINTSEL1N2.bit.INT1E         = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL       = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT      = 0;

    AdcaRegs.ADCINTSEL1N2.bit.INT2E         = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL       = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT2CONT      = 0;

    AdcaRegs.ADCINTSEL3N4.bit.INT3E         = 0;
    AdcaRegs.ADCINTSEL3N4.bit.INT3SEL       = 0;
    AdcaRegs.ADCINTSEL3N4.bit.INT3CONT      = 0;

    AdcaRegs.ADCINTSEL3N4.bit.INT4E         = 0;
    AdcaRegs.ADCINTSEL3N4.bit.INT4SEL       = 0;
    AdcaRegs.ADCINTSEL3N4.bit.INT4CONT      = 0;

    // Clear ADC Interrupt 1/2/3/4 Flag
    AdcaRegs.ADCINTFLGCLR.all               = 0x000F;

    // Clear ADC Interrupt 1/2/3/4 Overflow Flag
    AdcaRegs.ADCINTOVFCLR.all               = 0x000F;

    // =================================== ADC_C =========================== //
    // SOCx Channel Select
    AdccRegs.ADCSOC0CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC1CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC2CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC3CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC4CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC5CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC6CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC7CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC8CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC9CTL.bit.CHSEL           = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC10CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC11CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC12CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC13CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC14CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA
    AdccRegs.ADCSOC15CTL.bit.CHSEL          = 0;    // Set Soc0 channel select to ADCINA

    // Set ADCINT1/2 Trigger Socx
    // 00: No ADCINT trigger SOCx
    // 01: ADCINT1 Trigger SOCx
    // 10: ADCINT2 Trigger SOCx
    AdccRegs.ADCINTSOCSEL1.bit.SOC0         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC1         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC2         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC3         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC4         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC5         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC6         = 0;
    AdccRegs.ADCINTSOCSEL1.bit.SOC7         = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC8         = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC9         = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC10        = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC11        = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC12        = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC13        = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC14        = 0;
    AdccRegs.ADCINTSOCSEL2.bit.SOC15        = 0;

    //  Set SOCx Trigger Source
    //  00h ADCTRIG0 - Software only
    //  01h ADCTRIG1 - CPU1 Timer 0, TINT0n
    //  02h ADCTRIG2 - CPU1 Timer 1, TINT1n
    //  03h ADCTRIG3 - CPU1 Timer 2, TINT2n
    //  04h ADCTRIG4 - GPIO, Input X-Bar INPUT5
    //  05h ADCTRIG5 - ePWM1, ADCSOCA
    //  06h ADCTRIG6 - ePWM1, ADCSOCB
    //  07h ADCTRIG7 - ePWM2, ADCSOCA
    //  08h ADCTRIG8 - ePWM2, ADCSOCB
    //  09h ADCTRIG9 - ePWM3, ADCSOCA
    //  0Ah ADCTRIG10 - ePWM3, ADCSOCB
    //  0Bh ADCTRIG11 - ePWM4, ADCSOCA
    //  0Ch ADCTRIG12 - ePWM4, ADCSOCB
    //  0Dh ADCTRIG13 - ePWM5, ADCSOCA
    //  0Eh ADCTRIG14 - ePWM5, ADCSOCB
    //  0Fh ADCTRIG15 - ePWM6, ADCSOCA
    //  10h ADCTRIG16 - ePWM6, ADCSOCB
    //  11h ADCTRIG17 - ePWM7, ADCSOCA
    //  12h ADCTRIG18 - ePWM7, ADCSOCB
    //  13h - 3Fh - Reserved
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC6CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC7CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC8CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC9CTL.bit.TRIGSEL         = 0;
    AdccRegs.ADCSOC10CTL.bit.TRIGSEL        = 0;
    AdccRegs.ADCSOC11CTL.bit.TRIGSEL        = 0;
    AdccRegs.ADCSOC12CTL.bit.TRIGSEL        = 0;
    AdccRegs.ADCSOC13CTL.bit.TRIGSEL        = 0;
    AdccRegs.ADCSOC14CTL.bit.TRIGSEL        = 0;
    AdccRegs.ADCSOC15CTL.bit.TRIGSEL        = 0;

    // Set SH Window = (ACQPS + 1) * SYSCLK
    AdccRegs.ADCSOC0CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC1CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC2CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC3CTL.bit.ACQPS           = ACQP_A;
    AdccRegs.ADCSOC4CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC5CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC6CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC7CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC8CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC9CTL.bit.ACQPS           = ACQP_C;
    AdccRegs.ADCSOC10CTL.bit.ACQPS          = ACQP_C;
    AdccRegs.ADCSOC11CTL.bit.ACQPS          = ACQP_C;
    AdccRegs.ADCSOC12CTL.bit.ACQPS          = ACQP_C;
    AdccRegs.ADCSOC13CTL.bit.ACQPS          = ACQP_C;
    AdccRegs.ADCSOC14CTL.bit.ACQPS          = ACQP_C;
    AdccRegs.ADCSOC15CTL.bit.ACQPS          = ACQP_C;

    // Set SOC Priority
    AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY   = 16;   // All Soc are defined as high priority

    // Burst Mode Set
    AdccRegs.ADCBURSTCTL.bit.BURSTEN        = 0;
    AdccRegs.ADCBURSTCTL.bit.BURSTSIZE      = 0;
    AdccRegs.ADCBURSTCTL.bit.BURSTTRIGSEL   = 0;

    // ADC PPBx Set
    AdccRegs.ADCPPB1CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdccRegs.ADCPPB1CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdccRegs.ADCPPB1OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdccRegs.ADCPPB1OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdccRegs.ADCPPB1TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdccRegs.ADCPPB1TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value
    
    AdccRegs.ADCPPB2CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdccRegs.ADCPPB2CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdccRegs.ADCPPB2OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdccRegs.ADCPPB2OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdccRegs.ADCPPB2TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdccRegs.ADCPPB2TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value

    AdccRegs.ADCPPB3CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdccRegs.ADCPPB3CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdccRegs.ADCPPB3OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdccRegs.ADCPPB3OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdccRegs.ADCPPB3TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdccRegs.ADCPPB3TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value

    AdccRegs.ADCPPB4CONFIG.bit.CONFIG       = 0;    // SOCx/EOCx/RESULTx is associated with post processing block 1
    AdccRegs.ADCPPB4CONFIG.bit.TWOSCOMPEN   = 0;    // ADCPPB1RESULT = ADCRESULTx - ADCPPBxOFFREF
    AdccRegs.ADCPPB4OFFCAL.bit.OFFCAL       = 0;    // ADC output - 0 is stored into ADCRESULT
    AdccRegs.ADCPPB4OFFREF                  = 0;    // ADCRESULT - 0 is stored into ADCPPBxRESULT
    AdccRegs.ADCPPB4TRIPHI.all              = 0;    // Sets the digital Comparator trip HI limit value
    AdccRegs.ADCPPB4TRIPLO.all              = 0;    // Sets the digital Comparator trip LO limit value

    AdccRegs.ADCEVTSEL.all                  = 0;    // Post Processing Block x Limit HI/LO Zero Crossing Event Disable
    AdccRegs.ADCEVTINTSEL.all               = 0;    // Post processing Block x Limit HI/LO Zero Crossing Interrupt Disable

    // Clear PPBx Zero Crossing Limit HI/LO Flag
    AdccRegs.ADCEVTCLR.all                  = 0xFFFF;

    // Clear SOC Over Flag
    AdccRegs.ADCSOCOVFCLR1.all              = 0xFFFF;

    // INT Set
    AdccRegs.ADCINTSEL1N2.bit.INT1E         = 0;
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL       = 0;
    AdccRegs.ADCINTSEL1N2.bit.INT1CONT      = 0;

    AdccRegs.ADCINTSEL1N2.bit.INT2E         = 0;
    AdccRegs.ADCINTSEL1N2.bit.INT2SEL       = 0;
    AdccRegs.ADCINTSEL1N2.bit.INT2CONT      = 0;

    AdccRegs.ADCINTSEL3N4.bit.INT3E         = 0;
    AdccRegs.ADCINTSEL3N4.bit.INT3SEL       = 0;
    AdccRegs.ADCINTSEL3N4.bit.INT3CONT      = 0;

    AdccRegs.ADCINTSEL3N4.bit.INT4E         = 0;
    AdccRegs.ADCINTSEL3N4.bit.INT4SEL       = 0;
    AdccRegs.ADCINTSEL3N4.bit.INT4CONT      = 0;

    // Clear ADC Interrupt 1/2/3/4 Flag
    AdccRegs.ADCINTFLGCLR.all               = 0x000F;

    // Clear ADC Interrupt 1/2/3/4 Overflow Flag
    AdccRegs.ADCINTOVFCLR.all               = 0x000F;    

    EDIS;
}
