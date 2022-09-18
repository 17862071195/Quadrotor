#include "control.h"
#include "headfile.h"

#define  Thr_Fly_Start  1450//起飞油门量           <>????????????????????????????需要进行实际测试

uint16_t Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4;//四个电机输出PWM
extern int pwmout4;
unsigned short mm=1100;
//输出限幅函数
uint16_t Value_Limit(uint16_t Min,uint16_t Max,uint16_t Data)
{
   if(Data>=Max) Data=Max;
   else if(Data<=Min) Data=Min;
   return Data;
}


void Control_Output()
{
	Throttle_Control+=1000;
	
			 if(Throttle_Control>=Thr_Fly_Start)//大于起飞油门
			 {	
				
              Motor_PWM_1=(int16_t)(Throttle_Control
                            -Total_Controler.Roll_Gyro_Control.Control_OutPut
                            -Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            +Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_2=(int16_t)(Throttle_Control
                            +Total_Controler.Roll_Gyro_Control.Control_OutPut
                            +Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            +Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_3=(int16_t)(Throttle_Control
                            +Total_Controler.Roll_Gyro_Control.Control_OutPut
                            -Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            -Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_4=(int16_t)(Throttle_Control
                            -Total_Controler.Roll_Gyro_Control.Control_OutPut
                            +Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            -Total_Controler.Yaw_Gyro_Control.Control_OutPut);
			  }
			 else//小于起飞油门
			 {
			   Motor_PWM_1=(int16_t)(Throttle_Control);
               Motor_PWM_2=(int16_t)(Throttle_Control);
               Motor_PWM_3=(int16_t)(Throttle_Control);
               Motor_PWM_4=(int16_t)(Throttle_Control);
               Take_Off_Reset();//清积分
			 }
			  Motor_PWM_1=Value_Limit(0,2000,Motor_PWM_1);//总输出限幅
			  Motor_PWM_2=Value_Limit(0,2000,Motor_PWM_2);
              Motor_PWM_3=Value_Limit(0,2000,Motor_PWM_3);
		      Motor_PWM_4=Value_Limit(0,2000,Motor_PWM_4);

			  PWM_Set(Motor_PWM_1,
					  Motor_PWM_2,
					  Motor_PWM_3,
					  Motor_PWM_4);
			 if(pwmout4>=1800)
			 {
				
				 if(mm>=2100){mm=2100;}
				 else{mm++;}
			 }
			 if(pwmout4<=1400)
			 {
	
				if(mm<=1100){mm=1100;}
				else{mm--;}
			 }
			 TIM_SetCompare2(TIM2, mm);
}










