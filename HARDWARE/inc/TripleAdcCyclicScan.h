#ifndef __TRIPLEADCCYCLICSCAN_H
#define	__TRIPLEADCCYCLICSCAN_H
/********************************************************************************
 Author : jaytent (jaytent0704@gmail.com)

 Date : May 2019

 File name : TripleAdcCyclicScan.h

 Description : 该头文件声明了所有TripleAdcCyclicScan.c文件的函数、宏定义以及类型定义。

 Hardware Platform : STM32F407
********************************************************************************/
#define ADC_CDR_ADDRESS   ((uint32_t)0x40012308)

extern volatile unsigned int AD_Value[10][6];//AD采样寄存器

void AdcInit(void);
void AdcStart(void);
void DMA2_Stream0_IRQHandler(void);
#endif
