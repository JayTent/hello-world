#include "IIC.h"
#include "stm32f10x.h"
#include "IO.h"
#include "DWT.h"

//初始化IIC
void IIC_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
//产生IIC起始信号
void IIC_Start(void){
		SDA_OUT;
		IIC_SDA=1;
		IIC_SCL=1;
		Delayus(4);
		IIC_SDA=0;//START:when CLK is high,DATA change form high to low
		Delayus(4);
		IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void){
		SDA_OUT;
		IIC_SCL=0;
		IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		Delayus(4);
		IIC_SCL=1;
		IIC_SDA=1;//发送I2C总线结束信号
		Delayus(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
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
		IIC_SCL=0;//时钟输出0
		return 0;
}
//产生ACK应答
void IIC_Ack(void){
		IIC_SCL=0;
		SDA_OUT;
		IIC_SDA=0;
		Delayus(2);
		IIC_SCL=1;
		Delayus(2);
		IIC_SCL=0;
}
//不产生ACK应答
void IIC_NAck(void){
		IIC_SCL=0;
		SDA_OUT;
		IIC_SDA=1;
		Delayus(2);
		IIC_SCL=1;
		Delayus(2);
		IIC_SCL=0;
}
//IIC发送一个字节
//txd:将要发送的内容
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
//读1个字节
//返回值:读到的数据
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
