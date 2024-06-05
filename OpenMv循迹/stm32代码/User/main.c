#include "sys.h"

int main(void){	 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//中断分组
	
	/*1.任务函数名字 2.任务名字(字符串) 3.堆栈大小 4.任务传递参数	5.任务优先级	 6.任务句柄	*/
	xTaskCreate(Create_Task,"Create_Task",512,NULL,1,NULL);
	
	vTaskStartScheduler();//开启调度器
	
	while(1){  
		
	}
}
