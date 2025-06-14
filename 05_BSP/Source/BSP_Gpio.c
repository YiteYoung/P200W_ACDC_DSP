#include "f28x_project.h"
#include "BSP_Gpio.h"

// =========================== Power Board ================================= //
// GPIO0    PWM1A           LLC_PWM_BatForward_Hi
// GPIO1    PWM1B           LLC_PWM_BatForward_Lo
// GOIO2    PWM2A           LLC_PWM_BatBehind_Hi
// GPIO3    PWM2B           LLC_PWM_BatBehind_Lo
// GPIO4    PWM3A           LLC_PWM_HVForward_Hi
// GPIO5    PWM3B           LLC_PWM_HVForward_Lo
// GPIO6    PWM4A           LLC_PWM_HVBehind_Hi
// GPIO7    PWM4B           LLC_PWM_HVBehind_Lo
// GPIO8    PWM5A           PFC_PWM_Ind_Hi
// GPIO9    PWM5B           PFC_PWM_Ind_Lo
// GPIO10   PWM6A           PFC_PWM_NoInd_Hi
// GPIO11   PWM6B           PFC_PWM_NOInd_Lo
// GPIO12   PWM7A           FAN_PWM
//
// GPIO13   GPIO            TZ1：VBat_Hi/S12V_Low/OST
// GPIO16   GPIO            TZ2：Vbus_Hi/H12V_Low/OST
// GPIO17   GPIO            TZ3：PFC_Curr_Hi/CBC
// GPIO18   X1              X1
// GPIO19   X2              X2
//
// GPIO20   GPIO            Debug_Led
// GPIO21
// GPIO22   SCIB_TX         Debug_SciB_Tx
// GPIO23   SCIB_RX         Debug_SciB_Rx
// GPIO24   GPIO            3.3V
// GPIO28   SCIA_RX         Main_SciA_Rx
// GPIO29   SCIA_TX         Main_SciA_Tx
//
// GPIO32   GPIO            Rly_Grid_L
// GPIO33   GPIO            Rly_Grid_N
// GPIO39   GPIO            Rly_Soft_L
// GPIO40   GPIO            Rly_Inv_Out
// ========================================================================= //
// A0/C15   ADC             BatVolt_Sample
// A1       ADC             InvOutCurr_Sample
// A2/C9    ADC             InvOutVolt_Sample
// A3/C5    ADC             H_NTC1
// A4/C14   ADC             H_NTC2
// A5/C2    ADC             H_NTC3
// A6       ADC             GridVolt_Sample
// A7/C3    ADC             LLCRsCurr_Sample
// A11/C0   ADC             PFCIndCurr_Sample
// A12/C1   ADC             BatCurr_Sample
// C4/A14   
// A15/C7   ADC             InvOutCurr_Sample
// C6       ADC             BusVolt_Sample
// ========================================================================= //           
void sInitGpio(void)
{
    // Clear All Gpio Config
    InitGpio();

    // User Config
    GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 1;
}
