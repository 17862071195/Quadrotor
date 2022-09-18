/*
��д�ߣ�Hanker
��ַ��
����E-mail��1137364796@qq.com
���뻷����Keil5  
����ʱ��: 2016-1-31
���ԣ� Դ�����ڵ�STM32Coreƽ̨����ɲ���
���ܣ�
��STM32Coreƽ̨����2��ȡJY61�����ݣ�������115200��
����
USB-TTL����                 STM32Core              JY61
VCC          -----           VCC        ----        VCC
TX           -----           RX1     
RX           -----           TX1
GND          -----           GND        ----        GND
                             RX2        ----        TX
							 TX2        ----        RX
------------------------------------
 */
#include <string.h>
#include <stdio.h>
#include "Main.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "jy61.h"
#include "delay.h"
#include "JY901.h"

#include "oled.h"
#include "sys.h"
#include "bmp.h"

#include "headfile.h"



extern int pwmout1, pwmout2, pwmout3, pwmout4; 	//���ջ�ppm��� ��λ�� ��λ��us
extern unsigned short     int Time_Sys[4];
extern uint16_t Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4;//�ĸ�������PWM
extern unsigned short mm;

int main(void)
{  
	
	char str[100];
	unsigned char len,i;
/**********************JY61��ʼ��***********************/			
	SysTick_init(72,10);
	Initial_UART2(115200);//��JY-61ģ��Ĵ���
	
/**********************oled��ʼ��**********************/
//	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			
	OLED_Init();			//��ʼ��OLED  
    system_init();	        //ϵͳ��ʼ��
	OLED_Clear(); 
	delay_ms(1000);//�ȵ�JY-61��ʼ�����
	while(1)
	{	
//		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
//		OLED_ShowString(20,4,"2019/04/27");  
//		OLED_ShowString(0,6,"ASCII:");  
//		OLED_ShowString(63,6,"CODE:"); 
		
		
//		OLED_ShowNum(0,0,Gyro_yaw,5,16);        //��ʾ����
//		OLED_ShowNum(45,0,Gyro_pitch,5,16);        //��ʾ����
//		OLED_ShowNum(0,2,Gyro_roll,5,16);        //��ʾ����
		

		if(Time_Sys[1]%10<=5)//Time_Sys[1]%10<=5
		{
			if(Time_Sys[1]%10==0)  OLED_Clear(); 
//			sprintf(str,"%d:%d:\r",Target_Angle[0],Target_Angle[1]);
//			OLED_ShowString(0,0,str);
//		OLED_ShowNum(0,0,Target_Angle[0],5,16);                //pitch
//		OLED_ShowNum(60,0,Target_Angle[1],5,16);                //roll
			OLED_ShowCHinese(0,0,7);//ϵ
			OLED_ShowCHinese(18,0,8);//ͳ
			OLED_ShowCHinese(36,0,9);//��
			OLED_ShowCHinese(54,0,10);//��
			OLED_ShowCHinese(72,0,11);//ʱ
			OLED_ShowCHinese(90,0,12);//��
			sprintf(str,"%d:%d:%d\r",Time_Sys[3],Time_Sys[2],Time_Sys[1]);
			OLED_ShowString(0,2,str);				
		sprintf(str,"Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
		OLED_ShowString(0,4,str);	
			
		}
		else{
		if(Time_Sys[1]%10==6)  OLED_Clear(); 	
		sprintf(str,"Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
		OLED_ShowString(0,0,str);
		
		
			OLED_ShowNum(0,4,Target_Angle[0],2,16);
			OLED_ShowNum(18,4,Target_Angle[1],2,16);        //��ʾ����

			OLED_ShowNum(38,4,Motor_PWM_1,5,16);
			OLED_ShowNum(78,4,Motor_PWM_2,5,16);        //��ʾ����
		    OLED_ShowNum(0,6,Throttle_Control,5,16);        //��ʾ����
			OLED_ShowNum(41,6,Motor_PWM_3,5,16);        //��ʾ����
			OLED_ShowNum(79,6,Motor_PWM_4,5,16);        //��ʾ����		
		}
		
		
//			OLED_ShowString(0,4,"pitch:");
//			OLED_ShowNum(48,4,Angle_pitch,3,16);        //��ʾ����
//			
//			OLED_ShowString(70,6,"yaw:");
//			OLED_ShowNum(105,6,Angle_yaw,3,16);        //��ʾ����

//			OLED_ShowString(0,6,"roll:");
//			OLED_ShowNum(39,6,Angle_roll,3,16);        //��ʾ����		
		
//		OLED_ShowNum(45,6,pwmout3,5,16);        //��ʾ����
		
//		OLED_ShowString(63,6,stcAngle.Angle[0]);
		
		
//			delay_ms(500);
//		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);		
//			UART1_Put_String(str);		
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
//			UART1_Put_String(str);
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
//		OLED_ShowString(0,4,str);

//		sprintf(str,"Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
//		OLED_ShowString(0,0,str);	
		
//			UART1_Put_String(str);
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);
//			UART1_Put_String(str);		
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"Pressure:%ld Height%.2f\r\n",stcPress.lPressure,(float)stcPress.lAltitude/100);
//			UART1_Put_String(str);
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"DStatus:%d %d %d %d\r\n",stcDStatus.sDStatus[0],stcDStatus.sDStatus[1],stcDStatus.sDStatus[2],stcDStatus.sDStatus[3]);
//			UART1_Put_String(str);
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"Longitude:%ldDeg%.5fm Lattitude:%ldDeg%.5fm\r\n",stcLonLat.lLon/10000000,(double)(stcLonLat.lLon % 10000000)/1e5,stcLonLat.lLat/10000000,(double)(stcLonLat.lLat % 10000000)/1e5);
//			UART1_Put_String(str);
//			delay_ms(10);//�ȴ��������
//		sprintf(str,"GPSHeight:%.1fm GPSYaw:%.1fDeg GPSV:%.3fkm/h\r\n\r\n",(float)stcGPSV.sGPSHeight/10,(float)stcGPSV.sGPSYaw/10,(float)stcGPSV.lGPSVelocity/1000);
//			UART1_Put_String(str);
//			delay_ms(10);//�ȴ��������
	}//��ѭ�� 
}



