#include "BSP_Ram.h"

void    sInitRam(void)
{
    unsigned char RamErr;

    RamErr = false;

    EALLOW;

    // M0/M11(stack(M1) can not be inital)
    MemCfgRegs.DxINIT.all       = 0x00000002;

    // LS0~LS7
    MemCfgRegs.LSxINIT.all      = 0x000000003;

    // Check inital M0/M1
    if( MemCfgRegs.DxINITDONE.all & 0x00000002 != 0x00000002)
    {
        RamErr = true;
    }

    // Check inital LS0 ~ LS1
    if( MemCfgRegs.LSxINITDONE.all & 0x00000003 != 0x00000003)
    {
        RamErr = true;
    }

    if(RamErr == true)
    {
        // do
    }
}

