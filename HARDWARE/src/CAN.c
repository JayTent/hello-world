#include "CAN.h"
#include "stm32f10x.h"
#include "RTL.h"
#include "config.h"
#include "communicate.h"

#ifdef COM_CAN
extern unsigned int CANRxMailBox;

CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
void InitCAN1(void){
		GPIO_InitTypeDef 				GPIO_InitStructure;
		CAN_InitTypeDef        	CAN_InitStructure;
		NVIC_InitTypeDef  			NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ
		CAN_InitStructure.CAN_ABOM=ENABLE;	//Ӳ���Զ����߹���
		CAN_InitStructure.CAN_AWUM=DISABLE;	//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
		CAN_InitStructure.CAN_NART=DISABLE;	//��ֹ�����Զ��ش�
		CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�
		CAN_InitStructure.CAN_TXFP=DISABLE;	//����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ��
		CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;
/*
		CAN ������ = RCC_APB1Periph_CAN / Prescaler / (SJW + BS1 + BS2);
		
		SJW = synchronisation_jump_width
		BS = bit_segment
		
		�����У�����CAN������Ϊ470Kbps
		CAN ������ = 36 / 5 / (1 + 8 + 7) = 450Kbps
*/
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
		CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~ CAN_BS1_16tq
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~ CAN_BS2_8tq
		CAN_InitStructure.CAN_Prescaler=5;//��Ƶϵ��(Fdiv)Ϊbrp+1
		CAN_Init(CAN1, &CAN_InitStructure); //��ʼ��CAN1
		CAN_FilterInitStructure.CAN_FilterNumber=0;//������0
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//����λģʽ
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32λ��
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;//��������16λÿλ����ƥ��
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//��������16λÿλ����ƥ��
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0
		CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_Init(&NVIC_InitStructure);
		CAN_ITConfig(CAN1, CAN_IT_FMP0|CAN_IT_TME, ENABLE);//FIFO0��Ϣ�Һ��ж�����
// 		CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);//������ж�ʧ��
}

void CAN_ID_Reset(unsigned char ID){
		CAN_FilterInitStructure.CAN_FilterIdHigh=ID<<10;
		CAN_FilterInitStructure.CAN_FilterIdLow=CAN_ID_STD;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x3C1F;//��������16λÿλ��ID5~ID8����ƥ��
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFD;//��������16λRTR����ƥ��
		CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
}

CanRxMsg RxMessage;
void USB_LP_CAN1_RX0_IRQHandler(void){
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		if(isr_mbx_check(&CANRxMailBox)!=0){
			isr_mbx_send(&CANRxMailBox, &RxMessage);
		}
}

void USB_HP_CAN1_TX_IRQHandler(void){
		if(CAN_GetITStatus(CAN1, CAN_IT_TME)){
			isr_evt_set(EVT_CANTX_EN, CANTxTaskHandle);
			CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
		}
}
#endif
