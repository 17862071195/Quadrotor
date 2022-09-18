#include "pid.h"
#include "stdint.h"

/*
1偏差限幅标志；  2积分限幅标志；3积分分离标志；   4期望；
5反馈            6偏差；        7上次偏差；       8偏差限幅值；
9积分分离偏差值；10积分值       11积分限幅值；    12控制参数Kp；
13控制参数Ki；   14控制参数Kd； 15控制器总输出；  16上次控制器总输出
17总输出限幅度
*/
/***************************觉得不错，欢迎打赏，请小哥吃热干面：支付宝账号：1094744141@qq.com*********************************/
const float Control_Unit[6][17]=
{
/*                                         Kp        Ki        Kd            */
 /*1  2  3  4  5  6   7  8   9   10  11    12        13        14  15  16  17*/
 {1  ,1 ,0 ,0 ,0 ,0 , 0 ,35  ,0  ,0 , 20,   4.50   ,0.0000  ,0.00 ,0  ,0 , 250},//Pitch_Angle;俯仰角度
 {0  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 200,  0.85   ,0.0150  ,4.5  ,0  ,0 , 500},//Pitch_Gyro;俯仰角速度
 {1  ,1 ,0 ,0 ,0 ,0 , 0 ,35  ,0  ,0 , 20,   4.50   ,0.0000  ,0.00 ,0  ,0 , 250},//Roll_Angle;横滚角
 {0  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 200,  0.85   ,0.0150  ,4.5  ,0  ,0 , 500},//Roll_Gyro;横滚角速度
 {1  ,1 ,0 ,0 ,0 ,0 , 0 ,45  ,0  ,0 , 0 ,   6.50   ,0       ,0.00  ,0  ,0 ,150},//Yaw_Angle;偏航角
 {1  ,1 ,0 ,0 ,0 ,0 , 0 ,250 ,0  ,0 , 100,  2.00   ,0.0080  ,3.5  ,0  ,0 ,300},//Yaw_Gyro;偏航角速度
};


float Yaw_Feedforward=0.25;//偏航角前馈控制;          //？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？


AllControler Total_Controler;//系统总控制器
void PID_Init(PID_Controler *Controler,Controler_Label Label)
{
  Controler->Err_Limit_Flag=(uint8)(Control_Unit[Label][0]);//1偏差限幅标志
  Controler->Integrate_Limit_Flag=(uint8)(Control_Unit[Label][1]);//2积分限幅标志
  Controler->Integrate_Separation_Flag=(uint8)(Control_Unit[Label][2]);//3积分分离标志
  Controler->Expect=Control_Unit[Label][3];//4期望
  Controler->FeedBack=Control_Unit[Label][4];//5反馈值
  Controler->Err=Control_Unit[Label][5];//6偏差
  Controler->Last_Err=Control_Unit[Label][6];//7上次偏差
  Controler->Err_Max=Control_Unit[Label][7];//8偏差限幅值
  Controler->Integrate_Separation_Err=Control_Unit[Label][8];//9积分分离偏差值
  Controler->Integrate=Control_Unit[Label][9];//10积分值
  Controler->Integrate_Max=Control_Unit[Label][10];//11积分限幅值
  Controler->Kp=Control_Unit[Label][11];//12控制参数Kp
  Controler->Ki=Control_Unit[Label][12];//13控制参数Ki
  Controler->Kd=Control_Unit[Label][13];//14控制参数Ki
  Controler->Control_OutPut=Control_Unit[Label][14];//15控制器总输出
  Controler->Last_Control_OutPut=Control_Unit[Label][15];//16上次控制器总输出
  Controler->Control_OutPut_Limit=Control_Unit[Label][16];//16上次控制器总输出
}


void Total_PID_Init(void)
{
 PID_Init(&Total_Controler.Pitch_Angle_Control,Pitch_Angle_Controler);                      //俯仰控制
 PID_Init(&Total_Controler.Pitch_Gyro_Control,Pitch_Gyro_Controler);
 PID_Init(&Total_Controler.Roll_Angle_Control,Roll_Angle_Controler);                        //滚转控制
 PID_Init(&Total_Controler.Roll_Gyro_Control,Roll_Gyro_Controler);
 PID_Init(&Total_Controler.Yaw_Angle_Control,Yaw_Angle_Controler);                          //偏航控制
 PID_Init(&Total_Controler.Yaw_Gyro_Control,Yaw_Gyro_Controler);                            
// PID_Init(&Total_Controler.High_Position_Control,High_Position_Controler);                  //高度控制       (需要气压计，暂时没有)
// PID_Init(&Total_Controler.High_Speed_Control,High_Speed_Controler);
		
// PID_Init(&Total_Controler.Longitude_Position_Control,Longitude_Position_Controler);        //经度控制
// PID_Init(&Total_Controler.Longitude_Speed_Control,Longitude_Speed_Controler);
// PID_Init(&Total_Controler.Latitude_Position_Control,Latitude_Position_Controler);          //纬度控制
// PID_Init(&Total_Controler.Latitude_Speed_Control,Latitude_Speed_Controler);

// PID_Init(&Total_Controler.High_Acce_Control,High_Acce_Controler);                          //高度加速度控制（三环控制）
// PID_Init(&Total_Controler.Longitude_Acce_Control,Longitude_Acce_Controler);                //经度加速度控制（三环控制）
// PID_Init(&Total_Controler.Latitude_Acce_Control,Latitude_Acce_Controler);                  //纬度加速度控制（三环控制）
}


/************************************************************************************************/
/************姿态环控制器：角度+角速度*************************************************************/
void Altitude_Control(void)
{
  Angle_Control();//角度控制
  Gyro_Control();//角速度控制
}

void Angle_Control()//角度环节
{
	 Total_Controler.Pitch_Angle_Control.Expect=Target_Angle[0];
     Total_Controler.Roll_Angle_Control.Expect=Target_Angle[1];              
	
	
      //角度反馈
     Total_Controler.Pitch_Angle_Control.FeedBack=Angle_pitch;               
     PID_Control(&Total_Controler.Pitch_Angle_Control);                        
     Total_Controler.Roll_Angle_Control.FeedBack =Angle_roll;               
     PID_Control(&Total_Controler.Roll_Angle_Control);

	
        Total_Controler.Yaw_Angle_Control.Expect=0;//偏航角期望给0,不进行角度控制
        Total_Controler.Yaw_Gyro_Control.Expect=Yaw_Control;//偏航角速度环期望，直接来源于遥控器打杆量
     
}



void Gyro_Control()//角速度环
{
//	if(GYRO_CONTROL_MODE==PID_MODE)//俯仰、横滚方向姿态内环角速度控制器采用PID控制器
//	{
		 /***************内环角速度期望****************/
		 Total_Controler.Pitch_Gyro_Control.Expect=Total_Controler.Pitch_Angle_Control.Control_OutPut;
		 Total_Controler.Roll_Gyro_Control.Expect=Total_Controler.Roll_Angle_Control.Control_OutPut;
		 /***************内环角速度反馈****************/
		 Total_Controler.Pitch_Gyro_Control.FeedBack=Gyro_pitch;               //角速度数字量转化成角度制，单位:度/秒(deg/s)
		 Total_Controler.Roll_Gyro_Control.FeedBack=Gyro_roll;

		 /***************内环角速度控制****************/
		 PID_Control_Div_LPF(&Total_Controler.Pitch_Gyro_Control);
		 PID_Control_Div_LPF(&Total_Controler.Roll_Gyro_Control);
//	}
//	else if(GYRO_CONTROL_MODE==ADRC_MODE)//俯仰、横滚方向姿态内环角速度控制器采用ADRC自抗扰控制器
//	{


//	}
//		else//测试用、正常只选择一种模式
//		{
//			/***************内环角速度期望****************/
//			Total_Controler.Pitch_Gyro_Control.Expect=Total_Controler.Pitch_Angle_Control.Control_OutPut;
//			Total_Controler.Roll_Gyro_Control.Expect=Total_Controler.Roll_Angle_Control.Control_OutPut;
//			/***************内环角速度反馈****************/
//			Total_Controler.Pitch_Gyro_Control.FeedBack=Pitch_Gyro;
//			Total_Controler.Roll_Gyro_Control.FeedBack=Roll_Gyro;

//			/***************内环角速度控制****************/
//			PID_Control_Div_LPF(&Total_Controler.Pitch_Gyro_Control);
//			PID_Control_Div_LPF(&Total_Controler.Roll_Gyro_Control);
//		}

//偏航角前馈控制
 Total_Controler.Yaw_Gyro_Control.FeedBack=Gyro_yaw;
 PID_Control_Div_LPF(&Total_Controler.Yaw_Gyro_Control);
 //**************************偏航角前馈控制**********************************
 Total_Controler.Yaw_Gyro_Control.Control_OutPut=Total_Controler.Yaw_Gyro_Control.Control_OutPut
   +Yaw_Feedforward*Total_Controler.Yaw_Gyro_Control.Expect;//偏航角前馈控制
 if(Total_Controler.Yaw_Gyro_Control.Control_OutPut>=Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit)
   Total_Controler.Yaw_Gyro_Control.Control_OutPut=Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit;
 if(Total_Controler.Yaw_Gyro_Control.Control_OutPut<=-Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit)
   Total_Controler.Yaw_Gyro_Control.Control_OutPut=-Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit;
}




float PID_Control(PID_Controler *Controler)
{
/*******偏差计算*********************/
  Controler->Last_Err=Controler->Err;//保存上次偏差
  Controler->Err=Controler->Expect-Controler->FeedBack;//期望减去反馈得到偏差
  if(Controler->Err_Limit_Flag==1)//偏差限幅度标志位
  {
  if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
  if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
  }
/*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag==1)//积分分离标志位
  {
    if(ABS(Controler->Err)<=Controler->Integrate_Separation_Err)
    Controler->Integrate+=Controler->Ki*Controler->Err;
  }
  else
  {
    Controler->Integrate+=Controler->Ki*Controler->Err;
  }
/*******积分限幅*********************/
 if(Controler->Integrate_Limit_Flag==1)//积分限制幅度标志
 {
  if(Controler->Integrate>=Controler->Integrate_Max)
    Controler->Integrate=Controler->Integrate_Max;
  if(Controler->Integrate<=-Controler->Integrate_Max)
    Controler->Integrate=-Controler->Integrate_Max ;
 }
/*******总输出计算*********************/
  Controler->Last_Control_OutPut=Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut=Controler->Kp*Controler->Err//比例
                         +Controler->Integrate//积分
                         +Controler->Kd*(Controler->Err-Controler->Last_Err);//微分
/*******总输出限幅*********************/
  if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
  Controler->Control_OutPut=Controler->Control_OutPut_Limit;
  if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
  Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
/*******返回总输出*********************/
  return Controler->Control_OutPut;
}





float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
        /* 加速度计Butterworth滤波 */
	/* 获取最新x(n) */
        Buffer->Input_Butter[2]=curr_inputer;
	/* Butterworth滤波 */
        Buffer->Output_Butter[2]=
         Parameter->b[0] * Buffer->Input_Butter[2]
        +Parameter->b[1] * Buffer->Input_Butter[1]
	+Parameter->b[2] * Buffer->Input_Butter[0]
        -Parameter->a[1] * Buffer->Output_Butter[1]
        -Parameter->a[2] * Buffer->Output_Butter[0];
	/* x(n) 序列保存 */
        Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
        Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
	/* y(n) 序列保存 */
        Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
        Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
        return (Buffer->Output_Butter[2]);
}



Butter_Parameter Control_Device_Div_LPF_Parameter={
 //200---20hz
  1,    -1.14298050254,   0.4128015980962,
  0.06745527388907,   0.1349105477781,  0.06745527388907
};


float PID_Control_Div_LPF(PID_Controler *Controler)
{
  int16_t  i=0;
/*******偏差计算*********************/
  Controler->Last_Err=Controler->Err;//保存上次偏差
  Controler->Err=Controler->Expect-Controler->FeedBack;//期望减去反馈得到偏差
  Controler->Dis_Err=Controler->Err-Controler->Last_Err;//原始微分
  for(i=4;i>0;i--)//数字低通后微分项保存
  {
  Controler->Dis_Error_History[i]=Controler->Dis_Error_History[i-1];
  }
  Controler->Dis_Error_History[0]=Control_Device_LPF(Controler->Dis_Err,
                                  &Controler->Control_Device_LPF_Buffer,
                                  &Control_Device_Div_LPF_Parameter);//巴特沃斯低通后得到的微分项,20hz
  

//在哪进行的低通滤波？？？？？？？
  
  
  
  
  if(Controler->Err_Limit_Flag==1)//偏差限幅度标志位
  {
  if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
  if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
  }
/*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag==1)//积分分离标志位
  {
    if(ABS(Controler->Err)<=Controler->Integrate_Separation_Err)
    Controler->Integrate+=Controler->Ki*Controler->Err;
  }
  else
  {
    Controler->Integrate+=Controler->Ki*Controler->Err;
  }
/*******积分限幅*********************/
 if(Controler->Integrate_Limit_Flag==1)//积分限制幅度标志
 {
  if(Controler->Integrate>=Controler->Integrate_Max)
    Controler->Integrate=Controler->Integrate_Max;
  if(Controler->Integrate<=-Controler->Integrate_Max)
    Controler->Integrate=-Controler->Integrate_Max ;
 }
/*******总输出计算*********************/
  Controler->Last_Control_OutPut=Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut=Controler->Kp*Controler->Err//比例
                         +Controler->Integrate//积分
                         //+Controler->Kd*Controler->Dis_Err;//微分
                         +Controler->Kd*Controler->Dis_Error_History[0];//微分项来源于巴特沃斯低通滤波器
/*******总输出限幅*********************/
  if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
  Controler->Control_OutPut=Controler->Control_OutPut_Limit;
  if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
  Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
/*******返回总输出*********************/
  return Controler->Control_OutPut;
}


void  PID_Integrate_Reset(PID_Controler *Controler)  {Controler->Integrate=0.0f;}


void Take_Off_Reset(void)
{
  PID_Integrate_Reset(&Total_Controler.Roll_Gyro_Control);//起飞前屏蔽积分
  PID_Integrate_Reset(&Total_Controler.Pitch_Gyro_Control);
  PID_Integrate_Reset(&Total_Controler.Yaw_Gyro_Control);
  PID_Integrate_Reset(&Total_Controler.Pitch_Angle_Control);
  PID_Integrate_Reset(&Total_Controler.Roll_Angle_Control);
  PID_Integrate_Reset(&Total_Controler.Yaw_Angle_Control);
}


