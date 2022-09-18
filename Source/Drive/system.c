#include "system.h"
#include "headfile.h"

void system_init(void)
{

	PWM_Init();                 //time3
	PWM2_Init();
	Total_PID_Init();           //PID控制器初始化
//	TIM2_Configuration_Cnt();   //TIM2  系统定时器用于测量执行某段程序的耗时
//	TIM1_Cap_Init(0xffff,72-1); //time1 遥控器PPM捕获定时器初始化  以1Mhz的频率计数
	TIM4_Cap_Init(0xffff,72-1); //time4 遥控器PPM捕获定时器初始化  以1Mhz的频率计数
	Timer1_Configuration();//TIM3   系统任务调度中断初始化                                  与PWM的time3有冲突
	NVIC_Configuration();       //中断优先级设置
	
}



/***************************
   飞控中断优先级配置
****************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;//定义NVIC初始化结构体
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级组别2，具体参见misc.h line80
	
	
	//飞控系统定时器
//	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;//计数定时器
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
		
	//PPM输入捕获time4中断分组优先级配置，在ppm.c中完成   优先级：  1    0
	
	//JY61 usart2中断优先级配置，        在JY61.c中完成  优先级：  1    8
	
	
	////飞控任务调度定时器
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	   //从优先级0级
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

