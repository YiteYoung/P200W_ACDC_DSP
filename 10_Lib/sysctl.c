//###########################################################################
//
// FILE:   sysctl.c
//
// TITLE:  C28x system control driver.
//
//###########################################################################
// $Copyright:
// Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#include "sysctl.h"
#include "dcc.h"
#include "asysctl.h"

//
// Define to isolate inline assembly
//
#define SYSCTL_DELAY        __asm(" .if __TI_EABI__\n"                         \
                                  " .asg    SysCtl_delay    , _SysCtl_delay\n" \
                                  " .endif\n"                                  \
                                  " .def _SysCtl_delay\n"                      \
                                  " .sect \".TI.ramfunc\"\n"                   \
                                  " .global  _SysCtl_delay\n"                  \
                                  "_SysCtl_delay:\n"                           \
                                  " SUB    ACC,#1\n"                           \
                                  " BF     _SysCtl_delay, GEQ\n"               \
                                  " LRETR\n")

//
// Macro used to add wait cycles to allow load capacitors to charge
//
#define SYSCTL_XTAL_CHARGE_DELAY   asm(" RPT #5 || NOP");

//*****************************************************************************
//
// SysCtl_delay()
//
//*****************************************************************************
SYSCTL_DELAY;


//*****************************************************************************
//
// SysCtl_pollX1Counter()
//
//*****************************************************************************
static bool
SysCtl_pollX1Counter(void)
{
    uint16_t loopCount = 0U;
    uint32_t localCounter = 0U;
    bool status = false;

    //
    // Delay for 1 ms while the XTAL powers up
    //
    // 2000 loops, 5 cycles per loop + 9 cycles overhead = 10009 cycles
    //
    SysCtl_delay(2000);

    //
    // Clear and saturate X1CNT 4 times to guarantee operation
    //
    do
    {
        //
        // Keep clearing the counter until it is no longer saturated
        //
        while(HWREG(CLKCFG_BASE + SYSCTL_O_X1CNT) > 0x1FFU)
        {
            HWREG(CLKCFG_BASE + SYSCTL_O_X1CNT) |= SYSCTL_X1CNT_CLR;
        }

        //
        // Wait for the X1 clock to saturate
        //
        while(HWREGH(CLKCFG_BASE + SYSCTL_O_X1CNT) != SYSCTL_X1CNT_X1CNT_M)
        {
            //
            // If your application is stuck in this loop, please check if the
            // input clock source is valid.
            //
            localCounter++;
            if(localCounter>2500000U)
            {
                if(loopCount == 3U)
                {
                    status = false;
                }
                break;
            }
        }

        if(loopCount == 3U &&
           (HWREGH(CLKCFG_BASE + SYSCTL_O_X1CNT) == SYSCTL_X1CNT_X1CNT_M))
        {
            status = true;
        }
        //
        // Increment the counter
        //
        loopCount++;
        localCounter = 0U;
    }while(loopCount < 4U);

    return status;
}

//*****************************************************************************
//
// SysCtl_getClock()
//
//*****************************************************************************
uint32_t
SysCtl_getClock(uint32_t clockInHz)
{
    uint32_t temp;
    uint32_t oscSource;
    uint32_t clockOut;

    //
    // Don't proceed if an MCD failure is detected.
    //
    if(SysCtl_isMCDClockFailureDetected())
    {
        //
        // OSCCLKSRC2 failure detected. Returning the INTOSC1 rate. You need
        // to handle the MCD and clear the failure.
        //
        clockOut = SYSCTL_DEFAULT_OSC_FREQ;
    }
    else
    {
        //
        // If one of the internal oscillators is being used, start from the
        // known default frequency.  Otherwise, use clockInHz parameter.
        //
        oscSource = HWREG(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
                    (uint32_t)SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;

        if((oscSource == (SYSCTL_OSCSRC_OSC2 >> SYSCTL_OSCSRC_S)) ||
           (oscSource == (SYSCTL_OSCSRC_OSC1 >> SYSCTL_OSCSRC_S)))
        {
            clockOut = SYSCTL_DEFAULT_OSC_FREQ;
        }
        else
        {
            clockOut = clockInHz;
        }

        //
        // If the PLL is enabled calculate its effect on the clock
        //
        if((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &
            (SYSCTL_SYSPLLCTL1_PLLEN | SYSCTL_SYSPLLCTL1_PLLCLKEN)) == 3U)
        {
            //
            // Calculate integer multiplier
            //
            clockOut = clockOut * ((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                                    SYSCTL_SYSPLLMULT_IMULT_M) >>
                                   SYSCTL_SYSPLLMULT_IMULT_S);

            //
            // Calculate PLL divider
            //
            temp = ((((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                       SYSCTL_SYSPLLMULT_REFDIV_M) >>
                      SYSCTL_SYSPLLMULT_REFDIV_S) + 1U) *
                    (((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                       SYSCTL_SYSPLLMULT_ODIV_M) >>
                      SYSCTL_SYSPLLMULT_ODIV_S) + 1U));

            //
            //  Divide dividers
            //
            if(temp != 0U)
            {
                clockOut /= temp;
            }
        }

        if((HWREG(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
            SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M) != 0U)
        {
            uint32_t divider = 2U *
                               (HWREG(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
                                SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M);
            if((HWREG(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
                SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_LSB) != 0U)
            {
                divider++;
            }

            clockOut /= divider;
        }
    }

    return(clockOut);
}

//*****************************************************************************
//
// SysCtl_setClock()
//
//*****************************************************************************
bool
SysCtl_setClock(uint32_t config)
{
    uint16_t divSel, pllen, oscclksrcsel, pllLockStatus, xtalval;
    uint32_t oscSource, pllMult, mult;
    uint32_t timeout, refdiv;
    bool status = false;

    //
    // Check the arguments.
    //
    ASSERT((config & SYSCTL_OSCSRC_M) <= SYSCTL_OSCSRC_M);
    ASSERT(((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_ENABLE) ||
           ((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_BYPASS) ||
           ((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_DISABLE));

    //
    // Don't proceed to the PLL initialization if an MCD failure is detected.
    //
    if(SysCtl_isMCDClockFailureDetected())
    {
        //
        // OSCCLKSRC2 failure detected. Returning false. You'll need to clear
        // the MCD error.
        //
        status = false;
    }
    else
    {
        if((config & SYSCTL_OSCSRC_EXTROSC2) != 0U)
        {
            //
            // Switch OSCCLKSRC to INTOSC1 before switching INTOSC2 from INTR
            // to EXTR
            //
            SysCtl_selectOscSource(SYSCTL_OSCSRC_OSC1);
            SysCtl_delay(200U);

            //
            // Write EXTROSCCSR1 delay based on 10nf cap.
            //
            HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_EXTROSCCSR1) = 0x0000C00U;

            //
            // Switch INTOSC to XROSC
            //
            SysCtl_setIntOSC2_Mode(SYSCTL_INTOSC2_MODE_EXTR);
            SysCtl_delay(10000U);

            while(ASysCtl_getExtROscStatus() != ASYSCTL_EXTR_ENABLE_COMPLETE);
        }

        //
        // Bypass PLL
        //
        EALLOW;
        HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &=
            ~SYSCTL_SYSPLLCTL1_PLLCLKEN;
        EDIS;

        //
        // Delay of at least 120 OSCCLK cycles required post PLL bypass
        //
        SysCtl_delay(23U);

        //
        // Derive the current and previous oscillator clock source values
        //
        oscclksrcsel = HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
                      (uint16_t)SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;

        xtalval = (HWREGH(CLKCFG_BASE + SYSCTL_O_XTALCR) &
                  (uint16_t)SYSCTL_XTALCR_SE);

        oscSource = (config & SYSCTL_OSCSRC_M) >> SYSCTL_OSCSRC_S;

        //
        // Check if the oscillator clock source has changed
        //
        if((oscclksrcsel | xtalval) != oscSource)
        {
            //
            // Turn off PLL
            //
            EALLOW;
            HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &=
                ~SYSCTL_SYSPLLCTL1_PLLEN;
            EDIS;

            //
            // Delay of at least 66 OSCCLK cycles required between
            // powerdown to powerup of PLL
            //
            SysCtl_delay(12U);

            //
            // Configure oscillator source
            //
            SysCtl_selectOscSource(config & SYSCTL_OSCSRC_M);

            //
            // Delay of at least 60 OSCCLK cycles
            //
            SysCtl_delay(11U);
        }

        //
        // Set dividers to /1 to ensure the fastest PLL configuration
        //
        SysCtl_setPLLSysClk(1U);

        //
        // Configure PLL if PLL usage is enabled or bypassed in config
        //
        if(((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_ENABLE) ||
           ((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_BYPASS))
        {
            //
            // Get the PLL multiplier settings from config
            //
            pllMult  = ((config & SYSCTL_IMULT_M) <<
                         SYSCTL_SYSPLLMULT_IMULT_S);

            pllMult |= (((config & SYSCTL_REFDIV_M) >>
                        SYSCTL_REFDIV_S) <<
                        SYSCTL_SYSPLLMULT_REFDIV_S);

            pllMult |= (((config & SYSCTL_ODIV_M) >>
                          SYSCTL_ODIV_S) <<
                          SYSCTL_SYSPLLMULT_ODIV_S);

            //
            // Get the PLL multipliers currently programmed
            //
            mult  = ((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                      SYSCTL_SYSPLLMULT_IMULT_M) >>
                      SYSCTL_SYSPLLMULT_IMULT_S);

            mult |= (HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                     SYSCTL_SYSPLLMULT_REFDIV_M);

            mult |= (HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                     SYSCTL_SYSPLLMULT_ODIV_M);

            pllen = (HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &
                     SYSCTL_SYSPLLCTL1_PLLEN);

            //
            // Lock PLL only if the multipliers need an update or PLL needs
            // to be powered on / enabled
            //
            if((mult !=  pllMult) || (pllen != 1U))
            {
                //
                // Turn off PLL
                //
                EALLOW;
                HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &=
                    ~SYSCTL_SYSPLLCTL1_PLLEN;
                EDIS;

                //
                // Delay of at least 66 OSCCLK cycles required between
                // powerdown to powerup of PLL
                //
                SysCtl_delay(12U);

                //
                // Write multiplier, which automatically turns on the PLL
                //
                EALLOW;
                HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) = pllMult;

                //
                // Enable/ turn on PLL
                //
                HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) |=
                       SYSCTL_SYSPLLCTL1_PLLEN;

                //
                // Wait for the SYSPLL lock counter or a timeout
                // This timeout needs to be computed based on OSCCLK
                // with a factor of REFDIV.
                // Lock time is 1024 OSCCLK * (REFDIV+1)
                //
                refdiv  = ((config & SYSCTL_REFDIV_M) >> SYSCTL_REFDIV_S);

                timeout = (1024U * (refdiv + 1U));
                pllLockStatus = HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLSTS) &
                                SYSCTL_SYSPLLSTS_LOCKS;

                while((pllLockStatus != 1U) && (timeout != 0U))
                {
                    pllLockStatus = HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLSTS) &
                                    SYSCTL_SYSPLLSTS_LOCKS;
                    timeout--;
                }
                EDIS;

                //
                // Check PLL Frequency using DCC
                //
               status = SysCtl_isPLLValid(
                        (config & SYSCTL_DCC_BASE_M ),
                        (config & SYSCTL_OSCSRC_M),
                        (config & (SYSCTL_IMULT_M | SYSCTL_ODIV_M |
                                   SYSCTL_REFDIV_M)));

            }
            else
            {
                //
                // Re-Lock of PLL not needed since the multipliers
                // are not updated
                //
                status = true;
            }
        }
        else if((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_DISABLE)
        {
            //
            // Turn off PLL when the PLL is disabled in config
            //
            EALLOW;
            HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &=
                   ~SYSCTL_SYSPLLCTL1_PLLEN;
            SYSCTL_REGWRITE_DELAY;
            EDIS;

            //
            // PLL is bypassed and not in use
            // Status is updated to true to allow configuring the dividers later
            //
            status = true;
        }
        else
        {
            //
            // Empty
            //
        }

        //
        // If PLL locked successfully, configure the dividers
        // Or if PLL is bypassed, only configure the dividers
        //
        if(status)
        {
            //
            // Set divider to produce slower output frequency to limit current
            // increase.
            //
            divSel = (uint16_t)(config & SYSCTL_SYSDIV_M) >> SYSCTL_SYSDIV_S;

            if(divSel == 126U)
            {
                SysCtl_setPLLSysClk(divSel);
            }
            else
            {
                SysCtl_setPLLSysClk(divSel + 2U);
            }

            //
            // Feed system clock from SYSPLL only if PLL usage is enabled
            //
            if((config & SYSCTL_PLL_CONFIG_M) == SYSCTL_PLL_ENABLE)
            {

                //
                // Enable PLLSYSCLK is fed from system PLL clock
                //
                EALLOW;
                HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) |=
                       SYSCTL_SYSPLLCTL1_PLLCLKEN;
                EDIS;

            }

            //
            // ~200 PLLSYSCLK delay to allow voltage regulator to stabilize
            // prior to increasing entire system clock frequency.
            //
            SysCtl_delay(40U);

            //
            // Set the divider to user value
            //
            SysCtl_setPLLSysClk(divSel);
        }
        else
        {
            ESTOP0; // If the frequency is out of range, stop here.
        }
    }

    return(status);
}

//*****************************************************************************
//
// SysCtl_selectXTAL()
//
//*****************************************************************************
void
SysCtl_selectXTAL(void)
{
    bool status = false;
    uint16_t loopCount = 0U;

    EALLOW;

    //
    // Enable XOSC pads initialization and set X1, X2 high
    //
    HWREGH(CLKCFG_BASE + SYSCTL_O_XTALCR2) |= SYSCTL_XTALCR2_FEN |
                                              SYSCTL_XTALCR2_XIF |
                                              SYSCTL_XTALCR2_XOF;
    //
    // Wait for few cycles to allow load capacitors to charge
    //
    SYSCTL_XTAL_CHARGE_DELAY;

    //
    // Turn on XTAL and select crystal mode
    //
    HWREGH(CLKCFG_BASE + SYSCTL_O_XTALCR) &= ~SYSCTL_XTALCR_OSCOFF;
    SYSCTL_REGWRITE_DELAY;
    HWREGH(CLKCFG_BASE + SYSCTL_O_XTALCR) &= ~SYSCTL_XTALCR_SE;
    SYSCTL_REGWRITE_DELAY;
    EDIS;

    //
    // Wait for the X1 clock to saturate
    //
    status = SysCtl_pollX1Counter();

    //
    // Select XTAL as the oscillator source
    //
    EALLOW;
    HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) =
    ((HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
      (~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M)) |
     (SYSCTL_OSCSRC_XTAL >> SYSCTL_OSCSRC_S));
    SYSCTL_CLKSRCCTL_DELAY;
    EDIS;

    //
    // If a missing clock failure was detected, try waiting for the X1 counter
    // to saturate again. Consider modifying this code to add a 10ms timeout.
    //
    while(SysCtl_isMCDClockFailureDetected() && (status == (bool)FALSE) &&
          (loopCount < 4U))
    {
        //
        // Clear the MCD failure
        //
        SysCtl_resetMCD();

        //
        // Wait for the X1 clock to saturate
        //
        status = SysCtl_pollX1Counter();

        //
        // Select XTAL as the oscillator source
        //
        EALLOW;
        HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) =
        ((HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
          (~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M)) |
         (SYSCTL_OSCSRC_XTAL >> SYSCTL_OSCSRC_S));
        EDIS;
        loopCount ++;
    }
    while(status == (bool)FALSE)
    {         
        // If code is stuck here, it means crystal has not started.  
        //Replace crystal or update code below to take necessary actions if 
        //crystal is bad         
        ESTOP0;     
    }
}

//*****************************************************************************
//
// SysCtl_selectXTALSingleEnded()
//
//*****************************************************************************
void
SysCtl_selectXTALSingleEnded(void)
{
    bool status = false;
    
    //
    // Turn on XTAL and select single-ended mode.
    //
    EALLOW;
    HWREGH(CLKCFG_BASE + SYSCTL_O_XTALCR) &= ~SYSCTL_XTALCR_OSCOFF;
    SYSCTL_REGWRITE_DELAY;
    HWREGH(CLKCFG_BASE + SYSCTL_O_XTALCR) |= SYSCTL_XTALCR_SE;
    SYSCTL_REGWRITE_DELAY;
    EDIS;

    //
    // Wait for the X1 clock to saturate
    //
    status = SysCtl_pollX1Counter();

    //
    // Select XTAL as the oscillator source
    //
    EALLOW;
    HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) =
    ((HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
      (~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M)) |
     (SYSCTL_OSCSRC_XTAL >> SYSCTL_OSCSRC_S));
    EDIS;

    //
    // Something is wrong with the oscillator module. Replace the ESTOP0 with
    // an appropriate error-handling routine.
    //
    while(SysCtl_isMCDClockFailureDetected() && (status == (bool)FALSE))
    {
        // If code is stuck here, it means crystal has not started.  
        //Replace crystal or update code below to take necessary actions if 
        //crystal is bad 
        ESTOP0;
    }
}

//*****************************************************************************
//
// SysCtl_selectOscSource()
//
//*****************************************************************************
void
SysCtl_selectOscSource(uint32_t oscSource)
{
    ASSERT((oscSource == SYSCTL_OSCSRC_OSC1) ||
           (oscSource == SYSCTL_OSCSRC_OSC2) ||
           (oscSource == SYSCTL_OSCSRC_XTAL) ||
           (oscSource == SYSCTL_OSCSRC_XTAL_SE));

    //
    // Select the specified source.
    //
    EALLOW;
    switch(oscSource)
    {
        case SYSCTL_OSCSRC_OSC2:
            //
            // Clk Src = INTOSC2
            //
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                ~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;

            break;

        case SYSCTL_OSCSRC_XTAL:
            //
            // Select XTAL in crystal mode and wait for it to power up
            //
            SysCtl_selectXTAL();
            break;

        case SYSCTL_OSCSRC_XTAL_SE:
            //
            // Select XTAL in single-ended mode and wait for it to power up
            //
            SysCtl_selectXTALSingleEnded();
            break;

        case SYSCTL_OSCSRC_OSC1:
            //
            // Clk Src = INTOSC1
            //
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) =
                   (HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
                    ~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M) |
                   (SYSCTL_OSCSRC_OSC1 >> SYSCTL_OSCSRC_S);

            SYSCTL_CLKSRCCTL_DELAY;
            break;

        default:
            //
            // Do nothing. Not a valid oscSource value.
            //
            break;
    }
    EDIS;
}

//*****************************************************************************
//
// SysCtl_getLowSpeedClock()
//
//*****************************************************************************
uint32_t
SysCtl_getLowSpeedClock(uint32_t clockInHz)
{
    uint32_t clockOut;

    //
    // Get the main system clock
    //
    clockOut = SysCtl_getClock(clockInHz);

    //
    // Apply the divider to the main clock
    //
    if((HWREG(CLKCFG_BASE + SYSCTL_O_LOSPCP) &
        SYSCTL_LOSPCP_LSPCLKDIV_M) != 0U)
    {
        clockOut /= (2U * (HWREG(CLKCFG_BASE + SYSCTL_O_LOSPCP) &
                            SYSCTL_LOSPCP_LSPCLKDIV_M));
    }

    return(clockOut);
}

//*****************************************************************************
//
// SysCtl_getDeviceParametric()
//
//*****************************************************************************
uint16_t
SysCtl_getDeviceParametric(SysCtl_DeviceParametric parametric)
{
    uint32_t value;

    //
    // Get requested parametric value
    //
    switch(parametric)
    {
        case SYSCTL_DEVICE_QUAL:
            //
            // Qualification Status
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_QUAL_M) >> SYSCTL_PARTIDL_QUAL_S);
            break;

        case SYSCTL_DEVICE_PINCOUNT:
            //
            // Pin Count
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_PIN_COUNT_M) >>
                     SYSCTL_PARTIDL_PIN_COUNT_S);
            break;

        case SYSCTL_DEVICE_INSTASPIN:
            //
            // InstaSPIN Feature Set
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_INSTASPIN_M) >>
                     SYSCTL_PARTIDL_INSTASPIN_S);
            break;

        case SYSCTL_DEVICE_FAMILY:
            //
            // Device Family
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDH) &
                      SYSCTL_PARTIDH_FAMILY_M) >> SYSCTL_PARTIDH_FAMILY_S);
            break;

        case SYSCTL_DEVICE_PARTNO:
            //
            // Part Number
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDH) &
                      SYSCTL_PARTIDH_PARTNO_M) >> SYSCTL_PARTIDH_PARTNO_S);
            break;

        case SYSCTL_DEVICE_CLASSID:
            //
            // Class ID
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDH) &
                      SYSCTL_PARTIDH_DEVICE_CLASS_ID_M) >>
                     SYSCTL_PARTIDH_DEVICE_CLASS_ID_S);
            break;

        default:
            //
            // Not a valid value for PARTID register
            //
            value = 0U;
            break;
    }

    return((uint16_t)value);
}
//*****************************************************************************
//
// SysCtl_isPLLValid()
//
//*****************************************************************************
bool
SysCtl_isPLLValid(uint32_t base, uint32_t oscSource,
                  uint32_t pllMultDiv)
{
    uint32_t imult, odiv, refdiv;
    float  fclk1_0ratio, fclk0_1ratio;
    float32_t total_error, window;

    ASSERT(base == SYSCTL_DCC_BASE_0);

    DCC_Count0ClockSource dccClkSrc0;
    DCC_Count1ClockSource dccClkSrc1;
    uint32_t dccCounterSeed0, dccCounterSeed1, dccValidSeed0;

    switch(oscSource)
    {
        case SYSCTL_OSCSRC_OSC2:
            //
            // Select DCC Clk Src0 as INTOSC2
            //
            dccClkSrc0 = DCC_COUNT0SRC_INTOSC2;
            break;
        case SYSCTL_OSCSRC_XTAL:
        case SYSCTL_OSCSRC_XTAL_SE:
            //
            // Select DCC Clk Src0 as XTAL
            //
            dccClkSrc0 = DCC_COUNT0SRC_XTAL;
            break;
        case SYSCTL_OSCSRC_OSC1:
            //
            // Select DCC Clk Src0 as INTOSC1
            //
            dccClkSrc0 = DCC_COUNT0SRC_INTOSC1;
            break;
        default:
            //
            // Select DCC Clk Src0 as INTOSC1
            //
            dccClkSrc0 = DCC_COUNT0SRC_INTOSC1;
            break;
    }

    //
    // Select DCC Clk Src1 as SYSPLL
    //
    dccClkSrc1 = DCC_COUNT1SRC_PLL;

    //
    // Assigning DCC for PLL validation
    //
    base = DCC0_BASE;

    //
    // Retrieving PLL parameters
    //
    imult = pllMultDiv & SYSCTL_IMULT_M;
    odiv = (pllMultDiv & SYSCTL_ODIV_M) >> SYSCTL_ODIV_S;
    refdiv = (pllMultDiv & SYSCTL_REFDIV_M) >> SYSCTL_REFDIV_S;

    fclk1_0ratio = (float)imult / (((float)odiv + 1.0F) *
                   ((float)refdiv + 1.0F));
    fclk0_1ratio = (((float)refdiv + 1.0F) *
                   ((float)odiv + 1.0F)) / (float)imult;

    if(fclk1_0ratio >= 1.0F)
    {
        //
        // Setting Counter0 & Valid Seed Value with +/-1% tolerance
        // Total error is 12
        //
        window = (12U * 100U) / ((uint32_t)SYSCTL_DCC_COUNTER0_TOLERANCE);
        dccCounterSeed0 = (uint32_t)window - 12U;
        dccValidSeed0 = 24U;
    }
    else
    {
        total_error = ((2.0F * fclk0_1ratio) + 10.0F);
        window = (total_error * 100.0F) /
                 ((float32_t)SYSCTL_DCC_COUNTER0_TOLERANCE);

        //
        // Setting Counter0 & Valid Seed Value with +/-1% tolerance
        //
        dccCounterSeed0 = window - total_error;
        dccValidSeed0 = 2U * (uint32_t)total_error;
    }

    //
    // Multiplying Counter-0 window with PLL Integer Multiplier
    //
    dccCounterSeed1 = (window * fclk1_0ratio);

    //
    // Enable peripheral clock to DCC
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DCC0);

    //
    // Clear Error & Done Flag
    //
    DCC_clearErrorFlag(base);
    DCC_clearDoneFlag(base);

    //
    // Disable DCC
    //
    DCC_disableModule(base);

    //
    // Disable Error Signal
    //
    DCC_disableErrorSignal(base);

    //
    // Disable Done Signal
    //
    DCC_disableDoneSignal(base);

    //
    // Configure Clock Source0 to whatever set as a clock source for PLL
    //
    DCC_setCounter0ClkSource(base, dccClkSrc0);

    //
    // Configure Clock Source1 to PLL
    //
    DCC_setCounter1ClkSource(base, dccClkSrc1);

    //
    // Configure COUNTER-0, COUNTER-1 & Valid Window
    //
    DCC_setCounterSeeds(base, dccCounterSeed0, dccValidSeed0,
                        dccCounterSeed1);

    //
    // Enable Single Shot mode
    //
    DCC_enableSingleShotMode(base, DCC_MODE_COUNTER_ZERO);


    //
    // Enable DCC to start counting
    //
    DCC_enableModule(base);

    //
    // Wait until Error or Done Flag is generated
    //
    while((HWREGH(base + DCC_O_STATUS) &
           (DCC_STATUS_ERR | DCC_STATUS_DONE)) == 0U)
    {
    }

    //
    // Returns true if DCC completes without error
    //
    return((HWREGH(base + DCC_O_STATUS) &
            (DCC_STATUS_ERR | DCC_STATUS_DONE)) == DCC_STATUS_DONE);
}
//*****************************************************************************
//
// SysCtl_configureType()
//
//*****************************************************************************
void
SysCtl_configureType(SysCtl_SelType type , uint16_t config, uint16_t lock)
{
    ASSERT(lock <= 1U);

    EALLOW;

    //
    // Check which type needs to be configured , the type would be enabled /
    // disabled along with making the configurations unalterable as per input.
    //
    switch(type)
    {
        case SYSCTL_ECAPTYPE:
            HWREGH(DEVCFG_BASE + SYSCTL_O_ECAPTYPE) =
                                config | (lock << SYSCTL_TYPE_LOCK_S);
            break;

        default:
            break;
    }
    EDIS;

}

//*****************************************************************************
//
// SysCtl_isConfigTypeLocked()
//
//*****************************************************************************
bool
SysCtl_isConfigTypeLocked(SysCtl_SelType type)
{
    bool lock = false;

    //
    // Check if the provided type registers can be modified or not.
    //
    switch(type)
    {
        case SYSCTL_ECAPTYPE:
            lock = ((HWREGH(DEVCFG_BASE + SYSCTL_O_ECAPTYPE) &
                     SYSCTL_ECAPTYPE_LOCK) != 0U);
            break;

        default:
            break;
    }

  return(lock);
}

//*****************************************************************************
//
// SysCtl_lockClkConfig()
//
//*****************************************************************************
void
SysCtl_lockClkConfig(SysCtl_ClkRegSel registerName)
{
    uint16_t bitIndex;

    //
    // Decode the register variable.
    //
    bitIndex = ((uint16_t)registerName & SYSCTL_PERIPH_BIT_M) >>
                SYSCTL_PERIPH_BIT_S;

    //
    // Locks the particular clock configuration register
    //
    EALLOW;
    HWREG(CLKCFG_BASE + SYSCTL_O_CLKCFGLOCK1) |= (1UL << bitIndex);
    EDIS;
}

//*****************************************************************************
//
// SysCtl_lockSysConfig()
//
//*****************************************************************************
void
SysCtl_lockSysConfig (SysCtl_CpuRegSel registerName)
{
    uint16_t regIndex;
    uint16_t bitIndex;

    //
    // Decode the register variable.
    //
    regIndex = 2U * ((uint16_t)registerName & SYSCTL_PERIPH_REG_M);
    bitIndex = ((uint16_t)registerName & SYSCTL_PERIPH_BIT_M) >>
                 SYSCTL_PERIPH_BIT_S;

    //
    // Locks the particular System configuration register
    //
    EALLOW;
    HWREG(CPUSYS_BASE + SYSCTL_O_CPUSYSLOCK1 + regIndex) |=
                                            (1UL << bitIndex);
    EDIS;
}


