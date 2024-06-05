#include "mytask.h"


TaskHandle_t Esp_Handle;//任务句柄


/*起始任务*/
void Create_Task(void *arg){
	
	delay_Init();				//延时初始化
	OLED_Init();				//OLED初始化
	UART4_Init(115200);			//debug串口
	USART3_Init(115200);		//ESP8266串口
	ESP8266_Init();
	
	
	while(OneNet_DevLink())			//接入服务器
		delay_ms(500);
	OLED_ShowString(1,1,"              ");
	
	
	Pwm_Tim1_Init(7200,1);		//电机pwm初始化
	Motor_Init();				//电机方向初始化
	Encoder1_Init();			//编码器1初始化
	Encoder2_Init();			//编码器2初始化
	Timer_Init();				//定时器初始化
	
	UART5_Init(9600);			//OpenMv通讯
	Servos_Init();				//舵机初始化
	
	xTaskCreate(Esp_Task,"Esp_Task",256,NULL,1,&Esp_Handle);//创建任务
	
	
	vTaskDelete(NULL);
}

void Esp_Task(void *arg){
	
	const char *topic_sub[] = {"/QT_Topic"};//订阅话题名
	const char topic_pub[] = {"/MCU_Topic"};//发布话题名
	char pub_buf[256];						//上传的数据BUF
	unsigned short timeCount = 0;			//发送间隔变量
	unsigned char *dataPtr = NULL;
	
	OneNet_Subscribe(topic_sub, 1);
	
	for(;;){
		
		if(++timeCount >= 25){

			//sprintf(pub_buf,"{\"Pitch\":%d,\"Roll\":%d,\"Yaw\":%d,\"Room\":%d,\"Speed\":%d,\"Flag\":%d,\"Power\":%d}",-30,40,50,RoomNum,Expect_Speed,1,(int)Vbatt);
			
			OneNet_Publish(topic_pub,pub_buf);//发送数据
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(2);//是否接收到消息
		
		if(dataPtr != NULL)	OneNet_RevPro(dataPtr);	
	
		vTaskDelay(10);
		
	}
}



