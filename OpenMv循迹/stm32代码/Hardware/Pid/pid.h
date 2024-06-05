#ifndef __pid_H
#define __pid_H

#include "stm32f10x.h" 
#include "sys.h" 

extern int err_sum;
extern int Servos_err_sum;
void Limit(int16_t *Motor1,int16_t *Motor2,int16_t fuzhi);

int GFP_abs (int p);

int16_t Speed_PID(int16_t speed,int16_t encoder);
int16_t Axis_PID(int16_t Expect_x,int16_t openmv_x);

#endif

