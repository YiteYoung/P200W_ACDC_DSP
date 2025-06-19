#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#ifdef INTERRUPT_FUNCTION_G
#define INTERRUPT_FUNCTION extern
#else
#define INTERRUPT_FUNCTION
#endif

#include "f28x_project.h"

INTERRUPT_FUNCTION  void    sInitInterrupt      (void);

#endif
