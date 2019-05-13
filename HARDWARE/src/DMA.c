#include "DMA.h"
#include "stm32f10x.h"
#include "RTL.h"
#include "config.h"
#include "communicate.h"

void InitDMA1(void){
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC->AHBENR |= 1<<0;//����DMA1ʱ��
		DMA1_Channel4->CPAR=(u32)&USART1->DR;//�����ַ
		DMA1_Channel4->CMAR=0;//�ڴ��ַ��ʱ����
		DMA1_Channel4->CNDTR=0;//��������ʱ����
		DMA1_Channel4->CCR=0X00000000;//��λ
		DMA1_Channel4->CCR|=1<<4;//���ڵ���
		DMA1_Channel4->CCR&=~(1<<6);//�����ַ����
		DMA1_Channel4->CCR|=1<<7;//�ڴ��ַ����
		DMA1_Channel4->CCR&=~(1<<8);//�������ݿ��Byte
		DMA1_Channel4->CCR&=~(1<<10);//�ڴ����ݿ��Byte
		DMA1_Channel4->CCR&=~(1<<5);//��ͨģʽ
		DMA1_Channel4->CCR|=1<<12;//�е����ȼ�
		DMA1_Channel4->CCR&=~(1<<14);//��ֹ�ڴ浽�ڴ洫��
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		DMA1_Channel4->CCR |= 1<<1;//DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
		USART1->CR3=1<<7;//USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

void Usart1DMATxData(unsigned char* menAdrr, unsigned short int DataLength){
		DMA1_Channel4->CMAR=(unsigned int)menAdrr;//���¶����ڴ��ַ
		DMA1_Channel4->CNDTR=DataLength;//���¶���������
		DMA1_Channel4->CCR|=1<<0;//DMA_Cmd(DMA_CHx, ENABLE);
}

#ifdef COM_485
void DMA1_Channel4_IRQHandler(void){
		if(DMA_GetITStatus(DMA1_IT_TC4) != RESET){
			isr_evt_set(EVT_485TX_EN, RS485TxTaskHandle);
			DMA1_Channel4->CCR&=~(1<<0);//DMA_Cmd(DMA_CHx, DISABLE);
			DMA_ClearITPendingBit(DMA1_IT_TC4);
		}
}
#endif
