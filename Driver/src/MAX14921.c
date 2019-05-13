/********************************************************************************
 Author : jaytent (jaytent0704@gmail.com)

 Date : October 2018

 File name : MAX14921.c

 Description : �ÿ�ִ���ļ����������в���IC:MAX14921�ĺ���.

 Hardware Platform : STM32F103 + MAX14921
********************************************************************************/
#include "MAX14921.h"
static unsigned int SPIControlBits;//SPI���üĴ���
/*
*********************************************************************************************************
*	�� �� ��: SPI_WR_MAX14921
*	����˵��: MAX14921��д
*	��    �Σ�send_data��SPI������
*	�� �� ֵ: receive_data��SPI״̬��
*********************************************************************************************************
*/
unsigned int SPI_WR_MAX14921(unsigned int send_data){
		unsigned char i;
		unsigned char send_databuf[3];
		unsigned char receive_databuf[3];
		unsigned int receive_data=0;
		send_databuf[0] = send_data&0x000000ff;
		send_databuf[1] = (send_data&0x0000ff00)>>8;
		send_databuf[2] = (send_data&0x00ff0000)>>16;
		MAX14921_CS=0;
		for(i=0;i<3;i++){
			receive_databuf[i] = MAX14921ReadWriteByte(send_databuf[i]);
		}
		MAX14921_CS=1;
		receive_data = receive_databuf[0]+(receive_databuf[1]<<8)+(receive_databuf[2]<<16);
		return receive_data;
}
/*
*********************************************************************************************************
*	�� �� ��: Sampling_And_Hold
*	����˵��: MAX14921�������ݲ���������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Sampling_And_Hold(void){
		MAX14921_SAMPL=1;
		Delayms(4);//4ms,40ms
		MAX14921_SAMPL=0;
		Delayus(50);
}
/*
*********************************************************************************************************
*	�� �� ��: AD_CHOOSE_BAT_MAX14921
*	����˵��: MAX14921ѡ���ؽ���AD����
*	��    �Σ�bat_num��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD_CHOOSE_BAT_MAX14921(unsigned char bat_num){
		if(bat_num<16){
			SPIControlBits &= 0X00E0FFFF;
			switch(bat_num){
				case 0:
						SPIControlBits |= 0X00010000;
				break;
				case 1:
						SPIControlBits |= 0X00030000;
				break;
				case 2:
						SPIControlBits |= 0X00050000;
				break;
				case 3:
						SPIControlBits |= 0X00070000;
				break;
				case 4:
						SPIControlBits |= 0X00090000;
				break;
				case 5:
						SPIControlBits |= 0X000B0000;
				break;
				case 6:
						SPIControlBits |= 0X000D0000;
				break;
				case 7:
						SPIControlBits |= 0X000F0000;
				break;
				case 8:
						SPIControlBits |= 0X00110000;
				break;
				case 9:
						SPIControlBits |= 0X00130000;
				break;
				case 10:
						SPIControlBits |= 0X00150000;
				break;
				case 11:
						SPIControlBits |= 0X00170000;
				break;
				case 12:
						SPIControlBits |= 0X00190000;
				break;
				case 13:
						SPIControlBits |= 0X001B0000;
				break;
				case 14:
						SPIControlBits |= 0X001D0000;
				break;
				case 15:
						SPIControlBits |= 0X001F0000;
				break;
				default:
				break;
			}
			SPI_WR_MAX14921(SPIControlBits);
			Delayus(10);
		}
}
/*
*********************************************************************************************************
*	�� �� ��: Read_Total_VOL
*	����˵��: ��ȡ�����ѹ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Read_Total_VOL(void){
		SPIControlBits &= 0X00E0FFFF;
		SPIControlBits |= 0X00180000;//[ECS��SC0��SC1��SC2��SC3]=[0��0��0��1��1]
		SPI_WR_MAX14921(SPIControlBits);
}
/*
*********************************************************************************************************
*	�� �� ��: AD_Choose_Tin
*	����˵��: MAX14921ѡ��T0\1\2����ģ��������AOOUT����
*	��    �Σ�Tin_num���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD_Choose_Tin(unsigned char Tin_num){
		if(Tin_num<3){
			SPIControlBits &= 0X00E0FFFF;
			switch(Tin_num){
				case 0:
						SPIControlBits |= 0X001A0000;
				break;
				case 1:
						SPIControlBits |= 0X001C0000;
				break;
				case 2:
						SPIControlBits |= 0X001E0000;
				break;
				default:
				break;
			}
			SPI_WR_MAX14921(SPIControlBits);
			Delayus(3);
		}
}
/*
*********************************************************************************************************
*	�� �� ��: Parasitic_Cap_Calibrate
*	����˵��: �����������У׼
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Parasitic_Cap_Calibrate(void){
		unsigned int CommandReg;
		MAX14921_SAMPL=1;
		CommandReg = SPIControlBits;
		SPIControlBits &= 0X00000000;
		SPI_WR_MAX14921(SPIControlBits);
		Delayms(400);//�ȴ�10����������
		MAX14921_SAMPL=0;
		SPIControlBits = CommandReg;
}
/*
*********************************************************************************************************
*	�� �� ��: Self_Calibration
*	����˵��: �������Ŵ�ʧ��У׼����Ҫ8ms
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Self_Calibration(void){
		unsigned int CommandReg;
		MAX14921_SAMPL=1;
		CommandReg = SPIControlBits;
		SPIControlBits = 0X00020000;
		SPI_WR_MAX14921(SPIControlBits);
		Delayms(8);
		MAX14921_SAMPL=0;
		SPIControlBits = CommandReg;
}
/*
*********************************************************************************************************
*	�� �� ��: MAX14921CellBalancing
*	����˵��: MAX14921��ؾ���
*	��    �Σ�CellNum�������ţ�NewState������״̬
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MAX14921CellBalancing(unsigned char CellNum, SwitchState NewState){
		if(CellNum<16){
			if(NewState) SPIControlBits |= 1<<(15-CellNum);
			else SPIControlBits &= ~(1<<(15-CellNum));
			SPI_WR_MAX14921(SPIControlBits);
		}
}
/*
*********************************************************************************************************
*	�� �� ��: OpenWireDetect
*	����˵��: ��·��⣬��Ҫ500ms
*	��    �Σ���
*	�� �� ֵ: ResponseReg����·�����
*********************************************************************************************************
*/
unsigned int OpenWireDetect(void){
		unsigned int CommandReg,ResponseReg;
		MAX14921_SAMPL=1;
		CommandReg = SPIControlBits;
		SPIControlBits = 0X00410000;
		SPI_WR_MAX14921(SPIControlBits);
		Delayms(500);
		MAX14921_SAMPL=0;
		SPIControlBits = 0X00010000;
		ResponseReg=SPI_WR_MAX14921(SPIControlBits);
		SPIControlBits = CommandReg;
		return ResponseReg;
}
