#ifndef _BSP_XBAR_H
#define _BSP_XBAR_H

#ifdef BSP_FUNCTION_XBAR_G
#define BSP_FUNCTION_XBAR extern
#else
#define BSP_FUNCTION_XBAR
#endif

BSP_FUNCTION_XBAR   void    sInitInputXbar      (void);
BSP_FUNCTION_XBAR   void    sInitOutputXbar     (void);
BSP_FUNCTION_XBAR   void    sInitEPwmXbar       (void);

#endif

