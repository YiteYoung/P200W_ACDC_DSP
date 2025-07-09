#ifndef _CTR_INV_H
#define _CTR_INV_H

#ifdef CTR_INV_FUNCTION_G
#define CTR_INV_FUNCTION extern
#else
#define CTR_INV_FUNCTION
#endif

#define cSwitchFreq         24000
#define cINV_TBPRD          2500
#define cINV_CMPA_MIN       60
#define cINV_CMPA_MAX       (cINV_TBPRD - cINV_CMPA_MIN)
#define cINV_CMPA_DBT_MIN   38

#define cInvAngleBase50Hz   0x300000    // （360 << 22） * 50 / 24K
#define cInvAngleBase60Hz   0x39999A

#ifndef UNION_TYPE
#define UNION_TYPE
typedef union 
{
    signed long dword;
    struct
    {
        unsigned int lword;
          signed int hword;
    }half;
}UNLONG;
#endif

typedef enum
{
    eBat_CC_Mode = 0,
    eBat_CV_Mode,
    eBat_Mode_End
}BatMode_e;

typedef enum
{
    eINV_FSM_OFF = 0,
    eINV_FSM_INIT,
    eINV_FSM_RUN,
    eINV_FSM_End
}InvFsm_e;

typedef struct
{
    int     i16Kp;
    int     i16Ki;

    int     i16Ref;
    int     i16Fed;
    int     i16Err;
    long    i32UpSum;
    long    i32UiSum;
    
    int     i16PIOut;
    int     i16PIOut_Pre;
    int     i16PIOutMax;
    int     i16PIOutMin;
}PI_t;

typedef struct
{
    PI_t    BusVolt;
    PI_t    PfcCurr;

    PI_t    GridPower;
    PI_t    Bat_CC;
    PI_t    Bat_CV;
}Pfc_PI_t;

typedef struct
{
    PI_t    Volt_ID;
    PI_t    Volt_IQ;
    PI_t    IndCurr;    
    PI_t    InvLimit;
}Inv_PI_t;

typedef struct
{
    unsigned int    Inv_En          :1;     // 逆变使能
    unsigned int    OpenLoop        :1;     // 开环使能

    unsigned int    TZ1_OST_Act     :1;     // TZ1 OST使能
    unsigned int    TZ2_OST_Act     :1;     // TZ2 OST使能、
    unsigned int    TZ3_CBC_Act     :1;     // TZ3 CBC使能
    unsigned int    Trip4_CBC_Act   :1;     // Trip CBC使能

    unsigned int    InvLimit_En     :1;     // 功率限制使能
    unsigned int    InvLimit_Act    :1;     // 功率现在使能
    unsigned int    Trip4Limit_Act  :1;     // 

    unsigned int    InvFsm          :2;     // 控制状态机
    unsigned int    Resv            :5;
}Control_Flag_t;

typedef struct
{
        Pfc_PI_t    t_PI;

      signed int    i16PfcVoltSet;
      signed int    i16PfcVoltRef;

      signed int    i16ComVoltPre;

    unsigned int    u16Burst_EN;
    unsigned int    u16Burst_Act;
    unsigned int    u16Burst_Cnt;

      signed int    i16Burst_ValueHi;
      signed int    i16Burst_VauleLo;

    unsigned int    BatCCCVState;
    unsigned int    u16BatCCCVSWCnt;
      signed int    i16BatCCCVPIOut;
}Pfc_t;

typedef struct
{
        Inv_PI_t    t_PI;

      signed int    i16InvVoltSet;
      signed int    i16InvVoltRef;

      signed int    i16InvPhaseSet;
      signed int    i16InvPhaseRef;

      signed int    i16WC_Q14;
      signed int    i16VoltLoopOutRef;

    unsigned int    u16VoltPos_Cnt;

      signed int    i16InvVoltDerate;
      signed int    i16InvVoltDerateDelta;
}Inv_t;

typedef struct
{
    unsigned int    u16Spwm_TBPRD;

      signed int    i16Spwm_Vbus;
      signed int    i16Spwm_Ref;
      signed int    i16Spwm_Set;

      signed int    i16Spwm_DEAD;

      signed int    i16Spwm_H_CMP;
      signed int    i16Spwm_L_CMP;   

    unsigned int    u16IGBT_EN_DLY;

    unsigned int    bLF_CBC_EN;         // 工频管CBC封波使能 & 过零封波1ms使能
    unsigned int    u16LFIGBT_DelayCnt; // 工频管封波计数
    unsigned int    bHF_CBC_EN;         // 高频管CBC封波使能
    unsigned int    u16HFIGBT_DelayCnt; // 高频管封波计数

           float    f32LLCTrancRaio;
}Var_t;

typedef struct
{
    Control_Flag_t  t_Flag;
    Inv_t           t_Inv;
    Pfc_t           t_Pfc;
    Var_t           t_Var;
}InvControl_t;

CTR_INV_FUNCTION  void        sInv_Control        (void);

CTR_INV_FUNCTION  Inv_t*      sInv_GetInvCtrPtr   (void);
CTR_INV_FUNCTION  Pfc_t*      sInv_GetPfcCtrPtr   (void);

CTR_INV_FUNCTION  void        sInv_SetInvEN       (unsigned int EN);
CTR_INV_FUNCTION  void        sInv_SetInvOpenLoop (unsigned int EN);

CTR_INV_FUNCTION  void        sInv_SetInvRef      (unsigned int Value);
CTR_INV_FUNCTION  void        sInv_SetPfcRef      (unsigned int Value);

#endif
