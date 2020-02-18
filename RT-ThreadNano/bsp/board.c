/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "usart.h"
#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF)
    {
        return 1;
    }
    
    _SYSTICK_LOAD = ticks - 1; 
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL  = 0;
    _SYSTICK_CTRL = 0x07;  
    
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
#ifdef UART_IT_RX_ENABLE
 static struct rt_semaphore shell_rx_sem;
#endif
void rt_hw_board_init()
{
    /* System Clock Update */
    SystemCoreClockUpdate();
		#if defined(RT_USING_CONSOLE)
		#if defined(UART_IT_RX_ENABLE)
	  rt_sem_init(&(shell_rx_sem), "shell_rx", 0, RT_IPC_FLAG_FIFO);
	  #endif
    uart_init(115200); 
  	#endif
    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}


/**********************************add FINSH**********************************/

#if UART_IT_RX_ENABLE

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	  //uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		   //Res =USART_ReceiveData(USART1);	//读取接收到的数据
			/* clear interrupt */
		   USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			 rt_sem_release(&shell_rx_sem);
		}   		 
} 
#endif

/*polling to revice*/
char rt_hw_console_getchar(void)
{ 
	int ch = -1;
	#ifdef UART_IT_RX_ENABLE
	rt_sem_take(&shell_rx_sem, RT_WAITING_FOREVER); 
	ch = (char)(USART_ReceiveData(USART1)&0XFF);
	return ch;
	#else
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) !=RESET)
	{
    ch = (char)(USART_ReceiveData(USART1)&0XFF);
	}else
	{
		 if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) != RESET)
				{
				 USART_ClearFlag(USART1,USART_FLAG_ORE);
				}
				rt_thread_mdelay(10);
	}
  return ch;
	#endif
}





void rt_hw_console_output(const char *str)
{
	rt_enter_critical();
	while (*str!='\0')
	{
		if (*str=='\n')
		{
			USART_SendData(USART1, '\r'); 
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
		USART_SendData(USART1, *str++);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	rt_exit_critical();

}

