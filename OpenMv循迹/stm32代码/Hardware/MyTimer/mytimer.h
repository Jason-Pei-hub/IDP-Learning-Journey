#ifndef __mytimer_H
#define __mytimer_H
#include "stm32f10x.h" 
#include "sys.h" 

extern int16_t Encoder_Count;
extern int16_t	Expect_Speed;
extern uint8_t stop_flag;

void Timer_Init(void);

#endif
