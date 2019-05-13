/********************************************************************************
 Author : jaytent (jaytent0704@gmail.com)

 Date : October 2018

 File name : AD7171.c

 Description : 该可执行文件定义了所有操作IC:AD7171的函数.

 Hardware Platform : STM32F103 + AD7171
********************************************************************************/
#include "AD7171.h"
/*
*********************************************************************************************************
*	函 数 名: InitAD7171
*	功能说明: 初始化AD7171
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitAD7171(void)
{
		AD7171_PDRST = 0;
		Delayus(1);
		AD7171_PDRST = 1;
		Delayms(1);
}
/*
*********************************************************************************************************
*	函 数 名: ErrorCheck
*	功能说明: 校验状态位
*	形    参：Status：状态位
*	返 回 值: 错误类型
*********************************************************************************************************
*/
static unsigned char ErrorCheck(unsigned char Status)
{
		if(Status != 0x0D){
			InitAD7171();
			if((Status & 0x20)==0x20) return OutOfRange;
			if((Status & 0x18)!=0x08)	return IDError;
			if((Status & 0x07)!=0x05)	return TransmitError;
		}
		return Normal;
}
/*
*********************************************************************************************************
*	函 数 名: ReadFromAD7171
*	功能说明: 从AD7171读取AD值并转换为电压值
*	形    参：无
*	返 回 值: VINx：输入电压值
*********************************************************************************************************
*/
int ReadFromAD7171(void)
{
		int VINx;
		unsigned short int AD_Result;
		unsigned char data_buf[3];
		unsigned char i,j,Check;
		Delayms(8);
		while(AD7171_DOUTRDY);
		for(i=0;i<3;i++){
			for(j=0;j<8;j++){
				data_buf[i] <<= 1;
				AD7171_SCLK = 0;
				Delayus(1);
				AD7171_SCLK = 1;
				if(AD7171_DOUTRDY) data_buf[i]++;
				Delayus(1);
			}
		}
		AD_Result = (unsigned short int)data_buf[0]<<8 | data_buf[1];
		VINx = (int)((AD_Result*VREF)>>15)-VREF;
		Check = ErrorCheck(data_buf[2]);
		if(Check){
			switch(Check){
				case OutOfRange:
					VINx = VREF;
					break;
				case IDError:
					VINx = 0;
					break;
				case TransmitError:
					VINx = 0;;
					break;
				default:
				  break;
			}
		}
		while(AD7171_DOUTRDY != 1);
		return VINx;
}
