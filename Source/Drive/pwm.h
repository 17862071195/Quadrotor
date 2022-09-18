#ifndef __PWM_H
#define __PWM_H
#include "headfile.h"

void PWM_GPIO_Init(void);
void PWM_Init(void);
void PWM2_Init(void);
void PWM_Set(const uint16_t pwm1, const uint16_t pwm2, const uint16_t pwm3, const uint16_t pwm4);
#endif
