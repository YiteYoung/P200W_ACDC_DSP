
#ifndef _INIT_H
#define _INIT_H

#ifdef INIT_FUNCTION_G
#define INIT_FUNCTION extern
#else
#define INIT_FUNCTION
#endif

INIT_FUNCTION   void    sInitDspConfig         (void);
INIT_FUNCTION   void    sInitSysConfig         (void);

#endif
