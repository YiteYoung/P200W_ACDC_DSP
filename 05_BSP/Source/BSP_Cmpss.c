#include "BSP_Cmpss.h"

const CMPCONFIG_t t_CMP_Para[] =
{
    // CH   DAC_Hi  DAC_Lo  HI_INV  Lo_INV  Hi_PWMXBar Lo_PWMXBar Hi_OutXBar LO_OutXBar CMPPIN_Hi   CMPPIN_Lo   CLK_Prescale SAMP_WIN   CMP_PHYS
    {CMP_CH_01,cDAC_MID_RANGE + cPFC_IND_OCP_DAC_10A,cDAC_MID_RANGE - cPFC_IND_OCP_DAC_10A,eREF_VDDA,eCMP_INV_DIS,eCMP_INV_EN,eCMP_OUT_SYN,eCMP_OUT_SYN,eCMP_OUT_ASYN,eCMP_OUT_ASYN,(unsigned int)A11_C0,(unsigned int)A11_C0,0x01,2,2,eCMP_PHYS_NONE},
    {CMP_CH_04,cDAC_MID_RANGE + cLLC_RES_OCP_DAC_10A,cDAC_MID_RANGE - cLLC_RES_OCP_DAC_10A,eREF_VDDA,eCMP_INV_DIS,eCMP_INV_EN,eCMP_OUT_LATCH,eCMP_OUT_LATCH,eCMP_OUT_ASYN,eCMP_OUT_ASYN,(unsigned int)A7_C3_CMPSS4,(unsigned int)A7_C3_CMPSS4,0x01,2,2,eCMP_PHYS_NONE}
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
