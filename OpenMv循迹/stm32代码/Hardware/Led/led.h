#ifndef __led_H
#define __led_H

#include "stm32f10x.h" 
#include "sys.h" 

typedef enum//	Ã¶¾Ù
{
	LEDR,
	LEDG,
	LEDB,
}Led_TypeDef;

void Led_Init(void);
void Led_On(Led_TypeDef _Led);
void Led_Off(void);

#endif

