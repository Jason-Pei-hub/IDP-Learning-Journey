#ifndef __mytask_H
#define __mytask_H

#include "stm32f10x.h" 
#include "sys.h" 

void Create_Task(void *arg);

void Esp_Task(void *arg);
void OLED_Task(void *arg);

#endif
