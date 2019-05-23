/********************************************************************************
 Author : jaytent (jaytent0704@gmail.com)

 Date : May 2019

 File name : TripleAdcCyclicScan.c

 Description :¸该可执行文件包含了STM32F4三重ADC循环扫描的所有配置。

 Hardware Platform : STM32F407
********************************************************************************/
#include "TripleAdcCyclicScan.h"
#include "stm32f4xx.h"

volatile unsigned int AD_Value[10][6];//AD采样寄存器

void AdcInit(void){
  GPIO_InitTypeDef       GPIO_InitStructure;
  DMA_InitTypeDef        DMA_InitStructure;
  ADC_CommonInitTypeDef  ADC_CommonInitStructure;
  ADC_InitTypeDef        ADC_InitStructure;
  NVIC_InitTypeDef       NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)ADC_CDR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)AD_Value;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 60;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 2;
  
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_56Cycles);
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 2, ADC_SampleTime_56Cycles);
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_1, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_2, 2, ADC_SampleTime_56Cycles);
  
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  DMA_ClearFlag(DMA2_Stream0, DMA_IT_TC);
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}

void AdcStart(void){
  ADC_SoftwareStartConv(RHEOSTAT_ADC1);
}

void DMA2_Stream0_IRQHandler(void){
  if(DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET){
    ;//事件标志或者信号量或者用户自定义标志位
    DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0);
  }
}
