#ifndef __IIC_H
#define __IIC_H
//IO方向设置
#define SDA_IN  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO操作函数
#define IIC_SCL    PBout(10)//SCL
#define IIC_SDA    PBout(11)//SDA
#define READ_SDA   PBin(11) //输入SDA
#define Delayus    DWT_DelayUs

//IIC所有操作函数
void IIC_Init(void);//初始化IIC的IO口
void IIC_Start(void);//产生IIC起始信号
void IIC_Stop(void);//产生IIC停止信号
unsigned char IIC_Wait_Ack(void);//等待应答信号到来
void IIC_Ack(void);//产生ACK应答
void IIC_NAck(void);//不产生ACK应答
void IIC_Send_Byte(unsigned char txd);//IIC发送一个字节
unsigned char IIC_Read_Byte(void);//读1个字节
#endif
