#ifndef __delay_H
#define __delay_H
#include "stm32f10x.h" 
#include "sys.h" 

void delay_Init(void);
void delay_us(u16 xus);
void delay_ms(u16 xms);


#endif
