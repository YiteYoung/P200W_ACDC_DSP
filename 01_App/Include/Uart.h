#ifndef _UART_H
#define _UART_H

#ifdef UART_FUNCTION_G
#define UART_FUNCTION extern
#else
#define UART_FUNCTION
#endif

#define UART_DATA_TABLE_SIZE    50


UART_FUNCTION void  sUart_DealData(void);

#endif
