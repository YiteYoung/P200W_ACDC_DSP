#include "BSP_Cmpss.h"

const CMPCONFIG_t t_CMP_Para[] =
{
    // CH   DAC_Hi  DAC_Lo  HI_INV  Lo_INV  Hi_PWMXBar Lo_PWMXBar Hi_OutXBar LO_OutXBar CMPPIN_Hi   CMPPIN_Lo   CLK_Prescale SAMP_WIN   CMP_PHYS
    {CMP_CH_01,}
};

void sInitCmpss(void)
{
    unsigned int i,CH;

    CH = sARRAY_SIZE( t_CMP_Para);

    for (i = 0; i < CH; i++) 
    {
        sDrv_InitCmpss(t_CMP_Para[i]);
    }
}
