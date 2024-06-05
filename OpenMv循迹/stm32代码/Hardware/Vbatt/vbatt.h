#ifndef __vbatt_H
#define __vbatt_H

#include "stm32f10x.h" 
#include "sys.h" 

void Vbatt_Init(void);

float Vbatt_Get(void);


#endif

