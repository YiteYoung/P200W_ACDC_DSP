#ifndef _BSP_RLY_H
#define _BSP_RLY_H

#ifdef  BSP_RLY_FUNCTION_G
#define BSP_RLY_FUNCTION extern
#else
#define BSP_RLY_FUNCTION
#endif

typedef enum 
{
    Rly_Out = 0,
    Rly_SoftR,
    Rly_Grid,
    Rly_EV,
    Rly_END
}Rly_e;

#endif
