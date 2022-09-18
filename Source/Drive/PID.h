#ifndef _PID_H_
#define _PID_H_
#include "headfile.h"
#define uint8 unsigned char
	
typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;

typedef struct
{
 const float a[3];
 const float b[3];
}Butter_Parameter;


typedef struct
{
    uint8 Err_Limit_Flag :1;//偏差限幅标志
    uint8 Integrate_Limit_Flag :1;//积分限幅标志
    uint8 Integrate_Separation_Flag :1;//积分分离标志
    float Expect;//期望
    float FeedBack;//反馈值
    float Err;//偏差
    float Last_Err;//上次偏差
    float Err_Max;//偏差限幅值
    float Integrate_Separation_Err;//积分分离偏差值
    float Integrate;//积分值
    float Integrate_Max;//积分限幅值
    float Kp;//控制参数Kp
    float Ki;//控制参数Ki
    float Kd;//控制参数Kd
    float Control_OutPut;//控制器总输出
    float Last_Control_OutPut;//上次控制器总输出
    float Control_OutPut_Limit;//输出限幅
    /***************************************/
    float Last_FeedBack;//上次反馈值
    float Dis_Err;//微分量
    float Dis_Error_History[5];//历史微分量
    float Err_LPF;
    float Last_Err_LPF;
    float Dis_Err_LPF;
    Butter_BufferData Control_Device_LPF_Buffer;//控制器低通输入输出缓冲
}PID_Controler;

typedef struct
{
     PID_Controler Pitch_Angle_Control;
     PID_Controler Pitch_Gyro_Control;
     PID_Controler Roll_Angle_Control;
     PID_Controler Roll_Gyro_Control;
     PID_Controler Yaw_Angle_Control;
     PID_Controler Yaw_Gyro_Control;
//     PID_Controler High_Position_Control;    //高度控制结构体变量
//     PID_Controler High_Speed_Control;
}AllControler;

typedef enum
{
     Pitch_Angle_Controler=0,
     Pitch_Gyro_Controler=1,
     Roll_Angle_Controler=2,
     Roll_Gyro_Controler=3,
     Yaw_Angle_Controler=4,
     Yaw_Gyro_Controler=5,
//     High_Position_Controler=6,
//     High_Speed_Controler=7,
}Controler_Label;

extern AllControler Total_Controler;

void Total_PID_Init(void);
void PID_Init(PID_Controler *Controler,Controler_Label Label);
void Altitude_Control(void);
void Angle_Control();
void Gyro_Control();

float PID_Control(PID_Controler *Controler);
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter);
float PID_Control_Div_LPF(PID_Controler *Controler);

void  PID_Integrate_Reset(PID_Controler *Controler);
void  Take_Off_Reset(void);
#endif
