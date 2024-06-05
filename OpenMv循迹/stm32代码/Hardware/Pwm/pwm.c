#include "pwm.h"

void Pwm_Tim1_Init(u16 arr,u16 psc){ 
    TIM_TimeBaseInitTypeDef	 	TIM_TimeBaseStructure; //定义结构体
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);//先附上默认值，然后手动改所需的值
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//选择时基单元的时钟
	TIM_InternalClockConfig(TIM1);//这样TIM1的时基单元就由内部时钟来驱动了
	
	//配置时基单元
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up ;//计数器模式 向上计数
	TIM_TimeBaseStructure.TIM_Period=arr-1;//周期 就是ARR自动重装器的值    定时频率=72M/(PSC+1)/(ARR+1)
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;//PSC预分频器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//是重复计数器的值(高级定时器的功能，这里不需要)
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//设置输出比较模式
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//设置输出比较的极性(有效电平是高电平,REF有效时，输出高电平,)
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable   ;//设置输出使能
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);//通道1  PA8
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//设置输出比较模式
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//设置输出比较的极性(有效电平是高电平,REF有效时，输出高电平,)
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable   ;//设置输出使能
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);//通道2 PA9
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //预重装值，开启输出PWM
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable); 	//预重装值，开启输出PWM
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//启动自动重装
    TIM_Cmd(TIM1,ENABLE);//启动定时器
}



//2个轮子占空比调节
void Pwm_Out(int16_t motor1,int16_t motor2){   
	if(motor1>=0){	
		GPIO_SetBits(GPIOE,GPIO_Pin_2);
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	}
	else{
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	}
	
	if(motor2>=0){
		GPIO_SetBits(GPIOE,GPIO_Pin_6);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	}
	else{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
		GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	}
	
	motor1=GFP_abs(motor1);//取绝对值
	motor2=GFP_abs(motor2);//取绝对值
	
	Limit(&motor1,&motor2,7000);//限幅
	
	TIM1->CCR1 = motor1; //输出
	TIM1->CCR2 = motor2; //输出
}

