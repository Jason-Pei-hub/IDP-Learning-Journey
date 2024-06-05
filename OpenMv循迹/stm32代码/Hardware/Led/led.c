#include "led.h"

void Led_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_1);	
	GPIO_SetBits(GPIOF,GPIO_Pin_3);			
	GPIO_SetBits(GPIOF,GPIO_Pin_5);	
}

void Led_On(Led_TypeDef _Led){
	switch(_Led){
		case LEDB:	GPIO_ResetBits(GPIOF,GPIO_Pin_1);	
					GPIO_SetBits(GPIOF,GPIO_Pin_3);
					GPIO_SetBits(GPIOF,GPIO_Pin_5);	
					break;
		
		case LEDG:	GPIO_ResetBits(GPIOF,GPIO_Pin_3);
					GPIO_SetBits(GPIOF,GPIO_Pin_1);
					GPIO_SetBits(GPIOF,GPIO_Pin_5);
					break;
		
		case LEDR:	GPIO_ResetBits(GPIOF,GPIO_Pin_5);	
					GPIO_SetBits(GPIOF,GPIO_Pin_1);	
					GPIO_SetBits(GPIOF,GPIO_Pin_3);
					break;
		
		default:	break;
	}
}

void Led_Off(void){
	GPIO_SetBits(GPIOF,GPIO_Pin_1);	
	GPIO_SetBits(GPIOF,GPIO_Pin_3);
	GPIO_SetBits(GPIOF,GPIO_Pin_5);			
}


