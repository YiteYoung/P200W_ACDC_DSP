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

#endif
