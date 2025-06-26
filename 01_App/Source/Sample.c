#include "Sample.h"
#include "UserMath.h"

static void     sClrSampleCnt       (ADC_Sample_e Goal);
unsigned int    sSample_RmsCal      (float *SampleSum,unsigned int *SampleCnt);
signed   int    sSample_AvgCal      (float *SampleSum,unsigned int *SampleCnt);

unsigned long   sSample_VACal       (unsigned int VoltRms,unsigned int CurrRms, signed long *VAPre);
unsigned long   sSample_WattCal     (float *SampleSum,unsigned int *SampleCnt,signed long *WattPre);
unsigned long   sSample_VARCal      (signed long LoadWatt,unsigned long LoadVA);

void            sSample_LoadFilter  (signed long Watt,signed long *Watt_F,unsigned long VA,unsigned long *VA_F,unsigned int Filter);
void            sSample_PercentCal  (signed long LoadWatt,unsigned long LoadWatt100,unsigned long LoadVA,unsigned long LoadVA100,unsigned int *LoadPercent);
void            sSample_PFCal       (signed long LoadWatt,unsigned long LoadVA,signed int *LoadPF);

static          Sample_t            t_Sample;
static          Load_t              t_Load;

void    sSample_Filter(void)
{
    ADC_Real_t  *pReal;

    pReal = sGetAdc_RealStr(BatVolt);
    sI16Filter(0,pReal->f32Real_F,t_Sample.un_BatVolt_F.dword);
    pReal->f32Real_F = (float)t_Sample.un_BatVolt_F.half.hword;

    sI16Filter(10,pReal->f32Real_F,t_Sample.un_BatVolt_AVG.dword);
    pReal->f32Real_AVG = (float)t_Sample.un_BatVolt_AVG.half.hword;

    pReal = sGetAdc_RealStr(BatCurr);
    sI16Filter(0,pReal->f32Real,t_Sample.un_BatCurr_F.dword);
    pReal->f32Real_F = (float)t_Sample.un_BatCurr_F.half.hword;

    sI16Filter(9,pReal->f32Real_F,t_Sample.un_BatCurr_AVG.dword);
    pReal->f32Real_AVG = (float)t_Sample.un_BatCurr_AVG.half.hword;

    pReal = sGetAdc_RealStr(BusVolt);
    sI16Filter(0,pReal->f32Real,t_Sample.un_BusVolt_F.dword);
    pReal->f32Real_F = (float)t_Sample.un_BusVolt_F.half.hword;

    sI16Filter(9,pReal->f32Real_F,t_Sample.un_BusVolt_AVG.dword);
    pReal->f32Real_AVG = (float)t_Sample.un_BusVolt_AVG.half.hword;

    pReal = sGetAdc_RealStr(ComVolt);
    sF32LPFilter(0.1992,pReal->f32Real,pReal->f32Real_F );

    pReal = sGetAdc_RealStr(OutCurr);
    sF32LPFilter(0.1992,pReal->f32Real,pReal->f32Real_F );

    pReal = sGetAdc_RealStr(GridVolt);
    sF32LPFilter(0.1992,pReal->f32Real,pReal->f32Real_F );

    pReal = sGetAdc_RealStr(IndCurr);
    sF32LPFilter(0.1992,pReal->f32Real,pReal->f32Real_F );
}

void    sSample_Accumulate(void)
{
    float           f32Real;
    Sample_Cal_t    *pSample;
    int             i;

    for(i = eAdc_Sample_AC_Start; i < eAdc_Sample_AC_End; i++)
    {
        pSample = &t_Sample.GridVolt + i;
        f32Real = sGetAdc_Real((ADC_Sample_e)i);
        pSample->f32Rms_Sum_B += (f32Real* f32Real * 0.01f);
        pSample->u16SampleCnt_B++;
    }

    for(i = eAdc_Sample_DC_Start; i < eAdc_Sample_DC_End; i++)
    {
        pSample = &t_Sample.GridVolt + i;
        f32Real = sGetAdc_Real((ADC_Sample_e)i);
        pSample->f32Avg_Sum += f32Real;
        pSample->u16SampleCnt_B++;
    }
}

void    sSample_ZeroDeal(unsigned int GridN2P,unsigned int GridP2N, unsigned int InvN2P, unsigned int InvP2N)
{
    // Sample_Cal_t    *pSample;
    // int             i;

    if( GridN2P == true)
    {
        t_Sample.GridVolt.f32Rms_Sum    = t_Sample.GridVolt.f32Rms_Sum_B;
        t_Sample.GridVolt.u16SampleCnt  = t_Sample.GridVolt.u16SampleCnt_B;
        sClrSampleCnt(GridVolt);

        t_Sample.GridCurr.f32Rms_Sum    = t_Sample.GridCurr.f32Rms_Sum_B;
        t_Sample.GridCurr.u16SampleCnt  = t_Sample.GridCurr.u16SampleCnt_B;
        sClrSampleCnt(GridCurr);

        t_Load.GridLoad.f32LoadSum       = t_Load.GridLoad.f32LoadSum_B;
        t_Load.GridLoad.u16LoadCnt       = t_Load.GridLoad.u16LoadCnt_B;
        t_Load.GridLoad.f32LoadSum_B     = 0;
        t_Load.GridLoad.u16LoadCnt_B     = 0;

        // OSEventSend(cPrioInv,eInvPfcZero);
    }

    if( GridP2N == true )
    {
        // OSEventSend(cPrioFun,eFunGridZero);
    }

    if( InvN2P == true )
    {
        EALLOW;
        EPwm5Regs.TZCLR.bit.CBCPULSE    = 1; // CBC恢复,0-ZERO,1-Prd,2-Zero_Prd,3-不恢复
        EDIS;

        t_Sample.ComVolt.f32Rms_Sum     = t_Sample.ComVolt.f32Rms_Sum_B;
        t_Sample.ComVolt.u16SampleCnt   = t_Sample.ComVolt.u16SampleCnt_B;
        sClrSampleCnt(ComVolt);

        t_Sample.OutVolt.f32Rms_Sum     = t_Sample.OutVolt.f32Rms_Sum_B;
        t_Sample.OutVolt.u16SampleCnt   = t_Sample.OutVolt.u16SampleCnt_B;
        sClrSampleCnt(OutVolt);

        t_Sample.IndCurr.f32Rms_Sum     = t_Sample.IndCurr.f32Rms_Sum_B;
        t_Sample.IndCurr.u16SampleCnt   = t_Sample.IndCurr.u16SampleCnt_B;
        sClrSampleCnt(IndCurr);

        t_Sample.OutCurr.f32Rms_Sum     = t_Sample.OutCurr.f32Rms_Sum_B;
        t_Sample.OutCurr.u16SampleCnt   = t_Sample.OutCurr.u16SampleCnt_B;
        sClrSampleCnt(OutCurr);

        t_Load.InvLoad.f32LoadSum       = t_Load.InvLoad.f32LoadSum_B;
        t_Load.InvLoad.u16LoadCnt       = t_Load.InvLoad.u16LoadCnt_B;
        t_Load.InvLoad.f32LoadSum_B     = 0;
        t_Load.InvLoad.u16LoadCnt_B     = 0;

        // OSEventSend(cPrioInv,eInvInvZero);
    }

    if( InvP2N == true )
    {
        EALLOW;
        EPwm5Regs.TZCLR.bit.CBCPULSE    = 0; // CBC恢复,0-ZERO,1-Prd,2-Zero_Prd,3-不恢复
        EDIS;
        // OSEventSend(cPrioFun,eFunInvZero);
    }
}

void    sSample_Cal_DC(void)
{
    t_Sample.BusVolt.i16Rms     = sSample_AvgCal(&t_Sample.BusVolt.f32Avg_Sum, &t_Sample.BusVolt.u16SampleCnt);

    t_Sample.BatVolt.i16Rms     = sSample_AvgCal(&t_Sample.BatVolt.f32Avg_Sum, &t_Sample.BatVolt.u16SampleCnt);

    t_Sample.BatCurr.i16Rms     = sSample_AvgCal(&t_Sample.BatCurr.f32Avg_Sum, &t_Sample.BatCurr.u16SampleCnt);

    t_Load.BatPower             = (signed int)(((float)t_Sample.BatVolt.i16Rms * (float)t_Sample.BatCurr.i16Rms) * 0.01f);
}

void    sSample_Cal_Inv(void)
{
    t_Sample.ComVolt.i16Rms     = sSample_RmsCal(&t_Sample.ComVolt.f32Rms_Sum, &t_Sample.ComVolt.u16SampleCnt);

    t_Sample.OutVolt.i16Rms     = sSample_RmsCal(&t_Sample.OutVolt.f32Rms_Sum, &t_Sample.OutVolt.u16SampleCnt);

    t_Sample.OutCurr.i16Rms     = sSample_RmsCal(&t_Sample.OutCurr.f32Rms_Sum, &t_Sample.OutCurr.u16SampleCnt);

    t_Sample.IndCurr.i16Rms     = sSample_RmsCal(&t_Sample.IndCurr.f32Rms_Sum, &t_Sample.IndCurr.u16SampleCnt);
}

void    sSample_Cal_Grid(void)
{
    t_Sample.GridVolt.i16Rms     = sSample_RmsCal(&t_Sample.GridVolt.f32Rms_Sum, &t_Sample.GridVolt.u16SampleCnt);

    t_Sample.GridCurr.i16Rms     = sSample_RmsCal(&t_Sample.GridCurr.f32Rms_Sum, &t_Sample.GridCurr.u16SampleCnt);
}

void    sSample_Cal_InvLoad(void)
{
    // Watt
    t_Load.InvLoad.i32LoadWatt   = sSample_WattCal(&t_Load.InvLoad.f32LoadSum, &t_Load.InvLoad.u16LoadCnt, &t_Load.InvLoad.i32LoadWatt_Pre);

    // VA
    t_Load.InvLoad.u32LoadVA     = sSample_VACal(t_Sample.ComVolt.i16Rms, t_Sample.OutCurr.i16Rms, &t_Load.InvLoad.i32LoadWatt_Pre);

    // Filter
    sSample_LoadFilter(t_Load.InvLoad.i32LoadWatt, &t_Load.InvLoad.i32LoadWatt_F, t_Load.InvLoad.u32LoadVA, &t_Load.InvLoad.u32LoadVA_F, 200);

    // VAR
    t_Load.InvLoad.u32LoadVar_F  = sSample_VARCal(t_Load.InvLoad.i32LoadWatt_F,t_Load.InvLoad.u32LoadVA_F);

    // PF
    sSample_PFCal(t_Load.InvLoad.i32LoadWatt_F, t_Load.InvLoad.u32LoadVA_F, &t_Load.InvLoad.i16PF);

    // Power Percent
    sSample_PercentCal(t_Load.InvLoad.i32LoadWatt_F,t_Load.InvLoad.u32Watt100,t_Load.InvLoad.u32LoadVA_F,t_Load.InvLoad.u32VA100,&t_Load.InvLoad.u16PowerPercent);
    
    // Curr Percent
    t_Load.InvLoad.u16CurrPercent = (unsigned int)labs(__divf32((float)t_Sample.OutCurr.i16Rms * 1000.0f, t_Load.InvLoad.u32Curr100));
}

void    sSample_Cal_GridLoad(void)
{
    // Watt
    t_Load.GridLoad.i32LoadWatt   = sSample_WattCal(&t_Load.GridLoad.f32LoadSum, &t_Load.GridLoad.u16LoadCnt, &t_Load.GridLoad.i32LoadWatt_Pre);

    // VA
    t_Load.GridLoad.u32LoadVA     = sSample_VACal(t_Sample.ComVolt.i16Rms, t_Sample.OutCurr.i16Rms, &t_Load.GridLoad.i32LoadWatt_Pre);

    // Filter
    sSample_LoadFilter(t_Load.GridLoad.i32LoadWatt, &t_Load.GridLoad.i32LoadWatt_F, t_Load.GridLoad.u32LoadVA, &t_Load.GridLoad.u32LoadVA_F, 2000);

    // VAR
    t_Load.GridLoad.u32LoadVar_F  = sSample_VARCal(t_Load.GridLoad.i32LoadWatt_F,t_Load.GridLoad.u32LoadVA_F);

    // PF
    sSample_PFCal(t_Load.GridLoad.i32LoadWatt_F, t_Load.GridLoad.u32LoadVA_F, &t_Load.GridLoad.i16PF);

    // Power Percent
    sSample_PercentCal(t_Load.GridLoad.i32LoadWatt_F,t_Load.GridLoad.u32Watt100,t_Load.GridLoad.u32LoadVA_F,t_Load.GridLoad.u32VA100,&t_Load.GridLoad.u16PowerPercent);
    
    // Curr Percent
    t_Load.GridLoad.u16CurrPercent = (unsigned int)labs(__divf32((float)t_Sample.OutCurr.i16Rms * 1000.0f, t_Load.GridLoad.u32Curr100));
}

static void sClrSampleCnt(ADC_Sample_e Goal)
{
    Sample_Cal_t    *pSample;
    pSample = &t_Sample.GridVolt + Goal;

    pSample->f32Rms_Sum_B      = 0;
    pSample->u16SampleCnt_B    = 0;
}

unsigned int sSample_RmsCal(float *SampleSum, unsigned int *SampleCnt)
{
    unsigned int    CalRms;
    float           CalSampleSum;
    unsigned int    CalSampleCnt;

    asm(" SETC INTM ");
    CalSampleSum    = *SampleSum;
    CalSampleCnt    = *SampleCnt;
    *SampleSum      = 0;
    *SampleCnt      = 0;
    asm(" CLRC INTM ");

    if( CalSampleCnt != 0)
    {
        CalSampleSum = __divf32(CalSampleSum,(float)CalSampleCnt);
        CalSampleSum = CalSampleSum * 100.0;
        CalRms       = (unsigned int)__sqrt(CalSampleSum);
        return CalRms;
    }
    else 
    {
        return 0;
    }
}

signed int sSample_AvgCal(float *SampleSum, unsigned int *SampleCnt)
{
    unsigned int    CalAvg;
    float           CalSampleSum;
    unsigned int    CalSampleCnt;

    asm(" SETC INTM ");
    CalSampleSum    = *SampleSum;
    CalSampleCnt    = *SampleCnt;
    *SampleSum      = 0;
    *SampleCnt      = 0;
    asm(" CLRC INTM ");

    if( CalSampleCnt != 0)
    {
        CalAvg = (signed int)__divf32(CalSampleSum,(float)CalSampleCnt);
        return CalAvg;
    }
    else 
    {
        return 0;
    }
}

unsigned long sSample_WattCal(float *SampleSum, unsigned int *SampleCnt, long *WattPre)
{
    signed long     CalLoadWatt_T;
    signed long     CalLoadWatt;
    float           CalSampleSum;
    unsigned int    CalSampleCnt;

    asm(" SETC INTM ");
    CalSampleSum    = *SampleSum;
    CalSampleCnt    = *SampleCnt;
    *SampleSum      = 0;
    *SampleCnt      = 0;
    asm(" CLRC INTM ");

    if(CalSampleCnt != 0)
    {
        CalLoadWatt_T   = (signed long)__divf32(CalSampleSum,(float) CalSampleCnt);
        CalLoadWatt     = (CalLoadWatt_T + (*WattPre)) >> 1;
        (*WattPre)      = CalLoadWatt_T;
        return CalLoadWatt;
    }
    else
    {
        (*WattPre)      = 0;
        return 0;
    }
}

unsigned long sSample_VACal(unsigned int VoltRms, unsigned int CurrRms, signed long *VAPre)
{
    float f32TempA,f32TempB;
    unsigned long LoadVA;

    f32TempA = (float)VoltRms * CurrRms * 0.1f;
    f32TempB = (f32TempA + (float)(*VAPre)) * 0.5f;
    (*VAPre) = (unsigned long)f32TempA;
    LoadVA   = (unsigned long)f32TempB;

    return LoadVA;
}

unsigned long sSample_VARCal(long LoadWatt, unsigned long LoadVA)
{
    unsigned long u32TempA,u32TempB;

    u32TempA = (unsigned long)LoadVA * LoadVA;
    u32TempB = (unsigned long)LoadWatt * LoadWatt;

    if(u32TempA <= u32TempB)
    {
        return 0;
    }
    else
    {
        u32TempA = u32TempA - u32TempB;
        u32TempB = (unsigned long)__sqrt((float)u32TempA);
        return u32TempB;
    }
}

void    sSample_LoadFilter(long Watt, long *Watt_F, unsigned long VA, unsigned long *VA_F, unsigned int Filter)
{
    if(labs(Watt - (*Watt_F)) > (signed long)Filter)
    {
        (*Watt_F) = Watt;
    }
    else 
    {
        (*Watt_F) = ((*Watt_F) + Watt) >> 1;
    }

    if( labs((long)(*VA_F) - (long)VA) > (long)Filter)
    {
        (*VA_F) = VA;
    }
    else 
    {
        (*VA_F) = ((*VA_F) + VA) >> 1;
    }

    if ( labs(*Watt_F) > (*VA_F) )
    {
        if( (*Watt_F) >= 0)
        {
            (*Watt_F) = (*VA_F);
        }
        else
        {
            (*Watt_F) = 0 - (*VA_F);
        }
    }
}

void    sSample_PercentCal(long LoadWatt, unsigned long LoadWatt100, unsigned long LoadVA, unsigned long LoadVA100,unsigned int *LoadPercent)
{
    unsigned int    CalVAPercent;
    unsigned int    CalWattPercent;
    unsigned long   u32TempA;

    CalVAPercent    = 0;
    CalWattPercent  = 0;

    if(LoadWatt100 == 0)
    {
        CalWattPercent = 0;
    }
    else 
    {
        u32TempA = labs(LoadWatt) * 100;
        CalWattPercent = (unsigned long)__divf32((float)u32TempA,(float)LoadWatt100);
    }

    if(LoadVA100 == 0)
    {
        CalVAPercent = 0;
    }
    else 
    {
        u32TempA = LoadVA * 100;
        CalWattPercent = (unsigned long)__divf32((float)u32TempA,(float)LoadVA100);
    }

    if( CalWattPercent >= CalVAPercent)
    {
        (*LoadPercent) = CalWattPercent;
    }
    else 
    {
        (*LoadPercent) = CalVAPercent;
    }
}

void    sSample_PFCal(long LoadWatt, unsigned long LoadVA, int *LoadPF)
{
    unsigned int    CalPF;
    unsigned long   u32TempA;

    if(LoadVA == 0)
    {
        CalPF = 0;
    }
    else 
    {
        u32TempA = labs(LoadWatt) * 100;
        CalPF = (unsigned int)__divf32((float)u32TempA,(float)LoadVA);
    }

    if(CalPF > 100)
    {
        CalPF = 100;
    }

    if(LoadWatt >= 0)
    {
        (*LoadPF) = CalPF;
    }
    else 
    {
        (*LoadPF) = 0 - CalPF;
    }

    if( abs((*LoadPF)) > 100 )
    {
        if( (*LoadPF) > 0)
        {
            (*LoadPF) = 100;
        }
        else
        {
            (*LoadPF) = -100;
        }
    }
}

void    sSample_Cal_Freq(unsigned int PrdPoint,unsigned int SwitchFreq,signed int Volt,unsigned int *pFreq)
{
    if( Volt < 20 )
    {
        *pFreq = 0;
        return;
    }

    if( PrdPoint < 100)
    {
        *pFreq = 0;
    }
    else 
    {
        *pFreq = ((long)SwitchFreq * 100) / PrdPoint;
    }
}

signed int  sSample_Get_Rms(ADC_Sample_e Goal)
{
    Sample_Cal_t *pSample;
    if( Goal >= eAdc_Sample_End ) 
    {
        return 0;
    }

    pSample = &t_Sample.GridVolt + Goal;

    return pSample->i16Rms;
}


