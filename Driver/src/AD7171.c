/********************************************************************************
 Author : jaytent (jaytent0704@gmail.com)

 Date : October 2018

 File name : AD7171.c

 Description : �ÿ�ִ���ļ����������в���IC:AD7171�ĺ���.

 Hardware Platform : STM32F103 + AD7171
********************************************************************************/
#include "AD7171.h"
/*
*********************************************************************************************************
*	�� �� ��: InitAD7171
*	����˵��: ��ʼ��AD7171
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: ErrorCheck
*	����˵��: У��״̬λ
*	��    �Σ�Status��״̬λ
*	�� �� ֵ: ��������
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
*	�� �� ��: ReadFromAD7171
*	����˵��: ��AD7171��ȡADֵ��ת��Ϊ��ѹֵ
*	��    �Σ���
*	�� �� ֵ: VINx�������ѹֵ
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
