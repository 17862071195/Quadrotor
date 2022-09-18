#include "headfile.h"
#include "time.h"

float Angle_yaw=0,Angle_pitch=0,Angle_roll=0;//��Ԫ��������ĽǶ�
float Gyro_yaw=0,Gyro_pitch=0,Gyro_roll=0;//��Ԫ��������Ľ��ٶ�

void Timer1_Configuration(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

TIM_TimeBaseStructure.TIM_Period=5000;
TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
TIM_ClearFlag(TIM1, TIM_FLAG_Update);
TIM_ITConfig(                 //ʹ�ܻ���ʧ��ָ����TIM�ж�
			TIM1,             //TIM1
			TIM_IT_Update  |  //TIM �����ж�Դ
			TIM_IT_Trigger,   //TIM �����ж�Դ 
			ENABLE  	      //ʹ��
			);
TIM_Cmd(TIM1, ENABLE);
}


#define Hour         3
#define Minute       2
#define Second       1
#define MicroSecond  0
uint16_t Time_Sys[4]={0};
uint16_t Microsecond_Cnt=0;
u32 TIME_ISR_CNT=0;

void TIM1_UP_IRQHandler(void)//5msˢ��һ��
{
 if( TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET )
 {
	    TIME_ISR_CNT++;
   Microsecond_Cnt++;
   if(Microsecond_Cnt>=200)//1s
   {
     Microsecond_Cnt=0;
     Time_Sys[Second]++;
     if(Time_Sys[Second]>=60)//1min
     {
     Time_Sys[Second]=0;
     Time_Sys[Minute]++;
     if(Time_Sys[Minute]>=60)//1hour
     {
     Time_Sys[Minute]=0;
     Time_Sys[Hour]++;
     }
     }
   }
 Time_Sys[MicroSecond]=Microsecond_Cnt;
   
	 /******************ң������������ӳ��**************/
	 PPM_Mapping();
     /************��̬���ݽ���************************/
	 Angle_pitch=(float)stcAngle.Angle[0]/32768*180;
	 Angle_roll=(float)stcAngle.Angle[1]/32768*180;
	 Angle_yaw=(float)stcAngle.Angle[2]/32768*180;
	 Gyro_pitch=(float)stcGyro.w[0]/32768*2000;
	 Gyro_roll=(float)stcGyro.w[1]/32768*2000;
	 Gyro_yaw=(float)stcGyro.w[2]/32768*2000;
	 
	/*************��̬��������*****************/
	Altitude_Control(); 
	Control_Output();
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
 }
}
