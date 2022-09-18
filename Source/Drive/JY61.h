#ifndef __JY61_H
#define __JY61_H
#include "JY901.h"


extern struct STime		stcTime;
extern struct SAcc 		stcAcc;
extern struct SGyro 		stcGyro;
extern struct SAngle 	stcAngle;
extern struct SMag 		stcMag;
extern struct SDStatus stcDStatus;
extern struct SPress 	stcPress;
extern struct SLonLat 	stcLonLat;
extern struct SGPSV 		stcGPSV;

void CopeSerial2Data(unsigned char ucData);
void Initial_UART2(unsigned long baudrate);
void UART2_Put_Char(unsigned char DataToSend);
void UART2_Put_String(unsigned char *Str);
#endif

//------------------End of File----------------------------

