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
// GPIO41   GPIO            ？
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

    EALLOW;

    // Defines pin-muxing selection for GPIO
    GpioCtrlRegs.GPAGMUX1.all                   = 0x00000000;
    GpioCtrlRegs.GPAGMUX2.all                   = 0x00000000;
    GpioCtrlRegs.GPBGMUX1.all                   = 0x00000000;
    GpioCtrlRegs.GPBGMUX1.all                   = 0x00000CC0;
    GpioCtrlRegs.GPHGMUX1.all                   = 0x00000000;
    GpioCtrlRegs.GPHGMUX2.all                   = 0x00000000;

    // Selects between inverted and non-inverted input
    GpioCtrlRegs.GPAINV.all                     = 0x00000000;
    GpioCtrlRegs.GPBINV.all                     = 0x00000000;
    GpioCtrlRegs.GPHINV.all                     = 0x00000000;

    // Selects between normal and open-drain output
    GpioCtrlRegs.GPAODR.all                     = 0x00000000;
    GpioCtrlRegs.GPBODR.all                     = 0x00000000;
    GpioCtrlRegs.GPHODR.all                     = 0x00000000;

    // GPIO sample period
    // 0x00,QUALPRDx = PLLSYSCLK
    // 0x01,QUALPRDx = PLLSYSCLK/2
    // 0x02,QUALPRDx = PLLSYSCLK/4
    // ....
    // 0xFF,QUALPRDx = PLLSYSCLK/510
    GpioCtrlRegs.GPACTRL.bit.QUALPRD0           = 1;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1           = 1;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD2           = 1;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3           = 1;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0           = 1;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD1           = 1;
    GpioCtrlRegs.GPHCTRL.bit.QUALPRD0           = 1;
    GpioCtrlRegs.GPHCTRL.bit.QUALPRD1           = 1;
    GpioCtrlRegs.GPHCTRL.bit.QUALPRD2           = 1;

    // Select input qualification type 
    // 00,Sync
    // 01,Qualification (3 samples)
    // 10,Qualification (6 samples)
    // 11,Async (no Sync or Qualification) 
    GpioCtrlRegs.GPAQSEL1.all                   = 0x00000000;
    GpioCtrlRegs.GPAQSEL2.all                   = 0x00000000;
    GpioCtrlRegs.GPBQSEL1.all                   = 0x00000CC0;
    GpioCtrlRegs.GPHQSEL1.all                   = 0x00000000;
    GpioCtrlRegs.GPHQSEL2.all                   = 0x00000000;

    // AGPIO Configuration
    // Table shows how to configure the AGPIOs.
    // To enable the analog functionality, set the register AGPIOCTRLx from analog subsystem. 
    // To enable the digital functionality, set the register GPxAMSEL
    // || ======================================================== ||
    // || AGPIOCTRLx.GPIOy  || GPxAMSEL.GPIOy   || Pin Connect To: ||
    // ||  (Default = 0)    ||  (Default = 1)   ||  ADC || GPIOy   ||
    // ||       0           ||      0           ||   -  ||   Yes   ||
    // ||       0           ||      1           ||   -  ||    -    ||
    // ||       1           ||      0           ||   -  ||   Yes   ||
    // ||       1           ||      1           ||  Yes ||   -     ||
    // || ======================================================== ||
    AnalogSubsysRegs.AGPIOCTRLA.all             = 0x00000000;
    GpioCtrlRegs.GPAAMSEL.all                   = 0xFFFFFFFF;
    AnalogSubsysRegs.AGPIOCTRLH.all             = 0x00000000;
    GpioCtrlRegs.GPHAMSEL.all                   = 0xFFFFFFFF;

    // GPIOA Analog Select
    AnalogSubsysRegs.AGPIOCTRLA.bit.GPIO12      = 0;
    GpioCtrlRegs.GPAAMSEL.bit.GPIO12            = 0;

    AnalogSubsysRegs.AGPIOCTRLA.bit.GPIO13      = 0;
    GpioCtrlRegs.GPAAMSEL.bit.GPIO13            = 0;

    AnalogSubsysRegs.AGPIOCTRLA.bit.GPIO20      = 0;
    GpioCtrlRegs.GPAAMSEL.bit.GPIO20            = 0;

    AnalogSubsysRegs.AGPIOCTRLA.bit.GPIO21      = 0;
    GpioCtrlRegs.GPAAMSEL.bit.GPIO21            = 0;

    AnalogSubsysRegs.AGPIOCTRLA.bit.GPIO28      = 0;
    GpioCtrlRegs.GPAAMSEL.bit.GPIO28            = 0;

    // GPIOH Analog Select
    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO224     = 1;
    GpioCtrlRegs.GPHAMSEL.bit.GPIO224           = 1;

    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO226     = 1;
    GpioCtrlRegs.GPHAMSEL.bit.GPIO226           = 1;

    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO227     = 0;
    GpioCtrlRegs.GPHAMSEL.bit.GPIO227           = 0;

    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO228     = 1;
    GpioCtrlRegs.GPHAMSEL.bit.GPIO228           = 1;

    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO230     = 1;
    GpioCtrlRegs.GPHAMSEL.bit.GPIO230           = 1;

    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO242     = 1;
    GpioCtrlRegs.GPHAMSEL.bit.GPIO242           = 1;

    // GPIO A
    GpioCtrlRegs.GPAPUD.bit.GPIO0               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO1               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO2               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO3               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO4               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO5               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO6               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO6              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO7               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO8               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO8              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO9               = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO10              = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO10             = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO11              = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11             = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO12              = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12             = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO13              = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO13              = 0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13            = 2;

    GpioCtrlRegs.GPAPUD.bit.GPIO16              = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO16              = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16            = 2;

    GpioCtrlRegs.GPAPUD.bit.GPIO17              = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO17              = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17            = 2;

    GpioCtrlRegs.GPAPUD.bit.GPIO18              = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO18              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO20              = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO20              = 1;

    GpioCtrlRegs.GPAPUD.bit.GPIO21              = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO21             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO21              = 1;

    GpioCtrlRegs.GPAPUD.bit.GPIO22              = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO22             = 1;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22            = 1;

    GpioCtrlRegs.GPAPUD.bit.GPIO23              = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO23             = 1;

    GpioCtrlRegs.GPAPUD.bit.GPIO24              = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO24             = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO24              = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO28              = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28             = 1;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28            = 1;

    GpioCtrlRegs.GPAPUD.bit.GPIO29              = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29             = 1;
    
    // GPIO B
    GpioCtrlRegs.GPBPUD.bit.GPIO32              = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32             = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32              = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO32            = 1;

    GpioCtrlRegs.GPBPUD.bit.GPIO33              = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO33             = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO33              = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO33            = 1;

    GpioCtrlRegs.GPBPUD.bit.GPIO39              = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO39             = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO39              = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO39            = 1;

    GpioCtrlRegs.GPBPUD.bit.GPIO40              = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO40             = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO40              = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO40            = 1;

    GpioCtrlRegs.GPBPUD.bit.GPIO41              = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO41             = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO41              = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO41            = 1;

    // AGPIO
    // GpioCtrlRegs.GPHPUD.bit.GPIO224             = 1;
    // GpioCtrlRegs.GPHMUX1.bit.GPIO224            = 0;
    // GpioCtrlRegs.GPHDIR.bit.GPIO224             = 0;

    // GpioCtrlRegs.GPHPUD.bit.GPIO226             = 1;
    // GpioCtrlRegs.GPHMUX1.bit.GPIO226            = 0;
    // GpioCtrlRegs.GPHDIR.bit.GPIO226             = 0;

    GpioCtrlRegs.GPHPUD.bit.GPIO227             = 1;
    GpioCtrlRegs.GPHMUX1.bit.GPIO227            = 0;
    GpioCtrlRegs.GPHDIR.bit.GPIO227             = 0;

    // GpioCtrlRegs.GPHPUD.bit.GPIO228             = 1;
    // GpioCtrlRegs.GPHMUX1.bit.GPIO228            = 0;
    // GpioCtrlRegs.GPHDIR.bit.GPIO228             = 0;

    // GpioCtrlRegs.GPHPUD.bit.GPIO230             = 1;
    // GpioCtrlRegs.GPHMUX1.bit.GPIO230            = 0;
    // GpioCtrlRegs.GPHDIR.bit.GPIO230             = 0;

    // GpioCtrlRegs.GPHPUD.bit.GPIO242             = 1;
    // GpioCtrlRegs.GPHMUX1.bit.GPIO242            = 0;
    // GpioCtrlRegs.GPHDIR.bit.GPIO242             = 0;

    // AIO
    GpioCtrlRegs.GPHAMSEL.bit.GPIO225           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO231           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO233           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO237           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO238           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO239           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO241           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO244           = 1;

    GpioCtrlRegs.GPHAMSEL.bit.GPIO245           = 1;

    EDIS;

}
