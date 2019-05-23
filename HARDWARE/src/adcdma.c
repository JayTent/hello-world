#include "adc.h"

extern unsigned char Send_OK;
volatile unsigned int AD_Value[8][3];

void  ADCInit(void){
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
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)AD_Value;//ÄÚ´æµØÖ·
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//ÍâÉèµ½´æ´¢Æ÷
  DMA_InitStructure.DMA_BufferSize = 60;//Êý¾ÝÁ¿
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//ÍâÉè·ÇÔöÁ¿Ä£Ê½
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//´æ´¢Æ÷ÔöÁ¿Ä£Ê½
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//ÍâÉèÊý¾Ý³¤¶È°ë×Ö
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//´æ´¢Æ÷Êý¾Ý³¤¶È°ë×Ö
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//Ñ­»·Ä£Ê½
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//´æ´¢Æ÷Í»·¢µ¥´Î´«Êä
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//ÍâÉèÍ»·¢µ¥´Î´«Êä
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
	
	/* Configure ADC1 */
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;//ÈýÖØ¹æÔòÍ¬²½Ä£Ê½
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//¶àÖØÄ£Ê½ÏÂÁ½¸ö²ÉÑù½×¶ÎÖ®¼äµÄÑÓ³Ù5¸öÊ±ÖÓ
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//DMAÄ£Ê½1
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//2·ÖÆµ¡£ADCCLK=PCLK2/4=72/2=36Mhz,ADCÊ±ÖÓ×îºÃ²»Òª³¬¹ý36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//½ûÖ¹´¥·¢¼ì²â£¬Ê¹ÓÃÈí¼þ´¥·¢
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ÓÒ¶ÔÆë
  ADC_InitStructure.ADC_NbrOfConversion = 2;//2¸ö×ª»»ÔÚ¹æÔòÐòÁÐÖÐ
	
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);//²ÉÑùÊ±¼äÌ«¶Ì£¬É¨ÃèÍ¨µÀ¼ä»á·¢ÉúÐÅºÅñîºÏ´Ó¶øÓ°ÏìÏÂÒ»ADÐòÁÐ
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
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  DMA_ClearFlag(DMA2_Stream0, DMA_IT_TC);
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}


void DMA2_Stream0_IRQHandler(void){
  if(DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET){
    Send_OK=1;
    DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0);
  }
}
