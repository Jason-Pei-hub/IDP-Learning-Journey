#ifndef __usart_H
#define __usart_H
#include "stm32f10x.h" 
#include "sys.h" 
#include <stdarg.h>
#include <stdio.h>

#define USART_DEBUG		UART4		//调试打印所使用的串口组

extern int OpenMv_x;


void USART3_Init(u32 BoTeLv);
void UART4_Init(u32 BoTeLv);
void UART5_Init(u32 BoTeLv);



void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);



#endif

