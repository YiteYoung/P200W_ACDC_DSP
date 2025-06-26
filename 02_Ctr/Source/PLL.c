#include "UserHeader.h"
#include "PLL.h"

static  PLL_t   t_PLL;

#define cSetSinThetaQ14     (int)(__sinpuf32(__divf32(135.0,360.0)) * 16384.0)
#define cSetCosThetaQ14     (int)(__cospuf32(__divf32(135.0,360.0)) * 16384.0)

void    sSetSinCos(void)
{
    // float   f32TempA,f32TempB;
    long    i32TempA,i32TempB,i32TempC;
    long    i32TempSin,i32TempCos;
    int     i16TempA;
    int     i16SaveCnt;
    // int     volt_alfa,volt_beta;

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
    t_PLL.i16Volt_Alfa = sSample_Get_Rms(ComVolt);
    t_PLL.i16Volt_Beta = t_PLL.i16VbetaArray[i16SaveCnt];

    // 计算Sin/Cos及过零
    if(sGetPfcMode_EN() == true)
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
    
    }

}

