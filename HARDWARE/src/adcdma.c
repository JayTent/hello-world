#include "adc.h"
#include "delay.h"

extern unsigned char Send_OK;
volatile unsigned short int AD_Value[16][5];

void  Adc_Init(void){
  GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef  			DMA_InitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	
	/* ENABLE Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
  /* Configure PA.0\1\2\3\4  as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure ADC1 */
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);//��λ����
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//����ģʽ�����������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;//DMAģʽ2
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//2��Ƶ��ADCCLK=PCLK2/4=72/2=36Mhz,ADCʱ����ò�Ҫ����36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
  ADC_InitStructure.ADC_NbrOfConversion = 5;//5��ת���ڹ���������
  ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_3Cycles);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//����ADת����
	
	DMA_DeInit(DMA2_Stream0);
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//�ȴ�DMA������
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)AD_Value;//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��
  DMA_InitStructure.DMA_BufferSize = 80;//������
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ��Ȱ���
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ��Ȱ���
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_ClearFlag(DMA2_Stream0, DMA_IT_TC);
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream0, ENABLE);
}


void DMA2_Stream0_IRQHandler(void) 
{
	if(DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET){
		Send_OK=1;
		DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0);
	}
}
