#ifndef _PPM_H_

#define _PPM_H_
#include "headfile.h"

extern unsigned short int Throttle_Control;
extern signed short int Pitch_Control,Roll_Control,Yaw_Control;
extern signed short int Target_Angle[2];
void TIM4_Cap_Init(u16 arr, u16 psc);
void TIM1_Cap_Init(u16 arr, u16 psc);
void PPM_Mapping(void);

#endif
