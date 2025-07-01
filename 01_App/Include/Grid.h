#ifndef _GRID_H
#define _GRID_H

#ifdef GRID_FUNCTION_G
#define GRID_FUNCTION extern
#else
#define GRID_FUNCTION
#endif

typedef struct
{
    unsigned int    InWorkNow       :1;     // 当前市电正常标志位
    unsigned int    InWorkPre       :1;     // 上一时刻市电正常标志位
    unsigned int    VoltLoss        :1;     // 电压有效值异常
    unsigned int    FreqLoss        :1;     // 频率异常
    unsigned int    Waveloss        :1;     // 波形异常

    unsigned int    WaveAdcHiLoss   :1;     // 采样高压异常
    unsigned int    WaveAdcLoLoss   :1;     // 采样低压异常
    unsigned int    WavePosLoss     :1;     // 极性异常
    unsigned int    WaveZeroLoss    :1;     // 电压过零丢失

    unsigned int    VoltLossAct     :1;     // 电压异常(外部处理使用)
    unsigned int    VoltRestAct     :1;     // 电压恢复(外部处理使用)

    unsigned int    VoltPosNow      :1;     // 电压当前极性
    unsigned int    VoltPosPre      :1;     // 电压前一刻极性

    unsigned int    Resv            :3;     
}GridPortFlag_t;

typedef struct
{
    GridPortFlag_t  t_Flag;

    unsigned int    u16FreqReal;            // 市电频率

      signed int    i16RmsHiLossValue;      // 市电电压异常上限值
      signed int    i16RmsLoLossValue;      // 市电电压异常下限值

      signed int    i16RmsHiRestValue;      // 市电电压恢复上限值
      signed int    i16RmsLoRestValue;      // 市电电压恢复下限值

      signed int    i16FreqHiLossValue;     // 市电频率异常上限值
      signed int    i16FreqLoLossValue;     // 市电频率异常下限值

      signed int    i16FreqHiRestValue;     // 市电频率恢复上限值
      signed int    i16FreqLoRestValue;     // 市电频率恢复下限值

      signed int    i16WaveHiLossValue;     // 市电波形丢失上限值
      signed int    i16WaveLoLossValue;     // 市电波形丢失下限值

      signed int    i16WaveHiRestValue;     // 市电波形恢复上限值
      signed int    i16WaveLoRestValue;     // 市电波形恢复下限值

    unsigned int    u16HallLossCnt;         // 电网侧霍尔采样计数 -- 非实际产品不需要该计数和保护

    unsigned int    u16VoltLossCnt;         // 
    unsigned int    u16FreqLossCnt;         //
    unsigned long   u32WaveLossCnt;         //
    unsigned long   u32WaveAdcHiLossCnt;    //
    unsigned long   u32WaveAdcLoLossCnt;    //
    unsigned long   u32WavePosLossCnt;      //
    unsigned int    u16WaveZeroLostCnt;     //
    unsigned int    u16WaveZeroRestCnt;     //
    unsigned int    u16VoltLossDlyCnt;      //
    unsigned int    u16VoltRestDlyCnt;      //    
}GridProt_t;

GRID_FUNCTION   void            sGrid_StatusDeal        (void);
GRID_FUNCTION   void            sGrid_RmsCheck          (void);
GRID_FUNCTION   void            sGrid_FreqCheck         (void);
GRID_FUNCTION   void            sGrid_ZeroRestDeal      (void);
GRID_FUNCTION   void            sGrid_ZeroLossDeal      (void);
GRID_FUNCTION   void            sGrid_WaveLossDetect    (void);

GRID_FUNCTION   void            sGrid_FreqCal           (unsigned int Freq);
GRID_FUNCTION   signed int      sGrid_GetFreq           (void);

#endif
