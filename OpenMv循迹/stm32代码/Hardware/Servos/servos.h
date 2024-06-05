#ifndef __servos_H
#define __servos_H

#include "stm32f10x.h" 
#include "sys.h" 

void Servos_Init(void);
void Servos_SetAngle(float Angle);
void Servos_Pwm(int16_t pwm);

#endif
