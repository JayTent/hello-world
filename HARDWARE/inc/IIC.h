#ifndef __IIC_H
#define __IIC_H
//IO��������
#define SDA_IN  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO��������
#define IIC_SCL    PBout(10)//SCL
#define IIC_SDA    PBout(11)//SDA
#define READ_SDA   PBin(11) //����SDA
#define Delayus    DWT_DelayUs

//IIC���в�������
void IIC_Init(void);//��ʼ��IIC��IO��
void IIC_Start(void);//����IIC��ʼ�ź�
void IIC_Stop(void);//����IICֹͣ�ź�
unsigned char IIC_Wait_Ack(void);//�ȴ�Ӧ���źŵ���
void IIC_Ack(void);//����ACKӦ��
void IIC_NAck(void);//������ACKӦ��
void IIC_Send_Byte(unsigned char txd);//IIC����һ���ֽ�
unsigned char IIC_Read_Byte(void);//��1���ֽ�
#endif
