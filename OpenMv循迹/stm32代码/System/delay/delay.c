#include "delay.h"
#include "stm32f10x.h"

void delay_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM7时钟
	TIM_TimeBaseInitStructure.TIM_Period    = 50000-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 60-1;     //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
}

//微秒级延时
void delay_us(u16 xus)
{
	TIM_Cmd(TIM7,ENABLE); //启动定时器
	while(TIM7->CNT < xus);
	TIM7->CNT = 0;
	TIM_Cmd(TIM7,DISABLE); //关闭定时器
}

//毫秒级延时
void delay_ms(u16 xms)
{
	int i;
	for(i=0;i<xms;i++)
	{
		delay_us(1000);
	}
}

//秒级延时
void delay_s(u16 xs)
{
	int i;
	for(i=0;i<xs;i++)
	{
		delay_ms(1000);
	}
}
