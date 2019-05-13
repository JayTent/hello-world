#include "IIC.h"
#include "stm32f10x.h"
#include "IO.h"
#include "DWT.h"

//��ʼ��IIC
void IIC_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
//����IIC��ʼ�ź�
void IIC_Start(void){
		SDA_OUT;
		IIC_SDA=1;
		IIC_SCL=1;
		Delayus(4);
		IIC_SDA=0;//START:when CLK is high,DATA change form high to low
		Delayus(4);
		IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void){
		SDA_OUT;
		IIC_SCL=0;
		IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		Delayus(4);
		IIC_SCL=1;
		IIC_SDA=1;//����I2C���߽����ź�
		Delayus(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
unsigned char IIC_Wait_Ack(void){
		unsigned char ucErrTime=0;
		SDA_IN;
		IIC_SDA=1;Delayus(1);
		IIC_SCL=1;Delayus(1);
		while(READ_SDA){
			ucErrTime++;
			if(ucErrTime>250){
				IIC_Stop();
				return 1;
			}
		}
		IIC_SCL=0;//ʱ�����0
		return 0;
}
//����ACKӦ��
void IIC_Ack(void){
		IIC_SCL=0;
		SDA_OUT;
		IIC_SDA=0;
		Delayus(2);
		IIC_SCL=1;
		Delayus(2);
		IIC_SCL=0;
}
//������ACKӦ��
void IIC_NAck(void){
		IIC_SCL=0;
		SDA_OUT;
		IIC_SDA=1;
		Delayus(2);
		IIC_SCL=1;
		Delayus(2);
		IIC_SCL=0;
}
//IIC����һ���ֽ�
//txd:��Ҫ���͵�����
void IIC_Send_Byte(unsigned char txd){
    unsigned char t;
	  SDA_OUT;
    IIC_SCL=0;
    for(t=0;t<8;t++){
			IIC_SDA=(txd&0x80)>>7;
			txd<<=1;
			Delayus(2);
			IIC_SCL=1;
			Delayus(2);
			IIC_SCL=0;
			Delayus(2);
    }
}
//��1���ֽ�
//����ֵ:����������
unsigned char IIC_Read_Byte(void){
		unsigned char i, receive=0;
		SDA_IN;
		for(i=0;i<8;i++){
			IIC_SCL=0;
			Delayus(2);
			IIC_SCL=1;
			receive<<=1;
			if(READ_SDA) receive++;
			Delayus(1);
		}
		return receive;
}
