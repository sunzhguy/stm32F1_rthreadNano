#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


void uart_init(uint32_t bound);


#ifndef DEBUG
#define Printf(args...)  
#else
#define Printf(msg,args...)  {printf(msg,##args);}
#endif

#endif


