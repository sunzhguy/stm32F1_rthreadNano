/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  sunzhguy @super
  * @version V3.5.0
  * @date    11-Dec-2017
  * @brief   Main program body
  ******************************************************************************
  ******************************************************************************
  */                        //C8  64K  20K
/* Includes ------------------STM32F103RE------FLASH 512K  SDRAM 64K----------------MD--------------------------*/
/* Includes ------------------STM32F103RC------FLASH 256K  SDRAM 64K------------------------------------------*/
#include "stm32f10x.h"
#include <stdlib.h>
#include <math.h>
#include "usart.h"
#include <rtthread.h>

/*****************************************
        by sunzhguy

BOOT1=x  BOOT0=0  boot form  flash  normal  mode
BOOT1=0  BOOT0=1  boot from flash [bootload isp] ,by user 
BOOT1=1  BOOT0=1  boot from  sdram

**************************************/

#define LED_SET    PAout(2)
void  delay()
{
	int i = 0xffff;
  do{
	
	
	}while(i--);

}
void Led_init()
{
   GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); 
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	         
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOA, &GPIO_InitStructure);	  	                    
  GPIO_SetBits(GPIOA,GPIO_Pin_2); 			
  GPIO_ResetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15); 			 //LED_CTR1...4->out low 

}

int  main(void)
{

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
   Led_init();
	 //uart_init(115200);
	  while(1)
		{
		   LED_SET =1;
			 rt_thread_mdelay(500);
			 LED_SET =0;
			 rt_kprintf("hellowolrd\r\n");
			 rt_thread_mdelay(500);
		}
		return 0;
}
