#include "UserHeader.h"
// #include "PLL.h"

static  PLL_t   t_PLL;

#define cSetSinThetaQ14     (int)(__sinpuf32(__divf32(135.0,360.0)) * 16384.0)
#define cSetCosThetaQ14     (int)(__cospuf32(__divf32(135.0,360.0)) * 16384.0)

void    sSetSinCos(void)
{
    float   f32TempA,f32TempB;
    long    i32TempA,i32TempB,i32TempC;
    long    i32TempSin,i32TempCos;
    int     i16TempA;
    int     i16SaveCnt;
    int     volt_alfa,volt_beta;

    t_PLL.u16VbetaCnt++;
    UpDnLimit(t_PLL.u16VbetaCnt, 0, cMaxVitualSin - 1);

    // 电网过零清零计数
    if( t_PLL.t_GridWave.t_Flag.VoltCrossP2N == true)
    {
        t_PLL.u16VbetaCnt = 0;
    }

    // 延时90°保存Cos作为Sin(满足SOGI中滞后90°的电压)
    i16TempA = (int)(t_PLL.t_GridWave.i16PrdPoint >> 2);
    i16SaveCnt = t_PLL.u16VbetaCnt - i16TempA;

    if(i16SaveCnt < 0)
    {
        i16SaveCnt += t_PLL.t_GridWave.i16PrdPoint;
    }
    else if(i16SaveCnt >= t_PLL.t_GridWave.i16PrdPoint) 
    {
        i16SaveCnt -= t_PLL.t_GridWave.i16PrdPoint;        
    }
    UpDnLimit(i16SaveCnt, 0, cMaxVitualSin - 1);

    // 获取虚拟beta
    t_PLL.i16Volt_Alfa = sSample_GetRms(ComVolt);
    t_PLL.i16Volt_Beta = t_PLL.i16VbetaArray[i16SaveCnt];

    // 计算Sin/Cos及过零
    if( sMsw_GetPfcMode() == true)
    {
        // 计算mod
        i32TempA = t_PLL.i16Volt_Alfa;
        i32TempB = t_PLL.i16Volt_Beta;
        i32TempC = (long)i32TempA * i32TempA + (long)i32TempB * i32TempB;
        i32TempC = (long)__sqrt((float)i32TempC);

        // 计算电压相位对应的Sin&Cos, i16Sin = β/C，i16Cos = α/C
        i32TempA = (long)(i32TempA << 14);
        i32TempB = (long)(i32TempB << 14);

        i32TempSin = (int)__divf32( (float)i32TempB, (float)i32TempC );
        i32TempCos = (int)__divf32( (float)i32TempA, (float)i32TempC );

        // 优化iTHD 电流超前2.5° sin(2.5°) = 0.043619 Cos(2.5°) = 0.999048 随后进行和差化积
        // Sin(a + b) = sin(a) * cos(b) + cos(a) * sin(b)
        // Cos(a + b) = cos(a) * cos(b) - sin(a) * sin(b)
        t_PLL.i16Sin = (int)((float)i32TempSin * 0.999048f + (float)i32TempCos * 0.043619);
        t_PLL.i16Cos = (int)((float)i32TempCos * 0.999048f - (float)i32TempSin * 0.043619);

        // 计算二倍角
        // Sin(2α) = 2*Sin(α)*Cos(α)
        // Cos(2α) = Cos(α)^2 - Sin(α)^2
        t_PLL.i16_2Sin = (int)(((long)t_PLL.i16Cos * t_PLL.i16Sin) >> 13);  // >> 13 = ((>> 14) << 1)
        t_PLL.i16_2Cos = (int)(((long)t_PLL.i16Cos * t_PLL.i16Cos - (long)t_PLL.i16Sin * t_PLL.i16Sin) >> 14);

        // 计算CosX,X = (2α - 135°)
        // Cos(X) = cos(2a) * cos(135) + sin(2a) * sin(135)
        t_PLL.i16CosX = (int)(((long)t_PLL.i16_2Cos * cSetCosThetaQ14 + (long)t_PLL.i16_2Sin * cSetSinThetaQ14) >> 14);

        t_PLL.t_InvWave.t_Flag.VoltCrossN2P = t_PLL.t_GridWave.t_Flag.VoltCrossN2P;
        t_PLL.t_InvWave.t_Flag.VoltCrossP2N = t_PLL.t_GridWave.t_Flag.VoltCrossP2N;

        if(t_PLL.t_GridWave.t_Flag.VoltCrossN2P == true)
        {
            t_PLL.t_GridWave.t_Flag.u16PosWaveEn = true;
        }
        else if (t_PLL.t_GridWave.t_Flag.VoltCrossP2N) 
        {
            t_PLL.t_GridWave.t_Flag.u16PosWaveEn = false;
        }
    }
    else 
    {
        t_PLL.i32InvAngleBak = t_PLL.i32InvAngle;
        t_PLL.i32InvAngleStep = t_PLL.i32InvAngleBase + t_PLL.i32InvAnglePLL;
        t_PLL.i32InvAngle += t_PLL.i32InvAngleStep;

        if( t_PLL.i32InvAngle > t_PLL.i32TsPoint )
        {
            t_PLL.i32InvAngle -= t_PLL.i32TsPoint;
        }
        if (t_PLL.i32InvAngle < 0 ) 
        {
            t_PLL.i32InvAngle += t_PLL.i32TsPoint;
        } 
        
        t_PLL.f32InvAnglePu = __divf32((float)t_PLL.i32InvAngle, (float)t_PLL.i32TsPoint);
        f32TempA = __sinpuf32( t_PLL.f32InvAnglePu );
        f32TempB = __cospuf32( t_PLL.f32InvAnglePu );

        t_PLL.i16Sin = (int)(f32TempA * 16384);
        t_PLL.i16Cos = (int)(f32TempB * 16384);

        t_PLL.i32TsPoint1Inv2 = t_PLL.i32TsPoint >> 1;
        t_PLL.i32TsPoint1Inv4 = t_PLL.i32TsPoint >> 2;
        t_PLL.i32TsPoint3Inv4 = t_PLL.i32TsPoint - t_PLL.i32TsPoint1Inv4;

        t_PLL.t_InvWave.t_Flag.VoltCrossN2P = false;
        if( (t_PLL.i32InvAngle >= t_PLL.i32TsPoint3Inv4) && (t_PLL.i32InvAngleBak < t_PLL.i32TsPoint3Inv4) )
        {
            t_PLL.t_InvWave.t_Flag.u16PosWaveEn = true;
            t_PLL.t_InvWave.t_Flag.VoltCrossN2P = true;
            t_PLL.t_InvWave.t_Flag.VoltPosNow   = true;
        }

        t_PLL.t_InvWave.t_Flag.VoltCrossP2N = false;
                if( (t_PLL.i32InvAngle >= t_PLL.i32TsPoint3Inv4) && (t_PLL.i32InvAngleBak < t_PLL.i32TsPoint3Inv4) )
        {
            t_PLL.t_InvWave.t_Flag.u16PosWaveEn = false;
            t_PLL.t_InvWave.t_Flag.VoltCrossN2P = true;
            t_PLL.t_InvWave.t_Flag.VoltPosNow   = false;
        }

        if( t_PLL.t_InvWave.t_Flag.u16PosWaveEn != t_PLL.t_InvWave.t_Flag.u16PosWaveEnPre )
        {
            t_PLL.t_InvWave.t_Flag.VoltCrossWave = true;
        }
        t_PLL.t_InvWave.t_Flag.u16PosWaveEnPre = t_PLL.t_InvWave.t_Flag.u16PosWaveEn;

        // α = （2*A - B - C)/3
        // β = (B - C)/sqrt(3)
        // D = α * Cos + β * Sin
        // Q = β * Cos - α * Sin
        // Sin = β / sqrt(α^2 + β^2)
        // Cos = α / sqrt(α^2 + β^2)
        i32TempA = sAdc_GetReal(ComVolt);
        sSOGI_Cal(i32TempA);

        volt_alfa = i32TempA;
        volt_beta = sSOGI_GetBeta() * 1.01f;

        // Park变换
        t_PLL.i16Volt_D = (int)(((long)volt_alfa * t_PLL.i16Cos + (long)volt_beta * t_PLL.i16Sin) >> 14);
        t_PLL.i16Volt_Q = (int)(((long)volt_beta * t_PLL.i16Cos - (long)volt_alfa * t_PLL.i16Sin) >> 14);
    }
}

void    sHalfWavePointCal(void)
{
    signed int  i16InputVolt;
    
    t_PLL.t_GridWave.u16ISRPoint_P++;
    t_PLL.t_GridWave.u16ISRPoint_N++;
    UpLimit(t_PLL.t_GridWave.u16ISRPoint_P, 1000);
    UpLimit(t_PLL.t_GridWave.u16ISRPoint_N, 1000);

    i16InputVolt = sAdc_GetReal(GridVolt);
    
    if( t_PLL.t_GridWave.t_Flag.VoltPosWave == true )
    {
        if( i16InputVolt <= -cVac20V )
        {
            t_PLL.t_GridWave.u16PolarWaveCnt++;
        }
        else 
        {
            t_PLL.t_GridWave.u16PolarWaveCnt = 0;
        }

        if(t_PLL.t_GridWave.u16PolarWaveCnt >= 10)
        {
            t_PLL.t_GridWave.u16PolarWaveCnt        = 0;
            t_PLL.t_GridWave.t_Flag.VoltPosWave     = false;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt   = 0;
        }
    }
    else 
    {
        if( i16InputVolt >= cVac20V)
        {
            t_PLL.t_GridWave.u16PolarWaveCnt++;
        }
        else 
        {
            t_PLL.t_GridWave.u16PolarWaveCnt = 0;
        }

        if(t_PLL.t_GridWave.u16PolarWaveCnt >= 10)
        {
            t_PLL.t_GridWave.u16PolarWaveCnt        = 0;
            t_PLL.t_GridWave.t_Flag.VoltPosWave     = true;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt   = 0;
        }
    }

    if( t_PLL.t_GridWave.t_Flag.VoltPosNow == true)
    {
        if( t_PLL.t_GridWave.t_Flag.VoltPosWave == false)
        {
            t_PLL.t_GridWave.u16PolarNowCnt++;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt = 0;
        }
        else 
        {
            t_PLL.t_GridWave.u16PolarNowCnt = 0;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt++;
            UpLimit(t_PLL.t_GridWave.u16ForceSwapPhaseCnt, cISRCnt5ms);
        }

        if(  (t_PLL.t_GridWave.t_Flag.VoltPosWave == true && i16InputVolt <= cVac2V ) || t_PLL.t_GridWave.u16PolarNowCnt >= cISRCnt2ms )
        {
            t_PLL.t_GridWave.t_Flag.VoltPosNow      = false;
            t_PLL.t_GridWave.u16PolarNowCnt         = 0;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt   = 0;
        }
    }
    else 
    {
        if(t_PLL.t_GridWave.t_Flag.VoltPosWave == true)
        {
            t_PLL.t_GridWave.u16PolarNowCnt++;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt = 0;
        }
        else 
        {
            t_PLL.t_GridWave.u16PolarWaveCnt = 0;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt++;
            UpLimit(t_PLL.t_GridWave.u16ForceSwapPhaseCnt, cISRCnt5ms);
        }

        if( (t_PLL.t_GridWave.t_Flag.VoltPosWave == false && i16InputVolt >= -cVac2V) || t_PLL.t_GridWave.u16PolarNowCnt >= cISRCnt2ms)
        {
            t_PLL.t_GridWave.t_Flag.VoltPosNow      = true;
            t_PLL.t_GridWave.u16PolarNowCnt         = 0;
            t_PLL.t_GridWave.u16ForceSwapPhaseCnt   = 0;
        }
    }

    t_PLL.t_GridWave.t_Flag.VoltCrossN2P = false;
    if( t_PLL.t_GridWave.t_Flag.VoltPosNow == true && t_PLL.t_GridWave.t_Flag.VoltPosPre == false && t_PLL.t_GridWave.t_Flag.VoltPosWave == false && t_PLL.t_GridWave.u16ISRPoint_P >= cMaxHzPointFswCnst ||\
        t_PLL.t_GridWave.u16ISRPoint_P >= cMinHzPointFswCnst )
    {
        t_PLL.t_GridWave.u16ISRPoint_P       = 0;
        t_PLL.t_GridWave.t_Flag.VoltCrossN2P = true;
    }

    t_PLL.t_GridWave.t_Flag.VoltCrossP2N = false;
    if( t_PLL.t_GridWave.t_Flag.VoltPosNow == false && t_PLL.t_GridWave.t_Flag.VoltPosPre == true && t_PLL.t_GridWave.t_Flag.VoltPosWave == true && t_PLL.t_GridWave.u16ISRPoint_N >= cMaxHzPointFswCnst ||\
        t_PLL.t_GridWave.u16ISRPoint_N >= cMinHzPointFswCnst )
    {
        t_PLL.t_GridWave.u16T_ISRPoint       = t_PLL.t_GridWave.u16ISRPoint_N;
        t_PLL.t_GridWave.u16ISRPoint_N       = 0;
        t_PLL.t_GridWave.t_Flag.VoltCrossP2N = true;
    }

    t_PLL.t_GridWave.t_Flag.VoltPosPre = t_PLL.t_GridWave.t_Flag.VoltPosNow;

    sI16Filter(8, t_PLL.t_GridWave.u16T_ISRPoint, t_PLL.t_GridWave.un_AcTpointMean.dword);
    t_PLL.t_GridWave.i16PrdPoint             = t_PLL.t_GridWave.un_AcTpointMean.half.hword;

    if( t_PLL.t_GridWave.i16PrdPoint < 10 )
    {
        t_PLL.t_GridWave.i16PrdPoint = 10;
    }

    // t_Inv.t_PfcCtrl.t_BandStop.f32RadResnant = __divf32((float)sSOGI_GetCalFs() * 12.5663848f,(float)t_PLL.t_GridWave.i16PrdPoint);
}

int sPLL_GetGridPos(void)
{
    return t_PLL.t_GridWave.t_Flag.VoltPosNow;
}

int sPLL_GetGridPrdPoint(void)
{
    return t_PLL.t_GridWave.i16PrdPoint;
}

int sPLL_GetInvPrdPoint(void)
{
    return (int)(t_PLL.i32TsPoint / t_PLL.i32InvAngleStep);
}

