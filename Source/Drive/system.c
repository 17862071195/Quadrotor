#include "system.h"
#include "headfile.h"

void system_init(void)
{

	PWM_Init();                 //time3
	PWM2_Init();
	Total_PID_Init();           //PID��������ʼ��
//	TIM2_Configuration_Cnt();   //TIM2  ϵͳ��ʱ�����ڲ���ִ��ĳ�γ���ĺ�ʱ
//	TIM1_Cap_Init(0xffff,72-1); //time1 ң����PPM����ʱ����ʼ��  ��1Mhz��Ƶ�ʼ���
	TIM4_Cap_Init(0xffff,72-1); //time4 ң����PPM����ʱ����ʼ��  ��1Mhz��Ƶ�ʼ���
	Timer1_Configuration();//TIM3   ϵͳ��������жϳ�ʼ��                                  ��PWM��time3�г�ͻ
	NVIC_Configuration();       //�ж����ȼ�����
	
}



/***************************
   �ɿ��ж����ȼ�����
****************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;//����NVIC��ʼ���ṹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ����2������μ�misc.h line80
	
	
	//�ɿ�ϵͳ��ʱ��
//	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;//������ʱ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
		
	//PPM���벶��time4�жϷ������ȼ����ã���ppm.c�����   ���ȼ���  1    0
	
	//JY61 usart2�ж����ȼ����ã�        ��JY61.c�����  ���ȼ���  1    8
	
	
	////�ɿ�������ȶ�ʱ��
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	   //�����ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}




//void TIM2_Configuration_Cnt(void)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//    TIM_DeInit(TIM2);
//    //TIM_TimeBaseStructure.TIM_Period = 60000;//30ms
//    //TIM_TimeBaseStructure.TIM_Period = 20000;//10ms
//    TIM_TimeBaseStructure.TIM_Period = 20000;//20ms

//    TIM_TimeBaseStructure.TIM_Prescaler = 36-1; //36M/36/2=0.5us
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    TIM_Cmd(TIM2, ENABLE);
//}

//#define Hour         3
//#define Minute       2
//#define Second       1
//#define MicroSecond  0
//uint16_t Time_Sys[4]={0};
//uint16_t Microsecond_Cnt=0;
//u32 TIME_ISR_CNT=0;
//void TIM2_IRQHandler(void)//10ms
//{
// if( TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET )
// {

//   TIME_ISR_CNT++;

//   Microsecond_Cnt++;
//   if(Microsecond_Cnt>=100)//1s
//   {
//     Microsecond_Cnt=0;
//     Time_Sys[Second]++;
//     if(Time_Sys[Second]>=60)//1min
//     {
//     Time_Sys[Second]=0;
//     Time_Sys[Minute]++;
//     if(Time_Sys[Minute]>=60)//1hour
//     {
//     Time_Sys[Minute]=0;
//     Time_Sys[Hour]++;
//     }
//     }
//   }
// Time_Sys[MicroSecond]=Microsecond_Cnt;
// TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
// }
//}

