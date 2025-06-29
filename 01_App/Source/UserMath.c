#include "UserMath.h"

static SOGI_t   t_SOGI;

void    sSOGI_Init()
{

}

void    sSOGI_Cal(float WaveIn)
{
    t_SOGI.f32SOGI_In2nd    = t_SOGI.f32SOGI_InPre;
    t_SOGI.f32SOGI_InPre    = t_SOGI.f32SOGI_In;
    t_SOGI.f32SOGI_In       = WaveIn;

    t_SOGI.f32SOGI_Alfa2nd  = t_SOGI.f32SOGI_AlfaPre;
    t_SOGI.f32SOGI_AlfaPre  = t_SOGI.f32SOGI_Alfa;

    t_SOGI.f32SOGI_Beta2nd  = t_SOGI.f32SOGI_BetaPre;
    t_SOGI.f32SOGI_BetaPre  = t_SOGI.f32SOGI_Beta;

    // α
    t_SOGI.f32SOGI_Alfa     = ( t_SOGI.f32SOGI_In - t_SOGI.f32SOGI_In2nd ) * t_SOGI.f32SOGI_B0 +\
                                                    t_SOGI.f32SOGI_AlfaPre * t_SOGI.f32SOGI_A1 +\
                                                    t_SOGI.f32SOGI_Alfa2nd * t_SOGI.f32SOGI_A2;

    // β
    t_SOGI.f32SOGI_Beta     = ( t_SOGI.f32SOGI_In + t_SOGI.f32SOGI_InPre * 2.0f + t_SOGI.f32SOGI_In2nd ) * t_SOGI.f32SOGI_C0 +\
                                                                                  t_SOGI.f32SOGI_BetaPre * t_SOGI.f32SOGI_A1 +\
                                                                                  t_SOGI.f32SOGI_Beta2nd * t_SOGI.f32SOGI_A2;
}

void    sSOGI_Resh()
{
    int     i16TempA;
    float   f32TempA;
    float   f32Den;
    float   f32TsW,f32TsW_2;

    if( t_SOGI.i16PLLFs == 0)
    {
        t_SOGI.i16PLLFs = 5000;
    }

    if(t_SOGI.u32CalFs < 10)
    {
        t_SOGI.u32CalFs = 10;
    }

    i16TempA = t_SOGI.i16PLLFs / 100;
    f32TempA = (float)i16TempA * 6.2831853f;
    f32TsW   = __divf32(f32TempA,(float)t_SOGI.u32CalFs);
    f32TsW_2 = f32TsW * f32TsW;
    f32Den   = 4.0f + (float)t_SOGI.i16Kp * 2.0f * f32TsW + f32TsW_2;

    // A1
    f32TempA = 8.0f - 2.0f * f32TsW_2;
    t_SOGI.f32SOGI_A1 = __divf32(f32TempA,f32Den);

    // A2
    f32TempA = -4.0f + (float)t_SOGI.i16Kp * 2.0f * f32TsW - f32TsW_2;
    t_SOGI.f32SOGI_A2 = __divf32(f32TempA,f32Den);

    // B0
    f32TempA = (float)t_SOGI.i16Kp * 2.0f * f32TsW;
    t_SOGI.f32SOGI_B0 = __divf32(f32TempA,f32Den);

    // C0
    f32TempA = (float)t_SOGI.i16Kp * f32TsW_2;
    t_SOGI.f32SOGI_C0 = __divf32(f32TempA,f32Den);
}

float    sSOGI_GetAlfa(void)
{
    return t_SOGI.f32SOGI_Alfa;
}

float    sSOGI_GetBeta(void)
{
    return t_SOGI.f32SOGI_Beta;   
}

unsigned long   sSOGI_GetCalFs(void)
{
    return t_SOGI.u32CalFs;
}
