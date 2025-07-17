#ifndef _UART_H
#define _UART_H

#ifdef UART_FUNCTION_G
#define UART_FUNCTION extern
#else
#define UART_FUNCTION
#endif


UART_FUNCTION void  sUart_DealData(void);

#endif
