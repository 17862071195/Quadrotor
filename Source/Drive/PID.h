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
    uint8 Err_Limit_Flag :1;//ƫ���޷���־
    uint8 Integrate_Limit_Flag :1;//�����޷���־
    uint8 Integrate_Separation_Flag :1;//���ַ����־
    float Expect;//����
    float FeedBack;//����ֵ
    float Err;//ƫ��
    float Last_Err;//�ϴ�ƫ��
    float Err_Max;//ƫ���޷�ֵ
    float Integrate_Separation_Err;//���ַ���ƫ��ֵ
    float Integrate;//����ֵ
    float Integrate_Max;//�����޷�ֵ
    float Kp;//���Ʋ���Kp
    float Ki;//���Ʋ���Ki
    float Kd;//���Ʋ���Kd
    float Control_OutPut;//�����������
    float Last_Control_OutPut;//�ϴο����������
    float Control_OutPut_Limit;//����޷�
    /***************************************/
    float Last_FeedBack;//�ϴη���ֵ
    float Dis_Err;//΢����
    float Dis_Error_History[5];//��ʷ΢����
    float Err_LPF;
    float Last_Err_LPF;
    float Dis_Err_LPF;
    Butter_BufferData Control_Device_LPF_Buffer;//��������ͨ�����������
}PID_Controler;

typedef struct
{
     PID_Controler Pitch_Angle_Control;
     PID_Controler Pitch_Gyro_Control;
     PID_Controler Roll_Angle_Control;
     PID_Controler Roll_Gyro_Control;
     PID_Controler Yaw_Angle_Control;
     PID_Controler Yaw_Gyro_Control;
//     PID_Controler High_Position_Control;    //�߶ȿ��ƽṹ�����
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
