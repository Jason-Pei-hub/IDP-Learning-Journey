#ifndef __encoder_H
#define __encoder_H
#include "stm32f10x.h" 
#include "sys.h" 

void Encoder1_Init(void);
void Encoder2_Init(void);
int32_t Read_Encoder(u8 TIMX);

#endif

