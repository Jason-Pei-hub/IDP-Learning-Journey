#ifndef __pwm_H
#define __pwm_H

#include "stm32f10x.h" 
#include "sys.h" 

void Pwm_Tim1_Init(u16 arr,u16 psc);
void Pwm_Out(int16_t motor1,int16_t motor2);


#endif

