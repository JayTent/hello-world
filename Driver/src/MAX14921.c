/********************************************************************************
 Author : jaytent (jaytent0704@gmail.com)

 Date : October 2018

 File name : MAX14921.c

 Description : 该可执行文件定义了所有操作IC:MAX14921的函数.

 Hardware Platform : STM32F103 + MAX14921
********************************************************************************/
#include "MAX14921.h"
static unsigned int SPIControlBits;//SPI配置寄存器
/*
*********************************************************************************************************
*	函 数 名: SPI_WR_MAX14921
*	功能说明: MAX14921读写
*	形    参：send_data：SPI配置字
*	返 回 值: receive_data：SPI状态字
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
*	函 数 名: Sampling_And_Hold
*	功能说明: MAX14921采样电容采样并保持
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: AD_CHOOSE_BAT_MAX14921
*	功能说明: MAX14921选择电池进行AD采样
*	形    参：bat_num：电池序号
*	返 回 值: 无
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
*	函 数 名: Read_Total_VOL
*	功能说明: 读取整组电压
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Read_Total_VOL(void){
		SPIControlBits &= 0X00E0FFFF;
		SPIControlBits |= 0X00180000;//[ECS、SC0、SC1、SC2、SC3]=[0、0、0、1、1]
		SPI_WR_MAX14921(SPIControlBits);
}
/*
*********************************************************************************************************
*	函 数 名: AD_Choose_Tin
*	功能说明: MAX14921选择T0\1\2三个模拟量进入AOOUT引脚
*	形    参：Tin_num：引脚序号
*	返 回 值: 无
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
*	函 数 名: Parasitic_Cap_Calibrate
*	功能说明: 寄生电容误差校准
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Parasitic_Cap_Calibrate(void){
		unsigned int CommandReg;
		MAX14921_SAMPL=1;
		CommandReg = SPIControlBits;
		SPIControlBits &= 0X00000000;
		SPI_WR_MAX14921(SPIControlBits);
		Delayms(400);//等待10倍采样周期
		MAX14921_SAMPL=0;
		SPIControlBits = CommandReg;
}
/*
*********************************************************************************************************
*	函 数 名: Self_Calibration
*	功能说明: 缓冲器放大失调校准，需要8ms
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: MAX14921CellBalancing
*	功能说明: MAX14921电池均衡
*	形    参：CellNum：电池序号，NewState：开关状态
*	返 回 值: 无
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
*	函 数 名: OpenWireDetect
*	功能说明: 开路检测，需要500ms
*	形    参：无
*	返 回 值: ResponseReg：开路检测结果
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
