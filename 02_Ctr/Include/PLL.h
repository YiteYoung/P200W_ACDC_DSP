#ifndef _PLL_H
#define _PLL_H

#ifdef PLL_FUNCTION_G
#define PLL_FUNCTION extern
#else
#define PLL_FUNCTION
#endif

#define cMaxVitualSin   1000    // 40Hz半周期点数 = 1 / 40 * 40K / 2

typedef struct
{
    unsigned int    VoltPosNow      :1;         // 当前是否正半轴
    unsigned int    VoltPosPre      :1;         // 前一刻是否正半轴
    unsigned int    VoltPosWave     :1;         // 正半轴波形？

    unsigned int    VoltCrossN2P    :1;         // 电压上穿过零(负穿正)
    unsigned int    VoltCrossP2N    :1;         // 电压下穿过零(正穿负)
    unsigned int    VoltCrossWave   :1;         // 含正负过零

    unsigned int    u16PosWaveEn    :1;         // 
    unsigned int    u16PosWaveEnPre :1;         //

    unsigned int    Resv            :8;         //
}Wave_Flag_t;

typedef struct
{
    Wave_Flag_t     t_Flag;

    // int          i16InputVolt;
    unsigned int    u16ISRPoint_P;          // 正半轴计数--中断
    unsigned int    u16ISRPoint_N;          // 负半轴计数--中断

    unsigned int    u16ForceSwapPhaseCnt;   // 强切相位计数
    unsigned int    u16PolarWaveCnt;        // 极性稳定判断计数
    unsigned int    u16PolarNowCnt;         // 极性快速判断计数

    unsigned int    u16T_ISRPoint;          // 周期点数-滤波前
    UNLONG          un_AcTpointMean;        // 一个周期内中断点数的平均值
    int             i16PrdPoint;            // 一个周期内中断点数的平均值
}WaveCal_t;

typedef struct
{
    WaveCal_t       t_InvWave;
    WaveCal_t       t_GridWave;

    unsigned int    u16VbetaCnt;
      signed int    i16VbetaArray[cMaxVitualSin];

      signed int    i16Volt_Alfa;
      signed int    i16Volt_Beta;

      signed int    i16Sin;
      signed int    i16Cos;
      signed int    i16_2Sin;
      signed int    i16_2Cos;
      signed int    i16CosX;

      signed long   i32InvAngle;
      signed long   i32InvAngleBak;
      signed long   i32InvAngleStep;
      signed long   i32InvAngleBase;
      signed long   i32InvAnglePLL;

      signed long   i32TsPoint;
      signed long   i32TsPoint1Inv2;
      signed long   i32TsPoint1Inv4;
      signed long   i32TsPoint3Inv4;

            float   f32InvAnglePu;

      signed int    i16Volt_D;
      signed int    i16Volt_Q;
}PLL_t;

PLL_FUNCTION        void        sSetSinCos          (void);
PLL_FUNCTION        void        sHalfWavePointCal   (void);

#endif
