#include "adc.h"

extern unsigned char Send_OK;
volatile unsigned short int AD_Value[16][5];

void  Adc_Init(void){
  GPIO_InitTypeDef  		GPIO_InitStructure;
	DMA_InitTypeDef  			DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	
	/* ENABLE Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2|RCC_APB2Periph_ADC3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
  /* Configure PA.0\1\2\3\4  as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	  //|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;//通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)AD_Value;//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到内存
  DMA_InitStructure.DMA_BufferSize = 24;//数据量
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度半字
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度半字
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	/* Configure ADC1 */
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_Interl;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//多重模式下两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//DMA模式1
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//2分频。ADCCLK=PCLK2/4=72/2=36Mhz,ADC时钟最好不要超过36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中
  ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Init(ADC2, &ADC_InitStructure);
	ADC_Init(ADC3, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_3Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_3Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_3Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器
	ADC_Cmd(ADC3, ENABLE);//开启AD转换器
	
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_ClearFlag(DMA2_Stream0, DMA_IT_TC);
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}


void DMA2_Stream0_IRQHandler(void) 
{
	if(DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET){
		Send_OK=1;
		DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0);
	}
}
