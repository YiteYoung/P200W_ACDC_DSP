#ifndef _USERMATH_H
#define _USERMATH_H

#ifdef USERMATH_FUNCTION_G
#define USERMATH_FUNCTION extern
#else
#define USERMATH_FUNCTION
#endif

#ifndef MAX_MIN_LIMIT
#define MAX_MIN_LIMIT
#define Max2(x,y)                           ( (x) > (y) ? (x) : (y) )
#define Min2(x,y)                           ( (x) < (y) ? (x) : (y) )
#define Max3(x,y,z)                         ( Max2( (x),(y) ) > (z) ? Max2( (x),(y) ) : (z))
#define Min3(x,y,z)                         ( Min2( (x),(y) ) < (z) ? Min2( (x),(y) ) : (z))
#define UpLimit(x,max)                      { x =  (x) >= (max)  ? (max) : (x); } 
#define DnLimit(x,min)                      { x =  (x) <= (min)  ? (min) : (x); }
#define UpDnLimit(x,min,max)                { UpLimit((x),(max)); DnLimit((x),(min)); }
#endif

#ifndef FILTER_DEF
#define FILTER_DEF
#define sI16Filter(i16K,i16Var,i32Mean)     { (i32Mean) = (((((long)(i16Var) << 16) - (i32Mean)) >> ((i16K) + 1)) + (i32Mean)); }
#define sI32Filter(i16K,i16Var,i32Mean)     { (i32Mean) = (((((long)(i16Var) << 9 ) - (i32Mean)) >> ((i16K) + 1)) + (i32Mean)); }
#define sLPFilter(i16K,i16X,i16XF)          { (i16XF) = (int)(((long)(i16XF) * (i16K) + (long)(i16X) * (1024 - (i16K))) >> 10);}
#define sF32LPFilter(K,in,out)              { (out) = (float)(out) * (float)(K) + (float)(in) * (float)(1.0f - (float)(K));}
#endif

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

typedef struct
{
      signed int    i16WaveIn;
      signed int    i16PLLFs;
    unsigned long   u32CalFs;

      signed int    i16Kp;

            float   f32SOGI_In;
            float   f32SOGI_InPre;
            float   f32SOGI_In2nd;

            float   f32SOGI_Alfa;
            float   f32SOGI_AlfaPre;
            float   f32SOGI_Alfa2nd;

            float   f32SOGI_Beta;
            float   f32SOGI_BetaPre;
            float   f32SOGI_Beta2nd;

            float   f32SOGI_A1;
            float   f32SOGI_A2;
            float   f32SOGI_B0;
            float   f32SOGI_C0; 
}SOGI_t;

typedef struct
{
    float       f32Kr;
    float       f32RadWi;
    float       f32RadResnant;

    float       Q0;
    float       Q1;
    float       R0;
    float       R1;
    float       P0;
    float       P1;
    
    float       Out0;
    float       Out1;
}BandStop_t;

USERMATH_FUNCTION   void            sSOGI_Init                (int PLL_Freq,unsigned long Cal_Freq,int Kp);
USERMATH_FUNCTION   void            sSOGI_Cal                 (float WaveIn);
USERMATH_FUNCTION   void            sSOGI_Resh                (void);
USERMATH_FUNCTION   float           sSOGI_GetAlfa             (void);
USERMATH_FUNCTION   float           sSOGI_GetBeta             (void);
USERMATH_FUNCTION   unsigned long   sSOGI_GetCalFs            (void);

USERMATH_FUNCTION   void            sBandStop_Init            (float Kr,float RadWi,float RadResnant);
USERMATH_FUNCTION   void            sBandStop_SetRadResnant   (float RadResnant);
USERMATH_FUNCTION   float           sBandStop_Refresh         (float Volt);

#endif
