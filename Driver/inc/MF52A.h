#ifndef __MF52A_H
#define __MF52A_H

#define MF52A_VOL   3300//MF52A1103F3950供电电压

/*
*********************************************************************************************************
*	函 数 名: AD_Value_To_Temperature
*	功能说明: 折半法查表将AD值转换为温度值，精确到小数点后一位
*	形    参：AD_Value
*	返 回 值: Temp
*********************************************************************************************************
*/
unsigned short int AD_Value_To_Temperature(unsigned short int AD_Value);
#endif
