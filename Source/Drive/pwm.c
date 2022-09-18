#include "pwm.h"
#include "headfile.h"


//#define  MAX_PWM 2500  //400hz    周期2.5ms
#define  MAX_PWM 20000  //50hz  周期20ms

void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开始TIM4 的时钟 及GPIOB时钟 和AFIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//定时器3作为PWM输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	//设置PA5、PA6、PB0、PB1 为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_1;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    uint16_t prescalerValue = 0, ccr1_PWMVal = 0;
	PWM_GPIO_Init();
	prescalerValue = (u16) (SystemCoreClock / 1000000)-1;//10us
	//-----TIM3定时配置-----//
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;		//40000/2M=20ms-->50Hz，从0开始计数,这个值被写入到Auto-Reload Register中
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	       //暂时不分频
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;	       //时钟分割
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//重复比较次数更新事件
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM3, prescalerValue, TIM_PSCReloadMode_Immediate);//预分频,现在计时器频率为20MHz

		//-----PWM配置-----//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//选择定时器模式:TIM脉冲宽度调制模式1-->向上计数为有效电平
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = ccr1_PWMVal;					//duty cycle
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//输出极性:TIM输出比较极性高

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  					//初始化外设TIM3 OC1-->Motor1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  					//初始化外设TIM3 OC2-->Motor2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  					//初始化外设TIM3 OC3-->Motor3
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  					//初始化外设TIM3 OC4-->Motor4


   	TIM_ARRPreloadConfig(TIM3, ENABLE);//自动重载寄存器使能，下一个更新事件自动更新影子寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能TIM3在CCR2上的预装载寄存器,在更新事件时，值才被写入到CCR


        TIM_Cmd(TIM3, ENABLE);
        PWM_Set(1000,1000,1000,1001);
}


void PWM2_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    uint16_t prescalerValue = 0, ccr1_PWMVal = 0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//定时器2作为PWM输出
	
	prescalerValue = (u16) (SystemCoreClock / 1000000)-1;//10us
	//-----TIM2定时配置-----//
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;		//40000/2M=20ms-->50Hz，从0开始计数,这个值被写入到Auto-Reload Register中
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	       //暂时不分频
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;	       //时钟分割
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//重复比较次数更新事件
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
TIM_PrescalerConfig(TIM2, prescalerValue, TIM_PSCReloadMode_Immediate);//预分频,现在计时器频率为20MHz
	

		//-----PWM配置-----//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//选择定时器模式:TIM脉冲宽度调制模式1-->向上计数为有效电平
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = ccr1_PWMVal;					//duty cycle
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//输出极性:TIM输出比较极性高

TIM_OC2Init(TIM2, &TIM_OCInitStructure);  					//初始化外设TIM3 OC1-->Motor1


TIM_ARRPreloadConfig(TIM2, ENABLE);

TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	
	
TIM_Cmd(TIM2, ENABLE);

TIM_SetCompare2(TIM2, 1000);						//************************************************************************	
        
}

/***********************************************************************************
函数名：void PWM_Set(const u16 pwm1, const u16 pwm2, const u16 pwm3, const u16 pwm4)
说明：PWM输出设置
入口：四个通道的值
出口：无
备注：满占空为2.5ms（20ms）    1000<=pwm<=2000
************************************************************************************/
void PWM_Set(const uint16_t pwm1, const uint16_t pwm2, const uint16_t pwm3, const uint16_t pwm4)
{
	TIM_SetCompare1(TIM3, pwm1);
	TIM_SetCompare2(TIM3, pwm2);
	TIM_SetCompare3(TIM3, pwm3);
	TIM_SetCompare4(TIM3, pwm4);
}
