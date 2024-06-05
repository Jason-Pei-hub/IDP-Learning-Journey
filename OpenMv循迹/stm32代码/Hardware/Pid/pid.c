#include "pid.h"
	
float speed_Kp=150,speed_Ki=17;
float axis_Kp=3.4,axis_Ki=0.005;
int err_sum=0;//积分和
int Servos_err_sum=0;//积分和
//限制幅度
void Limit(int16_t *Motor1,int16_t *Motor2,int16_t fuzhi){
	int16_t Pwm_Max=fuzhi;
	int16_t	Pwm_Min=-fuzhi;
	
	if(*Motor1>Pwm_Max)	*Motor1=Pwm_Max;
	if(*Motor1<Pwm_Min)	*Motor1=Pwm_Min;
	
	if(*Motor2>Pwm_Max)	*Motor2=Pwm_Max;
	if(*Motor2<Pwm_Min)	*Motor2=Pwm_Min;

}
//绝对值函数
int GFP_abs (int p){
	int q;
	q=p>0?p:(-p);
	return q;
}

int16_t Speed_PID(int16_t speed,int16_t encoder){
	static int16_t out , err,err_lowout,err_lowout_last;
	
	err=speed-encoder;
	err_lowout = 0.3* err+0.7*err_lowout_last;
	err_lowout_last=err_lowout;
	
	err_sum+=err_lowout;
	err_sum=err_sum>=5000?5000:(err_sum<(-5000)?(-5000):err_sum);//积分限幅
	
	out = speed_Kp*err_lowout + speed_Ki*err_sum ;
	
	return out;
}	

int16_t Axis_PID(int16_t Expect_x,int16_t openmv_x){
	static int16_t out , err,err_lowout,err_lowout_last;
	
	err=Expect_x-openmv_x;
	err_lowout = 0.3* err+0.7*err_lowout_last;
	err_lowout_last=err_lowout;
	
	Servos_err_sum+=err_lowout;
	Servos_err_sum=Servos_err_sum>=5000?5000:(Servos_err_sum<(-5000)?(-5000):Servos_err_sum);//积分限幅
	
	out = axis_Kp*err_lowout + axis_Ki*Servos_err_sum ;
	
	return out;
}	



